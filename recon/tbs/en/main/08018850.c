/* Current draft (2026-10-03), not exact: EN score 1310, 55 differing rows
   (29 register, 14 operand, 9 reordered, 3 inserted). EN object .text is
   518 bytes against the 512-byte listing; no linked extent is proved.
   Later 2026-10-03 bounded ownership trials, each cumulative:
   H1 reversed array declarations and stored count before width at both
   boundaries: score 2680/90, .text 510. Scratch slots became native but
   allocation diverged. H2 used one rounded-width accumulator and wrote
   height before rounding: 2210/80, .text 510. H3 made both scratch reads
   indexed: 2309/83, .text 506. It obtained one shared offset, but moved
   the loop counter to r7, lost the sp+4 counter spill and shrank the frame
   to 28. All three were worse; the truthful 1310/55 form is retained.
   Its remaining differences are scratch-slot order, width/count register
   assignment, finalization order and spacing-loop allocation. Its outline
   store at sp+0 and 32-byte frame already agree with the native routine.
   Uses WINDOW.H's UiRenderWork and GLYPH.H's FontGlyph. The nullable fourth
   argument is a u16 spacing output. Height is not limited to four lines.
   Own-ROM edition branches preserve Japanese character spacing, trailing
   width adjustments and spacing caps; control 1 clears outline only in
   ES/FR/IT, and render mode widens only JA/EN.
   All six branches compile. Object .text/native extent, in JA/EN/DE/ES/FR/IT
   order: 564/572, 518/512, 494/492, 502/504, 502/504, 502/504.
   Non-English byte matching remains unproved. Earlier trials follow. */
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
#include "WINDOW.H"
#include "GLYPH.H"

s32 __divsi3(s32 numerator, s32 denominator);

/* Measures the widest queued line and total height, adding 15 for every line.
   Only the four spacing slots saturate. Optional 8.8 spacing is between words
   internationally and between characters in Japanese. */
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
    u16 widths[4];
    s32 gap;
    u32 style;
    u16 counts[4];
    u32 tmp;
    u32 tmp2;
#if !EDITION_INTERNATIONAL
    s32 trailing;
#endif

    work = (struct UiRenderWork *)gWindowWork[0];
    height = 15;
    count = 0;
    width = 0;
    lines = 0;
    line_width = 0;
#if !EDITION_INTERNATIONAL
    trailing = 0;
#endif
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
        case 1:
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
            work->outline = 0;
#endif
        case 0:
            goto done;
        case 3:
#if EDITION_INTERNATIONAL
            count++;
#else
            line_width += trailing;
#endif
            widths[lines] = line_width;
            counts[lines] = count;
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
#if !EDITION_INTERNATIONAL
    line_width += trailing;
#endif
    widths[lines] = line_width;
    if (width < (tmp = line_width))
        width = line_width;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
    if (work->mode)
        width = width + 2;
#endif
    *out_width = width;
#if EDITION_INTERNATIONAL
    count++;
#endif
    counts[lines] = count;
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
#if EDITION_INTERNATIONAL
                if ((u32)gap > 0xc00)
                    gap = 0x200;
#else
                if ((u32)gap > 0x800)
                    gap = 0;
                if (gap > 0x100)
                    gap = 0x100;
#endif
                *spacing = gap;
            }
            spacing++;
            i++;
        }
    }
}
