typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02003668, 188 bytes: the counter-clockwise
 * MIRROR TWIN of Func_020035ac (resource_3a4_c_020035ac.c) -- the same
 * quarter-turn swing around the grid point one probe-length ahead, with
 * every rotation constant negated.
 *
 * Cross-verified against the twin's own bytes, colour-blend discipline:
 * shared body confirmed instruction for instruction (probe build,
 * Func_08000128(0x180000, heading, probe) displacement, the
 * `+0x80000 & 0xfff00000` grid-pivot rounding, heading += 0x8000
 * re-aim, 16 one-frame steps, stings 184 and 233).  Real deltas, each
 * read from this row's own pool words:
 *   - initial snap is (heading - 0x4000) & 0xc000 (pool 0xffffc000)
 *     where the twin adds +0x4000: the quadrant boundary BEHIND;
 *   - the mark call is Func_08009080(record, 6), twin passes 5;
 *   - the loop steps heading += -0x400 (pool 0xfffffc00), a
 *     counter-clockwise quarter turn, twin steps +0x400;
 *   - the write-back heading is `heading - 0x4000`, twin adds.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus high-register saves at
 * 0x02003668, `sub sp, #12`, through the unwind at 0x2003708-0x2003716,
 * trailing literal pool at 0x2003718-0x2003723.  The 20-byte owner at
 * 0x02003724 that follows is a separate function, not this row.  One
 * argument (the record pointer), void.
 *
 * All 7 bl sites resolve with the `+2` stored-displacement rule:
 *   0x2003698, 0x20036de -> Func_08000128   displace (scale, heading, pos)
 *   0x20036bc -> Func_08009080              mark record (record, n)
 *   0x20036c2, 0x2003704 -> Func_080f9010   sound/sting (id)
 *   0x20036f2 -> Func_080000c0              wait n frames, established
 */

void Func_08000128();          /* displace (scale, heading, position) */
void Func_08009080();          /* mark record (record, n), established */
void Func_080f9010();          /* sound/sting, established */
void Func_080000c0();          /* wait n frames, established */

void Func_02003668(u8 *record)
{
    s32 probe[3];
    s32 pivot_x;
    s32 pivot_z;
    s32 heading;
    s32 i;

    heading = (*(u16 *)(record + 6) + -0x4000) & 0xc000;

    probe[0] = *(s32 *)(record + 8);
    probe[1] = *(s32 *)(record + 12);
    probe[2] = *(s32 *)(record + 16);
    Func_08000128(0x180000, heading, probe);    /* 192 << 13 */

    pivot_x = (probe[0] + 0x80000) & 0xfff00000;
    pivot_z = (probe[2] + 0x80000) & 0xfff00000;

    heading += 0x8000;                          /* 128 << 8 */
    Func_08009080(record, 6);
    Func_080f9010(184);

    for (i = 15; i >= 0; i--) {
        heading += -0x400;                      /* pool 0xfffffc00 */
        probe[0] = pivot_x;
        probe[2] = pivot_z;
        Func_08000128(0x180000, heading, probe);
        *(s32 *)(record + 8) = probe[0];
        *(s32 *)(record + 16) = probe[2];
        *(u16 *)(record + 6) = heading - 0x4000;
        Func_080000c0(1);
    }

    Func_080f9010(233);
}
