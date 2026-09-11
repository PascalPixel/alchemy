#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_character.h"
#include "gs1_edition.h"

struct TextPosition_0801e7c0 {
    u8 padding[12];
    u16 x;
    u16 y;
};

extern u8 *gIw;

s32 UiText_BuildRenderEntries(s32 character, s32 count);

void UiText_DrawCharacter(
    s32 character,
    struct TextPosition_0801e7c0 *position,
    u32 offset_x,
    u32 offset_y)
{
    s32 byte_offset;
    s32 vram_address;
    u16 *text;
    s32 zero;
    u32 cell;
    u8 *base;
    u16 *counter;

    base = gIw;
    counter = (u16 *)(base + RENDER_ENTRY_COUNT_OFS);
    zero = 0;
    *counter = zero;
    UiText_BuildRenderEntries(character, 1);

    byte_offset = *counter * 2 + RENDER_ENTRY_TBL_OFS;
    *(u16 *)(base + byte_offset) = zero;
    *counter = (u16)((*counter + 1) & RENDER_ENTRY_MASK);

    cell = ((position->y + (offset_y >> 3) + 1) << 5)
        + (position->x + (offset_x >> 3)) + 1;
    if (cell < 0x280U) {
        byte_offset = cell * 2;
        vram_address = byte_offset + 0x06002000;
        text = (u16 *)(base + RENDER_ENTRY_TBL_OFS);
        Ui_SetMode(
            text,
            (s32)(base + byte_offset),
            vram_address,
            7 & offset_x);
    }
}
