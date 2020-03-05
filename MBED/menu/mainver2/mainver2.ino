
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "image.h"
//#include "menu.h"
#include "ThingSpeak.h" //install library for thing speak
#include <WiFi.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


//TaskHandle_t Task1;
//TaskHandle_t Task2;

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
    while(digitalRead(tact2)&&digitalRead(tact2)&&digitalRead(tact3)){
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

void setup() {
  Serial.begin(115200);  //Initialize serial


  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(tact1, INPUT_PULLUP); 
  pinMode(tact2, INPUT_PULLUP); 
  pinMode(tact3, INPUT_PULLUP); 
  delay(100); // Pause for 2 seconds
 
  // Clear the buffer.
  display.clearDisplay();
  
  // Draw bitmap on the screen
  display.drawBitmap(0, 15, Logo, 128, 31, 1);
  
  display.display();
  delay(2000);
}

//Core 1
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
      checkMenuTact();
  } 
}

//Core 2
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){

  }
}

void loop(){
  
}
