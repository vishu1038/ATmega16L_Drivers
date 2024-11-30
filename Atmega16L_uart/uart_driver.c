/*
 * uart_driver.c
 *
 * Created: 28-11-2024 12:45:47
 *  Author: Vishal Agarwal
 */ 

#include "uart_driver.h"
#include "sys_cnfg.h"

#define BAUD_UBRR(baud) ((uint16_t)((F_CPU/16UL/baud) - 1))

uart_config_tst uart_config_st = {
	.baud_b3 = UART_BAUD_9600,
	.int_en_b2 = UART_INT_RXEN,
	.usart_mode_b1 = UART_MODE,
	.double_stopbit_b1 = UART_STOPBIT_DOUBLE,
	.double_speed_b1 = UART_SPEED_NORMAL,

	.parity_type_b2 = UART_PARITY_ODD,
	.tx_rx_en_b2 = UART_TX_RX_EN,
	.data_length_b4 = 8,
};

uart_data_tst uart_data_st;

/* UART INIT FUNCTION */
U8 uart_init()
{
	U8 ret_status_u8 = 0;
	
	/* Configure Baud rate */
	U16 baud_u16 = 0;	  
	switch(uart_config_st.baud_b3)
	{
		case UART_BAUD_9600:
		{
			baud_u16 = BAUD_UBRR(9600);
		}
		break;
		case UART_BAUD_19200:
		{
			baud_u16 = BAUD_UBRR(19200);
		}
		break;
		case UART_BAUD_115200:
		{
			baud_u16 = BAUD_UBRR(115200);
		}
		break;
		default:
			return 1; //incorrect baud rate configuration
		break;
	}

	U8 ucsrc = (U8)(baud_u16 >> 8); //Defined this variable as UBRRH and UCSRC have same memory address and need to be written in this manner.
	UBRRH = ucsrc; 
	UBRRL = (U8)(baud_u16 & 0xFF);

	/* Configure UART or USART */
	//UCSRC = (1 << URSEL) | (uart_config_st.usart_mode_b1 << UMSEL); //Writing directly to UCSRC Register even with URSEL == 1	doesn't seem to work
	ucsrc = (1 << URSEL) | (uart_config_st.usart_mode_b1 << UMSEL);
	
	/* Configure Data Length */
	switch(uart_config_st.data_length_b4)
	{
		case 5:
		{
			UCSRB &= ~(1 << UCSZ2);
			//nothing required to be done in UART control registers
		}
		break;
		case 6:
		{
			UCSRB &= ~(1 << UCSZ2);
			ucsrc |= (1 << URSEL) | (1 << UCSZ0);
		}
		break;
		case 7:
		{
			UCSRB &= ~(1 << UCSZ2);
			ucsrc |= (1 << URSEL) | (1 << UCSZ1);
		}
		break;
		case 8:
		{
			UCSRB &= ~(1 << UCSZ2);
			ucsrc |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
		}
		break;
		case 9:
		{
			UCSRB |= (1 << UCSZ2);
			ucsrc |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
		}
		break;
		default:
			return 1; //data length out of range
		break;
	}

	/* Configure Parity Bit */
	switch(uart_config_st.parity_type_b2)
	{
		case UART_PARITY_NONE:
		{
			//nothing required to be done in UART control registers
		}
		break;
		case UART_PARITY_EVEN:
		{
			ucsrc |= (1 << URSEL) | (1 << UPM1); 
		}
		break;
		case UART_PARITY_ODD:
		{
			ucsrc |= (1 << URSEL) | (1 << UPM1) | (1 << UPM0); 
		}
		break;
		default:
			return 1; //Incorrect parity configuration
		break;
	}
	
	/* Configure Number of Stop Bits */
	ucsrc |= (1 << URSEL) | (uart_config_st.double_stopbit_b1 << USBS);

	/* Enable TX and RX */
	switch(uart_config_st.tx_rx_en_b2)
	{
		case UART_NONE_EN:
		{
			//nothing required to be done in UART control registers
		}
		break;
		case UART_TX_EN:
		{
			UCSRB |= (1 << TXEN);
		}
		break;
		case UART_RX_EN:
		{
			UCSRB |= (1 << RXEN);
		}
		break;
		case UART_TX_RX_EN:
		{
			UCSRB |= (1 << TXEN) | (1 << RXEN);
		}
		break;
		default:
			return 1; //Incorrect TX RX Enable Configuration
		break;
	}

	/* Set UART Interrupts */
	switch(uart_config_st.int_en_b2)
	{
		case UART_INT_NONE:
		{
			//nothing required to be done in UART control registers
		}
		break;
		case UART_INT_TXEN:
		{
			UCSRB |= (1 << TXCIE);
		}
		break;
		case UART_INT_RXEN:
		{
			UCSRB |= (1 << RXCIE);
		}
		break;
		case UART_INT_TXRX:
		{
			UCSRB |= (1 << TXCIE) | (1 << RXCIE);
		}
		break;
		default:
			return 1; //Incorrect Interrupt Configuration
		break;
	}

	/* Configure UART double speed */
	if(uart_config_st.usart_mode_b1 == 0 && uart_config_st.double_speed_b1 == 1)	
	{
		UCSRA |= (uart_config_st.double_speed_b1 << U2X);
	}
	
	/* Configure UCSRC Register - Writing at the end after all UCSRC Reg configs */
	UCSRC = ucsrc;
	
	return ret_status_u8;
}

/* UART RX ISR */
ISR(USART_RXC_vect)
{
	if(uart_config_st.data_length_b4 < 9)
	{
		uart_data_st.rx_data_b9 = 0;
		uart_data_st.rx_data_b9 = UDR;
		uart_data_st.rx_rcvd_b1 = 1;
	}
	else
	{
		uart_data_st.rx_data_b9 = 0;
		uart_data_st.rx_data_b9 = ((UCSRB >> RXB8) & 0x01) << 8;
		uart_data_st.rx_data_b9 |= UDR;
		uart_data_st.rx_rcvd_b1 = 1;
	}
}

/* UART TX on Demand */
U8 uart_transmit(U16 data_u16)
{
	if(data_u16 == 0)
	{
		uart_data_st.tx_data_b9 = data_u16;
		while(!(UCSRA & (1 << UDRE)));
		UDR = uart_data_st.tx_data_b9;
		
		return 0;
	}
	
	while(data_u16 != 0)
	{
		uart_data_st.tx_data_b9 = 0;
		uart_data_st.tx_data_b9 = data_u16;
		data_u16 >>= uart_config_st.data_length_b4;
		while(!(UCSRA & (1 << UDRE)));
		if(uart_config_st.data_length_b4 == 9)
		{
			UCSRB &= ~(1 << TXB8);
			if(uart_data_st.tx_data_b9 & 0x100)
			{
				UCSRB |= (1 << TXB8);	
			}
		}
		UDR = uart_data_st.tx_data_b9;
	}
	
	return 0;
}