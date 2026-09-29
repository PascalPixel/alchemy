/* Not-yet-C, main:08011f54, complete 132-byte function and pool.
 * 2026-09-28: 132 of 132 bytes, 10 differing halfwords (12 while the
 * function table has no label; it is the 16 words at 080134fc, the tail of
 * the unidentified Curve_SampleIndexTable block, and needs one there).
 * Computing the layer offset in its own local keeps the reference's
 * reg+reg cell-pointer load, and plain integer addresses give the two
 * separate 0202c000/0202c001 pool words. Remaining: the reference keeps
 * metatile*4 in r1 and loads 0202c001 into r7 (so it also saves r7); here
 * metatile*4 takes r0 and the second address reuses r1. Retyping metatile
 * (u8, s32), scaling it once, and reordering the operands do not move it.
 * Callers (0800cacc, 0800f7f4) call it as Func_08011f54.
 * 2026-09-29 (alchemy permute scorer): 100 = 8 register-only rows plus
 * the push/pop of r7 and the unlabelled table. The literal addresses keep
 * it a draft, and naming them does not work yet: with gMapCellBuffer and
 * gMapCollision (both defined at these addresses) GCC's CSE relates
 * gMapCollision+1 to gMapCollision, so the second pool word becomes an
 * add of 1 and the shape load a reg+reg ldrb; six symbol spellings (array
 * index, pointer arithmetic, u32 casts, scaling once) all score 500
 * (19 register-only, 1 inserted, 2 deleted). The ROM's code reads as if
 * the two addresses were unrelated integer constants.
 */
#include "TYPES.H"

typedef s32 (*TerrainHeightFn)(u8 *cell, s32 x, s32 y);

extern u8 *gMapWork;
extern TerrainHeightFn Map_TerrainHeightFunctions[16];

/* Height of the terrain at a 16.16 map position on one of the layers: the
   metatile's shape selects the height function, which gets the metatile's
   height bytes and the position inside it. */
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y)
{
    u8 *work = gMapWork;
    u8 *cells;
    u32 metatile;

    x >>= 16;
    y >>= 16;
    cells = (u8 *)0x02010000;
    if (work != NULL) {
        s32 offset = (layer & 3) * 48 + 304;

        cells = *(u8 **)(work + offset);
    }
    cells += (x / 16 + (y / 16 << 7)) * 4;
    metatile = cells[3];
    return Map_TerrainHeightFunctions[*(u8 *)(0x0202c000 + metatile * 4) & 15](
        (u8 *)(0x0202c001 + metatile * 4), x & 15, y & 15);
}
