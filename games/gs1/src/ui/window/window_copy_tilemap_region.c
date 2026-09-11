#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"

/* ui/window/copy_tilemap_region.c */
extern u8 *gIw;

struct UiWindowTilemapRegion {
    u8 padding0[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

s16 *Runtime_BumpAllocateAlternatePool(s32 size);

void UiWindow_CopyTilemapRegion(const struct UiWindowTilemapRegion *window, const void *source)
{
    s16 *mirror = (s16 *)gIw;
    s16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
    s16 *input = buffer;
    u32 cell;
    s16 *vram;
    s32 row;

    Ui_Apply(source, buffer);
    cell = window->y * 32 + window->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;

    for (row = 0; row < window->height; row++) {
        s32 column;

        for (column = 0; column < window->width; column++) {
            s16 value = *input++;

            *vram++ = value;
            *mirror++ = value;
        }
        vram += 32 - window->width;
        mirror += 32 - window->width;
    }
    Ui_Do(buffer);
}

/* ui/window/set_tile_attribute_rect.c */
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
