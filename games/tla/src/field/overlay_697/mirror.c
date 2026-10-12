#include "ov697.h"

s32 Func_02002ce8(s32 side)
{
    struct Ov697State *state;

    /* FAKEMATCH: the one-pass block loads the state address before the index is built; with the address as a constant in the indexed read the compiler stops with an internal error. */
    do {
        state = gOv697State;
    } while (0);
    return state->field_16[side & 1];
}

s32 Func_02002cfc(void)
{
    return gOv697State->mirrored;
}

s32 Func_02002d08(s32 x)
{
    if (Func_02002cfc())
        return 512 - x;
    return x;
}

s32 Func_02002d20(s32 x)
{
    if (Func_02002cfc())
        return (64 - x) << 3;
    return x << 3;
}

s32 Func_02002d38(s32 x)
{
    if (Func_02002cfc())
        return -x << 3;
    return x << 3;
}

s32 Func_02002d50(s32 angle)
{
    if (Func_02002cfc())
        return (0x8000 - angle) & 0xffff;
    return angle & 0xffff;
}

void Func_02002d78(s32 id, s32 x, s32 z, s32 angle)
{
    Func_020058e8(id, Func_02002d08(x) << 16, z << 16, (u16)Func_02002d50(angle));
}

void Func_02002db0(void)
{
}

void Func_02002db4(void)
{
    Func_02002d78(8, 288, 264, 0x8000);
    Func_02002d78(13, 296, 248, 0x8000);
    Func_02002d78(14, 296, 280, 0x8000);
}
