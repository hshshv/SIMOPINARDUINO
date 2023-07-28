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
  // initializng buttons to pull up state
  for (int i = 0; i < NUMBER_OF_COLORS; ++i)
  {
    pinMode(colors[i].button, INPUT);
    digitalWrite(colors[i].button, HIGH);
  }
}

void loop()
{
  int level = 0;
  for (level = 0; level < NUM_OF_LEVELS; ++level)
  {
    if (!doGameRound(level))
    {
      break;
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
        delay(AFTER_PRESS_DELAY);
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

}

void showWinningAnimation()
{

}
