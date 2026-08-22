#include "types.h"

extern void Func_0200583c(void);
extern s32 Func_02005812(s32);
extern void Func_0200591c(s32);
extern s32 Func_02005824(s32);
extern void Func_0200592e(s32);
extern void Func_02005936(s32);
extern void Func_02000ea0(s32);
extern void Func_02005878(void);

void Func_02000af4(void)
{
    Func_0200583c();
    if (Func_02005812(0x855) == 0) {
        Func_0200591c(0x1294);
    } else if (Func_02005824(0x85b) == 0) {
        Func_0200592e(0x1382);
    } else {
        Func_02005936(0x1cf4);
    }
    Func_02000ea0(18);
    Func_02005878();
}
