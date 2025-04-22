/*
 * sys_cnfg.h
 *
 * Created: 29-11-2024 21:03:14
 *  Author: Vishal Agarwal
 */  

#ifndef SYS_CNFG_H_
#define SYS_CNFG_H_

#define F_CPU 12000000UL
#define TICK_PART (float)1.0/F_CPU

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

typedef float SFP;
typedef uint32_t U32;
typedef uint8_t U8;
typedef uint16_t U16;
typedef int8_t S8;

#endif /* SYS_CNFG_H_ */