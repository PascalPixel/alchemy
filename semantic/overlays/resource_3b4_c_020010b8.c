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
 * resource_3b4 owner at 0x020010b8, 78 bytes.
 *
 * The shared "run this numbered step once" body.  Its callers are the
 * selector wrappers 0x02001108 (0), 0x02001114 (1), 0x02001120 (2) and
 * 0x0200115c (3).
 *
 * Guard order: slot 0's halfword at +6 must be 0xc000 (`movs r2,#192 /
 * lsls r2,#8`); the scene flag numbered 2496 + selector (`movs r3,#156 /
 * lsls r3,#4`) must still be clear; and Func_08077040(244) must not return -1.
 * Only then is the flag set and the step at 0x02000f40 run with the selector
 * folded into 0x100 by `orrs`, after which Func_08077048(244) closes the pair
 * opened by Func_08077040.
 *
 * Func_080770c0/Func_080770c8 are the scene-flag test and set pair; the
 * semantic sources for resource_39f use them the same way.
 *
 * The epilogue is `pop {r5, r6} / pop {r0} / bx r0`, so the owner is void.
 */

/* Returns the record for a numbered slot. */
u8 *Func_0808a080();
/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
/* Scene flag set. */
void Func_080770c8();
/* Returns -1 when the subject is unavailable. */
s32 Func_08077040();
void Func_08077048();
void Func_02000f40();

void Func_020010b8(s32 selector)
{
    u8 *slot = Func_0808a080(0);
    s32 flag;

    if (*(u16 *)(slot + 6) != 0xc000) return;

    flag = selector + 2496;
    if (Func_080770c0(flag) != 0) return;
    if (Func_08077040(244) == -1) return;

    Func_080770c8(flag);
    Func_02000f40(0x100 | selector);
    Func_08077048(244);
}
