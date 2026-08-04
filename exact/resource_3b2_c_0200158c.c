#include "types.h"

extern u8 *Func_0200461c(s32 arg0);
extern s32 Func_0200462e(s32 arg0);
extern void Func_020045e4(s32 arg0, s32 arg1);
extern void Func_02004604(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02004622(s32 arg0);

void Func_0200158c(s32 arg0) {
    s32 a = 0x600000;
    s32 b = 0x1500000;
    u8 *p = Func_0200461c(13);

    if (p != 0) {
        p[89] = 0;
    }
    Func_020045e4(Func_0200462e(arg0), 0);
    Func_02004604(0, a, b, 253);
    Func_02004622(578);
}
