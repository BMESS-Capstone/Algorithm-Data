//
// Created by Jeff Roszell on 2022-02-24.
//

#include "screen.h"

void screen::check_battery(int life){
    if (life <= 20)
        display.drawBitmap(110,0,battery, 18, 16, 1);
}

void screen::communication_display(bool out [], int in){
    int out_loc [] = {108,20};
    int in_loc [] = {107, 44};

    if (out[0] == true)       // connected to wifi
        display.drawBitmap(out_loc[0],out_loc[1], wifi, 20, 16, 1);
    else if (out[1] == true)  // connected to satellite
        display.drawBitmap(out_loc[0],out_loc[1], satellite, 16, 20, 1);
    else if (out[2] == true)  // connected to cellular
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

void screen::oxygen_level(int value, int counter) {
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
        if (counter%4 == 0 || (counter - 1)%4 == 0)
            display.println("Caution");
    }
    else if(value <= 70 && value >= 0){
        if (counter%2 == 0)
            display.println("Warning");
    }
    else
        display.println("Error");
}