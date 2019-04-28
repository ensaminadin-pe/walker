#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

/*
 * Arduino Nano pins :
 * 
 * Nano       NRF24
 * D12(MISO)  MISO
 * D11(MOSI)  MOSI
 * D13(SCK)   SCK
 * D10(SS)    CSN
 * D9         CE
 * 3.3V       VCC
 * G or GND   GND
 */

//RF24 radio(9, 10); // CE, CSN
RF24 radio(9, 10);
const byte address[6] = "1Node";


void setup()
{
  Serial.begin(115200);
  Serial.print("A1");
  delay(1);
  radio.begin();
  delay(1);
  Serial.print("A2");
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  delay(1);
  Serial.print("A3");
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  delay(1);
  Serial.print("A4");
  radio.startListening();              //This sets the module as receiver
  delay(1);
  Serial.print("A5");
}

void loop()
{
  Serial.print("A6");
  if (radio.available())              //Looking for the data.
  {
    int msg = 0;                 //Saving the incoming data
    radio.read(&msg, sizeof(int));    //Reading the data
    Serial.println(msg);
  }
  delay(50);
}
