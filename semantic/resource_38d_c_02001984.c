#include "types.h"

extern void Func_02003ef0(s32, s32, s32);
extern void Func_02003efc(s32, s32, s32);
extern void Func_02003f08(s32, s32, s32);

void Func_02001984(void)
{
    Func_02003ef0(1, 0xC000, 0);
    Func_02003efc(2, 0xC000, 0);
    Func_02003f08(3, 0xA000, 0);
}
