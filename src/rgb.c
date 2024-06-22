#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "rgb.h"

static uint32_t  g_ui32Colors[3];
static float g_fIntensity = 0.3f;

void
RGBBlinkIntHandler(void)
{
    static unsigned long ulFlags;

    // Clear the timer interrupt.
    ROM_TimerIntClear(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);

    // Toggle the flag for the blink timer.
    ulFlags ^= 1;

    if(ulFlags)
    {
        RGBEnable();
    }
    else
    {
        RGBDisable();
    }
}

// Initializes the Timer and GPIO functionality associated with the RGB LED
void
RGBInit(uint32_t ui32Enable)
{
    //
    // Enable the GPIO Port and Timer for each LED
    //
    ROM_SysCtlPeripheralEnable(RED_GPIO_PERIPH);
    ROM_SysCtlPeripheralEnable(RED_TIMER_PERIPH);

    ROM_SysCtlPeripheralEnable(GREEN_GPIO_PERIPH);
    ROM_SysCtlPeripheralEnable(GREEN_TIMER_PERIPH);

    ROM_SysCtlPeripheralEnable(BLUE_GPIO_PERIPH);
    ROM_SysCtlPeripheralEnable(BLUE_TIMER_PERIPH);

    //
    // Configure each timer for output mode
    //
    HWREG(GREEN_TIMER_BASE + TIMER_O_CFG)   = 0x04;
    HWREG(GREEN_TIMER_BASE + TIMER_O_TAMR)  = 0x0A;
    HWREG(GREEN_TIMER_BASE + TIMER_O_TAILR) = 0xFFFF;

    HWREG(BLUE_TIMER_BASE + TIMER_O_CFG)   = 0x04;
    HWREG(BLUE_TIMER_BASE + TIMER_O_TBMR)  = 0x0A;
    HWREG(BLUE_TIMER_BASE + TIMER_O_TBILR) = 0xFFFF;

    HWREG(RED_TIMER_BASE + TIMER_O_CFG)   = 0x04;
    HWREG(RED_TIMER_BASE + TIMER_O_TBMR)  = 0x0A;
    HWREG(RED_TIMER_BASE + TIMER_O_TBILR) = 0xFFFF;

    //
    // Invert the output signals.
    //
    HWREG(RED_TIMER_BASE + TIMER_O_CTL)   |= 0x4000;
    HWREG(GREEN_TIMER_BASE + TIMER_O_CTL)   |= 0x40;
    HWREG(BLUE_TIMER_BASE + TIMER_O_CTL)   |= 0x4000;

    if(ui32Enable)
    {
        RGBEnable();
    }

    //
    // Setup the blink functionality
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
    ROM_TimerConfigure(WTIMER5_BASE, TIMER_CFG_B_PERIODIC | TIMER_CFG_SPLIT_PAIR);
    ROM_TimerLoadSet64(WTIMER5_BASE, 0xFFFFFFFFFFFFFFFF);
    ROM_IntEnable(INT_WTIMER5B);
    ROM_TimerIntEnable(WTIMER5_BASE, TIMER_TIMB_TIMEOUT);


}

// Enable the RGB LED with already configured timer settings
void
RGBEnable(void)
{

    //
    // Enable timers to begin counting
    //
    ROM_TimerEnable(RED_TIMER_BASE, TIMER_BOTH);
    ROM_TimerEnable(GREEN_TIMER_BASE, TIMER_BOTH);
    ROM_TimerEnable(BLUE_TIMER_BASE, TIMER_BOTH);

    //
    // Reconfigure each LED's GPIO pad for timer control
    //
    ROM_GPIOPinConfigure(GREEN_GPIO_PIN_CFG);
    ROM_GPIOPinTypeTimer(GREEN_GPIO_BASE, GREEN_GPIO_PIN);
    ROM_GPIOPadConfigSet(GREEN_GPIO_BASE, GREEN_GPIO_PIN, GPIO_STRENGTH_8MA_SC,
                     GPIO_PIN_TYPE_STD);

    ROM_GPIOPinConfigure(BLUE_GPIO_PIN_CFG);
    ROM_GPIOPinTypeTimer(BLUE_GPIO_BASE, BLUE_GPIO_PIN);
    ROM_GPIOPadConfigSet(BLUE_GPIO_BASE, BLUE_GPIO_PIN, GPIO_STRENGTH_8MA_SC,
                     GPIO_PIN_TYPE_STD);

    ROM_GPIOPinConfigure(RED_GPIO_PIN_CFG);
    ROM_GPIOPinTypeTimer(RED_GPIO_BASE, RED_GPIO_PIN);
    ROM_GPIOPadConfigSet(RED_GPIO_BASE, RED_GPIO_PIN, GPIO_STRENGTH_8MA_SC,
                     GPIO_PIN_TYPE_STD);
}

// Disable the RGB LED by configuring the GPIO's as inputs.
void
RGBDisable(void)
{
    //
    // Configure the GPIO pads as general purpose inputs.
    //
    ROM_GPIOPinTypeGPIOInput(RED_GPIO_BASE, RED_GPIO_PIN);
    ROM_GPIOPinTypeGPIOInput(GREEN_GPIO_BASE, GREEN_GPIO_PIN);
    ROM_GPIOPinTypeGPIOInput(BLUE_GPIO_BASE, BLUE_GPIO_PIN);

    //
    // Stop the timer counting.
    //
    ROM_TimerDisable(RED_TIMER_BASE, TIMER_BOTH);
    ROM_TimerDisable(GREEN_TIMER_BASE, TIMER_BOTH);
    ROM_TimerDisable(BLUE_TIMER_BASE, TIMER_BOTH);
}

// Set the current output intensity.
void
RGBIntensitySet(float fIntensity)
{
    g_fIntensity = fIntensity;
    RGBColorSet(g_ui32Colors);
}

// Set the output color.
void
RGBColorSet(volatile uint32_t * pui32RGBColor)
{
    uint32_t ui32Color[3];
    uint32_t ui32Index;

    for(ui32Index=0; ui32Index < 3; ui32Index++)
    {
        g_ui32Colors[ui32Index] = pui32RGBColor[ui32Index];
        ui32Color[ui32Index] = (uint32_t) (((float) pui32RGBColor[ui32Index]) *
                            g_fIntensity + 0.5f);

        if(ui32Color[ui32Index] > 0xFFFF)
        {
            ui32Color[ui32Index] = 0xFFFF;
        }
    }

    ROM_TimerMatchSet(RED_TIMER_BASE, RED_TIMER, ui32Color[RED]);
    ROM_TimerMatchSet(GREEN_TIMER_BASE, GREEN_TIMER, ui32Color[GREEN]);
    ROM_TimerMatchSet(BLUE_TIMER_BASE, BLUE_TIMER, ui32Color[BLUE]);
}
