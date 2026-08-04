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
 * resource_3b4 owner at 0x02001bc4, 100 bytes: code 0x02001bc4-0x02001c1b and
 * the pool words 0x02009e95, 0x00000207 and 0x00000206 at 0x02001c1c.
 *
 * Publish the overlay's own 0x02001e94 as a callback through Func_080000d8 --
 * the pool word 0x02009e95 is 0x1e94 + 0x8000 + 1, i.e. that function's
 * in-image address with the Thumb bit, which is one of the witnesses for this
 * overlay's 0x02008000 link base.  Then place slot 14, repaint one collision
 * cell whose shape depends on whether scene flag 0x207 is already set, run the
 * step at 0x02001ec0, and set scene flag 0x206.
 */

void Func_080000d8();
void Func_0808a0f0();
/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
void Func_080091c0();
void Func_02001ec0();
/* Scene flag set. */
void Func_080770c8();

/* The callback this owner installs; defined at 0x02001e94 in this overlay. */
void Func_02001e94(void);

void Func_02001bc4(void)
{
    Func_080000d8(Func_02001e94);
    Func_0808a0f0(14, 0, 0);

    if (Func_080770c0(0x207) != 0) {
        Func_080091c0(58, 36, 1, 1, 45, 43);
    } else {
        Func_080091c0(46, 43, 1, 1, 45, 43);
    }

    Func_02001ec0();
    Func_080770c8(0x206);
}
