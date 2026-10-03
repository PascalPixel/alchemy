#include "DMA.H"
#include "TBS_EDITION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"
#include "WINDOW.H"

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width);
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);
typedef s32 (*UiFillFn)(void *dst, s32 size, u32 value);

void UiWork_ResetCounters();

void RenderOutput_PrepareForRedraw(struct UiWindow *window);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
void RenderOutput_ClearList(void *work);
void RenderOutput_Release(struct RenderOutput *node);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWork_DrawByAttributes(struct UiWindow *window);
void UiWork_WaitUntilField1aClear(struct UiWindow *window);

void UiWork_UploadDirtyBlocks(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 flags;
    u8 *src;
    u8 *dst;
    if (!work->menu_busy) {
        flags = work->dirty;
        if (flags) {
            dst = (u8 *)0x06002000;
            src = (u8 *)work->tilemap;
            if (flags & 1) flags = 63;
            flags &= 63;
            flags >>= 1;
            do {
                if (flags & 1) Dma_Set(src, dst, 0x84000040, (volatile u32 *)0x040000d4);
                flags >>= 1;
                src += 256;
                dst += 256;
            } while (flags);
            work->dirty = flags;
        }
    }
}

void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
    s32 tile;
    u32 bottom;
    u32 row;
    u8 *mode;

    tile = 240;
    bottom = y + height;
    tile <<= 8;
    if (bottom > 20)
        height = 20 - y;
    if (width <= 1)
        width = 2;
    if (width > 30)
        width = 30;
    if (height <= 1)
        height = 2;
    if (height > 30)
        height = 30;

    UiWindow_ClearTileAttributesInRect(x, y, width, height);

    row = 0;
    if (row < height) {
        mode = &canvas->menu_state;
        do {
            u32 column;

            if (*mode != 0) {
                if ((u32)(y + row) > 16)
                    tile = 0xF07F;
                else
                    tile = 0xF000;
            }
            column = 0;
            if (column < width) {
                do {
                    *cursor++ = tile;
                    column++;
                } while (column < width);
            }
            row++;
            cursor += 32 - width;
        } while (row < height);
    }
    canvas->dirty = 1;
}

void UiWork_DrawByAttributes(struct UiWindow *window)
{
    u32 flags;
    u32 value;
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    void *dst;
    UiFillFn fill;
    struct UiRenderWork *work;

    /* 描画属性に従い転送方法を切り替える。 */
    work = (struct UiRenderWork *)gWindowWork[0];
    value = window->height;
    flags = window->flags;
    height = value;
    value = 0;
    window->duration = value;
    x = window->x;
    y = window->y;
    width = window->width;
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

/* Open a window at a tile position and size in the first free record, with
   the attribute bits that choose its frame and drawing; a window drawn
   through its attributes appears at once, any other one opens over eight
   frames. Returns the record, or 0 when every record is in use. */
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs)
{
    struct UiWindow *slot;
    struct UiWindow *found;
    s32 i;

    slot = ((struct UiRenderWork *)gWindowWork[0])->windows;
    found = 0;
    i = 0;
    while ((slot->flags & 1) != 0 || slot->duration != 0) {
        i++;
        slot++;
        if (i == WINDOW_COUNT) {
            goto done;
        }
    }
    found = slot;
done:
    if (found != 0) {
        found->y = y;
        found->width = width;
        found->height = height;
        found->x = x;
        found->output.head = NULL;
        found->state = 0;
        found->output.tail_link = &slot->output.head;
        found->unknown_10 = 1;
        found->flags = 1;
        UiWork_ResetCounters(x); /* the reset ignores the x it is passed */
        if (attrs & 8) {
            found->flags |= 8;
        }
        if (attrs & 32) {
            found->flags |= 32;
        }
        if (attrs & 64) {
            found->flags |= 64;
        }
        if (attrs & 128) {
            found->flags |= 128;
        }
        if (attrs & 0x100) {
            found->flags |= 0x100;
        }
        if (attrs & 2) {
            found->flags |= 2;
            found->frame = 0;
            found->duration = 1;
            UiWork_DrawByAttributes(found);
        } else {
            found->duration = 8;
            found->frame = 7;
            UiWork_WaitUntilField1aClear(found);
            WaitFrames(1);
        }
    }
    return found;
}

void UiWork_WaitUntilField1aClear(struct UiWindow *window)
{
    /* 値が0になるまで更新処理を進める。 */
    if (!(2 & window->flags) && window->duration != 0) {
        do {
            WaitFrames(1);
        } while (window->duration != 0);
    }
}

void UiWork_Finalize(struct UiWindow *work, s32 release)
{
    u16 zero;

    if (work == 0)
        return;

    RenderOutput_PrepareForRedraw(work);
    work->previous_x = work->x;
    work->previous_y = work->y;
    work->previous_width = work->width;
    zero = 0;
    work->flags = zero;
    work->previous_height = work->height;

    if (release != 0) {
        UiWindow_EraseBorderRect(work->x, work->y, work->width, work->height);
        work->output.head = NULL;
        work->output.tail_link = NULL;
        work->width = zero;
        work->height = zero;
        work->x = zero;
        work->y = zero;
        work->unknown_10 = zero;
        work->unknown_12 = zero;
        work->state = zero;
        work->flags = zero;
        work->frame = zero;
        work->duration = zero;
        work->previous_x = zero;
        work->previous_y = zero;
        work->previous_width = zero;
        work->previous_height = zero;
    } else {
        work->frame = release;
        work->duration = 4;
    }
}

void RenderOutput_PrepareForRedraw(struct UiWindow *window)
{
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & window->flags)) {
        RenderOutput_RedrawSavedRect(window);
        RenderOutput_ClearList(window);
    }
}

void RenderOutput_RedrawSavedRect(struct UiWindow *window)
{
    /* 保存済みの矩形を再描画する。 */
    UiWindow_DrawFrame(window->x, window->y, window->width, window->height);
}

void RenderOutput_ClearList(void *work)
{
    struct RenderOutputList *list = work;
    struct RenderOutput *node;
    struct RenderOutput *next;

    if (list != NULL) {
        node = list->head;
        list->tail_link = &list->head;
        list->head = NULL;
        while (node != NULL) {
            next = node->next;
            RenderOutput_Release(node);
            node = next;
        }
    }
}
