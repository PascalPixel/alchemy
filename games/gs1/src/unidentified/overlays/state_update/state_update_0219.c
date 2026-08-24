#include "types.h"

extern u8 Data_02000240[];
extern u8 Value_0000004f;
extern void Func_0200406e(s32, s32);
extern void Func_02004066(s32, s32);

void Func_020003b4(void)
{
    Data_02000240[0x22b] = 3;
    Func_0200406e((s32)&Value_0000004f, 99);
    Func_02004066(53, 2);
}
