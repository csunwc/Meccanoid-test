#include <MeccaBrain.h>


//Pins to connect Meccanoids' servos; pins can be any digital pins, not necessary PWM
const int chainPin1 = 3;

MeccaBrain chain1(chainPin1); //each brain obj controls a chain 




void setup() {
  // set up pin # for control
  pinMode(chainPin1, OUTPUT);
  Serial.begin(9600);

  //"Discover" all the modules (make them blue-colored instead of green-colored)
  //for some unknown reason, I have to repeat it from time to time
  
  for (int i = 0; i < 50; i++)
  {
    chain1.communicate();
    //chain2.communicate();
    //chain3.communicate();
  }
  
  //delay to be sure that all modules are ready
  //delay(2000);
}


void loop() {

 chain1.setServoPosition(0, 0);
 chain1.setServoPosition(1, 255);
 chain1.communicate();
 delay(5000);

 chain1.setServoPosition(0, 127);
 chain1.setServoPosition(1, 127);
 chain1.communicate();
 delay(5000); 

 chain1.setServoPosition(0, 255);
 chain1.setServoPosition(1, 0);
 chain1.communicate();
 delay(5000); 

      
}

