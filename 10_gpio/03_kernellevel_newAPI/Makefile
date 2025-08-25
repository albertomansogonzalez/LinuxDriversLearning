obj-m += 01_blinking_API_gpiod_.o   #poner nombre del fichero

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
