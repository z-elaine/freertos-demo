#include "blinky.h"

void 
setupHardware(void){

    // Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable the GPIO pins for the LED .
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 |  GPIO_PIN_3);	
}

void 
vRedLEDBlinkTask(void *pvParameters){
	while(1){
	// Turn on the BLUE LED.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);

	// Delay for a bit.
	SysCtlDelay(SysCtlClockGet() / 10 / 1);

	// Turn off the BLUE LED.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);

	// Delay for a bit.
	SysCtlDelay(SysCtlClockGet() / 10 / 1);
	}
}

void 
vBlueLEDBlinkTask(void *pvParameters){
	while(1){
	// Turn on the BLUE LED.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

	// Delay for a bit.
	SysCtlDelay(SysCtlClockGet() / 10 / 5);

	// Turn off the BLUE LED.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

	// Delay for a bit.
	SysCtlDelay(SysCtlClockGet() / 10 / 5);
	}
}

void 
vGreenLEDBlinkTask(void *pvParameters){
	while(1){
	// Turn on the BLUE LED.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);

	// Delay for a bit.
	SysCtlDelay(SysCtlClockGet() / 10 / 9);

	// Turn off the BLUE LED.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);

	// Delay for a bit.
	SysCtlDelay(SysCtlClockGet() / 10 / 9);
	}
} 
 	