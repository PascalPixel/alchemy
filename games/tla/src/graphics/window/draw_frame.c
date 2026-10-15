#include "types.h"
#include "ram_buffer.h"
#include "window.h"

u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count);
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    u16 *cursor = &canvas->tilemap[y][x];
    u32 row;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    if (canvas->mode != 0)
        *cursor++ = 0xf01c;
    else
        *cursor++ = 0xf010;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf011f011, width - 2);
    if (canvas->mode != 0)
        *cursor++ = 0xf41c;
    else
        *cursor++ = 0xf012;
    cursor += 32 - width;
    for (row = 1; row < height - 1; row++) {
        *cursor++ = 0xf016;
        if (width != 2)
            cursor = Memory_FillHalfwordsDma(cursor, 0xf020f020, width - 2);
        *cursor++ = 0xf017;
        cursor += 32 - width;
    }
    if (canvas->mode != 0)
        *cursor++ = 0xf81c;
    else
        *cursor++ = 0xf013;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf014f014, width - 2);
    if (canvas->mode != 0)
        *cursor = 0xfc1c;
    else
        *cursor = 0xf015;
    canvas->dirty = 1;
}

void Func_0803a1c0(s32 x, s32 y, u32 width, u32 height)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    u16 *cursor = &canvas->tilemap[y][x];
    u32 row;

    if (width <= 1 || height <= 1 || width > 30 || height > 30)
        return;
    UiWindow_ClearTileAttributesInRect(x, y, width, height);
    *cursor++ = 0xf008;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf009f009, width - 2);
    *cursor++ = 0xf00a;
    cursor += 32 - width;
    for (row = 1; row < height - 1; row++) {
        *cursor++ = 0xf00e;
        if (width != 2)
            cursor = Memory_FillHalfwordsDma(cursor, 0xf020f020, width - 2);
        *cursor++ = 0xf00f;
        cursor += 32 - width;
    }
    *cursor++ = 0xf00b;
    cursor = Memory_FillHalfwordsDma(cursor, 0xf00cf00c, width - 2);
    *cursor = 0xf00d;
    canvas->dirty = 1;
}
