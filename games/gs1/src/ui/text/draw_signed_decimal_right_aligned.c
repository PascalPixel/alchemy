#include "low_runtime.h"
#include "global_cells.h"

#define Text_DrawSignedDecimalRightAligned Func_0800473c

void Func_08004620(s32);

void Text_DrawSignedDecimalRightAligned(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 9U) {
        count = 0xA;
    }
    Func_08004620(value);
    Func_080046c4((const u8 *)(ADDR_03001F7A - count));
}
