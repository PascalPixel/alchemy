#include "EDITION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "TBS_EDITION.H"
#include "TEXT_FONT.H"

struct GlyphWork {
    u8 unknown_000[RENDER_MODE_OFS];
    u8 fixed_colour;
    u8 unknown_ea5[7];
    u16 outlined;
    u16 colour;
};

extern struct GlyphWork *gWindowWork;
extern u8 UiText_Glyphs[];
extern u8 UiText_SecondGlyphs[];

/* The Japanese canvas combines a kana or fixed-width kanji with its voicing
   mark. The localized canvas combines two variable-width glyphs. Both pack
   the result into four 4bpp tiles and return its width. */
#if EDITION_INTERNATIONAL
s32 UiText_RenderGlyphPair(s32 code, u32 *out)
{
    u8 buf[384];
    struct GlyphWork *work;
    const u8 *input;
    s32 length;
    s32 colour;
    s32 transparent;
    s32 second_index;
    s32 second_width;
    u8 *digits;
    s32 group;
    s32 row;
    s32 column;
    s32 nibble;
    u32 value;

    work = gWindowWork;
    second_index = (s32)((u32)code << 8) >> 16;
    code &= 255;
    Iwram_ClearWords(buf, 384);
    if (work->fixed_colour != 0) {
        transparent = 0;
        colour = 8;
    } else {
        transparent = 1;
        colour = work->colour;
    }
    input = UiText_Glyphs + ((code - 32) << 5);
    length = *(const u16 *)input;
    input += 2;
    if (work->outlined == 1) {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        Iwram_ExpandBitRuns(input, buf + 50, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        Iwram_ExpandBitRuns(input, buf + 33, colour);
        length++;
    } else {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
    }
    if ((u16)second_index != 0) {
        input = UiText_SecondGlyphs + ((u16)second_index << 5);
        second_width = *(const s16 *)input;
        input += 2;
        if (work->outlined == 1) {
            u8 *dst = buf + length;
            Iwram_ExpandBitRuns(input, dst + 49, transparent);
            Iwram_ExpandBitRuns(input, dst + 50, transparent);
            Iwram_ExpandBitRuns(input, dst + 32, colour);
            Iwram_ExpandBitRuns(input, dst + 33, colour);
            /* FAKEMATCH: the signed halfword width grows through its shifted form. */
            second_width = (s32)(((u32)second_width << 16) + 0x10000) >> 16;
        } else {
            u8 *dst = buf + length;
            u8 *p;
            p = dst + 49;
            Iwram_ExpandBitRuns(input, p, transparent);
            p = dst + 32;
            Iwram_ExpandBitRuns(input, p, colour);
        }
        length += (u16)second_width;
    }

    digits = buf + 7;
    group = 0;
    do {
        row = 0;
        do {
            column = 0;
            do {
                value = 0;
                nibble = 7;
                do {
                    value = (value << 4) + *digits--;
                    nibble--;
                } while (nibble >= 0);
                *out++ = value;
                column++;
                digits += 24;
            } while (column <= 7);
            row++;
            digits -= 120;
        } while (row <= 1);
        group++;
        digits += 112;
    } while (group <= 1);
    return length;
}
#else
s32 UiText_RenderGlyphPair(u32 code, u32 *out)
{
    u8 buf[256];
    struct GlyphWork *work;
    const u8 *input;
    const u8 *mark;
    s32 length;
    s32 colour;
    s32 transparent;
    u32 mark_code;
    u8 *digits;
    s32 group;
    s32 row;
    s32 column;
    s32 nibble;
    u32 value;

    /* FAKEMATCH: the out input schedules its spill before the work-address load; the plain version reverses them. */
    asm ("" : : "r" (out));
    work = gWindowWork;
    mark = 0;
    mark_code = UiText_MarkGlyphCodes[code >> 14];
    if (mark_code != 0)
        mark = UiText_Glyphs + (mark_code - 32) * 26 + 2;
    code &= 0x3fff;
    if (code <= 0xff) {
        input = UiText_Glyphs + (code - 32) * 26;
        length = *(const u16 *)input;
        input += 2;
    } else {
        input = UiText_KanjiGlyphs + (code - 256) * 24;
        length = 10;
    }
    Iwram_ClearWords(buf, 256);
    if (work->fixed_colour != 0) {
        transparent = 0;
        colour = 8;
    } else {
        transparent = 1;
        colour = work->colour;
    }
    if (work->outlined == 1) {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        Iwram_ExpandBitRuns(input, buf + 50, transparent);
        if (mark != 0) {
            Iwram_ExpandBitRuns(mark, buf + 24, transparent);
            Iwram_ExpandBitRuns(mark, buf + 25, transparent);
        }
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        Iwram_ExpandBitRuns(input, buf + 33, colour);
        if (mark != 0) {
            /* FAKEMATCH: assigning digits in the mark argument makes the
               argument register compute buf + 7 before its loop copy. */
            Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
            Iwram_ExpandBitRuns(mark, buf + 8, colour);
        }
        length++;
    } else {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        if (mark != 0)
            Iwram_ExpandBitRuns(mark, buf + 24, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        if (mark != 0) {
            /* FAKEMATCH: this mark path needs the same argument-before-loop
               order as the outlined path. */
            Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
        }
    }

    digits = buf + 7;
    group = 0;
    do {
        row = 0;
        do {
            column = 0;
            do {
                value = 0;
                nibble = 7;
                do {
                    value = (value << 4) + *digits--;
                    nibble--;
                } while (nibble >= 0);
                *out++ = value;
                column++;
                digits += 24;
            } while (column <= 7);
            row++;
            digits -= 120;
        } while (row <= 1);
        group++;
        digits += 112;
    } while (group <= 1);
    return length;
}
#endif
