obj-m := helloworld.o

#obj-m := hellowolrd.o: obj-m lists modules we want to build. For each <filename>.o, the build system will look for a <filename>.c to build. obj-m is used to build a module, whereas obj-y will result in a built-in object

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

#KERNELDIR is the location of the prebuilt kernel source

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@

#M=$(shell pwd): This is relevant to the kernel build system. The kernel Makefile uses this variable to locate the directory of the external module to build. Your .c files should be placed

#all default: modules: This line instructs the make utility to execute the modules target, whether all or default targets, which are classical targets when it comes to building user apps. In other words, make default or make all or simply make commands will be translated into make modules

#modules modules_install help clean:: This line represents the list target valid in this Makefile

#$(MAKE) -C $(KERNELDIR ) M=$(shell pwd) $@: This is the rule to be executed for each target enumerated above. $@ will be replaced with the name of the target that caused the rule to run. In other words, if one calls make modules, $@ will be replaced with modules, and the rule will become: $(MAKE) -C $(KERNELDIR ) M=$(shell pwd) module
