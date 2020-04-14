#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int downButtonPin = 2;
const int upButtonPin = 4;
const int ledPin = 13;
const int fanPin = 5;
const int gasJump = 10;

int ledState = HIGH;
int downButtonState;
int lastDownButtonState = LOW;
int upButtonState;
int lastUpButtonState = LOW;
int gas = 0;

int analogValue;

const unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 20;

void setup()
{
  Serial.begin(9600);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 0);
  display.print("Fan Boss");

  display.setTextSize(3);
  display.setCursor(70, 30);
  display.print("%");

  // update display with all of the above graphics
  display.display();
}

void loop()
{
  int downReading = digitalRead(downButtonPin);
  int upReading = digitalRead(upButtonPin);

  readDownButton(downReading);
  readUpButton(upReading);
}

void setDisplayValue()
{
  display.fillRect(15, 30, 51, 22, BLACK);
  display.setTextSize(3);
  display.setCursor(15, 30);
  display.print(gas);
  display.display();
}

void updateFanSpeed()
{
  analogValue = map(gas, 0, 100, 0, 255);
  analogWrite(fanPin, analogValue);
}

void readDownButton(int reading)
{
  if ((millis() - lastDebounceTime) > debounceDelay)
  {

    if (reading != downButtonState)
    {
      downButtonState = reading;

      if (downButtonState == HIGH && gas > 4)
      {
        ledState = !ledState;
        gas = gas - gasJump;
        setDisplayValue();
        updateFanSpeed();
        Serial.println(gas);
      }
    }
  }
  digitalWrite(ledPin, ledState);
  lastDownButtonState = reading;
}

void readUpButton(int reading)
{
  if ((millis() - lastDebounceTime) > debounceDelay)
  {

    if (reading != upButtonState)
    {
      upButtonState = reading;

      if (upButtonState == HIGH && gas < 96)
      {
        ledState = !ledState;
        gas = gas + gasJump;
        setDisplayValue();
        updateFanSpeed();
        Serial.println(gas);
      }
    }
  }
  digitalWrite(ledPin, ledState);
  lastUpButtonState = reading;
}