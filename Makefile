obj-m := heartbeat.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default: clean
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	- rm -f heartbeat.o *.ko
