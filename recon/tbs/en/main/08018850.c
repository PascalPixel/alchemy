/* Draft, not exact (2026-09-25): 504 of 512 bytes, 164 differing halfwords.
   Split from its listing (which also held UiText_MeasureStringVariant and
   UiText_DrawGlyph) and written from it; the entry walk, the control-code
   switch and the spacing loop line up.
   Remaining: the ROM stores the glyph style halfword to a stack slot it
   never reads (a 32-byte frame; here 28), keeps the widest line in r7, the
   height in fp, the line count in sl and its halfword offset in ip, and
   reloads the jump-table base into lr after each line break. */
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

/* Measures the queued message starting at entry pos: the widest line and
   the total height (15 per line, up to four lines), and optionally the
   per-glyph spacing (8.8) that would justify each line to the window. */
void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height, u16 *spacing)
{
    struct TextWork *work;
    u32 lines;
    u32 height;
    u32 width;
    u32 count;
    u32 line_width;
    u32 c;
    u32 glyph;
    u32 i;
    s32 gap;
    s16 zero;
    u32 style;
    u16 counts[4];
    u16 widths[4];

    work = (struct TextWork *)gWindowWork;
    lines = 0;
    height = 15;
    width = 0;
    count = 0;
    line_width = 0;
    for (;;) {
        c = work->entries[pos];
        pos = (pos + 1) & 0x1ff;
        if (c > 31) {
            if (c == 32) {
                line_width += 5;
                count++;
            } else {
                glyph = Data_08032224[c - 32].width;
                style = work->style;
                if (style == 1 || style == 5)
                    glyph++;
                line_width += glyph;
            }
            continue;
        }
        switch (c) {
        case 0:
        case 1:
            goto done;
        case 3:
            counts[lines] = ++count;
            widths[lines] = line_width;
            if (width < line_width)
                width = line_width;
            if (lines < 3)
                lines++;
            height += 15;
            count = 0;
            line_width = 0;
            break;
        case 14:
        case 28:
            pos = (pos + 1) & 0x1ff;
        case 8:
        case 10:
        case 15:
        case 17:
            pos = (pos + 1) & 0x1ff;
            break;
        case 9:
            work->style = work->entries[pos];
            pos = (pos + 1) & 0x1ff;
            break;
        }
    }
done:
    counts[lines] = ++count;
    widths[lines] = line_width;
    if (width < line_width)
        width = line_width;
    if (work->framed != 0)
        width += 2;
    *out_width = width;
    *out_height = height;
    width = ((width + 19) >> 3 << 3) - 16;
    if (spacing != NULL) {
        for (i = 0; i <= lines; i++) {
            if (counts[i] <= 1) {
                zero = 0;
                *spacing = zero;
            } else {
                gap = width - widths[i] - 4;
                if (gap < 0)
                    gap = 0;
                gap = Math_Div(gap << 8, counts[i] - 1);
                if ((u32)gap > 0xc00)
                    gap = 0x200;
                *spacing = gap;
            }
            spacing++;
        }
    }
}
