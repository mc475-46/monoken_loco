#define OUT_A 9
#define OUT_B 10
#define SHUTDOWN 6
#define TRANSF 7
#define TRANSB 8
#define DIMMER_SW 4
#define DIMMER_OUT 5
#define MASCON 0

void setup() {
  pinMode(SHUTDOWN, OUTPUT);
  pinMode(TRANSF, INPUT_PULLUP);
  pinMode(TRANSB, INPUT_PULLUP);
  pinMode(DIMMER_SW, INPUT_PULLUP);
  pinMode(DIMMER_OUT, OUTPUT);

  analogWrite(OUT_A, 0);
  analogWrite(OUT_B, 0);
  digitalWrite(SHUTDOWN, HIGH);
  digitalWrite(DIMMER_OUT, LOW);
}

void loop() {
  // 出力を計算
  int mascon = analogRead(MASCON);
  float throttle = mascon > 25 ? mascon / 1023.0 * 200 : 0; // 最大値は200程度にしておく(ICの仕様)

  // 速度制御
  byte a = 0, b = 0;
  if (throttle == 0){
    // 惰性走行
    digitalWrite(SHUTDOWN, LOW);
  } else if (digitalRead(TRANSF) == LOW){
    // 正転PWM走行
    digitalWrite(SHUTDOWN, HIGH);
    a = 0; b = throttle;
  } else if (digitalRead(TRANSB) == LOW){
    // 逆転PWM走行
    digitalWrite(SHUTDOWN, HIGH);
    a = throttle; b = 0;
  } else {
    // 中立時、惰性走行
    digitalWrite(SHUTDOWN, LOW);
  }
  analogWrite(OUT_A, a);
  analogWrite(OUT_B, b);

  // ヘッドライトの明るさ制御
  if (digitalRead(DIMMER_SW) == LOW){
    // 減灯
    digitalWrite(DIMMER_OUT, HIGH);
  } else {
    // 減灯しない
    digitalWrite(DIMMER_OUT, LOW);
  }
}
