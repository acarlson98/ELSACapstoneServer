
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "image.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

int menunum = 4;
int menumod = 4;
int const tact1 = 16;
int const tact2 = 17;
int const tact3 = 18;

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
void setup() {
  Serial.begin(115200);
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
