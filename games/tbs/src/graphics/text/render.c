#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "text_render_runtime.h"
#include "window.h"
#include "mem.h"
#include "global_cells.h"
#include "tbs_edition.h"

void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    u16 *counter = &canvas->count;
    s32 zero = 0;

    *counter = zero;
    UiText_BuildRenderEntries(arg0, 1);
    canvas->entries[*counter] = zero;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;
    UiText_RenderWideStringAtOffset(canvas->entries, (struct UiWindow *)arg1, arg2, arg3);
}

void UiText_DrawCharacterAtOffset(
    s32 character,
    struct RenderInput *position,
    u32 offset_x,
    u32 offset_y)
{
    s32 byte_offset;
    s32 vram_address;
    u16 *text;
    s32 zero;
    u32 cell;
    struct UiRenderWork *canvas;
    u16 *counter;

    canvas = (struct UiRenderWork *)gWindowWork[0];
    counter = &canvas->count;
    zero = 0;
    *counter = zero;
    UiText_BuildRenderEntries(character, 1);

    canvas->entries[*counter] = zero;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;

    cell = ((position->y + (offset_y >> 3) + 1) << 5)
        + (position->x + (offset_x >> 3)) + 1;
    if (cell < 0x280U) {
        byte_offset = cell * 2;
        vram_address = byte_offset + 0x06002000;
        text = canvas->entries;
        UiText_RenderStringTiles(
            text,
            (u16 *)((u8 *)canvas->tilemap + byte_offset),
            (u16 *)vram_address,
            7 & offset_x);
    }
}

void UiText_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3)
{
    s16 *buffer;
    s16 *output;
    u8 *input;

    input = text;
    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    output = buffer;
    if (*input != 0) {
        do {
            *output = *input;
            input++;
            output++;
        } while (*input != 0);
    }
    *output = 0;
    UiText_RenderWideStringAtOffset((u16 *)buffer, (struct UiWindow *)arg1, arg2, arg3);
    MemFree(buffer);
}

void UiText_DrawStringAtOffset(
    u8 *text,
    struct RenderInput *work,
    s32 offset_x,
    s32 offset_y)
{
    s16 *buffer;
    s16 *output;
    struct UiRenderWork *canvas;
    u32 cell;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    canvas = (struct UiRenderWork *)gWindowWork[0];
    output = buffer;
    if (*text != 0) {
        do {
            *output = *text;
            text++;
            output++;
        } while (*text != 0);
    }
    *output = 0;

    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)canvas->tilemap + cell;
        phase = offset_x & 7;
        UiText_RenderStringTiles(
            (u16 *)buffer,
            (u16 *)src,
            (u16 *)dst,
            phase);
        MemFree(buffer);
    }
}

void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y)
{
    u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
    u16 *output = buffer;

    while (*text != 0) {
        *output = *text;
        text++;
        output++;
    }
    *output = 0;
    x >>= 3;
    y >>= 3;
    UiText_RenderWideStringInWindow((s16 *)buffer, (struct UiWindow *)arg1, x, y);
    MemFree(buffer);
}

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberAtOffset(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    u8 data[16];

    UiText_DrawStringAtOffset(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    UiText_DrawStringInWindow(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawPrefixedNumberAtOffset(
    s32 value,
    struct RenderInput *work,
    s32 offset_x,
    s32 offset_y,
    s32 variant)
{
    u8 formatted[16];
    s16 output[8];
    u8 *text;
    s32 index;
    u32 cell;
    struct UiRenderWork *canvas;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    canvas = (struct UiRenderWork *)gWindowWork[0];
    text = UiText_FormatNumber(formatted, value, 4);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
    if (variant == 0) {
        output[0] = 0xf01d;
        output[1] = 0xf01e;
    } else {
        output[0] = 0xf01f;
        output[1] = 0xf005;
    }
#else
    if (variant == 0) {
        output[0] = 0xf01d;
    } else {
        output[0] = 0xf01f;
    }
    output[1] = 0xf01e;
#endif
    for (index = 0; index <= 4; index++) {
        output[index + 2] = text[index];
    }
    output[6] = 0;

    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)canvas->tilemap + cell;
        phase = offset_x & 7;
        UiText_RenderStringTiles(
            (u16 *)output,
            (u16 *)src,
            (u16 *)dst,
            phase);
    }
}
