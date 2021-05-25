#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinh Quang Le <vinhqle.neobux@gmail.com>");
MODULE_DESCRIPTION("Raspberry Pi LED control driver module");
MODULE_VERSION("1.0.0");

#define GPIO_ADDR_BASE      0xFE200000      // GPIO base address
#define GPIO_MOD_SIZE       0xF3            // GPIO module size
#define GPIO_FUNC_SEL2      0x08            // GPIO function select for gpio 2x
#define GPIO_PIN_OUT_SET    0x1C            // GPIO pin output set for gpio 0~31
#define GPIO_PIN_OUT_CLR    0x28            // GPIO pin output clear for gpio 0~31
#define GPIO_PIN_LEVEL      0x34            // GPIO pin level for gpio 0~31
#define GPIO_PIN_PUP_PDN    0xE8            // GPIO pin set resistor pull up/ pull down

#define GPIO_23             23
#define GPIO_23_FUNC	    1 << ((GPIO_23 % 10) * 3)
#define GPIO_23_INDEX 	    1 << (GPIO_23 % 32)
#define FSEL_23_MASK 	    0b111 << ((GPIO_23 % 10) * 3)
#define GPIO_23_INDEX 	    1 << (GPIO_23 % 32)

static int device_open(struct inode *i, struct file *f);
static int device_release(struct inode *i, struct file *f);
static ssize_t device_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t device_write(struct file *f, const char __user *buf, size_t len,	loff_t *off);

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

unsigned int *GPFSEL2_ADDR = NULL;
unsigned int *GPSET0_ADDR = NULL;
unsigned int *GPCLR0_ADDR = NULL;

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
struct timer_list drv_tmr;

static int ledState = 0;
void handle_timer_func(struct timer_list* tmr){
	ledState = 1 - ledState;
	if(ledState == 0){
		// Clear GPIO 23 level output
		iowrite32(GPIO_23_INDEX, GPCLR0_ADDR);
	} else {
		// Set GPIO 23 level output
		iowrite32(GPIO_23_INDEX, GPSET0_ADDR);
	}

	mod_timer(&drv_tmr, jiffies + 1*HZ);
}

static int device_init(void){
	int ret;
	struct device *dev_ret;

	// Mapping register
	GPFSEL2_ADDR = (unsigned int*)ioremap(GPIO_ADDR_BASE + GPIO_FUNC_SEL2, sizeof(__u32));
	if(NULL == GPFSEL2_ADDR){
		printk(KERN_ERR "Mapping set function failure!\n");
		return -EBUSY;
	}
	GPSET0_ADDR = (unsigned int*)ioremap(GPIO_ADDR_BASE + GPIO_PIN_OUT_SET, sizeof(__u32));
	if(NULL == GPSET0_ADDR){
		printk(KERN_ERR "Mapping set level failure!\n");
		return -EBUSY;
	}
	GPCLR0_ADDR = (unsigned int*)ioremap(GPIO_ADDR_BASE + GPIO_PIN_OUT_CLR, sizeof(__u32));
	if(NULL == GPCLR0_ADDR){
		printk(KERN_ERR "Mapping clear level failure!\n");
		return -EBUSY;
	}

	// Register major number for device
	if ((ret = alloc_chrdev_region(&first, 0, 1, "ex04_file_operation_number")) < 0)
	{
		return ret;
	}
	// Create device class
	if (IS_ERR(cl = class_create(THIS_MODULE, "ex04_file_operation_class")))
	{
		unregister_chrdev_region(first, 1);
		return PTR_ERR(cl);
	}
	if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "ex04_file_operation_device")))
	{
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return PTR_ERR(dev_ret);
	}
	// Initialize character device
	cdev_init(&c_dev, &fops);
	if ((ret = cdev_add(&c_dev, first, 1)) < 0)
	{
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return ret;
	}

	// Read GPFSEL2 register value
	u32 fsel2Read;
	fsel2Read = ioread32(GPFSEL2_ADDR);
	// Set GPIO 23 to OUTPUT
	iowrite32((fsel2Read & ~FSEL_23_MASK) | (GPIO_23_FUNC & FSEL_23_MASK), GPFSEL2_ADDR);

	timer_setup(&drv_tmr, handle_timer_func, 0);
	mod_timer(&drv_tmr, jiffies + 1*HZ);

	printk(KERN_ALERT "LED control driver module is loaded.");
	return 0;
}

static void device_exit(void){
	// Clear GPIO 23 level output
	iowrite32(GPIO_23_INDEX, GPCLR0_ADDR);

	// Unmap register
	iounmap(GPFSEL2_ADDR);
	iounmap(GPSET0_ADDR);
	iounmap(GPCLR0_ADDR);

	del_timer(&drv_tmr);

	// Clear device instance
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);

	printk(KERN_ALERT "LED control driver module is unloaded.");
	return;
}

static int device_open(struct inode *i, struct file *f)
{
	return 0;
}
static int device_release(struct inode *i, struct file *f)
{
	return 0;
}
static ssize_t device_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	return 0;
}

static ssize_t device_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
	return len;
}

module_init(device_init);
module_exit(device_exit);