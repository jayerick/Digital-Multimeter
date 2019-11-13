#include "msp.h"
#include <DAC.h>

/*
 * DAC.c
 *
 *  Created on: Oct 17, 2019
 *      Author: jayerickson
 */

#define START 0x30
#define CHIP_SEL BIT4

void DAC_init()
{
    // SPI Pin Enables
    P1->SEL0 |= (BIT5 | BIT6);   //1.5 = B0-clk, 1.6= B0-SDI
    P1->SEL1 &= ~(BIT5 | BIT6);

    // Chip Select Enable
    P4->SEL0 &= ~BIT4;   //4.4 = CS as GPIO
    P4->SEL1 &= ~BIT4;
    P4->DIR |= BIT4;
    P4->OUT |= BIT4;

    // Initialize CLTW0
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;

    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST |
                      EUSCI_B_CTLW0_MST |
                      EUSCI_B_CTLW0_SYNC|
                      EUSCI_B_CTLW0_CKPL|
                      EUSCI_B_CTLW0_MSB;

    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;  //selects SMCLK as clk source
    EUSCI_B0->BRW = 0x01;           //clk divider = 1 (no division)
    EUSCI_B0->STATW |= EUSCI_B_STATW_LISTEN;
    EUSCI_B0->IE |= (BIT0 | BIT1);  //enables interupts on TX and RX
    EUSCI_B0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST);    //clears software reset bit
}

void DAC_voltage(int voltage){
    int lowByte;
    int highByte;


    lowByte = voltage & 0xFF;
    highByte = ((voltage >> 8) & 0x0F) | (START);

    P4->OUT &= ~CHIP_SEL; //set CS low
    EUSCI_B0->TXBUF = highByte; // set data in to TXbuffer
    /* wait for TX buffer to be empty */
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG));
    EUSCI_B0->TXBUF = lowByte;
    /* wait for recieve buffer to fill with our transmited data */
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG));

    P4->OUT |= CHIP_SEL; //set CS high

}
