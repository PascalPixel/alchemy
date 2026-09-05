#include "types.h"

#define FieldScene_ConfigureFixedPointValues Func_0200007c

extern void Func_02001172(s32, s32, s32, s32, s32, s32, s32);

void FieldScene_ConfigureFixedPointValues(void)
{
    Func_02001172(0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
}
