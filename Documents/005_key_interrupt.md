# 005 中断方式实现按键  

## 1. 设备树配置
参考`gpio-keys.txt`。
```cpp
keys {
    compatible = "gpio-keys";
    #address-cells = <1>;
    #size-cells = <0>;
    autorepeat;
    button@17 {
        label = "green button";
        linux,code = <28>;
        gpios = <&gpio 27 GPIO_ACTIVE_LOW>;
    };
```
参照README表格可知key1连接gpio27。然后编译拷贝。
```console
$ make dtbs
$ sudo cp arch/arm/boot/dts/*.dtb /boot/
```

## 2. 应用程序
安装evtest，启动evtest。
```console
$ sudo apt install evtest
$ sudo evtest
```


