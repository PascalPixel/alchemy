#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"

s32 VramBlock_LoadCached(s32, s32, s32);
extern u8 Resource_FixedBlockBTiles[];

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, 0x80, Resource_FixedBlockBTiles);
    return slot;
}

/* A routine that only reports success, after the fixed resource block
   loader; nothing in the image calls it by name. */
s32 Resource_ReturnTrue(void)
{
    return 1;
}
