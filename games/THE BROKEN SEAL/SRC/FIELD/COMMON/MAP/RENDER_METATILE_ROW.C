/* Exact candidate: whole owner [0800fec8, 0800ff54), 140 bytes, four own pool
   words. Baseline 2026-09-26: 144 bytes, 64 differing halfwords, 59
   aligned edits. No callees. Raw caller 08010000 selects this row update
   on a vertical scroll boundary and ff54 on a horizontal boundary.
   H1: use the adopted ff54 family's typed per-cell helper and explicit
   outer row/base lifetimes, with paired word stores for this row owner.
   Prediction: only r8/sl saved, base in ip, all four pools reloaded where
   the reference owns them. Accept only exact 140 bytes plus landing gates.
   H1 result: exact 140/140 bytes, zero differing halfwords/aligned edits,
   topology equal; complete normalized comparison read. The same-source
   family evidence is the adopted ff54 implementation, not another project.
   The ff54 owner is unchanged. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* FAKEMATCH: helper scope gives each cell its own table-pointer lifetime,
   following the independently matched neighbouring column renderer. */
static __inline__ void CopyCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    u32 *tiles;
    u8 *dest;
    u32 index = (*map << 20) >> 19;

    tiles = (u32 *)Ram_MapBlocks;
    tiles += index;
    dest = base + (rowmod + colmod) * 2;
    *(u32 *)dest = *tiles;
    tiles = (u32 *)(Ram_MapBlocks + 4);
    tiles += index;
    *(u32 *)(dest + 64) = *tiles;
}

void Map_RenderMetatileRow(u32 a0, s32 a1, s32 a2)
{
    u8 *dest = (u8 *)(0x06002800 + (a0 << 11));
    u32 row = ((a2 / 2) & 0x7F) << 7;
    u32 rowmod = (a2 & 30) << 5;
    u32 col = (a1 / 2) & 0x7F;
    u32 colmod = a1 & 30;
    u32 counter;

    for (counter = 0; counter <= 15; counter++) {
        u32 *map = (u32 *)Ram_MapCellBuffer;

        map += row + col;
        CopyCell(map, dest, rowmod, colmod);
        col = (col + 1) & 0x7F;
        colmod = (colmod + 2) & 30;
    }
}
