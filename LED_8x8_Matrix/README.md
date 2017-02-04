Animated 8x8 LED Matrix

Adapted by Kevin Edzenga


This script will run this animation-

https://www.youtube.com/watch?v=bccOX-seROI



This is for the - 5-pin MAX7219 8x8 Matrix Array

MAX7219 pin - Arduino pin --

CLK -> 10

CS -> 9

DIN -> 8

For more help setting up your 8x8 LED Matrix, check out-

http://www.instructables.com/id/LED-Matrix-with-Arduino/


Most 8x8 Matrix array code online relies on set hex code to display designs; smiley, letters, numbers.

So I've set up an 8x8 array variable that will turn on and off lights based on a 0-1 value

Convert the array into hex to send to the led driver on the 8x8 LED Matrix module.


