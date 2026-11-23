/// @file
/// Characters rendered into tiles, string widths and number formatting.
// Part of window.c.

#include "window_internal.h"

/// Text_DrawGlyphPair draws its glyphs into a buffer of one byte per pixel
/// (Iwram_ExpandBitRuns writes a glyph's rows there in a colour), in rows 16
/// bytes apart, then packs its top 16 rows into 4bpp tiles.
/// A glyph is drawn from row 2, its shadow a pixel right of and below it, and
/// a kana's mark at the top right.
#define GLYPH_BUFFER_WIDTH 16
#define GLYPH_PIXEL(x, y) ((y) * GLYPH_BUFFER_WIDTH + (x))
/// The pixels across a tile, which the packing reads a row at a time.
#define GLYPH_TILE_WIDTH 8

/// The first character of a pair, in the low byte of its code, and the second,
/// in bits 8 to 23.
#define GLYPH_PAIR_FIRST(code) ((code) & 0xff)
#define GLYPH_PAIR_SECOND(code) (((code) << 8) >> 16)

/// The code a character past the font is measured as: '@' in the font's
/// Latin-1.
#define TEXT_PAST_FONT_CODE 64

/// The colour of Mind Read's text, which has no shadow.
#define MIND_READ_TEXT_COLOR 8
/// The colour of a glyph's shadow, a pixel right of and below it.
#define GLYPH_SHADOW_COLOR 1

#if LANGUAGE != LANGUAGE_JAPANESE
/// Draws a character, and a second one when the code carries it in bits 8 to
/// 15, into a 16x16-pixel cell of four 4bpp tiles at out: in the text colour
/// over a shadow, or outlined in TEXT_STYLE_OUTLINED. Returns the pixels the
/// pair takes.
s32 Text_DrawGlyphPair(s32 code, u32 *out) {
  // 24 rows, of which the top 16 are packed.
  u8 pixels[24 * GLYPH_BUFFER_WIDTH];
  struct WindowWork *work;
  const u8 *glyph;
  s32 width;
  s32 colour;
  s32 shadowColour;
  s32 firstIndex;
  s32 secondIndex;
  u16 secondWidth;
  u8 *pixel;
  s32 tileColumn;
  s32 tileRow;
  s32 line;
  s32 pixelsLeft;
  u32 tileLine;

  work = WINDOW_WORK;
  secondIndex = GLYPH_PAIR_SECOND(code);
  code = GLYPH_PAIR_FIRST(code);
  Iwram_ClearWords(pixels, sizeof(pixels));
  if (work->mindRead != 0) {
    shadowColour = 0;
    colour = MIND_READ_TEXT_COLOR;
  } else {
    shadowColour = GLYPH_SHADOW_COLOR;
    colour = work->colour;
  }
  firstIndex = code - TEXT_SPACE;
  glyph = firstIndex * sizeof(struct FontGlyph) + (const u8 *)gFont_Glyphs;
  width = ((const struct FontGlyph *)glyph)->width;
  glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  // Outlined text is the glyph drawn twice a pixel apart, over its shadow
  // drawn the same way: one pixel wider.
  if (work->outline == TEXT_STYLE_OUTLINED) {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
  }
  // The second character follows the first, starting width pixels along.
  if ((u16)secondIndex != 0) {
#ifdef UBFIX
    // Form only the actual glyph address: &glyphs[-TEXT_SPACE] is already
    // outside the array before the later byte offset is added.
    glyph = (const u8 *)&gFont_Glyphs[(u16)secondIndex - TEXT_SPACE];
#else
    glyph = (const u8 *)&gFont_Glyphs[-TEXT_SPACE] + ((u16)secondIndex << 5);
#endif
    secondWidth = *(const u16 *)glyph;
    glyph += 2;
    if (work->outline == TEXT_STYLE_OUTLINED) {
      u8 *dst = pixels + width;
      Iwram_ExpandBitRuns(glyph, dst + GLYPH_PIXEL(1, 3), shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + GLYPH_PIXEL(2, 3), shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + GLYPH_PIXEL(0, 2), colour);
      Iwram_ExpandBitRuns(glyph, dst + GLYPH_PIXEL(1, 2), colour);
      secondWidth += 1;
    } else {
      u8 *dst = pixels + width;
      u8 *p;
      p = dst + GLYPH_PIXEL(1, 3);
      Iwram_ExpandBitRuns(glyph, p, shadowColour);
      p = dst + GLYPH_PIXEL(0, 2);
      Iwram_ExpandBitRuns(glyph, p, colour);
    }
    width += secondWidth;
  }

#else
/// Draws a character into a 16x16-pixel cell of four 4bpp tiles at out: in the
/// text colour over a shadow, or outlined in TEXT_STYLE_OUTLINED, with the kana
/// mark in the code's top two bits above its top right. Returns its width.
s32 Text_DrawGlyphPair(u32 code, u32 *out) {
  u8 pixels[GLYPH_BUFFER_WIDTH * GLYPH_BUFFER_WIDTH];
  struct WindowWork *work;
  const u8 *glyph;
  const u8 *markGlyph;
  s32 width;
  s32 colour;
  s32 shadowColour;
  u32 markCode;
  u8 *pixel;
  s32 tileColumn;
  s32 tileRow;
  s32 line;
  s32 pixelsLeft;
  u32 tileLine;

  work = WINDOW_WORK;
  markGlyph = 0;
  markCode = gFont_MarkGlyphCodes[code >> TEXT_CODE_MARK_SHIFT];
  if (markCode != 0)
    markGlyph =
        (const u8 *)((const struct FontGlyph *)((const u8 *)gFont_Glyphs +
                                                (markCode - TEXT_SPACE) *
                                                    sizeof(struct FontGlyph)))
            ->rows;
  code &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
  if (code <= TEXT_LAST_FONT_CODE) {
    glyph = (const u8 *)gFont_Glyphs +
            (code - TEXT_SPACE) * sizeof(struct FontGlyph);
    width = ((const struct FontGlyph *)glyph)->width;
    glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  } else {
    glyph = gFont_KanjiGlyphs +
            (code - (TEXT_LAST_FONT_CODE + 1)) * KANJI_GLYPH_SIZE;
    width = KANJI_WIDTH;
  }
  Iwram_ClearWords(pixels, sizeof(pixels));
  if (work->mindRead != 0) {
    shadowColour = 0;
    colour = MIND_READ_TEXT_COLOR;
  } else {
    shadowColour = GLYPH_SHADOW_COLOR;
    colour = work->colour;
  }
  if (work->outline == TEXT_STYLE_OUTLINED) {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
    if (markGlyph != 0) {
      Iwram_ExpandBitRuns(markGlyph, pixels + GLYPH_PIXEL(8, 1), shadowColour);
      Iwram_ExpandBitRuns(markGlyph, pixels + GLYPH_PIXEL(9, 1), shadowColour);
    }
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
    if (markGlyph != 0) {
      pixel = pixels + GLYPH_PIXEL(7, 0);
      Iwram_ExpandBitRuns(markGlyph, pixel, colour);
      Iwram_ExpandBitRuns(markGlyph, pixel + 1, colour);
    }
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    if (markGlyph != 0)
      Iwram_ExpandBitRuns(markGlyph, pixels + GLYPH_PIXEL(8, 1), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
    if (markGlyph != 0) {
      pixel = pixels + GLYPH_PIXEL(7, 0);
      Iwram_ExpandBitRuns(markGlyph, pixel, colour);
    }
  }
#endif

  // Pack either font into four tiles, left to right and then down, with the
  // leftmost pixel in the low nibble: each tile row is read right to left, so
  // the next starts a buffer row on and a tile's width back.
  pixel = pixels + GLYPH_TILE_WIDTH - 1;
  tileRow = 0;
  do {
    tileColumn = 0;
    do {
      line = 0;
      do {
        tileLine = 0;
        for (pixelsLeft = 0; pixelsLeft < GLYPH_TILE_WIDTH; pixelsLeft++) {
          tileLine = (tileLine << 4) + *pixel--;
        }
        *out++ = tileLine;
        line++;
        pixel += GLYPH_BUFFER_WIDTH + GLYPH_TILE_WIDTH;
      } while (line <= 7);
      tileColumn++;
      // Back up to the top row, at the next tile's right edge.
      pixel -= GLYPH_TILE_WIDTH * GLYPH_BUFFER_WIDTH - GLYPH_TILE_WIDTH;
    } while (tileColumn <= 1);
    tileRow++;
    // Down to the next row of tiles, at the first tile's right edge.
    pixel += GLYPH_TILE_WIDTH * GLYPH_BUFFER_WIDTH - 2 * GLYPH_TILE_WIDTH;
  } while (tileRow <= 1);
  return width;
}

/// From the German release on, every code up to TEXT_LAST_FONT_CODE counts as
/// 10 pixels, and those above as nothing.
#if TBS_PATCH_2
#define WIDE_WIDTH_FIXED 1
#endif

/// The main font's glyph n, counted from the space character.
#define FONT_GLYPH(n)                                                          \
  ((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) +                 \
                              (const u8 *)gFont_Glyphs))

/// Returns the width in pixels of a zero-terminated string of 16-bit character
/// codes: 4 for a space, and for other codes up to TEXT_LAST_FONT_CODE the font
/// width, but nothing for the kana marks; 10 for a kanji (any code above it).
s32 Text_GetWideStringWidth(u16 *text) {
  s32 width;
  u16 *cursor;
  u32 code;
#if !defined(WIDE_WIDTH_FIXED)
  u32 glyphIndex;
#endif

  code = *text;
  width = 0;
  cursor = text + 1;
  while (code != 0) {
    if (code == TEXT_SPACE) {
      width += 4;
    } else if (code <= TEXT_LAST_FONT_CODE) {
#if defined(WIDE_WIDTH_FIXED)
      width += 10;
#else
      glyphIndex = code - TEXT_MARK_VOICED;
      if (glyphIndex > 1U) {
        // The font starts at the space.
        glyphIndex += TEXT_MARK_VOICED - TEXT_SPACE;
        width += FONT_GLYPH(glyphIndex)->width;
      }
    } else {
      width += KANJI_WIDTH;
#endif
    }
    code = *cursor;
    cursor += 1;
  }
  return width;
}

/// Draws a zero-terminated string of 16-bit character codes into a text canvas
/// window at pixel (x, y), running its colour, outline, line spacing and new
/// line codes; other control codes are skipped with their arguments.
void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y) {
  u8 *base;
  struct WindowWork *work;
  u32 code;
#if LANGUAGE != LANGUAGE_JAPANESE
  u32 nextCode;
#endif
  s16 lineStartX;

  base = gWindowWork[0];
  work = (struct WindowWork *)base;
  code = 0;
  lineStartX = x;
  if (text == NULL) {
    text = work->textBuffer;
    work->textBuffer[work->textBufferEnd] = code;
    work->textBufferEnd = (work->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  }
#if LANGUAGE != LANGUAGE_JAPANESE
  for (;;)
#else
  while ((code = *text++) != 0)
#endif
  {
#if LANGUAGE != LANGUAGE_JAPANESE
    code = *text++;
    if (code > TEXT_LAST_FONT_CODE)
      code = TEXT_PAST_FONT_CODE;
    if (code == 0)
      break;
#endif
    // Colour, style, line spacing and new lines take effect; the other control
    // codes are skipped with their operands.
    if (code <= TEXT_END_ALT) {
      switch (code) {
      case TEXT_COLOR:
        work->colour = *text;
        text++;
        break;
      case TEXT_STYLE:
        work->outline = *text;
        text++;
        break;
      case TEXT_LINE_SPACING:
        work->lineSpacing = *text;
        text++;
        break;
      case TEXT_RESET_STYLE:
        Text_ResetStyle();
        break;
      case TEXT_NEWLINE:
        x = lineStartX;
        y += TEXT_LINE_HEIGHT;
        break;
      case TEXT_UNUSED_14:
      case TEXT_WINDOW_STATE:
      case TEXT_ACTOR_NAME_START:
        text++;
        // Skip the second operand too.
        // Fall through.
      case TEXT_UNUSED_11:
      case TEXT_UNUSED_12:
      case TEXT_ACTOR_NAME:
#if LANGUAGE != LANGUAGE_JAPANESE
      case TEXT_ARTICLE_CLASS:
#endif
        text++;
        break;
      }
    } else {
#if LANGUAGE != LANGUAGE_JAPANESE
      // Pair two narrow characters into one cell, as Message_Type
      // does.
      if ((window->flags & WINDOW_FLAG_TEXT_CANVAS) == 0) {
        nextCode = *text;
#if LANGUAGE == LANGUAGE_SPANISH
        if (code > TEXT_SPACE && nextCode > TEXT_SPACE) {
          s16 width = gFont_Glyphs[code - TEXT_SPACE].width +
                      gFont_Glyphs[nextCode - TEXT_SPACE].width;

          if (work->outline == TEXT_STYLE_OUTLINED) {
            if ((u16)width <= GLYPH_PAIR_OUTLINED_WIDTH_MAX) {
              code |= nextCode << 8;
              text++;
            }
          } else if ((u16)width <= GLYPH_PAIR_WIDTH_MAX) {
            code |= nextCode << 8;
            text++;
          }
        }
#else
        if (code > TEXT_SPACE && nextCode > TEXT_SPACE &&
            (u16)(gFont_Glyphs[code - TEXT_SPACE].width +
                  gFont_Glyphs[nextCode - TEXT_SPACE].width) <=
                GLYPH_PAIR_WIDTH_MAX) {
          code |= nextCode << 8;
          text++;
        }
#endif
      }
#else
      if (*text == TEXT_MARK_VOICED) {
        code |= TEXT_CODE_VOICED;
        text++;
      } else if (*text == TEXT_MARK_SEMIVOICED) {
        code |= TEXT_CODE_SEMIVOICED;
        text++;
      }
#endif
      x += Glyph_Draw(window, code, x, y, GLYPH_DRAW_CHARACTER);
    }
  }
}

/// Unused: draws a string of 16-bit character codes as tiles in a window at
/// tile (offsetX, offsetY) of its interior, as Text_DrawMessageAsTiles draws a
/// message; a NULL string draws the decoded text in WindowWork.textBuffer.
UNUSED void Text_DrawGlyphTileAtWorkOffset(u16 *buffer,
                                           const struct Window *window,
                                           s32 offsetX, s32 offsetY) {
  struct WindowWork *work = WINDOW_WORK;
  u32 cell;

  if (buffer == NULL) {
    buffer = work->textBuffer;
    work->textBuffer[work->textBufferEnd] = 0;
    work->textBufferEnd = (work->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  }

  cell = ((window->y + offsetY + 1) << 5) + (window->x + offsetX) + 1;
  if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT) {
    // Inside the tilemap.
    s32 src;
    s32 dst;

    cell *= 2;
    dst = (s32)WINDOW_TILEMAP_VRAM + cell;
    src = (s32)work->tilemap + cell;

    Text_DrawStringAsTiles(buffer, (u16 *)src, (u16 *)dst,
                           (s32)WINDOW_TILEMAP_VRAM);
  }
}

/// Draws a zero-terminated string of 16-bit character codes into a window's
/// tilemap at tile (x, y) of its interior, one tile per character, skipping
/// control codes; a NULL text draws the decoded text in WindowWork.textBuffer.
void Text_DrawWideStringInWindow(s16 *text, struct Window *window, s32 x,
                                 s32 y) {
  struct WindowWork *work;
  u16 code;
  s16 lineStartX;

  work = WINDOW_WORK;
  lineStartX = x;
  if (text == NULL) {
    text = work->textBuffer;
    work->textBuffer[work->textBufferEnd] = 0;
    work->textBufferEnd = (work->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  }
  code = *text++;
  if (code != 0)
    do {
      if (code <= TEXT_END_ALT) {
        switch (code) {
        case TEXT_NEWLINE:
          x = lineStartX;
          y++;
          break;
        case TEXT_UNUSED_14:
        case TEXT_WINDOW_STATE:
        case TEXT_ACTOR_NAME_START:
          text++;
          // Skip the second operand too.
          // Fall through.
        case TEXT_RESET_STYLE:
        case TEXT_COLOR:
        case TEXT_STYLE:
        case TEXT_LINE_SPACING:
        case TEXT_UNUSED_11:
        case TEXT_UNUSED_12:
        case TEXT_ACTOR_NAME:
#if LANGUAGE != LANGUAGE_JAPANESE
        case TEXT_ARTICLE_CLASS:
#endif
          text++;
          break;
        }
      } else {
        Window_PutGlyph(window, code, x, y, GLYPH_DRAW_CHARACTER);
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
        x++;
#else
        // The kana marks share a tile.
        if ((u16)(code - TEXT_MARK_VOICED) > 1)
          x++;
#endif
      }
      code = *text++;
    } while (code != 0);
  work->dirty = 1;
}

/// The digits String_FormatNumber writes, in buffer[1] to [12].
#define NUMBER_DIGITS 12

/// Writes value as decimal text into buffer (NUMBER_DIGITS + 2 bytes) and
/// returns where the text starts.
u8 *String_FormatNumber(u8 *buffer, s32 input, s32 width) {
  s32 offset;
  s32 value;
  s32 negative;
  s32 spaceCharacter;
  s32 minusCharacter;
  u8 *leading;
  u8 *trim;

  value = input;
  negative = 0;
  if (value < 0) {
    if (width == 0)
      negative = 1;
    value = 0U - value;
  }

  // The digits, then spaces for the leading zeros (the last digit stays) and
  // the sign in front; the byte after them ends the text.
  buffer[0] = ' ';
  for (offset = NUMBER_DIGITS; offset != 0; offset--) {
    buffer[offset] = value % 10 + '0';
    value = value / 10;
  }

  offset = 0;
  buffer[NUMBER_DIGITS + 1] = offset;
  spaceCharacter = ' ';
  offset = 1;
  minusCharacter = '-';
  for (leading = buffer; offset != NUMBER_DIGITS + 1; leading++, offset++) {
    if (leading[1] == '0') {
      if (offset != NUMBER_DIGITS)
        leading[1] = spaceCharacter;
    } else {
      if (negative != 0)
        leading[0] = minusCharacter;
      break;
    }
  }

  if (width == 0) {
    offset = 0;
    if (buffer[0] == ' ') {
      trim = buffer;
      do {
        offset++;
        if (offset == NUMBER_DIGITS)
          break;
        trim++;
      } while (*trim == ' ');
    }
    return buffer + offset;
  }

  if (width < 0 || width > NUMBER_DIGITS)
    width = NUMBER_DIGITS;
  return buffer + NUMBER_DIGITS + 1 - width;
}
