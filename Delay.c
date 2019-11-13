/*
 * Delay.c
 *
 *  Created on: Oct 7, 2019
 *      Author: jayerickson
 */


/*
 * Delay.c
 *
 * Created on: 8/1/19
 * Author: Murray_Erickson
 */

#include <Delay.h>

void T32_INT1_IRQHandler(void) {
    TIMER32_1->INTCLR = 0; // ISR clears interrupt status bit
}

void delay_CLK(uint32_t n) {
    // set MCLK to DCO
    CS->KEY = CS_KEY_VAL;
    CS->CTL1 = CS_CTL1_SELM_3 | CS_CTL1_SELS_3;
    CS->KEY = 0;

    // set priority level and enable T32 interrupt
    NVIC->IP[25] |= 0x00;
    NVIC->ISER[0] |= 0x02000000;

    // initialize timer32 module
   TIMER32_1->CONTROL = TIMER32_CONTROL_SIZE;
   TIMER32_1->LOAD = n;
   TIMER32_1->CONTROL |= TIMER32_CONTROL_ONESHOT;
   TIMER32_1->CONTROL |= TIMER32_CONTROL_IE;
   TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;

   __sleep(); // sleep until interrupt raised

   TIMER32_1->CONTROL = TIMER32_CONTROL_IE; // reset timer
}

void delay_ms(uint32_t delay, uint32_t frequency) {
    uint32_t n, int_freq, overhead;

    int_freq = set_DCO(frequency);
    n = int_freq/1000*delay;  // number of clock cycles to delay for
    overhead = 22*int_freq/1000000 + 253; // overhead from linear curve fit based on scope data
    n -= overhead;  // adjust for overhead
    delay_CLK(n);
}

void delay_us(uint32_t delay, uint32_t frequency) {
    uint32_t n, int_freq, overhead;

    int_freq = set_DCO(frequency);
    n = int_freq/1000000*delay;
    overhead = 22*int_freq/1000000 + 253;
    n -= overhead;
    delay_CLK(n);
}

uint32_t set_DCO(uint32_t frequency) {
    uint32_t int_freq;

    // return integer representing frequency depending upon user entry
    if(frequency == FREQ_1_5_MHz)
        int_freq = 1500000;
    else if(frequency == FREQ_3_MHz)
        int_freq = 3000000;
    else if(frequency == FREQ_6_MHz)
        int_freq = 6000000;
    else if(frequency == FREQ_12_MHz)
        int_freq = 12000000;
    else if(frequency == FREQ_24_MHz)
        int_freq = 24000000;
    else if(frequency == FREQ_48_MHz) { // must account for MCU power necessities in this mode
        // Transition to VCORE Level 1: AM0_LDO --> AM1_LDO
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
        PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
        while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));

        // Configure Flash wait-state to 1 for both banks 0 & 1
        FLCTL->BANK0_RDCTL = (FLCTL->BANK0_RDCTL &
        ~(FLCTL_BANK0_RDCTL_WAIT_MASK)) | FLCTL_BANK0_RDCTL_WAIT_1;
        FLCTL->BANK1_RDCTL = (FLCTL->BANK0_RDCTL &
        ~(FLCTL_BANK1_RDCTL_WAIT_MASK)) | FLCTL_BANK1_RDCTL_WAIT_1;
        int_freq = 48000000;
    }
    // set DCO to one of six possible values
    CS->KEY = CS_KEY_VAL;
    CS->CTL0 = 0;
    CS->CTL0 |= frequency;
    CS->KEY = 0;

    return int_freq;
}
