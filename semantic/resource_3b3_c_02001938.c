#include "types.h"

/*
 * Resource 3b3 unindexed helper at 0x02001938 (184 bytes incl. pool,
 * 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,lr}` at 0x02001938, epilogue `pop {r5,r6} / pop {r1} / bx r1`
 * at 0x020019de-0x020019e2. The trailing pool at 0x020019e4-0x020019ec
 * (0xfff00000, 0x0000ffff, 0xffe00000) is included per the usual pool
 * rule, immediately followed by the next function's push {r5,r6,lr} at
 * 0x020019f0 (not part of this overlay's item-28 population, already
 * indexed elsewhere), so the span is exactly 0x02001938-0x020019f0,
 * 184 bytes.
 *
 * A proximity + priority conflict check between two objects: exact
 * position match on `+8`/`+12`/`+16` returns 0 immediately; otherwise
 * an X-axis window test (`+8`, +-0x100000), a "same Z cell" test
 * (`+12`, rounded division by 0x10000), and a Y-axis window test
 * (`+16`, b must be up to 0x200000 ahead of a) each fail-fast to 0 if
 * not satisfied. Once all three pass, a 2-bit priority field (bits
 * 2-3 of each object's linked-record `+9` byte) decides the outcome:
 * if `a`'s field is already `>= b`'s, return 1 unchanged; otherwise
 * clear a flag bit on `a`, copy `b`'s priority field into `a`'s `+9`
 * byte and `b`'s `+21` bits 2-3 into `a`'s `+21` byte, then return 1.
 *
 * Raw callee naming (no calls in this function; the queue's "2 calls"
 * count is the number of external call sites reaching this row, not
 * calls this function makes).
 */

static s32 RoundedZCell_02001938(s32 z)
{
    if (z < 0) {
        z += 0xffff;
    }
    return z >> 16;
}

s32 Func_02001938(u8 *a, u8 *b)
{
    if (*(s32 *)(b + 8) == *(s32 *)(a + 8) &&
        *(s32 *)(b + 12) == *(s32 *)(a + 12) &&
        *(s32 *)(b + 16) == *(s32 *)(a + 16)) {
        return 0;
    }

    if (*(s32 *)(b + 8) - 0x100000 >= *(s32 *)(a + 8)) {
        return 0;
    }
    if (*(s32 *)(a + 8) >= *(s32 *)(b + 8) + 0x100000) {
        return 0;
    }

    if (RoundedZCell_02001938(*(s32 *)(b + 12)) != RoundedZCell_02001938(*(s32 *)(a + 12))) {
        return 0;
    }

    if (*(s32 *)(b + 16) <= *(s32 *)(a + 16)) {
        return 0;
    }
    if (*(s32 *)(b + 16) - 0x200000 >= *(s32 *)(a + 16)) {
        return 0;
    }

    {
        u8 *linkedA = *(u8 **)(a + 0x50);
        u8 *linkedB = *(u8 **)(b + 0x50);
        s32 fieldB = (linkedB[9] >> 2) & 3;
        s32 fieldA = (linkedA[9] >> 2) & 3;

        if (fieldA < fieldB) {
            a[0x23] &= 0xfe;
            linkedA[21] = (linkedA[21] & 0xf3) | (linkedB[21] & 0x0c);
            linkedA[9] = (linkedA[9] & 0xf3) | (fieldB << 2);
        }
    }

    return 1;
}
