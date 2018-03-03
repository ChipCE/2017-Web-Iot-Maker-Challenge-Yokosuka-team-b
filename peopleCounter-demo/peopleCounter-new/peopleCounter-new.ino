#include <DHT_U.h>
#include <DHT.h>



#define DHTPIN 8
#define DHTTYPE DHT11
DHT_Unified sensor(DHTPIN, DHTTYPE);


const int trig1 = 2;
const int echo1 = 3;

const int trig2 = 6;
const int echo2 = 7;

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
  return 0;
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
  sensors_event_t event;  

  //read temp
  sensor.temperature().getEvent(&event);

  //check err
  if (isnan(event.temperature)) 
  {
    Serial.println("Error reading temperature!");
  }
  else 
  {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  }
  
  // Get humidity event and print its value.
  sensor.humidity().getEvent(&event);
  //check err
  if (isnan(event.relative_humidity)) 
  {
    Serial.println("Error reading humidity!");
  }
  else 
  {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
  int testVal = (int)event.relative_humidity;
  Serial.println(testVal);
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

    sensor.begin();
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
