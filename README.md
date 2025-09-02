 Health and Environment Monitoring System  

This project is an Arduino-based health and environment monitoring system. It uses sensors to track vital signs and environmental conditions, providing real-time feedback on a display.  

 Features
- Heart Rate & SpO2 Monitoring: Uses MAX30105 sensor with SpO2 algorithm.  
- Humidity Monitoring: DHT22 sensor to check room humidity.  
- Real-Time Display: Results shown on HCDisplay graphical screen.  
- Status Detection: Classifies readings as *Normal* or *Dangerous* based on thresholds.  

 Hardware Components
- Arduino Uno  
- MAX30105 Pulse Oximeter Sensor  
- DHT22 Humidity Sensor  
- HCDisplay (Graphical LCD)  
- LEDs for indication  

 Software & Libraries
- `HCDisplay.h`  
- `MAX30105.h`  
- `spo2_algorithm.h`  
- `Wire.h`  

 How It Works
1. Humidity values are read from the DHT22 sensor and displayed as Low / Normal / Dangerous.  
2. MAX30105 measures IR & Red light to calculate heart rate and SpO2 using the `spo2_algorithm`.  
3. Values are shown on the display in real time.  
4. System flags dangerous conditions if HR < 60 bpm or SpO2 < 90%.  

 Applications
- Remote health monitoring.  
- Smart healthcare solutions for patients.  
- Environmental monitoring in medical facilities.  

