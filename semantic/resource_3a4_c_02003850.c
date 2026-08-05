#include "types.h"

/*
 * resource_3a4 owner at 0x02003850, 376 bytes: the SLIDE DRIVER -- the
 * orchestrator that unifies this overlay's whole movement family.  It
 * launches a record onto a rail/slide and runs it tile by tile: the
 * marker code under the record picks a clockwise quarter-turn
 * (Func_020035ac, code 98), a counter-clockwise one (Func_02003668,
 * code 97), or a countdown re-arm (the exact Func_02003724, code 96 --
 * sets the +102 countdown to 33 with sting 0x120), every frame steps
 * through the slope slider Func_02003738, and code 99 ends the ride
 * with the tile-snap settle Func_02003484.  This is the row whose
 * pc-relative misdecodes ("Func_02007xxx past the envelope") triggered
 * the phantom-cluster hunt; under the +2 rule every local target is a
 * drafted or exact sibling.
 *
 * Arguments (r0 = slot id, r1 = heading or -1):
 * 1. rec = Func_0808a080(slot); companion = Func_0808a400(scene table
 *    +500 handle) -- the same companion the settle row fetches; open
 *    the Func_0808a018 bracket.  heading == -1 means "use the
 *    record's own +6 heading".
 * 2. Direction probe, at most four tries: displace a copy of the
 *    record's position by 1.0 (0x100000) along the heading; if the
 *    ground query Func_080091a8(2, x, z) equals the record's own y,
 *    that heading works; otherwise turn a quarter (+0x4000) and try
 *    again.  Four failures: return without touching anything.
 * 3. Launch: +0x22 state 2, companion parked at (0, 0),
 *    Func_08009048(sprite, 16), Func_0808a200(slot, 1),
 *    Func_0808a218(), Func_0808a208(0x100000, 0x20000), heading
 *    written back, +0x30 = 0x20000, +0x34 = 0xccc, +0x5b = 0, the
 *    +100 reference level set from y >> 16 (negative-rounded via
 *    +0xffff) and the +102 countdown zeroed -- the exact two fields
 *    the slope slider reads.  Then displace by 1.5 (0x180000), walk
 *    there with Func_08009150/Func_08009158, sting 233.
 * 4. Ride loop: switch (Func_080091b0(2, x, z)) { 98 -> 0x35ac;
 *    97 -> 0x3668; 96 -> 0x3724; 99 -> break out }, then
 *    Func_02003738(rec) and one frame, forever until 99; finish with
 *    Func_02003484(rec) and the Func_0808a020 bracket close.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus high-register saves at
 * 0x02003850, `sub sp, #12`, through the unwind at 0x20039b6-0x20039c4,
 * interior pool at 0x200391a-0x2003926 (hopped by the b.n at
 * 0x2003918), immediately followed by the exact owner at 0x020039c8.
 * Two arguments, void.
 *
 * All 21 bl sites resolve with the `+2` stored-displacement rule; 15
 * veneers, six overlay-local (0x35ac, 0x3668, 0x3724, 0x3738, 0x3484
 * and none other):
 *   0x2003860 -> Func_0808a080, 0x2003870 -> Func_0808a400
 *   0x2003876 -> Func_0808a018, 0x20039b2 -> Func_0808a020
 *   0x20038b0 (1.0), 0x200394a (1.5) -> Func_08000128
 *   0x20038ba -> Func_080091a8   ground query (2, x, z)
 *   0x20038dc -> Func_08009048   (sprite, 16)
 *   0x20038e4 -> Func_0808a200   (slot, 1), established
 *   0x20038e8 -> Func_0808a218
 *   0x20038f4 -> Func_0808a208   (0x100000, 0x20000), established
 *   0x2003956 -> Func_08009150   move to (rec, x, y, z)
 *   0x200395c -> Func_08009158   (rec)
 *   0x2003962 -> Func_080f9010   sting 233
 *   0x200396c -> Func_080091b0   marker code query, established
 *   0x20039a6 -> Func_080000c0   one frame
 */

u8 *Func_0808a080();           /* scene-record accessor, established */
u8 *Func_0808a400();           /* handle -> record, established */
void Func_0808a018();          /* open bracket, established */
void Func_0808a020();          /* close bracket, established */
void Func_08000128();          /* displace (scale, heading, position) */
s32 Func_080091a8();           /* ground query (2, x, z) */
void Func_08009048();          /* (sprite, n) */
void Func_0808a200();          /* (slot, n), established */
void Func_0808a218();
void Func_0808a208();          /* pair of 16.16 rates, established */
void Func_08009150();          /* move to (record, x, y, z) */
void Func_08009158();          /* (record) */
void Func_080f9010();          /* sound/sting, established */
s32 Func_080091b0();           /* marker code query, established */
void Func_080000c0();          /* wait n frames, established */
void Func_020035ac(u8 *record);     /* clockwise quarter-turn */
void Func_02003668(u8 *record);     /* counter-clockwise quarter-turn */
void Func_02003724();               /* re-arm countdown, exact */
void Func_02003738(u8 *record);     /* per-frame slope step */
void Func_02003484(u8 *record);     /* tile-snap settle */

void Func_02003850(s32 slot, s32 heading)
{
    s32 probe[3];
    u8 *record;
    u8 *companion;
    u32 tries;
    s32 y;
    s32 code;

    record = Func_0808a080(slot);
    companion = Func_0808a400(*(s32 *)((u8 *)0x02000240 + 500));
    Func_0808a018(companion);

    if (heading == -1) {
        heading = *(u16 *)(record + 6);
    }

    for (tries = 0; tries <= 3; tries++) {
        probe[0] = *(s32 *)(record + 8);
        probe[1] = *(s32 *)(record + 12);
        probe[2] = *(s32 *)(record + 16);
        Func_08000128(0x100000, heading, probe);    /* 128 << 13 */
        if (Func_080091a8(2, probe[0], probe[2])
            == *(s32 *)(record + 12)) {
            break;
        }
        heading += 0x4000;                          /* 128 << 7 */
    }
    if (tries == 4) {
        return;
    }

    Func_08009048(*(u8 **)(record + 0x50), 16);
    record[0x22] = 2;
    *(s32 *)(companion + 16) = 0;
    *(s32 *)(companion + 8) = 0;
    Func_0808a200(slot, 1);
    Func_0808a218();
    Func_0808a208(0x100000, 0x20000);               /* 128<<13, 128<<10 */

    record[0x5b] = 0;
    *(u16 *)(record + 6) = heading;
    *(s32 *)(record + 0x34) = 0xccc;
    *(s32 *)(record + 0x30) = 0x20000;

    y = *(s32 *)(record + 12);
    *(s16 *)(record + 100) = y >> 16;
    if (y < 0) {
        y += 0xffff;
    }
    *(s16 *)(record + 102) = 0;

    probe[0] = *(s32 *)(record + 8);
    probe[1] = *(s32 *)(record + 12);
    probe[2] = *(s32 *)(record + 16);
    Func_08000128(0x180000, heading, probe);        /* 192 << 13 */
    Func_08009150(record, probe[0], *(s32 *)(record + 12), probe[2]);
    Func_08009158(record);
    Func_080f9010(233);

    for (;;) {
        code = Func_080091b0(2, *(s32 *)(record + 8),
                             *(s32 *)(record + 16));
        if (code == 99) {
            break;
        }
        if (code == 98) {
            Func_020035ac(record);
        } else if (code == 97) {
            Func_02003668(record);
        } else if (code == 96) {
            Func_02003724(record);
        }
        Func_02003738(record);
        Func_080000c0(1);
    }

    Func_02003484(record);
    Func_0808a020();
}
