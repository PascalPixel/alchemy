#include "types.h"

extern void Func_02002700(void);
extern void Func_020027ce(s32);
extern s32 Func_020026ec(s32);
extern void Func_020027de(s32);
extern void Func_020027f6(s32, s32);
extern void Func_0200270c(s32);
extern void Func_02002730(void);

void Func_0200028c(void)
{
    Func_02002700();
    Func_020027ce(0x1756);
    if (Func_020026ec(0x303) != 0) {
        Func_020027de(0x176C);
    }
    Func_020027f6(15, 0);
    Func_0200270c(0x303);
    Func_02002730();
}
