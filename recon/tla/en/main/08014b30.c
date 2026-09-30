extern u8 Data_03001f78[];

void Text_FormatHexToWork(u32);

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
