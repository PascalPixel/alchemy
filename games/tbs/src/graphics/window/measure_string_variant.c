#include "types.h"
#include "window.h"
#include "glyph.h"

s32 __divsi3(s32 numerator, s32 denominator);

void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height, u16 *spacing)
{
    struct UiRenderWork *work;
    u32 lines;
    u32 width;
    u32 height;
    u32 line_width;
    u32 count;
    u32 c;
    u32 glyph;
    u32 i;
    u16 counts[4];
    u16 widths[4];
    s32 gap;
    u32 style;
#if !EDITION_INTERNATIONAL
    s32 trailing;
#endif

    work = (struct UiRenderWork *)gWindowWork[0];
#if !EDITION_INTERNATIONAL
    trailing = 0;
#endif
    width = 0;
    height = 15;
    lines = 0;
    line_width = 0;
    count = 0;
    for (;;) {
        c = work->entries[pos];
        pos = (pos + 1) & RENDER_ENTRY_MASK;
        if (c > 31) {
#if EDITION_INTERNATIONAL
            if (c == 32) {
                line_width += 5;
                count++;
            } else {
                glyph = UiText_Glyphs[c - 32].width;
                style = work->outline;
                if (style == 1 || style == 5)
                    glyph++;
                line_width += glyph;
            }
#else
            if (c != 0xde && c != 0xdf) {
                if (c == 32) {
                    line_width += 7;
                } else {
                    if (c == 0xa5)
                        trailing = -1;
                    else if (c == 0x21)
                        trailing = -3;
                    else if (c == 0xa1 || c == 0xa4)
                        trailing = -6;
                    else
                        trailing = 0;
                    glyph = 10;
                    if (c <= 0xff)
                        glyph = UiText_Glyphs[c - 32].width;
                    style = work->outline;
                    if (style == 1 || style == 5)
                        glyph++;
                    line_width += glyph;
                }
            }
            count++;
#endif
            continue;
        }
        switch (c) {
        case 0:
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
        case 1:
#endif
            goto done;
        case 3:
#if EDITION_INTERNATIONAL
            count++;
#else
            line_width += trailing;
#endif
            counts[lines] = count;
            widths[lines] = line_width;
            if (width < line_width)
                width = line_width;
            if (lines < 3)
                lines++;
            count = 0;
            line_width = 0;
            height += 15;
            break;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        case 1:
            work->outline = 0;
            goto done;
#endif
        case 14:
        case 28:
            pos = (pos + 1) & RENDER_ENTRY_MASK;
        case 8:
        case 10:
        case 15:
        case 17:
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
        case 9:
            work->outline = work->entries[pos];
            pos = (pos + 1) & RENDER_ENTRY_MASK;
            break;
        }
    }
done:
#if EDITION_INTERNATIONAL
    count++;
#else
    line_width += trailing;
#endif
    counts[lines] = count;
    widths[lines] = line_width;
    if (width < line_width)
        width = line_width;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
    if (work->mode)
        width = width + 2;
#endif
    *out_width = width;
    *out_height = height;
    width = (width + 19) >> 3;
    width = (width << 3) - 16;
    if (spacing != NULL) {
        i = 0;
        while (i <= lines) {
            if (counts[i] <= 1) {
                *spacing++ = 0;
            } else {
                gap = width - widths[i];
                gap -= 4;
                if (gap < 0)
                    gap = 0;
                gap = __divsi3(gap << 8, counts[i] - 1);
#if EDITION_INTERNATIONAL
                if (gap < 0 || gap > 0xc00)
                    gap = 0x200;
#else
                if (gap < 0 || gap > 0x800)
                    gap = 0;
                if (gap < 0 || gap > 0x100)
                    gap = 0x100;
#endif
                *spacing++ = gap;
            }
            i++;
        }
    }
}

void UiText_MeasureStringVariant(s32 start, u32 *out_width, u32 *out_height,
                                 u16 *spacing)
{
    u32 heights[16];
    u16 counts[4];
    u16 widths[4];
    struct UiRenderWork *work;
    u32 max_width;
    u32 line;
    u32 width;
    u32 count;
    u32 variant;
    u32 code;
    u32 glyph;
    u32 outline;
    u32 i;
    u32 j;
    s32 gap;
#if !EDITION_INTERNATIONAL
    s32 trailing;
#endif

#if !EDITION_INTERNATIONAL
    trailing = 0;
#endif
    work = (struct UiRenderWork *)gWindowWork[0];
    heights[0] = 15;
    heights[1] = 15;
    heights[2] = 15;
    heights[3] = 15;
    heights[4] = 15;
    heights[5] = 15;
    heights[6] = 15;
    heights[7] = 15;
    heights[8] = 15;
    heights[9] = 15;
    heights[10] = 15;
    heights[11] = 15;
    heights[12] = 15;
    max_width = 0;
    heights[13] = 15;
    heights[14] = 15;
    heights[15] = 15;
    line = 0;
    width = 0;
    count = 0;
    variant = 0;
    for (;;) {
        code = work->entries[start];
        start = (start + 1) & RENDER_ENTRY_MASK;
        if (code >= 32) {
#if EDITION_INTERNATIONAL
            if (code == 32) {
                width += 5;
                count++;
            } else {
                glyph = UiText_Glyphs[code - 32].width;
                outline = work->outline;
                if (outline == 1 || outline == 5)
                    glyph++;
                width += glyph;
            }
#else
            if (code != 0xde && code != 0xdf) {
                if (code == 32) {
                    width += 7;
                } else {
                    if (code == 0xa5)
                        trailing = -1;
                    else if (code == 0x21)
                        trailing = -3;
                    else if (code == 0xa1 || code == 0xa4)
                        trailing = -6;
                    else
                        trailing = 0;
                    glyph = 10;
                    if (code <= 0xff)
                        glyph = UiText_Glyphs[code - 32].width;
                    outline = work->outline;
                    if (outline == 1 || outline == 5)
                        glyph++;
                    width += glyph;
                }
            }
            count++;
#endif
            continue;
        }
        switch (code) {
        case 3:
#if EDITION_INTERNATIONAL
            counts[line] = ++count;
#else
            width += trailing;
            counts[line] = count;
#endif
            widths[line] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            if (line < 3)
                line++;
            count = 0;
            width = 0;
            heights[variant] += 15;
            break;
        case 0:
#if EDITION_INTERNATIONAL
            counts[line] = ++count;
#else
            width += trailing;
            counts[line] = count;
#endif
            widths[line] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            variant++;
            goto done;
        case 1:
#if EDITION_INTERNATIONAL
            counts[line] = ++count;
#else
            width += trailing;
            counts[line] = count;
#endif
            widths[line] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            work->outline = 0;
#endif
            variant++;
            break;
        case 14:
        case 28:
            start = (start + 1) & RENDER_ENTRY_MASK;
        case 8:
        case 10:
        case 15:
        case 17:
            start = (start + 1) & RENDER_ENTRY_MASK;
            break;
        case 9:
            work->outline = work->entries[start];
            start = (start + 1) & RENDER_ENTRY_MASK;
            break;
        }
    }
done:
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_FR)
    if (work->mode != 0)
        max_width += 2;
#endif
    for (i = 0; i < variant; i++) {
        if (i == 0)
            *out_height = heights[0];
        else if (*out_height < heights[i])
            *out_height = heights[i];
    }
    *out_width = max_width;
    max_width = (((max_width + 19) >> 3) << 3) - 16;
    if (spacing != NULL) {
        for (j = 0; j <= line; j++) {
            if (counts[j] <= 1) {
                *spacing++ = 0;
            } else {
                gap = max_width - widths[j];
                gap -= 4;
                if (gap < 0)
                    gap = 0;
                gap = (gap << 8) / (counts[j] - 1);
#if !EDITION_INTERNATIONAL
                if (gap < 0 || gap > 0x800)
                    gap = 0;
                if (gap < 0 || gap > 0x100)
                    gap = 0x100;
#endif
                *spacing++ = gap;
            }
        }
    }
}
