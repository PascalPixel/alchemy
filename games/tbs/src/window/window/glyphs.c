/// @file
/// Characters rendered into tiles, string widths and number formatting.
// Part of window.c.

#include "../window_internal.h"

// UiText_RenderGlyphPair draws its glyphs into a buffer of one byte per pixel
// (Iwram_ExpandBitRuns writes a glyph's rows there in a colour), in rows 16
// bytes apart, then packs its top 16 rows into 4bpp tiles. Offset 32 is row 2,
// column 0, where the glyph goes; 49 (row 3, column 1) takes its shadow, one
// pixel down and right; 24 and 7 take a kana mark above its top right.

#if (LANGUAGE != LANGUAGE_JAPANESE)
/// Draws a character, and a second one when the code carries it in bits 8 to
/// 15, into a 16x16-pixel cell of four 4bpp tiles at out: in the text colour
/// over a shadow, or with an outline when UiRenderWork.outline is 1. Returns
/// the width of the text drawn in pixels.
/// The caller supplies 32 words for four tiles, written in a 16x16 sprite's
/// order: top left, top right, bottom left, bottom right.
s32 UiText_RenderGlyphPair(s32 code, u32 *out) {
  // 24 rows of 16 pixels, of which the top 16 are packed. Iwram_ExpandBitRuns
  // draws 14 rows here, so a glyph covers rows 2 to 15 and its shadow rows 3
  // to 16; a second character starts width pixels along, and what passes
  // column 15 runs on into the next row.
  u8 pixels[384];
  struct UiRenderWork *work;
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

  work = UI_WORK;
  secondIndex = (code << 8) >> 16;
  code &= 255;
  Iwram_ClearWords(pixels, 384);
  // While UiRenderWork.mode is set, the text is colour 8 with no shadow.
  if (work->mode != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = work->colour;
  }
  firstIndex = code - 32;
  glyph = firstIndex * sizeof(struct FontGlyph) + (const u8 *)gUiText_Glyphs;
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
    // Form only the actual glyph address: &glyphs[-32] is already outside
    // the array before the later byte offset is added. The direct index
    // changes native instruction selection (EN: text grows by 8 bytes).
    glyph = (const u8 *)&gUiText_Glyphs[(u16)secondIndex - 32];
#else
    glyph = (const u8 *)&gUiText_Glyphs[-32] + ((u16)secondIndex << 5);
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
/// text colour over a shadow, or with an outline when UiRenderWork.outline is
/// 1, with the kana mark in the code's top two bits above its top right.
/// Codes from 256 are kanji. Returns the character's width in pixels.
/// The caller supplies 32 words for four tiles, in a 16x16 sprite's order.
s32 UiText_RenderGlyphPair(u32 code, u32 *out) {
  u8 pixels[256];
  struct UiRenderWork *work;
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

  /* FAKEMATCH(one-pass): an empty block keeps the spill of out ahead of the
   * work-address load; as no block, 2 of 185 instructions differ in tbs-ja,
   * the only edition with this body. */
  do {
  } while (0);
  work = UI_WORK;
  markGlyph = 0;
  markCode = gUiText_MarkGlyphCodes[code >> TEXT_CODE_MARK_SHIFT];
  if (markCode != 0)
    markGlyph =
        (const u8 *)((const struct FontGlyph *)((const u8 *)gUiText_Glyphs +
                                                (markCode - 32) *
                                                    sizeof(struct FontGlyph)))
            ->rows;
  code &= (1 << TEXT_CODE_MARK_SHIFT) - 1;
  if (code <= 0xff) {
    glyph = (const u8 *)gUiText_Glyphs + (code - 32) * sizeof(struct FontGlyph);
    width = ((const struct FontGlyph *)glyph)->width;
    glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  } else {
    glyph = gUiText_KanjiGlyphs + (code - 256) * 24;
    width = 10;
  }
  Iwram_ClearWords(pixels, 256);
  if (work->mode != 0) {
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
      /* FAKEMATCH(temporary): pixel assigned in the argument computes pixels
       * + 7 in r1 and copies it to r5, as native; as a plain pixels + 7
       * argument, 4 of 185 instructions differ in tbs-ja. */
      Iwram_ExpandBitRuns(markGlyph, pixel = pixels + 7, colour);
      Iwram_ExpandBitRuns(markGlyph, pixels + 8, colour);
    }
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    if (markGlyph != 0)
      Iwram_ExpandBitRuns(markGlyph, pixels + 24, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    if (markGlyph != 0) {
      /* FAKEMATCH(temporary): pixel assigned in the argument, as in the
       * outlined path above; as a plain pixels + 7 argument, 4 of 185
       * instructions differ in tbs-ja. */
      Iwram_ExpandBitRuns(markGlyph, pixel = pixels + 7, colour);
    }
  }
#endif

  // Pack either font into four tiles, with the leftmost pixel in the low
  // nibble. Each tile line reads eight bytes from right to left; the inner
  // loops step to the next row (+24 after the eight reads) and the right half
  // (-120), the outer one to the bottom half (+112).
  pixel = pixels + 7;
  tileColumn = 0;
  do {
    tileRow = 0;
    do {
      line = 0;
      do {
        tileLine = 0;
        pixelsLeft = 7;
        do {
          tileLine = (tileLine << 4) + *pixel--;
          pixelsLeft--;
        } while (pixelsLeft >= 0);
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

// From the German release on, every character up to 0xff counts as 10
// pixels, and those above as nothing. English draws the same Latin font with
// its glyph widths, so the change is an update, not the language's.
#if TBS_PATCH_2
#define WIDE_WIDTH_FIXED 1
#endif

/// The main font's glyph n, counted from the space character.
#define FONT_GLYPH(n)                                                          \
  ((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) +                 \
                              (const u8 *)gUiText_Glyphs))

/// Returns the width in pixels of a zero-terminated string of 16-bit character
/// codes: 4 for a space, and for other codes up to 0xff the font width, but
/// nothing for the kana marks; 10 for a kanji (code 0x100 and up). German,
/// Spanish, French and Italian count every code up to 0xff but the space as a
/// flat 10, and codes above it as nothing. Control codes are counted as
/// characters.
s32 UiText_GetWideStringWidth(u16 *text) {
  s32 width;
  u16 *cursor;
  u32 code;
#if !defined(WIDE_WIDTH_FIXED)
  u32 glyphIndex;
#endif

  code = *text;
  width = 0;
  cursor = text + 1;
  if (code != 0) {
    do {
      if (code == 0x20) {
        width += 4;
      } else if (code <= 0xffU) {
#if defined(WIDE_WIDTH_FIXED)
        width += 10;
#else
        glyphIndex = code - TEXT_VOICED_MARK;
        if (glyphIndex > 1U) {
          // The font starts at the space.
          glyphIndex += TEXT_VOICED_MARK - TEXT_SPACE;
          width += FONT_GLYPH(glyphIndex)->width;
        }
      } else {
        width += 10;
#endif
      }
      code = *cursor;
      cursor += 1;
    } while (code != 0);
  }
  return width;
}

/// Draws a zero-terminated string of 16-bit character codes into a text canvas
/// window at pixel (x, y), running its colour, outline, line spacing and new
/// line codes; other control codes are skipped with their arguments. A NULL
/// text draws the ring of decoded text from its first entry, after writing a
/// 0 at UiRenderWork.count to end it. Each character is drawn at once, with
/// no typing. A new line returns to x and goes 15 pixels down. The style
/// codes change the shared style (UiRenderWork), which stays changed after.
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x,
                                     s32 y) {
  u8 *base;
  struct UiRenderWork *work;
  u32 code;
#if (LANGUAGE != LANGUAGE_JAPANESE)
  u32 nextCode;
#endif
  s16 lineStartX;

  base = gWindowWork[0];
  work = (struct UiRenderWork *)base;
  code = 0;
  lineStartX = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = code;
    work->count = (work->count + 1) & RENDER_ENTRY_MASK;
  }
#if (LANGUAGE != LANGUAGE_JAPANESE)
  for (;;)
#else
  while ((code = *text++) != 0)
#endif
  {
#if (LANGUAGE != LANGUAGE_JAPANESE)
    // Codes above 0xff become code 0x40, as the decoder makes them.
    code = *text++;
    if (code > 0xff)
      code = 0x40;
    if (code == 0)
      break;
#endif
    // Two entries are skipped after TEXT_WINDOW_STATE and codes 14 and 28, one
    // after codes 11, 12, 17 and, outside Japanese, 29; what those codes do
    // in decoded text is unknown.
    if (code <= TEXT_END_ALT) {
      switch (code) {
      case TEXT_COLOUR:
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
      case TEXT_COLOURS_RESET:
        UiWork_ResetCounters();
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
#if (LANGUAGE != LANGUAGE_JAPANESE)
      case 29:
#endif
        text++;
        break;
      }
    } else {
#if (LANGUAGE != LANGUAGE_JAPANESE)
      // Pair two narrow characters into one cell, as UiWork_StepChannelScript
      // does.
      if ((window->flags & UI_WINDOW_TEXT_CANVAS) == 0) {
        nextCode = *text;
#if (LANGUAGE == LANGUAGE_SPANISH)
        if (code > 32 && nextCode > 32) {
          s16 width = gUiText_Glyphs[code - 32].width +
                      gUiText_Glyphs[nextCode - 32].width;

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
            (u16)(gUiText_Glyphs[code - 32].width +
                  gUiText_Glyphs[nextCode - 32].width) <= 15) {
          code |= nextCode << 8;
          text++;
        }
#endif
      }
#else
      if (*text == TEXT_VOICED_MARK) {
        code |= TEXT_CODE_VOICED;
        text++;
      } else if (*text == TEXT_SEMIVOICED_MARK) {
        code |= TEXT_CODE_SEMIVOICED;
        text++;
      }
#endif
      x += UiText_DrawGlyph(window, code, x, y, 0);
    }
  }
}

/// Draws a string of 16-bit character codes straight into the UI tilemap, as
/// tiles, inside a window at tile (offsetX, offsetY) of its interior, writing
/// both the work block's copy and VRAM. A NULL buffer draws the decoded text
/// in UiRenderWork.entries.
/// Nothing calls it.
UNUSED void UiText_RenderGlyphTileAtWorkOffset(u16 *buffer,
                                               const struct UiWindow *work,
                                               s32 offsetX, s32 offsetY) {
  struct UiRenderWork *canvas = UI_WORK;
  u32 cell;

  if (buffer == NULL) {
    u16 *counter = &canvas->count;

    buffer = canvas->entries;
    canvas->entries[*counter] = 0;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;
  }

  cell = ((work->y + offsetY + 1) << 5) + (work->x + offsetX) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    s32 src;
    s32 dst;

    cell *= 2;
    dst = (s32)UI_TILEMAP_VRAM + cell;
    src = (s32)canvas->tilemap + cell;

    UiText_RenderStringTiles(buffer, (u16 *)src, (u16 *)dst,
                             (s32)UI_TILEMAP_VRAM);
  }
}

/// Draws a zero-terminated string of 16-bit character codes into a window's
/// tilemap at tile (x, y) of its interior, one tile per character, skipping
/// control codes; a NULL text draws the decoded text in UiRenderWork.entries.
/// Control operands are skipped, not rendered as glyphs; the input is read
/// only. The character code is the tile's number (UiWindow_PutGlyph), so only
/// codes up to 0xff show. The Djinn preview (battle_command/djinn_preview.c)
/// draws its numbers this way.
void UiText_RenderWideStringInWindow(s16 *text, struct UiWindow *window, s32 x,
                                     s32 y) {
  struct UiRenderWork *work;
  u16 code;
  s16 lineStartX;

  work = UI_WORK;
  lineStartX = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = 0;
    work->count = (work->count + 1) & RENDER_ENTRY_MASK;
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
        case TEXT_COLOURS_RESET:
        case TEXT_COLOUR:
        case TEXT_OUTLINE:
        case TEXT_LINE_SPACING:
        case 11:
        case 12:
        case 17:
#if (LANGUAGE != LANGUAGE_JAPANESE)
        case 29:
#endif
          text++;
          break;
        }
      } else {
        UiWindow_PutGlyph(window, code, x, y, 0);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
        x++;
#else
        // The kana marks share a tile.
        if ((u16)(code - TEXT_VOICED_MARK) > 1)
          x++;
#endif
      }
      code = *text++;
    } while (code != 0);
  work->dirty = 1;
}

/// Writes value as decimal text into buffer (14 bytes) and returns where the
/// text starts. With width 0 the number has no padding and a minus sign when
/// negative; otherwise it is right-aligned in width characters (up to 12;
/// another width counts as 12), padded with spaces, and a negative number
/// shows its magnitude with no sign. A number of more than width digits loses
/// its leading digits.
u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width) {
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
