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
 * resource_3b4 owner at 0x02000fdc, 148 bytes: code 0x02000fdc-0x02001069, two
 * bytes of alignment, and the pool word 0x0000e666 at 0x0200106c.
 *
 * Called from 0x02001070 with 0x100.  Only bit 8 of the argument is examined
 * (`movs r3,#128 / lsls r3,#1 / ands r0,r3`); the rest of the word is ignored,
 * so the parameter is kept as a flag word rather than narrowed.
 *
 * With that bit set the owner waits 157 frames, retunes the camera through two
 * Func_080091f0 calls -- (0x20000, 0x20000, 0x10000), all three materialised as
 * `movs #128 / lsls`, then (-1, -1, 0xe666) from the pool -- repaints a 1x3
 * collision strip and runs Func_080000c0(60).  The trailing repaints run
 * unconditionally; r5 holds the constant 6 the last two calls store at [sp,#0].
 *
 * The epilogue is `add sp,#8 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.
 */

void Func_080f9010();
void Func_080091f0();
void Func_080091b8();
void Func_080000c0();
void Func_080091c0();

void Func_02000fdc(s32 flags)
{
    if ((flags & 0x100) != 0) {
        Func_080f9010(157);
        Func_080091f0(0x20000, 0x20000, 0x10000);
        Func_080091f0(-1, -1, 0xe666);
        Func_080091b8(84, 29, 1, 3, 70, 49);
        Func_080000c0(60);
    }

    Func_080091b8(85, 29, 1, 3, 70, 49);
    Func_080091c0(6, 49, 1, 1, 6, 50);
    Func_080091c0(6, 49, 1, 1, 6, 51);
}
