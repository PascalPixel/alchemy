#include "LOW_RUNTIME.H"
#include "GLOBAL_CELLS.H"

extern u8 Data_03001f78[];
void Text_FormatHexToWork(u32);

extern u8 Data_03001f7a[];
void Text_FormatSignedDecimalToWork(s32);

void Text_DrawHexRightAligned(u32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 7U) {
        count = 8;
    }
    Text_FormatHexToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&Data_03001f78) - count));
}

void Text_DrawSignedDecimalRightAligned(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 9U) {
        count = 0xA;
    }
    Text_FormatSignedDecimalToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&Data_03001f7a) - count));
}
