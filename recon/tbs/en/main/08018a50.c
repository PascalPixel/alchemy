/* 2026-09-29: eight minutes of permutation: 5510 -> 4770 (41 register-only,
 * 29 operand, 16 reordered, 16 inserted, 14 deleted), kept in natural form:
 * locals reordered with variant as a register local, max_width and width
 * cleared before the line setup, line 13 and the variant's line height
 * reached through pointer arithmetic, and the height and spacing loops
 * rotated into tested do/while form with the spacing tables read through
 * pointers. */
/* Draft, not exact: 600 of 604 bytes, 249 differing halfwords.
   Complete owner: [0x08018a50, 0x08018cac). Control code 0 finishes,
   1 advances the variant, and 3 adds a line. The indirect dispatch,
   register allocation and parse/spacing loop shapes remain different. */
#include "TYPES.H"

struct GlyphInfo {
    u16 width;
    u8 unknown_02[30];
};

struct TextWork {
    u8 unknown_000[0xea4];
    u8 framed;
    u8 unknown_ea5[7];
    u16 style;
    u8 unknown_eae[2];
    u16 entries[0x200];
};

extern u8 *gWindowWork;
extern struct GlyphInfo UiText_Glyphs[];
s32 __divsi3(s32 numerator, s32 denominator);

void UiText_MeasureStringVariant(s32 start, s32 *out_width, s32 *out_height,
                                 u16 *spacing)
{
    s32 line_heights[16];
    struct TextWork *work;
    u16 line_widths[4];
    u32 line_count;
    u16 glyph_counts[4];
    s32 line_offset;
    register u32 variant;
    u32 max_width;
    u32 code;
    s32 glyph_count;
    s32 width;
    s32 glyph;
    u32 i;
    s32 adjusted_width;
    s32 gap;

    work = (struct TextWork *)gWindowWork;
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
    max_width = 0;
    *(line_heights + 13) = 15;
    line_heights[14] = 15;
    line_heights[15] = 15;
    line_count = 0;
    line_offset = 0;
    width = 0;
    variant = 0;
    glyph_count = 0;
    for (;;) {
        code = work->entries[start];
        start = (start + 1) & 0x1ff;
        if (code > 31) {
            if (code == 32) {
                width += 5;
                glyph_count++;
                continue;
            }
            glyph = UiText_Glyphs[code - 32].width;
            if (work->style == 1 || work->style == 5)
                glyph++;
            width += glyph;
            continue;
        }
        switch (code) {
        case 0:
            line_widths[line_offset >> 1] = width;
            glyph_counts[line_offset >> 1] = glyph_count + 1;
            if (variant == 0 && max_width < width)
                max_width = width;
            variant++;
            goto done;
        case 1:
            glyph_counts[line_offset >> 1] = glyph_count + 1;
            line_widths[line_offset >> 1] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            variant++;
            break;
        case 3:
            glyph_counts[line_offset >> 1] = glyph_count + 1;
            line_widths[line_offset >> 1] = width;
            if (variant == 0 && max_width < width)
                max_width = width;
            if (line_count <= 2) {
                line_count++;
                line_offset = line_count * 2;
            }
            *(line_heights + variant) += 15;
            width = 0;
            glyph_count = 0;
            break;
        case 14:
        case 28:
            start = (start + 1) & 0x1ff;
            /* fall through */
        case 8:
        case 10:
        case 15:
        case 17:
            start = (start + 1) & 0x1ff;
            break;
        case 9:
            work->style = work->entries[start];
            start = (start + 1) & 0x1ff;
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
    if (work->framed != 0)
        max_width += 2;
    if (variant != 0) {
        i = 0;
        if (i < variant) {
            while (1) {
                if (i == 0 || (u32)*out_height < (u32)line_heights[i])
                    *out_height = line_heights[i];
                i++;
                if (i >= variant)
                    break;
            }
        }
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
                    *spacing = (u16)__divsi3(gap << 8, glyph_counts[i] - 1);
                }
                spacing++;
                i++;
            } while (i <= line_count);
        }
    }
}
