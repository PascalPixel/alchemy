#include "metadata_lookup.h"
#include "types.h"

struct Entry_08012d70 {
    s16 no;
    s16 x;
    u8 value_04;
    u8 unknown_05[7];
    s32 *table_0c;
    s32 value_10;
    s8 value_14;
    s8 value_15;
    u8 value_16;
    s8 value_17;
};

void Func_08012d70(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    u8 *metadata;
    struct Entry_08012d70 *entry;
    u8 *work;

    work = *(u8 **)0x03001E60;
    count = 0;
    offset = ((3 & slot) * 4) + 0x28;
    index = 0;
    do {
        entry = *(struct Entry_08012d70 **)(work + offset);
        if (entry->table_0c != 0) {
            metadata = Func_08185000(entry->no);
            if (value < metadata[5]) {
                entry->value_04 = metadata[4];
                entry->value_10 = entry->table_0c[value];
                entry->x = count * 0x10;
                entry->value_15 = 0x10;
                entry->value_14 = index;
                entry->value_17 = index;
                entry->value_16 = 0xFF;
            }
            work[0x23] = metadata[7];
            *(s16 *)(work + 0x1e) = index;
        }
        count += 1;
        work += 0x38;
    } while (count <= 9);
}
