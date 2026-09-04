#include "types.h"

#define SceneState_SetValue0ThenCall Func_020055c0

extern void Func_0200b5aa(s32);
extern void Func_0200b226(void);

void SceneState_SetValue0ThenCall(void)
{
    Func_0200b5aa(0);
    Func_0200b226();
}
