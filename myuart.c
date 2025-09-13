#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define MYUART_VENDOR_ID  0x1a86
#define MYUART_PRODUCT_ID 0x55d4

/* 匹配表 */
static const struct usb_device_id myuart_table[] = {
    { USB_DEVICE(MYUART_VENDOR_ID, MYUART_PRODUCT_ID) },
    {}
};
MODULE_DEVICE_TABLE(usb, myuart_table);

static int myuart_probe(struct usb_interface *interface,
                        const struct usb_device_id *id)
{
    printk(KERN_INFO "myuart: device plugged (VID=0x%04x, PID=0x%04x)\n",
           id->idVendor, id->idProduct);
    return 0;
}

static void myuart_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "myuart: device disconnected\n");
}

static struct usb_driver myuart_driver = {
    .name       = "myuart2",
    .id_table   = myuart_table,
    .probe      = myuart_probe,
    .disconnect = myuart_disconnect,
};

static int __init myuart_init(void)
{
    return usb_register(&myuart_driver);
}
static void __exit myuart_exit(void)
{
    usb_deregister(&myuart_driver);
}

module_init(myuart_init);
module_exit(myuart_exit);

MODULE_LICENSE("GPL");
