#include "types.h"

extern void Func_02007c7a(s32, s32);
extern u8 Data_0200a2c5[];

s32 SceneData_LoadBlockA2c5(void)
{
    s32 n = 0xc80;

    Func_02007c7a((s32)Data_0200a2c5, n);
    return 0;
}
