# PCI Linux Driver Programming

- In this branch, I will show all knowledge about developing an pci driver in Linux.

## PCI Driver Knowledge
- The primary userspace utility is ```lspci``` This command can show a lot of useful information.  Run this command with ```-nn``` argument to get all devices with IDs.
- The Linux kernel PCI implementation can be found in the kernel source tree drivers/pci directory. For the driver developers kernel provides a header file linux/pci.h
- APIs to play with a pci driver in Linux
    + Register pci driver device via function ```pci_register_driver(struct pci_driver *)```
    + Unregister pci driver device via function ```pci_unregister_driver(struct pci_driver *)```
    + All 3 below functions will be used to read data from a PCI device:
        ```int pci_read_config_byte(const struct pci_dev *dev, int where, u8 *val)```
        ```int pci_read_config_word(const struct pci_dev *dev, int where, u16 *val)```
        ```int pci_read_config_dword(const struct pci_dev *dev, int where, u32 *val)```
    + All 3 below functions will be used to write data to a PCI device:
        ```int pci_write_config_byte(const struct pci_dev *dev, int where, u8 val)```
        ```int pci_write_config_word(const struct pci_dev *dev, int where, u16 val)```
        ```int pci_write_config_dword(const struct pci_dev *dev, int where, u32 val)```

- NOTE:
    + Driver will not be registered if there is any driver in system using the device.