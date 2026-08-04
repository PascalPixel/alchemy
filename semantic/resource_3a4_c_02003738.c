typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02003738, 280 bytes: per-frame slide/slope
 * step for a record -- advance it along its quadrant heading by a
 * table-scaled distance, tick a countdown with stings at 20 and 0, then
 * probe the ground behind it and set one of three tilt marks.
 *
 * Distance table: `*(s32 *)(0x0200d1b4 + (rec->s16 at +100 -
 * (y >> 16)) * 4 + 0x40)` -- 0x0200d1b4 is past the image end, runtime
 * scratch like the twins' 0x0200d238 counter; the index is the height
 * difference between a stored reference level (+100) and the record's
 * current 16.16 y (+12, rounded toward zero via the +0xffff fixup on
 * negatives).  The value is scaled twice with the established IWRAM
 * 16.16 multiply Func_03000118 (called raw at 0x03000118 through the
 * `mov ip, pc / bx r3` idiom): by 0xc000 (0.75) for the forward step
 * that is written back to the record's x/z, and by 0x18000 (1.5),
 * negated, for a backward probe that is NOT written back -- its
 * (x, z) only feeds the second Func_080091a8(2, x, z) ground query.
 * The two query results pick the tilt: equal -> Func_08009080(rec, 2),
 * forward higher -> (rec, 3), else (rec, 4), and only while the
 * countdown at +102 is at most 20.
 *
 * The countdown at +102 (s16) decrements only while non-zero, fires
 * Func_080f9010(184) exactly when it reaches 20 (the compare is
 * `(count << 16) == 160 << 13`) and Func_080f9010(233) when it
 * reaches 0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus high-register saves at
 * 0x02003738, `sub sp, #12`, through the unwind at 0x2003832-0x2003840,
 * trailing literal pool at 0x2003842-0x200384f, immediately followed by
 * the next unindexed candidate at 0x02003850.  One argument (the
 * record pointer), void.
 *
 * All 9 bl sites resolve with the `+2` stored-displacement rule (the
 * two IWRAM multiplies are bx calls, not bl):
 *   0x200379c (184), 0x20037ac (233) -> Func_080f9010   sound/sting
 *   0x20037d0, 0x20037f6 -> Func_08000128   displace (scale, heading, pos)
 *   0x20037de, 0x2003800 -> Func_080091a8   ground query (2, x, z)
 *   0x2003816 (2), 0x2003824 (3), 0x200382e (4) -> Func_08009080
 */

void Func_08000128();          /* displace (scale, heading, position) */
s32 Func_080091a8();           /* ground query (2, x, z) */
void Func_08009080();          /* mark record (record, n), established */
void Func_080f9010();          /* sound/sting, established */

typedef s32 (*Multiply16_16_02003738)(s32 value, s32 scale);

void Func_02003738(u8 *record)
{
    s32 probe[3];
    s32 heading;
    s32 y;
    s32 distance;
    s16 *countdown;
    s32 forward;
    s32 backward;
    Multiply16_16_02003738 multiply16_16 =
        (Multiply16_16_02003738)0x03000118;

    heading = *(u16 *)(record + 6) & 0xc000;

    y = *(s32 *)(record + 12);
    if (y < 0) {
        y += 0xffff;
    }
    distance = *(s32 *)((u8 *)0x0200d1b4
                        + (*(s16 *)(record + 100) - (y >> 16)) * 4 + 0x40);

    countdown = (s16 *)(record + 102);
    if (*countdown != 0) {
        *countdown -= 1;
        if (*countdown == 20) {
            Func_080f9010(184);
        }
        if (*countdown == 0) {
            Func_080f9010(233);
        }
    }

    probe[0] = *(s32 *)(record + 8);
    probe[1] = *(s32 *)(record + 12);
    probe[2] = *(s32 *)(record + 16);
    Func_08000128(multiply16_16(distance, 0xc000), heading, probe);
    *(s32 *)(record + 8) = probe[0];
    *(s32 *)(record + 16) = probe[2];
    forward = Func_080091a8(2, probe[0], probe[2]);

    Func_08000128(-multiply16_16(distance, 0x18000), heading, probe);
    backward = Func_080091a8(2, probe[0], probe[2]);

    if (*countdown <= 20) {
        if (forward == backward) {
            Func_08009080(record, 2);
        } else if (forward > backward) {
            Func_08009080(record, 3);
        } else {
            Func_08009080(record, 4);
        }
    }
}
