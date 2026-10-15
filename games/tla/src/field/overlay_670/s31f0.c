#include "ov670.h"

void Func_020031f0(s32 id)
{
    Func_02005b8c(id);
    Object_SetPartAttribute(Object_GetById(id), 15);
    ObjectMotion_SetActionVariant(id, 1);
    Object_GetById(id)->priority_flags |= 2;
}

void Func_02003220(void)
{
    if (GameFlag_Test(265)) {
        if (GameFlag_Test(0x200) == 0)
            Func_0200057c();
    } else {
        GameFlag_SetBit(0x200);
    }
}
