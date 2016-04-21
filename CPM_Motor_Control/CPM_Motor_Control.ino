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
int zeroSpeed = 194;                // PWM value for no motion
volatile int aCount = 0;            // Record #of ChA pulses
volatile int bCount = 0;            // Record #of ChB pulses
volatile int currSpeed = zeroSpeed;      // record of current PWM output
volatile int Dir = 1;               // Direction of Arm (1-extend, 0-flex). Arm extends at start
int maxCount = 194;                 // Max allowable channelA/B count before reversing direction. 
                                    // For 140 degrees of motion (set to 201 for 145 degrees)

int ms = 50;                        // Set Delay Time (in milliseconds)
int MAXVALUE = 241;                 // Max speed value in extension (241 is 3.125V)
int MINVALUE = 145;                 // Max speed value in flexion (145 is 1.875V)

// FUNCTIONS -----------------------------------------

void eStop() {                  
  analogWrite(saberT,zeroSpeed);
  while(1){
  }
}

void incrA(){
  if(aCount==maxCount){
    //Change Direction to Backward
    analogWrite(saberT,(2*zeroSpeed)-currSpeed);
    Dir = 0;
    currSpeed = 2*zeroSpeed-currSpeed;
  }
  if(aCount==0){
    //Change Direction to Forward
    analogWrite(saberT,(2*zeroSpeed)-currSpeed);
    Dir = 1;
    currSpeed = 2*zeroSpeed-currSpeed;
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
    analogWrite(saberT,(2*zeroSpeed)-currSpeed);
    Dir = 0;
    currSpeed = 2*zeroSpeed-currSpeed;
  }
  if(bCount==0){
    //Change Direction to Forward
    analogWrite(saberT,(2*zeroSpeed)-currSpeed);
    Dir = 1;
    currSpeed = 2*zeroSpeed-currSpeed;
  }
  if(Dir == 1){
    bCount++;
  }
  else{ //Dir == 0
    bCount=bCount-1;
  }
}

void slower(){
  if (currSpeed > zeroSpeed){
    analogWrite(saberT, currSpeed-1);
    currSpeed = currSpeed - 1;
  }
  if (currSpeed < zeroSpeed){
    analogWrite(saberT, currSpeed+1);    
    currSpeed = currSpeed + 1;
  }
}


void faster(){
  if (currSpeed >= zeroSpeed && currSpeed < MAXVALUE){
    analogWrite(saberT, currSpeed+1);
    currSpeed = currSpeed + 1;
  }
  if(currSpeed < zeroSpeed && currSpeed > MINVALUE){
    analogWrite(saberT, currSpeed-1);
    currSpeed = currSpeed - 1;
  }
}


void reverse(){
  if (Dir){
    Dir = 0;
  analogWrite(saberT,(2*zeroSpeed)-currSpeed);
  currSpeed = 2*zeroSpeed-currSpeed;
  }
  else {
    Dir = 1;
  analogWrite(saberT,(2*zeroSpeed)-currSpeed);
  currSpeed = 2*zeroSpeed-currSpeed;
  }
}

//-----------------------------------------

void setup() {     
  pinMode(startPin,INPUT_PULLUP);                                  // set startPin to Input
  pinMode(eStopPin,INPUT_PULLUP);                                  // set eStopPin to Input
  pinMode(slowDown,INPUT_PULLUP);                                  // set slowDown to Input
  pinMode(speedUp,INPUT_PULLUP);                                   // set speedUp to Input
  pinMode(saberT,OUTPUT);                                          // Output to Sabertooth
  pinMode(changeDir,INPUT);                                        // set changeDir to Input
  pinMode(chA,INPUT);                                              // set ChA to Input
  pinMode(chB,INPUT);                                              // set ChB to Input

  analogWrite(saberT,zeroSpeed);                                   //Set to 2.5V (Middle Value, no motor motion) 
  while (digitalRead(startPin)==0){                                //User must press startPin to begin
  }
  
  attachInterrupt(eStopPin, eStop, HIGH);                          // set EStop Interrupt
  attachInterrupt(slowDown, slower, RISING);                       // set slowDown Interrupt (digitalPinToInterrupt function not working...)
  attachInterrupt(speedUp, faster, RISING);                        // set speedUp Interrupt
  attachInterrupt(changeDir, reverse, RISING);                     // set change Direction Interrupt
  
  attachInterrupt(chA, incrA, RISING);                             // set ChannelA Interrupt
  attachInterrupt(chB, incrB, RISING);                             // set ChannelB Interrupt


}


void loop() {
  // put your main code here, to run repeatedly:
  // Run all on Interrupts. Don't require looped code
}


