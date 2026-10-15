/* Draft, differ 94 of 69: ⚓️'s UiWindow_SetTilemapFlagRect (graphics/window/
 * fixed_block.c) as overlay 64A's copy. The native strength-reduces the row
 * base (work + y * 64, stepping 64) and adds x * 2 + 8 per row, keeps width
 * in r7 and x in r5, and leaves the shifted field in its stack argument slot
 * (str [sp, #20], reloaded each pass with the 0xffffefff mask); this C
 * reduces the whole pixel address and keeps field and mask in registers.
 * goto loops measure 87. */
#include "types.h"
#include "ram_buffer.h"
#include "window.h"

void Func_02000db4(
    const struct UiWindow *object, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;

    x += (s16)object->x + 1;
    y += (s16)object->y + 1;
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

