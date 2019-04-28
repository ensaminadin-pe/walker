/*
Created by Yvan / https://Brainy-Bits.com

This code is in the public domain...
You can: copy it, use it, modify it, share it or just plain ignore it!
Thx!

*/

/*
 * ESP8266 pins :
 * 
 * ESP            NRF24
 * D6(HMISO)      MISO  |
 * D7(HMOSI)      MOSI  | Parts of the
 * D5(HCLK)       SCK   | SPI BUS
 * D8(HCS)        CSN   |
 * D2             CE    - Data pin
 * 3.3V           VCC
 * G or GND       GND
 * 
 * And then init the NRF24 like so :
 * RF24 radio(4, 15);
 * https://www.electronicwings.com/nodemcu/nodemcu-gpio-with-arduino-ide
 * 4  = GPIO4  (D2)
 * 15 = GPIO15 (D8)
 */ 

// NRF24L01 Module Tutorial - Code for Transmitter using Arduino NANO

//Include needed Libraries at beginning
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

RF24 radio(4,15);

const uint64_t pipe = 0x0123456789123; // Needs to be the same for communicating between 2 NRF24L01 
const byte address[6] = "1Node";
byte addresses[][6] = {"1Node","2Node"};   

void setup(void)
{
  Serial.begin(115200);
  Serial.print("A1");
  radio.begin(); // Start the NRF24L01
  delay(10);
  Serial.print("A2");
  radio.setPALevel(RF24_PA_MIN);
  delay(1);
  Serial.print("A3");
  radio.openWritingPipe(address); // Get NRF24L01 ready to transmit
  delay(1);
}

int main_delay = 1000;

void loop(void)
{
  Serial.print("A4");
  int msg = 111;
  Serial.print("A5");
  delay(1);
  ESP.wdtDisable();
  Serial.print("A6");
  radio.write(&msg, sizeof(int)); // Send value through NRF24L01
  Serial.print("A7");
  ESP.wdtEnable(1);
  Serial.print("A8");
  delay(1);
  Serial.println(main_delay);
  Serial.print("A9");
  delay(main_delay);
  /*
  main_delay += 100;
  if (main_delay > 1000)
    main_delay = 100;
    */
}
