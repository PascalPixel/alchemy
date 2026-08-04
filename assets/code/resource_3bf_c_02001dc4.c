#include "types.h"

extern u8 *Data_03001ebc;
extern u8 Data_02000240[];

void Func_02007474(s32, s32);
void Func_0200757a(s32);
void Func_020074f6(s32, s32, s32);
void Func_020039c8(s32);
void Func_02007564(s32, s32);
void Func_020074ae(s32, s32, s32);
void Func_0200744a(void);
void Func_02007430(s32);

void Func_02001dc4(void)
{
    Func_02007474(0, 1);
    Func_0200757a(113);
    Func_020074f6(15, 256, 60);
    Func_020039c8(15);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_02007564(98, 2);
    Func_020074ae(15, 0, 0);
    Func_0200744a();
    Func_02007430(2380);
}
