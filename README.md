# raspberry_pi_ldd
Writing device drives on Raspberry Pi 4B step by step.

一步步写树莓派4B的设备驱动。

## Pins 原理图

![Alt text](./Image/001_Shematic_RPI_40_Pin.png?raw=true "Raspberry Pi 4B Pins")

## 扩展板连线表

下表是自定制的扩展板与 Raspberry Pi 连线图。后面的文档、程序均引用此连线图定义。

| 功能 | 扩展板IO | Raspberry Pi IO |
| :-----| :----: | :----: |
| UART   | RXD   | GPIO15(RXD) |
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

## 开启调试串口
* 使能 UART 登陆
```console
sudo raspi-config
```
* 更改 `cmdline.txt`
修改第一个console：console=ttyS0,115200；移除第二个console：console=tty1；移除quiet字段。
```
console=ttyS0,115200 root=PARTUUID=fbf6707b-02 rootfstype=ext4 elevator=deadline fsck.repair=yes rootwait splash plymouth.ignore-serial-consoles ip=192.168.3.250
```

## Documents

* [001  GPIO 子系统。](Documents/001_Lighting_first_led.md)
* [002  LED 子系统。](Documents/002_Lighting_another_led.md)
* [003  Platform 驱动点亮 LED 灯。](Documents/003_Lighting_third_led.md)
* [004  I2C 总线访问 eeprom 设备。](Documents/004_i2c_bus.md)
* [005  GPIO 中断之按键驱动。](Documents/005_key_interrupt.md)

