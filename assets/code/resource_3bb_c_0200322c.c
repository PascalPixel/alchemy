#include "types.h"

extern u16 Data_02001000;

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void Func_0200322c(void)
{
    u16 *p = &Data_02001000;
    u16 v = 9;
    *p = v;
}
