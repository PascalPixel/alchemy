#include "LOW_RUNTIME.H"
#include "GLOBAL_CELLS.H"

void Text_FormatSignedDecimalToWork(s32);

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
