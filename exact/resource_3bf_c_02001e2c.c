#include "types.h"

extern u8 *Data_03001ebc;
extern u8 Data_02000240[];

void Func_020074dc(s32, s32);
void Func_020075e2(s32);
void Func_0200755e(s32, s32, s32);
void Func_02003a30(s32);
void Func_020075cc(s32, s32);
void Func_02007516(s32, s32, s32);
void Func_020074b2(void);
void Func_02007498(s32);

void Func_02001e2c(void)
{
    Func_020074dc(0, 1);
    Func_020075e2(113);
    Func_0200755e(11, 256, 60);
    Func_02003a30(11);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_020075cc(98, 2);
    Func_02007516(11, 0, 0);
    Func_020074b2();
    Func_02007498(2377);
}
