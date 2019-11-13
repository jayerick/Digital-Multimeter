#include "msp.h"
#include "DAC.h"
#include "Delay.h"
#include "Keypad.h"
#include "DMM.h"
#include "UART.h"
#include "ADC.h"


static uint8_t operation_mode = 0;
static uint8_t Home_Request = 1;

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	DMM_init();
	UART_init();
	ADC_init();
	Keypad_init();
	P4_interrupt_init();

    P4->IE &= ~(ROW1 | ROW2 | ROW3 | ROW4);
    P5->OUT |= (BIT0 | BIT1 | BIT2);

    P4->IFG &= ~(ROW1 | ROW2 | ROW3 | ROW4);
    P4->IE |= (ROW1 | ROW2 | ROW3 | ROW4);

	uint16_t Voltage_DC;
	uint16_t Voltage_AC;
	uint16_t Frequency;

	__enable_irq();
	while(1){
	    switch(operation_mode){
	    case 0:
	        if (Home_Request == 1){
	            Terminal_init();
	        }
	        Home_Request = 0;
	        //Get_Key();
	        break;

	    case 1:

	        Voltage_DC = ADC_Data_to_Voltage(DC_Voltage_Reading());
	        uint16_t DC_Bar =0;
	        Terminal_Update_DC(Voltage_DC, DC_Bar, 0);

	        break;

	    case 2:
	        Voltage_AC = ADC_Data_to_Voltage(AC_Voltage_Reading());
	        uint16_t AC_Bar = Voltage_AC / 3.3 * 27;
	        Terminal_Update_AC(Voltage_AC, AC_Bar, 0);
	        break;

	    case 3:
	        Frequency_Reading();
	        break;

	    case 4:

	        break;
	    }
	}
}

	void PORT4_IRQHandler(void)
	{
	    P4->IE &= ~(ROW1 | ROW2 | ROW3 | ROW4);
	    P5->OUT &= ~(BIT0 | BIT1 | BIT2);
	    P4->IFG &= ~(ROW1 | ROW2 | ROW3 | ROW4);

	    char character;
	    character = Get_Key();

	    switch(character){

	        case '1':
	            operation_mode = 0; //Returns to start screen for instructions
	            Home_Request = 1;
	            break;

	        case '2':
	            operation_mode = 1; //DC Voltage acquisition
	            break;

	        case '3':
	            operation_mode = 2; //AC Voltage acquisition
	            break;

	        case '4':
                operation_mode = 3; //Frequency acquisition
                break;

            case '5':
                operation_mode = 4; //AC Voltage and Frequency
                break;

            default:
                break;

	    }
	    P4->IE &= ~(ROW1 | ROW2 | ROW3 | ROW4);
	    P5->OUT |= (BIT0 | BIT1 | BIT2);

	    P4->IFG &= ~(ROW1 | ROW2 | ROW3 | ROW4);
	    P4->IE |= (ROW1 | ROW2 | ROW3 | ROW4);
	}

	void ADC14_IRQHandler(void){
	    ADC_Flag_Set();

	}



