#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "low_runtime.h"

/* ui/text/fmt/text_format_hex_to_work.c */
/* ui/text/fmt/format_hex_to_work.c */
extern const u8 RomBytes_0800795c[];

void Text_FormatHexToWork(u32 value)
{
    u8 *buffer = (u8 *)ADDR_03001F70;
    const u8 *digits = RomBytes_0800795c;
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

/* ui/text/format_signed_decimal_to_work.c */
extern s32 FunctionHead_080022f4(u32, s32);
extern u8 gRom[];
extern u8 gWork[];

void Text_FormatSignedDecimalToWork(s32 arg0) {
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
    out = (s8 *)gWork;
    if (val < 0) {
        val = -val;
        sign = 0x2D;
    }
    word = *tbl++;
    count = 9;
    if ((u32) val < word) {
        do {
            count -= 1;
            *out++ = 0x20;
            if (count == 0) break;
            word = *tbl++;
        } while ((u32) val < word);
    }
    *out++ = sign;
    tbl -= 1;
    if (count != 0) {
        do {
            word = *tbl++;
            result = FunctionHead_080022f4((u32) val, word);
            *out++ = result + 0x30;
            val -= result * word;
            count -= 1;
        } while (count != 0);
    }
    out[0] = val + 0x30;
    out[1] = 0;
}
