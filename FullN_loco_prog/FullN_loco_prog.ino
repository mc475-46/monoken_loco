/* Monoken_loco powered by ANCT Roboken
 * 参考: nekolab, ロボ研の技術の粋
 * 
 * 論理
 * OUT_A = LOW  OUT_B = LOW  SHUTDOWN = LOW  ローサイドブレーキ
 * OUT_A = LOW  OUT_B = HIGH SHUTDOWN = LOW  正転
 * OUT_A = HIGH OUT_B = LOW  SHUTDOWN = LOW  逆転
 * OUT_A,OUT_B = Don't care  SHUTDOWN = HIGH 惰性走行
 * Shutdownは2系統で、2台のモータのどちらかまたはどちらもを惰性走行させることが可能
 */
#define TRANSF 3
#define TRANSB 4
#define HEADLIGHT 5
#define TAILLIGHT 6
#define SHUTDOWN1 7
#define SHUTDOWN2 8
#define OUT_A 9
#define OUT_B 10
#define MASCON 0

#define SHUTDOWN_ENABLE(n) digitalWrite(n, HIGH)
#define SHUTDOWN_DISABLE(n) digitalWrite(n, LOW)

void setup() {
  // データの方向を設定
  pinMode(TRANSF, INPUT_PULLUP);
  pinMode(TRANSB, INPUT_PULLUP);
  for(int i=HEADLIGHT;i<=SHUTDOWN2;++i)
    pinMode(i, OUTPUT);

  // ピンの初期化
  analogWrite(OUT_A, 0);
  analogWrite(OUT_B, 0);
  for(int i=HEADLIGHT;i<=SHUTDOWN2;++i)
    digitalWrite(i, LOW);
    // 初期状態は(ローサイド)ブレーキ
}

void loop() {
  // 出力を計算
  int mascon = analogRead(MASCON);
  byte throttle = mascon > 10 ? (int)(mascon / 1023.0 * 200) : 0;
  // スロットルの最大値は200程度にしておく(ICの仕様)

  // 速度制御
  byte a = 0, b = 0;
  if (throttle == 0){
    // 惰性走行
    SHUTDOWN_ENABLE(SHUTDOWN1);
    SHUTDOWN_ENABLE(SHUTDOWN2);
  } else if (digitalRead(TRANSF) == LOW){
    // 正転PWM走行
    SHUTDOWN_DISABLE(SHUTDOWN1);
    SHUTDOWN_DISABLE(SHUTDOWN2);
    a = 0; b = throttle;
  } else if (digitalRead(TRANSB) == LOW){
    // 逆転PWM走行
    SHUTDOWN_DISABLE(SHUTDOWN1);
    SHUTDOWN_DISABLE(SHUTDOWN2);
    a = throttle; b = 0;
  } else {
    // 中立時、惰性走行
    SHUTDOWN_ENABLE(SHUTDOWN1);
    SHUTDOWN_ENABLE(SHUTDOWN2);
  }
  analogWrite(OUT_A, a);
  analogWrite(OUT_B, b);
}
