/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.  The band
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
 * resource_3b4 owner at 0x020017ac, 140 bytes: code 0x020017ac-0x02001827 and
 * the pool words 0x000fffff, 0x03001ebc, 0x0000024a and 0x02000240 at
 * 0x02001828.
 *
 * Sibling of 0x02001308 and 0x0200141c.  This one tests Data_02000240[293]
 * against 12 *before* the scene flag, requires X == 19 and Z in 15..16, and
 * writes 96 into the workspace halfword at +386.
 *
 * The prologue saves r8 by hand (`mov r7,r8 / push {r7}`) and the epilogue
 * restores it (`pop {r3} / mov r8,r3`) before `pop {r5, r6, r7} / pop {r0} /
 * bx r0`, so the owner is void.  r8 only carries the X tile across the calls.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

/* Returns the record for a numbered slot. */
s32 *Func_02003cf6();
s32 Func_02003cf2();
void Func_02003d22();
                     
/* Scene flag test: zero means not yet set. */
                    
/* Scene flag set. */

void Func_020017ac(void)
{
    s32 *slot = Func_02003cf6(0);
    s32 x = slot[2] / 0x100000;   /* +8  */
    s32 z = slot[4] / 0x100000;   /* +16 */
    u8 *workspace = Data_03001ebc;

    if (Data_02000240[293] == 12) return;
    if (Func_02003cf2(0x220) != 0) return;
    if (Data_02000240[294] != 0) return;
    if (x != 19) return;
    if ((u32)(z - 15) > 1) return;

    Func_02003d22(0x220);
    *(s16 *)(workspace + 386) = 96;
}
