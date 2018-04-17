int ledsSec[] = {0, 1, 4, 5, 6, 7};
int ledsMin[] = {8, 9, 10, 11, 12, 13};
int ledsHr[] = {14, 15, 16, 17, 18};

volatile int globalCount = 0;
const int redButton = 2;
const int greenButton = 3;

long startMil;
long endMil;

int countSeconds = 0;   
int countMinutes = 0;  
int countHours = 0;  

int secNum;
int minNum;
int hourNum;

void setup(void)
{
  for (int i = 0; i < 5; i++){
    pinMode(ledsSec[i], OUTPUT);
    pinMode(ledsMin[i], OUTPUT);
    pinMode(ledsHr[i], OUTPUT);
  }
  pinMode(ledsSec[5], OUTPUT);
  pinMode(ledsMin[5], OUTPUT);
    
  attachInterrupt(digitalPinToInterrupt(redButton), ISRselect, RISING);
  attachInterrupt(digitalPinToInterrupt(greenButton), ISRchange, RISING);
}

void loop(void)
{
  // prevents clock from ticking
  while(globalCount != 3){
    
  }
  countSeconds = (countSeconds + 1);
  if(countSeconds > 59){
    countSeconds = 0;
    countMinutes = (countMinutes + 1);
    if(countMinutes > 59){
      countMinutes = 0;
      countHours = (countHours + 1);
      if(countHours > 23){
        countHours = 0;
        countMinutes = 0;
        countSeconds = 0;
      }
    }
  }

  // turning the leds on (using binary # division)
  secNum = countSeconds;
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledsSec[i], secNum & 1);
    secNum = secNum/2;
  }

  minNum = countMinutes;
  for (int i = 0; i < 6; i++) {
    digitalWrite(ledsMin[i], minNum & 1);
    minNum = minNum/2;
  }

  hourNum = countHours;
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledsHr[i], hourNum & 1);
    hourNum = hourNum/2;
  }

  // waiting for the 1 second to pass
  startMil = millis();
  endMil = millis();
  while(endMil - startMil < 1000){
    endMil = millis();
  }
}

void ISRselect()
{
  // increment between the 3 setting
  static unsigned long lastTrigger = 0;
  unsigned long thisTrigger = millis();
  if (thisTrigger - lastTrigger > 200){ 
    globalCount ++;
    if(globalCount == 4){
      globalCount = 0;
    }
  }
  lastTrigger = thisTrigger;
}

void ISRchange()
{
  // changing the values
  static unsigned long lastTrigger2 = 0;
  unsigned long thisTrigger2 = millis();
  if (thisTrigger2 - lastTrigger2 > 200){ 
    if(globalCount == 0){
      if (countHours > 22){
        countHours = 0;
      }
      else{
        countHours++;
        countMinutes = 0;
        countSeconds = 0;
      }
    }
    else if(globalCount == 1){
      if(countMinutes > 58){
        countMinutes = 0;
        countSeconds = 0;
      }
      else{
        countMinutes++;
        countSeconds = 0;
      }
    }
    else if(globalCount == 2){
      if(countSeconds > 58){
        countSeconds = 0;
      }
      else{
        countSeconds++;
      }
    }

    // turned on LED
    secNum = countSeconds;
    for (int i = 0; i < 6; i++) {
     digitalWrite(ledsSec[i], secNum & 1);
     secNum = secNum/2;
    }

    minNum = countMinutes;
    for (int i = 0; i < 6; i++) {
     digitalWrite(ledsMin[i], minNum & 1);
     minNum = minNum/2;
    }

    hourNum = countHours;
    for (int i = 0; i < 5; i++) {
     digitalWrite(ledsHr[i], hourNum & 1);
     hourNum = hourNum/2;
    }
  }
  lastTrigger2 = thisTrigger2;
}

