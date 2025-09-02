#include <HCDisplay.h>
#include <Options.h>
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

#define DHT22_PIN A5  // Pin for DHT22 sensor

// Struct to hold sensor data and configurations
struct SensorData {
    int32_t spo2;            // Changed to int32_t
    int8_t validSPO2;
    int32_t heartRate;       // Changed to int32_t
    int8_t validHeartRate;
    uint16_t irBuffer[3];    // Reduced buffer size for saving memory
    uint16_t redBuffer[3];   // Reduced buffer size for saving memory
    int32_t bufferLength = 3;  // Adjusted to match the new buffer size
};

SensorData sensorData;  // Only one instance for encapsulated sensor data

HCDisplay hcDisplay;    // Instance of the HCDisplay library
MAX30105 particleSensor;  // Instance of MAX30105 library

// Define display pins
#define RS 10
#define pulseLED 11
#define readLED 13

// Variables for display dimensions
unsigned int maxX, maxY;

void setup() {
    hcDisplay.Init(RS);         // Initialize the display
    pinMode(DHT22_PIN, INPUT);  // Set DHT22 pin as input

    // Get the screen’s X & Y resolution
    maxX = hcDisplay.ResX() - 1;
    maxY = hcDisplay.ResY() - 1;

    hcDisplay.Rect(0 , 0, maxX, maxY, OUTLINE, 1);
    hcDisplay.Rect(3 , 3, maxX - 3, maxY - 3, OUTLINE, 1);

    Serial.begin(115200);
    pinMode(pulseLED, OUTPUT);
    pinMode(readLED, OUTPUT);

    // Initialize MAX30105 sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
        Serial.println(F("MAX30105 was not found. Please check wiring/power."));
        while (1);
    }

    Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));
    while (Serial.available() == 0);
    Serial.read();

    // Sensor configuration
    particleSensor.setup(60, 4, 2, 100, 411, 4096);  // brightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange
}

void loop() {
    int dhtValue = analogRead(DHT22_PIN);
    displayHumidityStatus(dhtValue);

    acquireSensorData();

    processHeartRateAndSpo2(sensorData);  // Process the heart rate and SpO2 data

    delay(200);
}

void displayHumidityStatus(int dhtValue) {
     serial.Clear();
    if (dhtValue < 50) {
         serial.Pos(0, 1);
         serial.Print(f("The humidity"));
         serial.Pos(2, 18);
         serial.Print(f("is low"));
    } else if (dhtValue > 200 && dhtValue < 300) {
         serial.Pos(44, 8);
         serial.Println(f("you are"));
         serial.Pos(24, 18);
        serial.Print(f("in dangerous"));
    } else {
         serial.Pos(44, 8);
       serial.Print(f("The humidity"));
        serial.Pos(24, 18);
         serial.Print(f("is normal"));
    }
}

void acquireSensorData() {
    // Fill the buffer with IR and red values
    for (int i = 0; i < sensorData.bufferLength; i++) {
        while (!particleSensor.available())
            particleSensor.check();

        sensorData.redBuffer[i] = particleSensor.getRed();
        sensorData.irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample();
    }
}

void processHeartRateAndSpo2(SensorData &data) {
    // Calculate heart rate and SpO2
    maxim_heart_rate_and_oxygen_saturation(data.redBuffer, data.irBuffer, data.bufferLength, 
                                            &data.spo2, &data.validSPO2, &data.heartRate, &data.validHeartRate);

    if (data.validHeartRate && data.validSPO2) {
      serial.Clear();
         serial.Pos(0, 0);
         serial.Print(f("HR="));
        serial.Print(f(data.heartRate);

       serial.Pos(0, 10);
        serial.Print(f("SPO2="));
         serial.Print(f(data.spo2);

        if (data.heartRate < 60 || data.spo2 < 90) {
          serial.Pos(44, 8);
             serial.Print(f("you are"));
             serial.Pos(24, 18);
             serial.Print(f("in dangerous"));
        } else if (data.heartRate >= 60 && data.heartRate <= 100 && data.spo2 >= 95) {
           serial.Pos(44, 8);
             serial.Print(f("you are"));
           serial.Pos(24, 18);
            serial.Print(f("normal"));
        }
    }
}
