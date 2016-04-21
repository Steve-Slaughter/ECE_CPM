// PINS --------------------------------------------
int startPin = 1;       // Start Button Connected to Digital Pin 1
int eStopPin = 2;       // EStop Connected to Digital Pin 2
int slowDown = 3;       // slowDown Connected to Digital pin 3
int speedUp = 4;        // speedUp Connected to Digital Pin 4
int changeDir = 5;      // Change Direaction Connected to digital pin 5

int saberT = 6;         // Analog Controller Output to Pin 6

int chA = 11;           // Encoder Channel A Connected to Pin 11
int chB = 12;           // Encoder Channel B Connected to Pin 12

//Variables
volatile int aCount = 0;        // Record #of ChA pulses
volatile int bCount = 0;        // Record #of ChB pulses
volatile int speedDif = 0;      // Absolute Value of current output setting minus 193(zero-motion)
volatile int Dir = 1;           // Direction of Arm (1-extend, 0-flex). Arm extends at start
int maxCount = 194;           // Max allowable channelA/B count before reversing direction. 
                              // For 140 degrees of motion (set to 201 for 145 degrees)
int ms = 50;                 // Set Delay Time (in milliseconds)
unsigned int MAXVALUE = 203;            // Max speed value in extension (241 is 3.125V)
unsigned int MINVALUE = 183;            // Max speed value in flexion (145 is 1.875V)

// FUNCTIONS -----------------------------------------

void eStop() {                  
  analogWrite(saberT,193);
  while(1){
  }
}

void incrA(){
  if(aCount==maxCount){
    //Change Direction to Backward
    analogWrite(saberT,(2*193)-analogRead(saberT));
    Dir = 0;
  }
  if(aCount==0){
    //Change Direction to Forward
    analogWrite(saberT,(2*193)-analogRead(saberT));
    Dir = 1;
  }
  if(Dir == 1){
    aCount++;
  }
  else{ //Dir == 0
    aCount=aCount-1;
  }
}

void incrB() {
  if(bCount==maxCount){
    //Change Direction to Backward
    analogWrite(saberT,(2*193)-analogRead(saberT));
    Dir = 0;
  }
  if(bCount==0){
    //Change Direction to Forward
    analogWrite(saberT,(2*193)-analogRead(saberT));
    Dir = 1;
  }
  if(Dir == 1){
    bCount++;
  }
  else{ //Dir == 0
    bCount=bCount-1;
  }
}

void slower(){
  if (analogRead(saberT)>193){
    analogWrite(saberT, analogRead(saberT)-1);
  }
  if (analogRead(saberT)<193){
    analogWrite(saberT, analogRead(saberT)+1);    
  }
}


void faster(){
  if (analogRead(saberT)>=193 && analogRead(saberT)<MAXVALUE){
    analogWrite(saberT, analogRead(saberT)+1);
  }
  if(analogRead(saberT)<193 && analogRead(saberT)>MINVALUE){
    analogWrite(saberT, analogRead(saberT)-1);
  }
}


void reverse(){
  if (Dir){
    Dir = 0;
  analogWrite(saberT,(2*193)-analogRead(saberT));
  }
  else {
    Dir = 1;
  analogWrite(saberT,(2*193)-analogRead(saberT));
  }
}

//-----------------------------------------

void setup() { 
    
  pinMode(startPin,INPUT_PULLUP);                                         // set startPin to Input
  
  pinMode(eStopPin,INPUT_PULLUP);                                  // set eStopPin to Input
  attachInterrupt(eStopPin, eStop, HIGH);   // set EStop Interrupt
  
  pinMode(slowDown,INPUT_PULLUP);                                  // set slowDown to Input
  attachInterrupt(slowDown, slower, RISING);  // set slowDown Interrupt (digitalPinToInterrupt function not working...)
  
  pinMode(speedUp,INPUT_PULLUP);                                   // set speedUp to Input
  attachInterrupt(speedUp, faster, RISING);   // set speedUp Interrupt
  
  pinMode(saberT,OUTPUT);                                          // Output to Sabertooth

  pinMode(changeDir,INPUT);                                        // set changeDir to Input
  attachInterrupt(changeDir, reverse, RISING); // set change Direction Interrupt
  
  pinMode(chA,INPUT);                                              // set ChA to Input
  attachInterrupt(chA, incrA, RISING);      // set ChannelA Interrupt
  
  pinMode(chB,INPUT);                                              // set ChB to Input
  attachInterrupt(chB, incrB, RISING);      // set ChannelB Interrupt


  while (digitalRead(startPin)==0){         //User must press startPin to begin
  }
  analogWrite(saberT,193);     //Set to 2.5V (Middle Value, no motor motion) 
}


void loop() {
  // put your main code here, to run repeatedly:
  // Run all on Interrupts. Don't require looped code
}

