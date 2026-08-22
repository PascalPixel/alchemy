#include "types.h"

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void Func_02003cc4(void)
{
    u16 *p = (u16 *)0x02001000;
    u16 v = 9;
    *p = v;
}
