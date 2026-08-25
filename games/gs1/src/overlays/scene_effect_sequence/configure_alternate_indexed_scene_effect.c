#include "types.h"

void Func_020004ac();
void Func_02000456();
void Func_020004ba(s32, s32, s32);
void Func_020004c6();

extern s16 Data_020085d0[];

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Func_020004ba(actor, axis, offset);
}

void Func_02000194(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 index = *(s16 *)(workspace + 364);
    u16 x = Data_020085d0[index * 2];
    u16 y = Data_020085d0[index * 2 + 1];

    Func_020004ac(158);
    Func_02000456(0x020085fe, x, y);
    SetOffset(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Func_020004c6(index);
}
