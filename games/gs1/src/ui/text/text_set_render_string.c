#include "types.h"
#include "scene.h"
#include "runtime_interfaces.h"
#include "global_cells.h"

/* ui/text/misc/set_render_string.c */
s32 UiText_SetRenderString(const u8 *str)
{
    u8 *base;
    u16 *dst;
    s32 count;
    s32 offset;
    s32 count_out;
    s32 width_out;

    base = *(u8 **)ADDR_03001E8C;
    count = 0;
    if (*str != 0) {
        dst = (u16 *)(base + 0xEB0);
        do {
            *dst = *str;
            str++;
            dst++;
            count++;
        } while (*str != 0);
    }
    offset = 0xEB0 + count * 2;
    *(u16 *)(base + offset) = 0;
    Ui_Run(0, &count_out, &width_out, 0);
    return count_out;
}

/* ui/text/draw/draw_padded_label.c */
void UiText_DrawString(u8 *arg0, s32 arg1, s32 arg2, s32 arg3);

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
    u8 text[20];
    s32 length = 0;

    if (*input != 0) {
        do {
            text[length] = *input;
            input++;
            length++;
        } while (*input != 0);
    }

    text[length++] = 8;
    text[length++] = 2;

    while (length <= 6) {
        text[length++] = 95;
    }

    text[length++] = 8;
    text[length++] = 15;
    text[length] = 0;
    UiText_DrawString(text, output, 0, -2);
}
