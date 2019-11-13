#include "msp.h"
#include "delay.h"
#include "ADC.h"
#include "DMM.h"
/*
 * ADC.c
 *
 *  Created on: Nov 4, 2019
 *      Author: jayerickson
 */

static uint16_t ADC_Data;
static uint8_t ADC_Flag;
void ADC_init(){

    //Set P6.0 to A15
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT1;

    ADC14->CTL0 = ADC14_CTL0_SHP |
                  ADC14_CTL0_ON  |
                  ADC14_CTL0_SHT0__192;

    ADC14->CTL1 = (10 << ADC14_CTL1_CSTARTADD_OFS) | ADC14_CTL1_RES_3;

    ADC14->MCTL[10] |= ADC14_MCTLN_INCH_15;

    ADC14->IER0 |= ADC14_IER0_IE10;
    ADC14->CTL0 |= ADC14_CTL0_ENC;
    __NVIC_EnableIRQ(ADC14_IRQn);
}

void ADC_Convert(){
    ADC14->CTL0 |= ADC14_CTL0_SC |
                   ADC14_CTL0_ENC;
}
void ADC_Data_Set(){
    ADC_Data = ADC14->MEM[10];
}

uint16_t ADC_Data_Request(){
    return ADC_Data;
}



uint16_t ADC_Data_to_Voltage(uint16_t voltage){
    return (uint16_t)((voltage * 0.2 + 4));
}
void ADC_Flag_Clear(){
    ADC_Flag = 0;
}
void ADC_Flag_Set(){
    ADC_Flag = 1;
}
uint8_t ADC_Flag_Request(){
    return ADC_Flag;
}
