#include<Wire.h>
#include <SPI.h>
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET    -1  // Reset pin # (or -1 if sharing reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BLYNK_TEMPLATE_ID "TMPLc4F-HjnE"
#define BLYNK_DEVICE_NAME "gyro"
 
char auth[] = "W5DuJE-ETyUxfzZz38wt_xiI6ZxbQIv5";       // You should get Auth Token in the Blynk App.
char ssid[] = "holaa";                       // Your WiFi credentials.
char pass[] = "yasnuhu12";
 
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;
 
void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(14, OUTPUT);
 
if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
{ 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
}
 display.display();
 delay(2);
 display.clearDisplay();
 
 display.clearDisplay();
 display.setTextColor(WHITE);
 display.setTextSize(2);
 display.setCursor(0,5);
 display.print("MPU Angles");
 display.display();
 delay(3000);
 
}
void loop(){
  digitalWrite(14, LOW);
Blynk.run();
Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-180,180);
int yAng = map(AcY,minVal,maxVal,-180,180);
int zAng = map(AcZ,minVal,maxVal,-180,180);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");
if(x>150 && y>150){
  digitalWrite(14, HIGH);
}
 
display.clearDisplay();
display.setTextSize(2);
display.setCursor(0,0);
display.print("X: ");
display.println(x);
 
display.setTextSize(2);
display.setCursor(0,20);
display.print("Y: ");
display.println(y);
 
display.setTextSize(2);
display.setCursor(0,40);
display.print("Z: ");
display.println(z);
display.display();
 
Blynk.virtualWrite(V2, x);
Blynk.virtualWrite(V3, y);
Blynk.virtualWrite(V4, z);
 
delay(1000);
}