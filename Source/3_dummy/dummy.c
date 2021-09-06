#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("tango");
MODULE_ALIAS("A Dummy module");
MODULE_DESCRIPTION("A Dummy module, implement read & write");

#define DUMMY_NAME  "dummy_driver"
#define DUMMY_CLASS "dummy_class"

static dev_t dummy_number;
static struct class* dummy_class;
static struct cdev dummy_dev;

static char s_buff[256];
static char* pointer;

static int dummy_open(struct inode* device_file, struct file* inst)
{
	printk("Dummy, open was callback \n");
	return 0;
}

static int dummy_close(struct inode* device_file, struct file* inst)
{
	printk("Dummy, close was callback \n");
	return 0;
}

static ssize_t dummy_read(struct file* f, char* user_buff, size_t cnt, loff_t* offs)
{
	printk("Dummy, read was callback \n");
	size_t min_cnt, not_copyed;

	min_cnt = min(cnt, sizeof(s_buff));
	not_copyed = copy_to_user(user_buff, s_buff, min_cnt);

	return min_cnt - not_copyed;
}

static ssize_t dummy_write(struct file* f, const char* user_buff, size_t cnt, loff_t* offs)
{
	printk("Dummy, write was callback \n");
	size_t min_cnt, not_copyed;

	min_cnt = min(cnt, sizeof(s_buff));
	not_copyed = copy_from_user(s_buff, user_buff, min_cnt);

	return min_cnt - not_copyed;
}

static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= dummy_open,
	.release    = dummy_close,
	.read       = dummy_read,
	.write      = dummy_write,
};

static int __init dummy_init(void)
{
	int ret;
	printk("Kernel enter\n");

	// allocate device number
	if(alloc_chrdev_region(&dummy_number, 0, 1, DUMMY_NAME) < 0)
	{
		printk("%s allocate number failed\n", DUMMY_NAME);
		return -1;
	}
	printk("%s allocate success, major: %d, minor: %d \n", DUMMY_NAME, dummy_number >> 20, dummy_number & 0xFFFFF);

	if((dummy_class = class_create(THIS_MODULE, DUMMY_CLASS)) == NULL)
	{
		printk("create device class faile");
		goto err_class;
	}
	
	// create device file
	if(device_create(dummy_class, NULL, dummy_number, NULL, DUMMY_NAME) == NULL)
	{
		printk("create device file faile");
		goto err_file;
	}

	// init dummy device
	cdev_init(&dummy_dev, &fops);

	// register dummy device to kernel
	if(cdev_add(&dummy_dev, dummy_number, 1) == -1)
	{
		printk("register device file faile");
		goto err_register;
	}

	return 0;

err_register:
	device_destroy(dummy_class, dummy_number);
err_file:
	class_destroy(dummy_class);
err_class:
	unregister_chrdev_region(dummy_number, 1);

return -1;
}

static void __exit dummy_exit(void)
{
	cdev_del(&dummy_dev);
	device_destroy(dummy_class, dummy_number);
	class_destroy(dummy_class);
	unregister_chrdev_region(dummy_number, 1);
	printk("Goodbye, Kernel\n");
}

module_init(dummy_init);
module_exit(dummy_exit);

