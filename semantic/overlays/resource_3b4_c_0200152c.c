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
 * resource_3b4 owner at 0x0200152c, 160 bytes: code 0x0200152c-0x020015bb and
 * the pool words 0x000fffff, 0x0000024a, 0x02000240 and 0x0200af74 at
 * 0x020015bc.
 *
 * Re-aim one of the three slots 10, 11 and 12 at slot 0's current X column.
 * The caller passes 0, 1 or 2 (the wrappers at 0x020015cc, 0x020015d8 and
 * 0x020015e4); the slot number is that value plus 10.
 *
 * Nothing happens when the shared selector Data_02000240[293] already equals
 * the slot number, or when the column has not changed since last time.  The
 * previous column is remembered per caller in `Data_0200af74`, in-image data
 * at file offset 0x2f74 under this overlay's 0x02008000 link base -- the last
 * four words of the image.  The overlay lives in writable EWRAM, so this is a
 * genuine stored cursor and not a constant table.
 *
 * On a change the slot is retuned (0x48000, 0x24000, both `movs #144 / lsls`),
 * 188 frames are waited out, and it is aimed at (16 * column + 8, 360).  The
 * extra Func_0808a0e0 nudge only runs while slot 0 is at Z tile 22 or lower.
 *
 * The prologue saves r8 and sl by hand and the epilogue restores them before
 * `pop {r5, r6, r7} / pop {r0} / bx r0`, so the owner is void.
 */

extern s16 Data_02000240[];
/* Per-caller remembered column, in-image at file offset 0x2f74. */
extern s32 Data_0200af74[];

/* Returns the record for a numbered slot. */
s32 *Func_0808a080();
void Func_0808a090();
void Func_080f9010();
void Func_0808a0b8();
void Func_0808a0e0();
void Func_0808a0e8();

void Func_0200152c(s32 which)
{
    s32 *origin = Func_0808a080(0);
    s32 column = origin[2] / 0x100000;   /* +8  */
    s32 row = origin[4] / 0x100000;      /* +16 */
    s32 slot = which + 10;

    if (Data_02000240[293] == slot) return;
    if (column == Data_0200af74[which]) return;

    Func_0808a090(slot, 0x48000, 0x24000);
    Func_080f9010(188);
    Func_0808a0b8(slot, (column << 4) + 8, 360);

    Data_0200af74[which] = column;

    if (row <= 22) {
        Func_0808a0e0(0, 0, 8);
    }
    Func_0808a0e8(0);
}
