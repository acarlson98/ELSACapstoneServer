#include <Arduino.h>

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

