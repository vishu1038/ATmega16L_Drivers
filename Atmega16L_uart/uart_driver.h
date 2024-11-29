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
	uint8_t baud_b3				:3;
	uint8_t int_en_b2			:2;
	uint8_t usart_mode_b1		:1;
	uint8_t double_stopbit_b1	:1;
	uint8_t double_speed_b1		:1;

	uint8_t	parity_type_b2		:2;
	uint8_t tx_rx_en_b2			:2;
	uint8_t data_length_b4		:4;
	
}uart_config_tst;

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
	UART_PARITY_ODD,
	UART_PARITY_EVEN,
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

uint8_t uart_init();

#endif /* UART_DRIVER_H_ */
	
 