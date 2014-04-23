// Author: Constantine Apostolou
// e-mail: conmarap@osarena.net

#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "comfunc.h"
#include "def.h"

#define MULTIBOOT_FLAG_MEM     0x001
#define MULTIBOOT_FLAG_DEVICE  0x002
#define MULTIBOOT_FLAG_CMDLINE 0x004
#define MULTIBOOT_FLAG_MODS    0x008
#define MULTIBOOT_FLAG_AOUT    0x010
#define MULTIBOOT_FLAG_ELF     0x020
#define MULTIBOOT_FLAG_MMAP    0x040
#define MULTIBOOT_FLAG_CONFIG  0x080
#define MULTIBOOT_FLAG_LOADER  0x100
#define MULTIBOOT_FLAG_APM     0x200
#define MULTIBOOT_FLAG_VBE     0x400

struct multiboot
{
    UINT32 flags;
    UINT32 mem_lower;
    UINT32 mem_upper;
    UINT32 boot_device;
    UINT32 cmdline;
    UINT32 mods_count;
    UINT32 mods_addr;
    UINT32 num;
    UINT32 size;
    UINT32 addr;
    UINT32 shndx;
    UINT32 mmap_length;
    UINT32 mmap_addr;
    UINT32 drives_length;
    UINT32 drives_addr;
    UINT32 config_table;
    UINT32 boot_loader_name;
    UINT32 apm_table;
    UINT32 vbe_control_info;
    UINT32 vbe_mode_info;
    UINT32 vbe_mode;
    UINT32 vbe_interface_seg;
    UINT32 vbe_interface_off;
    UINT32 vbe_interface_len;
}  __attribute__((packed));

typedef struct multiboot_header multiboot_header_t;

#endif
