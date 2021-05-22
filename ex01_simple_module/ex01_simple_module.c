#include <linux/init.h>
#include <linux/module.h>

static int __init device_init(void){
    printk(KERN_ALERT "Device module is loaded.");
    return 0;
}

static void __exit device_exit(void){
    printk(KERN_ALERT "Device module is unloaded.");
    return;
}

module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinh Quang Le <vinhqle.neobux@gmail.com>");
MODULE_DESCRIPTION("Simple driver module");
MODULE_VERSION("1.0.0");
