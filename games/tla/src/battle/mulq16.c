#include "types.h"
#include "io_reg.h"
#include "iwram_call.h"

void Func_081269ac(void)
{
    /* FAKEMATCH: the native loads the blend value from the pool before it starts the register address; the plain store starts the address first (one reordered row, differ 2). Only a halfword register loads from the pool and a u16 local is widened, so the value is pinned to the r3 it already takes, as in f18284c.c. Measured against it: a pointer local, the address as base and offset, and the value in a local of each width. */
    register u16 blend __asm__("r3") = 0xbf;

    /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is loaded. */
    __asm__("" : "+l"(blend) : : "r2");
    REG_BLDCNT = blend;
}

s32 Func_081269bc(s32 a, s32 b)
{
	return Iwram_MulQ16(a, b);
}
