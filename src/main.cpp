#include "Arduino.h"
#include "ArduinoJson.h"
#include "TFT_eSPI.h"
#include "SPI.h"
#include "BluetoothSerial.h"
#include "esp_bt_device.h"

#define RED_PIN   14
#define GREEN_PIN 12
#define BLUE_PIN  13

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
TFT_eSPI tft = TFT_eSPI();

String fullMessage = "";
unsigned long lastMessageTime = 0;
unsigned long messageTimeout = 5000;
bool isTimeoutActive = false;

int red = 0;
int green = 0;
int blue = 0;

void drawKey(String key) {
  tft.fillRect(0, 0, tft.width(), 20, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println(key);
}

void drawFullMessage(String message) {
  tft.fillRect(0, 40, tft.width(), tft.height() - 40, TFT_BLACK);
  tft.setCursor(0, 40);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println(message);
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP_BT_Client");
  Serial.println("The device started, now you can pair it with bluetooth!");

  const u_int8_t* mac = esp_bt_dev_get_address();
  if (mac) {
    Serial.printf("Device MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  } else {
    Serial.println("Failed to get device MAC Address");
  }
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  tft.init();
  tft.fillScreen(TFT_BLACK);
  drawFullMessage("Waiting for\ninput...");
}

void loop() {
  static String buffer = "";
  while (SerialBT.available()) {
    char c = (char)SerialBT.read();
    buffer += c;

    if (c == '}') { // Test-purposes!!! Replace with proper check to see if message is complete
      Serial.println("Received: " + buffer); 
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, buffer);
      if (error) {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        buffer = "";
        return;
      }
  
      String key = doc["key"];
      String type = doc["type"];

      if (type == "char" || key == "space" || key == "backspace") {
        if (fullMessage.length() >= 50 && key == "backspace") {
          fullMessage = fullMessage.substring(0, fullMessage.length() - 1);
          drawFullMessage(fullMessage);
        } else if (fullMessage.length() < 50) {
          if (key == "space") {
            fullMessage += " ";
          } else if (key == "backspace") {
            fullMessage = fullMessage.substring(0, fullMessage.length() - 1);
          } else {
            fullMessage += key;
          }
          drawFullMessage(fullMessage);
        }
        isTimeoutActive = true;
        lastMessageTime = millis();
      }

      if (type == "name") {
        if (key == "left") {
          red = 255;
          green = 0;
          blue = 0;
        } else if (key == "right") {
          red = 0;
          green = 255;
          blue = 0;
        } else if (key == "up") {
          red = 0;
          green = 0;
          blue = 255;
        } else if (key == "down") {
          red = 255;
          green = 255;
          blue = 0;
        }
        analogWrite(RED_PIN, red);
        analogWrite(GREEN_PIN, green);
        analogWrite(BLUE_PIN, blue);
      }
  
      drawKey("Key: " + key);

      buffer = "";
    }
  }
  
  if ((millis() - lastMessageTime > messageTimeout) && isTimeoutActive) {
    fullMessage = "";
    drawFullMessage("Waiting for\ninput...");
    isTimeoutActive = false;
  }
  
  delay(20);
}
