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
 * resource_3b4 owner at 0x02000e50, 120 bytes: code 0x02000e50-0x02000eb3 and
 * the pool words 0x03001ebc, 0x000009ca, 0x0000024a, 0x02000240 and
 * 0x0200a808 at 0x02000eb4.
 *
 * Only while scene flag 0x9ca is already set and the shared selector
 * Data_02000240[293] is not 15: copy slot 0's word at +0x30 into slot 15, then
 * look up a pair of words for the stage the workspace halfword at +364
 * (`movs r2,#182 / lsls r2,#1`) names and hand them to Func_0808a0c8 for slot
 * 15.
 *
 * The table is `Data_0200a808`, eight bytes per entry, indexed by
 * `stage - 30` (`subs #30 / lsls #3`).  Its pool word 0x0200a808 is in-image
 * data at file offset 0x2808 under this overlay's 0x02008000 link base, the
 * same way the byte-exact sources for this overlay name `Data_0200a898` and
 * friends.  The overlay image is writable EWRAM, so the table is not const.
 *
 * Recorded as observed rather than corrected: the second copy reads slot 0's
 * word at +0x30 again -- not +0x34 -- and stores it into slot 15's +0x34.  The
 * two `ldr r3,[r0,#48]` encodings are identical, so this is what the reference
 * does.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
/* In-image table at file offset 0x2808, two words per stage. */
extern s32 Data_0200a808[];

/* Scene flag test: zero means not yet set. */
s32 Func_080770c0();
/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
void Func_0808a0c8();

void Func_02000e50(void)
{
    u8 *workspace = Data_03001ebc;
    s32 stage;

    if (Func_080770c0(0x9ca) == 0) return;
    if (Data_02000240[293] == 15) return;

    stage = *(s16 *)(workspace + 364);

    Func_0808a080(15)[12] = Func_0808a080(0)[12];   /* +0x30 */
    Func_0808a080(15)[13] = Func_0808a080(0)[12];   /* +0x34 from +0x30 */

    Func_0808a0c8(15,
                  Data_0200a808[(stage - 30) * 2],
                  Data_0200a808[(stage - 30) * 2 + 1]);
}
