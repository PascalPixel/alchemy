#include "types.h"

#define Ui_CountIconTableEntries Func_08019ebc

extern u8 Data_0802a2e0[];
extern u8 Data_08029ee4[];

/* 2つの境界アドレス間を4バイト単位で数える。 */
s32 Ui_CountIconTableEntries(void)
{
    return (s32)((u32)Data_0802a2e0 - (u32)Data_08029ee4) >> 2;
}
