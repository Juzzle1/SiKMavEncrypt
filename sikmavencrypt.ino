//Simple MAVLINK message scrambler for use with SiK Telemetry Radios
//
//This does not guarantee any kind of genuine security, however would make it extremely difficult for
//someone to maliciously take control of an aircraft.
//
#include "Cape.h"
Cape newencrypt(


//Setup.................

//Set your encryption key and it's length here. Ensure this is the same at both ends of the radio link.

"mySecretKey1234",15


);
//Set the serial baud rate here

int telemBaud = 57600; 

//9600, 14400, 19200, 28800, 38400, 57600, or 115200
// End Setup


int boot, data, encDataAvailable, mavDataAvailable, encStrSize;
char encryptedIn[255];
char encryptedOut[255];
char decryptedIn[255];
char decryptedOut[255];
String mavMSG="";

//sent at the begining of every encrypted packet, 4 bytes
String encHeader="0#5#";

int msgLength=0;
int strSize=0;

void setup() {

Serial1.begin(telemBaud);
Serial2.begin(telemBaud);

}



void loop() {

//check radio (encrypted) interface for data
if (Serial2.available()>8) {
readRadio();

//decrypt and write data to mavlink interface
writeMav();
}

//check mavlink (unencrypted) interface for data
if (Serial1.available()>8) {
readMav();

//encypt and write data to radio
writeRadio();
}

//reset variables

mavMSG="";
msgLength=0;
strSize=0;

}


//read unencrypted mavlink messages
void readMav() {
int mavIndex=0;
int payloadLength=0;
int frameLength=255;
int startMsg=0;
mavDataAvailable=0;
mavMSG="";

  if (Serial1.available()) 
     {

      byte readByte = Serial1.read();          
          mavIndex++; //keeping a note of mavlink index
      
          //check for start of Mavlink message (0xFE)
         if (int(readByte) == 254)     
             {
              //ready to read mavlink packet
              startMsg=1;
              
              //add start packet to begining of mavlink string
              mavMSG += char(readByte);

             //read next byte to string. This is the payload size
            byte readByte = Serial1.read(); 
            payloadLength=int(readByte);   
            mavMSG += char(readByte);
            mavIndex++;

            //mavlink frame is 6 header bytes + payload + 2 trailing CRC bytes. Use
            //Use this to calculate total frame length
            frameLength=(8+payloadLength);
                           
             }

             
             //read in the rest of the frame 
              while ((mavIndex<=frameLength) && (startMsg==1))
                {
                if (Serial1.available()==0) {
                  delay(1); //pause if we're processing faster than data is arriving
                }
                
                byte readByte = Serial1.read();          
                mavIndex++;
                mavMSG += char(readByte);
                if (mavIndex == frameLength) {
              
                //all data is read, set data available flag
                  mavDataAvailable=1;
                
                //stop reading data
                startMsg=0;

                }

                }
              }
             
          
     }   

//read encrypted radio link

void readRadio() {
int startMsg=0;
String startSeq="";

     
          //check for start message sequence, if not found discard.
          for (int i=0; i<4;i++) {
         startSeq += char(Serial2.read());
          }
     
          if (encHeader==startSeq) {
            
            //if we're here, we've found start of message
            startMsg=1;
          //get size of encrypted message
              encStrSize = int(Serial2.read());
          }
          
          
           if (startMsg==1) {

             //read in the encrypted data from radio interface
                for (int i=0; i<(encStrSize); i++)
                {
               
               if ((Serial2.available()==0)) {
                  delay(1); //wait if we're processing faster than data is arriving
                }
                
                encryptedIn[i] = Serial2.read();   
                
                 }

                //set encrypted data available flag
                encDataAvailable=1;           
                        
                }
     
             
     }
   


void  writeMav() {
    if (encDataAvailable==1) {

//decrypt data      
newencrypt.decrypt(encryptedIn,decryptedOut,(encStrSize));

//write decrypted data to Mavlink interface
for (int i=0; i<(encStrSize); i++){
Serial1.print ((decryptedOut[i]));
}

//reset variables
encStrSize=0;
encDataAvailable=0;
}
}

//write encrypted radio data

void  writeRadio() {
    if (mavDataAvailable==1) {

    //get entire Mavlink packet size
    strSize = (mavMSG.length());

    //put mavlink message in array 
    for (int j=0 ; j < strSize ; j++) 
    { decryptedIn[j] = mavMSG.charAt(j);}

//send start message sequence
Serial2.print(String(encHeader));

//send encrypted packet length
Serial2.write((strSize+1));

//encrypt mavlink packet
newencrypt.encrypt(decryptedIn, encryptedOut, strSize);

//write encrypted data to radio interface
for (int i=0; i<(strSize+1); i++){
Serial2.print (encryptedOut[i]);
}

}

//reset variables
mavMSG="";
mavDataAvailable=0;
 }
