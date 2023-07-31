/*
  SIMON - the game of simon
*/

#include "colorIO.h"
#include "settings.h"

// globals
colorIO colors[NUMBER_OF_COLORS] =
{
  { RED_BUTTON, RED_LED, RED_FREQ },
  { GREEN_BUTTON, GREEN_LED, GREEN_FREQ },
  { BLUE_BUTTON, BLUE_LED, BLUE_FREQ },
  { YELLOW_BUTTON, YELLOW_LED, YELLOW_FREQ }
};


void setup()
{
  Serial.begin(9600);
  // initializng buttons to pull up state and LEDs to OUTPUT
  for (int i = 0; i < NUMBER_OF_COLORS; ++i)
  {
    pinMode(colors[i].button, INPUT);
    digitalWrite(colors[i].button, HIGH);

    pinMode(colors[i].led, OUTPUT);
  }
  SIMON();
}

void loop()
{
  SIMON();
  delay(3000);
}

void SIMON()
{
  int level = 0;
  for (level = 1; level < NUM_OF_LEVELS; ++level)
  {
    if (!doGameRound(level))
    {
      break;
    }
    else
    {
      showLevelEndAnimation();
      delay(DELAY_BETWEEN_LEVELS);
    }
  }
  if (level == NUM_OF_LEVELS)
  {
    showWinningAnimation();
  }
  else
  {
    showLoosingAnimation();
  }
}

int getNextPress()
{
  while (true)
  {
    for (int i = 0; i < NUMBER_OF_COLORS; ++i)
    {
      if (digitalRead(colors[i].button) == LOW)
      {
        digitalWrite(colors[i].led, HIGH);
        delay(AFTER_PRESS_DELAY);
        while(digitalRead(colors[i].button) == LOW) {} // whiting for the button to de-press
        digitalWrite(colors[i].led, LOW);
        return (i);
      }
    }
  }
}

int* newColorSequence(int sequenceLength)
{
  int* seq = new int[sequenceLength];
  for (int i = 0; i < sequenceLength; ++i)
  {
    seq[i] = analogRead(EMPTY_ANALOG_IN_FOR_RANDOM) % NUMBER_OF_COLORS;
  }
  return seq;
}

void displayColor(colorIO color)
{
  digitalWrite(color.led, HIGH);
  tone(BUZZER_PIN, color.soundFrequency);
  delay(COLOR_DISPLAY_DURATION);
  noTone(BUZZER_PIN);
  digitalWrite(color.led, LOW);
}

void showColorSeq(int* seq, int numOfColors)
{
  for (int i = 0; i < numOfColors; ++i)
  {
    displayColor(colors[seq[i]]);
    delay(AFTER_DISPLAY_DELAY);
  }
}

bool doGameRound(int level)
{
  int* colorSeq = newColorSequence(level);
  showColorSeq(colorSeq, level);
  for (int i = 0; i < level; ++i)
  {
    if (getNextPress() != colorSeq[i])
    {
      return false;
    }
  }
  delete colorSeq;
  return true;
}

void showLoosingAnimation()
{
  for (int i = 0; i < 10; ++i)
  {
    setAllLeds(HIGH);
    delay(300);
    setAllLeds(LOW);
    delay(300);
  }
}

void showWinningAnimation()
{
  for (int i = 0; i < 20; ++i)
  {
    setAllLeds(HIGH);
    delay(150);
    setAllLeds(LOW);
    delay(150);
  }
}

void showLevelEndAnimation()
{
  for (int i = 0; i < 4; ++i)
  {
    setAllLeds(HIGH);
    delay(200);
    setAllLeds(LOW);
    delay(200);
  }
}

void setAllLeds(bool newLedsState)
{
  for (int i = 0; i < NUMBER_OF_COLORS; ++i)
  {
    digitalWrite(colors[i].led, newLedsState);
  }
}
