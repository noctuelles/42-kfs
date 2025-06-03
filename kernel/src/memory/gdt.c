#include <kernel/memory/gdt.h>
#include <stddef.h>
#include <string.h>

__attribute__((section(".gdt"))) static gdt_entry_t gdt_entries[] = {
    {0},
    {.limit_low                  = 0xFFFF,
     .base_low                   = 0,
     .base_mid                   = 0,
     .segment_type               = SEGMENT_TYPE_CODE_EXECUTE_READ,
     .descriptor_type            = DESCRIPTOR_TYPE_CODE_OR_DATA,
     .descriptor_privilege_level = DPL_RING0,
     .segment_present            = true,
     .limit_high                 = 0xF,
     .avl                        = 0,
     .code64_segment             = false,
     .db                         = 1,
     .granularity                = GRANULARITY_4KB,
     .base_high                  = 0},

    {.limit_low                  = 0xFFFF,
     .base_low                   = 0,
     .base_mid                   = 0,
     .segment_type               = SEGMENT_TYPE_DATA_READ_WRITE,
     .descriptor_type            = DESCRIPTOR_TYPE_CODE_OR_DATA,
     .descriptor_privilege_level = DPL_RING0,
     .segment_present            = true,
     .limit_high                 = 0xF,
     .avl                        = 0,
     .code64_segment             = false,
     .db                         = 1,
     .granularity                = GRANULARITY_4KB,
     .base_high                  = 0},

    {.limit_low                  = 0,
     .base_low                   = 0,
     .base_mid                   = 0,
     .segment_type               = SEGMENT_TYPE_DATA_READ_WRITE_EXPAND_DOWN,
     .descriptor_type            = DESCRIPTOR_TYPE_CODE_OR_DATA,
     .descriptor_privilege_level = DPL_RING0,
     .segment_present            = true,
     .limit_high                 = 0,
     .avl                        = 0,
     .code64_segment             = false,
     .db                         = 1,
     .granularity                = GRANULARITY_4KB,
     .base_high                  = 0},

    {.limit_low                  = 0xFFFF,
     .base_low                   = 0,
     .base_mid                   = 0,
     .segment_type               = SEGMENT_TYPE_CODE_EXECUTE_READ,
     .descriptor_type            = DESCRIPTOR_TYPE_CODE_OR_DATA,
     .descriptor_privilege_level = DPL_RING3,
     .segment_present            = true,
     .limit_high                 = 0xF,
     .avl                        = 0,
     .code64_segment             = false,
     .db                         = 1,
     .granularity                = GRANULARITY_4KB,
     .base_high                  = 0},

    {.limit_low                  = 0xFFFF,
     .base_low                   = 0,
     .base_mid                   = 0,
     .segment_type               = SEGMENT_TYPE_DATA_READ_WRITE,
     .descriptor_type            = DESCRIPTOR_TYPE_CODE_OR_DATA,
     .descriptor_privilege_level = DPL_RING3,
     .segment_present            = true,
     .limit_high                 = 0xF,
     .avl                        = 0,
     .code64_segment             = false,
     .db                         = 1,
     .granularity                = GRANULARITY_4KB,
     .base_high                  = 0},

    {.limit_low                  = 0xFFFF,
     .base_low                   = 0,
     .base_mid                   = 0,
     .segment_type               = SEGMENT_TYPE_DATA_READ_WRITE_EXPAND_DOWN,
     .descriptor_type            = DESCRIPTOR_TYPE_CODE_OR_DATA,
     .descriptor_privilege_level = DPL_RING3,
     .segment_present            = true,
     .limit_high                 = 0xF,
     .avl                        = 0,
     .code64_segment             = false,
     .db                         = 1,
     .granularity                = GRANULARITY_4KB,
     .base_high                  = 0},
};

static const gdt_t gdt = {
    .limit = sizeof(gdt_entries) - 1,
    .base_address = (uint32_t *)&gdt_entries,
};

void
gdt_load() {
    asm volatile(
        "push eax\n"
        "lgdt [%0]\n"
        "jmp 0x08:.reload_cs\n"
        ".reload_cs:\n"
        "mov ax, 0x10\n"
        "mov ds, ax\n"
        "mov es, ax\n"
        "mov fs, ax\n"
        "mov gs, ax\n"
        "mov ax, 0x18\n"
        "mov ss, ax\n"
        "pop eax"
        : : "m"(gdt)
    );
}