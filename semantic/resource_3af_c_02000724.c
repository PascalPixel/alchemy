#include "types.h"

/*
 * resource_3af owner at 0x02000724, 376 bytes: a ten-state per-frame
 * behaviour machine driven off the s16 at record +100. Called once per
 * frame with the record; always returns 1.
 *
 * PUBLISHED, NOT CALLED. Thumb pointer 0x02008725 at 0x020048d4 in the
 * script-record table. Absent from every inventory and from
 * `tools/overlay-unindexed`; see resource_3af_c_02000b98.c for the
 * sweep that finds this class.
 *
 * SIBLING OF 0x02000598, for actor 22 rather than actor 21. The two
 * share their whole skeleton -- same ten-state layout, same jump-table
 * bound, same 0x80000000 triple-wait in states 4 and 8, same
 * record +99 selector, same shared tails -- and differ in the actor id,
 * the Func_08009150 constants, and one structural detail called out
 * under state 6 below. A halfword diff over the first 376 bytes shows
 * 80 identical halfwords against 108 differing ones, so this is a
 * genuine sibling rather than a near-copy: it was read in full, not
 * derived by sed.
 *
 * ITS OWN JUMP TABLE, the overlay's fourth. Ten words at
 * 0x02000740-0x02000767, bounded by `cmp r3,#9 / bls`, indexed by the
 * state directly. Entries are image addresses in the +0x8000 link form
 * (0x02008768 means image 0x02000768), and they disassemble as
 * plausible `strh`/`ldrh` instructions. The table sits INSIDE the body,
 * entered by `mov pc,r3` at 0x0200073c with the first arm resuming at
 * 0x02000768.
 *
 * Two shared tails, and telling them apart is what makes the states
 * readable:
 *   0x0200088a  write the accumulated value back to the state field
 *   0x0200088c  return 1 without touching the state
 * Most arms compute `state + 1` into r3 and fall to the first; the
 * guarded arms branch to the second to stall in place.
 *
 * The states:
 *   0  stall on a random draw -- `(draw * 40) >> 16` must be zero to
 *      advance, so roughly a 1-in-40 chance per frame. The multiplier
 *      is a shift/add chain (`r*4 + r = r*5`, then `<<3`).
 *   1  advance.
 *   2  set record +40 and +48 to 0x40000 and +52 to 0x20000, then
 *      Func_08009150(record, 0xb00000, 0, 0x2b80000); advance.
 *   3  advance.
 *   4  WAIT: all three of record +56, +60 and +64 must equal
 *      0x80000000 (built as `movs r2,#128 / lsls r2,#24`, and the
 *      second and third compares chain off the previous register
 *      rather than reloading the constant). When they do: advance and
 *      store immediately, play sound 152, then steer actor 21 by
 *      record +99 -- nonzero gives Func_0808a1b8(22, 0xd000, 0),
 *      zero gives 0 -- and take a second draw, `(draw * 4) >> 16`.
 *      Nonzero sets actor 22's own record +40 to 0x20000; zero calls
 *      Func_0808a1e8(22, 0x103, 0) and sets it to 0x60000 instead.
 *   5  advance.
 *   6  advance and store, set record +40/+48/+52 to 0x40000/0x20000/
 *      0x10000, then ONE Func_08009150 call whose two constants are
 *      chosen by record +99 -- (184, 168) if set, (202, 173) if clear,
 *      shifted left by 16 and 18 respectively. This is the structural
 *      difference from 0x02000598, which instead emits two separate
 *      Func_08009150 calls in the two arms. Same effect, different
 *      shape, and worth not flattening: the branch here selects
 *      registers and joins before the call.
 *   7  advance.
 *   8  the same three-way 0x80000000 wait as state 4, then advance.
 *   9  reset the state to 0, so the machine loops.
 *
 * Record +99 is read in states 4 and 6 as a two-way selector; it is the
 * same byte 0x020022c0 writes per actor (values 0, 1 and 2) while
 * staging its ensemble, which is the cross-link that gives it meaning
 * beyond "a flag".
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000724 through
 * `pop {r5, r6} / pop {r1} / bx r1` at 0x0200088e-0x02000892 -- the
 * unwind returns through r1, not r0, because r0 carries the return
 * value 1 loaded at 0x0200088c; two trailing pool words (0x02008740 --
 * the table's own base -- and 0x00000103) end at 0x0200089b, and the
 * next owner (0x0200089c, already tracked byte-exact) begins there.
 *
 * Second read done: the correction was reading state 6 as two calls by
 * analogy with 0x02000598. It is one call with a selected pair of
 * constants; the `b.n 0x02000858` at 0x02000852 joins before the `bl`.
 * Assuming a sibling matches its twin arm-for-arm is exactly the error
 * the twin-diff habit is supposed to prevent.
 *
 * Uncertainty: the record field roles and the constants passed to
 * Func_08009150 are transcribed; the 0x80000000 sentinel in +56/+60/+64
 * is exact.
 */

extern s32 Func_080000f8(void);
extern void Func_080f9010(s32 sound_id);
extern void Func_08009150(void *record, s32 arg1, s32 arg2, s32 arg3);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);

s32 Func_02000724(u8 *record)
{
    s16 *state;
    s32 next;
    u8 *other;
    u32 draw;
    s32 first;
    s32 second;

    state = (s16 *)(record + 100);
    if ((u32)*state > 9) {
        return 1;
    }

    switch (*state) {
    case 0:
        next = *(u16 *)state + 1;
        draw = ((u32)Func_080000f8() * 40) >> 16;
        if (draw != 0) {
            return 1;
        }
        break;

        next = *(u16 *)state + 1;
        break;
    case 1:

    case 2:
        *(s32 *)(record + 40) = 128 << 11;
        *(s32 *)(record + 48) = 128 << 11;
        *(s32 *)(record + 52) = 128 << 10;
        Func_08009150(record, 176 << 16, 0, 174 << 18);
        next = *(u16 *)state + 1;
        break;

    case 3:
        next = *(u16 *)state + 1;
        break;

    case 4:
        /* the sentinel is built as movs r2,#128 / lsls r2,#24 */
        if (*(s32 *)(record + 56) != (s32)0x80000000) {
            return 1;
        }
        if (*(s32 *)(record + 60) != *(s32 *)(record + 56)) {
            return 1;
        }
        if (*(s32 *)(record + 64) != *(s32 *)(record + 60)) {
            return 1;
        }
        *state = (s16)(*(u16 *)state + 1);
        Func_080f9010(152);
        if (record[99] != 0) {
            Func_0808a1b8(22, 208 << 8, 0);
        } else {
            Func_0808a1b8(22, 0, 0);
        }
        draw = ((u32)Func_080000f8() * 4) >> 16;
        if (draw != 0) {
            other = Func_0808a080(22);
            *(s32 *)(other + 40) = 128 << 10;
        } else {
            Func_0808a1e8(22, 0x103, 0);
            other = Func_0808a080(22);
            *(s32 *)(other + 40) = 192 << 11;
        }
        return 1;

    case 5:
        next = *(u16 *)state + 1;
        break;

    case 6:
        *state = (s16)(*(u16 *)state + 1);
        *(s32 *)(record + 40) = 128 << 11;
        *(s32 *)(record + 48) = 128 << 10;
        *(s32 *)(record + 52) = 128 << 9;
        if (record[99] != 0) {
            first = 184;
            second = 168;
        } else {
            first = 202;
            second = 173;
        }
        Func_08009150(record, first << 16, 0, second << 18);
        return 1;

    case 7:
        next = *(u16 *)state + 1;
        break;

    case 8:
        /* the sentinel is built as movs r2,#128 / lsls r2,#24 */
        if (*(s32 *)(record + 56) != (s32)0x80000000) {
            return 1;
        }
        if (*(s32 *)(record + 60) != *(s32 *)(record + 56)) {
            return 1;
        }
        if (*(s32 *)(record + 64) != *(s32 *)(record + 60)) {
            return 1;
        }
        next = *(u16 *)state + 1;
        break;

    default:                    /* case 9 */
        next = 0;
        break;
    }

    *state = (s16)next;
    return 1;
}
