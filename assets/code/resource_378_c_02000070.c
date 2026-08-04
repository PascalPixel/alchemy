#include "types.h"

/*
 * resource_378 owner at 0x02000070, 220 bytes: select this scene's placement
 * script from its stored sub-state.
 *
 * Complete owner: `push {lr}` at 0x02000070 through `pop {r1} / bx r1` at
 * 0x0200012a-0x0200012c.  The return address is popped into r1, not r0
 * (HANDOVER section 0, epilogue rule), so r0 survives and IS the result: every
 * arm sets r0 to a script-table address and falls into that shared epilogue.
 * No argument register is read before being written, so it takes none.
 *
 * Two non-code regions lie inside the advertised span and are why the
 * inventory reports this row as calls=0 / code=34 of 220 with one unresolved
 * instruction -- it is the "strict-queue filter hides a dispatcher" tier from
 * HANDOVER section 0, and like the others it converts normally:
 *  - the 140-byte jump table at 0x0200008c-0x02000117, and
 *  - the 28-byte literal pool at 0x02000130-0x0200014b.
 * The linear walk stops at the table, which is exactly the documented tell.
 *
 * Link base.  Image offset 0 is the exported-entry veneer table; all six of
 * its words are odd and land on function starts at `word - 0x8000`:
 * 0x020085cd -> 0x05cc, 0x0200805d -> 0x005c, 0x02008069 -> 0x0068,
 * 0x02008071 -> 0x0070 (this owner, entry 3), 0x0200814d -> 0x014c,
 * 0x02008065 -> 0x0064.  The jump table's own base word 0x0200808c resolves
 * to file offset 0x008c, which is physically where the table sits -- the
 * strongest of the cheap witnesses, and it agrees.  So 0x02008000 is settled
 * for this overlay, and the five 0x0200bxxx pool words below are in-image
 * data, not RAM globals.
 *
 * Zero call sites (`bun tools/overlay_call_targets.ts resource_378 0070`
 * reports sites=0), which is correct rather than a decoding failure: the only
 * non-linear instruction in the owner is the `mov pc, r3` dispatch.
 *
 * THE SELECTOR.  `movs r2,#225 / lsls r2,#1` is 450, and the owner reads a
 * SIGNED halfword (`ldrsh`) at `0x02000240 + 450`.  That is the same word
 * resource_384's root at 0x020001e4 reads, built the same way -- a
 * cross-overlay confirmation that offset 450 of the shared scene-record block
 * is the scene sub-state.  0x02000240 sits below the 0x02008000 band on the
 * two-sided pool-word test, so it is a RAM global.
 *
 * THE TABLE.  `subs r3,#1 / cmp r3,#34 / bhi` bounds the index to 0..34, so
 * the table has exactly 35 entries and the selector is 1-based.  Its entries
 * are EVEN in-image addresses (`mov pc, rN` does not interwork on this core,
 * so bit 0 is not a Thumb flag here) and resolve to five distinct arms at
 * 0x0118, 0x011c, 0x0120, 0x0124 and 0x0128, each of which loads one pooled
 * table address and branches to the common return.  Read out of the table, the
 * mapping is:
 *   sub-state 1, 2          -> 0x0200bc0c
 *   sub-state 10, 11, 12    -> 0x0200bccc
 *   sub-state 20, 21        -> 0x0200bd2c
 *   sub-state 29, 32        -> 0x0200be04
 *   sub-state 35            -> 0x0200bccc   (the 10/11/12 table again)
 *   anything else, in range or out -> 0x0200bbf4
 * The 0x0128 arm is both the `bhi` out-of-range destination and 26 of the 35
 * in-range entries, so it is a genuine `default` and not a separate case.
 *
 * WHAT THE FIVE TABLES ARE.  Each is a run of 24-byte records terminated by
 * the word 0x0000ffff, in the shape `{0xffff0000 | selector, count, x, 0, z,
 * value}` with the coordinates in the engine's usual 16.16 fixed point:
 *   0x0200bccc: {0xffff0079, 1, 0x00c80000, 0, 0x00820000, 0x00013000}, ...
 *   0x0200bd2c: {0xffff0001, 1, 0x00d00000, 0, 0x00a80000, 0x0003d000}, ...
 *   0x0200be04: {0xffff0079, 1, 0x00c80000, 0, 0x00840000, 0x00014000}, ...
 *   0x0200bc0c: {0xffff0001, 1, 0,          0, 0,          0x00004000}, ...
 *   0x0200bbf4: 0x0000ffff -- the empty table, i.e. "place nothing".
 * The `0xffff0000 | selector` first word is the three-word scene-script record
 * head HANDOVER records from resource_389/resource_38e, widened here to six
 * words.  The x/z field positions agree with the tracked byte-exact sibling
 * assets/code/resource_378_c_02000030.c, which reads +8 and +0x10 of a record
 * as the two coordinates it differences.
 *
 * THE SELECTOR ADDRESS IS BUILT, NOT FOLDED.  The reference loads the pool
 * word 0x02000240 and reaches +450 with `movs r2,#225 / lsls r2,#1 / adds
 * r3,r3,r2`; a folded `(u8 *)0x02000240 + 450` pools 0x02000402 instead and
 * costs the head six bytes, which then puts the whole jump table four low
 * once the assembler realigns.  The `s32 off = 450;` local in its own block
 * is what forces the build -- the same lever that closed the dispatcher head
 * of resource_3b9:0x02001a4c.
 *
 * Uncertainties.  The record layout above is read off the data, not off a
 * consumer in this overlay, so the fields at +12 and +20 are named by position
 * only.  Why sub-state 35 shares the 10/11/12 table, and why 29 and 32 share
 * one while the sub-states between them do not, is a property of the scene
 * this overlay drives and is not decidable here.  The return type is spelled
 * as an opaque pointer for the same reason.
 */

/* In-image script tables, at `pool word - 0x8000` under the proven link base.
 * They are in EWRAM, which this project has established is writable and used
 * as save state, so they are deliberately not `const`. */
extern u8 Data_0200bbf4[];   /* empty table: place nothing */
extern u8 Data_0200bc0c[];
extern u8 Data_0200bccc[];
extern u8 Data_0200bd2c[];
extern u8 Data_0200be04[];

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */
extern u8 Data_02000240[];

void *Func_02000070(void)
{
    s16 sub_state;

    {
        s32 off = 450;

        sub_state = *(s16 *)(Data_02000240 + off);
    }
    switch ((s32)sub_state) {
    case 1:
    case 2:
        return Data_0200bc0c;

    case 10:
    case 11:
    case 12:
    case 35:
        return Data_0200bccc;

    case 20:
    case 21:
        return Data_0200bd2c;

    case 29:
    case 32:
        return Data_0200be04;

    default:
        return Data_0200bbf4;
    }
}
