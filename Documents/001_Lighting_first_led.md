# 001 用 GPIO 子系统点亮第一个LED

## 1. 先上步骤

在 README.md扩展板连线图中可见 Raspberry Pi的 GPIO19 连接着 LED1。

```console
$ cd /sys/class/gpio
$ echo 19 > export
$ cd gpio19
$ echo out > direction
$ echo 0 > value 
```

执行上述操作后，第一盏灯 LED1已经被点亮了。

## 2. 为什么
### 2.1 GPIO 子系统

对于 GPIO模块，内核已经抽象出来 GPIO子系统。随着内核的发展，基本已经把各种驱动的都抽象出了对应的子系统。GPIO子系统利用 sysfs文件系统与用户空间进行交互，也就是上面的操作步骤，请查看这边内核[文档](https://github.com/torvalds/linux/blob/master/Documentation/admin-guide/gpio/sysfs.rst)。

### 2.2 设备树
#### 2.2.1 rpi 设备树概览

对于 GPIO模块，内核已经封装好统一驱动，用户需要改的只有设备树，把资源信息传递给 GPIO 驱动就好了。

```cpp
linux/arch/arm/boot/dts/bcm2711-rpi-4-b.dts   // 板级设备树，定义板级资源
	#include "bcm2711.dtsi"                   // 芯片级设备树，主要定义芯片上控制器
	#include "bcm2835-rpi.dtsi"               // 2835是老rpi使用的芯片，见附录
```

#### 2.2.3 gpio19 pinctrl

[gpio19 pinctrl 在 kernel 中位置](https://github.com/torvalds/linux/blob/master/arch/arm/boot/dts/bcm2711.dtsi#L792)

```cpp
	pwm0_1_gpio19: pwm0_1_gpio19 {
		pin-pwm {
			pins = "gpio19";
			function = "alt5";
			bias-disable;
		};
	};
```

#### 2.2.2 gpio19 节点

[gpio 节点在 kernel 中的位置](https://github.com/torvalds/linux/blob/master/arch/arm/boot/dts/bcm283x.dtsi#L107)

gpio 整个控制器挂在 / 跟节点上的，所以 gpio19 也在其中。

## 附录
* [历代树莓派主芯片型号](https://www.raspberrypi.org/documentation/computers/processors.html)
* [设备树语法概要](../Documents/000_device_tree_syntax.md)
* [raspberry pi 4B 反编译后的设备树文件](../Reference/rpi_4b.dts)
