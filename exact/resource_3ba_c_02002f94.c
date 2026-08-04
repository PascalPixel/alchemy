#include "types.h"

extern u16 Data_02001000;

void Func_02002f94(void)
{
    u16 *p = &Data_02001000;
    s32 t = 9;

    *p = t;
}
