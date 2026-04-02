#define BUZZER_PIN 9
#define BUZZER_FREQ 300
#define YELLOW_TIME 1200

class TestChannel {
  int out, in, red, green, disabled;
  unsigned long lastCheckFailed;

public:

  // o: output (one end of cable under test)
  // i: input (other end of cable under test)
  // r: red LED channel
  // g: green LED channel
  // d: "disable" switch (OPEN = disabled, GND = enabled)
  TestChannel(int o, int i, int r, int g, int d) : out(o), in(i), red(r), green(g), disabled(d) {
    pinMode(out, OUTPUT);
    pinMode(in, INPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(disabled, INPUT_PULLUP);

    lastCheckFailed = 0;
  }

  bool check(unsigned long t) {
    if (digitalRead(disabled)) {
      digitalWrite(out, LOW);
      return true;
    }

    digitalWrite(out, HIGH);

    if (digitalRead(in) == HIGH) {
      digitalWrite(green, HIGH);
      digitalWrite(red, (t - lastCheckFailed) < YELLOW_TIME ? HIGH : LOW);
      return true;
    } else {
      lastCheckFailed = t;
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      return false;
    }
  }
};

TestChannel chan1(2, 3, 4, 5, 6),
            chan2(7, 8, 10, 11, 12),
            chan3(A0, A1, A2, A3, A4);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  unsigned long t = millis();
  bool result1 = chan1.check(t);
  bool result2 = chan2.check(t);
  bool result3 = chan3.check(t);

  if (result1 && result2 && result3) {
    noTone(BUZZER_PIN);
  } else {
    tone(BUZZER_PIN, BUZZER_FREQ);
  }
}
