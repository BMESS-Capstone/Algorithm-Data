#include "SatCON.h"

SatCON::SatCON(){ 

}

boolean SatCON::connect(){
    int err;

    // Start the console serial port
    Serial.begin(115200);
    while (!Serial); // Wait for the user to open the serial monitor
    Serial.println(F("Iridium SBD Time I2C"));

    //empty the serial buffer
    while(Serial.available() > 0) Serial.read();

    //wait for the user to press any key before beginning
    Serial.println(F("Press any key to start example."));
    while(Serial.available() == 0);

    //clean up
    while(Serial.available() > 0) Serial.read();

    // Start the I2C wire port connected to the satellite modem
    Wire.begin();
    Wire.setClock(400000); //Set I2C clock speed to 400kHz

    // Check that the Qwiic Iridium is attached
    if (!modem.isConnected())
    {
        Serial.println(F("Qwiic Iridium is not connected! Please check wiring. Freezing."));
        while(1);
    }

    // Enable the supercapacitor charger
    Serial.println(F("Enabling the supercapacitor charger..."));
    modem.enableSuperCapCharger(true);

    // Wait for the supercapacitor charger PGOOD signal to go high
    while (!modem.checkSuperCapCharger())
    {
        Serial.println(F("Waiting for supercapacitors to charge..."));
        delay(1000);
    }
    Serial.println(F("Supercapacitors charged!"));

    // Enable power for the 9603N
    Serial.println(F("Enabling 9603N power..."));
    modem.enable9603Npower(true);

    // Begin satellite modem operation
    Serial.println(F("Starting modem..."));
    modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE); // Assume 'USB' power (slow recharge)
    err = modem.begin();
    if (err != ISBD_SUCCESS)
    {
        Serial.print(F("Begin failed: error "));
        Serial.println(err);
        if (err == ISBD_NO_MODEM_DETECTED)
        Serial.println(F("No modem detected: check wiring."));
        return;
    }
}



boolean SatCON::disconnect(){
    int err;
    // Power down the modem
    Serial.println(F("Putting the 9603N to sleep."));
    err = modem.sleep();
    if (err != ISBD_SUCCESS)
    {
        Serial.print(F("sleep failed: error "));
        Serial.println(err);
    }

    // Disable 9603N power
    Serial.println(F("Disabling 9603N power..."));
    modem.enable9603Npower(false);

    // Disable the supercapacitor charger
    Serial.println(F("Disabling the supercapacitor charger..."));
    modem.enableSuperCapCharger(false);

    Serial.println(F("Done!"));
}

boolean SatCON::send(String message){

    int err;
    // Send the message
    Serial.println(F("Trying to send the message.  This might take several minutes."));
    // This is where the message should be sent
    err = modem.sendSBDText(message.c_str());
    if (err != ISBD_SUCCESS)
    {
        Serial.print(F("sendSBDText failed: error "));
        Serial.println(err);
        if (err == ISBD_SENDRECEIVE_TIMEOUT)
        Serial.println(F("Try again with a better view of the sky."));
    }

    else
    {
        Serial.println(F("Hey, it worked!"));
    }

    // Clear the Mobile Originated message buffer
    Serial.println(F("Clearing the MO buffer."));
    err = modem.clearBuffers(ISBD_CLEAR_MO); // Clear MO buffer
    if (err != ISBD_SUCCESS)
    {
        Serial.print(F("clearBuffers failed: error "));
        Serial.println(err);
    }

}

String SatCON::getTime(){

    struct tm t;
    int err = modem.getSystemTime(t);
    while(err != ISBD_SUCCESS){
        if (err == ISBD_NO_NETWORK){
            Serial.println(F("No network detected.  Waiting 10 seconds."));
        }

        else{
            Serial.print(F("Unexpected error "));
            Serial.println(err);
            return;
        }

        // Delay 10 seconds
        delay(10 * 1000UL);
    }

    if (err == ISBD_SUCCESS){
        char buf[32];
        sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d",
        t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
        Serial.print(F("Iridium date/time is "));
        Serial.println(buf);
    }

    String timeStr = String(t.tm_hour);
    timeStr += ":";
    timeStr += String(t.tm_min);
    timeStr += ":";
    timeStr += String(t.tm_sec);


    return timeStr;
}
