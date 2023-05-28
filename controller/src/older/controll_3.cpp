
//Library: TMRh20/RF24, https://github.com/tmrh20/RF24/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Adafruit_GFX.h>
#include<Wire.h>
#include<Adafruit_I2CDevice.h>
#include<Adafruit_SSD1306.h>


//oled w&h
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

//oled init
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//NRF24 init
RF24 radio(7, 8); // CE, CSN


//NRF24 adress write&read
const byte address[][6] = {"00001","00002"};


//servo cam, motors data to send
const int x = A2;
const int y = A1;
const int dx = A6;
const int dy = A7;
const int trim = A0;

//switch
#define sw1 2


void startup()
{
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("System check");
display.display();

delay(1000);

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("System check");
display.println("Motors ok!");
display.display();

delay(1000);

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("System check");
display.println("Motors ok!");
display.println("Servos ok!");
display.display();

delay(1000);

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("System check");
display.println("Motors ok!");
display.println("Servos ok!");
display.println("temperature ok!");
display.display();

delay(1000);

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("System check");
display.println("Motors ok!");
display.println("Servos ok!");
display.println("temperature ok!");
display.println("Humidity ok!");
display.display();

delay(1000);

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("Everything is ok");
display.println(" ");
display.println("Rover i redy to go");
display.display();

}

void controll()
{
unsigned int x_pos,y_pos,dx_pos,dy_pos,trim_pos;
unsigned int pos[10];

  //aRead to int
x_pos = analogRead(x); //servo_cam
y_pos = analogRead(y); //servo_cam
dx_pos = analogRead(dx); //drive
dy_pos = analogRead(dy); //drive
trim_pos = analogRead(trim);//speed controll
int sw_1 = digitalRead(sw1);

  delay(2);
  radio.stopListening();

//int to table
pos[0] = x_pos;
pos[1] = y_pos;
pos[2] = dx_pos;
pos[3] = dy_pos;
pos[4] = sw_1;
pos[5] = trim_pos;

//radio send data
  radio.write(&pos, sizeof(pos));
  delay(2);
  radio.startListening();
}

void nrf_temp()
{
int temp;

int sw_1 = digitalRead(sw1);

Serial.println(sw_1);

  if(sw_1 == 1)
  {
    display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println("Everything is ok");
display.println(" ");
display.println("Rover i redy to go");
display.display();
  }
  if(sw_1 == 0)
  {
    radio.startListening();
    if(radio.available())
    {
      radio.read(&temp, sizeof(temp));
delay(10);

display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0,0);
display.println(temp);
display.display();
    }
  radio.stopListening();
}
}



void setup() {
Serial.begin(9600);

//sw input
pinMode(sw1, INPUT_PULLUP);

  //radio init
  radio.begin();
  radio.openReadingPipe(1,address[1]);
  radio.openWritingPipe(address[0]);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

//jot inputs
  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(dx, INPUT);
  pinMode(dy, INPUT);

//oled tra begin
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

//startup sequence
  startup();
}

void loop() {
  delay(10);
controll();
delay(10);
nrf_temp();
delay(10);
}
