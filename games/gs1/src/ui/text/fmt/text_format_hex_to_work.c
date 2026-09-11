#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "low_runtime.h"

/* ui/text/fmt/text_format_hex_to_work.c */
/* ui/text/fmt/format_hex_to_work.c */
extern const u8 gRom[];

void Text_FormatHexToWork(u32 value)
{
    u8 *buffer = (u8 *)ADDR_03001F70;
    const u8 *digits = gRom;
    s32 index = 7;

    do {
        buffer[index] = digits[value & 0xF];
        value >>= 4;
        index--;
    } while (index >= 0);

    {
        u8 *terminator = (u8 *)ADDR_03001F70;
        terminator[8] = 0;
    }
}

/* ui/text/fmt/format_signed_decimal_to_work.c */
extern u8 gRom[];
extern u8 gIw[];

void Text_FormatSignedDecimalToWork(s32 arg0)
{
    u32 *tbl;
    s8 *out;
    s32 count;
    u32 word;
    s32 result;
    s32 val;
    s8 sign;

    result = arg0;
    val = result;
    tbl = (u32 *)gRom;
    sign = 0x20;
    out = (s8 *)gIw;
    if (val < 0) {
        val = -val;
        sign = 0x2D;
    }
    word = *tbl++;
    count = 9;
    if ((u32)val < word) {
        do {
            count -= 1;
            *out++ = 0x20;
            if (count == 0) break;
            word = *tbl++;
        } while ((u32)val < word);
    }
    *out++ = sign;
    tbl -= 1;
    if (count != 0) {
        do {
            word = *tbl++;
            result = Ui_Apply((u32)val, word);
            *out++ = result + 0x30;
            val -= result *word;
            count -= 1;
        } while (count != 0);
    }
    out[0] = val + 0x30;
    out[1] = 0;
}

/* graphics/fill_word_stream_with_f000.c */
/* graphics/fill_word_stream_with_f000.c */
extern u16 *gIwFillWordStreamWithF000;

void Graphics_FillWordStreamWithF000(u32 count)
{
    u16 *dst = gIwFillWordStreamWithF000;
    u32 index;

    for (index = 0; index < count; index++)
        *dst++ = 0xf000;
    gIwFillWordStreamWithF000 = dst;
}

/* runtime/write_debug_text_tiles.c */
void Runtime_WriteDebugTextTiles(const u8 *src)
{
    if (*(u8 *)ADDR_03001AC4 != 0) {
        u32 addr = ADDR_03001CBC;
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
            addr = ADDR_03001CBC;
        }
        *(u16 **)addr = dst;
    }
}

/* ui/text/draw/text_draw_hex_right_aligned.c */
/* ui/text/draw/draw_hex_right_aligned.c */
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

/* ui/text/draw/draw_signed_decimal_right_aligned.c */
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
