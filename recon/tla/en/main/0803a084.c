/* ported from games/tbs/src/graphics/window/draw_frame.c */
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "window.h"


void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
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
