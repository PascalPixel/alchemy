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
 * resource_3b4 owner at 0x0200141c, 128 bytes: code 0x0200141c-0x0200148b and
 * the pool words 0x000fffff, 0x03001ebc, 0x02000240 and 0x0000024a at
 * 0x0200148c.
 *
 * Sibling of 0x02001308: the same scene flag 0x220, the same
 * Data_02000240[294]/[293] gates (this one rejects 9 rather than 8), the
 * rectangle X == 10 by Z in 16..18, and the value 92 written into the
 * workspace halfword at +386.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
/* Scene flag set. */
void Func_080770c8();

void Func_0200141c(void)
{
    s32 *slot = Func_0808a080(0);
    s32 x = slot[2] / 0x100000;   /* +8  */
    s32 z = slot[4] / 0x100000;   /* +16 */
    u8 *workspace = Data_03001ebc;

    if (Func_080770c0(0x220) != 0) return;
    if (Data_02000240[294] != 0) return;
    if (Data_02000240[293] == 9) return;
    if (x != 10) return;
    if ((u32)(z - 16) > 2) return;

    *(s16 *)(workspace + 386) = 92;
    Func_080770c8(0x220);
}
