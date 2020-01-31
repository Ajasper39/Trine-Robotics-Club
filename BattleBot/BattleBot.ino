//Comment out DEBUG line before competition
#define DEBUG

//SOFTWARE DERIVED REQUIREMENTS
//Arduino 5V to Receiver THRO +
//Arduino GND to Receiver THRO -
//Arduino GND to ESC GND (Brown)
//Arduino D2 to Receiver AILE Data 
//Arduino D3 to Receiver ELEV Data

//Receiver input values are accurate for > +- 5 units

//Channel Pinout for Arduino
#define CH1 2
#define CH2 3

//Variables to store channel values
int ch1;
int ch2;

void setup() 
{
  //Make each channel an input to read value from receiver
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);

  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

void loop()
{
  //Read in channel values
  ch1 = pulseIn(CH1, HIGH, 30000);
  ch2 = pulseIn(CH2, HIGH, 30000);

  #ifdef DEBUG
  //Print channel values for testing
  Serial.print("Channel 1: ");
  Serial.println(map(ch1,1000,2000,-500,500));
  Serial.print("Channel 2: ");
  Serial.println(map(ch2,1000,2000,-500,500));
  delay(100);
  #endif
}
