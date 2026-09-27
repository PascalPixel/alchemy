/* NONMATCHING: 184/184 bytes, 42 halfwords / 39 aligned edits (2026-09-27).
 * Pillars H1 transfers the own-ROM equivalent 394:02000098
 * draft's per-cell ReadFirstTile/CopySecondTile consumer model. Predict
 * complete 184-byte extent, frame8, all six pool offsets/words and tile
 * table reloads per cell; retain only with the complete normalized diff.
 * One transfer trial, no row-limit/declaration/constant spelling sweep.
 * Full normalized diff read: predicted frame, per-cell table reloads and
 * complete extent/pool all hold. Retain this stronger consumer model.
 * Remaining row bound lives in r8 rather than sp+4; source/top r4/r5
 * rather than r5/r4; width/mask/bank/stride high-register roles disagree.
 * The sibling's bounds-record and parameter-reuse trials already closed
 * that allocation axis. Do not repeat them; no adoption or alignment credit.
 * Historical restored 192/184 bytes, 84 halfwords / 56 edits (2026-09-27).
 * Rejected typed-table/helper transfer is committed in 0b4bfe722:
 * 196/184 bytes, 84 differing halfwords / 71 aligned edits, frame 20 vs 8.
 * Named strided arrays hoisted table bases into saved registers. The older
 * historical direct-base model was stronger than that transfer:
 * frame 12 versus reference 8, compared with the rejected trial's 20.
 * STOP the named-array transfer; no declaration or register spelling sweep.
 * Original baseline: 192 of 184 bytes, 84 differing halfwords (2026-09-24).
 * Hand-written from the disassembly: copies a width x height block of the
 * 128-wide cell map at 0x02010000 into the BG screen block at 0x06002800 (two
 * words per cell from the table at 0x02020000). The loop shape matches;
 * register allocation does not: the reference keeps top in r4, src in r5,
 * width in sl, block << 4 in r9 and 128 - width in fp and spills only bottom
 * and the row base, here two more values spill. */
#include "TYPES.H"

/* FAKEMATCH: the exact row renderer's helper scope gives each tile-table
 * pointer its own lifetime; this rectangle uses word-sized cells. */
static __inline__ u32 ReadFirstTile(u32 cell)
{
    u32 *tiles = (u32 *)0x02020000;

    return tiles[cell * 2];
}

static __inline__ void CopySecondTile(u32 cell, s32 base)
{
    u32 *tiles;
    u32 *dest;

    tiles = (u32 *)0x02020004;
    tiles += cell * 2;
    dest = (u32 *)0x06002840;
    dest += base;
    *dest = *tiles;
}

void TakaraHashira_CopyCellBlock(s32 x, s32 y, s32 width, s32 height, s32 block, s32 left, s32 top)
{
    u32 *src;
    s32 bounds[1];
    s32 base;
    s32 cell;

    src = (u32 *)0x02010000 + (y * 128 + x);
    /* FAKEMATCH: retain the sibling's one-word local bounds record. */
    bounds[0] = top + height;
    for (; top < bounds[0]; top++) {
        for (x = left; x < left + width; x++) {
            cell = *src++ & 0xfff;
            base = ((top & 15) + block * 16) * 32 + (x & 15);
            ((u32 *)0x06002800)[base] = ReadFirstTile(cell);
            CopySecondTile(cell, base);
        }
        src += 128 - width;
    }
}
