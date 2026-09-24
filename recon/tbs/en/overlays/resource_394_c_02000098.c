/* NONMATCHING: 204 of 184 bytes, 69 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: tile-map block copy; loop.c hoists all six address constants out of the inner loop where the reference keeps only 0xfff, 15 and 0x06002800 in registers and reloads the others each cell; the row base goes to the stack in both. Twin of resource_3b3:02000cc0 (69 edits). */
#include "TYPES.H"

void Local_02000098(s32 x, s32 y, s32 width, s32 height, s32 bank, s32 dest_x, s32 dest_y)
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
            ((u32 *)0x06002800)[base] = ((u32 *)0x02020000)[cell * 2];
            ((u32 *)0x06002840)[base] = ((u32 *)0x02020004)[cell * 2];
        }
        src += 128 - width;
    }
}
