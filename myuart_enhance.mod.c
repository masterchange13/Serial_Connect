#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x18\x00\x00\x00\x65\xfe\x0f\x5b"
	"device_create\0\0\0"
	"\x1c\x00\x00\x00\x3a\x81\xfa\xaf"
	"usb_register_driver\0"
	"\x18\x00\x00\x00\xad\x2f\xcf\x4b"
	"device_destroy\0\0"
	"\x18\x00\x00\x00\x8f\xc9\x3e\x5a"
	"class_destroy\0\0\0"
	"\x24\x00\x00\x00\x33\xb3\x91\x60"
	"unregister_chrdev_region\0\0\0\0"
	"\x1c\x00\x00\x00\x91\xc9\xc5\x52"
	"__kmalloc_noprof\0\0\0\0"
	"\x14\x00\x00\x00\x4b\x8d\xfa\x4d"
	"mutex_lock\0\0"
	"\x18\x00\x00\x00\x21\x80\x49\xf2"
	"usb_bulk_msg\0\0\0\0"
	"\x18\x00\x00\x00\x38\xf0\x13\x32"
	"mutex_unlock\0\0\0\0"
	"\x1c\x00\x00\x00\x48\x9f\xdb\x88"
	"__check_object_size\0"
	"\x18\x00\x00\x00\xe1\xbe\x10\x6b"
	"_copy_to_user\0\0\0"
	"\x18\x00\x00\x00\x31\xe1\x34\x72"
	"usb_deregister\0\0"
	"\x1c\x00\x00\x00\x63\xa5\x03\x4c"
	"random_kmalloc_seed\0"
	"\x18\x00\x00\x00\x98\x04\xd2\xf2"
	"kmalloc_caches\0\0"
	"\x20\x00\x00\x00\xfe\x6e\xe0\xb2"
	"__kmalloc_cache_noprof\0\0"
	"\x18\x00\x00\x00\x9f\x0c\xfb\xce"
	"__mutex_init\0\0\0\0"
	"\x14\x00\x00\x00\xee\xf9\xf8\xe0"
	"usb_get_dev\0"
	"\x14\x00\x00\x00\x44\x63\x51\x03"
	"cdev_init\0\0\0"
	"\x14\x00\x00\x00\x61\x45\xc0\x11"
	"cdev_add\0\0\0\0"
	"\x18\x00\x00\x00\xc2\x9c\xc4\x13"
	"_copy_from_user\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\x33\xb3\xe9\xdd"
	"cdev_del\0\0\0\0"
	"\x14\x00\x00\x00\x7c\x46\xa4\x7a"
	"usb_put_dev\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\x2b\x2f\xec\xe3"
	"alloc_chrdev_region\0"
	"\x18\x00\x00\x00\x28\xb0\x32\x80"
	"class_create\0\0\0\0"
	"\x18\x00\x00\x00\x34\x61\x23\x68"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v1A86p55D4d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "575AA65AD4D5AA5259E9EC3");
