#include "types.h"

#define SceneState_SetValue1ThenCall Func_020055b0

extern void Func_0200b59a(s32);
extern void Func_0200b216(void);

void SceneState_SetValue1ThenCall(void)
{
    Func_0200b59a(1);
    Func_0200b216();
}
