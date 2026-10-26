// Draft, differ 2 of 203 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer (device search).
// Near miss, differ 2 of 203: The Broken Seal's international
// UiText_RenderGlyphPair. ⚓️ reads the first glyph's width before it clears
// the pixels, the second glyph's width signed, and sets the shadow colour
// before the text colour. Iwram_ExpandBitRuns is the resident routine at
// 0x03000314 (the listing's IwramFillWords + 0xb4), which ⚓️'s
// iwram_call.h does not list yet: adopting needs its entry there and a label
// in system/runtime_math.s. Left: in the packing loop the native advances
// pixel by 24 between the store and the spill of out; agscc spills first.
// Reordering the three statements, out++ apart, or a barrier before it, do
// not move it. The old draft (another work layout) differed 182.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "glyph.h"
#include "iwram_call.h"

#define Iwram_ExpandBitRuns                                                    \
  ((s32 (*)(const void *bits, void *destination, s32 value))0x03000314)

s32 UiText_RenderGlyphPair(s32 code, u32 *out) {
  u8 pixels[384];
  struct UiRenderWork *work;
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

  /* FAKEMATCH: the one-pass block keeps the spill of out ahead of the work
   * load, as in ☀️'s Japanese twin (differ 4 without it). */
  do {
  } while (0);
  work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  secondIndex = (code << 8) >> 16;
  code &= 255;
  firstIndex = code - 32;
  glyph = (const u8 *)gUiText_Glyphs + firstIndex * sizeof(struct FontGlyph);
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
    glyph = (const u8 *)&gUiText_Glyphs[-32] + ((u16)secondIndex << 5);
    secondWidth = *(const s16 *)glyph;
    glyph += 2;
    if (work->outline == 1) {
      u8 *dst = pixels + width;
      Iwram_ExpandBitRuns(glyph, dst + 49, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 50, shadowColour);
      Iwram_ExpandBitRuns(glyph, dst + 32, colour);
      Iwram_ExpandBitRuns(glyph, dst + 33, colour);
      /* FAKEMATCH: the signed halfword width grows through its shifted form;
       * as (s16)(secondWidth + 1) it differs 28. */
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
