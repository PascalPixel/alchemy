#include "types.h"

extern void Func_02004882(void);
extern void Func_02004940(s32);
extern void Func_02004958(s32, s32);
extern s32 Func_02004856(s32);
extern void Func_02004868(s32);
extern void Func_020049d4(void);
extern void Func_020048a2(s32);
extern void Func_0200498e(s32, s32, s32);
extern void Func_02004968(s32, s32, s32);
extern void Func_02004990(s32, s32);
extern void Func_020048c6(s32);
extern void Func_0200495e(s32, s32);
extern void Func_020048d4(s32);
extern void Func_020049ac(s32, s32);
extern void Func_020049c0(s32, s32, s32);
extern void Func_020048fc(void);

void Func_0200049c(s32 a0, s32 a)
{
    Func_02004882();
    Func_02004940(0x2052);
    Func_02004958(a, 0);
    if (Func_02004856(0x968) == 0) {
        Func_02004868(0x968);
        Func_020049d4();
        Func_020048a2(50);
        Func_0200498e(a, 0x100, 0x46);
        Func_02004968(a, 0, 0x28);
        Func_02004990(a, 0);
        Func_020048c6(30);
        Func_0200495e(a, 4);
        Func_020048d4(20);
        Func_020049ac(a, 0);
        Func_020049c0(a, 0x8000, 0);
    }
    Func_020048fc();
}
