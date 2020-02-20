//Comment out DEBUG lines before competition
//#define DEBUG
//#define DEBUG2

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

//Receiver input values are accurate for > +- 7 units

//Channel Pinout for Arduino
#define CH1 2 //X-Axis
#define CH2 3 //Y-Axis

//Motor Controller Pinout for Arduino
//POSSIBLE NEED TO FLIP M2A and M2B Arduino Pins
#define M1A 7
#define M1B 8
#define M2A 9
#define M2B 10

//Ranges for radio input and deadzones
#define deadPos 20
#define deadNeg (-20)
#define rangePos 290
#define rangeNeg (-290)
#define origin 0
#define noInput (-1500)

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
  if(((ch1 <= deadPos) && (ch1 >= deadNeg)) && ((ch2 >= deadNeg) && (ch2 <= deadPos)))
  {
    Stop();
    #ifdef DEBUG
    Serial.println("Stop");
    #endif
  }
  else
  {
    //Forward
    if(((ch1 <= rangePos) && (ch1 >= rangeNeg)) && (ch2 > origin))
    {
      Forward();
      #ifdef DEBUG
      Serial.println("Forward");
      #endif
    }
    //Backward
    else if(((ch1 <= rangePos) && (ch1 >= rangeNeg)) && (ch2 < origin))
    {
      Backward();
      #ifdef DEBUG
      Serial.println("Backward");
      #endif
    }
    //Spin Left
    else if((ch1 < rangeNeg) && ((ch2 <= rangePos) && (ch2 >= rangeNeg)))
    {
      SpinLeft();
      #ifdef DEBUG
      Serial.println("SpinLeft");
      #endif
    }
    //Spin Right
    else if((ch1 > rangePos) && ((ch2 <= rangePos) && (ch2 >= rangeNeg)))
    {
      SpinRight();
      #ifdef DEBUG
      Serial.println("SpinRight");
      #endif
    }
    //Turn Right Forward
    else if((ch1 > rangePos) && (ch2 > rangePos))
    {
      TurnRightForward();
      #ifdef DEBUG
      Serial.println("TurnRightForward");
      #endif
    }
    //Turn Right Backward
    else if((ch1 > rangePos) && (ch2 < rangeNeg))
    {
      TurnRightBackward();
      #ifdef DEBUG
      Serial.println("TurnRightBackward");
      #endif
    }
    //Turn Left Forward
    else if((ch1 < rangeNeg) && (ch2 > rangePos))
    {
      TurnLeftForward();
      #ifdef DEBUG
      Serial.println("TurnLeftForward");
      #endif
    }
    //Turn Left Backward
    else if((ch1 < rangeNeg) && (ch2 < rangeNeg))
    {
      TurnLeftBackward();
      #ifdef DEBUG
      Serial.println("TurnLeftBackward");
      #endif
    }
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

  //initialize as LOW (stop)
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);

  #ifdef DEBUG
  //Enable console window for debugging
  Serial.begin(9600);
  #endif

  //Stay here while receiver starts up
  while((pulseIn(CH1, HIGH, 30000) > noInput) || (pulseIn(CH2, HIGH, 30000) > noInput))
  {
    ;
  }
}

void loop()
{  
  //Read receiver channel values and map into coordinate plane
  int ch1 = map(pulseIn(CH1, HIGH, 30000), 1000, 2000, -500, 500);
  int ch2 = map(pulseIn(CH2, HIGH, 30000), 1000, 2000, -500, 500);

  //only translate if receiver is connected
  if((ch1 > noInput) && (ch2 > noInput))
  {
    translateReceiver(ch1, ch2);
  }
   
  #ifdef DEBUG2
  //Print channel values for testing
  Serial.print("Channel 1: ");
  Serial.println(ch1);
  Serial.print("Channel 2: ");
  Serial.println(ch2);
  delay(100);
  #endif
}
