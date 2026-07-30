/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/overlay_call_targets.ts`.  The band
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
 * resource_3b4 owner at 0x02001c28, 52 bytes: code 0x02001c28-0x02001c57 and
 * the pool word 0x000fffff at 0x02001c58.
 *
 * Run the scene at 0x02001bc4 once slot 9 has reached tile (45, 43).  Both
 * tiles are the placement words at +8 and +16 divided by 0x100000 (the
 * `adds 0x000fffff when negative / asrs #20` pair is C's truncating signed
 * division by that power of two).
 *
 * r0 happens to still hold the Z tile at the branch, but 0x02001bc4 opens by
 * loading r0 from its own pool, so nothing is passed.
 */

/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
void Func_02001bc4(void);

void Func_02001c28(void)
{
    s32 *slot = Func_0808a080(9);
    s32 x = slot[2] / 0x100000;   /* +8  */
    s32 z = slot[4] / 0x100000;   /* +16 */

    if (x == 45 && z == 43) {
        Func_02001bc4();
    }
}
