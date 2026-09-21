#include "LOW_RUNTIME.H"
#include "GLOBAL_CELLS.H"

void Func_08004620(s32);
#define Text_FormatSignedDecimalToWork Func_08004620

void Text_DrawSignedDecimalRightAligned(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 9U) {
        count = 0xA;
    }
    Text_FormatSignedDecimalToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(ADDR_03001F7A - count));
}
