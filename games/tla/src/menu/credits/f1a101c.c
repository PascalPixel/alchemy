#include "types.h"
#include "heap_release.h"
#include "iwram_call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);

extern const u8 gDisplayScroll_GlyphWidths[];
extern const u8 gDisplayScroll_Font[];
static __inline__ void DisplayScroll_CopyWords(void *destination, void *source,
                                               s32 size) {
  Iwram_CopyWords(destination, source, size);
}

s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align) {
  u8 *buf = (u8 *)Heap_AllocEwram(0x900);
  s32 x = 0;
  s32 width = 192;
  const u8 *font = gDisplayScroll_Font;
  s32 tiles;
  s32 pos;
  const u8 *p;
  u8 *dst;
  u32 c;
  s32 glyph;
  s32 w;
  s32 row;
  s32 bit;
  u32 bits;
  u32 mask;
  /* FAKEMATCH(dead): an unused array for the stack frame's 32 bytes no
   * instruction touches; as no array, 4 of 279 instructions differ in tla-en.
   */
  u32 unused[8];

  if (text == NULL)
    return -1;
  if (!GameFlag_Test(0x200)) {
    Iwram_FillWords(buf, 0x900, 0);
    GameFlag_SetBit(0x200);
  } else {
    /* FAKEMATCH(wrapper): the copy forwarded through DisplayScroll_CopyWords,
     * which keeps its own constants; as Iwram_CopyWords called directly, 71
     * of 279 instructions differ in tla-en. */
    DisplayScroll_CopyWords(buf, buf + 0x800, 0x100);
    Iwram_FillWords(buf + 0x100, 0x800, 0);
  }

  p = text;
  pos = 0;
  while ((c = *p++) != 0) {
    if (c > 31)
      pos += gDisplayScroll_GlyphWidths[c - 32];
  }
  if (align == 2)
    x = width - pos;
  else if (align == 1)
    x = (width - pos) / 2;

  p = text;
  pos = 0;
  while ((c = *text++) != 0) {
    if (c > 31) {
      glyph = c - 32;
      p = font + glyph * 8;
      dst = buf + x + pos;
      for (row = 0; row < 8; row++) {
        bits = *p++;
        mask = 0x80;
        for (bit = 7; bit >= 0; bit--) {
          if (bits & mask) {
            dst[0x101] = 1;
            dst[0] = 15;
          }
          dst++;
          mask >>= 1;
        }
        dst += 248;
      }
      w = 1;
      if (c > 31)
        w = gDisplayScroll_GlyphWidths[glyph];
      pos += w;
    }
  }

  tiles = width / 8;
  p = buf;
  dst = buf;
  for (row = 0; row < 8; row++) {
    for (bit = 0; bit < tiles * 4; bit++) {
      c = *p++;
      c |= *p++ << 4;
      *dst++ = c;
    }
    dst += 256 - tiles * 4;
    p += 256 - tiles * 8;
  }

  for (row = 0; row < tiles; row++) {
    s32 tile = slot + row;
    u32 *words = (u32 *)buf + row;

    *(u32 *)(0x06010000 + tile * 32) = words[0x000];
    *(u32 *)(0x06010004 + tile * 32) = words[0x040];
    *(u32 *)(0x06010008 + tile * 32) = words[0x080];
    *(u32 *)(0x0601000c + tile * 32) = words[0x0c0];
    *(u32 *)(0x06010010 + tile * 32) = words[0x100];
    *(u32 *)(0x06010014 + tile * 32) = words[0x140];
    *(u32 *)(0x06010018 + tile * 32) = words[0x180];
    *(u32 *)(0x0601001c + tile * 32) = words[0x1c0];
  }
  Heap_Release(buf);
  return 0;
}

// Unused: an empty function nothing calls, after the pool.
UNUSED void Func_081a1284(void) {}
