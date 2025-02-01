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

format:
	clang-format -i $(MODULE_NAME).c

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
