/* Astra byte-stride ownership (2026-09-27): explicit stride=(128-width)*4
 * and a byte-pointer row advance give 184/184 bytes, 46 halfwords / 43
 * aligned edits. The stride is still spilled at sp+4, row end still r8;
 * only pretest scheduling changes. Full diff preserves the six pools but
 * falsifies the predicted ownership exchange. Reject this one trial and
 * retain the canonical implicit stride; do not propagate to the twin.
 * NONMATCHING kuupuappu H3 (2026-09-27): one-word bounds record did
 * not establish the predicted stack ownership: GCC promotes its row limit
 * to r8. Complete 184/184 extent and all six pool words/offsets agree;
 * 42 differing halfwords / 39 aligned edits remain. This is not an
 * admitted structural improvement. Full normalized diff and identical
 * normal/diagnostic text checked. Retain the failed experiment and close
 * the row-bound allocation axis; no exact-byte credit.
 * NONMATCHING kuupuappu H2 (2026-09-27): reuse source-x, destination-y
 * and height parameters as column, row and row-end. Complete 184/184 extent
 * and all six pool offsets/words now agree, with 44 halfwords / 38 edits.
 * The width/end-row/stride allocation remains wrong: height in r8 and
 * stride at sp+4. Full normalized diff and normal/diagnostic text agree.
 * Retain this complete-extent witness; no exact-byte credit. H1 is in
 * 3a161950f. A final test must establish row-end stack ownership explicitly,
 * preserving the per-cell reloads and per-row column bound.
 * NONMATCHING kuupuappu H1 (2026-09-27): 188/184 bytes,
 * 82 differing halfwords / 43 aligned edits. In-loop column bound now
 * recomputes per row; loop-owned first store retains the destination in lr
 * while both tile tables and second destination reload per cell. These
 * predicted lifetime facts hold. Width/end-row/stride roles still differ:
 * end-row lives in r8 and stride at sp+4, opposite the reference allocation.
 * Complete normalized diff and normal/diagnostic text read; no credit.
 * Keep this structural witness before the width-lifetime causal follow-up.
 * Previous NONMATCHING H1: 192/184 bytes, 84 halfwords / 60 aligned edits.
 * H2 (2026-09-27): pass the first destination into CopyCell from a
 * row-local caller pointer, as the exact main-image row renderer does.
 * Prediction: retain that base across cells while reloading both tile
 * tables and the second destination. Full 192-byte output is identical
 * to H1 (cmp checked), including its frame, loops and six pool words.
 * A constant caller argument supplies no new lifetime after inlining.
 * Reject this boundary transfer; keep H1 and stop the helper-argument axis.
 * 2026-09-27: transfer the exact main-image
 * COMMON/MAP/RENDER_METATILE_ROW.C per-cell pointer helper boundary.
 * Whole ROM owner 02000098..02000150 is 184 bytes including six pool words.
 * ROM keeps the mask, wrap mask and first destination base in registers,
 * but reloads both tile-table bases and the second destination per cell.
 * Baseline reproduced at 204/184 bytes, 93 halfwords / 69 aligned edits,
 * frame 20 versus reference 8. Previous array/byte-address spellings did
 * not give these pointers a per-cell helper lifetime. Predicted local table
 * reloads without changing rectangle bounds, indexing or copied values.
 * Result: complete normalized diff is topology equal; frame shrinks from
 * 20 to the reference 8. Both table bases and the second destination now
 * reload per cell, as in ROM. The first destination also reloads, unlike
 * ROM's lr base retained across the inner loop. The outer column bound is
 * still hoisted across rows rather than recomputed; row end remains in sl
 * instead of sp+4. Six pool words remain, with differing placement/order.
 * Retain this improved source model, not a match. The exact row renderer
 * passes its destination base into the helper; that caller/helper ownership
 * boundary is the next untested fact, not another address spelling. Do not
 * sweep constants or declaration order. Exact main-image source remains
 * untouched. No new DONE bytes and no partial-owner credit.
 *
 * Previous NONMATCHING: 204 of 184 bytes, 69 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: tile-map block copy; loop.c hoists all
 * six address constants out of the inner loop where the reference keeps only
 * 0xfff, 15 and 0x06002800 in registers and reloads the others each cell;
 * the row base goes to the stack in both. Writing the column bound as
 * col < dest_x + width in the for gives the reference's per-row bound
 * (196 bytes) but still hoists five constants whatever the address
 * spelling (arrays, byte sums, [base + 16]). Twin of resource_3b3:02000cc0
 * (69 edits).
 * Astra 2026-09-27: forcing the row bound to volatile memory does not
 * reproduce an allocator spill. The one-word array gives 196/184 bytes,
 * 89 halfwords / 53 edits; a direct scalar gives 192/184, 84/49. Both
 * retain its address across the loop and grow the frame from 8 to 12.
 * Reject explicit-memory forcing; keep the 184-byte nonvolatile body. */
#include "TYPES.H"

/* FAKEMATCH: the exact row renderer's helper scope gives each tile-table
 * pointer its own lifetime; this rectangle uses word-sized cell coordinates. */
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

void Func_02000098(s32 x, s32 y, s32 width, s32 height, s32 bank, s32 dest_x, s32 dest_y)
{
    u32 *src;
    s32 bounds[1];
    s32 base, cell;

    src = (u32 *)0x02010000 + (y * 128 + x);
    /* FAKEMATCH: the row limit is owned by a one-word local bounds record. */
    bounds[0] = dest_y + height;
    for (; dest_y < bounds[0]; dest_y++) {
        for (x = dest_x; x < dest_x + width; x++) {
            cell = *src++ & 0xfff;
            base = ((dest_y & 15) + bank * 16) * 32 + (x & 15);
            ((u32 *)0x06002800)[base] = ReadFirstTile(cell);
            CopySecondTile(cell, base);
        }
        src += 128 - width;
    }
}
