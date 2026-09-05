#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08004620(s32 out);

s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value) {
    s16 *dst;
    s32 n;
    u8 *src;

    dst = out;
    Func_08004620(value);
    src = (u8 *)ADDR_03001F70;
    n = 0xD;
    do {
        n -= 1;
        *dst = (s16) *src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}
