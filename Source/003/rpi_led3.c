/*!The RPI-4B Device Drive
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       rpi_led3.c
 * @ingroup    rpi_led3
 * @author     tango
 * @date       2021-09-05 
 * @brief      rpi_led3.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include <linux/module.h>
#include <linux/platform_device.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>   

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

static int s_major = 0;
static struct class *s_led_class = NULL;
static struct gpio_desc *s_led_gpio = NULL;


/* //////////////////////////////////////////////////////////////////////////////////////
 * static implementation
 */
static int rpi_led_open(struct inode *inode, struct file *filp)
{
    printk("Open led\n");

    // set output
    gpiod_direction_output(s_led_gpio, 0);

    return 0;
}
static int rpi_led_release(struct inode *inode, struct file *file)
{
    printk("Release led\n");

    return  0
}
static ssize_t rpi_led_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    printk("Read led, nothing to do\n");

    return  0
}
static ssize_t rpi_led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    int ok;
    unsigned char status;

    printk("Write led\n");
    ok = copy_from_user(&status, buf, 1);
    if(ok == -1) printk("Write err\n");

    // set gpio value
    gpiod_set_value(s_led_gpio, (int)status);

    return 1;
}

static const struct file_operations rpi_led_fops = {
    .owner      = THIS_MODULE,
	.read		= rpi_led_read,
	.write		= rpi_led_write,
	.open		= rpi_led_open,
	.release	= rpi_led_release,
};

static int rpi_led_probe(struct platform_device *pdev)
{
    printk("Rpi led probe\n");

    // 从设备树中获取led硬件资源 "led12-gpios = <&gpio 12 GPIO_ACTIVE_LOW>;"
    s_led_gpio = gpiod_get(&pdev->dev, "led12");
    if(IS_ERR(s_led_gpio))
    {
        dev_err(&pdev->dev, "Failed to get led from device tree\n");
        return PTR_ERR(s_led_gpio);
    }

    // 注册file_operations rpi_led_fops 
    s_major = register_chrdev(0, "rpi_led", &rpi_led_fops);

    // create class
    s_led_class = class_create(THIS_MODULE, "rpi_led_class");
    if (IS_ERR(s_led_class)) {
		printk("Failed create rpi_led_class\n");
		unregister_chrdev(major, "rpi_led");
		gpiod_put(s_led_gpio);
		return PTR_ERR(s_led_class);
	}

    // 创建设备节点 /dev/rpi_led0
    device_create(s_led_class, NULL, MKDEV(s_major, 0), NULL, "rpi_led%d", 0);

    return 0;
}

static int rpi_led_remove(struct platform_device *pdev)
{
	device_destroy(s_led_class, MKDEV(s_major, 0));
	class_destroy(s_led_class);
	unregister_chrdev(s_major, "rpi_led");
	gpiod_put(s_led_gpio);
    
    return 0
}

static const struct of_device_id rpi_led_table[] = {
    { .compatible = "rpi_4, led_gpio12" },
    { },
}

static struct platform_driver rpi_led_driver = {
	.probe		= rpi_led_probe,
	.remove		= rpi_led_remove,
	.driver		= {
		.name	= "rpi_led",
		.of_match_table = rpi_led_table,
	},
};

static int __init rpi_led_init(void)
{
	return platform_driver_register(&rpi_led_driver);
}

static void __exit rpi_led_exit(void)
{
	platform_driver_unregister(&rpi_led_driver);
}

module_init(rpi_led_init);
module_exit(rpi_led_exit);

MODULE_AUTHOR("tangaoo");
MODULE_DESCRIPTION("Raspberry Pi 4B led driver using GPIO12");
MODULE_LICENSE("GPL");