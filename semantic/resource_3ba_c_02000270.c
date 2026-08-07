#include "types.h"

/*
 * Resource 3ba, cutscene scene at 0x02000270.
 *
 * The owner is complete: `push {r5, r6, lr}` plus the high-register save
 * `mov r6, sl ; mov r5, r8 ; push {r5, r6}` and an 8-byte frame at 0x02000270,
 * with the matching unwind at 0x02000320..0x0200032c, an alignment halfword,
 * then the three-word literal pool 0x02000330-0x0200033b that closes the
 * 204-byte row.  Nothing live escapes the row.
 *
 * `pop {r0} ; bx r0` — r0 is the popped return address, so the owner returns
 * nothing.  The prologue writes no argument register before the first call
 * except r0 (loaded from the pool), so the owner takes no arguments.
 *
 * CALL ACCOUNTING.  Fourteen `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3ba --annotate`: Func_0808a080
 * x2 and Func_08009150 x2, plus one each of Func_080770c8, Func_0808a018,
 * Func_0808a208, Func_0808a210, Func_08009080, Func_0808a218, Func_08009158,
 * Func_0808a010, Func_080091c0 and Func_0808a020.  The C below reproduces that
 * multiset exactly; the inventory's `calls=14` agrees.
 *
 * SHAPE.  Story flag, camera setup, then the same three-store preparation
 * applied to two scene records (actor selectors 13 and 14) before each is
 * placed, the second becoming the camera target.  The bracket
 * Func_0808a018 ... Func_0808a020 around the body is the established
 * cutscene-mode bracket used all over this package.
 *
 * LINK BASE.  0x02008000, proven for this overlay from the byte-exact sibling
 * `assets/code/resource_3ba_c_02000158.c` (pool word 0x0200804d =
 * Func_0200004c + the Thumb bit).  None of this owner's pool words is an
 * address: 0x00000301, 0x0000cccc and 0x00006666 are all plain constants.
 *
 * UNCERTAINTIES.
 *  - Record fields.  +8 and +16 are the record's x and z as read back into the
 *    Func_08009150(record, x, y, z) placement call, matching the byte-exact
 *    and semantic sources elsewhere in the tree.  +0x30 and +0x34 receive the
 *    fixed pair 0xcccc / 0x6666 and +0x55 is cleared; those three fields are
 *    not otherwise identified here, so they are written through byte/word
 *    pointers rather than a named struct.
 *  - The y coordinate differs between the two placements — 0x80000 for the
 *    first record and 0x200000 for the second — while x and z are read back
 *    from the record itself.  Both are 16.16 (8.0 and 32.0).
 *  - Func_0808a210's four arguments are 16.16 values, not addresses:
 *    0x02580000 (600.0), -1, 0x00c80000 (200.0), 1.
 */

/* Old-style declarations: these imports' interfaces are not fully known, and
 * several of the names take different argument counts elsewhere. */
void Func_080770c8();           /* set a story flag */
void Func_0808a018();           /* enter cutscene presentation mode */
void Func_0808a020();           /* leave cutscene presentation mode */
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a010();           /* wait n frames */
void Func_08009080();           /* select presentation mode (record, mode) */
void Func_08009150();           /* place the record at (x, y, z) */
void Func_08009158();           /* re-attach the camera to a record */
void Func_080091c0();           /* six-argument renderer, last two on stack */

/* Used for its return value: the scene record for an actor selector. */
u8 *Func_0808a080();

void Func_02000270(void)
{
    u8 *record;

    Func_080770c8(0x301);

    record = Func_0808a080(13);

    Func_0808a018();
    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x02580000, -1, 0x00c80000, 1);
    Func_08009080(record, 3);
    Func_0808a218();

    *(s32 *)(record + 0x34) = 0x6666;
    *(record + 0x55) = 0;
    Func_08009150(record, *(s32 *)(record + 8), 0x80000,
                  *(s32 *)(record + 16));
    *(s32 *)(record + 0x30) = 0xcccc;

    record = Func_0808a080(14);

    *(record + 0x55) = 0;
    *(s32 *)(record + 0x34) = 0x6666;
    Func_08009150(record, *(s32 *)(record + 8), 0x200000,
                  *(s32 *)(record + 16));
    *(s32 *)(record + 0x30) = 0xcccc;

    Func_08009158(record);
    Func_0808a010(45);
    Func_080091c0(43, 12, 1, 1, 41, 12);
    Func_0808a020();
}
