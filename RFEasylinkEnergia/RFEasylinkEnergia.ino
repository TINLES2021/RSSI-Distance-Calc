/*
  EasyLinkRx
  
  A basic EasyLink Receive example..
  Receive a packet and print the 16-bit value stored in the payload.
  This Sketch is the counterpart of teh EasyLinkTx example.
  
  Hardware Required:
  * CC1310 LaunchPad
  
  This example code is in the public domain.
*/

#include "EasyLink.h"

EasyLink_RxPacket rxPacket;
EasyLink_TxPacket txPacket;

EasyLink myLink;

int myID = 1;
int receiveID = 0;
void transmitt(){
  Serial.println("transmitt called upon");
   int value = 8;

  // Copy the analog value into the txPacket payload
  memcpy(&txPacket.payload, &value, sizeof(value));

  // Set the length of the packet
  txPacket.len = sizeof(value);
  // Transmit immediately
  txPacket.absTime = EasyLink_ms_To_RadioTime(0);

  EasyLink_Status status = myLink.transmit(&txPacket);

  if(status == EasyLink_Status_Success) {
    Serial.println("Packet transmitted successfully");
  } else {
    Serial.print("Transmit failed with status code: ");
    Serial.print(status);
    Serial.print(" (");
    Serial.print(myLink.getStatusString(status));
    Serial.println(")");
  }

  delay(1000);
  }


void receivv(){
  // Wait / block for 2 second to receive a packet.
  // rxTimeout is in Radio time and needs to be converted from miliseconds to Radio Time
  rxPacket.rxTimeout = EasyLink_ms_To_RadioTime(2000);
  // Turn the receiver on immediately
  rxPacket.absTime = EasyLink_ms_To_RadioTime(0);

  EasyLink_Status status = myLink.receive(&rxPacket);
  if (status == EasyLink_Status_Success) {
    float rxRssi;
    rxRssi = rxPacket.rssi;
    float distance = distanceCalc(rxRssi);
    int value;
    
    memcpy(&value, &rxPacket.payload, sizeof(value));
    Serial.print("Packet received with lenght ");
    Serial.print(rxPacket.len);
    Serial.print(" and value ");
    Serial.println(value);
    Serial.print(" and Rssi: ");
    Serial.println(rxRssi);
    Serial.print(" distance: ");
    Serial.println(distance);

    if (receiveID == myID-1){
      transmitt();
    
    }
  }
  else if(status == 7 && myID == 1){
    Serial.print("Error receiving packet with status code: ");
    Serial.print(status);
    Serial.print(" (");
    Serial.print(myLink.getStatusString(status));
    Serial.println(")");
    transmitt();
  }
    else {

    Serial.print("Error receiving packet with status code: ");
    Serial.print(status);
    Serial.print(" (");
    Serial.print(myLink.getStatusString(status));
    Serial.println(")");
  }
}
float distanceCalc (int8_t rssi){
       int8_t Po;
       float d;
       float n;
       n = -3.2;
       Po = -17;
       if (Po-rssi == 0){
               d = 0;
       }
       else{
           d = rssi-Po;
           d = (d/(10*n));
           d = pow(10,d);
       }
       return d;
   }


void setup() {
  Serial.begin(115200);
  // begin defaults to EasyLink_Phy_50kbps2gfsk
  myLink.begin();
  Serial.println(myLink.version());
  Serial.print("Bootup message! Greetings! ");
}

void loop() {
  receivv();  
 }
