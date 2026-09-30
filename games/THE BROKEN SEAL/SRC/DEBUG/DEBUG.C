#include "LOW_RUNTIME.H"
#include "GLOBAL_CELLS.H"

extern u8 Data_03001ac4[];
extern u8 gDebugTextCursor[];

extern u8 Data_03001f78[];
void Text_FormatHexToWork(u32);
extern u8 Data_03001f7a[];
void Text_FormatSignedDecimalToWork(s32);

void Runtime_WriteDebugTextTiles(const u8 *src)
{
    if (*(u8 *)((u32)&Data_03001ac4) != 0) {
        u32 addr = ((u32)&gDebugTextCursor);
        u32 c = *src;
        u16 *dst = *(u16 **)addr;
        u32 cnt = 0;
        src++;

        if (c != 0) {
            u32 mask = 0xf000;
            addr = 0x06002500;
            do {
                *dst++ = c | mask;
                if (dst == (u16 *)addr)
                    dst = (u16 *)0x06002000;
                cnt++;
                if (cnt > 31)
                    break;
                c = *src++;
            } while (c != 0);
            addr = ((u32)&gDebugTextCursor);
        }
        *(u16 **)addr = dst;
    }
}

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
