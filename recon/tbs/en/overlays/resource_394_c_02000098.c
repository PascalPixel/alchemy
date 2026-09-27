/* NONMATCHING H1: 192/184 bytes, 84 halfwords / 60 aligned edits.
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
 * (69 edits). */
#include "TYPES.H"

/* FAKEMATCH: the exact row renderer's helper scope gives each tile-table
 * pointer its own lifetime; this rectangle uses word-sized cell coordinates. */
static __inline__ void CopyCell(u32 cell, s32 base)
{
    u32 *tiles;
    u32 *dest;

    tiles = (u32 *)0x02020000;
    tiles += cell * 2;
    dest = (u32 *)0x06002800;
    dest += base;
    *dest = *tiles;
    tiles = (u32 *)0x02020004;
    tiles += cell * 2;
    dest = (u32 *)0x06002840;
    dest += base;
    *dest = *tiles;
}

void Func_02000098(s32 x, s32 y, s32 width, s32 height, s32 bank, s32 dest_x, s32 dest_y)
{
    u32 *src;
    s32 row, col, end_row, end_col, base, cell;

    src = (u32 *)0x02010000 + (y * 128 + x);
    end_row = dest_y + height;
    for (row = dest_y; row < end_row; row++) {
        end_col = dest_x + width;
        for (col = dest_x; col < end_col; col++) {
            cell = *src++ & 0xfff;
            base = ((row & 15) + bank * 16) * 32 + (col & 15);
            CopyCell(cell, base);
        }
        src += 128 - width;
    }
}
