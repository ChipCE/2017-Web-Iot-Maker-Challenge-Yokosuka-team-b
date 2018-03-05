
#define TRIG1 11
#define ECHO1 10
#define TRIG2 13
#define ECHO2 12

#define THRESHOLD 10
#define SPACETIME 500
#define LOOPDELAY 1000

//added this time out
#define TIMEOUT 10000

int in = 0;
int out = 0;

int readUltrasonic(int mode)
{
  if (mode == 0)
  {
    digitalWrite(TRIG1, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG1, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO1, HIGH);
    //calc the distance in cm
    long distance = microsecondsToCentimeters(duration);
    if (distance <= THRESHOLD)
      return 1;
    return 0;
  }
  else if (mode == 1)
  {
    digitalWrite(TRIG2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIG2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG2, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO2, HIGH);
    //calc the distance in cm
    long distance = microsecondsToCentimeters(duration);
    if (distance <= THRESHOLD)
      return 1;
    return 0;
  }
  return -1;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

void setup()
{
  // initialize serial communication:
  Serial.begin(9600);
  //pin setup
  pinMode(TRIG1, OUTPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(ECHO2, INPUT);
}

void loop()
{
  int delayFlag = 0;

  int s0 = readUltrasonic(0);
  int s1 = readUltrasonic(1);

  int oldIn = in;
  int oldOut = out;

  if (s0 || s1)
  {
    if (s0)
    {
      Serial.println("# Debug : IN triggered");
      //set star time out
      unsigned long strigTime = millis();

       //wait for other sensor trigger
      while (!readUltrasonic(1) != 0 && (millis() - strigTime < TIMEOUT))
        ;
      //and wait for it out of range
      while (readUltrasonic(1) == 1 && (millis() - strigTime < TIMEOUT))
        ;

      //check TIMEOUT
      if (millis() - strigTime < TIMEOUT)
      {
        Serial.println("# Debug : IN ++");
        in++;
        delayFlag = 1;
      }
      else
        Serial.println("# Debug : IN - TIMEOUT");
    }
    else if (s1)
    {
      Serial.println("# Debug : OUT triggered");
      //set star time out
      unsigned long strigTime = millis();

      //wait for other sensor trigger
      while (!readUltrasonic(0) != 0 && (millis() - strigTime < TIMEOUT))
        ;
      //and wait for it out of range
      while (readUltrasonic(0) == 1 && (millis() - strigTime < TIMEOUT))
        ;

      //check TIMEOUT
      if (millis() - strigTime < TIMEOUT)
      {
        Serial.println("# Debug : OUT ++");
        out++;
        delayFlag = 1;
      }
      else
        Serial.println("# Debug : OUT - TIMEOUT");
    }
  }

  if (oldIn != in || oldOut != out)
  {
    Serial.print("IN :");
    Serial.print(in);
    Serial.println();
    Serial.print("OUT :");
    Serial.print(out);
    Serial.println();
    Serial.println();
  }
  if (delayFlag)
    delay(LOOPDELAY);
}
