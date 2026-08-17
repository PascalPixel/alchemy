#include "types.h"

extern u8 Value_00000003;
extern s32 Data_0200c468[];

void Func_02000ee0(u8 *o)
{
    u16 *p = (u16 *)(o + 0x64);
    s32 m = (s32)&Value_00000003;
    s32 *table = Data_0200c468;
    s32 v = ((s16)*p >> 2) & m;
    s32 nv;

    {
        s32 idx = v << 2;
        v = *(s32 *)((u8 *)table + idx);
    }
    *(s32 *)(o + 24) = v;
    *(s32 *)(o + 28) = v;
    nv = *p + 1;
    nv &= 15;
    *p = nv;
}
