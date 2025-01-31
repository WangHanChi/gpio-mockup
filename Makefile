PWD := $(shell pwd) 
KVERSION := $(shell uname -r)
KERNEL_DIR = /usr/src/linux-headers-$(KVERSION)/

MODULE_NAME = gpio-mockup
obj-m := $(MODULE_NAME).o

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

insert:
	sudo insmod $(MODULE_NAME).ko

remove:
	sudo rmmod $(MODULE_NAME)

list:
	sudo lsmod | grep $(MODULE_NAME)

# Making `sudo modprobe MODULE_NAME` possible. Even when not in this directory
install:
	sudo install --mode=644 $(MODULE_NAME).ko /lib/modules/$(KVERSION)/
	sudo depmod --all  # update "database"

uninstall:
	sudo rm -f /lib/module/$(KVERSION)/$(MODULE_NAME).ko
	sudo depmod --all  # also update "database"

format:
	clang-format -i $(MODULE_NAME).c

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean