#include "local.h"
#include "call.h"

void Func_02004da0(s32 id, s32 x, s32 z);

void Func_02002868(void)
{
    if (!GameFlag_Test(0x305))
    {
        Object_SetPartAttribute(Object_GetById(11), 15);
        Func_02004da0(11, 136 << 16, 360 << 16);
        Object_GetById(11)->scaleX = Object_GetById(11)->scaleY = 0x1000;
    }
}
