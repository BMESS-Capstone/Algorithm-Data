//
// Created by Jeff Roszell on 2022-02-24.
//

#include "screen.h"

extern uint8_t connectionCounter;
extern bool connBool[3];
extern int batteryValue;
extern float oxyValue;

Screen::Screen(){}

void Screen::init() {
  display1 = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display1.display();
    delay(400);
    display1.clearDisplay();
}

void Screen::displayBattery(){
    if (batteryValue <= 20)
        display1.drawBitmap(110,0, battery, 18, 16, 1);
}

void Screen::displayConn(){
    int out_loc [] = {108,20};
    int in_loc [] = {107, 44};

    if (connBool[0] == true)       // connected to wifi
        display1.drawBitmap(out_loc[0],out_loc[1], wifi, 20, 16, 1);
    else if (connBool[1] == true) {  // connected to satellite
        display1.drawBitmap(out_loc[0],out_loc[1], satellite, 16, 20, 1);
        Serial.println("satellite");
    }
    else if (connBool[2] == true)  // connected to cellular
        display1.drawBitmap(out_loc[0],out_loc[1], cellular, 16, 16, 1);

    // displays each connected bluetooth
    Serial.println("Testing bluetooth");
    for (int i=0; i < connectionCounter; i++){
        display1.drawBitmap(in_loc[0] - i*20, in_loc[1], bluetooth, 13, 20, 1);
        display1.setTextSize(1);
        display1.setTextColor(WHITE);
        display1.setCursor(in_loc[0] - i*20 + 14,in_loc[1] + 12);
        display1.println(i+1);
    }
}

void Screen::displayOxy() {
    display1.setTextSize(3);
    display1.setTextColor(WHITE);
    display1.setCursor(0,20);
    display1.println(oxyValue);

    display1.setTextSize(2);
    display1.setCursor(0,0);

    // runs through possible states
    if(oxyValue <= 100 && oxyValue > 85){
        display1.println("Stable");
    }
    else if(oxyValue <= 85 && oxyValue > 70){
        display1.println("Caution");
    }
    else if(oxyValue <= 70 && oxyValue >= 0){
        display1.println("Warning");
    }
    else
        display1.println("Error");
}

void Screen::showDisplay() {
//    Serial.println("Displaying battery");
    displayBattery();
//    Serial.println("Displaying conn");
    displayConn();
//    Serial.println("Displaying value");
    displayOxy();
//    Serial.println("Displaying display");
    display1.display();
//    Serial.println("Displaying delay");
    delay(400);
//    Serial.println("Displaying clear");
    display1.clearDisplay();
}
