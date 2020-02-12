//Comment out DEBUG line before competition
//#define DEBUG

//SOFTWARE DERIVED REQUIREMENTS
////// Radio Receiver //////
//Arduino 5V to Receiver THRO +
//Arduino GND to Receiver THRO -
//Arduino GND to ESC GND (Brown)
//Arduino D2 to Receiver AILE Data 
//Arduino D3 to Receiver ELEV Data
////// Cytron Motor Controller //////
//Arduino D7 to Cytron Motor Controller M1A
//Arduino D8 to Cytron Motor Controller M1B
//Arduino D9 to Cytron Motor Controller M2A
//Arduino D10 to Cytron Motor Controller M2B
//Arduino VIN to Cytron Motor Controller 5V0
//Arduino GND to Cytron Motor Controller GND

//Receiver input values are accurate for > +- 5 units

//Channel Pinout for Arduino
#define CH1 2 //Y-Axis
#define CH2 3 //X-Axis

//Motor Controller Pinout for Arduino
#define M1A 7
#define M1B 8
#define M2A 9
#define M2B 10

//Ranges for radio input and deadzones
#define deadPos 20
#define deadNeg -20
#define rangePos 250
#define rangeNeg -250

//Drive Forward
void Forward()
{
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}
//Drive Backward
void Backward()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}
//Spin Right
void SpinRight()
{
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}
//Spin Left
void SpinLeft()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}
//Turn Right Forward
void TurnRightForward()
{
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}
//Turn Right Backward
void TurnRightBackward()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}
//Turn Left Forward
void TurnLeftForward()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}
void TurnLeftBackward()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}
//Stop
void Stop()
{
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}

void translateReceiver(int ch1, int ch2)
{
  //Quadrant 1
  if((ch1 > deadPos) && (ch2 > deadPos))
  {
    //Go Forward
    if(ch2 < rangePos)
    {
      Forward();
    }
    //Spin Right
    else if((ch1 > rangePos) && (ch2 < rangePos))
    {
      SpinRight();
    }
    //Turn Right Forward
    else
    {
      TurnRightForward();
    }
  }
  //Quadrant 2
  else if((ch1 > deadPos) && (ch2 < deadNeg))
  {
    //Go Forward
    if(ch2 > rangePos)
    {
      Forward();
    }
    //Spin Left
    else if((ch1 < rangePos) && (ch2 < rangeNeg))
    {
      SpinLeft();
    }
    //Turn Left Forward
    else
    {
      TurnLeftForward();
    }
  }
  //Quadrant 3
  else if((ch1 < deadNeg) && (ch2 < deadNeg))
  {
    //Go Backward
    if(ch2 > rangeNeg)
    {
      Backward();
    }
    //Spin Left
    else if((ch1 > rangeNeg) && (ch2 < rangeNeg))
    {
      SpinLeft();
    }
    //Turn Left Backward
    else
    {
      TurnLeftBackward();
    }
  }
  //Quadrant 4
  else if((ch1 < deadNeg) && (ch2 < deadNeg))
  {
    //Go Backward
    if(ch2 < rangePos)
    {
      Backward();
    }
    //Spin Right
    else if((ch1 > rangeNeg) && (ch2 > rangePos))
    {
      SpinRight();
    }
    //Turn Left Backward
    else
    {
      TurnRightBackward();
    }
  }
  else
  {
    Stop();
  }
}

void setup() 
{
  //Make each channel an input to read value from receiver
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);

  //Make each pin an output for the motor controller
  //Set as output
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);

  //initialize as LOW (brake)
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);

  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

void loop()
{
  #ifdef DEBUG
  for(int i = -500; i < 500; i+=25)
  {
    for(int j = -500; j < 500; j+=25)
    {
      translateReceiver(i, j);
      Serial.print(i); Serial.print(", "); Serial.println(j);
      delay(10);
    }
  }
  #endif
  
  //Read in channel values
  int ch1 = pulseIn(CH1, HIGH, 30000);
  int ch2 = pulseIn(CH2, HIGH, 30000);

  translateReceiver(ch1, ch2);

  #ifdef DEBUG
  //Print channel values for testing
  Serial.print("Channel 1: ");
  Serial.println(map(ch1,1000,2000,-500,500));
  Serial.print("Channel 2: ");
  Serial.println(map(ch2,1000,2000,-500,500));
  delay(100);
  #endif
}
