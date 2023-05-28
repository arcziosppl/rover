
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

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

//motor a
#define in1 22
#define in2 24

//motor b
#define in3 26
#define in4 28

//servo
int spos;

Servo s1;
Servo s2;
Servo s3;


int pos[2];
int btn;
int x_pos;
int y_pos;

int initial_position = 90;
int initial_position1 = 90;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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

void nrf_drive()
{
  if (radio.available()) {
  
    radio.read(&pos[0], sizeof(pos[0]));
    delay(10);
	radio.read(&pos[1], sizeof(pos[1]));

    Serial.println(pos[0]);
	Serial.println(pos[1]);

  }


if(pos[0] > 600)
{
	forward();
}
if(pos[0] > 450 && pos[0] < 600)
{
	stop();
}
if(pos[0] < 450)
{
	backward();
}
if(pos[1] > 600)
{
	right();
}
if(pos[1] < 450)
{
	left();
}
}

void nrf_cam()
{
  if (radio.available()) {
  
    radio.read(&pos[0], sizeof(pos[0]));
    delay(10);
	radio.read(&pos[1], sizeof(pos[1]));

  }

if (pos[1] < 300){
if (initial_position < 10) { } else{ initial_position = initial_position - 20; s1.write ( initial_position ) ; delay (100) ; } } if (pos[1] > 700){
if (initial_position > 180)
{  
}  
else{
initial_position = initial_position + 20;
s1.write ( initial_position ) ;
delay (100) ;
}
}

if (pos[0] < 300){
if (initial_position1 < 10) { } else{ initial_position1 = initial_position1 - 20; s2.write ( initial_position1 ) ; delay (100) ; } } if (pos[0] > 700){
if (initial_position1 > 180)
{  
}        
else{
initial_position1 = initial_position1 + 20;
s2.write ( initial_position1 ) ;
delay (100) ;
}
}


/*s1.write(pos[1]);
delay(10);
if(pos[1] > 130 || pos[1] < 70)
{
  s2.write(90);
}
delay(10);
s2.write(pos[0]);
delay(10);*/





}

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
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    //servo
    s1.attach(6);
    s2.attach(2);
    s3.attach(3);


dp();

s1.write (initial_position);
s2.write (initial_position1);

}

void loop() {

stop();

nrf_cam();

}
