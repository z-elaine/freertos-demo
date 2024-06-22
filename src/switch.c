#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "buttons.h"
#include "utils/uartstdio.h"
#include "blinky.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "buttons.h"
#include "switch.h"

// The stack size for the display task.
#define SWITCHTASKSTACKSIZE        128         // Stack size in words

// The priorities of the various tasks.
#define PRIORITY_SWITCH_TASK    2

extern xQueueHandle g_pLEDQueue;
extern xSemaphoreHandle g_pUARTSemaphore;

// This task reads the buttons' state and passes this information to LEDTask.
static void 
SwitchTask(void *pvParameters)
{	
	portTickType ui16LastTime;
    uint32_t ui32SwitchDelay = 25;
    uint8_t ui8CurButtonState, ui8PrevButtonState;
    uint8_t ui8Message;

    ui8CurButtonState = ui8PrevButtonState = 0;
	
	//Get the current tick count.
	ui16LastTime = xTaskGetTickCount();
	
	while(1)
	{
		// Poll the debounced state of the buttons.
        ui8CurButtonState = ButtonsPoll(0, 0);
		
		// Check if previous debounced state is equal to the current state.
		if(ui8CurButtonState != ui8PrevButtonState)
		{
			ui8PrevButtonState = ui8CurButtonState;
			
			// Check to make sure the change in state is due to button press
            // and not due to button release.
			if((ui8CurButtonState & ALL_BUTTONS) != 0)
			{
				if((ui8CurButtonState & ALL_BUTTONS) == LEFT_BUTTON)
				{
					ui8Message = LEFT_BUTTON;
					
					// Guard UART from concurrent access.
					xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
                    UARTprintf("Left Button is pressed.\n");
                    xSemaphoreGive(g_pUARTSemaphore);					
				}else if((ui8CurButtonState & ALL_BUTTONS) == RIGHT_BUTTON)
				{
					ui8Message = RIGHT_BUTTON;

                    // Guard UART from concurrent access.
                    xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
                    UARTprintf("Right Button is pressed.\n");
                    xSemaphoreGive(g_pUARTSemaphore);
				}
				
				if(xQueueSend(g_pLEDQueue, &ui8Message, portMAX_DELAY) != pdPASS)
				{
					// Error. The queue should never be full. If so print the
                    // error message on UART and wait for ever.
					UARTprintf("\nQueue full. This should never happen.\n");
                    while(1)
                    {
                    }
				}
			}			
		}
		
		// Wait for the required amount of time to check back.
        vTaskDelayUntil(&ui16LastTime, ui32SwitchDelay / portTICK_RATE_MS);	
	}
}

// Initializes the switch task.
uint32_t
SwitchTaskInit(void)
{
	// Unlock the GPIO LOCK register for Right button to work.
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0xFF;
	
	// Initialize the buttons
    ButtonsInit();
	
	// Create the switch task.
	if(xTaskCreate(SwitchTask, (signed portCHAR *)"Switch",
                   SWITCHTASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
                   PRIORITY_SWITCH_TASK, NULL) != pdTRUE)
	{
		return (1);
	}
	
	return (0);	
}
