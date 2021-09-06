obj-m := dev_nr.o

CURRENT_PATH := $(shell pwd)
# LINUX_KERNEL := $(shell uname -r)
# LINUX_KERNEL_PATH := /usr/src/linux-headers-$(LINUX_KERNEL)
LINUX_KERNEL_PATH := /home/pi/linux-rpi-4.19.y-rt

all:
	$(MAKE) -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean

