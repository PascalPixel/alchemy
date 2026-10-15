#include "local.h"

void Func_02000de4(void)
{
    s32 i;

    Func_02003628();
    Func_02003688(44, 0, 34, 2);
    for (i = 4; i >= 0; i--) {
        Func_020036b8(33, 55);
        WaitFrames(15);
    }
    GameFlag_SetBit(0xa67);
}
