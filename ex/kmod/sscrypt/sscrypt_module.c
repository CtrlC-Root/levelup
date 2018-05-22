// Linux Kernel
#include <linux/init.h> //__init, __exit
#include <linux/module.h> // MODULE_ macros
#include <linux/kernel.h>

// SSCrypt
#include "sscrypt.h"

// Provide module information. Note: this must be defined in all object files.
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexandru Barbur");
MODULE_DESCRIPTION("A simple and secure encryption module.");
MODULE_VERSION("0.1");

// Instantiate the global module configuration data.
struct ssc_data sscrypt;

/**
 * TODO.
 *
 * @see https://stackoverflow.com/a/6079839/937006
 */
struct file_operations ssc_fops = {
  .owner = THIS_MODULE,
  .read = ssc_device_read,
  .write = ssc_device_write,
  .open = ssc_device_open,
  .release = ssc_device_release
};

/**
 * Initialize the kernel module.
 *
 * @returns returns 0 if successful
 */
static int __init ssc_init(void) {
  // status
  printk(KERN_INFO "sscrypt: init\n");

  // register the character device
  sscrypt.device_major = register_chrdev(0, SSC_DEVICE_NAME, &ssc_fops);
  if (sscrypt.device_major < 0) {
    // quit
   printk(KERN_ALERT "sscrypt: failed to register character device: %d\n", sscrypt.device_major);
   return sscrypt.device_major;
  }

  printk(KERN_INFO "sscrypt: registered character device with major number: %d\n", sscrypt.device_major);

  // register the device class
  sscrypt.device_class = class_create(THIS_MODULE, SSC_DEVICE_CLASS);
  if (IS_ERR(sscrypt.device_class)) {
    // cleanup
    unregister_chrdev(sscrypt.device_major, SSC_DEVICE_NAME);

    // quit
    printk(KERN_ALERT "sscrypt: failed to register device class: %ld\n", PTR_ERR(sscrypt.device_class));
    return PTR_ERR(sscrypt.device_class);
  }

  // register the device driver
  sscrypt.device = device_create(sscrypt.device_class, NULL, MKDEV(sscrypt.device_major, 0), NULL, SSC_DEVICE_NAME);
  if (IS_ERR(sscrypt.device)) {
    // cleanup
    class_destroy(sscrypt.device_class);
    unregister_chrdev(sscrypt.device_major, SSC_DEVICE_NAME);

    // quit
    printk(KERN_ALERT "sscrypt: failed to create device: %ld\n", PTR_ERR(sscrypt.device));
    return PTR_ERR(sscrypt.device);
  }

  // done
  printk(KERN_INFO "sscrypt: ready\n");
  return 0;
}

/**
 * Clean up the kernel module.
 */
static void __exit ssc_exit(void) {
  // unregister device driver
  device_destroy(sscrypt.device_class, MKDEV(sscrypt.device_major, 0));

  // unregister the device class
  class_unregister(sscrypt.device_class);
  class_destroy(sscrypt.device_class);

  // unregister character device
  unregister_chrdev(sscrypt.device_major, SSC_DEVICE_NAME);

  // status
  printk(KERN_INFO "sscrypt: exit\n");
}

// Register the module init and exit functions.
module_init(ssc_init);
module_exit(ssc_exit);
