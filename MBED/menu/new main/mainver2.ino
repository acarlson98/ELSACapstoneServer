
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include "image.h"
//#include "menu.h"
#include "ThingSpeak.h" //install library for thing speak
#include <WiFi.h>

#include "MAX30105.h"
#include "heartRate.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int menunum = 4;
int menumod = 4;
int const tact1 = 16;
int const tact2 = 17;
int const tact3 = 18;

//Thing Speak neededd
char ssid[] = "PKI";        // your network SSID (name) 
char pass[] = "12345678";   // your network password
int keyIndex = 0;           // your network key Index number (needed only for WEP)
WiFiClient  client;
unsigned long myChannelNumber = 985815;
const char * myWriteAPIKey = "77PYOKK03FXL15CW";
// Initialize our values
String myStatus = "";

int BPM = 0;
int O2  = 0;
int HTemp = 0;

MAX30105 particleSensor;
const byte RATE_SIZE = 6; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t Logo[496]  PROGMEM = {  
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x2e, 0xfd, 0xbc, 0x01, 0xfd, 0xbe, 0x1e, 0xfd, 0xbb, 0x87, 0x00, 0x00, 0xc0, 0x07, 0xbb, 0xd0, 
    0x3f, 0xff, 0xfe, 0x07, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0x8f, 0x00, 0x01, 0xf0, 0x3f, 0xff, 0xf8, 
    0x3f, 0xff, 0xfe, 0x1f, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0x8f, 0x80, 0x01, 0xf0, 0x7f, 0xff, 0xf8, 
    0x7f, 0xff, 0xfe, 0x1f, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xcf, 0x00, 0x01, 0xe0, 0xff, 0xff, 0xf8, 
    0x7f, 0x9e, 0xdc, 0x3f, 0x9e, 0xdc, 0x1e, 0xbf, 0xfd, 0x8f, 0x80, 0x01, 0xf0, 0xfe, 0xdd, 0xb0, 
    0x3c, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x01, 0xe0, 0xf0, 0x00, 0x00, 
    0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x80, 0x00, 0xf0, 0xf0, 0x00, 0x00, 
    0x7c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x01, 0xe1, 0xf0, 0x00, 0x00, 
    0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x80, 0x01, 0xf0, 0xf0, 0x00, 0x00, 
    0x7c, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x01, 0xe0, 0xf8, 0x00, 0x00, 
    0x7c, 0xff, 0xf8, 0x3f, 0xff, 0xf8, 0x00, 0x0f, 0x00, 0x0f, 0x80, 0x01, 0xf0, 0xff, 0xff, 0xe0, 
    0x7d, 0xff, 0xf8, 0x1f, 0xff, 0xfe, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x01, 0xf0, 0x7f, 0xff, 0xf0, 
    0x7c, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0x00, 0x0f, 0x00, 0x0f, 0x80, 0x00, 0xe0, 0x7f, 0xff, 0xfc, 
    0x3d, 0xff, 0xf8, 0x07, 0xff, 0xff, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x01, 0xf0, 0x1f, 0xff, 0xfc, 
    0x3c, 0x20, 0x40, 0x00, 0x10, 0x5f, 0x80, 0x0f, 0x00, 0x0f, 0x80, 0x01, 0xf0, 0x00, 0x41, 0x7e, 
    0x7c, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x0f, 0x00, 0x07, 0x80, 0x01, 0xe0, 0x00, 0x00, 0x3e, 
    0x7c, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x0f, 0x00, 0x07, 0xc0, 0x01, 0xf0, 0x00, 0x00, 0x1e, 
    0x3c, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x0f, 0x80, 0x07, 0xc0, 0x01, 0xe0, 0x00, 0x00, 0x3e, 
    0x7c, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x0f, 0x00, 0x03, 0xf0, 0x01, 0xf0, 0x00, 0x00, 0x3e, 
    0x3e, 0x45, 0xa8, 0x14, 0x52, 0x1f, 0x80, 0x0f, 0x00, 0x01, 0xfe, 0x55, 0xe0, 0x15, 0xaa, 0x7e, 
    0x3f, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0x00, 0x0f, 0x00, 0x01, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xfc, 
    0x3f, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0x00, 0x0f, 0x00, 0x00, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xf8, 
    0x0f, 0xff, 0xfe, 0x3f, 0xff, 0xfc, 0x00, 0x0f, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0xff, 0xff, 0xf0, 
    0x07, 0xff, 0xfc, 0x3f, 0xff, 0xf8, 0x00, 0x0f, 0x00, 0x00, 0x07, 0xff, 0xe0, 0xff, 0xff, 0xc0, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  
void menu1(){
  display.clearDisplay();
  display.drawBitmap(0, 0, Logo, 128, 31, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.println(">WiFi       Heart");
  display.println(" Motor  ");
  display.println(" LED");
  display.display();
  return;
}
void menu2(){
  display.clearDisplay();
  display.drawBitmap(0, 0, Logo, 128, 31, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.println(" WiFi       Heart");
  display.println(">Motor");
  display.println(" LED");
  display.display();
  return;
}
void menu3(){
  display.clearDisplay();
  display.drawBitmap(0, 0, Logo, 128, 31, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.println(" WiFi       Heart");
  display.println(" Motor");
  display.println(">LED");
  display.display();
  return;
}
void menu4(){
  display.clearDisplay();
  display.drawBitmap(0, 0, Logo, 128, 31, 1);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.println(" WiFi      >Heart");
  display.println(" Motor");
  display.println(" LED");
  display.display();
  return;
}
void WiFiMenu(){
    delay(200);
    while(digitalRead(tact2)){
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.println("WiFi Menu");
      display.println("SSID: ");
      display.println("text");
      display.display();
    }
  delay(200);
  return;
}
void MotorMenu(){
    delay(200);
    while(digitalRead(tact2)){
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 10);
      display.println("Motor Menu");
      display.println();
      display.setTextSize(1);
      display.println("STEP1    DIR   STEP2");
      display.println("  ^       ^      ^");
      display.display();
    }
  delay(200);
  return;
}
void LEDMenu(){
    delay(200);
    while(digitalRead(tact2)){
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println("LED Menu");
        display.println(" LED");
        display.println(">LED");
        display.display();
      }
    
    return;
}
void HeartMenu(){
    delay(200);
    while(digitalRead(tact2)){
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println("Heart Menu");
        display.setTextSize(1);
        display.println("BPM :");
        display.println("O2  :");
        display.println("TEMP:");
        display.display();
      }
    return;
}
void checkMenuTact(){
    int menu_temp = menunum;
  //Serial.println(menunum);
  delay(150);

  Serial.println(digitalRead(tact2));
 
  if (!digitalRead(tact1)){
    menunum--;
  }
  if (!digitalRead(tact3)){
    menunum++;
  }
  if(menunum % menumod == 0){
    menu1();
    if (!digitalRead(tact2)){
      WiFiMenu();
    }
    return;
  }
  else if(menunum % menumod == 1){
    menu2();
    if (!digitalRead(tact2)){
      MotorMenu();
    }
    return;
  }
  else if(menunum % menumod == 2){
    menu3();
    if (!digitalRead(tact2)){
      LEDMenu();
    }
    return;
  }
  else if(menunum % menumod == 3){
    menu4();
    if (!digitalRead(tact2)){
      HeartMenu();
    }
    return;
  }
  
}
void sendHeart(){
    // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  // set the fields with the values
  ThingSpeak.setField(1, BPM);
  ThingSpeak.setField(1, O2);
  ThingSpeak.setField(1, HTemp);
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
void readHeart(){
 // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  int countmonitor = 1000;
  while(countmonitor>0){
        long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
  countmonitor--;
  }
  return;
}

void setup() {
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
  pinMode(tact1, INPUT_PULLUP); 
  pinMode(tact2, INPUT_PULLUP); 
  pinMode(tact3, INPUT_PULLUP); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(100); // Pause for 2 seconds
 
  // Clear the buffer.
  display.clearDisplay();
  
  // Draw bitmap on the screen
  display.drawBitmap(0, 15, Logo, 128, 31, 1);
  
  display.display();
  delay(2000);
}

void loop() {
  checkMenuTact();

}