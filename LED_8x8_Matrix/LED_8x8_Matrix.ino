///////////////////////////////////////////////////////////////
// Animated 8x8 LED Matrix                                   //
// Adapted by Kevin Edzenga                                  //
//                                                           //
// This is for the - 5-pin MAX7219 8x8 Matrix Array          //
// MAX7219 pin - Arduino pin --                              //
//   CLK -> 10                                               //
//   CS -> 9                                                 //
//   DIN -> 8                                                //
// For more help setting up your 8x8 LED Matrix, check out-  //
// http://www.instructables.com/id/LED-Matrix-with-Arduino/  //
//                                                           //
// Most 8x8 Matrix array code online relies on set hex code  //
//   to display designs; smiley, letters, numbers.           //
// So I've set up an 8x8 array variable that will turn on    //
//   and off lights based on a 0-1 value                     //
// Convert the array into hex to send to the led driver on   //
//   the 8x8 LED Matrix module.                              //
//                                                           //
// This script will run this animation-                      //
// https://www.youtube.com/watch?v=bccOX-seROI               //
//                                                           //
///////////////////////////////////////////////////////////////


#include <EEPROM.h> // Used to generate a 'random' initial value to have unique animations ever start

/*Port Definitions*/
int Max7219_pinCLK = 10;
int Max7219_pinCS = 9;
int Max7219_pinDIN = 8;
double runner=-1;
int prevCount=5;
// Store trailing values on the swirl function, so its a line, rather than a single lit LED
double xPrev[5]={-1,-1,-1,-1,-1};  // Previous swirl x
double yPrev[5]={-1,-1,-1,-1,-1};  // Previous swirl y
double xBPrev[5]={-1,-1,-1,-1,-1}; // Previous secondary swirl x
double yBPrev[5]={-1,-1,-1,-1,-1}; // Previous secondary swirl y
int prep=1;
double value;
 
unsigned char disp[2][8]={ // Set character
{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0
{0x08,0x18,0x28,0x08,0x08,0x08,0x08,0x3E}//1
};

//  Display and orientation of the board to the 8x8 array variable---
//       ___[0][0]        [0][7]
//      | M | * * * * * * * *
// -VCC | A | * * * * * * * *
// -GND | X | * * * * * * * *
// -DIN | 7 | * * * * * * * *
// -CS  | 2 | * * * * * * * *
// -CLK | 1 | * * * * * * * *
//      | 9 | * * * * * * * *
//      |___| * * * * * * * *
//          [7][0]        [7][7]

char rowVals[8][8]={  // 8x8 array to set on/off LED values
{0,0,0,0,0,0,0,0}, //0 Top row
{0,0,0,0,0,0,0,0}, //1
{0,0,0,0,0,0,0,0}, //2
{0,0,0,0,0,0,0,0}, //3
{0,0,0,0,0,0,0,0}, //4
{0,0,0,0,0,0,0,0}, //5
{0,0,0,0,0,0,0,0}, //6
{0,0,0,0,0,0,0,0}  //7 Bottom row
};

char rowValUpdated[8][8]={  // For the on/off neighbor animation
{0,0,0,0,0,0,0,0}, //0 Top row
{0,0,0,0,0,0,0,0}, //1
{0,0,0,0,0,0,0,0}, //2
{0,0,0,0,0,0,0,0}, //3
{0,0,0,0,0,0,0,0}, //4
{0,0,0,0,0,0,0,0}, //5
{0,0,0,0,0,0,0,0}, //6
{0,0,0,0,0,0,0,0}  //7 Bottom row
};
 
// Convert the array to a format the MAX Matrix array can understand
char arrayToByte(const char inputArray[8]){
  char result = 0;
  int idx;
  for (idx=0;idx<8;++idx){
    result |= (inputArray[7-idx] << idx);
  }
  return result;
}
 
// Write to the MAX7219
void Write_Max7219_byte(unsigned char DATA) {   
  unsigned char i;
  digitalWrite(Max7219_pinCS,LOW);		
  for(i=8;i>=1;i--){
    digitalWrite(Max7219_pinCLK,LOW);
    digitalWrite(Max7219_pinDIN,DATA&0x80); // Extracting bit data
    DATA = DATA<<1;
    digitalWrite(Max7219_pinCLK,HIGH);
  }                                 
}
 
// Main function to use unsigned char data to write to the MAX7219
void Write_Max7219(unsigned char address,unsigned char dat){
  digitalWrite(Max7219_pinCS,LOW);
  Write_Max7219_byte(address);           //address，code of LED
  Write_Max7219_byte(dat);               //data，figure on LED 
  digitalWrite(Max7219_pinCS,HIGH);
}

// Init the MAX7219
void Init_MAX7219(void){
 Write_Max7219(0x09, 0x00);       //decoding ：BCD
 Write_Max7219(0x0a, 0x03);       //brightness 
 Write_Max7219(0x0b, 0x07);       //scanlimit；8 LEDs
 Write_Max7219(0x0c, 0x01);       //power-down mode：0，normal mode：1
 Write_Max7219(0x0f, 0x00);       //test display：1；EOT，display：0
}
 
// Prep the arduino
void setup(){
  pinMode(Max7219_pinCLK,OUTPUT);
  pinMode(Max7219_pinCS,OUTPUT);
  pinMode(Max7219_pinDIN,OUTPUT);
  Serial.begin(9600);
  value = EEPROM.read(0);  // Read previously generated value
  value=(sin(value))*2000; // Offset generated value; based on previously stored value
  EEPROM.write(0, int(value)%255); // Store the value for the next start up
  delay(50);
  Init_MAX7219();
}
 
// Loop the set functions for the Arduino to address the MAX7219 LED Matrix
void loop(){
  // Use this function to test some set displays using hex or the 8x8 variable array
  // Good to check orientation of your LED array
  // test();

  if(int(floor(runner/200)) %2 == 1){  // Switch between swirling and toggling neighbor lights
    othello();
  }else{
    swirl();
    prep=1;
  }

}

// This function runs 3 different test displays, good for checking orientation of your module
void test(){ 
  byte curBin;
  int i;
  runner++;
  // Notice the 1's and 0's in the array below
	char testRowVals[8][8]={
	{1,0,0,0,0,0,0,0}, //0
	{0,1,0,0,0,0,0,0}, //1
	{0,0,1,0,0,0,0,0}, //2
	{0,0,0,1,0,0,0,0}, //3
	{0,0,0,0,1,0,0,0}, //4
	{0,0,0,0,1,0,0,0}, //5
	{0,0,0,0,1,0,0,0}, //6
	{0,0,0,0,1,0,0,0}  //7
	};

  for(i=0;i<8;i++){
    int switchVal=int(runner)%3;
    switch(switchVal){
      case 0:
        curBin=arrayToByte(testRowVals[i]);
        break;
      case 1:
        curBin=disp[0][i]; // Read the hex of '0' from the disp variable set above
        break;
      case 2:
        curBin=disp[1][i]; // Read the hex of '1' from the disp variable set above
        break;
      default:
        curBin=arrayToByte(testRowVals[i]);
        break;
    }
    Write_Max7219((i+1),curBin);
  }
  delay(1500);
}

// This function generates two swirling lines to thicken the single LED visibility
void swirl(){ 
  byte curBin;
  char i,c,v;  // These should probably be ints
  double x,y,flipper;
  int exit=0;
  while(exit == 0){ // Get the front of the line to move
    runner++;
    flipper=sin(runner/13)*.5+.5;
    for(i=0;i<8;i++){ // Row
      for(c=0;c<8;c++){ // Column
          x=( (sin(runner/6+value)*cos(runner/10+value)))*flipper;
          y=( (cos(runner/6+value)*sin(runner/10+value)))*flipper;
          x=( (sin(runner/6+value)))*(1-flipper)+x;
          y=( (cos(runner/6+value)))*(1-flipper)+y;
          x=x*4+4;
          y=y*4+4;
          if( (floor(x)==i && floor(y)==c)){
            rowVals[i][c]=1;
            exit=1;
          }else{
            rowVals[i][c]=0;
          }
      }
    }
  }
  // Set visibility of the previously lit LEDs
  // These previous LED values are stored in xPrev, yPrev
  for(v=prevCount-1; v>=0; v--){
    for(i=0;i<8;i++){ // Row
      for(c=0;c<8;c++){ // Column
        if(floor(xPrev[v])==i && floor(yPrev[v])==c ){
          rowVals[i][c]=1;
        }
      }
    }
    if(v>0){
      xPrev[v]=xPrev[v-1];
      yPrev[v]=yPrev[v-1];
    }
  }
  xPrev[0]=x;
  yPrev[0]=y;
  
  // I'm doing the above functions again, but slightly offset to make the line appear thicker
  // There are better methods of doing this, but this is the brute force approach
  exit=0;
  while(exit == 0){
    runner++;
    flipper=sin(runner/13)*.5+.5;
    for(i=0;i<8;i++){ // Row
      for(c=0;c<8;c++){ // Column
          x=( (sin(runner/6+value)*cos(runner/10+value)))*flipper;
          y=( (cos(runner/6+value)*sin(runner/10+value)))*flipper;
          x=( (sin(runner/6+value)))*(1-flipper)+x;
          y=( (cos(runner/6+value)))*(1-flipper)+y;
          x=x*3+4;
          y=y*3+4;
          if( (floor(x)==i && floor(y)==c)){
            rowVals[i][c]=1;
            exit=1;
          }
      }
    }
  }
  for(v=prevCount-1; v>=0; v--){
    for(i=0;i<8;i++){ // Row
      for(c=0;c<8;c++){ // Column
        if(floor(xBPrev[v])==i && floor(yBPrev[v])==c ){
          rowVals[i][c]=1;
        }
      }
    }
    if(v>0){
      xBPrev[v]=xBPrev[v-1];
      yBPrev[v]=yBPrev[v-1];
    }
  }
  xBPrev[0]=x;
  yBPrev[0]=y;
  // Write to the Max7219
  for(i=0;i<8;i++){
    curBin=arrayToByte(rowVals[i]);
    Write_Max7219((i+1),curBin);
  }
  delay(80);
}


// This function will randomly start with one on light and turn on and off
//   neighboring lights until all have been flipped.
void othello(){ 
  byte curBin;
  char i,c,v;  // These should probably be ints
  int gen=-1;
  int localRun=0;
  int maxRun=3;
  runner+=3; // Making the steps larger makes the animation run faster
  if(int(runner)%20==0){
    prep=1;
    maxRun=int(random(runner+value)*4);
    for(v=prevCount-1; v>=0; v--){
        xPrev[v]=-1;
        yPrev[v]=-1;
    }
  }
  if(prep == 1){
    gen=(sin(runner*1.43+value)*.5+.5)*36;
    for(i=0;i<8;i++){ // Row
      for(c=0;c<8;c++){ // Column
        if(localRun==gen){
          rowVals[i][c]=1;
        }else{
          rowVals[i][c]=0;
        }
        localRun++;
      }
    }
    prep=0;
    for(i=0;i<8;i++){ // Row
      for(c=0;c<8;c++){ // Column
        rowValUpdated[i][c]=0;
      }
    }
  }
  // Lets toggle neighbouring LEDs while limiting any toggling to the 0-7 height and width of the matrix array itself. 
  localRun=0;
  for(i=0;i<8;i++){ // Row
    for(c=0;c<8;c++){ // Column
        if(localRun==gen){
          rowVals[i][c]=1;
        }
      // Limit any LEDs -1 or 8 to be toggled, stopping the toggling on/off at the edges
      if(rowVals[i][c] == 1){
        if(i-1 != -1){
          if(rowValUpdated[i-1][c]<=maxRun){
            rowVals[i-1][c]=(rowVals[i-1][c]+1)%2;
            rowValUpdated[i-1][c]++;
          }
        }
        if(i+1 != 8){
          if(rowValUpdated[i+1][c]<=maxRun){
            rowVals[i+1][c]=(rowVals[i+1][c]+1)%2;
            rowValUpdated[i+1][c]++;
          }
        }
        if(c-1 != -1){
          if(rowValUpdated[i][c-1]<=maxRun){
            rowVals[i][c-1]=(rowVals[i][c-1]+1)%2;
            rowValUpdated[i][c-1]++;
          }
        }
        if(c+1 != 8){
          if(rowValUpdated[i][c+1]<=maxRun){
            rowVals[i][c+1]=(rowVals[i][c+1]+1)%2;
            rowValUpdated[i][c+1]++;
          }
        }
      }
        localRun++;
    }
  }
  for(i=0;i<8;i++){
    curBin=arrayToByte(rowVals[i]);
    Write_Max7219((i+1),curBin);
  }
  delay(100);
}
