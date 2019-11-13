/*
 * ADC.h
 *
 *  Created on: Nov 4, 2019
 *      Author: jayerickson
 */

#ifndef ADC_H_
#define ADC_H_

void ADC_init();
void ADC_Convert();
uint16_t ADC_Data_Request();
void ADC_Data_Set();
uint16_t ADC_Data_to_Voltage(uint16_t voltage);
void ADC_Flag_Set();
void ADC_Flag_Clear();
uint8_t ADC_Flag_Request();


#endif /* ADC_H_ */
