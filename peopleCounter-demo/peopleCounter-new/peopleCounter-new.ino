#include <DHTesp.h>

DHTesp sensor;
const int sensorPin = 14;

const int trig1 = 5;
const int echo1 = 4;

const int trig2 = 13;
const int echo2 = 12;

int in = 0;
int out = 0;

#define THRESHOLD 30
#define SPACETIME 500
#define LOOPDELAY 500

int readUltrasonic(int mode)
{
  if(mode==0)
  {
    digitalWrite(trig1, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
     digitalWrite(trig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig1, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long  duration = pulseIn(echo1, HIGH);
    //calc the distance in cm 
    long distance = microsecondsToCentimeters(duration);
    if(distance <= THRESHOLD)
      return 1;
    return 0;
  }
  else if(mode==1)
  {
    digitalWrite(trig2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
     digitalWrite(trig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig2, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long  duration = pulseIn(echo2, HIGH);
    //calc the distance in cm 
    long distance = microsecondsToCentimeters(duration);
    if(distance <= THRESHOLD)
      return 1;
    return 0;
  }
  return -1;
}


long debugUltrasonic(int mode)
{
  if(mode==0)
  {
    digitalWrite(trig1, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
     digitalWrite(trig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig1, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long  duration = pulseIn(echo1, HIGH);
    //calc the distance in cm 
    long distance = microsecondsToCentimeters(duration);
    return distance;
  }
  else if(mode==1)
  {
    digitalWrite(trig2, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
     digitalWrite(trig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig2, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long  duration = pulseIn(echo2, HIGH);
    //calc the distance in cm 
    long distance = microsecondsToCentimeters(duration);
    return distance;
  }
  return -1;
}

long microsecondsToCentimeters(long microseconds)
{
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the object we
    // take half of the distance travelled.
    return microseconds / 29 / 2;
}

void readSensor()
{
  delay(sensor.getMinimumSamplingPeriod());

  float humidityVal = sensor.getHumidity();
  float temperatureVal = sensor.getTemperature();
  Serial.print("\t");
  Serial.print(humidityVal, 1);
  Serial.print("\t\t");
  Serial.print(temperatureVal, 1);
  Serial.println();
}


void setup()
{
    // initialize serial communication:
    Serial.begin(9600);
    //pin setup
    pinMode(trig1,OUTPUT);
    pinMode(trig2,OUTPUT);
    pinMode(echo1,INPUT);
    pinMode(echo2,INPUT);

    sensor.setup(sensorPin);
}


void loop()
{

    int s0 = readUltrasonic(0);
    int s1 = readUltrasonic(1);

    int oldIn = in;
    int oldOut = out;

    if (s0 || s1)
    {
        if (s0)
        {
            in++;
            delay(SPACETIME);
            //wait for unblock
            while (readUltrasonic(0) || readUltrasonic(1));
        }
        else
        if (s1)
        {
            out++;
            delay(SPACETIME);
            //wait for unblock
            while (readUltrasonic(0) || readUltrasonic(1));
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
        readSensor();
    }
  delay(LOOPDELAY);

  
}
