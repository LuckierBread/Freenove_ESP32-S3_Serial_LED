#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 48

CRGB leds[NUM_LEDS];

String LED_State = "white";
String Last_State = "white";
String command = "";
String order = "";
int brightness = 4;

bool updateFlag = false;

String GetCommand()
{
  if (Serial.available() > 0)
  {
    String lastCommand = "";
    char c = Serial.read();
    command = command + c;
    if (command.endsWith("\n"))
    {
      command.trim();
      lastCommand = command;
      command = "";
      Serial.println(lastCommand);
      return lastCommand;
    }
  }
  return "";
}

void SetLED(String LED_State)
{
  if (LED_State == "red")
    leds[0] = CRGB::Red;
  else if (LED_State == "green")
    leds[0] = CRGB::Green;
  else if (LED_State == "blue")
    leds[0] = CRGB::Blue;
  else if (LED_State == "white")
    leds[0] = CRGB::White;
  else if (LED_State == "off")
    leds[0] = CRGB::Black;
  else if (LED_State.startsWith("brightness"))
  {
    int newBrightness = LED_State.substring(10).toInt();
    brightness = constrain(newBrightness, 0, 255); // Ensure valid brightness
    FastLED.setBrightness(brightness);
  }
  FastLED.show();
}

void setup()
{
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  leds[0] = CRGB::White;
  FastLED.setBrightness(brightness);
  FastLED.show();
  Serial.println("Ready For Commands");
}

void loop()
{
  order = GetCommand();
  if (!order.isEmpty() && order != Last_State)
  {
    Serial.println("New Command: " + order);
    SetLED(order);
    Last_State = order;
  }
}