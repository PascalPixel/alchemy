/// @file
/// Characters rendered into tiles, string widths and number formatting.
// Part of window.c.

#include "window_internal.h"

// Text_DrawGlyphPair draws its glyphs into a buffer of one byte per pixel
// (Iwram_ExpandBitRuns writes a glyph's rows there in a colour), in rows 16
// bytes apart, then packs its top 16 rows into 4bpp tiles.

#if LANGUAGE != LANGUAGE_JAPANESE
/// Draws a character, and a second one when the code carries it in bits 8 to
/// 15, into a 16x16-pixel cell of four 4bpp tiles at out: in the text colour
/// over a shadow, or with an outline when WindowWork.outline is 1.
s32 Text_DrawGlyphPair(s32 code, u32 *out) {
  // 24 rows of 16 pixels, of which the top 16 are packed.
  u8 pixels[384];
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
  secondIndex = (code << 8) >> 16;
  code &= 255;
  Iwram_ClearWords(pixels, 384);
  // While WindowWork.mindRead is set, the text is colour 8 with no shadow.
  if (work->mindRead != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = work->colour;
  }
  firstIndex = code - 32;
  glyph = firstIndex * sizeof(struct FontGlyph) + (const u8 *)gFont_Glyphs;
  width = ((const struct FontGlyph *)glyph)->width;
  glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  // Outlined text is the glyph drawn twice a pixel apart, over its shadow
  // drawn the same way: one pixel wider.
  if (work->outline == TEXT_STYLE_OUTLINED) {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 50, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    Iwram_ExpandBitRuns(glyph, pixels + 33, colour);
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
  }
  // The second character follows the first, starting width pixels along.
  if ((u16)secondIndex != 0) {
#ifdef UBFIX
    // Form only the actual glyph address: &glyphs[-32] is already outside the
    // array before the later byte offset is added.
    glyph = (const u8 *)&gFont_Glyphs[(u16)secondIndex - 32];
#else
    glyph = (const u8 *)&gFont_Glyphs[-32] + ((u16)secondIndex << 5);
#endif
    secondWidth = *(const u16 *)glyph;
    glyph += 2;
    if (work->outline == TEXT_STYLE_OUTLINED) {
      u8 *dst = pixels + width;
      Iwram_ExpandBitRuns(glyph, dst + 49, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 50, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 32, colour);
      Iwram_ExpandBitRuns(glyph, dst + 33, colour);
      secondWidth += 1;
    } else {
      u8 *dst = pixels + width;
      u8 *p;
      p = dst + 49;
      Iwram_ExpandBitRuns(glyph, p, shadowColour);
      p = dst + 32;
      Iwram_ExpandBitRuns(glyph, p, colour);
    }
    width += secondWidth;
  }

#else
/// Draws a character into a 16x16-pixel cell of four 4bpp tiles at out: in the
/// text colour over a shadow, or with an outline when WindowWork.outline is 1,
/// with the kana mark in the code's top two bits above its top right.
s32 Text_DrawGlyphPair(u32 code, u32 *out) {
  u8 pixels[256];
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
                                                (markCode - 32) *
                                                    sizeof(struct FontGlyph)))
            ->rows;
  code &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
  if (code <= TEXT_LAST_FONT_CODE) {
    glyph = (const u8 *)gFont_Glyphs + (code - 32) * sizeof(struct FontGlyph);
    width = ((const struct FontGlyph *)glyph)->width;
    glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  } else {
    glyph = gFont_KanjiGlyphs + (code - 256) * 24;
    width = 10;
  }
  Iwram_ClearWords(pixels, 256);
  if (work->mindRead != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = work->colour;
  }
  if (work->outline == TEXT_STYLE_OUTLINED) {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 50, shadowColour);
    if (markGlyph != 0) {
      Iwram_ExpandBitRuns(markGlyph, pixels + 24, shadowColour);
      Iwram_ExpandBitRuns(markGlyph, pixels + 25, shadowColour);
    }
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    Iwram_ExpandBitRuns(glyph, pixels + 33, colour);
    if (markGlyph != 0) {
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(markGlyph, pixel, colour);
      Iwram_ExpandBitRuns(markGlyph, pixel + 1, colour);
    }
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    if (markGlyph != 0)
      Iwram_ExpandBitRuns(markGlyph, pixels + 24, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    if (markGlyph != 0) {
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(markGlyph, pixel, colour);
    }
  }
#endif

  // Pack either font into four tiles, with the leftmost pixel in the low
  // nibble.
  pixel = pixels + 7;
  tileColumn = 0;
  do {
    tileRow = 0;
    do {
      line = 0;
      do {
        tileLine = 0;
        for (pixelsLeft = 0; pixelsLeft < 8; pixelsLeft++) {
          tileLine = (tileLine << 4) + *pixel--;
        }
        *out++ = tileLine;
        line++;
        pixel += 24;
      } while (line <= 7);
      tileRow++;
      pixel -= 120;
    } while (tileRow <= 1);
    tileColumn++;
    pixel += 112;
  } while (tileColumn <= 1);
  return width;
}

// From the German release on, every code up to TEXT_LAST_FONT_CODE counts as 10
// pixels, and those above as nothing.
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
      width += 10;
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
    // Codes above TEXT_LAST_FONT_CODE become code 64, as the decoder makes
    // them.
    code = *text++;
    if (code > TEXT_LAST_FONT_CODE)
      code = 64;
    if (code == 0)
      break;
#endif
    // Two entries are skipped after TEXT_WINDOW_STATE and codes 14 and 28, one
    // after codes 11, 12, 17 and, outside Japanese, 29; what those codes do
    // in decoded text is unknown.
    if (code <= TEXT_END_ALT) {
      switch (code) {
      case TEXT_COLOR:
        work->colour = *text;
        text++;
        break;
      case TEXT_OUTLINE:
        work->outline = *text;
        text++;
        break;
      case TEXT_LINE_SPACING:
        work->lineSpacing = *text;
        text++;
        break;
      case TEXT_COLORS_RESET:
        Text_ResetStyle();
        break;
      case TEXT_NEWLINE:
        x = lineStartX;
        y += 15;
        break;
      case 14:
      case TEXT_WINDOW_STATE:
      case 28:
        text++;
        // Skip the second operand too.
        // Fall through.
      case 11:
      case 12:
      case 17:
#if LANGUAGE != LANGUAGE_JAPANESE
      case 29:
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
        if (code > 32 && nextCode > 32) {
          s16 width =
              gFont_Glyphs[code - 32].width + gFont_Glyphs[nextCode - 32].width;

          if (work->outline == TEXT_STYLE_OUTLINED) {
            if ((u16)width <= 14) {
              code |= nextCode << 8;
              text++;
            }
          } else if ((u16)width <= 15) {
            code |= nextCode << 8;
            text++;
          }
        }
#else
        if (code > 32 && nextCode > 32 &&
            (u16)(gFont_Glyphs[code - 32].width +
                  gFont_Glyphs[nextCode - 32].width) <= 15) {
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

/// Draws a string of 16-bit character codes straight into the UI tilemap, as
/// tiles, inside a window at tile (offsetX, offsetY) of its interior, writing
/// both the work block's copy and VRAM.
UNUSED void Text_DrawGlyphTileAtWorkOffset(u16 *buffer,
                                           const struct Window *window,
                                           s32 offsetX, s32 offsetY) {
  struct WindowWork *canvas = WINDOW_WORK;
  u32 cell;

  if (buffer == NULL) {
    buffer = canvas->textBuffer;
    canvas->textBuffer[canvas->textBufferEnd] = 0;
    canvas->textBufferEnd = (canvas->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  }

  cell = ((window->y + offsetY + 1) << 5) + (window->x + offsetX) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    s32 src;
    s32 dst;

    cell *= 2;
    dst = (s32)WINDOW_TILEMAP_VRAM + cell;
    src = (s32)canvas->tilemap + cell;

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
        case 14:
        case TEXT_WINDOW_STATE:
        case 28:
          text++;
          // Skip the second operand too.
          // Fall through.
        case TEXT_COLORS_RESET:
        case TEXT_COLOR:
        case TEXT_OUTLINE:
        case TEXT_LINE_SPACING:
        case 11:
        case 12:
        case 17:
#if LANGUAGE != LANGUAGE_JAPANESE
        case 29:
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

/// Writes value as decimal text into buffer (14 bytes) and returns where the
/// text starts.
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

  // Twelve digits in buffer[1] to [12], then spaces for the leading zeros
  // (the last digit stays) and the sign in front; buffer[13] ends the text.
  buffer[0] = ' ';
  for (offset = 12; offset != 0; offset--) {
    buffer[offset] = value % 10 + '0';
    value = value / 10;
  }

  offset = 0;
  buffer[13] = offset;
  spaceCharacter = ' ';
  offset = 1;
  minusCharacter = '-';
  for (leading = buffer; offset != 13; leading++, offset++) {
    if (leading[1] == '0') {
      if (offset != 12)
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
        if (offset == 12)
          break;
        trim++;
      } while (*trim == ' ');
    }
    return buffer + offset;
  }

  if (width < 0 || width > 12)
    width = 12;
  return buffer + 13 - width;
}
