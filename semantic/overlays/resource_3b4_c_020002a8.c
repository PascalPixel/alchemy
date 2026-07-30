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
 * resource_3b4 owner at 0x020002a8, 164 bytes: code 0x020002a8-0x0200033f and
 * the pool words 0x0200a644, 0xffff0000 and 0x0200a684 at 0x02000340.
 *
 * A movement guard for one object record.  The record's halfword at +6 carries
 * the facing in its top nibble (`lsrs #12`), which indexes the 16-word step
 * table `Data_0200a644` -- in-image data at file offset 0x2644 under this
 * overlay's 0x02008000 link base, sixteen words ending exactly where
 * `Data_0200a684` begins.
 *
 * Each step word packs two 16.16 deltas: masking with 0xffff0000 keeps the X
 * delta in place, and shifting left by 16 lifts the low halfword into the Z
 * delta.  The candidate position is the record's words at +8, +12 and +16 with
 * those deltas applied, built on the stack and passed to the byte-exact
 * occupancy lookup at 0x0200006c (that source returns "the occupying slot or
 * 0"; it is declared here old-style because this site sets r0 and r1 while the
 * exact source names one parameter).
 *
 * When something occupies the cell, its kind -- the signed halfword at the
 * head of the chain record[0x50][0x28] -- is compared against the six-entry
 * allow-list `Data_0200a684`; a match returns immediately and leaves the
 * record alone.  Otherwise, and also when the cell is free, the terrain probe
 * Func_080091d8 is asked about the same candidate.  Either failure stops the
 * record: the words at +36 and +44 are zeroed and the words at +56 and +64 are
 * set to 0x80000000 (`movs r3,#128 / lsls r3,#24`).
 *
 * The candidate is rebuilt from the table a second time before the terrain
 * probe rather than reused, and the two calls swap the argument order
 * (probe, record) and (record, probe); both are as the assembly has them.
 *
 * The epilogue is `add sp,#12 / pop {r5, r6, r7} / pop {r1} / bx r1`: the
 * return address is popped into r1, so r0 survives and the owner returns the 0
 * loaded at 0x02000336.
 */

/* Sixteen packed step words, in-image at file offset 0x2644. */
extern u32 Data_0200a644[];
/* Six allowed occupant kinds, in-image at file offset 0x2684. */
extern s32 Data_0200a684[];

/* Byte-exact in this overlay: returns the occupying slot record, or 0. */
s32 *Func_0200006c();
/* Terrain probe; a positive result blocks the move. */
s32 Func_080091d8();

s32 Func_020002a8(s32 *record)
{
    u32 step;
    s32 candidate[3];
    s32 *occupant;
    s32 facing = *(u16 *)((u8 *)record + 6) >> 12;
    s32 index;

    step = Data_0200a644[facing];
    candidate[0] = record[2] + (s32)(step & 0xffff0000);   /* +8  */
    candidate[1] = record[3];                              /* +12 */
    candidate[2] = record[4] + (s32)(step << 16);          /* +16 */

    occupant = Func_0200006c(candidate, record);
    if (occupant != 0) {
        s32 kind = **(s16 **)((u8 *)*(s32 **)((u8 *)occupant + 0x50) + 0x28);

        for (index = 0; index <= 5; index++) {
            if (kind == Data_0200a684[index]) return 0;
        }

        record[9] = 0;              /* +36 */
        record[11] = 0;             /* +44 */
        record[14] = 0x80000000;    /* +56 */
        record[16] = 0x80000000;    /* +64 */
    }

    step = Data_0200a644[facing];
    candidate[0] = record[2] + (s32)(step & 0xffff0000);
    candidate[1] = record[3];
    candidate[2] = record[4] + (s32)(step << 16);

    if (Func_080091d8(record, candidate) > 0) {
        record[9] = 0;
        record[11] = 0;
        record[14] = 0x80000000;
        record[16] = 0x80000000;
    }

    return 0;
}
