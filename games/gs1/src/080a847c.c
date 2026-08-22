#include "types.h"

struct State_080a847c {
    u8 padding[36];
    u8 *object;
};

extern struct State_080a847c *volatile Data_03001f2c;
extern u8 Data_080af2fc[];

void Func_080a2268(const u8 *, s32, s32, s32, s32, u32);

void Func_080a847c(s32 arg0, s32 selected, u8 *entries, s32 arg3)
{
    s32 saved_arg3 = arg3;
    struct State_080a847c *state = Data_03001f2c;
    u32 different;
    s32 count;
    s32 index;
    s32 x;
    s32 y;
    s32 last;
    u8 *object;
    u8 width;

    if (arg0 != 0)
        goto nonzero;

    y = selected * 2 + 5;
    x = 0;
    width = 5;
    count = 0;
    index = 0;
    if (entries[0] == 0)
        goto next;
    if (selected == 0) {
        width = Data_080af2fc[0];
        goto draw;
    }
increment:
    count++;
next:
    index++;
    if (index > 4)
        goto draw;
    if (entries[index] == 0)
        goto next;
    if (selected != count)
        goto increment;
    width = Data_080af2fc[index];
    goto draw;

nonzero:
    if (selected <= 3) {
        y = selected;
        x = 5;
        width = 13;
    } else {
        y = selected + 4;
        x = 8;
        width = 20;
    }

draw:
    different = 1 ^ saved_arg3;
    last = 15 - (((0u - different) | different) >> 31);
    object = state->object;
    Func_080a2268(object, x, y, width, 1, last);
}
