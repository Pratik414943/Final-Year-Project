#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define OLED_I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class PMBus {
public:
    void begin() {
        Wire.begin();
    }
       float getVoltageIn(uint8_t address) {
        Wire.beginTransmission(address);
        Wire.write(0x88); 
        Wire.endTransmission(false);
        Wire.requestFrom(address, 2);
        uint16_t value = Wire.read() << 8;
        value |= Wire.read();
        return value * 0.0041;
    }
       float getVoltageOut(uint8_t address) {
        Wire.beginTransmission(address);
        Wire.write(0x8B); 
        Wire.endTransmission(false);

        Wire.requestFrom(address, 2);
        uint16_t value = Wire.read() << 8;
        value |= Wire.read();
        return value * 0.0041; 
    }
       float getTemperature(uint8_t address) {
        Wire.beginTransmission(address);
        Wire.write(0x8D); 
        Wire.endTransmission(false);

        Wire.requestFrom(address, 2);
        uint16_t value = Wire.read() << 8;
        value |= Wire.read();
        return (value *0.0005); 
    }
    float getCurrent(uint8_t address) {
        Wire.beginTransmission(address);
        Wire.write(0x8C); 
        Wire.endTransmission(false);

        Wire.requestFrom(address, 2);
        uint16_t value = Wire.read() << 8;
        value |= Wire.read();
        return value * 0.0001; 
    }
    float getFanSpeed(uint8_t address) {
        Wire.beginTransmission(address);
        Wire.write(0x90); 
        Wire.endTransmission(false);

        Wire.requestFrom(address, 2);
        uint16_t value = Wire.read() << 8;
        value |= Wire.read();
        return value; 
    }
    
};

PMBus pmbus;

void setup() {
      Serial.begin(9600);
  
if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  pmbus.begin();
    Wire.begin();
}
void loop() {
    float voltageIn = pmbus.getVoltageIn(0x5B);
    float voltageOut = pmbus.getVoltageOut(0x5B);
    float temperature = pmbus.getTemperature(0x5B);
    float current = pmbus.getCurrent(0x5B);
     float fanSpeed = pmbus.getFanSpeed(0x5B);

readAndPrintI2CDevice(0x5B, 0x9A, "Model ID");
    
    Serial.print("Voltage In: ");
    Serial.println(voltageIn, 4); 
    Serial.print("Voltage Out: ");
    Serial.println(voltageOut, 4);
    Serial.print("Temperature: ");
    Serial.println(temperature, 2);
    Serial.print("Current: ");
    Serial.println(current, 4);
     Serial.print("Fan Speed: ");
    Serial.println(fanSpeed);
   delay(5000); 
}

void readAndPrintI2CDevice(uint8_t address, uint8_t registerAddress, const char* label) {
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    Wire.endTransmission(false);
  
    Wire.requestFrom(address, (uint8_t) 8, (uint8_t) true);

    Serial.print(label);
    Serial.print(": ");
    while (Wire.available()) {
        char c = Wire.read();
        Serial.print(c);
    }
    Serial.println();
}
