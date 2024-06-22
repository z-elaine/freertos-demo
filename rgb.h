#ifndef __RGBLED_H__
#define __RGBLED_H__

#define RED                     0
#define GREEN                   1
#define BLUE                    2

// Ratio for percent of full on that should be "true" white.
#define RED_WHITE_BALANCE        0.497f
#define GREEN_WHITE_BALANCE      0.6f
#define BLUE_WHITE_BALANCE       1.0f

// GPIO, Timer, Peripheral, and Pin assignments for the colors
#define RED_GPIO_PERIPH         SYSCTL_PERIPH_GPIOF
#define RED_TIMER_PERIPH        SYSCTL_PERIPH_TIMER0
#define BLUE_GPIO_PERIPH        SYSCTL_PERIPH_GPIOF
#define BLUE_TIMER_PERIPH       SYSCTL_PERIPH_TIMER1
#define GREEN_GPIO_PERIPH       SYSCTL_PERIPH_GPIOF
#define GREEN_TIMER_PERIPH      SYSCTL_PERIPH_TIMER1


#define RED_GPIO_BASE           GPIO_PORTF_BASE
#define RED_TIMER_BASE          TIMER0_BASE
#define BLUE_GPIO_BASE          GPIO_PORTF_BASE
#define BLUE_TIMER_BASE         TIMER1_BASE
#define GREEN_GPIO_BASE         GPIO_PORTF_BASE
#define GREEN_TIMER_BASE        TIMER1_BASE

#define RED_GPIO_PIN            GPIO_PIN_1
#define BLUE_GPIO_PIN           GPIO_PIN_2
#define GREEN_GPIO_PIN          GPIO_PIN_3


#define RED_GPIO_PIN_CFG        GPIO_PF1_T0CCP1
#define BLUE_GPIO_PIN_CFG       GPIO_PF2_T1CCP0
#define GREEN_GPIO_PIN_CFG      GPIO_PF3_T1CCP1

#define RED_TIMER_CFG           TIMER_CFG_B_PWM
#define BLUE_TIMER_CFG          TIMER_CFG_A_PWM
#define GREEN_TIMER_CFG         TIMER_CFG_B_PWM

#define RED_TIMER               TIMER_B
#define BLUE_TIMER              TIMER_A
#define GREEN_TIMER             TIMER_B

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Functions exported from rgb.c
//
//*****************************************************************************
extern void RGBInit(uint32_t ui32Enable);

extern void RGBEnable(void);
extern void RGBDisable(void);
extern void RGBSet(volatile uint32_t * pui32RGBColor, float fIntensity);
extern void RGBColorSet(volatile uint32_t * pui32RGBColor);

extern void RGBIntensitySet(float fIntensity);


// Mark the end of the C bindings section for C++ compilers.
#ifdef __cplusplus
}
#endif

#endif // __RGBLED_H__
