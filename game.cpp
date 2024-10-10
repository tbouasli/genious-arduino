struct Game {
  int sequence[100];
  int sequenceLength;
  int currentSequenceIndex;
};

Game generateGame() {
  Game game;
  game.sequenceLength = 1;
  game.currentSequenceIndex = 0;
  game.sequence[0] = random(4) + 1;
  return game;
}

void playGame(Game game) {
  for (int i = 0; i < game.sequenceLength; i++) {
    int led = game.sequence[i];
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}