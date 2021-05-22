# embedded-linux
## linux-driver-programming branch
### 1/ ex01_simple_module
- This excercise will show how to programming a simple module driver.
- In a module driver, we should have to create 2 potential functions ("device_init" and "device_exit", name of these functions are up to you) that will be used to load and unload driver to kernel system.
- How to run this excercise:
    + Run command ```make all``` to compile driver. After finishing this step, there are many files will be created in the directory, a file with ".ko" extension will be driver.
    + Run command ```sudo insmod <filename>.ko``` to load module driver to system.
    + Run command ```sudo rmmod <filename>``` to unload module driver from system.
    + Run command ```make clean``` to clean up compiled files in the directory.
