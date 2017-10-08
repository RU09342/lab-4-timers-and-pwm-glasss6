/**
 * main.c
 Software debouncing two buttons controlling two LEDs.
 Will provide debouncing measures for 250ms from when the button was pressed.
 Accomplished using Timer modules and interrupts.
 MSP430F5529
 Stephen Glass
 */

#include <msp430.h>

#define LED0 BIT0
#define LED1 BIT7
#define LED0OUT P1OUT
#define LED0DIR P1DIR
#define LED1OUT P4OUT
#define LED1DIR P4DIR

#define BTN0    BIT1
#define BTN1    BIT1

#define BTN0REN P1REN
#define BTN0OUT P1OUT
#define BTN0IES P1IES
#define BTN0IE  P1IE
#define BTN0IFG P1IFG

#define BTN1REN P2REN
#define BTN1OUT P2OUT
#define BTN1IES P2IES
#define BTN1IE  P2IE
#define BTN1IFG P2IFG

void initializeTimer(int capture);

unsigned int debounceCount0 = 1;        // Counter to check button to change speed
unsigned int debounceCount1 = 1;
unsigned int debouncePressed0 = 0;
unsigned int debouncePressed1 = 0;;

unsigned int timerCount = 0;
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    //PM5CTL0 &= ~LOCKLPM5; // needed for FR series

    LED0DIR |= (LED0);                // configure P1.0 as output
    LED1DIR |= (LED1);                // configure P4.7 as output
    LED0OUT &= ~(LED0);               // reset the LEDs
    LED1OUT &= ~(LED1);               // reset the LEDs

    BTN0REN |= BTN0;                  // puller-resistor on the button pin P1.1
    BTN0OUT |= BTN0;                  // writes "1" to portpin, telling the resistor to pullup
    BTN0IES |= BTN0;                  // triggers when you press the button, "~BIT1" for when release
    BTN0IE |= BTN0;                   // enables selector-mask for generating interrupts on the pin
    BTN0IFG &= BTN0;

    BTN1REN |= BTN1;                  // puller-resistor on the button pin P2.1
    BTN1OUT |= BTN1;                  // writes "1" to portpin, telling the resistor to pullup
    BTN1IES |= BTN1;                  // triggers when you press the button, "~BIT1" for when release
    BTN1IE |= BTN1;                   // enables selector-mask for generating interrupts on the pin
    BTN1IFG &= BTN1;

    initializeTimer(20); // Initialize timer at 20Hz or 0.05s

    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled
}

void initializeTimer(int hertz) // Seconds = 1/Hertz, 10Hz = 0.1s
{
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_3; // SMCLK/8, UPMODE
    // CLK/HERTZ = CAPTURE
    // CLK = 1MHZ/8 = 125kHz
    int capture = (125000)/hertz;
    TA0CCR0 = capture; // (1000000/8)/(12500) = 10 Hz = 0.1 seconds
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) // function called when the port is interrupted (button pressed)
{
    if(BTN0IFG & BTN0) // If Player 1 button was pressed
    {
        if(debouncePressed0 == 0) // If the user is not going through software debounce process
        {
            debouncePressed0 = 1;
            LED0OUT ^= (LED0); // Toggle LED to blink
            BTN0IFG &= ~BTN0; // Button IFG cleared
        }
    }
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) // function called when the port is interrupted (button pressed)
{
    if(BTN1IFG & BTN1)
    {
        if(debouncePressed1 == 0) // If the user is not going through software debounce process
        {
            debouncePressed1 = 1;
            LED1OUT ^= (LED1); // Toggle LED to blink
            BTN1IFG &= ~BTN1; // Button IFG cleared
        }
    }
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    if(debounceCount0 >= 5) // 0.05 seconds 5 times, called every 250ms
    {
        debouncePressed0 = 0;
        debounceCount0 = 0; // Reset the counter variable
    }
    else debounceCount0++; // Increment the counter variable until 10s

    if(debounceCount1 >= 5) // 0.05 seconds 5 times, called every 250ms
    {
        debouncePressed1 = 0;
        debounceCount1 = 0; // Reset the counter variable
    }
    else debounceCount1++; // Increment the counter variable until 10s
}
