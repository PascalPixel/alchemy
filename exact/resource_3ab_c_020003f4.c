#include "types.h"
extern u8 *Func_02001dfc(s32);
extern u8 *Func_02001e04(s32);
extern void Func_02001da8(s32, s32, s32, s32, s32, s32);
extern void Func_02001dba(s32, s32, s32, s32, s32, s32);
extern void Func_02001dcc(s32, s32, s32, s32, s32, s32);
extern void Func_02001de0(u8 *, s32);
extern void Func_02001e26(s32);
void Func_020003f4(void)
{
    u8 *actor = Func_02001dfc(8);
    Func_02001e04(0);
    {
        s32 cell = 14;
        s32 row = 4;
        Func_02001da8(17, 4, 1, 1, cell, row);
        Func_02001dba(15, 3, 1, 1, 15, row);
        Func_02001dcc(15, 3, 1, 1, 13, row);
    }
    if (actor != 0) {
        Func_02001de0(actor, 0);
        actor[0x55] = 2;
        actor[0x23] = 1;
    }
    Func_02001e26(0x200);
}
