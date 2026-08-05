#include "types.h"

/*
 * Overlay resource_3b3 owner at 0x02000cc0, 184 bytes
 * (0x02000cc0-0x02000d78): item-28 unindexed-function population,
 * 6 calls. Full body traced in this reconstruction; previously parked only for
 * span-scoping (a prior working note), including an
 * open question about the exact stack-argument offsets that this
 * draft resolves against the live disassembly.
 *
 * A nested row/column tile-attribute blitter: streams 12-bit tile
 * indices sequentially from a fixed 128-column-wide EWRAM source
 * buffer (Data_02010000), looks each one up in a two-word-per-entry
 * attribute table (Data_02020000, stride 8 bytes), and writes the two
 * looked-up words into two parallel VRAM destination regions 0x40
 * bytes apart (Data_06002800 / Data_06002840). Source position
 * advances by exactly one full 128-word row per outer iteration
 * regardless of how many columns were actually written that row;
 * destination position is independently addressed by
 * (destRowBlock, destRow, destCol).
 *
 * Register-to-argument mapping confirmed against the live prologue:
 * r0-r3 are the first four arguments; the three stack arguments sit
 * at [sp+40]/[sp+44]/[sp+48] measured from the post-prologue sp (the
 * prologue pushes 8 registers = 32 bytes, then `sub sp, #8`, so the
 * caller's own [sp+0]/[sp+4]/[sp+8] land 40 bytes in -- confirmed
 * directly against the body's own `ldr rN,[sp,#40/44/48]` immediates).
 *
 * VRAM addressing and the attribute-table entry layout are not
 * established beyond the arithmetic reproduced here -- like
 * resource_3b3_c_02000ddc.c's bucket table, this is transcribed
 * faithfully from the disassembly rather than derived from a known
 * struct.
 */

extern u32 Data_02010000[]; /* source tile-index stream, 128 words/row */
extern u32 Data_02020000[]; /* attribute table, 2 words (8 bytes) per tile index */
extern u32 Data_06002800[]; /* VRAM destination A */
extern u32 Data_06002840[]; /* VRAM destination B, 0x40 bytes after A */

void Func_02000cc0(s32 srcCol, s32 srcRow, s32 colCount, s32 rowCount,
                    s32 destRowBlock, s32 destColStart, s32 destRowStart)
{
    s32 permuted_4;
    u32 *src = Data_02010000 + (srcRow * 128 + srcCol);
    s32 colSkip = 128 - colCount;
    s32 destRow = destRowStart;
    s32 destRowEnd = destRowStart + rowCount;

    while (destRow < destRowEnd) {
        s32 destCol = destColStart;
        s32 destColEnd = destColStart + colCount;
        s32 rowByteOff = ((destRow & 0xf) + (destRowBlock << 4)) << 5;

        while (destCol < destColEnd) {
            s32 idx = rowByteOff + (destCol & 0xf);
            u32 tileIndex = (*src++) & 0xfff;
            u32 *entry = Data_02020000 + tileIndex * 2;

            permuted_4 = entry[0];
            Data_06002840[idx] = entry[1];
            Data_06002800[idx]  = permuted_4;

            destCol++;
        }

        destRow++;
        src += colSkip;
    }
}
