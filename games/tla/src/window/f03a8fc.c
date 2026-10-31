#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "glyph.h"
#include "iwram_call.h"

#define Iwram_ExpandBitRuns                                                    \
  ((s32 (*)(const void *bits, void *destination, s32 value))0x03000314)

s32 Func_0803a8fc(s32 code, u32 *out) {
  u8 pixels[384];
  struct WindowWork *work;
  const u8 *glyph;
  s32 width;
  s32 colour;
  s32 shadowColour;
  s32 firstIndex;
  s32 secondIndex;
  s32 secondWidth;
  u8 *pixel;
  s32 tileColumn;
  s32 tileRow;
  s32 line;
  s32 pixelsLeft;
  u32 tileLine;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  secondIndex = (code << 8) >> 16;
  code &= 255;
  firstIndex = code - 32;
  glyph = (const u8 *)gFont_Glyphs + firstIndex * sizeof(struct FontGlyph);
  width = ((const struct FontGlyph *)glyph)->width;
  Iwram_ClearWords(pixels, 384);
  glyph = (const u8 *)((const struct FontGlyph *)glyph)->rows;
  if (work->mode != 0) {
    shadowColour = 0;
    colour = 8;
  } else {
    shadowColour = 1;
    colour = work->colour;
  }
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
