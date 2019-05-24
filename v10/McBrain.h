
/*
       LEGAL NOTICE:
      Meccano™, McBrain™, MeccanoTech™ and Meccanoid™ are trademarks of Meccano.  All rights reserved.     
      The information contained in this document is Proprietary to Spin Master Ltd.   
      This information should not be used in the creation and sale of any commercial product.”
*/
#ifndef McBrain_h
#define McBrain_h

#include "Arduino.h"

class McBrain{
	public:
		McBrain(int pin);
		
		byte outputByteInfo(int num);

		byte inputByteInfo();
		byte inputBytesInfo(int servoNum);

		byte checkSumByteInfo();

		byte servoPositionsInfo(int servoNum);

		char moduleTypeInfo(int num);
		byte moduleNumInfo();


		void setServoColor(int servoNum, byte color);
		void setServoPosition(int servoNum, byte pos);
		void setServotoLIM(int servoNum);
		byte getServoPosition(int servoNum);
		
		void communicate();		
		void sendByte(byte servoData);
		byte receiveByte();
		byte calculateCheckSum(byte Data1, byte Data2, byte Data3, byte Data4);
		
	private:
    int bitDelay = 417; // 417 us delay in start bit
    
		int _pmwPin; // pin #

		int moduleNum = 0;  // the next module to communicate
		int printModNum = 0; // the last module just communcated with


        // packet output from controlle to smart module
		byte header = 0xFF; // packet header
		byte outputByte[4];  // packet 4 data bytes
        byte checkSum;

        // data received from smart module
		byte inputByte;
		byte inputBytes[4];

		char moduleType[4];  //  module type S, L, _ nothing for each of the 4 pos in chain
		
};

#endif
