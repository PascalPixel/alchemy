#include "text_render_runtime.h"
#include "types.h"

#define UiText_DrawFourNumbersInRow Func_08020150

void UiText_DrawFourNumbersInRow(struct TextRenderWork *work, s32 base)
{
    s32 pos;
    s32 cnt;
    volatile u8 *src;
    s32 size;

    if (work != 0) {
        pos = 0;
        size = 0x10;
        cnt = 3;
        src = (volatile u8 *) (base + 0x28);
        do {
            UiText_DrawNumberAtOffset((s8) *src, 2, work, pos, size);
            src += 1;
            cnt -= 1;
            pos = pos + 0x18;
        } while (cnt >= 0);
    }
}
