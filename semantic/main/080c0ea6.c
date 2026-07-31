#include "types.h"

/*
 * Main-image core-band gap at 0x080c0ea6, 18 bytes (census kind
 * `executable_gap_continuation`, manifest retention `merge_with_owner`).
 *
 * A single hardware register write: sets `REG_BLDCNT` (0x04000050, the
 * GBA alpha-blend/special-effects control register) to 0xbf. The
 * leading `movs r0,r0` is a 2-byte alignment pad (the row's own
 * address, 0x080c0ea6, is not 4-aligned, so the pad brings the two
 * `ldr rN,[pc,#imm]` loads that follow onto 4-aligned PC-relative
 * targets); the two trailing pool words (0x000000bf, 0x04000050) are
 * both reached by those loads and nothing branches to them.
 */

void Func_080c0ea6(void)
{
    *(volatile u16 *)0x04000050 = 0xbf;
}
