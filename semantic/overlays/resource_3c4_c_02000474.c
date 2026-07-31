typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3c4 owner at 0x02000474, 404 bytes: code 0x02000474-0x020005f9, an
 * alignment halfword at 0x020005fa and the three pool words 0x0200b248,
 * 0x0200b1f0 and 0xffff0000 at 0x020005fc-0x02000607.
 *
 * The slide raycast.  Given a six-word request record it asks Func_0200034c
 * for the subject the player is acting on, then marches that subject's
 * footprint one tile at a time along the request's heading, testing every cell
 * of the footprint with Func_080091d8, until a cell reports 2 (blocked).  The
 * request record is then filled with the last clear position and 1 is
 * returned; if the very first step is blocked, 0 is returned instead.
 *
 * SHARED ROUTINE.  Halfword-identical to resource_39f's 0x02000474 over all
 * 202 halfwords except one `bl` displacement (different veneer-table offset)
 * and two data pool words, and `tools/overlay_call_targets.ts` reports the same
 * resolved profile for both: 0x0200034c plus veneer -> Func_080091d8.  This
 * overlay's 0x0200034c is itself halfword-identical to resource_39f's apart
 * from one veneer displacement and its three data pool words.
 *
 * Link base 0x02008000.  Both tables were read out of this overlay's image and
 * are word-for-word the same as resource_39f's:
 *  - Data_0200b1f0 (file offset 0x31f0) is the packed direction word table
 *    also used by 0x020000c4 and 0x020002a8: X step in the high halfword,
 *    Z step in the low halfword.
 *  - Data_0200b248 (file offset 0x3248) is a table of 16-byte footprint boxes
 *    indexed by the request's kind word: (minX, minZ, maxX, maxZ) in sixteenths
 *    of a tile.  The first entries read (-32,-8,32,8), (-8,-32,8,32), ... so
 *    the sweep counts below are 4x1, 1x4 and so on.
 *
 * UNCERTAINTY: the outer march has no iteration bound.  Its only exit is
 * Func_080091d8 returning 2, reached by an unconditional `b` back to the loop
 * head.  That is what the assembly says, and it is consistent with a map that
 * is always enclosed, but nothing in this routine guarantees termination.
 *
 * The epilogue is `add sp, #32 / pop {r3, r5, r6, r7} / ... / pop {r1} / bx r1`
 * with r1 carrying the return address, so r0 survives and is the result.
 */

extern s32 Data_0200b1f0[];   /* 16 packed direction words */
extern s32 Data_0200b248[];   /* 16-byte footprint boxes, four words each */

/* Resolves the acted-on subject: writes the heading index through its first
 * argument and fills the request record through its third.  Returns the
 * subject record or 0. */
u8 *Func_0200034c();
s32 Func_080091d8();

/* Terrain probe.  A result of 2 means the tested cell is blocked. */

s32 Func_02000474(s32 *request)
{
    u8 *subject;
    u8 *state;
    s32 heading;
    s32 probe[3];
    s32 step;
    s32 spanX;
    s32 spanZ;
    s32 steps;
    s32 x;
    s32 z;
    const s32 *box;

    request[5] = 0;

    subject = Func_0200034c(&heading, &request[1], request, 0);
    if (subject == 0) return 0;

    /* The subject is marked "in transit" for the duration of the sweep and
     * cleared again on the way out. */
    state = subject + 34;
    *state = 2;

    steps = 0;

    box = &Data_0200b248[request[0] * 4];
    spanZ = ((box[1] < 0 ? -box[1] : box[1]) + (box[3] < 0 ? -box[3] : box[3])) >> 4;
    spanX = ((box[0] < 0 ? -box[0] : box[0]) + (box[2] < 0 ? -box[2] : box[2])) >> 4;

    step = Data_0200b1f0[heading];
    probe[0] = *(s32 *)(subject + 8) + (step & (s32)0xffff0000);
    probe[1] = *(s32 *)(subject + 12);
    probe[2] = *(s32 *)(subject + 16) + (Data_0200b1f0[heading] << 16);

    request[3] = probe[1];

    for (;;) {
        /* The footprint's minimum corner is re-read from the table on every
         * pass; the box words are whole units promoted to 16.16 by `lsls #16`. */
        request[4] = probe[2] + (Data_0200b248[request[0] * 4 + 1] << 16);

        for (z = 0; z < spanZ; z++) {
            request[2] = probe[0] + (Data_0200b248[request[0] * 4] << 16);

            for (x = 0; x < spanX; x++) {
                if (Func_080091d8(subject, &request[2]) == 2) {
                    *state = 0;
                    if (steps == 0) return 0;

                    /* The accumulated travel is the per-step direction times
                     * the number of clear steps, applied to the subject's own
                     * position rather than to the probe. */
                    step = Data_0200b1f0[heading];
                    request[2] = *(s32 *)(subject + 8) + steps * (step & (s32)0xffff0000);
                    request[3] = *(s32 *)(subject + 12);
                    request[4] = *(s32 *)(subject + 16) + steps * (step << 16);
                    return 1;
                }
                request[2] += 0x100000;     /* one tile, 128 << 13 */
            }

            request[4] += 0x100000;
        }

        steps++;
        step = Data_0200b1f0[heading];
        probe[0] += step & (s32)0xffff0000;
        probe[2] += Data_0200b1f0[heading] << 16;
    }
}
