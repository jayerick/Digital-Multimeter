#include "msp.h"
#include <stdint.h>
#include <string.h>

#define Terminal_Width 27
#define ESC 0x1B
/*
 * UART.c
 *
 *  Created on: Nov 4, 2019
 *      Author: jayerickson
 */



void UART_init(){

    P1->SEL0 |= BIT2 | BIT3;
    P1->SEL1 &= ~(BIT2 | BIT3);

    EUSCI_A0->CTLW0 = EUSCI_A_CTLW0_SWRST |
                       EUSCI_A_CTLW0_SSEL__SMCLK;

    EUSCI_A0->BRW = 1;
    EUSCI_A0->MCTLW = EUSCI_A_MCTLW_OS16 |
                      (10 << EUSCI_A_MCTLW_BRF_OFS);

    EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);
    }

void UART_transmit(uint16_t data){
    while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A0->TXBUF = data;
}

void UART_transmit_str(char* data){
    int letter;
    for (letter = 0; letter < strlen(data); letter++){
            UART_transmit(data[letter]);
    }
}

void Bar(){
    int width;
    for (width = 0; width < Terminal_Width; width++){
            UART_transmit('-');
    }
}

void Escape(){
    UART_transmit(ESC);
    UART_transmit('[');
}

void Cursor_return(){
    Escape();
    UART_transmit('H');
}

void Cursor_up(char num){
    Escape();
    UART_transmit(num);
    UART_transmit('A');
}

void Cursor_down(char num){
    Escape();
    UART_transmit(num);
    UART_transmit('B');
}

void Cursor_left(char num){
    Escape();
    UART_transmit(num);
    UART_transmit('C');
}

void Cursor_right(char num){
    Escape();
    UART_transmit(num);
    UART_transmit('D');
}

void Terminal_clear(){
    Escape();
    UART_transmit('2');
    UART_transmit('J');
}

void UART_RX_Interrupt_En(){
    EUSCI_A0->IFG &= ~(EUSCI_A_IFG_RXIFG);
    EUSCI_A0->IE |= EUSCI_A_IE_RXIE;

    __NVIC_EnableIRQ(EUSCIA0_IRQn);
}







