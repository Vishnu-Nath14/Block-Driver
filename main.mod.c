#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x27683a56, "memset" },
	{ 0x855719f6, "blk_mq_alloc_tag_set" },
	{ 0x5b5c354a, "blk_mq_alloc_queue" },
	{ 0xf217cc79, "blk_mq_alloc_disk_for_queue" },
	{ 0x199aff68, "set_capacity" },
	{ 0xb0629c64, "device_add_disk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xf1de9e85, "vfree" },
	{ 0x32017814, "put_disk" },
	{ 0xa2575471, "blk_mq_destroy_queue" },
	{ 0x370c7b34, "blk_mq_free_tag_set" },
	{ 0x32017814, "del_gendisk" },
	{ 0x50eb2fe0, "blk_mq_start_request" },
	{ 0xbd03ed67, "vmemmap_base" },
	{ 0xbd03ed67, "page_offset_base" },
	{ 0xa53f4e29, "memcpy" },
	{ 0x275fc73d, "blk_mq_end_request" },
	{ 0xd272d446, "__fentry__" },
	{ 0xe8213e80, "_printk" },
	{ 0xd7a59a65, "vmalloc_noprof" },
	{ 0xd954c786, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x27683a56,
	0x855719f6,
	0x5b5c354a,
	0xf217cc79,
	0x199aff68,
	0xb0629c64,
	0xd272d446,
	0xf1de9e85,
	0x32017814,
	0xa2575471,
	0x370c7b34,
	0x32017814,
	0x50eb2fe0,
	0xbd03ed67,
	0xbd03ed67,
	0xa53f4e29,
	0x275fc73d,
	0xd272d446,
	0xe8213e80,
	0xd7a59a65,
	0xd954c786,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"memset\0"
	"blk_mq_alloc_tag_set\0"
	"blk_mq_alloc_queue\0"
	"blk_mq_alloc_disk_for_queue\0"
	"set_capacity\0"
	"device_add_disk\0"
	"__x86_return_thunk\0"
	"vfree\0"
	"put_disk\0"
	"blk_mq_destroy_queue\0"
	"blk_mq_free_tag_set\0"
	"del_gendisk\0"
	"blk_mq_start_request\0"
	"vmemmap_base\0"
	"page_offset_base\0"
	"memcpy\0"
	"blk_mq_end_request\0"
	"__fentry__\0"
	"_printk\0"
	"vmalloc_noprof\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0FDAD5476681AF073FCD6E8");
