#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>

#define WIFI_DRIVER "rtl8188ee_drv"

// Vendor ID & Product ID of device
static struct pci_device_id wifi_driver_id_table[] = {
    { PCI_DEVICE(0x10EC, 0x8179) },
    {0,}
};

MODULE_DEVICE_TABLE(pci, wifi_driver_id_table);

static int wifi_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void wifi_driver_remove(struct pci_dev *pdev);

// PCI device instance
static struct pci_driver swifi_driver = {
    .name = WIFI_DRIVER,
    .id_table = wifi_driver_id_table,
    .probe = wifi_driver_probe,
    .remove = wifi_driver_remove
};

static int __init wifi_device_init(void){
    return pci_register_driver(&swifi_driver);
}

static void __exit wifi_device_exit(void){
    pci_unregister_driver(&swifi_driver);
    return;
}

static int wifi_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent){
    u16 vendor, device;
    pci_read_config_word(pdev, PCI_VENDOR_ID, &vendor);
    pci_read_config_word(pdev, PCI_DEVICE_ID, &device);
    printk(KERN_ALERT "%s: device info [vid,pid] = [%04x,%04x]\n", WIFI_DRIVER, vendor,device);

    return 0;
}

static void wifi_driver_remove(struct pci_dev *pdev){

}

module_init(wifi_device_init);
module_exit(wifi_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinh Quang Le <vinhqle.neobux@gmail.com>");
MODULE_DESCRIPTION("Simple Wifi driver module");
MODULE_VERSION("1.0.0");