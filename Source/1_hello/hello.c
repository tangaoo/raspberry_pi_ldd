#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("tangaoo");
MODULE_DESCRIPTION("A Sample Hello World Module");
MODULE_ALIAS("A Sample module");

static int hello_init(void)
{
	printk(KERN_ALERT"Hello World enter\n");
	// test
//	return -1;
	return 0;
}
static void hello_exit(void)
{
	printk(KERN_ALERT"Hello World exit\n");
}

module_init(hello_init);
module_exit(hello_exit);
