


#include "Arduino.h"
#include "McBrain.h"



/******    Initialization of the "McBrain" to a specific I/O pin  *****/
McBrain :: McBrain(int pmwPin){
  pinMode(pmwPin, OUTPUT);
  _pmwPin = pmwPin;
  
  for (int x = 0; x < 4; x++){
    outputByte[x] = 0xFE;   // initialize output data
    moduleType[x] = '_';  // module type S, L, _ nothing
  }
}



/******    Methods to return specific internal bytes  *****/

byte McBrain :: outputByteInfo(int num){
	return outputByte[num];
}

byte McBrain :: inputByteInfo(){
	return inputByte;
}

byte McBrain :: inputBytesInfo(int modNum){

		return inputBytes[modNum];
	
}

byte McBrain :: checkSumByteInfo(){
	return checkSum;
}

char McBrain :: moduleTypeInfo(int num){
  return moduleType[num];
}

byte McBrain :: moduleNumInfo(){
  return printModNum;
}





/*   setServoColor(int servoNum, byte color)  ->  sets the color of the Meccano Smart Servo module
The byte SERVONUM refers to the order of the servo in the chain.  The first servo plugged into your Arduino is 0.  
The next servo is 1.  The third servo is 2.   The last servo in a chain of 4 servos is 3.

The byte COLOR is a value  0xF0 - 0xF7 which has 8 possible colors (including all off and all on)
 0xF7  =  R, G,B – all On
 0xF6  =  G, B  - On;   R – Off
 0xF5  =  R, B  - On;   G – Off
 0xF4  =  B  - On;   R, G – Off
 0xF3  =  R,G – On;   B - Off
 0xF2  =  G  - On;   R, B – Off
 0xF1  =  R – On,   G, B – Off
 0xF0  =  R, G,B  – all Off

For example, if you want to set the servo at position 0 to Red and the servo at position 2 to Blue-Green, you would send the following two commands
setServoColor(0,0xF1)
setServoColor(2,0xF6)

  end  */

void McBrain :: setServoColor(int servoNum, byte color){
	if(moduleType[servoNum] == 'S'){
		outputByte[servoNum] = color;
	}
}




/* setServoPosition(int servoNum, byte pos)  ->   sets a specific servo to a certain position 
The byte SERVONUM refers to the order of the servo in the chain.  The first servo plugged into your Arduino is 0.  
The next servo is 1.  The third servo is 2.   The last servo in a chain of 4 servos is 3.

The byte POS refers to the servo position  0x00 - 0xEF, which equals to a full 180 degree range.   
0x00 = full clockwise
0xEF = full counter clockwise

  end */

void McBrain :: setServoPosition(int servoNum, byte pos){
	int servoPos = 0;
	if(moduleType[servoNum] == 'S'){
	
		if(pos < 0x18){
			servoPos = 0x18;
		}else if(pos > 0xE8){
			servoPos = 0xE8;
		}else{
			servoPos = pos;
		}
		
	outputByte[servoNum] = servoPos;
	}		
}



/* setServotoLIM(int servoNum)  ->   sets a specific servo to LIM mode
The byte SERVONUM refers to the order of the servo in the chain.  The first servo plugged into your Arduino is 0.  
The next servo is 1.  The third servo is 2.   The last servo in a chain of 4 servos is 3.

LIM stands for Learned Intelligent Movement.  It is a special mode where the servo IC stops driving the motor and just sends back the position of the servo.

  end */


void McBrain :: setServotoLIM(int servoNum){
	if(moduleType[servoNum] == 'S'){
		outputByte[servoNum] = 0xFA;
	}
	
}




/* getServoPosition(int servoNum)  ->   returns a byte that is the position of a specific servo
The byte SERVONUM refers to the order of the servo in the chain.  The first servo plugged into your Arduino is 0.  
The next servo is 1.  The third servo is 2.   The last servo in a chain of 4 servos is 3.

First you must set the specific servo to LIM mode.  Then use this command.
The returned byte POS is the servo's position  0x00 - 0xEF, which equals to a full 180 degree range.   
0x00 = full clockwise
0xEF = full counter clockwise

  end */

byte McBrain :: getServoPosition(int servoNum){
  int temp = 0;  // last  input module #
  if(moduleType[servoNum] == 'S'){
        if (moduleNum > 0){
          temp = moduleNum - 1;
        }
        else{
          temp = 0;
        }

        if(temp == servoNum){
          return inputByte;
        }
    }
    return 0x00; // not valid reading
}



/*    communicate()  -  this is the main method that takes care of initializing, sending data to and receiving data from Meccano Smart modules

  The datastream consists of 6 output bytes sent to the Smart modules   and  one return input byte from the Smart modules.   
  Since there can be a maximum of 4 Smart modules in a chain, each module takes turns replying along the single data wire.

  Output bytes:   
      0xFF - the first byte is always a header byte of 0xFF
      Data 1 -  the second byte is the data for the Smart module at the 1st position in the chain
      Data 2 -  the third byte is the data for the Smart module at the 2nd position in the chain
      Data 3 -  the fourth byte is the data for the Smart module at the 3rd position in the chain
      Data 4 -  the fifth byte is the data for the Smart module at the 4th position in the chain 
      Checksum  -  the sixth byte is part checksum, part module ID.  The module ID tells which of the modules in the chain should reply
   end  */
void McBrain :: communicate(){
		

    // send 6-byte packet to chain    
    sendByte(0xFF);                         // send header
  
    for (int x = 0; x < 4; x++){            // send 4 data bytes
      sendByte(outputByte[x]);
    }

    checkSum = calculateCheckSum(outputByte[0], 
                      			 outputByte[1],
								 outputByte[2],
								 outputByte[3]);	
                       
    sendByte(checkSum);                    // send checksum

    // get response back from the module indexed by moduleNum var
    inputByte = receiveByte();
    inputBytes[moduleNum]=inputByte; // store recv byte from a module -CS

    // process module response
    if (inputByte == 0xFE){                 // if received 0xFE "exist", then request module type 
      outputByte[moduleNum] = 0xFC;         // mark output byte for this module number for next invocation, request type
    }
  
    if (inputByte == 0x01 && moduleType[moduleNum] == '_'){  // if received back 0x01 (module ID is a servo), then change servo color to Blue
    	outputByte[moduleNum] = 0xF4;  //  Blue
    	moduleType[moduleNum] = 'S';
    }

	
    if (inputByte == 0x02 && moduleType[moduleNum] == '_'){  // if received back 0x02 (module ID is a LED), then change servo color to Blue
    	  outputByte[moduleNum] = 0xF4;  //  Blue 
        moduleType[moduleNum] = 'L'; 
    }

    if(inputByte == 0x00){  // if received back 0, reset output data to start over?
      for(int x = moduleNum; x < 4; x++){
          outputByte[x] = 0xFE; 
          moduleType[x] = '_';  
        }

    }
	
    printModNum = moduleNum;  // store current module # just processed
    moduleNum++;              // increment to next module ID
    if (moduleNum > 3) {
      	moduleNum = 0;
    } 
	
	delay(10); //  response delay

}


void McBrain :: sendByte(byte servoData){
  pinMode(_pmwPin, OUTPUT);                                     
  digitalWrite(_pmwPin,LOW);
  delayMicroseconds(bitDelay);                  // start bit - 417us LOW

  for (byte mask = 00000001; mask>0; mask <<= 1) {   // iterate through bit mask
    if (servoData & mask){                           // if bitwise AND resolves to true
      
      digitalWrite(_pmwPin,HIGH);              // send 1
      
    }else{                                      // if bitwise and resolves to false
      
      digitalWrite(_pmwPin,LOW);               // send 0
    }
    delayMicroseconds(bitDelay);                //delay
  }
  
  digitalWrite(_pmwPin,HIGH);
  delayMicroseconds(bitDelay);         // stop bit - 417us HIGH
  
  digitalWrite(_pmwPin,HIGH);
  delayMicroseconds(bitDelay);         // stop bit - 417us HIGH
  
}

byte McBrain :: receiveByte(){
  byte tempByte;
  tempByte = 0;
  
  pinMode(_pmwPin, INPUT);
  
  delay(1.5);
  
  for (byte mask = 00000001; mask>0; mask <<= 1) {   // iterate through bit mask
         
    if (pulseIn(_pmwPin, HIGH, 2500) > 400){
      tempByte = tempByte | mask;
    }
  }
  return tempByte;  
}

byte McBrain :: calculateCheckSum(byte Data1, byte Data2, byte Data3, byte Data4){
  int CS;
  CS =  Data1 + Data2 + Data3 + Data4;  // ignore overflow
  CS = CS + (CS >> 8);                  // right shift 8 places
  CS = CS + (CS << 4);                  // left shift 4 places
  CS = CS & 0xF0;                     // mask off top nibble
  CS = CS | moduleNum;
  return CS;
}

/*  END  OF CODE */ 
