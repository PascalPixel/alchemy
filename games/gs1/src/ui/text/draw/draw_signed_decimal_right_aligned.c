#include "scene.h"
#include "abi/ui/text/draw/draw_signed_decimal_right_aligned.h"
#include "low_runtime.h"
#include "global_cells.h"

void Text_DrawSignedDecimalRightAligned(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 9U) {
        count = 0xA;
    }
    Ui_Do(value);
    Ui_Run((const u8 *)(ADDR_03001F7A - count));
}
