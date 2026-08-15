#include "resource_390.h"
#include "resource_390_table.h"

#include "types.h"

#define Resource390_InitRecords Func_02000030

void Resource390_InitRecords(struct Resource390TableEntry *entry) {
    u32 entry_index;
    register u8 value_16;
    register s32 value_04;
    register u16 default_value_00;
    register u16 alternate_value_00;

    entry_index = 0;
    value_16 = 2;
    value_04 = 1;
    default_value_00 = 0x69;
    alternate_value_00 = 0x6E;
    do {
        entry->unknown_16 = value_16;
        entry->unknown_04 = value_04;
        entry->unknown_00 = default_value_00;
        if (entry_index == 4 || entry_index == 7) {
            entry->unknown_00 = alternate_value_00;
        }
        entry_index++;
        entry = (struct Resource390TableEntry *)((u8 *)entry + 0x18);
    } while (entry_index <= 0xE);
}
