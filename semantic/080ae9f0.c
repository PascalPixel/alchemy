#include "types.h"

struct Entry {
    u8 filler0[4];
    u8 field4;
    u8 field5;
    u8 filler6[6];
    u16 field12;
};

struct Entry *Func_080150c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

s32 Func_080ae9f0(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *base = *(u8 **)0x03001f2c;
    s32 value;
    struct Entry *entry;

    if (arg3 == 0) {
        value = *(u16 *)(base + 914);
        arg2 -= 3;
    } else {
        value = *(u16 *)(base + 916);
        arg2 -= 4;
    }
    entry = Func_080150c8(value, 0x40000000, arg0, arg1, arg2);
    if (entry == 0)
        return -1;
    entry->field4 = 0;
    entry->field12 = 0;
    entry->field5 = 1;
    return 1;
}
