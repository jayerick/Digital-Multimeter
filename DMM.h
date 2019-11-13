/*
 * DMM.h
 *
 *  Created on: Nov 4, 2019
 *      Author: jayerickson
 */

#ifndef DMM_H_
#define DMM_H_

static uint8_t ADC_Flag;
static uint16_t ADC_Data;

void DMM_init();
void Terminal_init();
uint16_t DC_Voltage_Reading();
uint16_t AC_Voltage_Reading();
uint16_t Frequency_Reading();
void Terminal_Update_All();

void ADC_Flag_Set();

void Terminal_Update_DC(uint16_t DC, uint16_t DC_Bar, uint8_t all);
void Terminal_Update_AC(uint16_t AC, uint16_t AC_Bar, uint8_t all);


#endif /* DMM_H_ */
