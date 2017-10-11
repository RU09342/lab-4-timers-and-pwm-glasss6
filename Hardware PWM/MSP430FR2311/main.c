/**
 * main.c
 Cycle through PWM percentage for an LED using Hardware PWM.
 Press button to go from 0%-100% brightness for an LED.
 MSP430FR2311
 Stephen Glass
 */

#include <msp430.h>

#define FRSERIES 1

#define LED0    BIT0
#define LED0OUT P1OUT
#define LED0DIR P1DIR
#define LED0SEL P1SEL0

#define LED1    BIT0
#define LED1OUT P2OUT
#define LED1DIR P2DIR
#define LED1SEL P2SEL0

#define BTN0    BIT1
#define BTN0REN P1REN
#define BTN0OUT P1OUT
#define BTN0IES P1IES
#define BTN0IE  P1IE
#define BTN0IFG P1IFG
#define BTN0SEL P1SEL0


void initLED();
void initializeButton();
void initializePwmTimer();
void initializeTimer(int hertz);

unsigned int debounceCount = 1;
unsigned int debouncePressed = 0;

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    #if defined(FRSERIES)
        PM5CTL0 &= ~LOCKLPM5; // High-impedence mode needed for FR series
    #endif

    initLED();
    initializeButton();
    initializePwmTimer(); // Initialize timer at 10Hz or 0.1s
    initializeTimer(250); // 250Hz = 4ms .... 1MHz / 4000 = 250Hz = 4mS

    _BIS_SR(CPUOFF + GIE); // LPM0 with interrupts enabled
}

void initLED()
{
    P1SEL1 |= (BIT6); // Set LED to be controlled by PWM timer
    //P1SEL1 |= (BIT0);
    LED1SEL &= ~(LED1); // Disable non-PWM LED port mux
    //BTN0SEL &= ~(BTN0); // Disable button port mux

    LED0DIR |= (LED0); // Set LED to output direction

    LED1DIR |= (LED1); // Set LED to output direction
    LED1OUT &= ~(LED1); // Reset the LED
    // This LED will flash when the button is being pressed
}

void initializeButton()
{
    BTN0REN |= BTN0;                // puller-resistor on the button pin P1.1
    BTN0OUT |= BTN0;                // writes "1" to portpin, telling the resistor to pullup
    BTN0IES |= BTN0;                // triggers when you press the button, "~BIT1" for when release
    BTN0IE |= BTN0;                 // enables selector-mask for generating interrupts on the pin
    BTN0IFG &= BTN0;
}

void initializePwmTimer()
{
    /*** Timer0_A Init ***/
    TB0CTL = (MC_1 + TBSSEL_2); // Upmode using 1MHz clock
    TB0CCTL1 = (OUTMOD_7); // Output to GPIO
    TB0CCR0 = 1000; // Set priod to 1kHz, 1Mhz / 1000 = 1kHz
    TB0CCR1 = 500; // Set initial duty cycle to 50%
}

void initializeTimer(int hertz)
{
    /*** Timer1_A Init ***/
    unsigned int calc = (1000000 / hertz);
    TB1CCR0 |= calc;                // Counter value
    TB1CCTL0 |= CCIE;               // Enable Timer1_A interrupts
    TB1CTL |= TBSSEL_2 + MC_1;      // SMCLK, Up Mode (Counts to TA1CCR0)
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) // function called when the port is interrupted (button pressed)
{
    if(BTN0IFG & BTN0) // If the button is being pressed
    {
        if(debouncePressed == 0) // If there is no software debouncing currently happening
        {
            debouncePressed = 1; // The button was pressed so we need to do some software debouncing
            if(BTN0IES & BTN0) // If IES is on positive edge only (remember that we trigger the edge later)
            {
                if(TB0CCR1 >= 1000) TB0CCR1 = 0; // If PWM is at 100%, reset to 0%
                else TB0CCR1 += 100; // Increase by 10%
            }
            LED1OUT ^= (LED1); // Enable the LED while the button is being pressed down
            // The LED will toggle and reset when this is called again on the negative edge
            BTN0IFG &= ~BTN0; // Reset the interupt flag
            BTN0IES ^= BTN0; // This will trigger the edge so when it is released it will toggle the enable again
        }
    }
}

// Timer B0 interrupt service routine
#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer1_B0 (void) // Set to 250Hz, so called every 4ms
{
    if(debouncePressed == 1) // If the button was pressed, count for debouncing control
    {
        if(debounceCount >= 60) // 4ms called 60 times = 240ms
        {
            debouncePressed = 0; // Reset the button variable so it can be pressed again
            debounceCount = 0; // Reset the counter variable
        }
        else debounceCount++; // Increment the counter variable 240ms
    }
}
