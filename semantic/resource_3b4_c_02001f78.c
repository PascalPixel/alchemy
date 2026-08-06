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
 * resource_3b4 owner at 0x02001f78, 96 bytes: code 0x02001f78-0x02001fc3 and
 * the pool words 0x03001ebc, 0x02000240, 0x75, 0x76 and 0x78 at 0x02001fc4.
 *
 * Store 516 (`movs r3,#129 / lsls r3,#2`) into the workspace word at +448
 * (`movs r2,#224 / lsls r2,#1`), then run the per-edition fixups selected by
 * the shared global `Data_02000240[224]`.  That global and the three constants
 * 0x75/0x76/0x78 are the same selector the byte-exact
 * resource_3b4_c_020009f0.c, _02000a50.c and _02000ee0.c switch on; those
 * sources spell the constants `(s32)&Value_000000xx` because the reference
 * materialises them from the pool, and the numeric form here is the same value.
 *
 * The selector is re-read after each fixup (the assembly reloads
 * `ldrh r1,[r5,#0]`), so a fixup that rewrites it changes which of the later
 * tests fire.  The first test uses the signed halfword loaded before the
 * stores; the later two sign-extend the reloaded halfword with `lsls #16 /
 * asrs #16`, which is the same value.
 *
 * The epilogue is `pop {r5} / pop {r1} / bx r1`: the return address is popped
 * into r1, so r0 survives and the owner returns the 0 loaded at 0x02001fbc.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];





extern void Func_02004122();
extern void Func_0200423a();
extern void Func_020042ee();
s32 Func_02001f78(void)
{
    s16 *selector = &Data_02000240[224];

    *(s32 *)(Data_03001ebc + 448) = 516;

    if (*selector == 0x75) Func_02004122();
    if (*selector == 0x76) Func_0200423a();
    if (*selector == 0x78) Func_020042ee();

    return 0;
}
