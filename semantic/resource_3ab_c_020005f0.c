#include "types.h"










extern u8 * Func_02001ff8(s32);
extern void Func_02002042(s32, s32, s32);
extern void Func_0200204c(s32, s32, s32);
extern void Func_02001fa8(s32, s32, s32, s32, s32, s32);
extern void Func_02001fc2(s32, s32, s32, s32, s32, s32);
extern void Func_02001fd6(s32, s32, s32, s32, s32, s32);
extern void Func_02001fea(s32, s32, s32, s32, s32, s32);
extern void Func_0200214c(s32, s32, s32);
extern void Func_02002000(s32, s32, s32, s32, s32, s32);
extern void Func_02002050(s32);
extern void Func_020020d0(s32, s32);
extern u8 * Func_0200208e(s32);
extern u8 * Func_02002098(s32);
extern void Func_02002046(u8 *, s32);
extern void Func_02001fd4(u8 *);
extern void Func_02002072(s32);
void Func_020005f0(void)
{
    u8 *actor;
    Func_02001ff8(0);
    Func_02002042(19, 0, 0);
    Func_0200204c(8, 0, 0);
    Func_02001fa8(38, 38, 1, 1, 46, 4);
    Func_02001fc2(37, 37, 3, 3, 13, 3);
    Func_02001fd6(37, 37, 1, 1, 14, 2);
    Func_02001fea(8, 16, 1, 1, 7, 16);
    Func_0200214c(102, 0, 0);
    Func_02002000(32, 42, 3, 2, 1, 15);
    Func_02002050(0x200);
    Func_020020d0(8, 1);
    *(s32 *)(Func_0200208e(8) + 108) = 0;
    actor = Func_02002098(8);
    Func_02002046(actor, 0);
    Func_02001fd4((u8 *)0x02008469);
    Func_02002072(0x201);
}
