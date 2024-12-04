// LED 핀 정의
const int RED_PIN = 4;    // 빨강 핀
const int GREEN_PIN = 5;  // 초록 핀
const int BLUE_PIN = 6;   // 파랑 핀

void setup() {
  // LED 핀을 출력으로 설정
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // 초록색만 켜기 (녹색 LED 활성화)
  digitalWrite(RED_PIN, LOW);   // 빨강 OFF
  digitalWrite(GREEN_PIN, HIGH); // 초록 ON
  digitalWrite(BLUE_PIN, LOW);   // 파랑 OFF
}

void loop() {
  // 메인 루프에서는 추가 동작 없음
}
