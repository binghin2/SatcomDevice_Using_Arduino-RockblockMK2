#include <IridiumSBD.h>
#include <SoftwareSerial.h>


// SoftwareSerial 라이브러리로 다른 시리얼 포트 생성
// 핀 설정
#define MODEM_TX 2     // 모뎀 TX 핀 2번핀이 rockblock 의 TXD로 arduino uno
#define MODEM_RX 3     // 모뎀 RX 핀 3번핀이 rockblock 의 RXC로 arduino uno
//#define MODEM_TX 19     // 모뎀 RX 핀 19번핀이 rockblock 의 TXD로 arduino nano
//#define MODEM_RX 20     // 모뎀 RX 핀 20번핀이 rockblock 의 RXC로 arduino nano

#define POWER_CTRL 8    // 전원 제어 핀

#define MODEM_BAUD 19200

SoftwareSerial ssIridium(MODEM_RX, MODEM_TX);  // 소프트웨어 시리얼 포트

IridiumSBD modem(ssIridium, POWER_CTRL);  // 소프트웨어 시리얼 포트를 사용한 모뎀 객체 생성

const char* message = "mobyunghyun";  // 전송할 메시지

bool powerOn = false;  // 전원 상태 변수

void setup() {
  Serial.begin(19200);   // 시리얼 모니터와의 통신 시작
  ssIridium.begin(MODEM_BAUD);  // 모뎀과의 통신 시작 (소프트웨어 시리얼 포트)

  pinMode(POWER_CTRL, OUTPUT);  // 전원 제어 핀 설정

  // 전원 OFF로 초기화
  digitalWrite(POWER_CTRL, LOW);

  Serial.println("Iridium 모뎀 제어 시작...");

  // 전원 ON 후 모뎀 자동 실행
  powerOn = true;
  digitalWrite(POWER_CTRL, HIGH);  // 전원 ON
  Serial.println("전원 ON");

  // 모뎀 초기화
  int result = modem.begin();
  if (result != ISBD_SUCCESS) {
    Serial.print("모뎀 초기화 실패: ");
    Serial.println(result);
    powerOn = false;
    digitalWrite(POWER_CTRL, LOW);  // 전원 OFF
  } else {
    Serial.println("모뎀 초기화 성공!");
  }

  delay(2000);  // 모뎀 초기화 후 2초 대기 (예시)

  // 전원이 켜졌다면 메시지 전송
  if (powerOn) {
    delay(5000);
    sendSBDMessage();  // 전원이 켜지자마자 메시지 전송
  }
}

void loop() {
  // 여기에 다른 로직이 필요하면 추가 가능합니다.
}

// SBD 메시지 전송 함수
void sendSBDMessage() {
  Serial.println("SBD 메시지 전송 중...");

  int result = modem.sendSBDText(message);  // SBD 메시지 전송

  if (result != ISBD_SUCCESS) {
    Serial.print("메시지 전송 실패: ");
    Serial.println(result);
  } else {
    Serial.println("메시지 전송 성공!");
  }

  delay(1000);  // 메시지 전송 후 1초 대기 (예시)

  // 모뎀 절전 모드로 전환
  modem.sleep();
}
