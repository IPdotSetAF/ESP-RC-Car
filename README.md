# ESP 8266 RC
this an almost generic firmware project (written for `ESP8266`) for Remote Controlled Cars (RC). 
The remote works over a WIFI connection and in a single-page web application.

This project is still under development and research, so as soon as the hardware is figured out, all the hardware schematics and the PCB (maybe) will be available. 

## Supported Boards:
  - ESP-8266
    - [x] ESP-01
    - [ ] ESP-12
  - ESP-32
    - [ ] ESP-32-WROOM
    - [ ] ESP-32-CAM

## Project structure
  - The project contains the following:
    - Firmware
      - Developed in PlatformIO
      - Arduino Framework
      - Used YMALDuino
      - Used ArduinoJson
      - Used PCF8574
      - [x] Web Server
      - [x] Static File Server
      - [x] Web API
      - [x] mDNS
      - [x] OTA Update
    - Web page static files
      - Used LittleFS
      - Index page `HTML`, `CSS`, `JS` 
      - manifest.json
      - SVG and PNG icons
      - [ ] Config.yml

## Features
  - This firmware currently supports:
    - [x] Head Lights
    - [x] Break Lights
    - [x] Signal Lights
    - [x] Gear System (only `DRIVE`, `NEUTAL`, and `REVERSE` are supported)
    - [x] Horn
    - [ ] Fixed Speed `DC Motor` Drive (`FORWARD` and `REVERSE`)
    - [x] Steering (`Servo` based)
    - [x] Debug Console
  
  - These Features might also be implemented in the near future:
    - [ ] Underglow Lights
    - [ ] Variable Speed `DC Motor` Drive
    - [ ] Steering ('DC Motor' based)
    - [ ] Gear System (`Manual` gear system)
    - [ ] Pneumatic Height Control (`Servo` based)
    - [ ] `Brushless DC Motor` Drive
    - [ ] FPV Camera


## Components:
  
  | Name                       | Image                                            | Count |
  | :---:                      | :---:                                            | :---: |
  | ESP-01                     | <img src="images/esp01.jpg" height="100">        | 1     |
  | Geared hobby RC Motor 1:48 | <img src="images/geared-1-48.webp" height="100"> | 1     |
  | SG90 servo                 | <img src="images/sg90.webp" height="100">        | 1     |
  | L9110 Motor driver         | <img src="images/l9110s.jpg" height="100">       | 1     |
  | AMS1117 3.3v               | <img src="images/ams1117.webp" height="100">     | 1     |
  | PCF8574 I2C IO extender    | <img src="images/pcf8574.jpg" height="100">      | 1     |
  | Red LED                    | <img src="images/red-led.webp" height="100">     | 2     |
  | Yellow LED                 | <img src="images/yellow-led.jpg" height="100">   | 4     |
  | White LED                  | <img src="images/white-led.jpg" height="100">    | 4     |
  | 51Ω Resistor               | <img src="images/51.jpg" height="100">           | 1     |
  | 470Ω Resistor              | <img src="images/470.jpg" height="100">          | 4     |
  | 10KΩ Resistor              | <img src="images/10k.webp" height="100">         | 2     |
  | 10µF Capacitor             | <img src="images/10uf-25v.jpg" height="100">     | 1     |
  | 22µF Capacitor             | <img src="images/22uf.jpg" height="100">         | 1     |
  | 4xAA Battery holder        | <img src="images/bat-holder.webp" height="100">  | 1     |
  
