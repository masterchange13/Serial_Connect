#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MYUART_VENDOR_ID  0x1a86
#define MYUART_PRODUCT_ID 0x55d4
#define DEVICE_NAME "myuart"

struct myuart_dev {
    struct usb_device *udev;
    struct usb_interface *interface;
    unsigned char bulk_in;
    unsigned char bulk_out;
    struct cdev cdev;
    struct mutex io_mutex;
};

static struct class *myuart_class;
static dev_t myuart_devno;
static struct myuart_dev *myuart_device;

/* USB设备匹配表 */
static const struct usb_device_id myuart_table[] = {
    { USB_DEVICE(MYUART_VENDOR_ID, MYUART_PRODUCT_ID) },
    {}
};
MODULE_DEVICE_TABLE(usb, myuart_table);

/* ------------- 字符设备操作 ------------- */
static int myuart_open(struct inode *inode, struct file *file)
{
    file->private_data = myuart_device;
    return 0;
}

static int myuart_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t myuart_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    struct myuart_dev *dev = file->private_data;
    int retval;
    unsigned char *data;

    data = kmalloc(count, GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    mutex_lock(&dev->io_mutex);
    retval = usb_bulk_msg(dev->udev,
                          usb_rcvbulkpipe(dev->udev, dev->bulk_in),
                          data, count,
                          (int *)&count, 5000);
    mutex_unlock(&dev->io_mutex);

    if (retval) {
        kfree(data);
        return retval;
    }

    if (copy_to_user(buf, data, count)) {
        kfree(data);
        return -EFAULT;
    }

    kfree(data);
    return count;
}

static ssize_t myuart_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    struct myuart_dev *dev = file->private_data;
    int retval;
    unsigned char *data;

    data = kmalloc(count, GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    if (copy_from_user(data, buf, count)) {
        kfree(data);
        return -EFAULT;
    }

    mutex_lock(&dev->io_mutex);
    retval = usb_bulk_msg(dev->udev,
                          usb_sndbulkpipe(dev->udev, dev->bulk_out),
                          data, count,
                          (int *)&count, 5000);
    mutex_unlock(&dev->io_mutex);

    kfree(data);
    if (retval)
        return retval;

    return count;
}

static const struct file_operations myuart_fops = {
    .owner = THIS_MODULE,
    .open = myuart_open,
    .release = myuart_release,
    .read = myuart_read,
    .write = myuart_write,
};

/* ------------- USB 驱动 probe / disconnect ------------- */
static int myuart_probe(struct usb_interface *interface,
                        const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    printk(KERN_INFO "myuart: device plugged (VID=0x%04x, PID=0x%04x)\n",
           id->idVendor, id->idProduct);

    myuart_device = kzalloc(sizeof(struct myuart_dev), GFP_KERNEL);
    if (!myuart_device)
        return -ENOMEM;

    mutex_init(&myuart_device->io_mutex);

    myuart_device->udev = usb_get_dev(interface_to_usbdev(interface));
    myuart_device->interface = interface;

    iface_desc = interface->cur_altsetting;

    /* 自动找到 bulk in/out 端点 */
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
        endpoint = &iface_desc->endpoint[i].desc;
        if (usb_endpoint_is_bulk_in(endpoint))
            myuart_device->bulk_in = endpoint->bEndpointAddress;
        if (usb_endpoint_is_bulk_out(endpoint))
            myuart_device->bulk_out = endpoint->bEndpointAddress;
    }

    /* 注册字符设备 */
    cdev_init(&myuart_device->cdev, &myuart_fops);
    myuart_device->cdev.owner = THIS_MODULE;
    cdev_add(&myuart_device->cdev, myuart_devno, 1);

    usb_set_intfdata(interface, myuart_device);

    printk(KERN_INFO "myuart: device ready /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void myuart_disconnect(struct usb_interface *interface)
{
    struct myuart_dev *dev = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);

    cdev_del(&dev->cdev);
    usb_put_dev(dev->udev);
    kfree(dev);

    printk(KERN_INFO "myuart: device disconnected\n");
}

/* ------------- USB 驱动结构体 ------------- */
static struct usb_driver myuart_driver = {
    .name = "myuart2",
    .id_table = myuart_table,
    .probe = myuart_probe,
    .disconnect = myuart_disconnect,
};

/* ------------- 模块入口/出口 ------------- */
static int __init myuart_init(void)
{
    int retval;

    /* 创建设备号和class */
    alloc_chrdev_region(&myuart_devno, 0, 1, DEVICE_NAME);

    myuart_class = class_create(DEVICE_NAME);
    if (IS_ERR(myuart_class)) {
        printk(KERN_ERR "myuart: failed to create class\n");
        unregister_chrdev_region(myuart_devno, 1);
        return PTR_ERR(myuart_class);
    }
        
    device_create(myuart_class, NULL, myuart_devno, NULL, DEVICE_NAME);

    retval = usb_register(&myuart_driver);
    if (retval) {
        printk(KERN_ERR "myuart: usb_register failed\n");
        device_destroy(myuart_class, myuart_devno);
        class_destroy(myuart_class);
        unregister_chrdev_region(myuart_devno, 1);
    }

    return retval;
}

static void __exit myuart_exit(void)
{
    usb_deregister(&myuart_driver);
    device_destroy(myuart_class, myuart_devno);
    class_destroy(myuart_class);
    unregister_chrdev_region(myuart_devno, 1);
}

module_init(myuart_init);
module_exit(myuart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Master Change");
MODULE_DESCRIPTION("USB MyUART driver with bulk read/write");
