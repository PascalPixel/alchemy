/* Not-yet-C: complete 500-byte camera-centering owner, split from 08010000.
 * First ordinary-C reconstruction from the complete listing. Layer scale
 * fields are scalar Q16 values, not pointers. Active layers advance the
 * layer cursor and redraw 11 rows, or 16 when vertical animation is active.
 * Candidate 508/500 bytes, 239 differing halfwords / 146 aligned edits;
 * branch topology and 32-byte frame match. Remaining: saved pointer/argument
 * roles, IWRAM multiply carriers, masks and nested row/column lifetimes.
 * CopyCell follows the independently matched neighbouring row renderer. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct CameraLayer {
    s32 x, y;
    s32 offset_x, offset_y;
    s32 scale_x, scale_y;
    s32 speed_x, speed_y;
    s32 phase_x, phase_y;
    u16 mask_x, mask_y;
    s32 unknown_2c;
};
struct CameraWork {
    u8 unknown_00[0xe4];
    s32 view_x, view_y;
    s32 min_x, min_y, max_x, max_y;
    s32 unknown_fc;
    u8 enabled[3];
    u8 unknown_103;
    struct CameraLayer layers[3];
};
extern struct CameraWork *Data_03001e70;

static __inline__ void CopyCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    u32 *tiles;
    u8 *dest;
    u32 index = (*map << 20) >> 19;

    tiles = (u32 *)0x02020000;
    tiles += index;
    dest = base + (rowmod + colmod) * 2;
    *(u32 *)dest = *tiles;
    tiles = (u32 *)0x02020004;
    tiles += index;
    *(u32 *)(dest + 64) = *tiles;
}

void Map_SetCameraCenter(s32 x, s32 y)
{
    struct CameraWork *work = Data_03001e70;
    struct CameraLayer *layer = work->layers;
    u32 no;

    x -= 0x780000;
    y -= 0x600000;
    if (x < work->min_x)
        x = work->min_x;
    if (x > work->max_x - 0xf00000)
        x = work->max_x - 0xf00000;
    if (y < work->min_y)
        y = work->min_y;
    if (y > work->max_y - 0xa00000)
        y = work->max_y - 0xa00000;
    work->view_x = x;
    work->view_y = y;

    for (no = 0; no < 3; no++) {
        if (work->enabled[no]) {
            s32 sx, sy;
            u32 rows = 22;
            u32 row, col, rowmod, colmod, i, j;
            s32 first_col;
            u8 *dest;

            sx = Iwram_MulQ16(work->view_x, layer->scale_x);
            sy = Iwram_MulQ16(work->view_y, layer->scale_y);
            if (layer->speed_x) {
                layer->phase_x += layer->speed_x;
                sx = (sx + layer->phase_x) & ((layer->mask_x << 19) | 0x7ffff);
            }
            if (layer->speed_y) {
                layer->phase_y += layer->speed_y;
                sy = (sy + layer->phase_y) & ((layer->mask_y << 19) | 0x7ffff);
                rows = 32;
            }
            sx += layer->offset_x;
            sy += layer->offset_y;
            layer++;
            x = sx / 0x80000;
            y = sy / 0x80000;
            dest = (u8 *)(0x06002800 + (no << 11));
            row = ((y / 2) & 127) << 7;
            rowmod = (y & 30) << 5;
            rows >>= 1;
            first_col = x / 2;
            for (i = 0; i < rows; i++) {
                col = first_col & 127;
                colmod = x & 30;
                for (j = 0; j <= 15; j++) {
                    u32 *map = (u32 *)0x02010000;
                    map += row + col;
                    CopyCell(map, dest, rowmod, colmod);
                    col = (col + 1) & 127;
                    colmod = (colmod + 2) & 30;
                }
                row = (row + 128) & 0x3f80;
                rowmod = (rowmod + 64) & 0x3c0;
            }
        }
    }
}
