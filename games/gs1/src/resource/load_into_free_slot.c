#include "types.h"

#define Resource_LoadIntoFreeSlot Func_080040b4

s32 Resource_CopyData(s32, s32, s32);
s32 Resource_FindFreeSlot();

s32 Resource_LoadIntoFreeSlot(s32 arg0) {
    s32 slot;

    slot = Resource_FindFreeSlot();
    Resource_CopyData(slot, arg0, 0);
    return slot;
}
