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

* [001 Lighting the first led using GPIO subsystem 用 GPIO 子系统点亮第一个 LED 灯](Documents/001_Lighting_first_led.md)
* [002 Lighting the second led using LED subsystem 用 LED 子系统点亮第二个 LED 灯](Documents/002_Lighting_another_led.md)
* [003 Lighting the third led using coustom driver 用自己的驱动点亮第三个 LED 灯](Documents/003_Lighting_third_led.md)

