# 000 设备树语法

## 1. 设备树作用

[设备树官方文档](https://www.devicetree.org/specifications/)

设备树主要用来描述硬件资源并传递给驱动调用。相比老方法用 C 语言来描述硬件资源，显然更加简单灵活。

/sys/firmware/devicetree 目录已目录树结构呈现整个 dtb 设备树文件。base 对应跟节点，base 目录下每个子目录代表一个节点，目录下面的每个文件都是一个属性，属性值如果是字符串，用 cat 命令，如果是数值，用 hexdump 命令。/sys/firmware/fdt 就是设备树文件，可以反编译。

uboot 启动内核时，会同时把内核、设备树加载到内存，并且会告诉内核设备树所在的地址。

## 2. 设备树语法
### 2.1 设备树整体结构

```cpp
/dts-v1/;

/memreserve/    0x0000000000000000 0x0000000000001000;
/ {
    memreserve = <0x3b400000 0x4c00000>;
    serial-number = "10000000dbb7bdd5";
    compatible = "raspberrypi,4-model-b", "brcm,bcm2838";
    model = "Raspberry Pi 4 Model B Rev 1.4";
    interrupt-parent = <0x1>;
    #address-cells = <0x2>;
    #size-cells = <0x1>;

	cpus{
	};

	memory{
	};

	chosen{
	};

	aliases{
	};

	peripherals{
	};	
}
```
设备树总体概览如上，一般会有上面这些节点，peripherals 指代各种外设。

### 2.2 设备树格式
#### 2.2.1 DTS 文件布局

```cpp
/dts-v1/;                      // 设备树版本
[memory reservations]          // 格式为: /memreserve/ <address> <length>;
/{
	[property definitions]
	[child nodes] 
};
```

#### 2.2.2 node 节点布局

```cpp
[label:] node-name[@unit-address] { 
	[properties definitions]
	[child nodes]
};
```
node 是设备树的基本单元，[label] 是标号，方便其他地方引用。示例如下：

```cpp
/dts-v1/; 
/{
	uart0: uart@fe001000 {
		compatible="ns16550";
		reg=<0xfe001000 0x100>; 
	};
};
```

可以使用下面 2 种方法来修改 uart@fe001000 这个 node:

```cpp
// 在根节点之外使用 label 引用 node
&uart0 {
	status = “disabled”; 
};
// 或在根节点之外使用全路径 
&{/uart@fe001000} {
	status = “disabled”; 
};
```

#### 2.2.3 properties 格式

* properties 就是 “name=value” 健值对形式。
 * Property 形式1: `[label:] property-name = value;`
 * Property 形式2: `[label:] property-name;`

* value 有多种取值。
 * arrays of cells(1 个或多个 32 位数据, 64 位数据使用 2 个 32 位数据表示) 
 * string(字符串)
 * bytestring(1 个或多个字节)

* demo

```cpp
// Arrays of cells : cell 就是一个 32 位的数据，用尖括号包围起来 
interrupts = <17 0xc>;

// 64bit 数据使用 2 个 cell 来表示，用尖括号包围起来: 
clock-frequency = <0x00000001 0x00000000>;

// A null-terminated string (有结束符的字符串)，用双引号包围起来: 
compatible = "simple-bus";

// A bytestring(字节序列) ，用中括号包围起来:
local-mac-address = [00 00 12 34 56 78]; // 每个 byte 使用 2 个 16 进制数来表示 
local-mac-address = [000012345678]; // 每个 byte 使用 2 个 16 进制数来表示

// 可以是各种值的组合, 用逗号隔开:
compatible = "ns16550", "ns8250";
example = <0xf00f0000 19>, "a strange property format";
```

### 2.3 常用属性
#### 2.3.1 #address-cells、#size-cells
cell 表示一个32位的数值，address-cells 表示用几个32位的数来表示地址，size-cells 表示用几个32位的数来表示size。示例：

```cpp
/{
	#address-cells = <1>; 
	#size-cells = <1>; 
	memory {
		reg = <0x80000000 0x20000000>;
	}; 
};
```
上例表明用一个32位的数表示地址，一个32位的数表示长度。

#### 2.3.2 compatible

* compatible 属性用来与驱动程序匹配。
* compatible 可能有好几个值，表明兼容好几个驱动程序，优先匹配顺序是从左到右。
* 根节点下也有 compatible 属性，用来选择哪一个“machine desc”:一个内核可以支持 machine A，也支 持 machine B，内核启动后会根据根节点的 compatible 属性找到对应的 machine desc 结构体，执行其中的初 始化函数。

#### 2.3.3 model

* model 用来准确地定义这个硬件是什么。

```cpp
/ {
    compatible = "raspberrypi,4-model-b", "brcm,bcm2711";
    model = "Raspberry Pi 4 Model B";
}
```
示例表明此单板兼容"raspberrypi,4-model-b", "brcm,bcm2711"，但是此单板本身是 "Raspberry Pi 4 Model B".

#### 2.3.4 status

用 status 值，可以打开或关闭某个硬件模块。status 取值有okay disable fail fail-sss ，示例如下：

```cpp
&uart1 {
	status = "disabled";
};
```

#### 2.3.5 reg

reg 原本是用来表示寄存器地址，但是在 ARM 架构体系下寄存器和地址是统一编址，所以访问地址和内存是一样的，都用 reg。示例：

```cpp
/dts-v1/;
/{
	#address-cells = <1>; 
	#size-cells = <1>; 
	memory {
		reg = <0x80000000 0x20000000>; 
	};
};
```

#### 2.3.5 name (过时)
	
它的值是字符串，用来表示节点的名字。在跟 platform_driver 匹配时，优先级最低。 compatible 属性在匹配过程中，优先级最高。

#### 2.3.6 device_type (过时)

它的值是字符串，用来表示节点的名字。在跟 platform_driver 匹配时，优先级最低。 compatible 属性在匹配过程中，优先级最高。

### 2.4 常用节点
#### 2.4.1 / 根节点

根节点必须有，示例如下：
```cpp
/dts-v1/;
/{
	compatible = "raspberrypi,4-model-b", "brcm,bcm2711"; // 表明单板支持哪些平台
    model = "Raspberry Pi 4 Model B";  // 指定单板名字，如果compatible 属性相同的话，用 model 来区分单板
	#address-cells = <1>;   // 在它的子节点的 reg 属性中, 使用多少个 u32 整数来描述地址(address)
	#size-cells = <1>;      // 在它的子节点的 reg 属性中, 使用多少个 u32 整数来描述大小(size)
};
```

#### 2.4.2 CPU 节点
一般不需要我们设置，在芯片 .dtsi 文件已经定义好。

#### 2.4.3 memory 节点
需要自己根据实际内存大小定义。示例：

```cpp
memory {
	reg = <0x80000000 0x20000000>; 
};
```

#### 2.4.4 chosen 节点
可以通过设备树文件给内核传入一些参数，这要在 chosen 节点中设置 bootargs 属性 。示例：

```cpp
chosen {
	bootargs = "noinitrd root=/dev/mtdblock4 rw init=/linuxrc console=ttySAC0,115200"; 
};
```

## 3. 编译与反编译设备树
### 3.1 利用内核树编译

```console
make dtbs // 前提设置好 ARCH、CROSS_COMPILE、PATH 环境变量，配置好 .config
```

### 3.2 直接 dtc 编译

内核目录下 scripts/dtc/dtc 是设备树的编译工具，直接使用它的话，包含其他文件时不能使用“#include”， 而必须使用“/incldue”。
编译、反编译的示例命令如下，“-I”指定输入格式，“-O”指定输出格式，“-o”指定输出文件:

```console
./scripts/dtc/dtc -I dts -O dtb -o tmp.dtb arch/arm/boot/dts/xxx.dts // 编译 dts 为 dtb 
./scripts/dtc/dtc -I dtb -O dts -o tmp.dts arch/arm/boot/dts/xxx.dtb // 反编译 dtb 为 dts
```

