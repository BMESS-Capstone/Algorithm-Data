void setup() {
  // put your setup code here, to run once:
  bool wifiConnected;
  bool satConnected;
  bool cellConnected;

  bool alert = false;
  int sensorNumber = 0;
  
}

void loop() {
  // put your main code here, to run repeatedly:

  
  while(1){

    //The comm symbol tests
    int counter = 0;
    
    wifiConnected = true;
    while(wifiConnected==true){
      // Turn wifi light to white

      counter++;
      if(counter==5){
        wifiConencted = false;
        counter=0;
        break;
      }
    }

    satConnected = true;
    while(satConnected==true){
      // Turn wifi light to white
      
      counter++;
      if(counter==5){
        cellConencted = false;
        counter=0;
        break;
      }
    }
    
    cellConnected = true;
    while(cellConnected==true){
      // Turn wifi light to white

      counter++;
      if(counter==5){
        cellConencted = false;
        counter=0;
        break;
      }
    }
    

    // The caution lines
    int measurement = 85;
    int average = 0;
    int count = 1;

    while(1){
      average += measurement;
      average /= 2;
      measurement -= 5;

      if(average<=60){
        alert = true;
      }
      if(alert==true){
        // Show caution
        
        delay(1000);
        break;
      }
      
    }

    //Test the batteries
    int battery = 100;

    while(1){
      battery -= 7;

      if(battery<= 20){
        // Show battery icon
        break;
      }
      
    }

    // Bluetooth connection
    // 0 is none, 1 is hand, 2 is leg

    while(1){
      if(sensorNumber==0){
        // Show non bluetooth
        
        bluetooth+=1;
        delay(1000);
      }
      if(sensorNumber==1){
        // Show bluetooth 1;
        
        bluetooth+=1;
        delay(1000);
      }
      if(sensorNumber==2){
        // Show bluetooth 2;
        
        delay(1000);
        break;
      }
    }
  }
}
