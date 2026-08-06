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
 * resource_3b4 owner at 0x02001308, 132 bytes: code 0x02001308-0x0200137b and
 * the pool words 0x000fffff, 0x03001ebc, 0x02000240 and 0x0000024a at
 * 0x0200137c.
 *
 * A tile trigger.  Slot 0's X and Z tiles are the placement words at +8 and
 * +16 divided by 0x100000; the trigger fires only while scene flag 0x220
 * (`movs r0,#136 / lsls r0,#2`) is clear, the shared globals
 * Data_02000240[294] and [293] permit it, and slot 0 stands in the 21..23 by
 * 10..11 rectangle.  The two range tests are the usual unsigned-window form:
 * `subs #21 / cmp #2 / bhi` and `cmp #9 / ble` with `cmp #11 / bgt`.
 *
 * Firing sets the flag and writes 91 into the workspace halfword at +386
 * (`movs r3,#193 / lsls r3,#1`), which is the same field the 0x0200141c and
 * 0x020017ac siblings write with their own values.
 *
 * The byte offsets 0x24a and 147 << 2 into `Data_02000240` are elements 293
 * and 294 of that s16 array; the byte-exact sources for this overlay index the
 * same array as `Data_02000240[224]`.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

/* Returns the record for a numbered slot. */
s32 *Func_0200384e();
/* Scene flag test: zero means not yet set. */
s32 Func_0200383a();
/* Scene flag set. */
void Func_0200387a();

void Func_02001308(void)
{
    s32 *slot = Func_0200384e(0);
    s32 x = slot[2] / 0x100000;   /* +8  */
    s32 z = slot[4] / 0x100000;   /* +16 */
    u8 *workspace = Data_03001ebc;

    if (Func_0200383a(0x220) != 0) return;
    if (Data_02000240[294] != 0) return;
    if (Data_02000240[293] == 8) return;
    if ((u32)(x - 21) > 2) return;
    if (z <= 9) return;
    if (z > 11) return;

    Func_0200387a(0x220);
    {
        s16 *field = (s16 *)(workspace + 386);
        u16 value = 91;

        *field = value;
    }
}
