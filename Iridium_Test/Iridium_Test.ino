#include <IridiumSBD.h>

// RockBlock MK2와 연결된 시리얼 포트
#define PIN_TX 1
#define PIN_RX 2

IridiumSBD sb; // IridiumSBD 객체 생성

void setup() {
  Serial.begin(115200);
  Serial1.begin(19200, SERIAL_8N1, PIN_RX, PIN_TX); // ESP32 시리얼 포트 설정
  
  // RockBlock MK2와의 통신 초기화
  sb.begin(Serial1); // Serial1 포트 사용
  
  // Iridium 연결 확인
  if (sb.begin(Serial1)) {
    Serial.println("RockBlock MK2 연결 성공!");
  } else {
    Serial.println("RockBlock MK2 연결 실패.");
  }
  
  // 메시지 전송 예제
  String message = "Hello, from ESP32 with IridiumSBD!";
  sendSBDMessage(message);
}

void loop() {
  // 메시지 전송은 setup에서 한번만 실행되므로 loop는 비워두어도 됨.
}

void sendSBDMessage(String message) {
  Serial.println("SBD 메시지 전송 중...");
  // SBD 메시지 준비
  sb.sendSBDMessage(message.c_str());
  Serial.println("메시지 전송 완료!");
}
