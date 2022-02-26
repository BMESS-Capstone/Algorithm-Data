//
// Created by Jeff Roszell on 2022-02-24.
//

#include "screen.h"
void Screen(int value){

    // Initialize variables
    bool connections [] = {true,false,false};
    int in = 1;
    int life = 100;

    // Startup the device
    Serial.begin(115200);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000);
    display.clearDisplay();
}

void Screen::displayBattery(){
    if (life <= 20)
        display.drawBitmap(110,0,battery, 18, 16, 1);
}

void Screen::displayConn(){
    int out_loc [] = {108,20};
    int in_loc [] = {107, 44};

    if (this->connections[0] == true)       // connected to wifi
        display.drawBitmap(out_loc[0],out_loc[1], wifi, 20, 16, 1);
    else if (this->connections[1] == true)  // connected to satellite
        display.drawBitmap(out_loc[0],out_loc[1], satellite, 16, 20, 1);
    else if (this->connections[2] == true)  // connected to cellular
        display.drawBitmap(out_loc[0],out_loc[1], cellular, 16, 16, 1);

    // displays each connected bluetooth
    for (int i=0;i<in;i++){
        display.drawBitmap(in_loc[0] - i*20, in_loc[1], bluetooth, 13, 20, 1);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(in_loc[0] - i*20 + 14,in_loc[1] + 12);
        display.println(i+1);
    }
}

void Screen::displayOxy(int value) {
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0,20);
    display.println(value);

    display.setTextSize(2);
    display.setCursor(0,0);

    // runs through possible states
    if(value <= 100 && value > 85){
        display.println("Stable");
    }
    else if(value <= 85 && value > 70){
        display.println("Caution");
    }
    else if(value <= 70 && value >= 0){
        display.println("Warning");
    }
    else
        display.println("Error");
}

void Screen::showDisplay(int value) {
    displayBattery();
    displayConn();
    displayOxy(value);
    display.display();
    delay(400);
    display.clearDisplay();
}