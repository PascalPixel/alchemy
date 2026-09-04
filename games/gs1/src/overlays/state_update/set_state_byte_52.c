#include "types.h"

#define SceneState_SetStateByte52 Func_02000b14

void SceneState_SetStateByte52(void)
{
    u8 *state = *(u8 *volatile *)0x03001f30;
    state[52] = 1;
}
