

#include <SoftwareSerial.h>
#include "McBrain.h"


//  servo motor
const int chainRpin = 3; // RIGHT ARM
const int chainLpin = 5; // LEFT ARM
McBrain chainR(chainRpin); //  brain obj controls a chain on pin 3 RIGHT
McBrain chainL(chainLpin); //  brain obj controls a chain on pin 5 LEFT


void setup() {
   Serial.begin(9600);
            
  // set up pin # for control
  pinMode(chainRpin, OUTPUT); 
  pinMode(chainLpin, OUTPUT);

  //"Discover" all the modules (make them blue-colored instead of green-colored)
  for (int i = 0; i < 40; i++) {
    chainR.communicate();
    chainL.communicate();
  }
  //delay to be sure that all modules are ready
  delay(1000);
  Serial.println("servo ready");
}



void testChain(McBrain chain) {
  for (int servo = 0; servo < 3; servo++) {  // test 3 servos 
    for (int pos = 20; pos < 220; pos=pos+10) {  // move in 10 step increment 
      chain.setServoPosition(servo, pos);
      chain.communicate();
      delay(2000);
      if ( Serial.read()=='0') exit(0);  // interrup to exit 
      }
   }   
}



void loop() {

  testChain(chainR);
  testChain(chainL);

  if ( Serial.read()=='0') exit(0);  // interrup to exit 
}


