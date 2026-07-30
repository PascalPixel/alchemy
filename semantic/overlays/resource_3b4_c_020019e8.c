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
 * resource_3b4 owner at 0x020019e8, 96 bytes.
 *
 * Four collision repaints through Func_080091c0 followed by setting slot 8's
 * byte at +85 to 1.  r5 holds the constant 42 used for [sp,#4] throughout, and
 * for [sp,#0] as well in the third call.  The epilogue is
 * `add sp,#8 / pop {r5} / pop {r0} / bx r0`, so the owner is void.
 */

void Func_080091c0();
/* Returns the record for a numbered slot. */
u8 *Func_0808a080();

void Func_020019e8(void)
{
    Func_080091c0(57, 42, 1, 1, 40, 42);
    Func_080091c0(57, 42, 1, 1, 41, 42);
    Func_080091c0(58, 42, 1, 1, 42, 42);
    Func_080091c0(62, 37, 3, 1, 37, 42);

    Func_0808a080(8)[85] = 1;
}
