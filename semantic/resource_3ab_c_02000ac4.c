#include "types.h"
extern u8 *Data_03001ebc;   
extern u8 * Func_020024ca(s32);
extern s32 Func_0200249c(s32);
extern void Func_020024b2(s32);
void Func_02000ac4(void)
{
    s32 z = *(s32 *)(Func_020024ca(0) + 16);
    if (z < 0)
        z += 0xfffff;
    if (Func_0200249c(0x243) == 0 && (z >> 20) == 10) {
        Func_020024b2(0x243);
        *(u16 *)(*(u8 **)Data_03001ebc + 364) = 20;
    }
}
