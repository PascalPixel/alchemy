#include "types.h"

s32 Func_080920e8(s32);
s32 Motion_SetVarCb();

void Motion_SetVarCbAndRefresh(s32 arg0)
{
    Motion_SetVarCb();
    Func_080920e8(arg0);
}
