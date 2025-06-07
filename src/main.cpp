// Blynk Settings
#define BLYNK_TEMPLATE_ID "TMPL6Fj2fisKI"          // Ganti dengan Template ID dari Blynk
#define BLYNK_TEMPLATE_NAME "tubesMiot"
#define BLYNK_AUTH_TOKEN "DVkDquoJqQWyPBGYDS_j3-FqUFAfgb4E"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

void controlServo();
void updateDisplay();
void beepBuzzer();

// WiFi credentials
const char* ssid = "realme 13+ 5G";
const char* password = "kinep2rizki";

// Inisialisasi LCD dan Servo
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo doorLockServo;

// Pin
#define SERVO_PIN 14
#define BUZZER_PIN 25

String currentLockState = "";

void setup() {
  Serial.begin(115200);
  
  pinMode(BUZZER_PIN, OUTPUT);
  doorLockServo.attach(SERVO_PIN);
  doorLockServo.write(90); // posisi netral servo

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  lcd.clear();
  lcd.print("Blynk Ready");
}

// Ketika V1 berubah dari Blynk app
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  if (pinValue == 1 && currentLockState != "BUKA") {
    currentLockState = "BUKA";
    controlServo();
    updateDisplay();
    beepBuzzer();
  } else if (pinValue == 0 && currentLockState != "TUTUP") {
    currentLockState = "TUTUP";
    controlServo();
    updateDisplay();
    beepBuzzer();
  }
}

void loop() {
  Blynk.run();
}

void controlServo() {
  if (currentLockState == "TUTUP") {
    doorLockServo.write(0);
  } else if (currentLockState == "BUKA") {
    doorLockServo.write(90);
  }
}

void updateDisplay() {
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Status:");
  // lcd.setCursor(0, 1);
  Serial.println(currentLockState);
}

void beepBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(150);
  digitalWrite(BUZZER_PIN, LOW);
}
