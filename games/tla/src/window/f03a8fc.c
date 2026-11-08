#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "glyph.h"
#include "iwram_call.h"
#include "edition.h"
#include "text_font.h"

#define Iwram_ExpandBitRuns                                                    \
  ((s32 (*)(const void *bits, void *destination, s32 value))0x03000314)

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

s32 Func_0803a8fc(s32 code, u32 *out) {
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
  Iwram_ClearWords(pixels, GLYPH_PIXELS_SIZE);
#else
  secondIndex = (code << 8) >> 16;
  code &= 255;
  firstIndex = code - 32;
  glyph = (const u8 *)gFont_Glyphs + firstIndex * sizeof(struct FontGlyph);
  width = ((const struct FontGlyph *)glyph)->width;
  Iwram_ClearWords(pixels, GLYPH_PIXELS_SIZE);
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
      /* FAKEMATCH(dead): the loop's start stored before the mark's colour
       * pass, a store the loop overwrites; as no store, 6 of 204 instructions
       * differ in tla-ja. */
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(mark, pixels + 7, colour);
      Iwram_ExpandBitRuns(mark, pixels + 8, colour);
      width++;
    }
    width++;
  } else {
    Iwram_ExpandBitRuns(glyph, pixels + 49, shadowColour);
    if (mark != NULL)
      Iwram_ExpandBitRuns(mark, pixels + 24, shadowColour);
    Iwram_ExpandBitRuns(glyph, pixels + 32, colour);
    if (mark != NULL) {
      /* FAKEMATCH(dead): the loop's start stored before the mark's colour
       * pass, a store the loop overwrites; as no store, 6 of 204 instructions
       * differ in tla-ja. */
      pixel = pixels + 7;
      Iwram_ExpandBitRuns(mark, pixels + 7, colour);
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
      /* FAKEMATCH: the signed halfword width grows through its shifted form;
       * as (s16)(secondWidth + 1), 26 of 203 instructions differ in tla-en. */
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
        /* FAKEMATCH(barrier): the empty asm reading line and pixel puts
         * pixel += 24 between the stmia and the copy of out; as no asm, 2 of
         * 203 instructions differ in tla-en. */
        __asm__("" : "+r"(pixel) : "r"(line));
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
