#include "../../../../games/tla/src/field/overlay_662/ov662.h"
#include "object_runtime.h"

struct Ov662Cell {
    s16 column;
    s16 row;
};

extern const struct Ov662Cell Data_020019cc[];

s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020018a8(s32 id, s32 x, s32 z);

void Func_020011e0(void)
{
    s32 step = GameFlag_GetByte(0x278);
    s32 column = Data_020019cc[step / 32].column;
    s32 row = Data_020019cc[step / 32].row;

    if (Object_GetById(16)->movement_state == 0) {
        Func_020018a8(16, column << 19, row << 19);
        if (GameFlag_Test(0x104) == 0)
            step++;
        GameFlag_SetByte(0x278, step);
    }
}
