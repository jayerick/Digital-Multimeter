/*
 * UART.h
 *
 *  Created on: Nov 4, 2019
 *      Author: jayerickson
 */

#ifndef UART_H_
#define UART_H_

#define Terminal_Width 27

void UART_init();
void UART_transmit(uint16_t data);
void UART_transmit_str(char* data);
void Bar();
void Escape();
void Cursor_return();
void Cursor_up(char num);
void Cursor_down(char num);
void Cursor_left(char num);
void Cursor_right(char num);
void Terminal_clear();
void UART_RX_Interrupt_En();



#endif /* UART_H_ */
