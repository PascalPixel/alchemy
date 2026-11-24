/// @file
/// Characters drawn into a glyph sprite's tiles, strings measured and drawn
/// into windows, and numbers written as text.
// Part of window.c.

#include "window_internal.h"
#include "system/mul_fx.h"

/// The glyph's pixels, 16 to a line as its tiles are read: 16 lines for the
/// Japanese font, 24 for the other languages' taller one.
#define GLYPH_BUFFER_WIDTH 16
#define GLYPH_PIXEL(x, y) ((y) * GLYPH_BUFFER_WIDTH + (x))

/// The second character of a pair, in bits 8 to 23 of its code.
#define GLYPH_PAIR_SECOND(code) (((code) << 8) >> 16)

/// The glyph buffer's size: 16 lines of pixels for the Japanese font, 24 for
/// the others.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define GLYPH_PIXELS_SIZE (GLYPH_BUFFER_WIDTH * 16)
#else
#define GLYPH_PIXELS_SIZE (GLYPH_BUFFER_WIDTH * 24)
#endif

/// Draws a character, and a second one when the code carries it in bits 8 to
/// 15, into a 16x16-pixel cell of four 4bpp tiles at out: in the text colour
/// over a shadow, or with an outline when WindowState.outline is 1.
s32 Text_DrawGlyphPair(s32 code, u32 *out) {
  u8 pixels[GLYPH_PIXELS_SIZE];
  struct WindowState *state;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  const u16 *mark = NULL;
  const u16 *glyph;
  s32 markCode;
#else
  const u8 *glyph;
  s32 firstIndex;
  s32 secondIndex;
  u16 secondWidth;
#endif
  s32 width;
  s32 colour;
  s32 shadowColour;
  u8 *pixel;
  s32 tileRow;
  s32 tileColumn;
  s32 line;
  s32 pixelsLeft;
  u32 tileLine;

  state = WINDOW_STATE;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  // The voicing mark the code carries above TEXT_MARK_SHIFT, drawn over the
  // glyph one pixel wider.
  markCode = gFont_MarkGlyphCodes[(u32)code >> TEXT_MARK_SHIFT];
  if (markCode != 0) {
    markCode -= TEXT_SPACE;
    mark = gFont_Glyphs[markCode].rows;
  }
  code &= (1 << TEXT_MARK_SHIFT) - 1;
  if ((u32)code < KANJI_FIRST_CODE) {
    s32 index = code - TEXT_SPACE;

    // The width, then the rows after it.
    glyph = &gFont_Glyphs[index].width;
    width = *glyph++;
  } else {
    s32 index = code - KANJI_FIRST_CODE;

    glyph = (const u16 *)(gFont_KanjiGlyphs + index * (FONT_GLYPH_ROWS * 2));
    width = KANJI_WIDTH;
  }
  Iwram_ClearWordsEntry(pixels, GLYPH_PIXELS_SIZE);
#else
  secondIndex = GLYPH_PAIR_SECOND(code);
  code &= TEXT_LAST_FONT_CODE;
  firstIndex = code - TEXT_SPACE;
  glyph = (const u8 *)gFont_Glyphs + firstIndex * sizeof(struct FontGlyph);
  width = ((const struct FontGlyph *)glyph)->width;
  Iwram_ClearWordsEntry(pixels, GLYPH_PIXELS_SIZE);
  glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
#endif
  // While WindowState.mindRead is set, the text is colour 8 with no shadow.
  if (state->mindRead != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = state->colour;
  }
#if (LANGUAGE == LANGUAGE_JAPANESE)
  if (state->outline == TEXT_STYLE_OUTLINED) {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
    if (mark != NULL) {
      Iwram_ExpandBitRuns(mark, pixels + GLYPH_PIXEL(8, 1), shadowColour);
      Iwram_ExpandBitRuns(mark, pixels + GLYPH_PIXEL(9, 1), shadowColour);
    }
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
    if (mark != NULL) {
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(mark, pixel, colour);
      Iwram_ExpandBitRuns(mark, pixel + 1, colour);
      width++;
    }
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    if (mark != NULL)
      Iwram_ExpandBitRuns(mark, pixels + GLYPH_PIXEL(8, 1), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
    if (mark != NULL) {
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(mark, pixel, colour);
      width++;
    }
  }
#else
  if (state->outline == TEXT_STYLE_OUTLINED) {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
  }
  if ((u16)secondIndex != 0) {
    glyph = (const u8 *)&gFont_Glyphs[-TEXT_SPACE] + ((u16)secondIndex << 5);
    secondWidth = *(const u16 *)glyph;
    glyph += 2;
    if (state->outline == TEXT_STYLE_OUTLINED) {
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
#endif

  // Pack the cell into its four tiles, left to right and top to bottom, each
  // line read from its eighth pixel back so the leftmost lands in the low
  // nibble.
  pixel = pixels + 7;
  tileRow = 0;
  do {
    tileColumn = 0;
    do {
      line = 0;
      do {
        tileLine = 0;
        pixelsLeft = 7;
        do {
          tileLine = (tileLine << 4) + *pixel--;
          pixelsLeft--;
        } while (pixelsLeft >= 0);
        line++;
        pixel += GLYPH_BUFFER_WIDTH + 8; // the next line's eighth pixel
        *out++ = tileLine;
      } while (line <= 7);
      tileColumn++;
      pixel -= 8 * GLYPH_BUFFER_WIDTH - 8; // up to the tile on the right
    } while (tileColumn <= 1);
    tileRow++;
    pixel += 8 * GLYPH_BUFFER_WIDTH - 2 * 8; // down to the next row of tiles
  } while (tileRow <= 1);
  return width;
}

/// German, Spanish, French, and Italian count ten pixels for every character up
/// to TEXT_LAST_FONT_CODE but the space, and nothing for those above it.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define WIDE_WIDTH_FIXED 1
#endif

/// Glyph n in the font.
#define FONT_GLYPH(n)                                                          \
  ((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) +                 \
                              (const u8 *)gFont_Glyphs))

/// Measures a string of 16-bit character codes in pixels: a space takes four,
/// a character up to TEXT_LAST_FONT_CODE its glyph's width (TEXT_MARK_VOICED
/// and TEXT_MARK_SEMIVOICED nothing) and a code above it KANJI_WIDTH; see
/// WIDE_WIDTH_FIXED for the four editions that count otherwise.
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
  struct WindowState *state;
  u32 c;
  u32 next;
  s16 start;

  state = WINDOW_STATE;
  c = 0;
  start = x;
  if (text == NULL) {
    text = state->textBuffer;
    state->textBuffer[state->textBufferEnd] = c;
    state->textBufferEnd = (state->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  }
  for (;;) {
    c = *text++;
#if (LANGUAGE != LANGUAGE_JAPANESE)
    // A code past the font draws as @.
    if (c > TEXT_LAST_FONT_CODE)
      c = '@';
#endif
    if (c == 0)
      break;
    // Colour, style, line spacing and new lines take effect; the other control
    // codes are skipped with their operands, as are codes 11, 12 and 14, whose
    // meaning is open (window_decode.c).
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (c < TEXT_COMMANDS) {
#else
    if (c < TEXT_COMMANDS || c == TEXT_EM_DASH) {
#endif
      switch (c) {
      case TEXT_COLOR:
        state->colour = *text;
        text++;
        break;
      case TEXT_OUTLINE:
        state->outline = *text;
        text++;
        break;
      case TEXT_LINE_SPACING:
        state->lineSpacing = *text;
        text++;
        break;
      case TEXT_COLORS_RESET:
        Text_ResetStyle();
        break;
      case TEXT_NEWLINE:
        x = start;
        y += TEXT_LINE_HEIGHT;
        break;
      case 14:
      case TEXT_WINDOW_STATE:
      case TEXT_ACTOR_NAME_START:
        text++;
        // Skip the second operand too.
        // Fall through.
      case 11:
      case 12:
      case TEXT_ACTOR_NAME:
#if (LANGUAGE != LANGUAGE_JAPANESE)
      case TEXT_ARTICLE_CLASS:
#endif
        text++;
        break;
      }
    } else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
      // A kana followed by a voicing mark carries the mark in its code.
      next = *text;
      if (next == TEXT_MARK_VOICED) {
        c |= 1 << TEXT_MARK_SHIFT;
        text++;
      } else if (next == TEXT_MARK_SEMIVOICED) {
        c |= 2 << TEXT_MARK_SHIFT;
        text++;
      }
#else
      // Two characters narrow enough to share a glyph's 16-pixel cell are drawn
      // as one sprite.
      if ((window->flags & WINDOW_FLAG_TEXT_CANVAS) == 0) {
        next = *text;
#if TLA_PATCH_4
        if (c > TEXT_SPACE && next > TEXT_SPACE) {
          s16 pairWidth = gFont_Glyphs[c - TEXT_SPACE].width +
                          gFont_Glyphs[next - TEXT_SPACE].width;

          // Outlined text joins only pairs one pixel narrower.
          if (state->outline == TEXT_STYLE_OUTLINED) {
            if ((u16)pairWidth <= 14) {
              c |= next << 8;
              text++;
            }
          } else if ((u16)pairWidth <= 15) {
            c |= next << 8;
            text++;
          }
        }
#else
        if (c > TEXT_SPACE && next > TEXT_SPACE &&
            (u16)(gFont_Glyphs[c - TEXT_SPACE].width +
                  gFont_Glyphs[next - TEXT_SPACE].width) <= 15) {
          c |= next << 8;
          text++;
        }
#endif
      }
#endif
      x += Glyph_Draw(window, c, x, y, GLYPH_DRAW_CHARACTER);
    }
  }
}

/// Unused: draws a string of 16-bit character codes straight into the UI
/// tilemap, as tiles, inside a window at tile (offsetX, offsetY) of its
/// interior, writing both the window state's copy and VRAM. A NULL buffer draws
/// the decoded text in WindowState.textBuffer.
UNUSED void Text_DrawGlyphTileAtStateOffset(u16 *buffer,
                                            const struct Window *window,
                                            s32 offsetX, s32 offsetY) {
  struct WindowState *canvas = WINDOW_STATE;
  u32 cell;

  if (buffer == NULL) {
    buffer = canvas->textBuffer;
    canvas->textBuffer[canvas->textBufferEnd] = 0;
    canvas->textBufferEnd = (canvas->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  }

  cell = (((s16)window->y + offsetY + 1) << 5) + ((s16)window->x + offsetX) + 1;
  if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT) { // inside the tilemap
    s32 src;
    s32 dst;

    cell *= 2;
    dst = (s32)WINDOW_TILEMAP_VRAM + cell;
    src = (s32)canvas->tilemap + cell;

    Text_DrawStringAsTiles(buffer, (u16 *)src, (u16 *)dst, offsetX & 7);
  }
}

/// Draws a string of wide (16-bit) codes into the window, one glyph per cell,
/// skipping the control codes and their arguments; TEXT_NEWLINE starts a new
/// line. With no text, the decoded text in WindowState.textBuffer is drawn.
void Text_DrawWideStringInWindow(s16 *text, struct Window *window, s32 x,
                                 s32 y) {
  struct WindowState *state;
  u16 c;
  s16 start;

  state = WINDOW_STATE;
  start = x;
  if (text == NULL) {
    text = state->textBuffer;
    state->textBuffer[state->textBufferEnd] = 0;
    state->textBufferEnd = (state->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  }
  c = *text++;
  if (c != 0)
    do {
#if (LANGUAGE == LANGUAGE_JAPANESE)
      if (c < TEXT_COMMANDS) {
#else
      if (c < TEXT_COMMANDS || c == TEXT_EM_DASH) {
#endif
        switch (c) {
        case TEXT_NEWLINE:
          x = start;
          y++;
          break;
        case 14:
        case TEXT_WINDOW_STATE:
        case TEXT_ACTOR_NAME_START:
          text++;
          // Skip the second operand too.
          // Fall through.
        case TEXT_COLORS_RESET:
        case TEXT_COLOR:
        case TEXT_OUTLINE:
        case TEXT_LINE_SPACING:
        case 11:
        case 12:
        case TEXT_ACTOR_NAME:
#if (LANGUAGE != LANGUAGE_JAPANESE)
        case TEXT_ARTICLE_CLASS:
#endif
          text++;
          break;
        }
      } else {
        Window_PutGlyph(window, c, x, y, GLYPH_DRAW_CHARACTER);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
        x++;
#else
        if ((u16)(c - TEXT_MARK_VOICED) > 1)
          x++;
#endif
      }
      c = *text++;
    } while (c != 0);
  state->dirty = 1;
}

/// The digits String_FormatNumber writes, in buffer[1] to [12].
#define NUMBER_DIGITS 12

/// Writes value as decimal text into buffer (NUMBER_DIGITS + 2 bytes) and
/// returns where the text starts. With width 0 the number has no padding and a
/// minus sign when negative; otherwise it is right-aligned in width characters
/// (up to NUMBER_DIGITS) and unsigned.
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
    buffer[offset] =
        Math_UnsignedMulHighAdd(value, RECIPROCAL_OF(10), -10) + '0';
    value = Math_UnsignedMulHigh(value, RECIPROCAL_OF(10));
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

  if ((u32)width > NUMBER_DIGITS)
    width = NUMBER_DIGITS;
  return buffer + NUMBER_DIGITS + 1 - width;
}

#undef Iwram_ExpandBitRuns
#undef GLYPH_PIXELS_SIZE
#undef GLYPH_PIXEL
#undef GLYPH_BUFFER_WIDTH
#undef GLYPH_PAIR_SECOND
#undef WIDE_WIDTH_FIXED
#undef FONT_GLYPH
