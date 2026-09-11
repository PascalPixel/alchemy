#include "types.h"
#include "scene.h"
#include "abi/ui/text/fmt/format_number_to_halfwords.h"
#include "global_cells.h"


s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    s16 *dst;
    s32 n;
    u8 *src;

    dst = out;
    Ui_Do(value);
    src = (u8 *)ADDR_03001F70;
    n = 0xD;
    do {
        n -= 1;
        *dst = (s16)*src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}
