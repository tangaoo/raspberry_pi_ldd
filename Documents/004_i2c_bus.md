# 004 I2C bus 

```console
$ sudo apt-get install -y i2c-tools
```


## 1. 操作步骤

在 README.md扩展板连线图中可见 Raspberry Pi的 GPIO2 GPIO3（对应I2C-1）连接着 EEPROMG（24c16）芯片。

### 1.1 设备树添加 I2C-1 总线，和 24c16 驱动

在 `arch/arm/boot/dts/bcm2711-rpi-4-b.dts` 文件跟节点 / 上添加 i2c1 节点：

```cpp
&i2c1 {
    pinctrl-names = "default";
    pinctrl-0 = <&i2c1_pins>;
    clock-frequency = <100000>;
    status = "okay";

    eeprom: eeprom@50 {
        compatible = "atmel,24c16";
        pagesize = <16>;
        reg = <0x50>;
    };
};
```

编译 dtbs，拷贝到 rpi，并重启。

进入`/sys/bus/i2c/devices/i2c-1/1-0050`目录，eeprom 文件就是 `24c16`驱动与用户空间交互的设备节点。

```console
$ pi@raspberrypi:/sys/bus/i2c/devices/i2c-1/1-0050 $ ls
$ 1-00500  driver  eeprom  modalias  name  of_node  power  subsystem  uevent
```

### 1.2 用户空间访问驱动

```console
$ pi@raspberrypi:/sys/bus/i2c/devices/i2c-1/1-0050 $ echo "test" > eeprom
$ pi@raspberrypi:/sys/bus/i2c/devices/i2c-1/1-0050 $ cat eeprom
```

## 2. I2C 总线驱动 
### 2.1 I2C 子系统

