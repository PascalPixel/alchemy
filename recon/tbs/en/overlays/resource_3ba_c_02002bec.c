/* NONMATCHING: Korosseo_UpdateMarker, resource_3ba at 0x0200abec (316
 * bytes with both literal pools); twins resource_3bb:0x0200ae84 and
 * resource_3bc:0x0200b91c. Interpolates and blinks the Colosso marker
 * through the OAM queue.
 *
 * 2026-09-29 (round 3): 316/316 bytes, 10 differing halfwords, all in the
 * first (y) interpolation; the head, the second interpolation, both pools,
 * the OAM word and the tail are exact. Remaining: the reference keeps the
 * y start pointer in r1 and reads its unsigned view after the subtraction
 * (end in r2, diff tied to it), where this source reads the unsigned view
 * first with the pointer in r2; every spelling of the first channel tried
 * (pointer variables in any order, the sum on either side, a separate
 * difference) compiles to the same allocation. What moved it here from
 * 126 halfwords:
 * - the zero stores are plain halfword stores, which agscc loads from a
 *   halfword pool constant (short range) and which place the pool before
 *   the tail as the reference does; no link symbol stands in for 0;
 * - the OAM record pointer is read first, then the tile, then the steps;
 * - each channel is one expression, start + (end - start) * step / total;
 * - the size bit sits in a local, so the priority is or'ed in after it.
 * Earlier attempts (H1..H3, Astra, Sol) are in this file's history. */
#include "TYPES.H"

void Engine_OamSubmitRecord(s32 *record, s32 priority);

struct OamTile {
    u16 attr;
    u16 tile;
};

union MarkerCoordinate {
    s16 signed_value;
    u16 value;
};

extern struct OamTile gOamTiles[];

extern s16 Korosseo_MarkerIndex;
extern s16 Korosseo_MarkerSteps;
extern s16 Korosseo_MarkerStep;
extern s16 Korosseo_MarkerY;
extern union MarkerCoordinate Korosseo_MarkerStartY;
extern s16 Korosseo_MarkerEndY;
extern s16 Korosseo_MarkerX;
extern union MarkerCoordinate Korosseo_MarkerStartX;
extern s16 Korosseo_MarkerEndX;
extern s16 Korosseo_MarkerBlink;
extern s16 Korosseo_MarkerPriority;
extern s32 Korosseo_MarkerOam[3];

void Korosseo_UpdateMarker(void)
{
    s32 tile;
    s32 total;
    s32 step;
    s32 *oam;
    s16 *steps;
    s32 *dst;
    s32 x;
    s32 y;

    oam = Korosseo_MarkerOam;
    tile = gOamTiles[Korosseo_MarkerIndex].tile >> 5;
    steps = &Korosseo_MarkerSteps;
    total = *steps;
    if (total != 0) {
        step = ++Korosseo_MarkerStep;
        Korosseo_MarkerY = Korosseo_MarkerStartY.value
            + (Korosseo_MarkerEndY - Korosseo_MarkerStartY.signed_value) * step / total;
        Korosseo_MarkerX = Korosseo_MarkerStartX.value
            + (Korosseo_MarkerEndX - Korosseo_MarkerStartX.signed_value) * step / total;
        if (step >= total)
            *steps = 0;
        Korosseo_MarkerBlink = 0;
    }
    if (++Korosseo_MarkerBlink <= 13) {
        s32 size = 0x40000000;

        y = Korosseo_MarkerY;
        x = Korosseo_MarkerX;
        dst = oam;
        *dst++ = 0;
        *dst++ = (x - 8) | ((y - 8) << 16) | size | (Korosseo_MarkerPriority << 28);
        *dst = tile | 0x400;
        Engine_OamSubmitRecord(oam, 255);
    } else if (Korosseo_MarkerBlink > 19) {
        *(u16 *)&Korosseo_MarkerBlink = 0;
    }
}
