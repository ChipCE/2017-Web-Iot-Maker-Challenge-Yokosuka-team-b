// this constant won't change. It's the pin number of the sensor's output:
const int sensor0 = 7;
const int sensor1 = 8;


long microsecondsToCentimeters(long microseconds)
{
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the object we
    // take half of the distance travelled.
    return microseconds / 29 / 2;
}


void setup()
{
    // initialize serial communication:
    Serial.begin(9600);
}

long readSensor(int mode)
{
    int duration;
    pinMode(mode, OUTPUT);
    digitalWrite(mode, LOW);
    delayMicroseconds(2);
    digitalWrite(mode, HIGH);
    delayMicroseconds(5);
    digitalWrite(mode, LOW);
    pinMode(mode, INPUT);
    duration = pulseIn(mode, HIGH);
    return microsecondsToCentimeters(duration);
}

int in = 0;
int out = 0;

void loop()
{
    int s0 = readSensor(sensor0);
    int s1 = readSensor(sensor1);

    int oldIn = in;
    int oldOut = out;

    if (s0 < 20 || s1 < 20)
    {
        if (s0 < 20)
        {
            in++;
            //wait for unblock s1
            while (readSensor(sensor0) < 30 || readSensor(sensor1) < 30);
        }
        else
        if (s1 < 20)
        {
            out++;
            //wait for unblock s1
            while (readSensor(sensor0) < 30 || readSensor(sensor1)< 30)
                ;
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
    }
    sleep(1000);
}
