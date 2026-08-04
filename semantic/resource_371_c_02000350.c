#include "types.h"

/*
 * Resource 371 owner at 0x02000350 (44 bytes, 0 calls).
 *
 * Complete owner: `push {lr}` at 0x02000350 and `pop {r1} ; bx r1` at
 * 0x02000372.  The popped register is r1, not r0, so r0 survives the return
 * and IS the result — the constant 1 set at 0x02000370.  Bytes
 * 0x02000376-0x0200037b are alignment plus the single literal pool word
 * 0x03001e40; they are reached only by `ldr r3, [pc, #20]`.
 *
 * 0x03001e40 is an IWRAM word this overlay reads all over (see the sibling
 * owners at 0x02003f10/0x02003f4c/0x02003f88, which mask it with 1, 2 and
 * 0xf).  It is a live status/flag word, not overlay image data.
 *
 * The flag byte at object + 0x54 is cleared with `eors` against the same
 * bit that was just tested, which is a clear of bit 0 only.
 *
 * STILL-OPEN (4 differing bytes at baseline routing, 2 with -fno-regmove
 * tried standalone but not installed since it does not reach exact):
 * the reference's first `1 & value` test materializes the constant into
 * the AND's destination register (`adds r3,r1,#0` / `ands r3,r2`); every
 * source phrasing tried here (`1 & value`, `value & 1`, a separate
 * `masked` local) still compiles to the value-first form
 * (`adds r3,r2,#0` / `ands r3,r1`) because AND is commutative and the
 * front end canonicalizes it before this choice is visible in the tree.
 * alchemist.ts refused (tier: unaligned only, no original-order row).
 * overlay_mode_cohort.ts singles found -fno-regmove closes the mirrored
 * `eors` pair at the end (matches the documented NO_REGMOVE_SOURCES
 * fingerprint in tools/alchemy_gcc.ts) but leaves this first `ands` pair
 * at differing=2, not exact, so no route entry was added.
 */

extern u32 Data_03001e40;

s32 Func_02000350(u8 *object)
{
    u8 *flags = object + 0x54;
    u8 value = *flags;

    if ((value & 1) != 0 && (Data_03001e40 & 1) != 0) {
        *flags = (u8)(1 ^ value);
    }
    return 1;
}
