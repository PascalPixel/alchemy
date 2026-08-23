#include "types.h"

s32 Resource_CopyData(s32, s32, s32);
s32 Resource_FindFreeSlot();
extern u8 Data_080310a4[];

s32 Func_080209b0(void) {
    s32 temp_r0;

    temp_r0 = Resource_FindFreeSlot();
    Resource_CopyData(temp_r0, 0x80, Data_080310a4);
    return temp_r0;
}
