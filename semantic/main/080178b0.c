#include "types.h"

/*
 * Render one encoded field-text character pair into a 4bpp 16-by-16 tile
 * block.
 *
 * The caller (Func_08018cac) passes a packed character code and a 128 byte
 * tile buffer.  The low byte selects the primary glyph, the high byte an
 * optional second glyph drawn immediately after it.  Both glyphs are painted
 * into a linear 16-pixel-wide scratch bitmap, first as a drop shadow one row
 * down and one column right, then in the foreground colour.  The scratch
 * bitmap is finally packed into the four 8-by-8 tiles the object engine
 * expects.  The return value is the combined advance width, which the caller
 * treats as a non-zero success code.
 */

/*
 * Font entries are 32 bytes: a leading advance width followed by the packed
 * glyph rows the renderer veneer consumes.  The assembly reaches the primary
 * glyph through 0x08032224 with a (code - 32) index; that is the same as
 * indexing this table by the raw code, because 0x08032224 == 0x08031e24 +
 * 32 * 32.
 */
struct FontEntry_080178b0 {
    u16 advance;
    u8 rows[30];
};

/* Global engine state block; the same pointer Func_08018cac reads. */
#define STATE_080178B0 (*(u8 **)0x03001e8c)

#define FONT_080178B0 ((const struct FontEntry_080178b0 *)0x08031e24)

/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register.
 *
 * THERE ARE NOT TWO GLYPH BLITTERS.  This file previously read 0x080072f8 and
 * 0x080072fc as "the two glyph blitters" and filed an explicit Uncertainty
 * about the asymmetry -- why the second glyph used the neighbouring veneer
 * where the first used 0x080072f8.  That uncertainty dissolves: they are
 * `__call_via_r5` and `__call_via_r6`, and at all twelve sites in this
 * function BOTH hold the same pooled value, 0x03000214.  There is one
 * blitter, called through whichever register the compiler had the address in.
 * The asymmetry is a register-allocation detail with no meaning in the
 * source, and the Uncertainty it produced is withdrawn rather than preserved.
 *
 * The other veneer here is real and was already understood: the site at
 * 0x080178da is `__call_via_r3` dispatching the pooled 0x03000164, and this
 * file's own note that the assembly never initialises the third argument
 * still stands.  0x03000164 remains NOT established -- see the audit note.
 *
 * The blitter's argument shape is unchanged from the previous draft and was
 * re-checked against the ROM at 0x08017920: r0 the glyph, r1 the destination,
 * r2 the colour.  Only the callee was wrong.
 */
typedef void (*Resident_03000164)(void *destination, s32 size, s32 value);
typedef void (*GlyphBlit_03000214)(const void *glyph, void *destination,
                                   s32 colour);

/* Scratch bitmap geometry: 16 pixels per row, one byte per pixel. */
#define BITMAP_STRIDE_080178B0 16
#define BITMAP_BYTES_080178B0  384

/*
 * Glyph origin inside the scratch bitmap: the foreground copy starts at row 2
 * column 0, the shadow copy one row down and one column right.
 */
#define GLYPH_ORIGIN_080178B0 32
#define SHADOW_ORIGIN_080178B0 49

s32 Func_080178b0(s32 code, void *tile_data)
{
    u8 bitmap[BITMAP_BYTES_080178B0];
    u32 *output = (u32 *)tile_data;
    const u8 *state = STATE_080178B0;
    const struct FontEntry_080178b0 *entry;
    const void *glyph;
    u16 mode;
    s32 shadow_colour;
    s32 text_colour;
    u32 advance;
    s32 second_code;
    const u8 *source;
    s32 tile_row;
    s32 tile_column;
    s32 row;

    /*
     * The primary code is the low byte; the secondary code is a sign extended
     * halfword taken from bits 8..23 of the argument.  Callers pass a 16 bit
     * value, so in practice this is the high byte.
     */
    second_code = ((s32)((u32)code << 8)) >> 16;

    ((Resident_03000164)0x03000164)(bitmap, BITMAP_BYTES_080178B0, 0);

    if (*(const u8 *)(state + 0xea4) != 0) {
        text_colour = 8;
        shadow_colour = 0;
    } else {
        text_colour = *(const u16 *)(state + 0xeae);
        shadow_colour = 1;
    }

    entry = &FONT_080178B0[code & 0xff];
    advance = entry->advance;
    glyph = entry->rows;

    mode = *(const u16 *)(state + 0xeac);
    if (mode == 1) {
        /*
         * Emphasised text: every copy is drawn twice, one pixel apart, and
         * the advance grows by one pixel to match.
         */
        ((GlyphBlit_03000214)0x03000214)(glyph, &bitmap[SHADOW_ORIGIN_080178B0],
                      shadow_colour);
        ((GlyphBlit_03000214)0x03000214)(glyph, &bitmap[SHADOW_ORIGIN_080178B0 + 1],
                      shadow_colour);
        ((GlyphBlit_03000214)0x03000214)(glyph, &bitmap[GLYPH_ORIGIN_080178B0], text_colour);
        ((GlyphBlit_03000214)0x03000214)(glyph, &bitmap[GLYPH_ORIGIN_080178B0 + 1], text_colour);
        advance += 1;
    } else {
        ((GlyphBlit_03000214)0x03000214)(glyph, &bitmap[SHADOW_ORIGIN_080178B0],
                      shadow_colour);
        ((GlyphBlit_03000214)0x03000214)(glyph, &bitmap[GLYPH_ORIGIN_080178B0], text_colour);
    }

    if ((u16)second_code != 0) {
        u8 *slot = &bitmap[advance];
        s32 second_advance;

        entry = &FONT_080178B0[(u16)second_code];
        /*
         * Uncertainty: the second glyph reads its advance as a signed
         * halfword while the first reads it unsigned.  Both are reproduced
         * literally; no font entry observed so far carries a negative width.
         */
        second_advance = *(const s16 *)&entry->advance;
        glyph = entry->rows;

        mode = *(const u16 *)(state + 0xeac);
        if (mode == 1) {
            ((GlyphBlit_03000214)0x03000214)(glyph, slot + SHADOW_ORIGIN_080178B0,
                          shadow_colour);
            ((GlyphBlit_03000214)0x03000214)(glyph, slot + SHADOW_ORIGIN_080178B0 + 1,
                          shadow_colour);
            ((GlyphBlit_03000214)0x03000214)(glyph, slot + GLYPH_ORIGIN_080178B0, text_colour);
            ((GlyphBlit_03000214)0x03000214)(glyph, slot + GLYPH_ORIGIN_080178B0 + 1,
                          text_colour);
            second_advance = (s16)(second_advance + 1);
        } else {
            /*
             * The veneer asymmetry that used to be flagged here is resolved:
             * both entries dispatch the same pooled 0x03000214.  See header.
             */
            ((GlyphBlit_03000214)0x03000214)(glyph, slot + SHADOW_ORIGIN_080178B0,
                          shadow_colour);
            ((GlyphBlit_03000214)0x03000214)(glyph, slot + GLYPH_ORIGIN_080178B0, text_colour);
        }

        advance += (u16)second_advance;
    }

    /*
     * Pack the top 16 rows of the scratch bitmap into four 8-by-8 4bpp tiles,
     * ordered top-left, top-right, bottom-left, bottom-right.  Each output
     * word holds eight pixels with the leftmost in the low nibble, so the
     * source is walked backwards across each 8 pixel group.
     */
    source = &bitmap[7];
    for (tile_row = 0; tile_row <= 1; tile_row++) {
        for (tile_column = 0; tile_column <= 1; tile_column++) {
            for (row = 0; row <= 7; row++) {
                u32 packed = 0;
                s32 pixel;

                for (pixel = 7; pixel >= 0; pixel--) {
                    packed = (packed << 4) + *source;
                    source--;
                }
                *output++ = packed;
                /* Back to the same 8 pixel column on the next bitmap row. */
                source += 8 + BITMAP_STRIDE_080178B0;
            }
            /* Step back to the next 8 pixel column of the same tile row. */
            source -= 120;
        }
        /* Advance to the tile row eight bitmap rows further down. */
        source += 112;
    }

    return (s32)advance;
}
