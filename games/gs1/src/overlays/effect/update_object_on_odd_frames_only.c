#include "types.h"

#define SceneEffect_UpdateObjectOnOddFramesOnly Func_02005c98

extern volatile u32 Data_03001e40;
extern s32 Func_0200bb34(s32, s32);
extern void Func_0200bc2c(s32, s32);

void SceneEffect_UpdateObjectOnOddFramesOnly(s32 p)
{
    if ((Data_03001e40 & 1) != 0)
        Func_0200bc2c(p, Func_0200bb34(Data_03001e40 >> 1, 6));
}
