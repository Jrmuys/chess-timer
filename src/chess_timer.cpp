/**************************************************
 * ChessTimer
 * 
 * Author: Joel Muyskens
 * Date:   2014-04-01
 * Version: 1.0
 * 
 * Description: ChessTimer is an arduino program that 
 *              can be used to time chess games. It
 *              includes game setup where the user can
 *              set the time control and increment.
 * GitHub: 

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

#include <CountDown.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define BUTTON_PIN1 6
#define BUTTON_PIN2 7
#define BUTTON_PIN3 8

#define LED_1 3
#define LED_2 5
#define LED_3 4

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C
#define I2C_ADDRESS_2 0x3D

// Define proper RST_PIN if required.
#define RST_PIN -1

#define PLAYING 0
#define SELECT 1
#define END 2

volatile int mode = SELECT;

SSD1306AsciiAvrI2c oled;
SSD1306AsciiAvrI2c oled2;

CountDown CD(CountDown::MILLIS);
CountDown CD2(CountDown::MILLIS);
volatile byte controlPressed = HIGH;
volatile byte state = LOW;
volatile byte control = LOW;

long timeOptions[7] = {180000, 300000, 600000, 900000, 1200000, 1800000, 5000}; // 3600000
int incrementOptions[6] = {0, 1, 2, 10, 45};

int lastMs = 0;
int currentMs = 0;

int selectState = 0;
int timeOptionIndex = 0;
int incrementOptionIndex = 0;

int b1Pressed = 0;
int b2Pressed = 0;
int b3Pressed = 0;

int firstPress = 1;
int menu = 0;
int increment = 0;
int white = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

String toTimeString(long hundredths);

boolean debounce(int pin)
{
  if (digitalRead(pin))
  {
    return 1;
  }
  else
  {
    unsigned long milli = millis();
    if ((milli - lastDebounceTime) > debounceDelay)
    {
      Serial.print("Debounce: now");
      Serial.print(milli);
      Serial.print(" last: ");
      Serial.print(lastDebounceTime);
      Serial.print(" Difference: ");
      Serial.println(milli - lastDebounceTime);

      return 0;
    }
  }
}

int time = 60 * 5 * 100;
void setup()
{
#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  oled2.begin(&Adafruit128x64, I2C_ADDRESS_2, RST_PIN);

#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled2.begin(&Adafruit128x64, I2C_ADDRESS_2);

#endif // RST_PIN >= 0

  // Call oled.setI2cClock(frequency) to change from the default frequency.

  oled.setFont(Adafruit5x7);
  oled2.setFont(Adafruit5x7);

  Serial.begin(9600);
  Serial.println("Beginning Chess Clock Test...");

  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);
  oled.println("Time Control:");

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);

  oled.setRow(2);
  oled.setFont(fixednums15x31);

  oled.println(toTimeString(timeOptions[timeOptionIndex]));
  oled.setFont(Adafruit5x7);

  oled.setRow(6);
  oled.println("Use buttons to scroll");
}

void reset()
{
  selectState = 0;
  timeOptionIndex = 0;
  incrementOptionIndex = 0;
  increment = 0;

  menu = 0;
  firstPress = 1;
  mode = SELECT;
  oled.clear();
  oled2.clear();

  oled.setFont(Adafruit5x7);
  oled2.setFont(Adafruit5x7);
  oled.println("Time Control:");

  oled.setRow(2);
  oled.setFont(fixednums15x31);

  oled.println(toTimeString(timeOptions[timeOptionIndex]));
  oled.setFont(Adafruit5x7);
  oled.setRow(6);
  oled.println("Use buttons to scroll");
}

void loop()
{
  if (mode == SELECT)
  {
    if (selectState == 0)
    { // Selecting time control

      if (!debounce(BUTTON_PIN1))
      {
        if (!b1Pressed)
        {
          if (timeOptionIndex == 0)
          {
            timeOptionIndex = 6;
          }
          else
          {
            timeOptionIndex--;
          }

          oled.setFont(fixednums15x31);
          oled.setRow(2);
          oled.println(toTimeString(timeOptions[timeOptionIndex]));
          oled.setFont(Adafruit5x7);
          oled.setRow(6);
          oled.println("Push center button to");
          oled.println("select time option");
          b1Pressed = 1;
        }
      }
      else
      {
        if (b1Pressed)
        {
          b1Pressed = 0;
          lastDebounceTime = millis();
        }
      }
      if (!debounce(BUTTON_PIN2))
      {
        if (!b2Pressed)
        {
          timeOptionIndex = (timeOptionIndex + 1) % 7;
          oled.setFont(fixednums15x31);
          oled.setRow(2);

          oled.println(toTimeString(timeOptions[timeOptionIndex]));
          oled.setFont(Adafruit5x7);
          oled.setRow(6);
          digitalWrite(LED_2, HIGH);
          oled.println("Push center button to");
          oled.println("select time option");
          b2Pressed = 1;
        }
      }
      else
      {
        if (b2Pressed)
        {
          b2Pressed = 0;
          lastDebounceTime = millis();
        }
      }
      if (!debounce(BUTTON_PIN3))
      {
        if (!b3Pressed)
        {
          oled.setFont(Adafruit5x7);
          oled.setRow(7);
          oled.clearToEOL();
          oled.setRow(6);
          oled.clearToEOL();
          oled.setRow(6);
          oled.println("Time control set");
          selectState = 1;
          b3Pressed = 1;
          oled2.println("Increment:");
          oled2.println();
          oled2.setFont(fixednums15x31);
          oled2.clearToEOL();
          oled2.println(incrementOptions[incrementOptionIndex]);
          oled2.setRow(6);
          oled2.println("Push center button to");
          oled2.println("select increment");
        }
      }
      else
      {
        if (b3Pressed)
        {
          b3Pressed = 0;
          lastDebounceTime = millis();
        }
      }
    }
    else if (selectState == 1)
    { // Selecting Increment
      if (!debounce(BUTTON_PIN1))
      {
        if (!b1Pressed)
        {
          if (incrementOptionIndex == 0)
          {
            incrementOptionIndex = 4;
          }
          else
          {
            incrementOptionIndex--;
          }

          oled2.setFont(fixednums15x31);
          oled2.setRow(2);
          oled2.clearToEOL();
          oled2.println(incrementOptions[incrementOptionIndex]);
          oled2.setFont(Adafruit5x7);
          oled2.setRow(6);
          oled2.println("Push center button to");
          oled2.println("select time option");
          b1Pressed = 1;
        }
      }
      else
      {
        if (b1Pressed)
        {
          b1Pressed = 0;
          lastDebounceTime = millis();
        }
      }
      if (!debounce(BUTTON_PIN2))
      {
        if (!b2Pressed)
        {
          incrementOptionIndex = (incrementOptionIndex + 1) % 5;
          oled2.setFont(fixednums15x31);
          oled2.setRow(2);
          oled2.clearToEOL();
          oled2.println(incrementOptions[incrementOptionIndex]);
          oled2.setFont(Adafruit5x7);
          oled2.setRow(6);
          oled2.println("Push center button to");
          oled2.println("select time option");
          b2Pressed = 1;
        }
      }
      else
      {
        if (b2Pressed)
        {
          b2Pressed = 0;
          lastDebounceTime = millis();
        }
      }
      if (!debounce(BUTTON_PIN3))
      {
        if (!b3Pressed)
        {
          oled2.setFont(Adafruit5x7);
          oled2.setRow(7);
          oled2.clearToEOL();
          oled2.setRow(6);
          oled2.clearToEOL();
          oled2.setRow(6);
          oled.clear();
          oled2.clear();
          mode = PLAYING;
          CD.start(timeOptions[timeOptionIndex]);
          CD.stop();
          CD2.start(timeOptions[timeOptionIndex]);
          CD2.stop();
          increment = incrementOptions[incrementOptionIndex] * 1000;
          Serial.print("Increment: ");
          Serial.println(increment);
          b3Pressed = 1;
          firstPress = 1;
          selectState = 0;
          control = HIGH;
          oled.setRow(6);
          oled.println("Black press button");
          oled.println("when ready");
          oled2.setRow(6);
          oled2.println("Black press button");
          oled2.println("when ready");
          oled.setFont(fixednums15x31);
          oled2.setFont(fixednums15x31);
        }
      }
      else
      {
        if (b3Pressed)
        {
          b3Pressed = 0;
          lastDebounceTime = millis();
        }
      }
    }
  }
  else if (mode == PLAYING)
  {
    if (debounce(BUTTON_PIN3))
    {
      b3Pressed = 0;
      lastDebounceTime = millis();
    }
    else if (!b3Pressed && !firstPress)
    {
      b3Pressed = 1;
      if (control)
      {
        control = LOW;
        CD.stop();
        CD2.stop();
        Serial.println("STOP");
        oled.setRow(6);
        oled2.setFont(Adafruit5x7);
        oled.setFont(Adafruit5x7);
        oled2.setRow(7);

        oled.println("        PAUSED");
        if ((state && !white) || (!state && white))
        {
          oled.println("     White's Turn");
        }
        else
        {
          oled.println("     Black's Turn");
        }
        oled2.println("<      resume       >");
        menu = 0;
        oled.setFont(fixednums15x31);
        oled2.setFont(fixednums15x31);
      }
      else
      {
        Serial.println("START");
        if (menu == 0)
        {
          oled.setRow(7);
          oled2.setRow(7);
          oled.clearToEOL();
          oled2.clearToEOL();
          oled.setRow(6);
          oled.clearToEOL();
          control = HIGH;
          if (state)
          {
            CD2.cont();
          }
          else
          {
            CD.cont();
          }
        }
        else if (menu == 1)
        {
          reset();
          return;
        }
      }
    }

    if (!digitalRead(BUTTON_PIN1))
    {
      if (!b1Pressed)
      {
        b1Pressed = 1;

        if (control)
        {
          if (firstPress)
          {
            white = 0;
            oled2.setFont(Adafruit5x7);
            oled.setFont(Adafruit5x7);
            oled.setRow(0);
            oled.println("Black");
            oled2.setRow(0);
            oled2.println("White");
            oled.setFont(fixednums15x31);
            oled2.setFont(fixednums15x31);
            oled2.setRow(7);
            oled2.clearToEOL();
            oled2.setRow(6);
            oled2.clearToEOL();
            oled.setRow(7);
            oled.clearToEOL();
            oled.setRow(6);
            oled.clearToEOL();
            state = HIGH;
            CD2.cont();
            CD.stop();

            firstPress = 0;
          }
          else if (!state)
          {
            Serial.println("P1 TOGGLE!");

            state = HIGH;
            CD2.cont();
            CD.start(CD.remaining() + increment);
            CD.stop();
          }
        }
        else
        {
          if (menu == 0)
          {
            oled2.setFont(Adafruit5x7);
            oled2.setRow(7);
            oled2.println("<     end game      >");
            menu = 1;
            oled2.setFont(fixednums15x31);
          }
          else if (menu == 1)
          {
            oled2.setFont(Adafruit5x7);
            oled2.setRow(7);
            oled2.println("<      resume       >");
            menu = 0;
            oled2.setFont(fixednums15x31);
          }
        }
      }
    }
    else
    {
      if (b1Pressed)
      {
        b1Pressed = 0;
        lastDebounceTime = millis();
      }
    }
    if (!digitalRead(BUTTON_PIN2))
    {
      if (!b2Pressed)
      {

        b2Pressed = 1;

        if (control)
        {
          if (firstPress)
          {
            white = 1;
            oled2.setFont(Adafruit5x7);
            oled.setFont(Adafruit5x7);
            oled.setRow(0);
            oled.println("White");
            oled2.setRow(0);
            oled2.println("Black");
            oled.setFont(fixednums15x31);
            oled2.setFont(fixednums15x31);
            oled2.setRow(7);
            oled2.clearToEOL();
            oled2.setRow(6);
            oled2.clearToEOL();
            oled.setRow(7);
            oled.clearToEOL();
            oled.setRow(6);
            oled.clearToEOL();
            state = LOW;
            CD.cont();
            CD2.stop();
            firstPress = 0;
          }
          else if (state)
          {
            Serial.println("P2 TOGGLE!");

            state = LOW;
            CD.cont();
            CD2.start(CD2.remaining() + increment);

            CD2.stop();
          }
        }
        else
        {
          if (menu == 0)
          {
            oled2.setFont(Adafruit5x7);
            oled2.setRow(7);
            oled2.println("<     end game      >");
            menu = 1;
            oled2.setFont(fixednums15x31);
          }
          else if (menu == 1)
          {
            oled2.setFont(Adafruit5x7);
            oled2.setRow(7);
            oled2.println("<      resume       >");
            menu = 0;
            oled2.setFont(fixednums15x31);
          }
        }
      }
    }
    else
    {
      if (b2Pressed)
      {
        b2Pressed = 0;
        lastDebounceTime = millis();
      }
    }
    if (CD.remaining() == 0 || CD2.remaining() == 0)
    {
      mode = END;
    }

    oled.setRow(2);
    oled2.setRow(2);
    oled.println(toTimeString(CD.remaining()));
    oled2.println(toTimeString(CD2.remaining()));
  }
  else if (mode == END)
  {
    oled.setFont(Adafruit5x7);
    oled.setRow(7);
    oled.println("      GAME OVER");
    oled2.setFont(Adafruit5x7);
    oled2.setRow(7);
    oled2.println("Press center to reset");
    if (!debounce(BUTTON_PIN3))
    {
      if (!b3Pressed)
      {
        b3Pressed = 1;
        reset();
      }
    }
    else
    {
      if (b3Pressed)
      {
        b3Pressed = 0;
        lastDebounceTime = millis();
      }
    }
  }
}

String toTimeString(long hundredths)
{
  long tenths = hundredths / 100;
  int seconds = hundredths / 1000;

  int minutes = seconds / 60;

  String timeString = "00:00.00";
  timeString[0] = minutes / 10 % 10 + 48;
  timeString[1] = minutes % 10 + 48;
  timeString[3] = (seconds % 60) / 10 % 10 + 48;
  timeString[4] = (seconds % 60) % 10 + 48;
  timeString[6] = tenths % 10 + 48;
  timeString[7] = hundredths / 10 % 10 + 48;
  return timeString;
}
