/*
 * Delay.h
 *
 * Created on: 10/2/19
 * Author: Murray_Erickson
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "msp.h"
#include <stdint.h>

void delay_CLK(uint32_t);
void delay_ms(uint32_t, uint32_t);
void delay_us(uint32_t, uint32_t);
uint32_t set_DCO(uint32_t);

#define FREQ_1_5_MHz ((uint32_t)0x00000000)
#define FREQ_3_MHz ((uint32_t)0x00010000)
#define FREQ_6_MHz ((uint32_t)0x00020000)
#define FREQ_12_MHz ((uint32_t)0x00030000)
#define FREQ_24_MHz ((uint32_t)0x00040000)
#define FREQ_48_MHz ((uint32_t)0x00050000)

#endif /* DELAY_H */
