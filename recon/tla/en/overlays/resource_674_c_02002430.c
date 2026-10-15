/* Near miss, differ 4 of 60: the listing loads the address of Data_02005198 before the
 * first object's z; this draft loads it after. */
#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02002430(void)
{
    struct ObjectRuntime *first = Object_GetById(12);
    struct ObjectRuntime *second = Object_GetById(13);

    Data_0200519c = first->x;
    Data_02005194 = first->z;
    Data_02005198 = second->x;
    Data_02005190 = second->z;
    Func_02003e64();
    Func_02003fa4(0);
    if (Data_0200519c >> 20 == 7 && Data_02005194 >> 20 == 29
        && Data_02005198 >> 20 == 7 && Data_02005190 >> 20 == 28) {
        ((u8 *)first)[0x62] = 0;
        ((void (*)())Func_020020ec)(Data_0200519c, Data_02005194, 2);
    }
    Func_02003fe4();
    Func_020021d0();
    Func_02003e6c();
}
