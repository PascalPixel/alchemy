#include "types.h"
#include "scene.h"

s32 Resource_CopyData(s32, s32, s32);
s32 find_free_slot();
extern u8 gRom[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = find_free_slot();
    Resource_CopyData(slot, 0x80, gRom);
    return slot;
}
