/* Current draft (2026-10-03), not exact: EN score 3725, 110 differing rows
   (44 register, 30 operand, 18 reordered, 11 inserted, 7 deleted).
   Complete EN object .text is 610 bytes against the native 604.
   Bounded follow-up to the ownership repair below, two natural forms only:
   H1 reversed the scratch-array declarations to give native counts at sp20
   and widths at sp12. Score stayed 4195/119, 602 bytes.
   H2 made the first variant's height assignment separate from later maximum
   comparisons. Score became 3725/110; retained with H1. The glyph lookup
   already had the native instruction shape, so it was not changed.
   Remaining: work/max-width/line-count register lifetimes, boundary stores
   and switch-body order, out_height spilled instead of retained, and the
   spacing loop's extra advancing pointer. Frame size is the native 92.
   All six retained branches compile. Object .text/native extent, in
   JA/EN/DE/ES/FR/IT order: 698/704, 610/604, 582/576, 618/612,
   590/584, 618/612. No exact edition, adoption or new matching device. */
/* Earlier ownership repair (2026-10-03): EN score 4195, 119 differing rows
   (49 register, 29 operand, 19 reordered, 11 inserted, 11 deleted). EN
   object .text is 602 bytes against the 604-byte listing; no linked extent
   is proved. Uses the maintained render/font owners and unsigned width and
   height comparisons. Control 1 must retain the incremented count while
   advancing the variant; the older glyph_count + 1 store lost that update.
   Corrected semantics first scored 4985/128. Sixteen bounded natural forms
   tested register removal, height initialization/order, count expressions,
   direct slot indices and height/spacing loops. Direct slot indices and a
   simple height loop with consecutive initialization reached 4195/119.
   Own-ROM edition branches preserve Japanese character spacing, trailing
   width adjustments and caps; control 1 clears outline only in ES/FR/IT,
   and render mode widens JA/EN/ES/IT. All six branches compile.
   Object .text/native extent, in JA/EN/DE/ES/FR/IT order:
   690/704, 602/604, 582/576, 610/612, 590/584, 610/612.
   Non-English byte matching remains unproved. Earlier trials follow. */
/* 2026-09-29: eight minutes of permutation: 5510 -> 4770 (41 register-only,
 * 29 operand, 16 reordered, 16 inserted, 14 deleted), kept in natural form:
 * locals reordered with variant as a register local, max_width and width
 * cleared before the line setup, line 13 and the variant's line height
 * reached through pointer arithmetic, and the height and spacing loops
 * rotated into tested do/while form with the spacing tables read through
 * pointers. */
/* Earlier draft, not exact (2026-09-29): 600 of 604 bytes, 249 differing halfwords.
   Complete owner: [0x08018a50, 0x08018cac). Control code 0 finishes,
   1 advances the variant, and 3 adds a line. The indirect dispatch,
   register allocation and parse/spacing loop shapes remain different. */
#include "WINDOW.H"
#include "GLYPH.H"
s32 __divsi3(s32 numerator, s32 denominator);

/* Measures the first variant width and greatest variant height. Newlines add
   15 pixels to that variant; only the four spacing slots saturate. Control 1
   advances the variant while retaining its accumulated width and count. */
void UiText_MeasureStringVariant(s32 start, u32 *out_width, u32 *out_height,
                                 u16 *spacing)
{
    u32 line_heights[16];
    struct UiRenderWork *work;
    u16 glyph_counts[4];
    u32 line_count;
    u16 line_widths[4];
    u32 variant;
    u32 max_width;
    u32 code;
    u32 glyph_count;
    u32 width;
    u32 glyph;
    u32 i;
    u32 adjusted_width;
    s32 gap;
#if !EDITION_INTERNATIONAL
    s32 trailing;
#endif

    work = (struct UiRenderWork *)gWindowWork[0];
    line_heights[0] = 15;
    line_heights[1] = 15;
    line_heights[2] = 15;
    line_heights[3] = 15;
    line_heights[4] = 15;
    line_heights[5] = 15;
    line_heights[6] = 15;
    line_heights[7] = 15;
    line_heights[8] = 15;
    line_heights[9] = 15;
    line_heights[10] = 15;
    line_heights[11] = 15;
    line_heights[12] = 15;
    line_heights[13] = 15;
    line_heights[14] = 15;
    line_heights[15] = 15;
    max_width = 0;
    line_count = 0;
    width = 0;
    variant = 0;
    glyph_count = 0;
#if !EDITION_INTERNATIONAL
    trailing = 0;
#endif
    for (;;) {
        code = work->entries[start];
        start = (start + 1) & RENDER_ENTRY_MASK;
        if (code > 31) {
#if EDITION_INTERNATIONAL
            if (code == 32) {
                width += 5;
                glyph_count++;
                continue;
            }
            glyph = UiText_Glyphs[code - 32].width;
            if (work->outline == 1 || work->outline == 5)
                glyph++;
            width += glyph;
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
                    if (work->outline == 1 || work->outline == 5)
                        glyph++;
                    width += glyph;
                }
            }
            glyph_count++;
#endif
            continue;
        }
        switch (code) {
        case 0:
#if EDITION_INTERNATIONAL
            glyph_count++;
#else
            width += trailing;
#endif
            line_widths[line_count] = width;
            glyph_counts[line_count] = glyph_count;
            if (variant == 0 && max_width < width)
                max_width = width;
            variant++;
            goto done;
        case 1:
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            work->outline = 0;
#endif
#if EDITION_INTERNATIONAL
            glyph_count++;
#else
            width += trailing;
#endif
            glyph_counts[line_count] = glyph_count;
            line_widths[line_count] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            variant++;
            break;
        case 3:
#if EDITION_INTERNATIONAL
            glyph_count++;
#else
            width += trailing;
#endif
            glyph_counts[line_count] = glyph_count;
            line_widths[line_count] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            if (line_count <= 2) {
                line_count++;
            }
            *(line_heights + variant) += 15;
            width = 0;
            glyph_count = 0;
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
        case 2:
        case 4:
        case 5:
        case 6:
        case 7:
        case 11:
        case 12:
        case 13:
        case 16:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
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
            *out_height = line_heights[0];
        else if (*out_height < line_heights[i])
            *out_height = line_heights[i];
    }
    *out_width = max_width;
    adjusted_width = (((max_width + 19) >> 3) << 3) - 16;
    if (spacing != 0) {
        i = 0;
        if (i <= line_count) {
            do {
                if (*(glyph_counts + i) <= 1) {
                    *spacing = 0;
                } else {
                    gap = adjusted_width - *(line_widths + i) - 4;
                    if (gap < 0)
                        gap = 0;
                    gap = __divsi3(gap << 8, glyph_counts[i] - 1);
#if !EDITION_INTERNATIONAL
                    if ((u32)gap > 0x800)
                        gap = 0;
                    if (gap > 0x100)
                        gap = 0x100;
#endif
                    *spacing = gap;
                }
                spacing++;
                i++;
            } while (i <= line_count);
        }
    }
}
