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
 * resource_3b4 owner at 0x020011d8, 44 bytes.
 *
 * Repaint a 1x2 collision strip through Func_080091b8 -- four register
 * arguments plus the two stack words at [sp,#0] and [sp,#4] -- then notify
 * Func_0808a0f0 for slot 12 with the pair 0x80000, 0x80000 (both materialised
 * as `movs #0x80 / lsls #12`).
 */

void Func_080091b8();
void Func_0808a0f0();

void Func_020011d8(void)
{
    Func_080091b8(24, 48, 1, 2, 25, 48);
    Func_0808a0f0(12, 0x80000, 0x80000);
}
