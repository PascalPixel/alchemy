#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"

s32 VramBlock_LoadCached(s32, s32, s32);
extern u8 gRom[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, 0x80, gRom);
    return slot;
}
