#include "types.h"
#include "runtime_interfaces.h"
#include "gs1_edition.h"

s32 Func_08018a50(s32 start, s32 *width, s32 *count, s32 mode);

extern u8 *Data_03001e8c;

void Func_0801868c(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, s32 mode, u32 flags)
{
    u8 *base;
    s32 x;
    s32 y;
    s32 limit;
    s32 right;
    s32 bottom;
    s32 over;
    s32 pos;

    base = Data_03001e8c;
    x = *px;
    y = *py;
    limit = 30;

    if (!(flags & 2)) {
        if (flags & 1)
            Func_08018a50(no, (s32 *)pw, (s32 *)ph, mode);
        else
            Func_08018850(no, (s32 *)pw, (s32 *)ph, mode);
    }

    if (*pw == 0 && *ph == 0)
        return;

    if (!(flags & 2)) {
        *pw = (*pw + 19) >> 3;
        *ph = (*ph + 15) >> 3;
        if (base[RENDER_MODE_OFS] != 0) {
            *pw += 2;
            limit = 29;
        }
    }

    right = x + *pw;
    if (right > limit) {
        over = right - limit;
        pos = x - over;
        if (pos >= 0)
            x = pos;
        else
            x = 0;
    }
    bottom = y + *ph;
    if (bottom > 20) {
        over = bottom - 20;
        pos = y - over;
        if (pos >= 0)
            y = pos;
        else
            y = 0;
    }
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x > limit - *pw)
        x = limit - *pw;
    if (y > 20 - *ph)
        y = 20 - *ph;
    *px = x;
    *py = y;
}
