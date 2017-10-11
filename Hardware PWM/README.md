# Hardware Debouncing
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

The PWM timer is set to output to a GPIO pin directly. (This is done with the OUTMOD_7 parameter). Using the PXSELY registers we are able to specify to the processor which GPIO pin we wish to set as an output. We are able to set capture compare registers for the PWM timer to adjust the duty cycle of the timer.

Some boards (e.g FR2311, G2553) do not have the LED pin directly hooked up to an output pin of the timer. For this case, we set the output to an unused timer output and use a jumper wire to connect to the LED pin directly.

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
Both buttons have software debouncing implemented. There is a timer module which runs at a 250Hz clock and debouncing code that takes place for 240ms after the button is pressed. This will prevent any accidental presses.