#CFILES = kadvice_core.c
#obj-m+=kadvice.o
#obj-m+=kadvice_lsm.o
#obj-m+=shinpei.o
#obj-m+=advice_test2.o
#obj-m+=advice_test1.o
#obj-m+=kadvice_proc_write.o
#obj-m+=advice_inode_permission.o
#obj-m+=kadvice_io.o
#obj-m+=kadvice_sample.o
obj-m+=eye_sample.o
#obj-m+=advice_iotest.o
kadvice-objs := $(CFILES:.c=.o)
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
