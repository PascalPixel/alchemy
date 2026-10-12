/* Near miss: differ 2 of 6 (2026-10-12). The native loads the halfword 0xbf from
 * its pool, then builds the register address; a plain store builds the address
 * first, and a barrier on a temporary turns the pool load into movs #191. */
#include "types.h"

void Func_081269ac(void)
{
    u16 value = 0xbf;

    *(volatile u16 *)0x04000050 = value;
}
