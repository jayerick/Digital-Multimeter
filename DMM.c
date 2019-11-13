#include "msp.h"
#include "Keypad.h"
#include "UART.h"
#include "delay.h"
#include "ADC.h"
/*
 * DMM.c
 *
 *  Created on: Nov 4, 2019
 *      Author: jayerickson
 */



void DMM_init(){
    set_DCO(FREQ_3_MHz);
    CS->KEY = CS_KEY_VAL;   //Unlock

    CS->CTL1 |= CS_CTL1_SELA__REFOCLK; //ACLK -> REFOCLK (1-sec overflow counter)

    CS->KEY = 0;    //Lock

    TIMER_A0->CTL = TIMER_A_CTL_CLR;
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__ACLK     |    //ACLK
                    TIMER_A_CTL_MC__CONTINUOUS |    //Continuous Mode
                    TIMER_A_CTL_IE;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CM__RISING |  //Rising Edge captures
                        TIMER_A_CCTLN_CCIS__GND  |  //Ground reference
                        TIMER_A_CCTLN_SCS        |  //Synchronous mode
                        TIMER_A_CCTLN_CAP        |  //Capture mode
                        TIMER_A_CCTLN_CCIE;

    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIE;

    TIMER_A1->CTL = TIMER_A_CTL_CLR;
    TIMER_A1->CTL = TIMER_A_CTL_SSEL__ACLK  |       //ACLK
                    TIMER_A_CTL_MC__UP      |       //Up Mode
                    TIMER_A_CTL_IE;

    TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;         //Capture/Compare Interrupts

    P1->DIR &= ~BIT1;
    P1->REN |= BIT1;
    P1->OUT |= BIT1;
    P1->IES &= ~BIT1;
    P1->IE |= BIT1;
    P1->IFG &= ~BIT1;

    __NVIC_EnableIRQ(TA0_0_IRQn);
    __NVIC_EnableIRQ(TA1_0_IRQn);
    __NVIC_EnableIRQ(PORT4_IRQn);

    //Keypad_init();
    //P4_interrupt_init();
}

void Terminal_init(){
    Terminal_clear();
    Cursor_return();
    Bar();
    Cursor_return();
    Cursor_down('1');
    UART_transmit_str("Murray Erickson Digital Multimeter");
    Cursor_return();
    Cursor_down('2');
    UART_transmit_str("Press 1 to Return to Home Screen");
    Cursor_return();
    Cursor_down('3');
    UART_transmit_str("Press 2 for DC Voltage Measurement");
    Cursor_return();
    Cursor_down('4');
    UART_transmit_str("Press 3 for AC Voltage Measurement");
    Cursor_return();
    Cursor_down('5');
    UART_transmit_str("Press 4 for Frequency Measurement");
    Cursor_return();
    Cursor_down('6');
    UART_transmit_str("Press 5 for all Measurements");
    Cursor_return();
}

uint16_t DC_Voltage_Reading(){
    uint16_t DC_Volt;
    uint16_t Max_DC = 0;
    uint16_t Min_DC = 20000;
    ADC_Flag_Clear();
    int Counter;
    ADC_Convert();
    for (Counter = 0; Counter < 3;){

        if (ADC_Flag_Request()){
            ADC_Data_Set();
            DC_Volt = ADC_Data_Request();
            //DC_Volt = ADC_Data_Request();

            if (DC_Volt > Max_DC){
                Max_DC = DC_Volt;
            }
            if (DC_Volt < Min_DC){
                Min_DC = DC_Volt;
            }
            ADC_Convert();
            ADC_Flag_Clear();
            Counter += 1;
        }


    }
    ADC_Data_Set();
    ADC14->CTL0 &= ~ADC14_CTL0_ENC;
    return (uint16_t)((Max_DC + Min_DC) / 2);
}
uint16_t AC_Voltage_Reading(){
    uint16_t AC_Volt;
    uint16_t Max_AC = 0;
    uint16_t Min_AC = 20000;
    int Counter;
    ADC_Convert();
    //ADC_Data_Set();

    //Going to need to find the frequency here so that you can go over single period

    for (Counter = 0; Counter < 50; Counter ++){
        AC_Volt = ADC_Data_Request();
        if (AC_Volt > Max_AC){
            Max_AC = AC_Volt;
        }
        if (AC_Volt < Min_AC){
            Min_AC = AC_Volt;
        }
    }
    return (uint16_t)((Max_AC - Min_AC));
}

uint16_t Frequency_Reading(){
    uint8_t a;
    a = 4;

}


void Terminal_Update_DC(uint16_t DC, uint16_t DC_Bar, uint8_t all){
    Terminal_clear();
    Cursor_return();
    Bar();
    Cursor_return();
    Cursor_down('1');
    UART_transmit_str("DC Voltage:");
    UART_transmit(DC);
    UART_transmit_str(" V");
    Cursor_return();
    Cursor_down('2');
    int i;
    for (i = 1; i < DC_Bar; i++){
        UART_transmit_str("*");
    }
    Cursor_return();
    Cursor_down('3');
    UART_transmit_str("|");
    Cursor_right('8');
    UART_transmit_str("|");
    Cursor_right('8');
    UART_transmit_str("|");
    Cursor_right('8');
    UART_transmit_str("|");
    Cursor_return();
    Cursor_down('4');
    UART_transmit_str("0 V");
    Cursor_right('4');
    UART_transmit_str("1 V");
    Cursor_right('4');
    UART_transmit_str("2 V");
    Cursor_right('4');
    UART_transmit_str("3 V");
    Cursor_down('5');
    Bar();
    }

void Terminal_Update_AC(uint16_t AC, uint16_t AC_Bar, uint8_t all){
    Terminal_clear();
    Cursor_return();
    Bar();
    Cursor_return();
    Cursor_down('1');
    UART_transmit_str("AC Voltage:");
    UART_transmit(AC);
    UART_transmit_str(" V");
    Cursor_return();
    Cursor_down('2');
    int i;
    for (i = 1; i < AC_Bar; i++){
        UART_transmit_str("*");
    }
    Cursor_return();
    Cursor_down('3');
    UART_transmit_str("|");
    Cursor_right('8');
    UART_transmit_str("|");
    Cursor_right('8');
    UART_transmit_str("|");
    Cursor_right('8');
    UART_transmit_str("|");
    Cursor_return();
    Cursor_down('4');
    UART_transmit_str("0 V");
    Cursor_right('4');
    UART_transmit_str("1 V");
    Cursor_right('4');
    UART_transmit_str("2 V");
    Cursor_right('4');
    UART_transmit_str("3 V");
    Cursor_down('5');
    Bar();
    }
void Terminal_Update_All(){

}

