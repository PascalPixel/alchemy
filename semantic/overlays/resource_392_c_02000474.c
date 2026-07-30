typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_392 owner at 0x02000474, 404 bytes: the slide raycast.
 * Code 0x02000474-0x020005f9, an alignment halfword at 0x020005fa and the three
 * pool words 0x02008f20, 0x02008ec8 and 0xffff0000 at 0x020005fc-0x02000607.
 *
 * TRANSPOSED from semantic/overlays/resource_39f_c_02000474.c.  The two owners
 * are the same routine shared verbatim: over all 202 halfwords they differ in
 * only 4 places - the two halves of the single BL pair, and the low halves of
 * the two pool words naming the overlay's own data tables.  Nothing structural
 * differs, so the reconstruction below is the 39f one with the constants
 * substituted.
 *
 * What was changed:
 *  - Data_0200af50 (footprint boxes) becomes Data_02008f20, file offset 0x0f20;
 *  - Data_0200aef8 (packed direction words) becomes Data_02008ec8, offset 0x0ec8;
 *    both under the proven 0x02008000 overlay link base.  The direction table
 *    address is independently corroborated: resource_392's 0x020000c4 owner
 *    loads the same 0x02008ec8 for the same purpose.
 *  - the calls were re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_392 0474': the same two sites, 0x0200034c (this overlay's own
 *    subject resolver, at the same in-image offset) and the veneer publishing
 *    Func_080091d8.  Both names are therefore unchanged.
 *
 * Given a six-word request record it asks Func_0200034c for the subject the
 * player is acting on, then marches that subject's footprint one tile at a time
 * along the request's heading, testing every cell of the footprint with
 * Func_080091d8, until a cell reports 2 (blocked).  The request record is then
 * filled with the last clear position and 1 is returned; if the very first step
 * is blocked, 0 is returned instead.
 *
 * The footprint boxes are 16-byte entries indexed by the request's kind word:
 * (minX, minZ, maxX, maxZ) in sixteenths of a tile.
 *
 * UNCERTAINTY (inherited): the outer march has no iteration bound.  Its only
 * exit is Func_080091d8 returning 2, reached by an unconditional 'b' back to the
 * loop head.  That is what the assembly says.
 *
 * The epilogue is 'add sp, #32 / pop {r3, r5, r6, r7} / ... / pop {r1} / bx r1'
 * with r1 carrying the return address, so r0 survives and is the result.
 */

extern s32 Data_02008ec8[];   /* 16 packed direction words */
extern s32 Data_02008f20[];   /* 16-byte footprint boxes, four words each */

/* Resolves the acted-on subject: writes the heading index through its first
 * argument and fills the request record through its third.  Returns the
 * subject record or 0. */
u8 *Func_0200034c();

/* Terrain probe.  A result of 2 means the tested cell is blocked. */
s32 Func_080091d8();

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

    box = &Data_02008f20[request[0] * 4];
    spanZ = ((box[1] < 0 ? -box[1] : box[1]) + (box[3] < 0 ? -box[3] : box[3])) >> 4;
    spanX = ((box[0] < 0 ? -box[0] : box[0]) + (box[2] < 0 ? -box[2] : box[2])) >> 4;

    step = Data_02008ec8[heading];
    probe[0] = *(s32 *)(subject + 8) + (step & (s32)0xffff0000);
    probe[1] = *(s32 *)(subject + 12);
    probe[2] = *(s32 *)(subject + 16) + (Data_02008ec8[heading] << 16);

    request[3] = probe[1];

    for (;;) {
        /* The footprint's minimum corner is re-read from the table on every
         * pass; the box words are whole units promoted to 16.16 by `lsls #16`. */
        request[4] = probe[2] + (Data_02008f20[request[0] * 4 + 1] << 16);

        for (z = 0; z < spanZ; z++) {
            request[2] = probe[0] + (Data_02008f20[request[0] * 4] << 16);

            for (x = 0; x < spanX; x++) {
                if (Func_080091d8(subject, &request[2]) == 2) {
                    *state = 0;
                    if (steps == 0) return 0;

                    /* The accumulated travel is the per-step direction times
                     * the number of clear steps, applied to the subject's own
                     * position rather than to the probe. */
                    step = Data_02008ec8[heading];
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
        step = Data_02008ec8[heading];
        probe[0] += step & (s32)0xffff0000;
        probe[2] += Data_02008ec8[heading] << 16;
    }
}
