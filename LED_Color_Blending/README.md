LED NeoPixel Value Blending

Written by Kevin Edzenga
 
  
 
 
    
This file is an example of doing cross cycle LED value blending for Adafruit NeoPixels


The LED color blending doesn't need to be for Adadfruit NeoPixels, this is just the application I needed the scripting for.

The line -   strip.setPixelColor(i, strip.Color(  int(fadeR),   int(fadeG), int(fadeB)));

Is where individual NeoPixel values are set, prior to displaying on the strip


The line -   strip.show();

Is where the NeoPixels values are set, you can use ' byte fadeR, fadeG, fadeB; ' variables how ever you'd like once blended.


I've since changed my method behind blending.

This still uses a more brute force approach.

Updates will come as I advance this code base.


For ease of use, NeoPixels are amazing-

https://www.adafruit.com/category/168
 
Needed headers---

https://github.com/adafruit/Adafruit_NeoPixel


For the breadboard layout, 300-500 ohm resistor on pin 6 to SPI data on the NeoPixels

1000-4700 uf 16v capacitor should be used on VCC / GND

http://www.studiopieters.nl/wp-content/uploads/2017/01/Knowledge-Base-%E2%80%93-NeoPixel-Strip_bb.png 
