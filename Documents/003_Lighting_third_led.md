# 003 用自己的驱动点亮第3个LED

## 1. 操作步骤

在 README.md扩展板连线图中可见 Raspberry Pi的 GPIO12 连接着 LED3。

### 1.1 设备树添加 LED 节点

在 `arch/arm/boot/dts/bcm2711-rpi-4-b.dts` 文件跟节点 / 上添加 led_gpio12 节点：

```cpp
/ {
     led_gpio12 {
         compatible = "rpi_4, led_gpio12";
         label = "rpi_led";
         led12-gpios = <&gpio 12 GPIO_ACTIVE_LOW>;
     };
};
```
编译 dtbs，拷贝到 rpi，并重启。

进入`/sys/bus/platform/devices/`目录，已经可以看到 platform 设备 led_gpio12。此时进入 led_gpio12 目录发现并没有匹配的 driver:

```console
pi@raspberrypi:/sys/bus/platform/devices/led_gpio12 $ ls
driver_override  modalias  of_node  power  subsystem  uevent
```

### 1.2 编写 LED 驱动程序

[rpi_led3.c](../Source/4_rpi_led/rpi_led3.c) 是与 led_gpio12 匹配的驱动程序。编译并动态加载驱动：

```console
pi@raspberrypi:~/raspberry_pi_ldd/Source/4_rpi_led $ make
pi@raspberrypi:~/raspberry_pi_ldd/Source/4_rpi_led $ sudo insmod rpi_led3.ko
```

此时会生成`/dev/rpi_led0`节点。再查看 led_gpio12 设备，发现已经有与之匹配的驱动：

```console
pi@raspberrypi:/sys/bus/platform/devices/led_gpio12 $ ls
driver  driver_override  modalias  of_node  power  subsystem  uevent
```

### 1.3 LED 应用程序
[rpi_led3_test.c](../Source/4_rpi_led/rpi_led3_test.c) 是 led 应用程序，通过设备节点 `/dev/rpi_led0` 与驱动程序交互。

```console
pi@raspberrypi:~/raspberry_pi_ldd/Source/4_rpi_led $ gcc rpi_led3_test.c -o rpi_led3_test
pi@raspberrypi:~/raspberry_pi_ldd/Source/4_rpi_led $ sudo ./rpi_led3_test /dev/rpi_led0 on
```

执行上面操作后，LED 被点亮。


## 2. Platform 驱动框架 

[platform 参考](https://github.com/torvalds/linux/blob/master/Documentation/driver-api/driver-model/platform.rst)

platform 是一个虚拟总线，platform 作用是仿照真实总线把那些非总线设备和非总线驱动关联起来。

