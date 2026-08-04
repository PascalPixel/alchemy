#include "types.h"

extern u8 *Func_020045d8(s32 arg0);
extern s32 Func_020045ea(s32 arg0);
extern void Func_020045a0(s32 arg0, s32 arg1);
extern void Func_020045c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020045de(s32 arg0);

void Func_02001548(s32 arg0) {
    s32 a = 0x500000;
    s32 b = 0x1700000;
    u8 *p = Func_020045d8(12);

    if (p != 0) {
        p[89] = 0;
    }
    Func_020045a0(Func_020045ea(arg0), 0);
    Func_020045c0(0, a, b, 253);
    Func_020045de(577);
}
