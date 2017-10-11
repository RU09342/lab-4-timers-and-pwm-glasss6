# Software Debouncing
## Stephen Glass

## main.c
### Compatibility
* MSP430G2553
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
* MSP430FR5529

### Description
This code will generate a 1kHz PWM signal with a duty cycle between 0% and 100%. When the processor starts up it will initially set an LED to 50% duty cycle. When you press Button 1 it will increase the duty cycle of the LED by 100%. Once the duty cycle has reached 100% the duty cycle will go to 0% on the next button press.

Additionally, when Button 1 is pressed it will light up LED2 while the button is being depressed. This is figure out if the button has been triggered multiple times. However, the button should not be triggered multiple times because the buttons have Software debouncing code implemented.

The PWM timer will call an interrupt function when either of the capture compare registers are reached. In that interrupt function we will toggle an LED to simulate a PWM waveform with the duty cycle specified. We can change the duty cycle dynamically by changing the value of the capture compare registers.

## Functions
### initializeTimer(int capture)
This function will initialize the TimerA/B module with the timing of your choosing. This is done by using the SMCLK and using any clock dividers neccessary to achieve correct timing.
* int capture = time in hertz to cycle

Example:
initializeTimer(20); // Initialize a timer for 20Hz

### initLED(void)
This function will initialize the LEDs for PWM output.

### initializeButtons(void)
This function will initialize any buttons needed for interrupts.

### initializePwmTimer(void)
This function will setup a Timer module to create a 1kHz PWM wave with an initial duty cycle of 50%.

## Extra Work
Both buttons have software debouncing implemented. There is a debouncing code that takes place for 240ms after the button is pressed. This will prevent any accidental presses.