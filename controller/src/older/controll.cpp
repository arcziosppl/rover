/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Adafruit_GFX.h>
#include<Wire.h>
#include<Adafruit_I2CDevice.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

int x = A2;
int y = A1;

#define sw 2


int pos[2];
int s_pos[2];

int btn = digitalRead(sw);


void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(sw, INPUT_PULLUP);
}

void loop() {




pos[0] = analogRead(x);
pos[1] = analogRead(y);

//pos[0] = map(pos[0], 0,1023,0,180);
//pos[1] = map(pos[1], 0,1023,0,180);


  radio.write(&pos[0], sizeof(pos[0]));
  delay(10);
    radio.write(&pos[1], sizeof(pos[1]));

  



}
