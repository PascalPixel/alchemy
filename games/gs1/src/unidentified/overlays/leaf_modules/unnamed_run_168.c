/* Contiguous unnamed leaf-owner run for resource_381. */

#include "types.h"

extern void Func_0200637c(s32);
extern void Func_0200634c(s32, s32);
extern void Func_0200638c(s32);
extern void Func_0200635e(s32, s32);
extern void Func_0200636c(s32);
extern void Func_0200624e(s32);

void Func_02002d48(s32 a, s32 b, s32 c)
{
    if (a == 1) {
        Func_0200637c(0x134);
        Func_0200634c(0x203a52, 1);
    } else {
        Func_0200638c(0x121);
        Func_0200635e(0x10000, 1);
    }
    Func_0200636c(b);
    if (c != 0) {
        Func_0200624e(c);
    }
}

#include "types.h"

extern s32 Data_03001e40;
extern void Func_0200624c(s32, s32);
extern void Func_02006256(s32, s32);
extern void Func_02005c74(s32);

void Func_02002d94(s32 a)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_0200624c(a, 7);
    } else {
        Func_02006256(a, 0);
    }
    if ((Data_03001e40 & 15) == 0) {
        Func_02005c74(a);
    }
}

