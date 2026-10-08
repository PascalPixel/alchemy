#include "types.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "window.h"

void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width);
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);
typedef s32 (*UiFillFn)(void *dst, s32 size, u32 value);

void UiWork_DrawByAttributes(struct UiWindow *window)
{
    u32 flags;
    s32 x;
    s32 y;
    u32 width;
    u32 height;
    void *dst;
    UiFillFn fill;
    struct UiRenderWork *work;

    work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    x = (s16)window->x;
    y = (s16)window->y;
    height = window->height;
    width = window->width;
    flags = window->flags;
    window->duration = 0;
    if (8 & flags) {
        if (0x20 & flags) {
            UiWindow_DrawFrame(x, y, width, height);
            fill = Iwram_FillWords;
            dst = (void *)0x06002500;
            fill(dst, 0xF00, 0x44444444);
        } else {
            fill = Iwram_FillWords;
            dst = (void *)0x06002500;
            fill(dst, 0xF00, 0);
        }
        UiWindow_MapTextCanvasTiles(x, y, width, height, 0);
    } else {
        UiWindow_DrawFrame(x, y, width, height);
    }
    work->dirty = 1;
}
