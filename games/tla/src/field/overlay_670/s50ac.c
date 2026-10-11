#include "ov670.h"

void Func_020050ac(void)
{
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    Func_02005c04(0);
    Func_02005c64(Data_02006324);
    ObjectMotion_SetActionVariant(8, 3);
    Func_02005c54();
    Func_02005c5c(0, 0x9e9, 12, 13);
    Object_GetById(22)->collision_flags |= 0x80;
    Object_GetById(23)->collision_flags |= 0x80;
}
