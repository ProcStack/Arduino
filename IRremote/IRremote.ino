////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IR Remote codes for "Keyes" remote and IR module                                                       //
// Written by Kevin Edzenga                                                                               //
//                                                                                                        //
// These are the codes I've personally found for remotes listed when searching ebay for-                  //
// "IR Receiver Module"                                                                                   //
//                                                                                                        //
// Needed headers---                                                                                      //
// https://github.com/adafruit/Adafruit_NeoPixel                                                          //
//                                                                                                        //
// You'll need the IRremote header which can be found here -                                              //
// http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html                              //
//                                                                                                        //
// I'm revisiting this code, that I'm using string's instead of int's in a switch statment is             //
//  using more of the rom than desired.                                                                   //
// Also, there is a non 'keyes' labeled remote I have as well, I'll expand the checks with those values.  //
// For now, this is more of a cheat sheet to get started.                                                 //
//                                                                                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <IRremote.h>

IRrecv irrecv(11);
decode_results results;

String optionPrev; // This is set when a held down button is registered

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver  
}

void loop() {
    IRFeedback(20);
}

void IRFeedback(uint8_t wait) {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
    String hex =String(results.value);
    int optionHit=-1;
    if(hex=="4294967295"){ //Repeat
      hex=optionPrev;
    }else{
      optionPrev=hex;
    }
    Serial.print(hex+"\n");
    Serial.print("\n\n");
    if(hex=="16736925"){ //Up
    }
    if(hex=="16720605"){ //Left
    }
    if(hex=="16712445"){ //Ok
    }
    if(hex=="16761405"){ //Right
    }
    if(hex=="16754775"){ //Down
    }
    if(hex=="16738455"){ //1
    }
    if(hex=="16750695"){ //2
    }
    if(hex=="16756815"){ //3
    }
    if(hex=="16724175"){ //4
    }
    if(hex=="16718055"){ //5
    }
    if(hex=="16743045"){ //6
    }
    if(hex=="16716015"){ //7
    }
    if(hex=="16726215"){ //8
    }
    if(hex=="16734885"){ //9
    }
    if(hex=="16728765"){ // *
    }
    if(hex=="16730805"){ //0
    }
    if(hex=="16732845"){ //#
    }
  }
}
