#include "types.h"

u8 *Func_0200172e();

/* Complete entity-19 sprite-counter adjustment. */
void Func_02000358(void)
{
    u8 *entity = Func_0200172e(19);
    u8 *sprite = *(u8 **)(entity + 80);
    *(u16 *)(sprite + 30) += 0x1400;
}
