# 002 用 LED 子系统点亮另一个LED

## 1. 操作步骤

在 README.md扩展板连线图中可见 Raspberry Pi的 GPIO16 连接着 LED2。

### 1.1 修改设备树

在 `arch/arm/boot/dts/bcm2711-rpi-4-b.dts` 文件 147 行后添加 led_gpio16 节点：

```cpp
     led_gpio16: led_gpio16 {
         label = "led_gipo16";
         linux,default-trigger = "default-on";
         gpios = <&gpio 16 GPIO_ACTIVE_LOW>;
     };
```

编译设备树，并拷贝到 boot 目录。 [rpi 内核编译文档参考](https://www.raspberrypi.org/documentation/computers/linux_kernel.html)

```console
$ make dtbs
$ sudo cp arch/arm/boot/dts/*.dtb /boot/
```

重启后，LED2 已经被点亮了，同时在 `/sys/class/leds` 目录下可以看到 led_gipo16 设备。

```console
pi@raspberrypi:/sys/class/leds/led_gipo16 $ ls
brightness  device  max_brightness  power  subsystem  trigger  uevent
```

trigger 设置点亮 LED 的触发器，如下设置 heartbeat 后 LED 会心跳闪烁。

```console
$ echo heartbeat > trigger
```

## 2. 为什么
### 2.1 LED 子系统

对于 LED 模块，内核实现了通用 LED 驱动，抽象出来了 LED 子系统。LED 子系统利用 sysfs 文件系统与用户空间进行交互，也就是上面的操作步骤，请查看这边内核[文档](https://github.com/torvalds/linux/blob/master/Documentation/leds/leds-class.rst)。

### 2.2 LED 设备树节点与 LED 驱动程序的关联

我们在设备树中添加的 led_gpio16 节点是 leds 节点的子节点。leds 节点是挂在 / 根节点上的，同时在[该文件](https://github.com/torvalds/linux/blob/master/arch/arm/boot/dts/bcm2835-rpi.dtsi#L4)中可以看到 leds 的 `compatible = "gpio-leds" `。

设备树中指定的硬件资源就是通过 `compatible = "gpio-leds" ` 匹配上特定[驱动程序](https://github.com/torvalds/linux/blob/master/drivers/leds/leds-gpio.c#L188)。

下一篇会从源码分析 GPIO LED 子系统。
