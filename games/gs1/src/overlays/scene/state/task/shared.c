#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define HexDigits Data_0200c250
#define FieldScene_RunSingleStep Func_02000050
#define FieldScene_Forward4358 Func_02000304
#define Text_WriteU32AsHex Func_020031fc

extern u8 Data_0200c250[];

void Func_02003be4();
void Func_02004358(void);

void FieldScene_RunSingleStep(void)
{
    Func_02003be4();
}

void FieldScene_Forward4358(void)
{
    Func_02004358();
}

void Text_WriteU32AsHex(u8 *buf, u32 value)
{
    s32 i;

    buf += 8;
    *buf = 0;
    buf--;
    for (i = 7; i >= 0; i--) {
        *buf = HexDigits[value & 15];
        value >>= 4;
        buf--;
    }
}
