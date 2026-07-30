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
 * resource_3b4 owner at 0x02001984, 72 bytes.
 *
 * Repaint one collision cell, and -- only when slot 13's record exists -- clear
 * its byte at +85 and set its byte at +35 to 2.  r5 keeps the first
 * Func_0808a080(13) result across the repaint; the second Func_0808a080(13)
 * call returns the same record, and the two stores are written through the two
 * different pointers exactly as the assembly does (`adds r0,#85` on the fresh
 * result, `adds r2,#35` on r5).  Finally scene flag 512 is set
 * (`movs r0,#128 / lsls r0,#2`).
 *
 * The epilogue is `add sp,#8 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.
 */

/* Returns the record for a numbered slot, or 0 when it has none. */
u8 *Func_0808a080();
void Func_080091c0();
/* Scene flag set. */
void Func_080770c8();

void Func_02001984(void)
{
    u8 *slot = Func_0808a080(13);

    Func_080091c0(40, 54, 1, 1, 40, 55);

    if (slot != 0) {
        Func_0808a080(13)[85] = 0;
        slot[35] = 2;
    }

    Func_080770c8(512);
}
