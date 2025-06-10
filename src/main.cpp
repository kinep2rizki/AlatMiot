// Blynk Settings - Replace with your details
#define BLYNK_TEMPLATE_ID "TMPL6Fj2fisKI" 
#define BLYNK_TEMPLATE_NAME "tubesMiot"
#define BLYNK_AUTH_TOKEN "DVkDquoJqQWyPBGYDS_j3-FqUFAfgb4E"

// Include necessary libraries
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// --- Function Prototypes ---
void controlServo();
void updateDisplay();

// --- Hardware & Network Credentials ---
const char* ssid = "realme 13+ 5G";
const char* password = "kinep2rizki";

// Pin Definitions
#define SERVO_PIN 14     // Changed to pin 13 for the Servo

// --- Global Variables ---
String currentLockState = "TUTUP"; // Initial state of the lock

// --- Object Initialization ---
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD (address 0x27, 16 columns, 2 rows)
Servo doorLockServo;

//================================================================
// SETUP FUNCTION
//================================================================
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // Pastikan pin sesuai (SDA, SCL)

  doorLockServo.attach(SERVO_PIN);

  lcd.begin();         // GANTI dari lcd.init() ke lcd.begin()
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  lcd.clear();
  updateDisplay();
}


//================================================================
// MAIN LOOP
//================================================================
void loop() {
  Blynk.run();
}

//================================================================
// BLYNK FUNCTIONS
//================================================================
BLYNK_WRITE(V1) {
  int pinValue = param.asInt();

  // Condition to open the lock
  if (pinValue == 1 && currentLockState != "BUKA") {
    currentLockState = "BUKA";
    Serial.println("Received command: BUKA");
    controlServo();
    updateDisplay();
  }
  // Condition to close the lock
  else if (pinValue == 0 && currentLockState != "TUTUP") {
    currentLockState = "TUTUP";
    Serial.println("Received command: TUTUP");
    controlServo();
    updateDisplay();
  }
}

//================================================================
// CUSTOM FUNCTIONS
//================================================================

/**
 * @brief Controls the servo motor directly (no relay).
 */
void controlServo() {
  if (currentLockState == "TUTUP") {
    doorLockServo.write(45);      // Rotate servo to 'locked' position
  } else if (currentLockState == "BUKA") {
    doorLockServo.write(135);     // Rotate servo to 'unlocked' position
  }

  delay(1000); // Allow time for servo to reach position
}

/**
 * @brief Updates the LCD with the current lock status.
 */
void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Status Pintu:");
  lcd.setCursor(0, 1);
  lcd.print(currentLockState);
  Serial.print("Status updated to: ");
  Serial.println(currentLockState);
}

