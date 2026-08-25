#include "types.h"

void Func_02000438();
void Func_020003e2();
void Func_02000446(s32, s32, s32);
void Func_02000452();

extern s16 Data_020085d0[];

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Func_02000446(actor, axis, offset);
}

#define ConfigureIndexedSceneEffect Func_02000120

void ConfigureIndexedSceneEffect(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 index = *(s16 *)(workspace + 364);
    u16 x = Data_020085d0[index * 2];
    u16 y = Data_020085d0[index * 2 + 1];

    Func_02000438(158);
    Func_020003e2(0x020085e8, x, y);
    SetOffset(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Func_02000452(index);
}
