/*
 * Keypad.h
 *
 *  Created on: Oct 2, 2019
 *      Author: Matt Murray
 */


// Bitmask for Keypad
#define COL1 BIT0
#define COL2 BIT1
#define COL3 BIT2
#define ROW1 BIT0
#define ROW2 BIT1
#define ROW3 BIT2
#define ROW4 BIT3


//Shortcuts for fcns
char Get_Key(void);
void Keypad_init(void);
void P4_interrupt_init(void);
