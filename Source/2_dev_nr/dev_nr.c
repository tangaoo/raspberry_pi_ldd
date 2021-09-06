#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("tango");
MODULE_ALIAS("A Sample module");
MODULE_DESCRIPTION("Register a device nr, and implement some callback function");

static int dev_nr_open(struct inode* device_file, struct file* inst)
{
	printk("dev_nr, open was callback \n");
	return 0;
}

static int dev_nr_close(struct inode* device_file, struct file* inst)
{
	printk("dev_nr, close was callback \n");
	return 0;
}

static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= dev_nr_open,
	.release    = dev_nr_close,
};

#define MYMAJOR 90

static int __init dev_nr_init(void)
{
	int ret;
	printk("kernel enter\n");

	// register device nr
	ret = register_chrdev(MYMAJOR, "tango_nr", &fops);
	if (ret == 0) printk("dev_nr, register major: %d, minor: %d \n", MYMAJOR, 0);
	else if (ret > 0) printk("dev_nr, register major: %d, minor: %d \n", ret>>20, ret&0xFFFFF);
	else 
	{
		printk("register failed \n");
		return -1;
	}

	return 0;
}
static void __exit dev_nr_exit(void)
{
	unregister_chrdev(MYMAJOR, "tango_nr");
	printk("exit kernel\n");
}

module_init(dev_nr_init);
module_exit(dev_nr_exit);
