#include "scene.h"
#include "abi/ui/text/draw/draw_hex_right_aligned.h"
#include "low_runtime.h"
#include "global_cells.h"

void Text_DrawHexRightAligned(u32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 7U) {
        count = 8;
    }
    Ui_Do(value);
    Ui_Run((const u8 *)(ADDR_03001F78 - count));
}
