#include "types.h"

s32 Func_080162d4(s32, s32, s32, s32, s32);
extern u8 Data_03001f38;

void Func_080288a8(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *state;
    s32 i;

    state = *(u8 **)&Data_03001f38;

    *(u16 *)(state + 144) = (u16)((u32)arg2 + 2);
    *(u16 *)(state + 146) = (u16)arg3;
    *(u16 *)(state + 148) = (u16)arg1;

    for (i = 0; i < *(s16 *)(state + 142); i++) {
        *(u16 *)(state + i * 20 + 14) = (u16)((u32)arg1 << 3);
        *(u16 *)(state + i * 20 + 12) = (u16)((u32)arg0 << 3);
        arg0 = (s32)((u32)arg0 + 3);
    }

    *(s32 *)(state + 120) =
        Func_080162d4(arg0, arg1, *(s16 *)(state + 144), 3, 2);
}
