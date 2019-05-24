#include <MeccaBrain.h>


//Pins to connect Meccanoids' servos; pins can be any digital pins, not necessary PWM
const int chainPin1 = 3;

//each brain obj controls a chain 
MeccaBrain chain1(chainPin1); 


void setup() {
  // set up pin # for control
  pinMode(chainPin1, OUTPUT);
  Serial.begin(9600);

  //"Discover" all the modules (make them blue-colored instead of green-colored)
  //for some unknown reason, I have to repeat it from time to time
  /*
  for (int i = 0; i < 50; i++)
  {
    chain1.communicate();
    //chain2.communicate();
    //chain3.communicate();
  }
  */
  //delay to be sure that all modules are ready
  //delay(2000);
}

int  lc =0;

void loop() {

 //chain1.setServoPosition(0, 255);
 //chain1.setServoPosition(1, 0);

 Serial.print("before com loop #"); Serial.print(lc); 
 Serial.print(" entering module#");  Serial.print( chain1.moduleNumInfo(), DEC );
 Serial.print(" : "); 

 // print out the entire 4-byte packet data portion 
 for (int i=0; i<4 ; i++) { Serial.print( chain1.outputByteInfo(i), HEX);Serial.print(" ");}
 Serial.println();
 
 chain1.discoverModules();
 
 lc++;


 if (Serial.read()=='1') exit(0); // quit
 
 delay(1000); 

}

