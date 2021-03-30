/*  File name:    arduino-ws2812.ino
 *  Description:  Arduino code to control a number of LED strips using individual ws2812 programmable LEDs.
 *  Author:       Chris Saiko
 *  Email:        chris.saiko@gmail.com

    This file is part of desk-led.

    desk-led is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    desk-led is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with desk-led.  If not, see <https://www.gnu.org/licenses/>.
    
 */

#include <FastLED.h>

#define Z1_NUM_LEDS 6
#define Z1_DATA_PIN 4

#define Z2_NUM_LEDS 5
#define Z2_DATA_PIN 5

#define Z3_NUM_LEDS 4
#define Z3_DATA_PIN 6

CRGB Z1leds[Z1_NUM_LEDS];
CRGB Z2leds[Z2_NUM_LEDS];
CRGB Z3leds[Z3_NUM_LEDS];

// maybe useful?
CRGB *Z[3] = {&Z1leds[0], &Z2leds[0], &Z3leds[0]};
int numLEDs[3] = {Z1_NUM_LEDS, Z2_NUM_LEDS, Z3_NUM_LEDS};


//each command contains exactly eight bytes
// start  (always 0x7b)
// zone   (0x01 - 0x07)
// mode1  (number depends on how many modes I can fit into 30720 bytes)
// mode2  (dependent on mode1 byte supplied)
// red    (0 - 255)
// green  (0 - 255)
// blue   (0 - 255)
// end    (always 0x7d)
int zone = 0;
int modeOne = 0;
int modeTwo = 0;
int red = 0;
int green = 0;
int blue = 0;
long rgb;
int serialData[8] = {0, 0, 0, 0, 0, 0, 0, 0};
bool dataRecv = false;




void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);     // opens serial port, sets data rate to 115200 bps
  //Serial.println("reset");
  FastLED.addLeds<NEOPIXEL, Z1_DATA_PIN>(Z1leds, Z1_NUM_LEDS);  // zone one, GRB ordering is assumed
  FastLED.addLeds<NEOPIXEL, Z2_DATA_PIN>(Z2leds, Z2_NUM_LEDS);  // zone two, GRB ordering is assumed
  FastLED.addLeds<NEOPIXEL, Z3_DATA_PIN>(Z3leds, Z3_NUM_LEDS);  // zone three, GRB ordering is assumed
  FastLED.setBrightness(10);
  void fadeall();
  ledStatus(Z1leds,0);
}

void fadeall() { for(int i = 0; i < Z1_NUM_LEDS; i++) { Z1leds[i].nscale8(250); } }

void ledStatus(CRGB *zone, int statCode){

  // Set status colors for a zone(string) of LEDs
  // Used mostly for debugging command parsing and serial communication
  // statCode 0: Blue,  Startup complete
  // statCode 1: Green, String OK
  // statCode 2: Red,   String error

  //Zone One (Room edge)
  //Zone Two (Desk interior)
  //Zone Three (Above computer)
  zone[0] = CRGB::Red;
  if (statCode == 0) zone[1] = CRGB::Blue;
  else if (statCode == 1) zone[1] = CRGB::Green;
  else if (statCode == 2) zone[1] = CRGB::Red;

  FastLED.show();
  
} //end ledStatus


void zoneColor(int zoneMask, CRGB *zone[], int *numled, long rgb){
  
  //ledStatus(Z1leds,1);

  //iterate through the three zones
  for (int z = 0; z < 3; z++){
    //zone hit
    if ((zoneMask & 0x01) == 1){
      for (int i = 0; i < numled[z]; i++){
        zone[z][i] = rgb;
      }
    }
    //next zone
    zoneMask = zoneMask >> 1;
    
  }



  FastLED.show();

  
} //end zoneColor


  
void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    //wait a bit in case command is not fully sent
    delay(5);
    // assume all eight bytes are ready to be read
    // read the incoming byte:
    for (int i = 0; i < 8; i++){
      serialData[i] = Serial.read();
    }
    dataRecv = true;
  }

  //check for start and end byte
  if (dataRecv){
    if ((serialData[0] != 0x7b) || (serialData[7] != 0x7d)){
      //data comms failure
      ledStatus(Z1leds,2);
      //flush serialData buffer
      for (int j = 0; j < 8; j++){
         serialData[j] = 0;
      }
    }
    else {
      //data comms success
      //begin parsing data
      zone = (serialData[1] & 0x07); //mask off only the zones we care about
      modeOne = serialData[2];
      modeTwo = serialData[3];
      red = serialData[4];
      green = serialData[5];
      blue = serialData[6];
      rgb = red;
      rgb = rgb << 8;
      rgb = rgb + green;
      rgb = rgb << 8;
      rgb = rgb + blue;
    }
    dataRecv = false; //reset dataRecv
  }

  /* Modes:
   *  1:  color
   *  2:  
  */ 
  if (modeOne == 1){
    zoneColor(zone, Z, numLEDs, rgb);
  }
  

//  cylon FastLED example
//  if (incomingByte == 126){
//    static uint8_t hue = 0;
//    //Serial.print("x");
//    // First slide the led in one direction
//    for(int i = 0; i < NUM_LEDS; i++) {
////      if (i != 0){
////        leds[i-1] = CHSV(hue, 255, 255/2);
////      }
//      // Set the i'th led to red
//      leds[i] = CHSV(hue++, 255, 255);
//      // Show the leds
//      FastLED.show(); 
//      // now that we've shown the leds, reset the i'th led to black
//      //leds[i] = CRGB::Black;
//      leds[i] = CHSV(hue, 255, 255/2);
//      if (i != 0){
//        leds[i-1] = CRGB::Black;
//      }
//      //fadeall();
//      // Wait a little bit before we loop around and do it again
//      delay(50);
//    }
//    //Serial.print("x");
//  
//    // Now go in the other direction.  
//    for(int i = (NUM_LEDS)-1; i >= 0; i--) {
////      if (i != (NUM_LEDS - 1)){
////        leds[i+1] = CHSV(hue, 255, 255/2);
////      }
//      // Set the i'th led to red 
//      leds[i] = CHSV(hue++, 255, 255);
//      // Show the leds
//      FastLED.show();
//      // now that we've shown the leds, reset the i'th led to black
//      leds[i] = CHSV(hue, 255, 255/2);
////      leds[i] = CRGB::Black;      
//      if (i != (NUM_LEDS - 1)){
//        leds[i+1] = CRGB::Black;
//      }
//
//      //fadeall();
//      // Wait a little bit before we loop around and do it again
//      delay(50);
//    }
//  }


}
