#include "types.h"

/*
 * resource_383 owner at 0x020048c8, 612 bytes (0x020048c8-0x02004b2b):
 * code 0x020048c8-0x02004b1f and a three-word literal pool at 0x02004b20
 * (0x03001ebc, 0xfd92f005, 0x0200e4f8).
 *
 * This is the per-frame handler that Func_02004b2c installs (as the pool word
 * 0x0200c8c9 = file offset 0x48c8 plus the Thumb bit).  It steers three
 * followers - object 2, actor 24 and actor 25 - onto walkable cells:
 *
 *   * object 2 heads towards object 0, using Func_08000100 to turn the vector
 *     between them into an angle;
 *   * actors 24 and 25 each pick a random heading, and if that fails they
 *     retry with the heading reversed (+0x8000, half a turn).
 *
 * For each follower the pattern is the same and reuses this overlay's own
 * routines: Func_02004704 maps the follower's world position to its 16-byte
 * cell record, Func_02004754 chooses the candidate exit closest to a desired
 * heading, Func_02004840 tests whether that exit is free (0 = free), and
 * Func_020048a4 commits the move.  Func_08009080 then puts the follower into
 * walk state 2 (moving) or 1 / 4 (blocked).
 *
 * The +56 test `record->field_56 == 0x80000000` gates each follower on being
 * idle; the record layout (+8 x, +16 y, +6 heading, +56 state) is the one the
 * byte-exact assets/code/resource_383_c_02004704.c and _02004840.c already
 * prove.
 *
 * All 31 `bl` sites are placed and reach 9 distinct callees; 16 sites are
 * calls to this overlay's own four helpers and 15 reach the import veneer
 * table.  Targets come from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset =
 * stored displacement + 2).  The tool also reports one `unknown` site at
 * 0x02004b24: that is the literal pool word 0xfd92f005, which merely happens
 * to decode as a BL pair, not a call.
 *
 * Epilogue restores r8-fp and ends `pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner is void.
 *
 * UNCERTAINTIES.
 *  - The comparison at 0x02004966 is against that same 0xfd92f005 pool word,
 *    i.e. signed -40103931.  Its left operand is a sign-extended s16, so the
 *    `ble` can never be taken and the test is dead as written.  It is
 *    reproduced literally below rather than folded away, because folding it
 *    would hide a constant that may well be mis-sized in the original source.
 *  - Func_080000f8 is called with no argument register set up by this owner
 *    (r0 still holds the cell record from the preceding call), and its result
 *    is used as a random 16-bit quantity.  It is declared and called with no
 *    arguments here; if it turns out to take one, the value it receives is
 *    that stale record pointer.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_020097a0();
u8 *Func_020097b2();
u8 *Func_02008ffe(s32 *position);
s32 Func_020095ba();
u8 *Func_020090e6(u8 *cell, s16 *heading);
s32 Func_020091d8(u8 *exit);
void Func_02009248(s32 follower, u8 *exit);
void Func_02009658();
void Func_02009662();
u8 *Func_02009880();
u8 *Func_020090c8(s32 *position);
s32 Func_0200965c();
u8 *Func_02009150(u8 *cell, s16 *heading);
s32 Func_02009242(u8 *exit);
u8 *Func_0200916c(u8 *cell, s16 *heading);
s32 Func_0200925e(u8 *exit);
void Func_0200986e();
void Func_020096e0();
void Func_020092e6(s32 follower, u8 *exit);
void Func_020096f6();
u8 *Func_02009914();
u8 *Func_0200915c(s32 *position);
s32 Func_020096f0();
u8 *Func_020091e6(u8 *cell, s16 *heading);
s32 Func_020092d8(u8 *exit);
u8 *Func_02009202(u8 *cell, s16 *heading);
s32 Func_020092f4(u8 *exit);
void Func_02009904();
void Func_02009776();
void Func_0200937c(s32 follower, u8 *exit);
void Func_0200978c();
                                /* random source; see the note above */
                                /* vector -> angle, writes through its third argument */

                                /* returns the object/actor record */

/* This overlay's own helpers (byte-exact sources in assets/code for 0x4704,
 * 0x4840 and 0x48a4; semantic source in this tree for 0x4754). */

                            

/* In-image halfword at file offset 0x64f8: the stuck-frame counter that
 * Func_02004b2c clears when the scene opens. */
extern u16 Data_0200e4f8;

void Func_020048c8(void)
{
    u8 *workspace;
    u8 *leader;
    u8 *follower;
    u8 *cell;
    u8 *exit;
    s16 heading;
    s32 blocked;
    s32 angle;
    s32 dx;
    s32 dy;
    s32 distance;
    s32 delta;
    s32 spin;
    u16 count;

    leader = Func_020097a0(0);
    blocked = 0;
    workspace = *(u8 **)0x03001ebc;

    /* ---- object 2: steer towards object 0 ---------------------------- */
    follower = Func_020097b2(2);
    cell = Func_02008ffe((s32 *)(follower + 8));
    if (cell != 0 && *(s32 *)(follower + 56) == (s32)0x80000000) {
        dx = *(s32 *)(follower + 8) - *(s32 *)(leader + 8);
        dy = *(s32 *)(follower + 16) - *(s32 *)(leader + 16);

        /* Start from the leader's heading; Func_08000100 may replace it. */
        heading = *(s16 *)(leader + 6);
        angle = (s16)Func_020095ba(dy, dx, &heading);

        dx >>= 16;
        dy >>= 16;
        distance = dx * dx + dy * dy;

        if (*(s16 *)(workspace + 412) > 0 && distance <= 400) {
            delta = (s16)((u16)heading - (u16)angle);
            /* See the uncertainty note: the first bound is the pool word
             * 0xfd92f005, so only the second bound can ever reject. */
            if (delta > (s32)0xfd92f005 && delta < 0x1000) {
                goto steer2;
            }
        }
        if (distance > 64) {
            heading = *(s16 *)(follower + 6);
        }

    steer2:
        exit = Func_020090e6(cell, &heading);
        if (Func_020091d8(exit) == 0) {
            Func_02009248((s32)follower, exit);
            Func_02009658(follower, 2);
        } else {
            Func_02009662(follower, 1);
        }
    }

    /* ---- actor 24: random heading, then the reverse ------------------ */
    follower = Func_02009880(24);
    cell = Func_020090c8((s32 *)(follower + 8));
    if (cell != 0 && *(s32 *)(follower + 56) == (s32)0x80000000) {
        /* (random * 2) truncated to 16 bits, times 3, into the top three bits
         * of a quadrant, biased by 0xd0000000: one of eight headings. */
        spin = (u16)(Func_0200965c() << 1);
        heading = (s16)((u32)((spin * 3) << 29) >> 16);
        heading = (s16)(heading + *(u16 *)(follower + 6));

        exit = Func_02009150(cell, &heading);
        if (Func_02009242(exit) != 0) {
            heading = (s16)(*(u16 *)(follower + 6) + 0x8000);
            exit = Func_0200916c(cell, &heading);
            if (Func_0200925e(exit) == 0) {
                Func_0200986e(24, 2);
            } else {
                blocked = 1;
                Func_020096e0(follower, 4);
                goto actor25;
            }
        }
        Func_020092e6((s32)follower, exit);
        Func_020096f6(follower, 2);
    }

actor25:
    /* ---- actor 25: the same, with a finer heading step --------------- */
    follower = Func_02009914(25);
    cell = Func_0200915c((s32 *)(follower + 8));
    if (cell != 0 && *(s32 *)(follower + 56) == (s32)0x80000000) {
        spin = (s32)((u32)(Func_020096f0() * 3) >> 16);
        heading = (s16)((u32)((spin * 3) << 28) >> 16);
        heading = (s16)(heading + *(u16 *)(follower + 6));

        exit = Func_020091e6(cell, &heading);
        if (Func_020092d8(exit) != 0) {
            heading = (s16)(*(u16 *)(follower + 6) + 0x8000);
            exit = Func_02009202(cell, &heading);
            if (Func_020092f4(exit) == 0) {
                blocked += 2;
                Func_02009904(25, 2);
            } else {
                Func_02009776(follower, 4);
                goto tally;
            }
        }
        Func_0200937c((s32)follower, exit);
        Func_0200978c(follower, 2);
    }

tally:
    /* A frame in which anyone was blocked bumps the counter; once it passes
     * 29 the scene is nudged on by publishing 200 + the blocked mask into the
     * workspace halfword at +386.  A clean frame resets the counter. */
    if (blocked != 0) {
        count = (u16)(Data_0200e4f8 + 1);
        Data_0200e4f8 = count;
        if ((u32)(count << 16) > 0x1d0000) {
            *(s16 *)(workspace + 386) = (s16)(blocked + 200);
        }
    } else {
        Data_0200e4f8 = (u16)blocked;
    }
}
