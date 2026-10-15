#include "airs_rock_inside.h"

void Func_020007b4(void);

void Func_02004884(void)
{
    Func_02005e14();
    Func_02005f2c(0);
    if (Object_GetById(8)->x >> 19 == 65)
        GameFlag_SetBit(771);
    else
        GameFlag_ClearBit(771);
    Func_02005e1c();
}

void Func_020048bc(void)
{
    Func_020007b4();
}
