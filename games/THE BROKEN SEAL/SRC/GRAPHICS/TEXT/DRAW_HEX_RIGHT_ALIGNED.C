#include "LOW_RUNTIME.H"
#include "GLOBAL_CELLS.H"

void Func_080045f0(u32);
#define Text_FormatHexToWork Func_080045f0

void Text_DrawHexRightAligned(u32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 7U) {
        count = 8;
    }
    Text_FormatHexToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(ADDR_03001F78 - count));
}
