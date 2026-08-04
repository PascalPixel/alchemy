#include "types.h"

extern void Func_02004858(s32, s32);
extern void Func_02004878(s32, s32);
extern void Func_02004868(s32, s32);
extern u8 *Func_02004836(s32);
extern void Func_02004818(s32);

void Func_0200170c(void)
{
    u8 *flags;

    Func_02004858(20, 1);
    Func_02004878(20, 0);
    Func_02004868(20, 2);
    flags = Func_02004836(20) + 35;
    *flags &= 0xFD;
    Func_02004818(0x200);
}
