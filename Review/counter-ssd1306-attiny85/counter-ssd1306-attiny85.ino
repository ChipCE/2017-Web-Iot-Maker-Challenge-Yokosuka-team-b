/*

 *   RESET   -|_|  |- 5V
 *   SCL (3) -|    |- (2) PING_OUT
 *   SDA (4) -|    |- (1) PING_IN
 *   GND     -|____|- (0)
 
 *   OUTDOOR - PING_OUT - DOOR - PING_IN INDOOR

*/

#include "ssd1306.h"
#include "font6x8.h"

//Config
#define PING_OUT 2
#define PING_IN 1
#define THRESHOLD 32
#define TIMEOUT 3000
#define LOOPDELAY 1200

//Data
int inCounter;
int outCounter;

void initData()
{
  inCounter = 0;
  outCounter = 0;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

int isTriggered(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  pinMode(pin, INPUT);
  long duration = pulseIn(pin, HIGH);
  long cm = microsecondsToCentimeters(duration);
  if (cm <= THRESHOLD)
    return 1;
  return 0;
}



void resetDisplay()
{
  //display startup logo

  //display frame
  ssd1306_printFixed(0, 0," People counter demo", STYLE_NORMAL);
  ssd1306_drawLine(0,12, ssd1306_displayWidth() -1, 12);

  initData();
  updateDisplay();
}

void setup()
{
  ssd1306_128x64_i2c_init();
  ssd1306_fillScreen(0x00);
  ssd1306_setFixedFont(ssd1306xled_font6x8);
  resetDisplay();
}

char intToC(int num)
{
  return '0'+num;
}

void updateDisplay()
{
  //reset counter if needed
  if(inCounter>9 || outCounter>9)
    resetDisplay();

  char str[] = "IN  :      OUT :  ";
  str[6]=intToC(inCounter);
  str[17]=intToC(outCounter);
  ssd1306_printFixed(0, 40, str, STYLE_NORMAL);
}

void loop()
{
  int delayFlag = 0;

  char sensorStr[]= "Sensor1:x  Sensor2:x";
  ssd1306_printFixed(0, 24,sensorStr, STYLE_NORMAL);  

  int s0 = isTriggered(PING_OUT);
  int s1 = isTriggered(PING_IN);

  int oldIn = inCounter;
  int oldOut = outCounter;

  if (s0 || s1)
  {
    if (s0)
    {
      //set start time out
      unsigned long strigTime = millis();

      //update sensorStr
      sensorStr[8]='o';
      sensorStr[19]='w';
      ssd1306_printFixed(0, 24,sensorStr, STYLE_NORMAL);

      //wait for other sensor trigger
      while (!isTriggered(PING_IN) != 0 && (millis() - strigTime < TIMEOUT))
        ;
      sensorStr[19]='o';
      ssd1306_printFixed(0, 24,sensorStr, STYLE_NORMAL);


      //and wait for it out of range
      while (isTriggered(PING_IN) == 1 && (millis() - strigTime < TIMEOUT))
        ;

      sensorStr[8]='x';
      sensorStr[19]='x';
      ssd1306_printFixed(0, 24,sensorStr, STYLE_NORMAL);

      //check TIMEOUT
      if (millis() - strigTime < TIMEOUT)
      {
        inCounter++;
        delayFlag = 1;
      }
      else
      {
        //ignore
      }
    }
    else if (s1)
    {
      //set star time out
      unsigned long strigTime = millis();

      sensorStr[8]='w';
      sensorStr[19]='o';
      ssd1306_printFixed(0, 24,sensorStr, STYLE_NORMAL);

      //wait for other sensor trigger
      while (!isTriggered(PING_OUT) != 0 && (millis() - strigTime < TIMEOUT))
        ;

      sensorStr[8]='o';
      ssd1306_printFixed(0, 24,sensorStr, STYLE_NORMAL);
      
      //and wait for it out of range
      while (isTriggered(PING_OUT) == 1 && (millis() - strigTime < TIMEOUT))
        ;

      sensorStr[8]='x';
      sensorStr[19]='x';
      ssd1306_printFixed(0, 24,sensorStr, STYLE_NORMAL);

      //check TIMEOUT
      if (millis() - strigTime < TIMEOUT)
      {
        outCounter++;
        delayFlag = 1;
      }
      else
      {
        //ignore
      }
    }
  }

  if (oldIn != inCounter || oldOut != outCounter)
  {
    //update display
    updateDisplay();
  }

  if (delayFlag)
  {
    ssd1306_printFixed(0, 56,"Status : Busy ", STYLE_NORMAL);  
    delay(LOOPDELAY);
  }
  ssd1306_printFixed(0, 56,"Status : Ready", STYLE_NORMAL);  
}