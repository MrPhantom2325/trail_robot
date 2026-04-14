# trail_robot

Arduino/ESP32 sketches for a trail robot and sensor monitoring setup.

## Repository contents

- `trail_robo_final.ino`  
  Line-following and obstacle-avoidance robot logic using:
  - L298 motor driver
  - IR line sensors
  - Ultrasonic distance sensor
  - Servo scanning

- `sketch_dec15a/sketch_dec15a.ino`  
  ESP32 IoT sketch using:
  - DHT11 temperature/humidity sensor
  - LCD display
  - HTTP/Web server
  - Firebase RTDB
  - IFTTT webhook integration

## Getting started

1. Open either sketch in the Arduino IDE.
2. Install required board packages and libraries referenced in each sketch.
3. Configure your own Wi-Fi and cloud credentials.
4. Upload to your target board and monitor serial output.

## Note

The sketches currently include hardcoded network/API credentials; replace these with your own secure values before deployment.
