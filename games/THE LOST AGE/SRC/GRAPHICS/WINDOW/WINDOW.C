#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "WINDOW.H"
#include "IO_REG.H"

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

/* Copies each dirty quarter of the window tilemap to VRAM, unless a page
   is being redrawn. */
void UiWork_UploadDirtyBlocks(void)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    u32 flags;
    u8 *src;
    u8 *dst;

    flags = work->dirty;
    if (flags != 0) {
        if (!work->menu_busy) {
            src = (u8 *)work->tilemap;
            dst = (u8 *)0x06002000;
            if (flags & 1) flags = 63;
            flags &= 63;
            flags >>= 1;
            do {
                if (flags & 1) Dma_Set(src, dst, 0x84000040, REG_DMA3);
                flags >>= 1;
                src += 256;
                dst += 256;
            } while (flags);
            work->dirty = flags;
        }
    }
}

/* Blanks a window's rectangle of the tilemap, at least two cells and at
   most thirty each way; with the menu state set, rows below 16 take the
   menu's lower blank. */
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    u16 *cursor = &canvas->tilemap[y][x];
    s32 tile;
    u32 bottom;
    u32 row;

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
        do {
            u32 column;

            if (canvas->menu_state != 0) {
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
