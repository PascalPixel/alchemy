#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 Data_03001810[];

s32 Resource_ClearSlotReferences(s32 resource_id)
{
    s32 cleared_count = 0;
    s32 remaining;
    u8 *marker;
    u8 empty_marker;
    if ((u32)resource_id > 0x5F) {
        return -1;
    }
    marker = Data_03001810;
    empty_marker = 0xFF;
    remaining = 0x200;
    do {
        if (*marker == resource_id) {
            *marker = empty_marker;
            cleared_count++;
        }
        remaining--;
        marker++;
    } while (remaining != 0);
    if (cleared_count != 0) {
        return -1;
    }
    return 0;
}
