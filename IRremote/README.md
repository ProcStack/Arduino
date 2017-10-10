A cheat sheet for signals from the Keyes IR remote that can be found on ebay.

Using the IR Remote library from-
http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html

https://github.com/z3t0/Arduino-IRremote


I'm noticing quite a wide range of recieved signals from the IR module I have, even within a single remote used.  Fully aware how finicky cheap chinese products can be, I will be investigating why the signal varies so much. (Kinda wonder if its as simple as putting a small cap inline to reduce noise in power rail.)


I'll be looking into z3t0's IRremote.h and IRremoteint.h for this.  I doubt its fault in the header.  But currious by the wide range of returned values from the IR module.


I did notice quite a lot of code for diversity in tv remotes to work with this header.  Something that may be using a bit more rom than desired.  On a Pro Mini, seeing used rom values of 24% (a Switch statement for these values, with a woverflow warning, yeah, I need to address this) or 30% (with the If statements you can see in the current itteration of the ino file.)


If I actually come up with something, I'll send a pull request to z3to's main git repository for IRremote.  This will be for the sole intention of slimming down the code for smaller Atmel chipsets.
