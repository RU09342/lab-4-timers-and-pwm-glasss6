# Software Debouncing
## Stephen Glass

## main.c
### Compatibility
* MSP430G2553
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
* MSP430FR5529

NOTE: Boards with more than one button include additional functionality! (E.g: FR5994, FR6989, FR5529)

### Description
main.c will toggle an LED when a button is pressed. Pressing Button 1 will enable LED1. Pressing Button 2 will enable LED2. Both buttons have software debouncing code. A clock runs at 20Hz and checks for debouncing logic to be done. If a button was pressed, it will not allow you to press the button again until it passes through the 20Hz clock 5 times. This equates to a debouncing delay of 250ms. This is all done using Timer modules and interrupts so you can perform other actions while this is happening.

## Functions
### initializeTimer(int capture)
This function will initialize the TimerA/B module with the timing of your choosing. This is done by using the SMCLK and using any clock dividers neccessary to achieve correct timing.
* int capture = time in hertz to cycle

Example:
initializeTimer(20); // Initialize a timer for 20Hz

## Extra Work
Boards with more than one button will showcase software debouncing with two buttons. Button 1 will toggle LED 1 with software debouncing. Button 2 will toggle LED 2 with software debouncing. This process is still done using one Timer module.