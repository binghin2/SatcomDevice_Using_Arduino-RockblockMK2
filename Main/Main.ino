#include <IridiumSBD.h>   //rockblock mk2 라이브러리
#include <esp_camera.h>   // ESP 카메라 라이브러리
#include <HardwareSerial.h> // 하드웨어 시리얼 통신



// pin mapping
#define BUTTON_1 0  // 버튼1 == ESP32의 GPIO 0 에 연결(물리적)
#define BUTTON_2 1  // 버튼2 == ESP32의 GPIO 1 에 연결
#define BUTTON_3 2  // 버튼3 == ESP32의 GPIO 2 에 연결
#define BUTTON_4 3  // 버튼4 == ESP32의 GPIO 3 에 연결 (카메라 촬영)


#define RX_PIN 16     //rockblock PIN
#define TX_PIN 17     //rockblock PIN

// RockBlock 및 메시지 설정
IridiumSBD isbd(Serial1);       // RockBlock MK2 객체 생성
const char* emails[] = {        // 이메일 주소 목록
    "contact1@example.com",
    "contact2@example.com",
    "contact3@example.com"
};
const char* message = "Hello SBD!"; // 전송할 메시지

void setup() {
    // 버튼 핀 설정
    pinMode(BUTTON_1, INPUT_PULLUP);
    pinMode(BUTTON_2, INPUT_PULLUP);
    pinMode(BUTTON_3, INPUT_PULLUP);
    pinMode(BUTTON_4, INPUT_PULLUP);

    // RockBlock MK2 초기화
    Serial.begin(115200);            // 디버그용 시리얼 포트
    Serial1.begin(19200, SERIAL_8N1, RX_PIN, TX_PIN); // RockBlock 시리얼
    Serial.println("RockBlock Initialized!");

    // 카메라 초기화
    camera_init();
}

void loop() {
    // 버튼 1: 이메일 1로 메시지 전송
    if (digitalRead(BUTTON_1) == LOW) {
        sendSBDMessage(emails[0]);
    }
    // 버튼 2: 이메일 2로 메시지 전송
    else if (digitalRead(BUTTON_2) == LOW) {
        sendSBDMessage(emails[1]);
    }
    // 버튼 3: 이메일 3로 메시지 전송
    else if (digitalRead(BUTTON_3) == LOW) {
        sendSBDMessage(emails[2]);
    }
    // 버튼 4: 10초 동안 동영상 촬영
    else if (digitalRead(BUTTON_4) == LOW) {
        captureVideo(10); // 10초 동영상 촬영
    }

    delay(100); // 버튼 debounce 방지
}

// 메시지 전송 함수
void sendSBDMessage(const char* email) {
    Serial.println("Preparing to send message...");
    int status = isbd.sendSBDText(message); // 메시지 전송
    if (status == ISBD_SUCCESS) {
        Serial.print("Message sent to ");
        Serial.println(email);
    } else {
        Serial.print("Error sending message: ");
        Serial.println(status);
    }
}

// 동영상 촬영 함수
void captureVideo(int duration) {
    Serial.println("Recording video...");
    for (int i = 0; i < duration * 30; i++) { // 30 FPS 기준
        camera_fb_t *frame = esp_camera_fb_get();
        if (frame) {
            // 프레임 데이터 처리 또는 저장 가능
            esp_camera_fb_return(frame);
        }
        delay(33); // 30 FPS 딜레이
    }
    Serial.println("Recording complete.");
}

// 카메라 초기화
void camera_init() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if (psramFound()) {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    if (esp_camera_init(&config) != ESP_OK) {
        Serial.println("Camera init failed!");
        return;
    }
    Serial.println("Camera initialized!");
}