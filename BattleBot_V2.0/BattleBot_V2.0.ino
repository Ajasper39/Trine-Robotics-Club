//Comment out DEBUG lines before competition
#define DEBUG
#define DEBUG2

//SOFTWARE DERIVED REQUIREMENTS
////// Radio Receiver //////
//Arduino 5V to Receiver THRO +
//Arduino GND to Receiver THRO -
//Arduino GND to ESC GND (Brown)
//Arduino D2 to Receiver AILE Data 
//Arduino D3 to Receiver ELEV Data
////// Cytron Motor Controller MDD10A //////
//Arduino D7 to Cytron Motor Controller DIR1
//Arduino A3 to Cytron Motor Controller PWM1
//Arduino D8 to Cytron Motor Controller DIR2
//Arduino A4 to Cytron Motor Controller PWM2
//Arduino GND to Cytron Motor Controller GND

//Receiver input values are accurate for > +- 7 units

//Channel Pinout for Arduino
#define CH1 2 //X-Axis
#define CH2 3 //Y-Axis
#define SP 0  //Speed Toggle Pin

//Motor Controller Pinout for Arduino
#define DIR1 7
#define PWM1 A3
#define DIR2 8
#define PWM2 A4

//Ranges for radio input and deadzones
#define deadPos 20
#define deadNeg (-20)
#define rangePos 290
#define rangeNeg (-290)
#define origin 0
#define noInput (-1500)

bool speed;

int speedVal;

//Drive Forward
void Forward()
{

  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, speedVal);
  digitalWrite(DIR2, HIGH);  
  analogWrite(PWM2, speedVal);
}
//Drive Backward
void Backward()
{
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, speedVal);
  digitalWrite(DIR2, LOW);  
  analogWrite(PWM2, speedVal);
}
//Spin Right
void SpinRight()
{
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, speedVal);
  digitalWrite(DIR2, LOW);  
  analogWrite(PWM2, speedVal);
}
//Spin Left
void SpinLeft()
{
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, speedVal);
  digitalWrite(DIR2, HIGH);  
  analogWrite(PWM2, speedVal);
}
//Turn Right Forward
void TurnRightForward()
{
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, speedVal);
  digitalWrite(DIR2, LOW);  
  analogWrite(PWM2, 0);
}
//Turn Right Backward
void TurnRightBackward()
{
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, speedVal);
  digitalWrite(DIR2, LOW);  
  analogWrite(PWM2, 0);
}
//Turn Left Forward
void TurnLeftForward()
{
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, 0);
  digitalWrite(DIR2, HIGH);  
  analogWrite(PWM2, speedVal);
}
void TurnLeftBackward()
{
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, 0);
  digitalWrite(DIR2, LOW);  
  analogWrite(PWM2, speedVal);
}
//Stop
void Stop()
{
  digitalWrite(DIR1, HIGH);
  analogWrite(PWM1, 0);
  digitalWrite(DIR2, HIGH);  
  analogWrite(PWM2, 0);
}

//Set Speed
void setSpeed(int chs)
{
  if(speed)
  {
    speedVal = 255;
  }
  else
  {
    speedVal = 128;
  }
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
  //Initialize speed as half speed
  speed = false;
  speedVal = 128;
  
  //Make each channel an input to read value from receiver
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);

  //Make each pin an output for the motor controller
  //Set as output
  pinMode(DIR1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM2, OUTPUT);

  //Initialize as Stopped
  digitalWrite(DIR1, LOW);
  analogWrite(PWM1, 0);
  digitalWrite(DIR2, LOW);  
  analogWrite(PWM2, 0);

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
