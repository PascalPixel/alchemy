/* NONMATCHING: H0 196/184 bytes, 84 differing halfwords / 71 aligned edits (2026-09-27).
 * Complete own-ROM owner 02000cc0..02000d78, including six pool words.
 * Baseline 192/184 bytes, 84 differing halfwords (2026-09-24).
 * Transfers the exact main Map_RenderMetatileRow per-cell helper scope.
 * Separate upper/lower extern views follow the six own-ROM base/mask pools.
 * Prediction: eight-byte frame with only row base and bottom spilled.
 * Negative result: topology equal, but extern tables hoist into saved
 * registers and grow the frame to 20 bytes instead of 8. No adoption.
 * Stop this transfer; restore the stronger 192-byte baseline after recording
 * this witness. Do not sweep declarations to compensate for the hoisting. */
#include "TYPES.H"

extern u32 gPillarMapCells[];
extern u32 gPillarUpperTiles[][2];
extern u32 gPillarLowerTiles[][2];
extern u32 gPillarUpperScreen[];
extern u32 gPillarLowerScreen[];

/* FAKEMATCH: per-cell table lifetime follows the exact main row renderer. */
static __inline__ void CopyCell(u32 *src, s32 base, s32 col)
{
    u32 cell = *src & 0xfff;
    s32 pos = base + (col & 15);

    gPillarUpperScreen[pos] = gPillarUpperTiles[cell][0];
    gPillarLowerScreen[pos] = gPillarLowerTiles[cell][0];
}

void TakaraHashira_CopyCellBlock(s32 x, s32 y, s32 width, s32 height,
                                s32 block, s32 left, s32 top)
{
    u32 *src;
    s32 bottom;
    s32 col;
    s32 base;

    src = gPillarMapCells + (y << 7) + x;
    bottom = top + height;
    for (; top < bottom; top++) {
        for (col = left; col < left + width; col++) {
            base = ((top & 15) + (block << 4)) << 5;
            CopyCell(src++, base, col);
        }
        src += 128 - width;
    }
}
