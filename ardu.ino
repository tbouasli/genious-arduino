#define BUZZER 10
#define START_BUTTON 11
#define RESET_BUTTON 12

struct Value {
    int value;
    int led;
    int buzzer_frequency;
    int button;
}

Value values[4] = {
    {1, 2, 1000, 6},
    {2, 3, 2000, 7},
    {3, 4, 3000, 8},
    {4, 5, 4000, 9}
};

struct Game {
    int input_timeout = 1000;

    int sequence[100];
    int sequence_length;
    int current_sequence_index;

    Game() {
        sequence_length = 1;
        current_sequence_index = 0;
        sequence[0] = random(4) + 1;
    }

    void playSequence() {
        for (int i = 0; i < sequence_length; i++) {
            int led = values[sequence[i]].led;
            int buzzer_frequency = values[sequence[i]].buzzer_frequency;
            digitalWrite(led, HIGH);
            tone(BUZZER, buzzer_frequency);
            delay(500);
            digitalWrite(led, LOW);
            noTone(BUZZER);
            delay(500);
        }
    }

    int[] getResponseSequence() {
        int response_sequence[100];
        int response_sequence_length = 0;
        int start_time = millis();
        while (millis() - start_time < input_timeout) {
            for (int i = 0; i < 4; i++) {
                if (digitalRead(values[i].button) == HIGH) {
                    response_sequence[response_sequence_length] = i;
                    response_sequence_length++;
                    while (digitalRead(values[i].button) == HIGH) {
                        delay(10);
                    }
                }
            }
        }
        return response_sequence;
    }

    bool checkResponse(int response_sequence[]) {
        for (int i = 0; i < sequence_length; i++) {
            if (response_sequence[i] != sequence[i]) {
                return false;
            }
        }
        return true;
    }

    void nextRound() {
        sequence[sequence_length] = random(4) + 1;
        sequence_length++;
        current_sequence_index = 0;
    }

    void reset() {
        sequence_length = 1;
        current_sequence_index = 0;
        sequence[0] = random(4) + 1;
    }

    void gameOver() {
        tone(BUZZER, 100);
        delay(1000);
        noTone(BUZZER);
        delay(1000);
        tone(BUZZER, 100);
        delay(1000);
        noTone(BUZZER);
    }

    void play() {
        while (true) {
            playSequence();
            int response_sequence[] = getResponseSequence();
            if (!checkResponse(response_sequence)) {
                gameOver();
                reset();
            } else {
                nextRound();
            }
        }
    }
};

Game game;

void setup() {
    for (int i = 0; i < 4; i++) {
        pinMode(values[i].led, OUTPUT);
        pinMode(values[i].button, INPUT);
    }
    pinMode(BUZZER, OUTPUT);
    pinMode(START_BUTTON, INPUT);
    pinMode(RESET_BUTTON, INPUT);
}

void loop() {
    if (digitalRead(START_BUTTON) == HIGH) {
        game.play();
    }
}