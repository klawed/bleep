#include <Arduino.h>
#include <WiFi.h>

// Pin definitions
#define LED_PIN 2
#define BUTTON_PIN 0  // Boot button on most ESP32 boards

// WiFi credentials (leave empty for AP mode demo)
const char* ssid = "";
const char* password = "";

// State variables
bool ledState = false;
bool buttonPressed = false;
unsigned long lastButtonPress = 0;
unsigned long lastBlink = 0;
unsigned long blinkInterval = 1000;  // Start with 1 second
int blinkMode = 0;  // 0=slow, 1=fast, 2=heartbeat, 3=SOS

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.println("ESP32 IoT Template - Interactive Blink Demo");
  Serial.println("===============================================");
  
  // WiFi setup
  if (strlen(ssid) > 0) {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi connected!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi connection failed, continuing without WiFi");
    }
  } else {
    Serial.println("No WiFi credentials set, running in standalone mode");
  }
  
  Serial.println("Controls:");
  Serial.println("- Press BOOT button to cycle through blink modes");
  Serial.println("- Send 's' via Serial to scan for WiFi networks");
  Serial.println("- Send 'r' via Serial to restart");
  Serial.println("Blink Modes:");
  Serial.println("0: Slow blink (1s)");
  Serial.println("1: Fast blink (200ms)");
  Serial.println("2: Heartbeat pattern");
  Serial.println("3: SOS pattern");
  
  digitalWrite(LED_PIN, LOW);
}

void toggleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void heartbeatPattern(unsigned long currentTime) {
  static int beatState = 0;
  static unsigned long beatTimer = 0;
  
  if (currentTime - beatTimer >= 100) {
    switch (beatState) {
      case 0: digitalWrite(LED_PIN, HIGH); beatState = 1; break;   // First beat
      case 1: digitalWrite(LED_PIN, LOW); beatState = 2; break;
      case 2: digitalWrite(LED_PIN, HIGH); beatState = 3; break;   // Second beat
      case 3: digitalWrite(LED_PIN, LOW); beatState = 4; break;
      case 4: case 5: case 6: case 7: case 8: case 9: case 10:    // Long pause
        digitalWrite(LED_PIN, LOW);
        beatState++;
        break;
      default:
        beatState = 0;
        break;
    }
    beatTimer = currentTime;
  }
}

void sosPattern(unsigned long currentTime) {
  static int sosState = 0;
  static unsigned long sosTimer = 0;
  // SOS: ... --- ... (3 short, 3 long, 3 short)
  static int pattern[] = {1,0,1,0,1,0,0,0,3,0,3,0,3,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0};
  static int patternIndex = 0;
  
  if (currentTime - sosTimer >= 200) {
    int duration = pattern[patternIndex];
    if (duration == 0) {
      digitalWrite(LED_PIN, LOW);
    } else {
      digitalWrite(LED_PIN, HIGH);
    }
    
    patternIndex = (patternIndex + 1) % (sizeof(pattern) / sizeof(pattern[0]));
    sosTimer = currentTime;
  }
}

void scanWiFiNetworks() {
  Serial.println("Scanning for WiFi networks...");
  
  int networks = WiFi.scanNetworks();
  
  if (networks == 0) {
    Serial.println("No networks found");
  } else {
    Serial.printf("Found %d networks:\n", networks);
    for (int i = 0; i < networks; i++) {
      Serial.printf("   %d: %s (%d dBm) %s\n", 
                   i + 1, 
                   WiFi.SSID(i).c_str(), 
                   WiFi.RSSI(i),
                   WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Encrypted");
    }
  }
  Serial.println();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Handle button press
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    if (currentTime - lastButtonPress > 200) {  // Debounce
      buttonPressed = true;
      lastButtonPress = currentTime;
      
      blinkMode = (blinkMode + 1) % 4;
      Serial.print("🔄 Switched to blink mode: ");
      Serial.println(blinkMode);
      
      // Reset timing for new mode
      lastBlink = currentTime;
      digitalWrite(LED_PIN, LOW);
      ledState = false;
    }
  } else if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = false;
  }
  
  // Handle serial commands
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 's' || cmd == 'S') {
      scanWiFiNetworks();
    } else if (cmd == 'r' || cmd == 'R') {
      Serial.println("Restarting...");
      ESP.restart();
    }
  }
  
  // Handle different blink modes
  switch (blinkMode) {
    case 0: // Slow blink
      if (currentTime - lastBlink >= 1000) {
        toggleLED();
        lastBlink = currentTime;
      }
      break;
      
    case 1: // Fast blink
      if (currentTime - lastBlink >= 200) {
        toggleLED();
        lastBlink = currentTime;
      }
      break;
      
    case 2: // Heartbeat
      heartbeatPattern(currentTime);
      break;
      
    case 3: // SOS
      sosPattern(currentTime);
      break;
  }
  
  delay(10);
}
