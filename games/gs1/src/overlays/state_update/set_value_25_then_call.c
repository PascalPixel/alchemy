#include "types.h"

#define SceneState_SetValue25ThenCall Func_02005600

extern void Func_0200b5ea(s32);
extern void Func_0200b2a2(void);

void SceneState_SetValue25ThenCall(void)
{
    Func_0200b5ea(0x19);
    Func_0200b2a2();
}
