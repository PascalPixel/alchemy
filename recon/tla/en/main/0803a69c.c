#include "render_input.h"

struct MessageWindowWork {
    s32 records[0xf40 / 4];
    u16 entries[(0x1384 - 0xf40) / 2];
    u16 cursor;
    u16 scroll;
    u16 unknown_12f8;
    u8 busy;
    u8 pending;
};

#include "ram_buffer.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);
void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, s32 mode, u32 flags);
struct RenderInput *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 UiText_QueueRenderEntries(struct RenderInput *window, s32 entry, s32 x, s32 y, const u16 *colours, s32 flags);
void UiWork_Finalize(struct RenderInput *window, s32 release);

struct RenderInput *UiText_OpenMessageWindow(s32 message, s32 x, long y, u32 packed)
{
    s32 entry;
    u32 height;
    u32 width;
    u16 bounds[4];
    s32 flags;
    u16 *out;
    struct RenderInput *window;
    struct MessageWindowWork *work;

    work = (struct MessageWindowWork *)Ram_HeapSlots->window_tiles;
    work->cursor = (packed << 4) >> 20;
    /* FAKEMATCH: retain the null window as the scroll and layout zero. */
    window = NULL;
    work->scroll = (u32)window;
    packed &= 0xffff;
    flags = 0;
    entry = UiText_BuildRenderEntries(message, 1);
    if (work->entries[entry] == 0)
        return NULL;
    UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (s32)(out = bounds), (u32)window);
    if (width == 0 && height == 0)
        return NULL;
    if (!(packed & 1))
        flags |= 2;
    if (packed & 8)
        flags |= 8;
    if (packed & 16)
        flags |= 128;
    if (packed & 32)
        flags |= 256;
    window = UiWindow_Create(x, y, width, height, flags);
    if (window == NULL)
        return NULL;
    if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0) {
        UiWork_Finalize(window, 1);
        return NULL;
    }
    work->busy = 0;
    work->pending = 0;
    return window;
}
