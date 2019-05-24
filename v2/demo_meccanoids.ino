#include <MeccaBrain.h>


//Pins to connect Meccanoids' servos; pins can be any digital pins, not necessary PWM
const int chainPin1 = 3;

//each brain obj controls a chain 
MeccaBrain chain1(chainPin1); 


void setup() {
  // set up pin # for control
  pinMode(chainPin1, OUTPUT);
  Serial.begin(9600);

  //"Discover" all the modules 
  for (int i = 0; i < 30; i++)
  {
     Serial.print("before com loop #"); Serial.print(i); 
     Serial.print(" entering module#");  Serial.print( chain1.moduleNumInfo(), DEC );
     Serial.print(" : "); 
     // print out the entire 4-byte packet data portion 
     for (int j=0; j<4 ; j++) { Serial.print( chain1.outputByteInfo(j), HEX);Serial.print(" ");}
         Serial.println();
     chain1.discoverModules();
  }

  //delay to be sure that all modules are ready
  delay(100);

  chain1.setServotoLIM(0); // set LIM mode on 1st servo
  //delay(1000);
}


byte pos =0;

void loop() {

 /*
     Serial.print(" talk to module#");  Serial.print( chain1.moduleNumInfo(), DEC );
     Serial.print(" : "); 
     // print out the entire 4-byte packet data portion 
     for (int j=0; j<4 ; j++) { Serial.print( chain1.outputByteInfo(j), HEX);Serial.print(" ");}
         Serial.println();
*/

 chain1.sendPacket(true);

// read back position
 pos = chain1.getServoPosition(0);
 if (pos != 0x00)  Serial.println(pos, HEX); // print out valid position

 if ( Serial.read()=='0') exit(0); 
 
 //delay(1000); 
}

