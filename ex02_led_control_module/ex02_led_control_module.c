#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/io.h>

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

unsigned int *GPFSEL2_ADDR = NULL;
unsigned int *GPSET0_ADDR = NULL;
unsigned int *GPCLR0_ADDR = NULL;

static int device_init(void){
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

    // Read GPFSEL2 register value
    __u32 fsel2Read;
    fsel2Read = ioread32(GPFSEL2_ADDR);
    // Set GPIO 23 to OUTPUT
    iowrite32((fsel2Read & ~FSEL_23_MASK) | (GPIO_23_FUNC & FSEL_23_MASK), GPFSEL2_ADDR);
    // Set GPIO 23 level output
    iowrite32(GPIO_23_INDEX, GPSET0_ADDR);

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

    printk(KERN_ALERT "LED control driver module is unloaded.");
    return;
}

module_init(device_init);
module_exit(device_exit);