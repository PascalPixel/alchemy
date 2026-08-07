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
 * resource_3b4 owner at 0x02001cf8, 108 bytes: code 0x02001cf8-0x02001d5f and
 * the pool word 0x000fffff at 0x02001d60.
 *
 * Read slot 10's three placement words at +12, +8 and +16 as tiles (each
 * divided by 0x100000 with the negative-bias idiom) and drive two independent
 * decisions from them.  First, while scene flag 0x300 is clear and the +12 tile
 * is at most 2, run 0x02001ca0 and set the flag.  Second, dispatch on the +16
 * tile: on row 55 the +8 tile picks 0x02001ccc (42) or 0x02001ca0 (38) -- both
 * tests are made, they are simply mutually exclusive -- and any other row runs
 * 0x02001c6c instead.
 *
 * The epilogue is `pop {r5, r6, r7} / pop {r0} / bx r0`, so the owner is void.
 */

/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
/* Scene flag set. */
void Func_080770c8();
void Func_02001ca0();
void Func_02001ccc();
void Func_02001c6c();

void Func_02001cf8(void)
{
    s32 *slot = Func_0808a080(10);
    s32 height = slot[3] / 0x100000;   /* +12 */
    s32 x = slot[2] / 0x100000;        /* +8  */
    s32 z = slot[4] / 0x100000;        /* +16 */

    if (Func_080770c0(0x300) == 0 && height <= 2) {
        Func_02001ca0();
        Func_080770c8(0x300);
    }

    if (z == 55) {
        if (x == 42) Func_02001ccc();
        if (x == 38) Func_02001ca0();
    } else {
        Func_02001c6c();
    }
}
