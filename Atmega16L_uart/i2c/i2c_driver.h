/*
 * i2c.h
 *
 * Created: 19-04-2025 18:35:34
 *  Author: vishalagarwal
 */ 


#ifndef I2C_H_
#define I2C_H_

#include "sys_cnfg.h"

typedef struct __attribute__((packed)) i2c_config_tst_tag
{
	U8 self_addr_b7			:7;
	U8 reserved_b1			:1;

	U8 freq_b3				:3;
	U8 freq_prescalar_b2	:2;
	U8 int_en_b1			:1;
	U8 ack_en_b1			:1;
	U8 ack_chk_en_b1		:1;
}i2c_config_tst;

enum i2c_freq_cnfg_e_tag
{
	I2C_100KHZ = 1,
	I2C_400KHZ = 4,
};

enum i2c_freq_prescalar_e_tag
{
	FREQ_PRESCALAR_NONE = 0,
	FREQ_PRESCALAR_4 = 1,
	FREQ_PRESCALAR_16 = 2,
	FREQ_PRESCALAR_64 = 3,
};

enum i2c_int_cnfg_e_tag
{
	I2C_INT_DIS = 0,
	I2C_INT_EN,
};

enum i2c_ack_cnfg_e_tag
{
	I2C_ACK_DIS = 0,
	I2C_ACK_EN,
};

enum i2c_ack_chk_cnfg_e_tag
{
	I2C_ACK_CHK_DIS = 0,
	I2C_ACK_CHK_EN,
};

enum i2c_read_write_cnfg_e_tag
{
	I2C_READ = 0,
	I2C_WRITE,
};

U8 i2c_init();
U8 i2c_transmit_frame(U8 slave_addr_u8, U8 read_write_bool, U8* data_u8, U8 data_length_u8);

#endif /* I2C_H_ */