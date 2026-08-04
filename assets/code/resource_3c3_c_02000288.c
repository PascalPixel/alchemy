#include "types.h"

extern u8 *Data_03001ebc;

extern u8 *Func_02000b52(s32);
extern void Func_02000c16(s32);
extern void Func_02000b20(s32, s32, s32, s32, s32, s32);
extern void Func_02000b36(s32, s32, s32, s32, s32, s32);
extern void Func_02000b1e(s32);
extern void Func_02000b34(s32);
extern void Func_02000bc0(s32, s32, s32);
extern void Func_02000c46(s32);

void Func_02000288(void)
{
    u8 *workspace = Data_03001ebc;
    s16 token = *(s16 *)(workspace + 364);
    u8 *actor;

    actor = Func_02000b52(0);
    actor[85] = 0;

    Func_02000c16(158);

    Func_02000b20(0x42, 0x24, 0x47, 8, 2, 2);
    Func_02000b1e(4);

    Func_02000b36(0x44, 0x24, 0x47, 8, 2, 2);
    Func_02000b34(4);

    Func_02000bc0(0, 3, -16);

    Func_02000c46(token);
}
