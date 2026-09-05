#include "types.h"

s32 Resource_CopyData(s32, s32, s32);
s32 Resource_FindFreeSlot();
extern u8 Data_080310a4[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void) {
    s32 slot;

    slot = Resource_FindFreeSlot();
    Resource_CopyData(slot, 0x80, Data_080310a4);
    return slot;
}
