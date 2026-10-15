#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_0200217c(void)
{
    if (!GameFlag_Test(0x303)) {
        Object_SetPartAttribute(Object_GetById(21), 15);
        Func_02004da0(21, 600 << 16, 312 << 16);
        Object_GetById(21)->scaleX = Object_GetById(21)->scaleY = 0x1000;
    }
}
