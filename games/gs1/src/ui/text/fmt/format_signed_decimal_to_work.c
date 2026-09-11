#include "types.h"
#include "scene.h"
#include "abi/ui/text/fmt/format_signed_decimal_to_work.h"

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
