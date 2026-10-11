#include "scene.h"

void Func_0200316c(void)
{
    Func_02003820(Data_02004768);
    Func_02004180(0);
    if (gPartyState.entrance == 1) {
        GameFlag_SetBit(0xa29);
        GameFlag_ClearBit(0xa2a);
        GameFlag_ClearBit(0xa2b);
    }
}
