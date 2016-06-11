obj-m := hello.o
 
PWD       := `pwd` 
KDIR 	  := /lib/modules/`uname -r`/build
 
all:
	make -C $(KDIR) M=$(PWD) modules
 
clean:
	rm -rf *.o *~ core .*.cmd *.mod.c *.ko ./tmp_version *.order *.symvers
