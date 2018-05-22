// Linux Kernel
#include <linux/uaccess.h>

// SSCrypt
#include "sscrypt.h"

// SSCrypt global module configuration data.
extern struct ssc_data sscrypt;

// User process has opened the device file.
int ssc_device_open(struct inode *ip, struct file *filp) {
  // done
  return 0;
}

// User process has closed the device file.
int ssc_device_release(struct inode *ip, struct file *filp) {
  // done
  return 0;
}

// Handle a user process read from the device file.
ssize_t ssc_device_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
  return 0;
}

// Handle a user process write to the device file.
ssize_t ssc_device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset) {
  return 0;
}
