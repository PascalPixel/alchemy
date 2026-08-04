#include "types.h"

extern u8 Data_0200a0ac[];

extern s32 Func_02002612(s32);
extern void Func_0200278c(s32);
extern void Func_02002616(u8 *, s32, s32);
extern void Func_02002698(s32, s32, s32);
extern void Func_0200275e(s32);

void Func_02000ba8(void)
{
    if (Func_02002612(0x242) == 0) {
        Func_0200278c(0x9E);
        Func_02002616(Data_0200a0ac, 0x2B, 8);
    }
    Func_02002698(0, 0xE5, 0xD9);
    Func_0200275e(3);
}
