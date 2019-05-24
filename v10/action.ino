// low level arm-joint level actions
// low level sound actions 


#include <SoftwareSerial.h>
#include "McBrain.h"

// servo lib ***************

// set the chain to a fixed pos, for initial start pos
void setPos(McBrain chain, int pos[]){
  for  (int j=0; j<3 ; j++) {
       chain.setServoPosition(j, pos[j]);
       chain.communicate();    
       delay(15);
  }
}


// move from positin 1 to 2
// use float pt num to get smooth result at end
void move(McBrain chain, int p1[], int p2[], int steps) {
  float x1= p1[0];
  float x2= p2[0];
  float xInc = (x2-x1)/steps; // increment

  float y1= p1[1];
  float y2= p2[1];
  float yInc = (y2-y1)/steps; // increment

  float z1= p1[2];
  float z2= p2[2];
  float zInc = (z2-z1)/steps; // increment
  
  for  (int j=0; j<steps ; j++) {
    // x
    chain.setServoPosition(0, (int) x1);
    chain.communicate();
    x1=x1+xInc;
    
    // y
    chain.setServoPosition(1, (int) y1);
    chain.communicate();
    y1=y1+yInc;

    // z
    chain.setServoPosition(2, (int) z1);
    chain.communicate();
    z1=z1+zInc;
     
     delay(15);
    }

  // correct rounding error at final pos
  chain.setServoPosition(0, (int) x2);
  chain.setServoPosition(1, (int) y2);
  chain.setServoPosition(2, (int) z2);
  chain.communicate();
}



// synchronized move for 2 chains 
// use float pt num to get smooth result at end
void sync_move(McBrain chainR, McBrain chainL, int R1[], int R2[], int L1[], int L2[], int steps) {
  // RIGHT arm
  float Rx1= R1[0];
  float Rx2= R2[0];
  float RxInc = (Rx2-Rx1)/steps; // increment

  float Ry1= R1[1];
  float Ry2= R2[1];
  float RyInc = (Ry2-Ry1)/steps; // increment

  float Rz1= R1[2];
  float Rz2= R2[2];
  float RzInc = (Rz2-Rz1)/steps; // increment
   // LEFT arm
  float Lx1= L1[0];
  float Lx2= L2[0];
  float LxInc = (Lx2-Lx1)/steps; // increment

  float Ly1= L1[1];
  float Ly2= L2[1];
  float LyInc = (Ly2-Ly1)/steps; // increment

  float Lz1= L1[2];
  float Lz2= L2[2];
  float LzInc = (Lz2-Lz1)/steps; // increment 



  for  (int j=0; j<steps ; j++) {
    // x
    chainR.setServoPosition(0, (int) Rx1);
    chainR.communicate();
    Rx1=Rx1+RxInc;

    chainL.setServoPosition(0, (int) Lx1);
    chainL.communicate();
    Lx1=Lx1+LxInc;

    // y
    chainR.setServoPosition(1, (int) Ry1);
    chainR.communicate();
    Ry1=Ry1+RyInc;

    chainL.setServoPosition(1, (int) Ly1);
    chainL.communicate();
    Ly1=Ly1+LyInc;

    // z
    chainR.setServoPosition(2, (int) Rz1);
    chainR.communicate();
    Rz1=Rz1+RzInc;

    chainL.setServoPosition(2, (int) Lz1);
    chainL.communicate();
    Lz1=Lz1+LzInc;     

     delay(15);
    }

  // correct rounding error at final pos
  chainR.setServoPosition(0, (int) Rx2);
  chainR.setServoPosition(1, (int) Ry2);
  chainR.setServoPosition(2, (int) Rz2);
  chainR.communicate();
}



//** speech lib

// speech module
#define rxPin   10  // Serial input (connects to Emic 2's SOUT pin)
#define txPin   11  // Serial output (connects to Emic 2's SIN pin)
SoftwareSerial emicSerial =  SoftwareSerial(rxPin, txPin);

void speech_setup()  // Set up code called once on start-up
{
  // define pin modes
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT); 
  // set the data rate for the SoftwareSerial port
  emicSerial.begin(9600);
  emicSerial.print('\n');             // Send a CR in case the system is already up
  while (emicSerial.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);                         // Short delay
  emicSerial.flush();                 // Flush the receive buffer
  emicSerial.print("P0\n");          // use DECtalk parser for sing a song 
}

void speech_say(String s)  // Main code, to run repeatedly
{
  // Speak some text
  emicSerial.print('S');
  emicSerial.print(s);  // Send the desired string to convert to speech
  emicSerial.print('\n');
  while (emicSerial.read() != ':');   // Wait here until ":" indicating ready for command
  delay(500);    // 1/2 second delay
}


static String songDaisy = 
"[:phone arpa speak on][:rate 200] [:n0] [dey<650,22>ziy<600,19> dey<650,15>ziy<600,10>gih<200,12>v miy<200,14> yurr<200,15> ae<400,12>nsax<200,15>r duw<750,10> _<400,10>][:n0]";

void speech_sing()  // Main code, to run repeatedly
{ 
  //emicSerial.print("P0\n"); // stored song Daisy
  emicSerial.print('S');
  emicSerial.print(songDaisy);
  emicSerial.print('\n');
  while (emicSerial.read() != ':');   // Wait here until ":" indicating ready for command
  delay(500);    // 1/2 second delay
}

