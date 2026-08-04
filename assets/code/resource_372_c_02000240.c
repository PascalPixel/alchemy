#include "types.h"

extern void Func_02004afa(s32);
extern void Func_020048dc(u8 *, s32, s32);
extern void Func_020049d0(s32, s32, s32);
extern void Func_02004956(s32);
extern void Func_020003c4(s32);
extern u8 Data_0200d78a[];
extern u8 Value_000002d7;

void Func_02000240(void)
{
    Func_02004afa(0x9E);
    Func_020048dc(Data_0200d78a, 54, 32);
    Func_020049d0(0, 0x196, (s32)&Value_000002d7);
    Func_02004956(3);
    Func_020003c4(5);
}
