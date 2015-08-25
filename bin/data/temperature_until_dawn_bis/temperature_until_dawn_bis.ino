#include <eHealth.h>
//  VARIABLES
int pulsePin = 1;                 // Pulse Sensor purple wire connected to analog pin 0

// these variables are volatile because they are used during the interrupt service routine!
volatile int Signal;                // holds the incoming raw data

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);  
//  interruptSetup();  
}

// the loop routine runs over and over again forever:
void loop() {
  float temperature = eHealth.getTemperature();
//
  sendDataToProcessing('T', int(temperature * 100));
//  
  float conductance = eHealth.getSkinConductance();
  float resistance = eHealth.getSkinResistance();
  float conductanceVol = eHealth.getSkinConductanceVoltage();

  if(conductance > 0){
      sendDataToProcessing('C', int(conductance * 100)); 
      sendDataToProcessing('V', int(conductanceVol * 100));
  }
  Signal = analogRead(pulsePin); 
  sendDataToProcessing('S', Signal);
   
  delay(30);	// wait for a second 
}


void sendDataToProcessing(char symbol, int data ){
    Serial.print("[");                // symbol prefix tells Processing what type of data is coming
    Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
    Serial.print(data);                // the data to send culminating in a carriage return
    Serial.println("]");                // symbol prefix tells Processing what type of data is coming
 }



