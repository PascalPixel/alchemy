#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

struct UiWindowGeometry {
    u8 padding[12];
    u16 x;
    u16 y;
};

typedef char UiWindowGeometry_size[
    sizeof(struct UiWindowGeometry) == 0x10 ? 1 : -1
];
typedef char UiWindowGeometry_x_offset[
    (u32)&(((struct UiWindowGeometry *)0)->x) == 0x0c ? 1 : -1
];

void UiWindow_SetTileAttributeRect(const struct UiWindowGeometry *window,
    s32 x, s32 y, s32 width, s32 height, u32 field) {
    u8 *base = *(u8 **)ADDR_03001E8C;

    x += window->x + 1;
    y += window->y + 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *cell = (u16 *)((u32)x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *cell;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *cell = value;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        base[RENDER_DIRTY_OFS] = 1;
    }
}
