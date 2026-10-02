#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"
#include "MENU_LIST.H"

s32 UiText_MeasureStringVariant(s32 start, s32 *width, s32 *count, s32 mode);
s32 UiText_BuildRenderEntries(s32, s32);

void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, s32 mode, u32 flags);

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
    base = gWindowWork[0];
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

void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y)
{
    UiText_MeasureEntryDimensions(UiText_BuildRenderEntries(no, 0), x, y, 0);
}

s32 UiText_GetResourceDimensions(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)
{
    u16 *base;
    s32 temp;
    s32 offset;

    base = (u16 *)gWindowWork[0];
    temp = UiText_BuildRenderEntries(no, 0);
    offset = temp * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + offset) == 0)
    {
        return 0;
    }
    UiWindow_FitOnScreen(temp, x, y, width, height, 0, 0);
    return 1;
}

s32 UiText_GetResourceDimensionsAlt(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)
{
    u16 *base;
    s32 idx;
    s32 ofs;

    base = (u16 *)gWindowWork[0];
    idx = UiText_BuildRenderEntries(no, 0);
    ofs = idx * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + ofs) == 0)
    {
        return 0;
    }
    UiWindow_FitOnScreen(idx, x, y, width, height, 0, 1);
    return 1;
}
