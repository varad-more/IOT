/*
  OLED
  SCK---D1
  SDA---D2

  Ultrasonic
  ECHO--D3
  TRIG--D4

  Buzzer(Alert)
  BUZZ--D6

  Tilt Sensor
  TILT--D7
*/

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>  
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define FIREBASE_HOST "HOST"
#define FIREBASE_AUTH "Authentication Token"
#define WIFI_SSID "SSID_WIFI"
#define WIFI_PASSWORD  "***Password***"

#define SENSOR_PIN D7
#define OLED_RESET D5
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


// defines pins numbers
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
int  buz=D6;
// defines variables
long duration;
int distance;

void setup() {
Serial.begin(9600); // Starts the serial communication
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); /* Initialize display with address 0x3C */
  display.clearDisplay();  /* Clear display */
  display.setTextSize(1);  /* Select font size of text. Increases with size of argument. */
  display.setTextColor(WHITE);  /* Color of text*/
pinMode(buz,OUTPUT);
pinMode(SENSOR_PIN, INPUT_PULLUP);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
if (distance<=50 || digitalRead(SENSOR_PIN) == HIGH)
{
  digitalWrite(buz,HIGH);
}
else 
digitalWrite(buz,LOW);

  // set value
  Firebase.setFloat("Distance", distance);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100);
  
Serial.print("Distance: ");
Serial.println(distance);

 writeOLED();

if (distance <=50)  
digitalWrite(D0,HIGH);
//delay(100);
}

void writeOLED(){
  char time[30];
  display.clearDisplay();  /* Clear display */
  sprintf(time, "%d", distance);
  drawStr(50, 10, "Distance");
  drawStr(40, 30, time);
  drawStr(20, 50, "IOT Project");
  display.display();
}

void drawStr(uint8_t x, uint8_t y, char* str){
  display.setCursor(x, y);  /* Set x,y coordinates */
  display.println(str);
}
