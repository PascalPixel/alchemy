#include "types.h"

#define SceneState_SetValue202ThenCall Func_02001924

extern void Func_02004a06(s32);
extern void Func_02003072(void);

void SceneState_SetValue202ThenCall(void)
{
    Func_02004a06(0x202);
    Func_02003072();
}
