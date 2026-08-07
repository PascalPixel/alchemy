/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/lib/overlay_call_targets.ts`.  The band
 * 0x02002468-0x0200261f is an 8-byte-per-entry import veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <main-image address>`); calls landing there
 * are named by that trailing word, as the semantic resource_39f sources do.
 * Calls landing on an overlay prologue keep the overlay's `Func_0200xxxx` name.
 *
 * Link base: resource_3b4 is linked at 0x02008000.  The image is 0x2f84 bytes
 * and the table at 0x02002d40-0x02002f84 holds 59 words of the form
 * `<function offset> + 0x8000 + 1` (Thumb bit), e.g. 0x02008ec9 = Func_02000ec8.
 * So a pool word in 0x02008000-0x0200af83 is in-image data at value - 0x8000.
 * `Data_02000240` lies below the base and is referenced by byte-exact sources
 * in many other overlays, so it is a shared EWRAM global, not overlay data.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * is returned and the owner is void.
 *
 * Declarations are old-style because imports here are reached with differing
 * argument counts from different call sites.
 */
#include "types.h"

/*
 * resource_3b4 owner at 0x02001df8, 92 bytes: code 0x02001df8-0x02001e4d, two
 * bytes of alignment, and the pool word 0x000fffff at 0x02001e50.
 *
 * Run the four-cell repaint at 0x02001da0, then repaint one cell for each of
 * slots 11 and 12 at that slot's own X tile.  The tile is the word at +8
 * divided by 0x100000: the assembly adds 0x000fffff when the value is negative
 * before the arithmetic shift, which is exactly C's truncating signed division
 * by that power of two, and is the same idiom the byte-exact
 * resource_3b4_c_0200006c.c spells as `/ 0x10000`.
 *
 * r5 holds the constant 55 stored at [sp,#4] by both calls.  The epilogue is
 * `add sp,#8 / pop {r5} / pop {r0} / bx r0`, so the owner is void.
 */

void Func_02001da0();
/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
void Func_080091c0();

void Func_02001df8(void)
{
    Func_02001da0();

    Func_080091c0(53, 55, 1, 1, Func_0808a080(11)[2] / 0x100000, 55);
    Func_080091c0(53, 55, 1, 1, Func_0808a080(12)[2] / 0x100000, 55);
}
