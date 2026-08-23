#include "types.h"

s32 Resource_CopyData(s32, s32, s32);
s32 Resource_FindFreeSlot();

s32 Func_080040b4(s32 arg0) {
    s32 temp_r0;

    temp_r0 = Resource_FindFreeSlot();
    Resource_CopyData(temp_r0, arg0, 0);
    return temp_r0;
}
