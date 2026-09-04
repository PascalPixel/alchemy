#include "types.h"

#define FieldScene_SetupWithDescriptor8BD4 Func_020008e8

extern void Func_020014ba(s32);
extern void Func_0200138c(s32, s32, s32);
extern void Func_020013bc(s32);

void FieldScene_SetupWithDescriptor8BD4(void)
{
    Func_020014ba(0xBC);
    Func_0200138c(0x2008BD4, 0x34, 0xB);
    Func_020013bc(0x200);
}
