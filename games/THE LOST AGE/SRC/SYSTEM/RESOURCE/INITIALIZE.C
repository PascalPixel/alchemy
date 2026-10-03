#include "TYPES.H"
#include "RESOURCE.H"


s32 Resource_LoadIntoFreeSlot(s32 size)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    VramBlock_LoadCached(slot, size, 0);
    return slot;
}
