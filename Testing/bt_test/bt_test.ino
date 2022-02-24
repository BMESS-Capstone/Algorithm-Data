void setup() {
  // put your setup code here, to run once:
  btOneCount = 0;
  btTwoCount = 0;
  btThreeCount = 0;
  // 0 is first, 1 is second, 2 is third
  bool out[3];
}

void loop() {
  // put your main code here, to run repeatedly:

  if(btOneCount>=10){
    out[0]=true;
  }
  if(btTwoCount>=10){
    out[1]=true;
  }
  if(btThreeCount>=10){
    out[1]=true;
  }
}
