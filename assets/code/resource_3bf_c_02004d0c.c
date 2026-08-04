#include "types.h"

extern u8 *Data_03001ebc;

s32 Func_0200a322(s32);
void Func_0200a404(s32);
void Func_0200a41c(s32, s32);
void Func_0200a4c6(void);
void Func_02009936(void);

void Func_02004d0c(void)
{
    if (Func_0200a322(0x226)) {
        Func_0200a404(0x2435);
        Func_0200a41c(20, 0);
    } else {
        s16 *q = (s16 *)(Data_03001ebc + 382);

        *q = 0;
        Func_0200a4c6();
        Func_02009936();
    }
}
