#include "TYPES.H"
#include "RESOURCE.H"

extern char ResourceId_UiIconTiles;
extern char ResourceId_UiFixedBlockATiles;

void VramBlock_LoadResourceFar(s32 entry_no, s32 size, s32 resource_id);

/* Loads the 128-byte icon tile block into a free VRAM cache entry and returns
   the entry. */
s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
    return slot;
}

void Resource_CopyFixedBlockB(s32 slot)
{
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
}

void Resource_CopyFixedBlockA(s32 slot)
{
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiFixedBlockATiles);
}
