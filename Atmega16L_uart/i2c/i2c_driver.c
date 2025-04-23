/*
 * i2c.c
 *
 * Created: 19-04-2025 18:34:52
 *  Author: vishalagarwal
 */ 

#include "i2c_driver.h"

#define SELF_I2C_ADDR (0x69) //self addr, 0x00 is used for taling to everybody.

i2c_config_tst i2c_config_st = {
	.self_addr_b7 = SELF_I2C_ADDR,
	
	.freq_b3 = I2C_100KHZ,
	.freq_prescalar_b2 = FREQ_PRESCALAR_NONE,
	.int_en_b1 = I2C_INT_EN,
	.ack_en_b1 = I2C_ACK_EN,
	.ack_chk_en_b1 = I2C_ACK_CHK_DIS,
};

static U8 twbr_generator()
{
	//created separate logic to calculate power instead of using math lib for optimization
	U8 res_u8 = 1;
	if(i2c_config_st.freq_prescalar_b2 > FREQ_PRESCALAR_NONE)
	{
		for(U8 i = 0; i < i2c_config_st.freq_prescalar_b2; i++) //calc final prescalar value
		{
			res_u8 *= 4;
		}
	}
	
	return (U8)(((F_CPU / (i2c_config_st.freq_b3 * 100000UL)) - 16) / (2 * res_u8)); //formula in data sheet to calculate TWBR register	
}

/* I2C INIT FUNCTION */
U8 i2c_init()
{
	U8 ret_status_u8 = 0;
	
	/* Configure PRESCALAR for TWSR */
	TWSR |= i2c_config_st.freq_prescalar_b2;
	
	/* Configure Bit Rate for I2C */
	if(i2c_config_st.freq_b3 <= 4)
	{
		TWBR = twbr_generator();
	}
	else
	{
		ret_status_u8 = 1; //incorrect freq configuration
	}
	
	/* Set ACK Generation Enable */
	TWCR |= (i2c_config_st.ack_en_b1 << TWEA);
	
	/* Set I2C Interrupts */
	TWCR |= (i2c_config_st.int_en_b1 << TWIE);
	
	/*Enable I2C */
	TWCR |= (1 << TWEN);
	
	return ret_status_u8;
}

/* I2C RX ISR */
ISR(TWI_vect)
{
	//if(uart_config_st.data_length_b4 < 9)
	//{
		//uart_data_st.rx_data_b9 = 0;
		//uart_data_st.rx_data_b9 = UDR;
		//uart_data_st.rx_rcvd_b1 = 1;
	//}
	//else
	//{
		//uart_data_st.rx_data_b9 = 0;
		//uart_data_st.rx_data_b9 = ((UCSRB >> RXB8) & 0x01) << 8;
		//uart_data_st.rx_data_b9 |= UDR;
		//uart_data_st.rx_rcvd_b1 = 1;
	//}
	
	//clear Interrupt flag at end
	TWCR |= (0 << TWINT);
}

static void ack_chk()
{
	switch(i2c_config_st.ack_chk_en_b1)
	{
		case I2C_ACK_CHK_EN:
			while(((TWSR & ~((1 << TWPS0) | (1 << TWPS1))) != 0x18)); //0x18 is ack rcvd
		break;
		default:
			while(((TWSR & ~((1 << TWPS0) | (1 << TWPS1))) != 0x18) ||
				((TWSR & ~((1 << TWPS0) | (1 << TWPS1))) != 0x20)); //0x18 is ack rcvd, 0x20 is nack rcvd
		break;
	}
}

static void start_bit_send()
{
	/* Start bit send */
	TWCR |= (1 << TWSTA);
	
	/* Wait for start bit sent */
	while((TWSR & ~((1 << TWPS0) | (1 << TWPS1))) != 0x08); //0x08 is success status for START bit sent
	
	/* Start bit reset */
	TWCR |= (0 << TWSTA);
}

/* I2C TX on Demand */
U8 i2c_transmit_frame(U8 slave_addr_u8, U8 read_write_bool, U8* data_u8, U8 data_length_u8)
{
	start_bit_send();
	
	/* Set Interrupt Flag to keep data stable and not allow data shifting in hardware */
	TWCR |= (1 << TWINT);
	
	/* Send Slave Address */
	TWDR = slave_addr_u8;
	
	ack_chk();
	
	if(read_write_bool == I2C_WRITE)
	{
		while(data_length_u8 != 0)
		{
			/* Set Interrupt Flag to keep data stable and not allow data shifting in hardware */
			TWCR |= (1 << TWINT);
		
			TWDR = *data_u8;
		
			ack_chk();
		
			data_length_u8--;
		
			if(data_length_u8 != 0)
			{
				data_u8++;
				start_bit_send();
			}
		}
	}
	
	/* Set Interrupt Flag to keep data stable and not allow data shifting in hardware */
	TWCR |= (1 << TWINT);
	
	/* Top bit send */
	TWCR |= (1 << TWSTO);  //cleared automatically by hardware
	
	return 0;
}