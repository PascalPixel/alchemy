/// @file
/// Characters rendered into tiles, string widths and number formatting.
// Part of window.c.

#include "../window_internal.h"

// The glyph's pixels, 16 to a line as its tiles are read: 16 lines for the
// Japanese font, 24 for the other languages' taller one.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define GLYPH_PIXELS_SIZE (16 * 16)
#else
#define GLYPH_PIXELS_SIZE (16 * 24)
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The first kanji's code; the codes below it are the font's glyphs.
#define KANJI_FIRST_CODE 256
/// Every kanji is drawn 10 pixels wide.
#define KANJI_WIDTH 10
#endif

s32 Text_DrawGlyphPair(s32 code, u32 *out) {
  u8 pixels[GLYPH_PIXELS_SIZE];
  struct WindowWork *work;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  const u16 *mark = NULL;
  const u16 *glyph;
  s32 markCode;
#else
  const u8 *glyph;
  s32 firstIndex;
  s32 secondIndex;
  s32 secondWidth;
#endif
  s32 width;
  s32 colour;
  s32 shadowColour;
  u8 *pixel;
  s32 tileColumn;
  s32 tileRow;
  s32 line;
  s32 pixelsLeft;
  u32 tileLine;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
#if (LANGUAGE == LANGUAGE_JAPANESE)
  // The voicing mark the code carries above TEXT_MARK_SHIFT, drawn over the
  // glyph one pixel wider.
  markCode = gFont_MarkGlyphCodes[(u32)code >> TEXT_MARK_SHIFT];
  if (markCode != 0) {
    markCode -= 32;
    mark = gFont_Glyphs[markCode].rows;
  }
  code &= (1 << TEXT_MARK_SHIFT) - 1;
  if ((u32)code < KANJI_FIRST_CODE) {
    s32 index = code - 32;

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
  secondIndex = (code << 8) >> 16;
  code &= 255;
  firstIndex = code - 32;
  glyph = (const u8 *)gFont_Glyphs + firstIndex * sizeof(struct FontGlyph);
  width = ((const struct FontGlyph *)glyph)->width;
  Iwram_ClearWordsEntry(pixels, GLYPH_PIXELS_SIZE);
  glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
#endif
  if (work->mode != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = work->colour;
  }
#if (LANGUAGE == LANGUAGE_JAPANESE)
  if (work->outline == 1) {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 50, shadowColour);
    if (mark != NULL) {
      Iwram_ExpandBitRuns(mark, pixels + 24, shadowColour);
      Iwram_ExpandBitRuns(mark, pixels + 25, shadowColour);
    }
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    Iwram_ExpandBitRuns(glyph, pixels + 33, colour);
    if (mark != NULL) {
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(mark, pixel, colour);
      Iwram_ExpandBitRuns(mark, pixel + 1, colour);
      width++;
    }
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    if (mark != NULL)
      Iwram_ExpandBitRuns(mark, pixels + 24, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    if (mark != NULL) {
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(mark, pixel, colour);
      width++;
    }
  }
#else
  if (work->outline == 1) {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 50, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    Iwram_ExpandBitRuns(glyph, pixels + 33, colour);
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
  }
  if ((u16)secondIndex != 0) {
    glyph = (const u8 *)&gFont_Glyphs[-32] + ((u16)secondIndex << 5);
    secondWidth = *(const s16 *)glyph;
    glyph += 2;
    if (work->outline == 1) {
      u8 *dst = pixels + width;
      Iwram_ExpandBitRuns(glyph, dst + 49, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 50, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 32, colour);
      Iwram_ExpandBitRuns(glyph, dst + 33, colour);
      // The signed halfword width grows through its shifted form; as
      // (s16)(secondWidth + 1), 26 of 203 instructions differ in tla-en.
      secondWidth = (s32)(((u32)secondWidth << 16) + 0x10000) >> 16;
    } else {
      u8 *dst = pixels + width;
      u8 *p;
      p = dst + 49;
      Iwram_ExpandBitRuns(glyph, p, shadowColour);
      p = dst + 32;
      Iwram_ExpandBitRuns(glyph, p, colour);
    }
    width += (u16)secondWidth;
  }
#endif

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
        line++;
        pixel += 24;
        *out++ = tileLine;
      } while (line <= 7);
      tileRow++;
      pixel -= 120;
    } while (tileRow <= 1);
    tileColumn++;
    pixel += 112;
  } while (tileColumn <= 1);
  return width;
}

// German, Spanish, French, and Italian use ten pixels for nonspace characters
// up to 0xff and zero for characters above it.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define WIDE_WIDTH_FIXED 1
#endif

/// Glyph n in the font.
#define FONT_GLYPH(n)                                                          \
  ((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) +                 \
                              (const u8 *)gFont_Glyphs))

/// Spaces take four pixels; characters up to 0xff use their glyph width;
/// higher characters take ten. Voicing marks 0xde and 0xdf add no width.
s32 Text_GetWideStringWidth(u16 *text) {
  s32 width;
  u16 *p;
  u32 c;
#if !defined(WIDE_WIDTH_FIXED)
  u32 idx;
#endif

  c = *text;
  width = 0;
  p = text + 1;
  if (c != 0) {
    do {
      if (c == 0x20) {
        width += 4;
      } else if (c <= 0xffU) {
#if defined(WIDE_WIDTH_FIXED)
        width += 10;
#else
        idx = c - 0xde;
        if (idx > 1U) {
          idx += 0xbe; // The font starts at character 0x20.
          width += FONT_GLYPH(idx)->width;
        }
      } else {
        width += 10;
#endif
      }
      c = *p;
      p += 1;
    } while (c != 0);
  }
  return width;
}

void Text_ResetStyle(void);

void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y) {
  struct WindowWork *work;
  u32 c;
  u32 next;
  s16 start;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  c = 0;
  start = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = c;
    work->count = (work->count + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  }
  for (;;) {
    c = *text++;
#if (LANGUAGE != LANGUAGE_JAPANESE)
    if (c > 0xff)
      c = 0x40;
#endif
    if (c == 0)
      break;
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (c <= 30) {
#else
    if (c <= 30 || c == 176) {
#endif
      switch (c) {
      case 8:
        work->colour = *text;
        text++;
        break;
      case 9:
        work->outline = *text;
        text++;
        break;
      case 10:
        work->lineSpacing = *text;
        text++;
        break;
      case 7:
        Text_ResetStyle();
        break;
      case 3:
        x = start;
        y += 15;
        break;
      case 14:
      case 15:
      case 28:
        text++;
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
      if ((window->flags & 8) == 0) {
        next = *text;
#if TLA_PATCH_4
        if (c > 32 && next > 32) {
          s16 pairWidth =
              gFont_Glyphs[c - 32].width + gFont_Glyphs[next - 32].width;

          // Outlined text joins only pairs one pixel narrower.
          if (work->outline == 1) {
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
        if (c > 32 && next > 32 &&
            (u16)(gFont_Glyphs[c - 32].width + gFont_Glyphs[next - 32].width) <=
                15) {
          c |= next << 8;
          text++;
        }
#endif
      }
#endif
      x += Glyph_Draw(window, c, x, y, 0);
    }
  }
}

/// Draws a string of 16-bit character codes straight into the UI tilemap, as
/// tiles, inside a window at tile (offsetX, offsetY) of its interior, writing
/// both the work block's copy and VRAM. A NULL buffer draws the decoded text
/// in WindowWork.entries. The window's position is read signed.
// As ☀️'s function of the same name, which passes the VRAM base where this
// one passes offsetX & 7.
void Text_DrawGlyphTileAtWorkOffset(u16 *buffer, const struct Window *work,
                                    s32 offsetX, s32 offsetY) {
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u32 cell;

  if (buffer == NULL) {
    buffer = canvas->entries;
    canvas->entries[canvas->count] = 0;
    canvas->count = (canvas->count + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  }

  cell = (((s16)work->y + offsetY + 1) << 5) + ((s16)work->x + offsetX) + 1;
  if (cell < 32 * 20) { // inside the tilemap
    s32 src;
    s32 dst;

    cell *= 2;
    dst = 0x06002000 + cell;
    src = (s32)canvas->tilemap + cell;

    Text_DrawStringAsTiles(buffer, (u16 *)src, (u16 *)dst, offsetX & 7);
  }
}

/// Draws a string of wide (16-bit) codes into the window, one glyph per cell,
/// skipping the control codes and their arguments; code 3 starts a new line.
/// With no text, the render work's own entries are drawn, closed with a 0.
void Text_DrawWideStringInWindow(s16 *text, struct Window *window, s32 x,
                                 s32 y) {
  struct WindowWork *work;
  u16 c;
  s16 start;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  start = x;
  if (text == NULL) {
    text = work->entries;
    work->entries[work->count] = 0;
    work->count = (work->count + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  }
  c = *text++;
  if (c != 0)
    do {
#if (LANGUAGE == LANGUAGE_JAPANESE)
      if (c <= 30) {
#else
      if (c <= 30 || c == 176) {
#endif
        switch (c) {
        case 3:
          x = start;
          y++;
          break;
        case 14:
        case 15:
        case 28:
          text++;
        case 7:
        case 8:
        case 9:
        case 10:
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
        // Called with no prototype in scope, as its bytes need: declared
        // void, two argument moves trade places in tla-ja and tla-en.
        Window_PutGlyph(window, c, x, y, 0);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
        x++;
#else
        if ((u16)(c - 0xde) > 1)
          x++;
#endif
      }
      c = *text++;
    } while (c != 0);
  work->dirty = 1;
}

u32 Math_UnsignedMulHigh(u32 a, u32 b);
u32 Math_UnsignedMulHighAdd(u32 a, u32 b, s32 c);

/// Writes value as decimal text into buffer (14 bytes) and returns where the
/// text starts. With width 0 the number has no padding and a minus sign when
/// negative; otherwise it is right-aligned in width characters (up to 12) and
/// unsigned. Each digit divides by 10 through the long-multiply helpers:
/// 0x1999999a is 2^32 / 10, rounded up.
// As ☀️'s String_FormatNumber.
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

  // Twelve digits, then spaces for the leading zeros and the sign in front.
  buffer[0] = ' ';
  for (offset = 12; offset != 0; offset--) {
    buffer[offset] = Math_UnsignedMulHighAdd(value, 0x1999999a, -10) + '0';
    value = Math_UnsignedMulHigh(value, 0x1999999a);
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

  if ((u32)width > 12)
    width = 12;
  return buffer + 13 - width;
}

#undef Iwram_ExpandBitRuns
#undef GLYPH_PIXELS_SIZE
#undef KANJI_FIRST_CODE
#undef KANJI_WIDTH
#undef WIDE_WIDTH_FIXED
#undef FONT_GLYPH
