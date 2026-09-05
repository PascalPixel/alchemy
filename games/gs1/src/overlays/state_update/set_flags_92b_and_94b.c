#include "types.h"

#define SceneState_SetFlags92bAnd94b Func_020002c0

extern void Func_02005008(void);
extern void Func_02004fc0(s32, s32);
extern void Func_02004fc8(s32, s32);
extern void Func_02005024(void);

void SceneState_SetFlags92bAnd94b(void)
{
    Func_02005008();
    Func_02004fc0(0x92b, 1);
    Func_02004fc8(0x94b, 1);
    Func_02005024();
}
