#include "types.h"

extern s32 Func_02006ff4(s32);
extern void Func_0200702c(void);
extern void Func_02007012(s32);
extern s32 *Func_02007058(s32);
extern s32 *Func_02007068(s32);
extern s32 *Func_02007070(s32);
extern void Func_02007052(s32);
extern void Func_02006fbc(s32, s32, s32, s32, s32, s32);
extern void Func_02007222(s32);
extern void Func_02006fc4(s32, s32, s32);
extern void Func_02007088(void);
extern u8 Data_0200e03a[];

void Func_02001568(void)
{
    if (Func_02006ff4(0x256) == 0)
        return;
    Func_0200702c();
    Func_02007012(0x256);
    Func_02007058(0)[3] += 0x20000;
    {
        s32 *p = Func_02007068(0);
        p[15] = Func_02007070(0)[3];
    }
    Func_02007052(5);
    Func_02006fbc(7, 2, 5, 11, 1, 1);
    Func_02007222(0xd9);
    Func_02006fc4((s32)Data_0200e03a, 9, 7);
    Func_02007088();
}
