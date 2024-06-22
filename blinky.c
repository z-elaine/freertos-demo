#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "blinky.h"
#include "rgb.h"

// [G, R, B] range is 0 to 0xFFFF per color.
static uint32_t g_pui32Colors[3] = { 0x0000, 0x0000, 0x0000 };
static uint8_t g_ui8ColorsIndx;

// Default LED toggle delay value. LED toggling frequency is twice this number.
#define LED_TOGGLE_DELAY        500
#define LEDTASKSTACKSIZE        128         // Stack size in words
#define PRIORITY_LED_TASK       1

static void
LEDTask(void *pvParameters)
{
    portTickType ui32WakeTime;
    uint32_t ui32LEDToggleDelay;
	
    // Initialize the LED Toggle Delay to default value.
    ui32LEDToggleDelay = LED_TOGGLE_DELAY;

    // Get the current tick count.
    ui32WakeTime = xTaskGetTickCount();

    // Loop forever.
    while(1)
    {
        // Turn on the LED.
        RGBEnable();

        // Wait for the required amount of time.
        vTaskDelayUntil(&ui32WakeTime, ui32LEDToggleDelay / portTICK_RATE_MS);

        // Turn off the LED.
        RGBDisable();

        // Wait for the required amount of time.
        vTaskDelayUntil(&ui32WakeTime, ui32LEDToggleDelay / portTICK_RATE_MS);
    }
}

uint32_t
LEDTaskInit(void){
	RGBInit(1);
	RGBIntensitySet(0.3f);
	
    g_ui8ColorsIndx = 1;
    g_pui32Colors[g_ui8ColorsIndx] = 0x8000;
    RGBColorSet(g_pui32Colors);
	
	// Print the current loggling LED and frequency.
    UARTprintf("\nLed %d is blinking. [G, R, B]\n", g_ui8ColorsIndx);
    UARTprintf("Led blinking frequency is %d ms.\n", (LED_TOGGLE_DELAY * 2));
	
	// Create the LED task.
	if(xTaskCreate(LEDTask, (signed portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
                   tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL) != pdTRUE)
	{
		return (1);
	}
	
	return (0);
}
