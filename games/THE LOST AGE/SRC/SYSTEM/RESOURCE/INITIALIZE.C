#include "TYPES.H"
#include "RESOURCE.H"

s32 VramBlock_LoadCached(s32, s32, s32);

s32 Resource_LoadIntoFreeSlot(s32 arg0)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, arg0, 0);
    return slot;
}
