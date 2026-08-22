#include "types.h"

/* Four sites of one import, so four names. */
extern void Func_02003ed6(s32, s32, s32, s32, s32, s32);
extern void Func_02003ee8(s32, s32, s32, s32, s32, s32);
extern void Func_02003ef8(s32, s32, s32, s32, s32, s32);
extern void Func_02003f0a(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02003f78(s32);

void Func_020019e8(void)
{
    Func_02003ed6(57, 42, 1, 1, 40, 42);
    Func_02003ee8(57, 42, 1, 1, 41, 42);
    Func_02003ef8(58, 42, 1, 1, 42, 42);
    Func_02003f0a(62, 37, 3, 1, 37, 42);

    Func_02003f78(8)[85] = 1;
}
