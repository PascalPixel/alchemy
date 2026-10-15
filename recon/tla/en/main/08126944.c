/* Near miss: differ 4 of 20 (8 plain). The Broken Seal's
 * Graphics_FadeAlphaOut. The native loads BLDCNT's value from the pool between
 * the first two rows of its address, starts BLDALPHA's address (movs r7) before
 * the store, then loads the 0x1010 base between that address's rows. Pinning
 * the value to r3 fixes the first part (differ 6); a full empty barrier after
 * the store then keeps the loop setup after it, so movs r7 comes one row late
 * (differ 4). sched2 gives the store priority 1 and the native order needs 2,
 * as if something in the block depended on the store. Measured: the pin in a
 * one-pass block, the value in a one-pass block, an operand barrier, a memory
 * clobber with an output (differ 6 to 8), four minutes of permute (6). */
#include "types.h"
#include "io_reg.h"
#include "system.h"

void Func_08126944(void)
{
    s32 step;
    register u16 blend __asm__("r3") = 0x2044;

    REG_BLDCNT = blend;
    __asm__("" : : : "memory");
    for (step = 1; step <= 16; step += 2)
    {
        REG_BLDALPHA = 0x1010 - step;
        WaitFrames(1);
    }
}
