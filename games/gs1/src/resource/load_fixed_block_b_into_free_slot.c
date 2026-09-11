#include "types.h"
#include "scene.h"

s32 Resource_CopyData(s32, s32, s32);
s32 Resource_FindFreeSlot();
extern u8 gRom[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeSlot();
    Resource_CopyData(slot, 0x80, gRom);
    return slot;
}
