typedef int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_39e owner at 0x02002508, 68 bytes: a range test on a record
 * field that selects between two entirely different beats.
 *
 * Complete owner: `push {r5, lr}` at 0x02002508 through `pop {r5} /
 * pop {r0} / bx r0` at 0x0200253a-0x0200253e, then the three-word literal
 * pool 0x02002540-0x0200254b (0xffff5fff, 0x3ffe, 0x1a1c). The next
 * owner's prologue is exactly at 0x0200254c. **68 bytes**, measured to
 * the epilogue.
 *
 * Not found by the structural inventory walk (unindexed): reached only as
 * a published pointer. All 6 `bl` targets resolved with
 * `bun tools/overlay_call_targets.ts resource_39e 2508 254c` under the
 * `+2` rule.
 *
 * THE TEST IS A RANGE CHECK BUILT AS AN ADD AND AN UNSIGNED COMPARE, and
 * this is the whole content of the row. The halfword at record+6 is read
 * with `ldrh`, the pool constant 0xffff5fff is ADDED to it -- which is a
 * subtraction of 0xa001 in 32-bit wraparound -- and the result is compared
 * against 0x3ffe with `bhi`, an UNSIGNED higher. So the fall-through arm
 * is taken when the biased value is <= 0x3ffe, which is exactly the
 * closed interval 0xa001 <= value <= 0xdfff on the original halfword. The
 * two pool words are one idiom, not two constants: read 0xffff5fff as a
 * signed -0xa001 and the wraparound that makes the range test work
 * disappears. Transcribed as the arithmetic the reference performs, with
 * the interval recorded here rather than folded into the code.
 *
 * `bhi` and not `bgt` matters: the biased value is used unsigned, so a
 * record+6 below 0xa001 wraps to a very large number and takes the
 * OUT-of-range arm, which a signed reading would send the other way.
 *
 * Note the record is fetched BEFORE the bracket opens -- Func_0808a080(0)
 * at 0x0200250c precedes Func_0808a018 at 0x02002512 -- and only the
 * halfword survives in r5 across the call. The pointer itself is not
 * reused.
 *
 * Both arms converge on Func_0808a020 at 0x02002536; the in-range arm
 * reaches it by `b.n` over the other. It is written once below the
 * if/else.
 *
 * Uncertainty: the five callees are not identified beyond call shape.
 * What record+6 holds is unknown; only that it is a halfword and that the
 * interval above selects the Func_080b0010 arm.
 */

extern void Func_020068f0();
extern void Func_0200691c();
extern u8 *Func_02006912();
extern void Func_020069c8();
extern void Func_020069e0();
extern void Func_02006a98();

void Func_02002508(void)
{
    u8 *record;
    u32 biased;

    record = Func_02006912(0);
    biased = *(u16 *)(record + 6);
    Func_020068f0();

    biased = biased + 0xffff5fff;
    if (biased <= 0x3ffe) {
        Func_02006a98(13);
    } else {
        Func_020069c8(0x1a1c);
        Func_020069e0(13, 0);
    }

    Func_0200691c();
}
