#include "pitches.h"

const int MAX_GAME_LENGTH = 100;

// object orient this code
struct Game {
  uint8_t sequence[MAX_GAME_LENGTH] = {0};
  uint8_t index = 0;
  const uint8_t LED_PINS[4] = {9, 10, 11, 12};
  const uint8_t BUTTON_PINS[4] = {2, 3, 4, 5};
  const uint8_t SPEAKER_PIN = 8;
  const int LATCH_PIN = A1;
  const int DATA_PIN = A0;
  const int CLOCK_PIN = A2;
  const int gameTones[4] = {NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G5};

  void lightLedAndPlayTone(byte ledIndex) {
    digitalWrite(LED_PINS[ledIndex], HIGH);
    tone(SPEAKER_PIN, gameTones[ledIndex]);
    delay(300);
    digitalWrite(LED_PINS[ledIndex], LOW);
    noTone(SPEAKER_PIN);
  }

  void playSequence() {
    for (int i = 0; i < index; i++) {
      byte currentLed = sequence[i];
      lightLedAndPlayTone(currentLed);
      delay(50);
    }
  }

  byte readButtons() {
    while (true) {
      for (byte i = 0; i < 4; i++) {
        byte buttonPin = BUTTON_PINS[i];
        if (digitalRead(buttonPin) == LOW) {
          return i;
        }
      }
      delay(1);
    }
  }

  bool checkUserSequence() {
    for (int i = 0; i < index; i++) {
      byte expectedButton = sequence[i];
      byte actualButton = readButtons();
      lightLedAndPlayTone(actualButton);
      if (expectedButton != actualButton) {
        return false;
      }
    }

    return true;
  }

  void gameOver() {
    Serial.print("Game over! your score: ");
    Serial.println(index - 1);
    index = 0;
    delay(200);

    tone(SPEAKER_PIN, NOTE_DS5);
    delay(300);
    tone(SPEAKER_PIN, NOTE_D5);
    delay(300);
    tone(SPEAKER_PIN, NOTE_CS5);
    delay(300);
    for (byte i = 0; i < 10; i++) {
      for (int pitch = -10; pitch <= 10; pitch++) {
        tone(SPEAKER_PIN, NOTE_C5 + pitch);
        delay(5);
      }
    }
    noTone(SPEAKER_PIN);
    delay(500);
  }

  void playLevelUpSound() {
    tone(SPEAKER_PIN, NOTE_E4);
    delay(150);
    tone(SPEAKER_PIN, NOTE_G4);
    delay(150);
    tone(SPEAKER_PIN, NOTE_E5);
    delay(150);
    tone(SPEAKER_PIN, NOTE_C5);
    delay(150);
    tone(SPEAKER_PIN, NOTE_D5);
    delay(150);
    tone(SPEAKER_PIN, NOTE_G5);
    delay(150);
    noTone(SPEAKER_PIN);
  }

  void loop() {
    sequence[index] = random(0, 4);
    playSequence();
    if (!checkUserSequence()) {
      gameOver();
      return;
    }
    index++;
    if (index > 0) {
      playLevelUpSound();
    }
  }
};

Game game;

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 4; i++) {
    pinMode(game.LED_PINS[i], OUTPUT);
    pinMode(game.BUTTON_PINS[i], INPUT_PULLUP);
  }
  pinMode(game.SPEAKER_PIN, OUTPUT);
  pinMode(game.LATCH_PIN, OUTPUT);
  pinMode(game.CLOCK_PIN, OUTPUT);
  pinMode(game.DATA_PIN, OUTPUT);

  randomSeed(analogRead(A3));
}

void loop() {
  game.loop(); // Usa a instância global 'game'
}
