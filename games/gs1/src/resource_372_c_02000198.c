#include "types.h"

extern void Func_02004a52(s32);
extern void Func_02004834(u8 *, s32, s32);
extern void Func_02004928(s32, s32, s32);
extern void Func_020048ae(s32);
extern void Func_0200031c(s32);
extern u8 Data_0200d774[];
extern u8 Value_00000101;

void Func_02000198(void)
{
    Func_02004a52(0x9E);
    Func_02004834(Data_0200d774, 45, 11);
    Func_02004928(0, (s32)&Value_00000101, 0x1A4);
    Func_020048ae(3);
    Func_0200031c(11);
}
