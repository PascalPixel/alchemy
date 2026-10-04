#include "TYPES.H"
#include "WINDOW.H"
#include "GLYPH.H"

/* Measures a rendered message: the widest line of its first variant, the
   tallest variant (a newline adds 15 pixels to the variant it ends) and, for
   up to four lines, the 8.8 spacing that spreads each line across the window
   width. Control 0 ends the message, 1 starts the next variant, 3 the next
   line. The Japanese edition counts every character and takes back a few
   pixels after certain punctuation; the localized editions count spaces. */
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
            /* fall through */
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
                if ((u32)gap > 0x800)
                    gap = 0;
                if ((u32)gap > 0x100)
                    gap = 0x100;
#endif
                *spacing++ = gap;
            }
        }
    }
}
