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
 * resource_3b4 owner at 0x02001b60, 100 bytes: code 0x02001b60-0x02001bbf and
 * the pool word 0x000fffff at 0x02001bc0.
 *
 * Compare the X tiles of slots 0 and 8 (each the word at +8 divided by
 * 0x100000 -- the `adds 0x000fffff when negative / asrs #20` pair is C's
 * truncating signed division).  The special cases only apply when slot 0 sits
 * on tile 38 and slot 8 does not; then slot 0's halfword at +6 selects one of
 * two notifications.  Every other combination, including an unrecognised
 * halfword, runs the three-step ordinary path.
 *
 * The epilogue is `pop {r5} / pop {r0} / bx r0`, so the owner is void.
 */

/* Returns the record for a numbered slot. */
u8 *Func_020040a6();
u8 *Func_020040ae();
void Func_02004174();
void Func_0200417a();
void Func_02003598();
void Func_02001c78();
void Func_02003680();

                     

                     

void Func_02001b60(void)
{
    u8 *first = Func_020040a6(0);
    u8 *other = Func_020040ae(8);

    if (*(s32 *)(first + 8) / 0x100000 == 38 &&
        *(s32 *)(other + 8) / 0x100000 != 38) {
        u16 facing = *(u16 *)(first + 6);

        /* r0 holds `facing` at both branches; whether either callee reads it
         * is unverified, so no argument is asserted. */
        if (facing == 0xc000) {
            Func_02004174();
            return;
        }
        if (facing == 0x4000) {
            Func_0200417a();
            return;
        }
    }

    Func_02003598();
    Func_02001c78();
    Func_02003680();
}
