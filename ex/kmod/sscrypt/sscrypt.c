// Linux Kernel
#include <linux/init.h> //__init, __exit
#include <linux/kernel.h>

// SSCrypt
#include "sscrypt.h"

// Instantiate the global module configuration data.
static struct ssc_data sscrypt;

/**
 * Symmetric encryption key parameter.
 */
module_param(sscrypt.encryption_key, charp, S_IWUGO);
MODULE_PARM_DESC(sscrypt.encryption_key, "The key to encrypt or decrypt messages with.");

/**
 * TODO.
 *
 * @see https://stackoverflow.com/a/6079839/937006
 */
static struct file_operations ssc_fops = {
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
    printk(KERN_ALERT "sscrypt: failed to register device class: %d\n", PTR_ERR(sscrypt.device_class));
    return PTR_ERR(sscrypt.device_class);
  }

  // register the device driver
  sscrypt.device = device_create(sscrypt.device_class, NULL, MKDEV(sscrypt.device_major, 0), NULL, DEVICE_NAME);
  if (IS_ERR(sscrypt.device)) {
    // cleanup
    class_destroy(sscrypt.device_class);
    unregister_chrdev(sscrypt.device_major, SSC_DEVICE_NAME);

    // quit
    printk(KERN_ALERT "sscrypt: failed to create device: %d\n", PTR_ERR(sscrypt.device));
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
  // XXX unregister device driver
  device_destroy(sscrypt.device_class, MKDEV(sscrypt.device_major, 0));

  // XXX unregister the device class
  class_unregister(sscrypt.device_class);
  class_destroy(sscrypt.device_class);

  // unregister character device
  int rc = unregister_chrdev(sscrypt.device_major, SSC_DEVICE_NAME);
  if (rc < 0) {
    printk(KERN_ALERT "sscrypt: failed to unregister character device: %d\n", rc);
  }

  // status
  printk(KERN_INFO "sscrypt: exit\n");
}

// Register the module init and exit functions.
module_init(ssc_init);
module_exit(ssc_exit);
