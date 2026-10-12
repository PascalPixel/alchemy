#include "scene.h"

void Func_020036d0(void)
{
    if (Object_GetById(64) != 0)
        Func_02004090(64, 0, 0);
    if (gPartyState.entrance == 2) {
        GameFlag_ClearBit(0xa29);
        GameFlag_ClearBit(0xa2a);
        GameFlag_SetBit(0xa2b);
    }
}
