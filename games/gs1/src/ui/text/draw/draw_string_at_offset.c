#include "text_render_runtime.h"
#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_string_at_offset.h"

extern u8 *gIw;

s16 *Runtime_BumpAllocateAlternatePool(s32 arg0);

void UiText_DrawStringAtOffset(
    u8 *text,
    struct TextRenderWork *work,
    s32 offset_x,
    s32 offset_y)
{
    s16 *buffer;
    s16 *output;
    u8 *base;
    u32 cell;
    s32 vram;
    s32 src;
    s32 dst;
    s32 phase;

    buffer = Runtime_BumpAllocateAlternatePool(0x200);
    base = gIw;
    output = buffer;
    if (*text != 0) {
        do {
            *output = *text;
            text++;
            output++;
        } while (*text != 0);
    }
    *output = 0;

    /* 座標を8ドット単位のセル番号へ変換する。 */
    cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5)
        + (work->x + ((u32)offset_x >> 3)) + 1;
    if (cell < 0x280) {
        vram = 0x06002000;
        cell *= 2;
        dst = vram + cell;
        src = (s32)base + cell;
        phase = offset_x & 7;
        Ui_SetMode(
            buffer,
            src,
            dst,
            phase);
        Ui_Check(buffer);
    }
}
