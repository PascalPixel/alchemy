#include "types.h"

#define SceneState_SetValue30ThenCall Func_02000954

extern void Func_0200178e(s32);
extern void Func_020016c2(void);

void SceneState_SetValue30ThenCall(void)
{
    Func_0200178e(30);
    Func_020016c2();
}
