#include "types.h"

void *Func_08077008(s32);

s32 Func_080a3d9c(s32 arg0, s32 arg1) {
    s32 index;
    s32 result;
    u16 *slots;
    s32 entry;
    u16 test;
    s32 occupied;
    s32 mask;
    s32 himask;

    result = 0;
    slots = (u16 *) Func_08077008(arg0);
    mask = 0x1FF;
    himask = 0xF800;
    index = 0;
    slots += 0x6C;
loop_1:
    entry = *slots;
    test = entry;
    occupied = test;
    slots += 1;
    if ((occupied != 0) && ((mask & entry) == arg1)) {
        result = (u32) (himask & entry) >> 0xB;
        result += 1;
    } else {
        index += 1;
        if (index <= 0xE) { goto loop_1; }
    }
    return result;
}
