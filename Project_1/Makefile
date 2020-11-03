obj-m += ex_P-4.o
obj-m += asgn_1_jiffies.o
obj-m += asgn_2_seconds.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
