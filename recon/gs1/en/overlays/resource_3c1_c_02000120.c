#include "types.h"

/* Old-style declarations: overlay import arities are not fixed per name, and
 * each site names the veneer it reaches. */
void Func_02000438();
void Func_020003e2();
void Func_02000446();
void Func_02000452();

/* Fixed addresses touched more than once are declared objects, so the base
 * stays in a register and the pair is reached by index. */
extern s16 Data_020085d0[];

void Func_02000120(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    s32 index = *(s16 *)(workspace + 364);
    u16 x = Data_020085d0[index * 2];
    u16 y = Data_020085d0[index * 2 + 1];

    Func_02000438(158);
    Func_020003e2(0x020085e8, x, y);
    Func_02000446(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Func_02000452(index);
}
