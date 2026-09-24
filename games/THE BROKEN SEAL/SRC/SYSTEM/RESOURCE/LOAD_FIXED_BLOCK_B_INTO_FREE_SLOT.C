#include "TYPES.H"
#include "SCENE.H"

s32 Resource_CopyData(s32, s32, s32);
s32 Resource_FindFreeEntry();
extern u8 gRom[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    Resource_CopyData(slot, 0x80, gRom);
    return slot;
}
