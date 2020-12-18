KERN_DIR=/home/frank/linux-3.0.1

all:
	make -C $(KERN_DIR) M=`pwd` modules
clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order
obj-m  +=myled2.o
