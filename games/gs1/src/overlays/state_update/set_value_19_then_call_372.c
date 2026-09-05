#include "types.h"

extern void Func_02008cda(s32);
extern void Func_020088fe(void);

void SceneState_SetValue19ThenCall(void)
{
    Func_02008cda(19);
    Func_020088fe();
}
