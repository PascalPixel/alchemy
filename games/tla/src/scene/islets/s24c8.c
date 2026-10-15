#include "islets.h"

void Func_020024c8(void)
{
    Call6((void (*)())Func_02003e24, 5, 2, 1, 2, 5, 28);
    GameFlag_SetBit(0x213);
}

void Func_020024f0(void)
{
    struct ObjectRuntime *object;

    if (gInput.held & 0x40) {
        object = Object_GetById(gPartyState.current_owner);
        Func_02003e64();
        Func_02003fa4(0);
        object->motion_flags = 0;
        Call3((void (*)())ObjectMotion_SetPositionAndReset, gPartyState.current_owner, 72, 280);
        object->motion_flags = 3;
        Func_02003e6c();
    }
}
