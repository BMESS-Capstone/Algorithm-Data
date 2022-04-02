#include "SatCON.h"

SatCON::SatCON() {}

void SatCON::operator=(const SatCON &) {
  SatCON();
}

boolean SatCON::connect() {
  int err;

  // Start the console serial port
  Serial.begin(115200);

  // Start the I2C wire port connected to the satellite modem
  Wire.begin();
//  Wire.setClock(400000); //Set I2C clock speed to 400kHz

  // Check that the Qwiic Iridium is attached
  if (!modem.isConnected())
  {
    Serial.println(F("Qwiic Iridium is not connected! Please check wiring. Freezing."));
    while (1);
  }
  // Enable the supercapacitor charger
  modem.enableSuperCapCharger(true);

  // Wait for the supercapacitor charger PGOOD signal to go high
  while (!modem.checkSuperCapCharger())
  {
    Serial.println(F("charging"));
    delay(200);
  }
  //Serial.println(F("Supercapacitors charged!"));

  // Enable power for the 9603N
  Serial.println(F("Enabling 9603N power..."));
  modem.enable9603Npower(true);

  // Begin satellite modem operation
  modem.setPowerProfile(IridiumSBD::USB_POWER_PROFILE); // Assume 'USB' power (slow recharge)
  err = modem.begin();
  if (err != ISBD_SUCCESS)
  {
    //Serial.print(F("Begin failed: error "));
    //Serial.println(err);
    if (err == ISBD_NO_MODEM_DETECTED){
      Serial.println(F("No modem detected: check wiring."));
    }
  }
  //check maximum 5 times that satellite has proper connectivity
  uint8_t sig = 0;
  Serial.println(F("Searching for satellite connection"));

  for(int i = 0; i<5; i++){
    Serial.print(F(" ."));

    err = modem.getSignalQuality(signalQuality);
    if (signalQuality>=2){
      sig = 1;
      break;
    }
 
    if (err != ISBD_SUCCESS)
    {
      Serial.println(F("SignalQuality failed: error "));
      //Serial.println(err);
      return false;
    }
    delay(750);
  }
  if (sig !=1){
//connection never established, signal too weak
    Serial.println("no connection");
    return false;

  }

  return modem.isConnected();
}



boolean SatCON::disconnect() {
  int err;
  // Power down the modem
  Serial.println(F("Putting the 9603N to sleep."));
  err = modem.sleep();
  if (err != ISBD_SUCCESS)
  {
    Serial.print(F("sleep failed: error "));
    //Serial.println(err);
  }

  // Disable 9603N power
  Serial.println(F("Disabling 9603N power..."));
  modem.enable9603Npower(false);

  // Disable the supercapacitor charger
  Serial.println(F("Disabling the supercapacitor charger..."));
  modem.enableSuperCapCharger(false);

  //Serial.println(F("Done!"));
}

boolean SatCON::send(String message) {
//tries "try_numb" of times to send the message 
  int err;
  int try_numb = 2;
  // Send the message
  Serial.println(F("Trying to send the message.  This might take several minutes."));
  // This is where the message should be sent
  for(int i = 0; i<try_numb; i++){
    err = modem.sendSBDText(message.c_str());
    if (err != ISBD_SUCCESS)
    {
      Serial.print(F("sendSBDText failed: error "));
      //Serial.println(err);
      if (err == ISBD_SENDRECEIVE_TIMEOUT){
        //Serial.println(F("Try again with a better view of the sky."));
    }
  
    else
    {
      Serial.println(F("Message Sent"));
    }
  }
  
  // Clear the Mobile Originated message buffer
 // Serial.println(F("Clearing the MO buffer."));
  err = modem.clearBuffers(ISBD_CLEAR_MO); // Clear MO buffer
  if (err != ISBD_SUCCESS)
  {
    //Serial.print(F("clearBuffers failed: error "));
    //Serial.println(err);
    return false;
  }
  else
    return true;
}
}

std::vector<String> SatCON::getTime() {
  // Should be in format HH:MM:SS

  struct tm t;
  int err = modem.getSystemTime(t);
  while (err != ISBD_SUCCESS) {
    if (err == ISBD_NO_NETWORK) {
      //Serial.println(F("No network detected.  Waiting 10 seconds."));
    }
  

    else {
      //Serial.print(F("Unexpected error "));
      //Serial.println(err);
    }

    // Delay 10 seconds
    delay(10 * 1000UL);
  }

  if (err == ISBD_SUCCESS) {
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


  return split(timeStr, ':');
}

std::vector<String> SatCON::split(String source, char delim){ 

  std::vector<String> result = std::vector<String>();

  int ind1;
  int ind2;
  String hours;
  String minutes;
  String seconds;


  ind1 = source.indexOf(delim);
  hours = source.substring(0,ind1);
  ind2 = source.indexOf(delim, ind1+1);
  minutes = source.substring(ind1+1, ind2);
  seconds = source.substring(ind2+1);

  result.push_back(hours);
  result.push_back(minutes);
  result.push_back(seconds);

  return result;
}
