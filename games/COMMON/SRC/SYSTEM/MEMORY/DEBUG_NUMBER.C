#include "TYPES.H"

void Runtime_WriteDebugTextTiles(const u8 *src);
void Text_FormatHexToWork(u32);
void Text_FormatSignedDecimalToWork(s32);

/* The ends of the formatted number text: eight hex digits or ten decimal
   digits, each followed by its terminator. */
extern u8 gNumberTextHexEnd[];
extern u8 gNumberTextDecimalEnd[];

void Text_DrawHexRightAligned(u32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 7U) {
        count = 8;
    }
    Text_FormatHexToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextHexEnd) - count));
}

void Text_DrawSignedDecimalRightAligned(s32 value, s32 width)
{
    s32 count;

    count = width;
    if ((u32)(count - 1) > 9U) {
        count = 0xA;
    }
    Text_FormatSignedDecimalToWork(value);
    Runtime_WriteDebugTextTiles((const u8 *)(((u32)&gNumberTextDecimalEnd) - count));
}
