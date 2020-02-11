
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "src/image.h"
#include "src/menu.h"
#include "ThingSpeak.h" //install library for thing speak
#include <WiFi.h>

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

int BPM;
int O2;
int HTemp;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



void checkHeart(){
  
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
