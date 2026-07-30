/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `tools/overlay_call_targets.ts`.  The band
 * 0x02002468-0x0200261f is an 8-byte-per-entry import veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <main-image address>`); calls landing there
 * are named by that trailing word, as the semantic resource_39f sources do.
 * Calls landing on an overlay prologue keep the overlay's `Func_0200xxxx` name.
 *
 * Link base: resource_3b4 is linked at 0x02008000.  The image is 0x2f84 bytes
 * and the table at 0x02002d40-0x02002f84 holds 59 words of the form
 * `<function offset> + 0x8000 + 1` (Thumb bit), e.g. 0x02008ec9 = Func_02000ec8.
 * So a pool word in 0x02008000-0x0200af83 is in-image data at value - 0x8000.
 * `Data_02000240` lies below the base and is referenced by byte-exact sources
 * in many other overlays, so it is a shared EWRAM global, not overlay data.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * is returned and the owner is void.
 *
 * Declarations are old-style because imports here are reached with differing
 * argument counts from different call sites.
 */
#include "types.h"

/*
 * resource_3b4 owner at 0x02000474, 404 bytes: code 0x02000474-0x020005f9,
 * two bytes of alignment, and the pool words 0x0200a69c, 0x0200a644 and
 * 0xffff0000 at 0x020005fc.
 *
 * "How far can this object slide?"  The caller hands in a request record; the
 * owner resolves which object it names, walks the destination cell outwards
 * along the object's facing one tile at a time, and stops at the first step
 * whose footprint is blocked.  It returns 1 with the request's position words
 * rewritten to the last clear cell, or 0 when the object is unknown or cannot
 * move at all.
 *
 * Two in-image tables are used, both under this overlay's 0x02008000 link
 * base.  `Data_0200a644` (file offset 0x2644) is the same sixteen-word step
 * table 0x020000c4 and 0x020002a8 use, packing an X delta in the high halfword
 * and a Z delta in the low one.  `Data_0200a69c` (file offset 0x269c) is a
 * sixteen-byte-per-kind footprint table: words 0 and 2 bound the object in X
 * and words 1 and 3 in Z, and the sweep extents are the sums of their
 * magnitudes shifted right by four.  The assembly takes those magnitudes with
 * `cmp #0 / bge / negs`, so they are signed values whose sign is discarded.
 *
 * The footprint sweep writes each candidate cell into the request's own words
 * at +8, +12 and +16 and probes it with Func_080091d8; a result of exactly 2
 * ends the search.  One tile is 0x100000 in these 16.16 words (`movs #128 /
 * lsls #13`), which is the same unit the `/ 0x100000` tile divisions elsewhere
 * in this overlay use.
 *
 * The outer scan has no bound of its own -- it advances the probe and repeats
 * until the terrain probe answers 2 -- and the footprint entry is re-read from
 * `request[0]` on every pass even though nothing changes it.  Both are as the
 * assembly has them.
 *
 * The byte at +34 of the resolved object is set to 2 for the duration of the
 * search and cleared when it ends; 0x020000c4 writes the same byte.
 *
 * r4 is saved to [sp,#0] and reloaded around the Func_080091d8 call because
 * this project's ABI treats r4 as call-clobbered.
 *
 * The prologue saves r8, r9, sl and fp by hand and the epilogue restores them
 * before `pop {r5, r6, r7} / pop {r1} / bx r1`: the return address lands in r1,
 * so r0 survives and is the result.
 */

/* Sixteen packed step words, in-image at file offset 0x2644. */
extern u32 Data_0200a644[];
/* Four words of footprint bounds per kind, in-image at file offset 0x269c. */
extern s32 Data_0200a69c[];

/* Overlay-local at 0x0200034c: resolves the request, writing the facing index
 * through its first argument and returning the object record, or 0. */
s32 *Func_0200034c();
/* Terrain probe; a result of 2 means the tested cell is blocked. */
s32 Func_080091d8();

s32 Func_02000474(s32 *request)
{
    s32 facing;
    s32 probe[3];
    s32 *target;
    u8 *busy;
    s32 *bounds;
    s32 spanX;
    s32 spanZ;
    s32 steps = 0;
    u32 step;
    s32 low;
    s32 high;
    s32 row;
    s32 column;

    request[5] = 0;   /* +20 */

    target = Func_0200034c(&facing, request + 1, request);
    if (target == 0) return 0;

    busy = (u8 *)target + 34;
    *busy = 2;

    bounds = &Data_0200a69c[request[0] * 4];

    low = bounds[1];
    if (low < 0) low = -low;
    high = bounds[3];
    if (high < 0) high = -high;
    spanZ = (low + high) >> 4;

    low = bounds[0];
    if (low < 0) low = -low;
    high = bounds[2];
    if (high < 0) high = -high;
    spanX = (low + high) >> 4;

    step = Data_0200a644[facing];
    probe[0] = target[2] + (s32)(step & 0xffff0000);   /* +8  */
    probe[1] = target[3];                              /* +12 */
    probe[2] = target[4] + (s32)(step << 16);          /* +16 */

    request[3] = target[3];

    for (;;) {
        bounds = &Data_0200a69c[request[0] * 4];

        request[4] = probe[2] + (bounds[1] << 16);
        for (row = 0; row < spanZ; row++) {
            request[2] = probe[0] + (bounds[0] << 16);
            for (column = 0; column < spanX; column++) {
                if (Func_080091d8(target, request + 2) == 2) goto blocked;
                request[2] += 0x100000;
            }
            request[4] += 0x100000;
        }

        steps++;
        step = Data_0200a644[facing];
        probe[0] += (s32)(step & 0xffff0000);
        probe[2] += (s32)(step << 16);
    }

blocked:
    *busy = 0;
    if (steps == 0) return 0;

    step = Data_0200a644[facing];
    request[2] = target[2] + (s32)(step & 0xffff0000) * steps;
    request[3] = target[3];
    request[4] = target[4] + (s32)(step << 16) * steps;
    return 1;
}
