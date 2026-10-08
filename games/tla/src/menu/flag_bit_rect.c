#include "types.h"
#include "window.h"
#include "ram_buffer.h"

void Render_SetTilemapFlagBitRect(
    const struct UiWindow *object, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;

    x += (s16)object->x + 1;
    y += (s16)object->y + 1;
    field &= 1;
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
        do {
            u16 *pixel = &work->tilemap[y][x];
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *pixel;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *pixel = value;
                pixel++;
            }
            height--;
            y++;
        } while (height != 0);
        work->dirty = 1;
    }
}
