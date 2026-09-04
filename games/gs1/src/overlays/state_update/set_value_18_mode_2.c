#include "types.h"

#define SceneState_SetValue18Mode2 Func_02000cc0

extern void Func_02003b4c(s32 a, s32 b);

void SceneState_SetValue18Mode2(void) {
    Func_02003b4c(18, 2);
}
