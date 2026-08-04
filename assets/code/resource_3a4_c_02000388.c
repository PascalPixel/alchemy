#include "types.h"

extern u8 Data_02000240[];
extern u8 Value_0000004d;
extern void Func_02004042(s32, s32);
extern void Func_0200403a(s32, s32);

void Func_02000388(void)
{
    Data_02000240[0x22b] = 3;
    Func_02004042((s32)&Value_0000004d, 99);
    Func_0200403a(53, 2);
}
