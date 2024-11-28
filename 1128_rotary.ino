#include <IridiumSBD.h>

// 핀 설정
#define MODEM_TX 2     // 모뎀 TX 핀
#define MODEM_RX 3     // 모뎀 RX 핀
#define MODEM_SLEEP 4  // 모뎀 SLEEP 핀

// 로터리 스위치 핀 설정
#define ROTARY_CLK 5   // CLK 핀 (시계/반시계 회전 감지)
#define ROTARY_DT 6    // DT 핀 (회전 방향 확인)
#define ROTARY_SW 7    // SW 핀 (버튼 감지)
#define POWER_CTRL 8   // 전원 제어 핀 (RockBLOCK 전원 ON/OFF)

// 소프트웨어 시리얼 설정
#include <SoftwareSerial.h>
SoftwareSerial ssIridium(MODEM_RX, MODEM_TX);

// Iridium 모뎀 객체 생성
IridiumSBD modem(ssIridium, MODEM_SLEEP);

// 전송할 메시지
const char* message = "helloworld.";

// 전원 상태 및 상태 변수
bool powerOn = false;      // 현재 전원 상태
bool lastCLK = HIGH;       // 마지막 CLK 상태 저장
bool lastButtonState = HIGH; // 버튼 상태 저장

void setup() {
  // 시리얼 통신 설정
  Serial.begin(19200);
  ssIridium.begin(19200);

  // 핀 모드 설정
  pinMode(ROTARY_CLK, INPUT_PULLUP);  // CLK 핀
  pinMode(ROTARY_DT, INPUT_PULLUP);   // DT 핀
  pinMode(ROTARY_SW, INPUT_PULLUP);   // SW 핀
  pinMode(POWER_CTRL, OUTPUT);        // 전원 제어 핀

  // 초기 전원 OFF
  digitalWrite(POWER_CTRL, LOW);

  Serial.println("로터리 스위치와 Iridium 모뎀 제어 시작...");
}

void loop() {
  // 로터리 스위치로 전원 제어
  handleRotarySwitch();

  // 버튼 입력 처리
  handleButtonInput();
}

void handleRotarySwitch() {
  // CLK 신호 읽기
  bool currentCLK = digitalRead(ROTARY_CLK);

  // CLK 상태가 변경되었는지 확인 (회전 감지)
  if (currentCLK != lastCLK && currentCLK == LOW) { // LOW 상태일 때만 동작
    if (digitalRead(ROTARY_DT) == HIGH) { // 시계 방향 회전 감지
      if (!powerOn) {
        Serial.println("전원 ON");
        digitalWrite(POWER_CTRL, HIGH); // 전원 ON
        powerOn = true;

        // 모뎀 초기화
        int result = modem.begin();
        if (result != ISBD_SUCCESS) {
          Serial.print("모뎀 초기화 실패: ");
          Serial.println(result);
          powerOn = false;
          digitalWrite(POWER_CTRL, LOW); // 전원 OFF
        }
      }
    } else if (digitalRead(ROTARY_DT) == LOW) { // 반시계 방향 회전 감지
      if (powerOn) {
        Serial.println("전원 OFF");
        digitalWrite(POWER_CTRL, LOW); // 전원 OFF
        powerOn = false;
      }
    }
    delay(50); // 디바운싱
  }
  lastCLK = currentCLK; // 상태 업데이트
}

void handleButtonInput() {
  // 현재 버튼 상태 읽기
  bool currentButtonState = digitalRead(ROTARY_SW);

  // 버튼 상태 변화 확인 (낮은 신호가 버튼 누름)
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    delay(50); // 디바운싱
    if (digitalRead(ROTARY_SW) == LOW) { // 여전히 눌린 상태 확인
      Serial.println("버튼 눌림 감지: 메시지 전송 중...");
      if (powerOn) {
        sendSBDMessage();
      } else {
        Serial.println("전원이 꺼져 있어 메시지를 전송할 수 없습니다.");
      }
    }
  }
  lastButtonState = currentButtonState; // 상태 업데이트
}

void sendSBDMessage() {
  Serial.println("SBD 메시지 전송 중...");
  int result = modem.sendSBDText(message);
  if (result != ISBD_SUCCESS) {
    Serial.print("메시지 전송 실패: ");
    Serial.println(result);
  } else {
    Serial.println("메시지 전송 성공!");
  }
  modem.sleep();
}
