#include "text_render_runtime.h"
#include "window.h"
#include "ram_buffer.h"

s32 UiText_BuildRenderEntries(s32 message, s32 mode);

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

    canvas = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    counter = &canvas->count;
    zero = 0;
    *counter = zero;
    UiText_BuildRenderEntries(character, 1);

    canvas->entries[*counter] = zero;
    *counter = (u16)((*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1));

    cell = (((s16)position->y + (offset_y >> 3) + 1) << 5)
        + ((s16)position->x + (offset_x >> 3)) + 1;
    if (cell < 0x280U) {
        byte_offset = cell * 2;
        vram_address = byte_offset + 0x06002000;
        text = canvas->entries;
        UiText_RenderStringTiles(
            text,
            (s32)((u8 *)canvas->tilemap + byte_offset),
            vram_address,
            7 & offset_x);
    }
}
