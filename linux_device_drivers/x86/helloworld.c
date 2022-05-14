/*
	We all know about plug and play devices. They are handled by the kernel as soon as they
are plugged in. These may be USB or PCI express, or any other auto-discovered devices.
Therefore, other device types also exist, which are not hot-pluggable, and which the kernel
needs to know about prior to being managed. There are I2C, UART, SPI, and other devices
not wired to enumeration-capable buses.
There are real physical buses you may already know: USB, I2S, I2C, UART, SPI, PCI, SATA,
and so on. Such buses are hardware devices named controllers. Since they are a part of SoC,
they can't be removed, are non-discoverable, and are also called platform devices.

	From an SoC point of view, those devices (buses) are connected internally through
dedicated buses, and are most of the time proprietary and specific to the manufacturer.
From the kernel point of view, these are root devices, and connected to nothing. That is
where the pseudo platform bus comes in. The pseudo platform bus, also called platform bus is
a kernel virtual bus for devices that do not seat on a physical bus known to the kernel. In
this chapter, platform devices refer to devices that rely on the pseudo platform bus.
Dealing with platform devices essentially requires two steps:
	-	Register a platform driver (with a unique name) that will manage your devices
	-	Register your platform device with the same name as the driver, and their
	resources, in order to let the kernel know that your device is there
That being said, in this chapter, we will discuss the following:
	-	Platform devices along with their driver
	-	Devices and driver-matching mechanisms in the kernel
	-	Registering platform drivers with devices, as well as platform data
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init helloworld_init(void)
{
	pr_info("Hello World!\n");
	return 0;
}

static void __exit helloworld_exit(void)
{
	pr_info("End of the world!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);
MODULE_AUTHOR("Toan Nguyen (toannguyenkt69@gmail.com)");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello World Module");