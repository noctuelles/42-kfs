#ifndef _GDT_H
#define _GDT_H

#include <stdbool.h>
#include <stdint.h>

#define GDT_MAX_ENTRIES 12

typedef enum segment_type_e {
    SEGMENT_TYPE_DATA_READ_ONLY           = 0b0000,
    SEGMENT_TYPE_DATA_READ_ONLY_ACCESSED  = 0b0001,
    SEGMENT_TYPE_DATA_READ_WRITE          = 0b0010,
    SEGMENT_TYPE_DATA_READ_WRITE_ACCESSED = 0b0011,

    SEGMENT_TYPE_CODE_EXECUTE_ONLY          = 0b1000,
    SEGMENT_TYPE_CODE_EXECUTE_ONLY_ACCESSED = 0b1001,
    SEGMENT_TYPE_CODE_EXECUTE_READ          = 0b1010,
    SEGMENT_TYPE_CODE_EXECUTE_READ_ACCESSED = 0b1011
} segment_type_t;

typedef enum descriptor_type_e { DESCRIPTOR_TYPE_SYSTEM = 0, DESCRIPTOR_TYPE_CODE_OR_DATA = 1 } descriptor_type_t;

typedef enum dpl_e { DPL_RING0 = 0, DPL_RING1 = 1, DPL_RING2 = 2, DPL_RING3 = 3 } dpl_t;

typedef enum granularity_e { GRANULARITY_1B = 0, GRANULARITY_4KB = 1 } granularity_t;

typedef struct gdt_entry_e {
    uint16_t limit_low;  // Lower 16 bits of segment limit
    uint16_t base_low;   // Lower 16 bits of base address
    uint8_t  base_mid;   // Next 8 bits of base address

    segment_type_t    segment_type : 4;                // Segment type (code/data, read/write)
    descriptor_type_t descriptor_type : 1;             // 0 = system, 1 = code/data
    dpl_t             descriptor_privilege_level : 2;  // Ring level (0-3)
    bool              segment_present : 1;             // Segment present in memory

    uint8_t       limit_high : 4;      // Upper 4 bits of segment limit
    uint8_t       avl : 1;             // Available for system use
    bool          code64_segment : 1;  // 64-bit code segment
    uint8_t       db : 1;              // Default operation size (0 = 16-bit, 1 = 32-bit)
    granularity_t granularity : 1;     // Granularity (0 = 1B, 1 = 4KB)

    uint8_t base_high;  // Upper 8 bits of base address
} __attribute__((packed)) gdt_entry_t;

#endif
