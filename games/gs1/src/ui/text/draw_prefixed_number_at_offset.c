#include "text_render_runtime.h"
#include "types.h"

extern u8 *Data_03001e8c;

u8 *Text_FormatNumber(u8 *output, s32 value, s32 width);
void Func_0801de5c(s16 *text, s32 source, s32 destination, s32 phase);

void UiText_DrawPrefixedNumberAtOffset(
    s32 value,
    struct TextRenderWork *work,
    s32 offset_x,
    s32 offset_y,
    s32 variant)
{
    u8 formatted[16];
    s16 output[8];
    u8 *formatted_text;
    s32 index;
    u32 cell;
    u8 *base;
    s32 vram;
    s32 source_address;
    s32 destination_address;
    s32 phase;

    base = Data_03001e8c;
    formatted_text = Text_FormatNumber(formatted, value, 4);
    if (variant == 0) {
        output[0] = 0xf01d;
    } else {
        output[0] = 0xf01f;
    }
    output[1] = 0xf01e;
    for (index = 0; index <= 4; index++) {
        output[index + 2] = formatted_text[index];
    }
    output[6] = 0;

    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        destination_address = vram + cell;
        source_address = (s32)base + cell;
        phase = offset_x & 7;
        Func_0801de5c(
            output,
            source_address,
            destination_address,
            phase);
    }
}
