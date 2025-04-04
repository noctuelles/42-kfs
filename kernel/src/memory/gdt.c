#include <kernel/memory/gdt.h>
#include <stddef.h>

__attribute__((section(".gdt"))) static gdt_entry_t gdt_entries[GDT_MAX_ENTRIES] = {
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
};

static const gdt_t gdt = {
    .limit = sizeof(gdt_entries) - 1,
    .base_address = (gdt_entry_t *)&gdt_entries,
};

void
gdt_set_entry(gdt_entry_t *entry, uint32_t base, uint32_t limit, segment_type_t segment_type,
              descriptor_type_t descriptor_type, dpl_t dpl, bool present, bool code64, bool db,
              granularity_t granularity) {
    entry->limit_low                  = limit & 0xFFFF;
    entry->base_low                   = base & 0xFFFF;
    entry->base_mid                   = (base >> 16) & 0xFF;
    entry->segment_type               = segment_type;
    entry->descriptor_type            = descriptor_type;
    entry->descriptor_privilege_level = dpl;
    entry->segment_present            = present;
    entry->limit_high                 = (limit >> 16) & 0xF;
    entry->avl                        = 0;
    entry->code64_segment             = code64;
    entry->db                         = db;
    entry->granularity                = granularity;
    entry->base_high                  = (base >> 24) & 0xFF;
}

void
gdt_load() {
    asm volatile("lgdt [%0]" : : "m"(gdt));
}