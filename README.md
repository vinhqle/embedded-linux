# embedded-linux
## linux-driver-programming branch
### How to run these project in branch?
- Run command ```make all``` to compile driver. After finishing this step, there are many files will be created in the directory, a file with ".ko" extension will be driver.
- Run command ```sudo insmod <filename>.ko``` to load module driver to system.
- Run command ```sudo rmmod <filename>``` to unload module driver from system.
- Run command ```make clean``` to clean up compiled files in the directory.

### 1/ ex01_simple_module
- This excercise will show how to programming a simple module driver.
- In a module driver, we should have to create 2 potential functions ("device_init" and "device_exit", name of these functions are up to you) that will be used to load and unload driver to kernel system.

### 2/ ex02_led_control_module
- This excercise will show how to programmingg a simple module driver used to control an LED using Raspberry Pi 4.
- In this excercise, we re-use source code of ex01_simple_module and add more code used to set LED state in init function and clear LED state in exit function.
- Requirement:
    + Raspberry Pi 4 board.
    + An LED
- Connection:
    + LED (+) will be connected to GPIO23 (Pin 16) of the Raspberry Pi 4 and another pin of LED will be connected to GND pin of the Raspberry Pi 4.
- BCM2711 micro processor information:
    + GPIO base address (virtual): 0xfe200000
