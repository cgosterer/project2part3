obj-y := elevatorCalls.o
obj-m := elevatorSim.o
elevatorSim-y := elevator.o init.o eleproc.o floors.o

PWD := $(shell pwd)
KDIR := /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -f *.o *.ko *.mod.* Module.* modules.*
