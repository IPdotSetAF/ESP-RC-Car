# ESP 8266 RC
this an almost generic firmaware project (writen for `ESP8266`) for Remote Controlled Cars (RC). 
The remote works over WIFI connection and in a single page web application.

## Supported Boards:
  - ESP-8266
    - [x] ESP-01
    - [ ] ESP-12-F
    - [ ] ESP-12-E
  - ESP-32
    - [ ] ESP-32-WROOM

## Project structure
  - The project contains the following:
    - Firmware
      - Developed in platformio
      - Arduino Framework
      - Used YMALDuino
      - Used ArduinoJson
      - Used PCF8574
      - [x] Web Server
      - [x] Static File Server
      - [x] Web Api
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
    - [x] Gear System (only `DRIVE`, `NEUTAL`, `REVERSE` are supported)
    - [ ] Horn (single shot)
    - [ ] Fixed Speed `DC Motor` Drive (`FORWARD` , `REVERSE`)
    - [x] Steering (`Servo` based)
    - [ ] Debug Console
  
  - These Features might also be implemented in neer future:
    - [ ] Underglow Lights
    - [ ] Variable Speed `DC Motor` Drive
    - [ ] Horn (Continuous)
    - [ ] Steering ('DC Motor' based)
    - [ ] Gear System (`Manual` gear system)
    - [ ] Penumatic Height Control (`Servo` based)
    - [ ] `Brushless DC Motor` Drive
    - [ ] FPV Camera


## Components:
  
  | Name                       | Image | Count |
  | :---:                      | :---: | :---: |
  | ESP-01                     |       | 1     |
  | Geared hobby RC Motor 1:48 |       | 1     |
  | SG90 servo                 |       | 1     |
  | L9110 Motor driver         |       | 1     |
  | AMS1117 3.3v               |       | 1     |
  | Small speaker (toy)        |       | 1     |
  | PCF8574 I2C IO extender    |       | 1     |
  | Red LED                    |       | 2     |
  | Yellow LED                 |       | 4     |
  | White LED                  |       | 4     |
  | xxxΩ Resistor              |       | 0     |
  | 10µF Capacitor             |       | 1     |
  | 22µF Capacitor             |       | 1     |
  | 4xAA Battery holder        |       | 1     |
  
