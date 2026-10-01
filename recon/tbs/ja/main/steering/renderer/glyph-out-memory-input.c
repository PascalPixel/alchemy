/* 2026-10-01: measured Japanese renderer attempt, glyph-out-memory-input.
   Source parser refused this GNU form; approved compiler validation follows..
   Compared with our Japanese ROM and current-tree symbols using the approved
   TBS compiler options. The preserved C attempts change source scheduling or
   allocation; production adoption requires a complete byte-identical build. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "TBS_EDITION.H"

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

extern const u16 UiText_MarkGlyphCodes[];
extern u8 UiText_KanjiGlyphs[];

/* Render a kana, or past code 0xff a kanji ten pixels wide, into a nibble
   canvas, with the voicing mark its top two bits name drawn over the cell
   above-left of it, then pack the canvas into four 4bpp tiles. Returns the
   width. */
s32 UiText_RenderGlyphPair(u32 code, u32 *out)
{
    /* FAKEMATCH: this measured glyph out memory input draft preserves the source
       experiment and its remaining instruction differences stated above. */
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

    /* FAKEMATCH: probe the out-pointer spill before the work load. */
    asm ("" : : "m" (out));
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
            /* FAKEMATCH: assigning the loop's start in the argument makes
               the argument register hold buf + 7 first, as the reference
               does; the assignment after the branches repeats it. */
            Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
            Iwram_ExpandBitRuns(mark, buf + 8, colour);
        }
        length++;
    } else {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        if (mark != 0)
            Iwram_ExpandBitRuns(mark, buf + 24, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        if (mark != 0)
            Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
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
