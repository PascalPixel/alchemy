#include "types.h"

#define RunEventScript02 Func_020019bc

void Func_02003bf0(void);
void Func_02003d44(void);
void Func_02003c20(s32, s32, s32);
void Func_02003c5a(s32, s32, s32);
void Func_02003c16(void);

void RunEventScript02(void)
{
    Func_02003bf0();
    Func_02003d44();
    Func_02003c20(0, 0x20000, 0x1999);
    Func_02003c5a(0, 0xe8, 0xcc);
    Func_02003c16();
}
