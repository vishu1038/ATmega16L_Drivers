/*
 * uart_driver.h
 *
 * Created: 28-11-2024 12:45:47
 *  Author: Vishal Agarwal
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "sys_cnfg.h"

typedef struct __attribute__((packed)) uart_config_tst_tag
{
	U8 baud_b3				:3;
	U8 int_en_b2			:2;
	U8 usart_mode_b1		:1;
	U8 double_stopbit_b1	:1;
	U8 double_speed_b1		:1;

	U8	parity_type_b2		:2;
	U8 tx_rx_en_b2			:2;
	U8 data_length_b4		:4;
	
}uart_config_tst;

typedef struct __attribute__((packed)) uart_data_tst_tag
{
	U16 tx_data_b9	:9;
	U16 rx_data_b9	:9;
	
	U8 rx_rcvd_b1	:1;
	U8 reserved_b6	:5;
}uart_data_tst;

enum uart_baud_cnfg_e_tag
{
	UART_BAUD_9600 = 0,
	UART_BAUD_19200,
	UART_BAUD_115200,
};

enum uart_mode_cnfg_e_tag
{
	UART_MODE = 0,
	USART_MODE,
};

enum uart_stopbit_cnfg_e_tag
{
	UART_STOPBIT_SINGLE = 0,
	UART_STOPBIT_DOUBLE,
};

enum uart_parity_cnfg_e_tag
{
	UART_PARITY_NONE = 0,
	UART_PARITY_EVEN,
	UART_PARITY_ODD,
};

enum uart_tx_rx_cnfg_e_tag
{
	UART_NONE_EN = 0,
	UART_TX_EN,
	UART_RX_EN,
	UART_TX_RX_EN,
};

enum uart_int_cnfg_e_tag
{
	UART_INT_NONE = 0,
	UART_INT_TXEN,
	UART_INT_RXEN,
	UART_INT_TXRX,
};

enum uart_speed_cnfg_e_tag
{
	UART_SPEED_NORMAL = 0,
	UART_SPEED_DOUBLE,
};

extern uart_data_tst uart_data_st;

U8 uart_init();
U8 uart_transmit(U16 data_u16);
#endif /* UART_DRIVER_H_ */
	
 