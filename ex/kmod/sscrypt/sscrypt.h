#pragma once

// Linux Kernel
#include <linux/device.h> // struct class, struct device
#include <linux/fs.h> // struct inode, struct file

/**
 * Character device name.
 */
#define SSC_DEVICE_NAME "sscrypt"

/**
 * Device class.
 */
#define SSC_DEVICE_CLASS "sscrypt"

/**
 * Magic string for encrypted text.
 */
#define SSC_MAGIC "SSC"

/**
 * SSCrypt module configuration and data.
 */
struct ssc_data {
  // Parameters
  char *encryption_key;

  // Device major number
  int device_major;

  // Device class
  struct class *device_class;

  // Device driver
  struct device *device;
};

/**
 * User process has opened the device file.
 */
int ssc_device_open(struct inode *ip, struct file *filp);

/**
 * User process has closed the device file.
 */
int ssc_device_release(struct inode *ip, struct file *filp);

/**
 * Handle a user process read from the device file.
 */
ssize_t ssc_device_read(struct file *filp, char *buffer, size_t length, loff_t *offset);

/**
 * Handle a user process write to the device file.
 */
ssize_t ssc_device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset);
