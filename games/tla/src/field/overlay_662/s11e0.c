#include "ov662.h"
#include "object_runtime.h"

extern const s16 Data_020019cc[];

s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020018a8(s32 id, s32 x, s32 z);

static __inline__ s32 GameFlag_GetByte_(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer of The Broken Seal does. */
    return GameFlag_GetByte(flag);
}

static __inline__ void GameFlag_SetByte_(s32 flag, s32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer of The Broken Seal does. */
    GameFlag_SetByte(flag, value);
}

void Func_020011e0(void)
{
    s32 step = GameFlag_GetByte_(0x278);
    s32 index = (step / 32) * 2;
    s32 column = Data_020019cc[index];
    s32 row = Data_020019cc[index + 1];

    if (Object_GetById(16)->movement_state == 0) {
        Func_020018a8(16, column << 19, row << 19);
        if (GameFlag_Test(0x104) == 0)
            step++;
        GameFlag_SetByte_(0x278, step);
    }
}
