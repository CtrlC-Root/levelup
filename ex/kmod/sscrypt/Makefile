obj-m += sscrypt.o
sscrypt-objs += sscrypt_api.o

.PHONY: all clean

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
