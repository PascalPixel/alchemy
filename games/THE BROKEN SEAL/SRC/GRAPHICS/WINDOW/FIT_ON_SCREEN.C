#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "TBS_EDITION.H"

s32 UiText_MeasureStringVariant(s32 start, s32 *width, s32 *count, s32 mode);

extern u8 *gWindowWork;

/* The European editions never widen a window for the render mode. */
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define FIT_FIXED_LIMIT 1
#endif

void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, s32 mode, u32 flags)
{
#if !defined(FIT_FIXED_LIMIT)
    u8 *base;
#endif
    s32 x;
    s32 y;
    s32 limit;
    s32 right;
    s32 bottom;
    s32 over;
    s32 pos;

#if !defined(FIT_FIXED_LIMIT)
    base = gWindowWork;
#endif
    x = *px;
    y = *py;
    limit = 30;

    if (!(flags & 2)) {
        if (flags & 1)
            UiText_MeasureStringVariant(no, (s32 *)pw, (s32 *)ph, mode);
        else
            UiText_MeasureEntryDimensions(no, (s32 *)pw, (s32 *)ph, mode);
    }

    if (*pw == 0 && *ph == 0)
        return;

    if (!(flags & 2)) {
        *pw = (*pw + 19) >> 3;
        *ph = (*ph + 15) >> 3;
#if !defined(FIT_FIXED_LIMIT)
        if (base[RENDER_MODE_OFS] != 0) {
            *pw += 2;
            limit = 29;
        }
#endif
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
