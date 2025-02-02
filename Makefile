PWD := $(shell pwd) 
KVERSION := $(shell uname -r)
KERNEL_DIR = /usr/src/linux-headers-$(KVERSION)/

MODULE_NAME = gpio-mockup
obj-m := $(MODULE_NAME).o

compile:
	make -C $(KERNEL_DIR) M=$(PWD) modules

insert: compile
	sudo insmod $(MODULE_NAME).ko

remove:
	sudo rmmod $(MODULE_NAME)

list:
	lsmod | grep $(MODULE_NAME)

install: compile
	sudo make -C $(KERNEL_DIR) M=$(PWD) modules_install
	sudo depmod -A

format:
	clang-format -i $(MODULE_NAME).c

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean