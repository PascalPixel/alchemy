#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "runtime_interfaces.h"

/* ui/window/window_copy_tilemap_region.c */
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

/* resource/copy_fixed_block_b.c */
/* resource/copy_fixed_block_b.c */
s32 Resource_CopyData(s32, s32, s32);
extern u8 gRom[];

void Resource_CopyFixedBlockB(s32 arg0)
{
    Resource_CopyData(arg0, 0x80, gRom);
}

/* resource/copy_fixed_block_a.c */
s32 Resource_CopyData(s32, s32, s32);
extern const u8 gRom[];

void Resource_CopyFixedBlockA(s32 arg0)
{
    Resource_CopyData(arg0, 0x80, (s32)gRom);
}

/* ui/text/text_set_render_string.c */
/* ui/text/misc/set_render_string.c */
s32 UiText_SetRenderString(const u8 *str)
{
    u8 *base;
    u16 *dst;
    s32 count;
    s32 offset;
    s32 count_out;
    s32 width_out;

    base = *(u8 **)ADDR_03001E8C;
    count = 0;
    if (*str != 0) {
        dst = (u16 *)(base + 0xEB0);
        do {
            *dst = *str;
            str++;
            dst++;
            count++;
        } while (*str != 0);
    }
    offset = 0xEB0 + count * 2;
    *(u16 *)(base + offset) = 0;
    Ui_Run(0, &count_out, &width_out, 0);
    return count_out;
}

/* ui/text/draw/draw_padded_label.c */
void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
    u8 text[20];
    s32 length = 0;

    if (*input != 0) {
        do {
            text[length] = *input;
            input++;
            length++;
        } while (*input != 0);
    }

    text[length++] = 8;
    text[length++] = 2;

    while (length <= 6) {
        text[length++] = 95;
    }

    text[length++] = 8;
    text[length++] = 15;
    text[length] = 0;
    UiText_DrawString(text, output, 0, -2);
}
