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
extern struct GlyphInfo Data_08032224[];
s32 Math_Div(s32 numerator, s32 denominator);

void UiText_MeasureStringVariant(s32 start, s32 *out_width, s32 *out_height,
                                 u16 *spacing)
{
    struct TextWork *work;
    s32 line_heights[16];
    u16 glyph_counts[4];
    u16 line_widths[4];
    u32 line_count;
    s32 line_offset;
    u32 variant;
    u32 max_width;
    s32 glyph_count;
    s32 width;
    u32 code;
    s32 glyph;
    u32 i;
    s32 gap;
    s32 adjusted_width;

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
    line_heights[13] = 15;
    line_heights[14] = 15;
    line_heights[15] = 15;
    line_count = 0;
    line_offset = 0;
    variant = 0;
    max_width = 0;
    glyph_count = 0;
    width = 0;

    for (;;) {
        code = work->entries[start];
        start = (start + 1) & 0x1ff;
        if (code > 31) {
            if (code == 32) {
                width += 5;
                glyph_count++;
                continue;
            }
            glyph = Data_08032224[code - 32].width;
            if (work->style == 1 || work->style == 5)
                glyph++;
            width += glyph;
            continue;
        }

        switch (code) {
        case 0:
            glyph_counts[line_offset >> 1] = glyph_count + 1;
            line_widths[line_offset >> 1] = width;
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
            line_heights[variant] += 15;
            glyph_count = 0;
            width = 0;
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
        for (i = 0; i < variant; i++) {
            if (i == 0 || (u32)*out_height < (u32)line_heights[i])
                *out_height = line_heights[i];
        }
    }
    *out_width = max_width;
    adjusted_width = ((max_width + 19) >> 3 << 3) - 16;

    if (spacing != 0) {
        for (i = 0; i <= line_count; i++) {
            if (glyph_counts[i] <= 1) {
                *spacing = 0;
            } else {
                gap = adjusted_width - line_widths[i] - 4;
                if (gap < 0)
                    gap = 0;
                *spacing = (u16)Math_Div(gap << 8, glyph_counts[i] - 1);
            }
            spacing++;
        }
    }
}
