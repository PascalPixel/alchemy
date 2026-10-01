/* 2026-10-01: the Japanese UiText_RenderGlyphPair, at 080177fc in the
   Japanese ROM, for games/THE BROKEN SEAL/SRC/GRAPHICS/TEXT/
   RENDER_GLYPH_PAIR.C (compile it with the Japanese edition macro). It
   draws a 26-byte kana glyph, or past code 0xff a 24-byte kanji ten pixels
   wide, and lays the voicing mark its top two bits name over the cells
   above-left of it; the two Japanese tables it reads are named here
   UiText_MarkGlyphCodes (080345e0) and UiText_KanjiGlyphs (08033b30). Every
   instruction lines up but two orders: the reference stores the out pointer
   before it loads gWindowWork's address, and on each path that draws the
   mark it computes buf + 7 as the call's argument and copies it into r5 for
   the packing loop, where this build computes r5 first and copies it into
   the argument. alchemy permute (--target tbs-ja, listing made from the
   Japanese ROM) took the score from 510 to 455 in four minutes (7
   register-only, 2 reordered; the 15 operand differences are the listing's
   absolute call targets) by indexing the mark table through pointer
   arithmetic, and found no match. */
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
s32 UiText_RenderGlyphPair(s32 code, u32 *out)
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

    work = gWindowWork;
    mark = 0;
    mark_code = UiText_MarkGlyphCodes[(u32)code >> 14];
    if (mark_code != 0)
        mark = UiText_Glyphs + (mark_code - 32) * 26 + 2;
    code &= 0x3fff;
    if ((u32)code <= 0xff) {
        input = UiText_Glyphs + ((u32)code - 32) * 26;
        length = *(const u16 *)input;
        input += 2;
    } else {
        input = UiText_KanjiGlyphs + ((u32)code - 256) * 24;
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
            Iwram_ExpandBitRuns(mark, buf + 7, colour);
            Iwram_ExpandBitRuns(mark, buf + 8, colour);
        }
        length++;
    } else {
        Iwram_ExpandBitRuns(input, buf + 49, transparent);
        if (mark != 0)
            Iwram_ExpandBitRuns(mark, buf + 24, transparent);
        Iwram_ExpandBitRuns(input, buf + 32, colour);
        if (mark != 0)
            Iwram_ExpandBitRuns(mark, buf + 7, colour);
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
