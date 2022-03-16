//
// Created by Jeff Roszell on 2022-02-24.
//

#include "screen.h"

Screen::Screen() {}

Screen::Screen(int& value, bool connBools[]){

    display1 = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    // Initialize variables
    connections[0] = connBools[0];
    connections[1] = connBools[1];
    connections[2] = connBools[2];
    int in = 1;
    int life = 100;

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display1.display();
    delay(2000);
    display1.clearDisplay();
}

void Screen::displayBattery(){
    if (life <= 20)
        display1.drawBitmap(110,0,battery, 18, 16, 1);
}

void Screen::displayConn(){
    int out_loc [] = {108,20};
    int in_loc [] = {107, 44};

    if (connections[0] == true)       // connected to wifi
        display1.drawBitmap(out_loc[0],out_loc[1], wifi, 20, 16, 1);
    else if (connections[1] == true)  // connected to satellite
        display1.drawBitmap(out_loc[0],out_loc[1], satellite, 16, 20, 1);
    else if (connections[2] == true)  // connected to cellular
        display1.drawBitmap(out_loc[0],out_loc[1], cellular, 16, 16, 1);

    // displays each connected bluetooth
    for (int i=0;i<in;i++){
        display1.drawBitmap(in_loc[0] - i*20, in_loc[1], bluetooth, 13, 20, 1);
        display1.setTextSize(1);
        display1.setTextColor(WHITE);
        display1.setCursor(in_loc[0] - i*20 + 14,in_loc[1] + 12);
        display1.println(i+1);
    }
}

void Screen::displayOxy(int value) {
    display1.setTextSize(3);
    display1.setTextColor(WHITE);
    display1.setCursor(0,20);
    display1.println(value);

    display1.setTextSize(2);
    display1.setCursor(0,0);

    // runs through possible states
    if(value <= 100 && value > 85){
        display1.println("Stable");
    }
    else if(value <= 85 && value > 70){
        display1.println("Caution");
    }
    else if(value <= 70 && value >= 0){
        display1.println("Warning");
    }
    else
        display1.println("Error");
}

void Screen::showDisplay(int value, bool bools[]) {
    connections[0] = bools[0];
    connections[1] = bools[1];
    connections[2] = bools[2];
    displayBattery();
    displayConn();
    displayOxy(value);
    display1.display();
    delay(400);
    display1.clearDisplay();
}
