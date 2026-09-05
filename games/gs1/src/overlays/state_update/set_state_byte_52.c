#include "types.h"

void SceneState_SetStateByte52(void)
{
    u8 *state = *(u8 *volatile *)0x03001f30;
    state[52] = 1;
}
