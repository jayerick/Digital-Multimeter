/*
 * Keypad.c
 *
 *  Created on: Oct 7, 2019
 *      Author: jayerickson
 */


//#include "LCD.h"
#include "Delay.h"
#include "stdint.h"
#include "Keypad.h"

int col;

void Keypad_init(void){
    P5->DIR |= (COL1 | COL2 | COL3);  //set columns as outputs
    P5->OUT &= ~(COL1 | COL2 | COL3);  //initializes columns to 0
    P4->DIR &= ~(ROW1 | ROW2 | ROW3 | ROW4); //sets rows to inputs
    P4->REN |= (ROW1 | ROW2 | ROW3 | ROW4); //enables resistors
    P4->OUT &= ~(ROW1 | ROW2 | ROW3 | ROW4); //sets pull up resistors

}



char Get_Key(void) {
    uint8_t rows;
    int col;
    for (col=0; col<3; col++){              //loop to send a single column high
        P5->OUT &= ~(COL1 | COL2 | COL3);   //clear columns to 0
        P5->OUT |= (COL1 << col);           //shift bit0 to send the next colum high
        __delay_cycles(25);                 //wait for data to settle
        rows = (P4->IN & 0x0F);             //assign reading from rows

        if(rows != 0)                       //exit row if button on that column is pressed
            break;
    }

    uint8_t val;
    val = (col << 4);                       //shift column data to the left
    val += rows;                            //add rows data to create single value for each btn

    char val_out;

    // output for no btn pressed
    if (col == 3)
        val_out = '$'; //value for flagging error or no press

    switch (val){
        //Column 1
        case 0x01: val_out = '1';
                   break;
        case 0x02: val_out = '4';
                   break;
        case 0x04: val_out = '7';
                   break;
        case 0x08: val_out = '*';
                   break;
       // Column 2
        case 0x11: val_out = '2';
                   break;
        case 0x12: val_out = '5';
                   break;
        case 0x14: val_out = '8';
                   break;
        case 0x18: val_out = '0';
                   break;
       // Column 3
        case 0x21: val_out = '3';
                   break;
        case 0x22: val_out = '6';
                   break;
        case 0x24: val_out = '9';
                   break;
        case 0x28: val_out = '#';
                   break;
       // Default for no press or data error
        default:   val_out = '$';
    }
    return val_out;
}

void P4_interrupt_init(void) {
    P4->IES &= ~(ROW1|ROW2|ROW3|ROW4); // falling edge
    P4->IFG &= ~(ROW1|ROW2|ROW3|ROW4); // Clear flag
    P4->IE  |= (ROW1|ROW2|ROW3|ROW4);  // Enable interrupts
    //__NVIC_EnableIRQ(38);             // port 4 interrupts
}
