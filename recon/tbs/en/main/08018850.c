/* Draft, not exact (2026-09-25): 504 of 512 bytes, 164 differing halfwords.
   Split from its listing (which also held UiText_MeasureStringVariant and
   UiText_DrawGlyph) and written from it; the entry walk, the control-code
   switch and the spacing loop line up.
   Remaining: the ROM stores the glyph style halfword to a stack slot it
   never reads (a 32-byte frame; here 28), keeps the widest line in r7, the
   height in fp, the line count in sl and its halfword offset in ip, and
   reloads the jump-table base into lr after each line break.
   2026-09-29: alchemy permute took the score from 2754 to 1310 (29
   register-only, 14 operand, 9 reordered, 3 inserted) in six minutes;
   rerun and minimized, 19 of its 32 changed regions matter: declaration
   and statement order, a while loop over the spacings, the final count
   stored after the width, and a word temporary shared by the widest-line
   test and the tile rounding (35 without it). The count reads must stay
   *(counts + i): counts[i] scores 1917. */
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

/* Measures the queued message starting at entry pos: the widest line and
   the total height (15 per line, up to four lines), and optionally the
   per-glyph spacing (8.8) that would justify each line to the window. */
void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height, u16 *spacing)
{
    struct TextWork *work;
    u32 lines;
    u32 width;
    u32 height;
    u32 line_width;
    u32 count;
    u32 c;
    u32 glyph;
    u32 i;
    u16 widths[4];
    s32 gap;
    u32 style;
    u16 counts[4];
    u32 tmp;
    u32 tmp2;

    work = (struct TextWork *)gWindowWork;
    height = 15;
    count = 0;
    width = 0;
    lines = 0;
    line_width = 0;
    for (;;) {
        c = work->entries[pos];
        pos = (pos + 1) & 0x1ff;
        if (c > 31) {
            if (c == 32) {
                line_width += 5;
                count++;
            } else {
                glyph = UiText_Glyphs[c - 32].width;
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
            widths[lines] = line_width;
            counts[lines] = ++count;
            if (width < line_width)
                width = line_width;
            if (lines < 3)
                lines++;
            line_width = 0;
            count = 0;
            height += 15;
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
    widths[lines] = line_width;
    if (width < (tmp = line_width))
        width = line_width;
    if (work->framed)
        width = width + 2;
    *out_width = width;
    counts[lines] = ++count;
    tmp = (width + 19) >> 3;
    tmp2 = (tmp << 3) - 16;
    *out_height = height;
    width = tmp2;
    if (spacing != NULL) {
        i = 0;
        while (i <= lines) {
            if (*(counts + i) <= 1) {
                *spacing = 0;
            } else {
                gap = width - widths[i] - 4;
                if (gap < 0)
                    gap = 0;
                gap = __divsi3(gap << 8, *(counts + i) - 1);
                if ((u32)gap > 0xc00)
                    gap = 0x200;
                *spacing = gap;
            }
            spacing++;
            i++;
        }
    }
}
