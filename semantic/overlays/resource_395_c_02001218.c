typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 395 effect spawner at 0x02001218 (156 bytes,
 * 0x02001218-0x020012b3).
 *
 * Complete owner: `push {r5, r6, lr}` prologue at 0x02001218 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x0200129e.  The
 * popped return address lands in r0, so nothing is returned and the owner is
 * `void`.  Three of the body's exits are `b`/`beq` to that one epilogue.
 * 0x020012a4-0x020012b3 is this owner's four-word literal pool, after the
 * epilogue and never reached as code.
 *
 * All five call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), never from overlay_show.ts's
 * `bl` annotations, which are off by one veneer entry on this overlay.  Placed
 * multiset, matching the five sites exactly: Func_080f9010, Func_080090c8,
 * Func_08009080, Func_08009150, Func_08009098, one each.  The inventory
 * reports calls=5.
 *
 * Pool words, under the proven 0x02008000 overlay link base:
 *   0x03001e40 — RAM, below the image: the shared frame/mode word.
 *   0x02009dd0 — offset 0x1dd0, which is PAST the end of the overlay image
 *                (0x1dbc).  It is working RAM the loader leaves after the
 *                loaded overlay, not image data; its neighbour 0x02009dd4 is
 *                the step counter Func_020012b4 advances.
 *   0x00001999 — a plain constant stored to the record's +0x18 word.
 *   0x02009d9c — IN-IMAGE data at file offset 0x1d9c (inside the image, even,
 *                so data and not a Thumb entry), passed as the second argument
 *                of Func_08009098; that is the established shape of a motion
 *                script.
 * The base itself is proven by eleven pool words equal to this overlay's own
 * prologues plus the Thumb bit, 0x02009219 (this owner) among them.
 *
 * Behaviour: run only when the low two bits of the 0x03001e40 word are clear;
 * optionally play cue 200; create the effect record; then initialise its flag
 * bytes, its +0x18 word and its two 0x40000 scale words, select presentation
 * mode 2, place it, and start its scripted motion.
 *
 * Uncertainties:
 *  - r6 is the masked value from 0x03001e40 and is known to be zero on every
 *    path that reaches the stores; the compiler reuses it as the zero operand
 *    of the three `strb`s.  Written as literal 0 below.
 *  - the record at +0x50 is a second, linked record.  Only its bytes at +9 and
 *    +0x26 are touched here, so no layout beyond that is asserted.
 *  - the bit edit at +9 is `(b & ~0x0c) | 0x04`: `movs r3,#13 / negs r3,r3`
 *    builds 0xfffffff3, i.e. ~0x0c, not ~0x0d.
 */

void Func_080f9010();           /* play a sound cue */
u8 *Func_080090c8();            /* create an effect record, or 0 */
void Func_08009080();           /* select presentation mode (record, mode) */
void Func_08009150();           /* place the record at (x, y, z) */
void Func_08009098();           /* start a scripted motion (record, script) */

void Func_02001218(void)
{
    u8 *record;
    u8 *linked;

    if ((*(u32 *)0x03001e40 & 3) != 0) return;

    if (*(s32 *)0x02009dd0 != 0) {
        Func_080f9010(200);
    }

    /* 163 << 17 = 0x01460000, 128 << 14 = 0x00200000, 192 << 16 = 0x00c00000. */
    record = Func_080090c8(26, 0x01460000, 0x00200000, 0x00c00000);
    if (record == 0) return;

    linked = *(u8 **)(record + 0x50);
    linked[0x26] = 0;
    record[0x23] &= 0xfe;
    linked[9] = (u8)((linked[9] & ~0x0c) | 0x04);

    *(s32 *)(record + 0x18) = 0x1999;
    *(s32 *)(record + 0x30) = 0x40000;      /* 128 << 11 */
    *(s32 *)(record + 0x34) = 0x40000;
    record[0x55] = 0;

    Func_08009080(record, 2);
    Func_08009150(record, 0x01460000, 0, 0x00f00000);   /* 240 << 16 */
    Func_08009098(record, (void *)0x02009d9c);
}
