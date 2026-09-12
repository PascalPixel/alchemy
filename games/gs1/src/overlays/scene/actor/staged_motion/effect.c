#include "types.h"

#define SceneEffect_UpdateObjectOnOddFrames Func_02005c5c
#define SceneEffect_UpdateObjectOnOddFramesOnly Func_02005c98
extern volatile u32 Data_03001e40;

s32 Func_0200baf8(s32, s32);
void Func_0200bbf0(s32, s32);
void Func_0200b9f4(s32);
s32 Func_0200bb34(s32, s32);
void Func_0200bc2c(s32, s32);

void SceneEffect_UpdateObjectOnOddFrames(s32 p)
{
    if ((Data_03001e40 & 1) != 0)
        Func_0200bbf0(p, Func_0200baf8(Data_03001e40 >> 1, 6));
    if ((Data_03001e40 & 0xf) == 0)
        Func_0200b9f4(p);
}

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
    if ((Data_03001e40 & 1) != 0)
        Func_0200bc2c(p, Func_0200bb34(Data_03001e40 >> 1, 6));
}
