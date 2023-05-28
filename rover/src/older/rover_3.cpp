
//Library: TMRh20/RF24, https://github.com/tmrh20/RF24/


#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Adafruit_GFX.h>
#include<Wire.h>
#include<Adafruit_I2CDevice.h>
#include<Adafruit_Sensor.h>
#include<Adafruit_SSD1306.h>
#include <Servo.h>
#include <Adafruit_BMP085.h>


//NRF24 begin
RF24 radio(7,8); // CE, CSN

//NRF24 adress
const byte address[][6] = {"00002","00001"};

//motor a
#define in1 22
#define in2 24

//motor b
#define in3 26
#define in4 28

//motor speed
byte ena = 9;
byte enb = 10;

//servo begin
Servo s1;
Servo s2;

//read radio table
int pos[10];

//servo init pos
int initial_position = 90;
int initial_position1 = 90;

//oled w&h
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 

//oled begin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//BMP180 init
#define seaLevelPressure_hPa 1013.25
Adafruit_BMP085 bmp;
int temp;

//HEX BMP rover img
const unsigned char rover [] PROGMEM = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xc0, 0x07, 0xc0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xc0, 0x07, 0xf0, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1c, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0c, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x60, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x0c, 0xff, 0xff, 0xff, 0xff
};


void forward()
{
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
}

void backward()
{
  digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
}

void stop()
{
  digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
}

void right()
{
  digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
}

void left()
{
  digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
digitalWrite(in3, HIGH);
digitalWrite(in4, LOW);
}

//radio read data and drive
void nrf_drive()
{
  radio.startListening();
  if (radio.available()) {
  
    radio.read(&pos, sizeof(pos));
    delay(10);

 int x = pos[0];
  int y = pos[1];
     int dx = pos[2];
  int dy = pos[3];

  int trim_pos = pos[5];
  trim_pos  = map(trim_pos, 0, 1023, 0 , 255);
  Serial.println(trim_pos);

  analogWrite(ena, trim_pos);
  analogWrite(enb, trim_pos);



if(dx > 600)
{
	forward();
}
if(dx > 450 && dx < 600)
{
	stop();
}
if(dx < 450)
{
	backward();
}
if(dy > 600)
{
	right();
}
if(dy < 450)
{
	left();
}

if (x < 300){
if (initial_position < 10)
 { } 
 else
 {
    initial_position = initial_position - 5; 
    s1.write ( initial_position ) ; 
    delay (5) ; 
 } 
 } 
 if(x > 700)
 {
if (initial_position > 180)
{  
}  
else
{
initial_position = initial_position + 5;
s1.write ( initial_position ) ;
delay (5) ;
}
}

if(y < 300)
{
if (initial_position1 < 10) 
{ } 
else
{ initial_position1 = initial_position1 - 5; s2.write ( initial_position1 ) ; delay (5) ; 
} 
} 
if(y > 700)
{
if (initial_position1 > 180)
{  
}        
else{
initial_position1 = initial_position1 + 5;
s2.write ( initial_position1 ) ;
delay (5) ;
}
}

  }
}

//radio read data and run servo
void nrf_cam()
{
  radio.startListening();
  if (radio.available()) {
  
    radio.read(&pos, sizeof(pos));
    delay(10);

 int x = pos[0];
  int y = pos[1];
  

if (x < 300){
if (initial_position < 10)
 { } 
 else
 {
    initial_position = initial_position - 5; 
    s1.write ( initial_position ) ; 
    delay (5) ; 
 } 
 } 
 if(x > 700)
 {
if (initial_position > 180)
{  
}  
else
{
initial_position = initial_position + 5;
s1.write ( initial_position ) ;
delay (5) ;
}
}

if(y < 300)
{
if (initial_position1 < 10) 
{ } 
else
{ initial_position1 = initial_position1 - 5; s2.write ( initial_position1 ) ; delay (5) ; 
} 
} 
if(y > 700)
{
if (initial_position1 > 180)
{  
}        
else{
initial_position1 = initial_position1 + 5;
s2.write ( initial_position1 ) ;
delay (5) ;
}
}
  }
}


//BMP180 temp read
void nrf_temp()
{
  int sw_1 = pos[4];
  temp = bmp.readTemperature();
 
if(sw_1 == 1)
 {}
 if(sw_1 == 0)
 {
   radio.stopListening();
   radio.write(&temp, sizeof(temp));
   delay(10);
   radio.startListening();
 }

}
//rover system check(startup seq)
void dp()
{
display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
display.drawBitmap(0,0,rover,128,32,WHITE);
  display.display();

delay(2000);

display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("System check");
  display.display();

delay(1000);

display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_");
  display.display();

  delay(100);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("__");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("___");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("____");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_____");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("______");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_______");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("________");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_________");
  display.display();

  delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("__________");
  display.display();

   delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("___________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("____________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_____________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("________________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_________________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_________________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("__________________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("___________________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("____________________");
  display.display(); delay(100);display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("_____________________");
  display.display();

  delay(500);

display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("System ok");
  display.display();

  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 16);
display.println("Rover is ready to go");
  display.display();



}



void setup() {
  //NRF24 begin
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address[0]);
  radio.openReadingPipe(1, address[1]);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

//oled begin
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    //servo attach to pins...
    s1.attach(6);
    s2.attach(3);

//start up sequence
dp();

//set servo to 90deg pos
s1.write (initial_position);
s2.write (initial_position1);

//BMP180 begin
if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }

}

void loop() {
nrf_drive();
delay(5);
nrf_temp();
delay(5);
}
