#include "types.h"

u8 *Func_02004ba2();
extern u8 *Data_03001ee0;

/* Complete scene/entity linker through return and its sole pool word. */
void Func_02002080(void)
{
    u8 *entity = Func_02004ba2(0);
    *(u8 **)(Data_03001ee0 + 24) = entity;
    entity[98] = 1;
}
