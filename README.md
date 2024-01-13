# ESP 8266 RC
this an almost generic firmaware project (writen for `ESP8266`) for Remote Controlled Cars (RC). 
this firmware currently supports `ESP-01` module.

The remote works over WIFI connection and in a single page web application.

- The project contains the following:
  - Firmware
    - Developed in platformio
    - Arduino Framework
  - Web page static files
    - LittleFS

- This firmware currently supports:
  - [x] Head Lights
  - [x] Break Lights
  - [x] Signal Lights
  - [x] Gear System (only `DRIVE`, `NEUTAL`, `REVERSE` are supported)
  - [x] Horn (single shot)
  - [ ] Fixed Speed `DC Motor` Drive (`FORWARD` , `REVERSE`)
  - [x] Steering (`Servo` based)

- These Features might also be implemented in neer future:
  - [ ] Underglow Lights
  - [ ] Variable Speed `DC Motor` Drive
  - [ ] Horn (Continuous)
  - [ ] Steering ('DC Motor' based)
  - [ ] Gear System (`Manual` gear system)
  - [ ] Penumatic Height Control (`Servo` based)
  - [ ] `Brushless Motor` Drive
  - [ ] FPV Camera
  - [ ] Debug Console

- Supported Boards:
  - ESP-8266
    - [x] ESP-01
    - [ ] ESP-12-F
    - [ ] ESP-12-E
  - ESP-32
    - [ ] ESP-32-WROOM

  
