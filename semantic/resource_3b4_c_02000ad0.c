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
 * resource_3b4 owner at 0x02000ad0, 152 bytes: code 0x02000ad0-0x02000b61, two
 * bytes of alignment, and the pool word 0x000009c8 at 0x02000b64.
 *
 * A once-only cutscene guarded by scene flag 0x9c8: the flag is tested, set
 * immediately, and the whole body is skipped when it was already set.
 *
 * The body brings slot 15 on, waits 152 frames, writes 0x80000 into its word
 * at +40, moves it to (584, 680) and hands control back.  Every constant is
 * materialised with `movs`/`lsls` pairs: 0x20000 and 0x4000 for
 * Func_0808a208, 258 for Func_0808a1f0, 0x10000 and 0x8000 for Func_0808a090,
 * 0x80000 for the +40 store, 146 << 2 and 170 << 2 for Func_0808a0d0, and
 * 0x4000 for the second Func_0808a1b8.
 *
 * Func_0808a1b8 is reached twice with three register arguments only; other
 * overlays reach the same import with four registers plus stack words, which
 * is why the declarations here are old-style.
 */

/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
/* Scene flag set. */
void Func_080770c8();
void Func_0808a018();
void Func_0808a208();
void Func_0808a220();
void Func_0808a218();
void Func_0808a1b8();
void Func_0808a1f0();
void Func_0808a138();
void Func_0808a010();
void Func_0808a090();
void Func_080f9010();
/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
void Func_0808a0d0();
void Func_0808a020();

void Func_02000ad0(void)
{
    if (Func_080770c0(0x9c8) != 0) return;
    Func_080770c8(0x9c8);

    Func_0808a018();
    Func_0808a208(0x20000, 0x4000);
    Func_0808a220(15, 1);
    Func_0808a218();
    Func_0808a1b8(15, 0, 20);
    Func_0808a1f0(15, 258);
    Func_0808a138(15, 2);
    Func_0808a010(20);
    Func_0808a090(15, 0x10000, 0x8000);
    Func_080f9010(152);

    Func_0808a080(15)[10] = 0x80000;   /* +40 */

    Func_0808a0d0(15, 584, 680);
    Func_0808a1b8(15, 0x4000, 20);
    Func_0808a020();
}
