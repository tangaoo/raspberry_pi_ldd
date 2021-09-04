# raspberry_pi_ldd
Writing device drives on Raspberry Pi 4B step by step.
一步步写树莓派4B的设备驱动。

## Pins 原理图

![Alt text](./Image/001_Shematic_RPI_40_Pin.png?raw=true "Raspberry Pi 4B Pins")

## 扩展板连线表

| 功能 | 扩展板IO | Raspberry Pi IO |
| :-----| :----: | :----: |
| RS485  | RXD   | GPIO15(RXD) |
| -      | TXD   | GPIO14(TXD) |
| -      | EN    | GPIO4 |
| SEGLED | SCLK  | GPIO20 |
| -      | DIO   | GPIO21 |
| -      | RCLK  | GPIO26 |
| TF-Card | MISO | GPIO10(MOSI) |
| -      | MOSI  | GPIO9(MISO) |
| -      | CLK   | GPIO11(CLK) |
| -      | CS    | GPIO28(CE0) |
| LED    | LED1  | GPIO19 |
| -      | LED2  | GPIO16 |
| -      | LED3  | GPIO12 |
| KEY    | KEY1  | GPIO27 |
| -      | KEY2  | GPIO18 |
| -      | KEY3  | GPIO17 |
| EEPROM | CLK   | GPIO3(CLK) |
| -      | DAT   | GPIO2(DAT) |
| BUZZER | BUZZ  | GPIO13 |

## Documents

* [001 Lighting first led/ 点亮第一盏 LED 灯](Documents/001_Lighting_first_led.md)

