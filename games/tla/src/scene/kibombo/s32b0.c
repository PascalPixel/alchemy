#include "kibombo.h"

void Func_020032b0(void)
{
    u32 i;

    for (i = 0; i <= 7; i++)
        Object_SetModeById(i + 18, 5);
}

void Func_020032c8(void)
{
    u32 i;

    for (i = 0; i <= 7; i++)
        Object_SetModeById(i + 18, 1);
}

void Func_020032e0(void)
{
    Func_02005c14(143, 1);
    Func_02005c1c(17, 27);
    Func_02005c34();
    Field_DispatchTypeHandler(1);
    Func_02005c24();
    Func_02005c2c();
}
