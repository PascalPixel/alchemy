#include "TYPES.H"
#include "IWRAM_CALL.H"

struct GlyphWork {
    u8 unknown_000[0xea4];
    u8 fixed_colour;
    u8 unknown_ea5[7];
    u16 outlined;
    u16 colour;
};

extern struct GlyphWork *gWindowWork;
extern u8 UiText_Glyphs[];
extern u8 UiText_SecondGlyphs[];

/* Render a character, and the second glyph its upper bits name, into a
   24-pixel-wide nibble canvas, then pack the canvas into four 4bpp tiles.
   Returns the combined width. */
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
