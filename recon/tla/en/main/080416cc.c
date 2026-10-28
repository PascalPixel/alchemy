// Draft, differ 6 of 428 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 2 with
// an empty asm after `u32 bits;`; an empty asm after `glyphs =
// Resource_GetTableEntry((s32)&ResourceId_WindowTiles);`, and finds nothing
// closer.
// Near miss, differ 26 of 428, 2026-10-15: The Broken Seal's
// Text_DrawStringAsTiles in the form of its English edition (the German
// one is the same size; Spanish, French and Italian are 12 bytes longer and
// Japanese 32, as in The Broken Seal's edition branches). The IWRAM clear
// and fill go through inline wrappers, as The Broken Seal's call layer
// does: called straight through the fixed address, the pens' address is
// kept in sl across the clear instead of being rebuilt in each branch
// (differ 95). Remaining: ten reordered pairs, each a load or store the
// native issues before a constant or copy of equal priority (the incoming
// arguments' stores, the spilled attribute's reload before 0xf000, the
// glyph sheet and canvas reloads); the sched2 dump shows the constant
// winning on its class against the call before it or on dependents.
// Five minutes of alchemy permute on the direct-call form found nothing.
#include "types.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "window.h"
#include "iwram_call.h"

extern char ResourceId_WindowTiles;
extern const u8 gText_ColourPens[];
extern const u8 gFont_GlyphWidths[];
void *Resource_GetTableEntry(s32 id);

static inline s32 ClearWords(void *destination, s32 size) {
  return Iwram_ClearWordsEntry(destination, size);
}

static inline s32 FillWords(void *destination, s32 size, u32 value) {
  return Iwram_FillWordsEntry(destination, size, value);
}

s32 Text_DrawStringAsTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x) {
  /* FAKEMATCH: an unread 32-byte reserve, the native stack frame's extra bytes. */
  u32 reserve[8];
  u8 pens[16];
  struct WindowWork *work;
  u8 *canvas;
  u8 *glyphs;
  u32 attr;
  u32 *src;
  u8 *dst;
  u32 c;
  u32 code;
  s32 count;
  u32 w;
  s32 i;
  s32 j;
  u32 bits;

  work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  canvas = Heap_Alloc(0x800);
  glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  attr = work->level << 12;
  ClearWords(pens, 16);
  if (attr == 0xF000) {
    pens[1] = gText_ColourPens[work->colour & 15];
    pens[3] = 3;
    FillWords(canvas, 0x800, 0x04040404);
  } else {
    pens[1] = (u8)work->colour & 15;
    pens[3] = 1;
    FillWords(canvas, 0x800, 0x0E0E0E0E);
  }
  if (script != 0) {
    for (;;) {
      c = *script++;
      code = c;
      if (c == 0)
        break;
      if (c <= 30) {
        switch (c) {
        case 8:
          work->colour = *script++;
          pens[1] = gText_ColourPens[work->colour & 15];
          break;
        case 7:
        case 9:
        case 10:
          work->colour = 15;
          pens[1] = gText_ColourPens[work->colour];
          break;
        case 3:
          x += gFont_GlyphWidths[0];
          break;
        case 14:
        case 15:
        case 28:
          script++;
        case 11:
        case 12:
        case 17:
        case 29:
          script++;
          break;
        }
      } else {
        c &= 0xFF;
        src = (u32 *)(glyphs + c * 32);
        dst = canvas + x;
        for (i = 0; i < 8; i++) {
          bits = *src++;
          for (j = 0; j < 4; j++) {
            u8 pen = pens[bits & 15];

            if (pen != 0)
              *dst = pen;
            dst++;
            bits >>= 4;
            pen = pens[bits & 15];
            if (pen != 0)
              *dst = pen;
            dst++;
            bits >>= 4;
          }
          dst += 248;
        }
        if (code == 0xF01D || code == 0xF01F)
          x += 8;
        else if (code == 0xF01E)
          x += 3;
        else {
          if (c > 31)
            w = gFont_GlyphWidths[c - 32];
          else
            w = 1;
          x += w;
        }
      }
    }
  }

  count = (u32)(x + 7) >> 3;
  src = (u32 *)canvas;
  dst = canvas;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < count; j++) {
      c = src[0];
      bits = src[1];
      c |= c << 4;
      bits |= bits >> 4;
      c &= 0x0FF00FF0;
      bits &= 0x00FF00FF;
      c |= c << 8;
      bits |= bits >> 8;
      src += 2;
      *(u32 *)dst = (c << 4) >> 16 | bits << 16;
      dst += 4;
    }
    dst = dst - count * 4 + 0x100;
    src = (u32 *)((u8 *)src - count * 8 + 0x100);
  }

  for (i = 0; i < count; i++) {
    u32 mask = 127;
    u32 tile;
    u32 *p;

    if (work->alt != 0)
      mask = 255;
    tile = *tiles & 0x3FF;
    if ((tile < 0x80 || tile > 0xFF) &&
        (work->alt == 0 || tile < 0x200 || tile > 0x27F)) {
      tile = work->tileCursor & mask;
      for (j = 0; j <= mask && work->tileAttributes[tile] != 0; j++)
        tile = (tile + 1) & mask;
      work->tileCursor = (tile + 1) & mask;
      work->tileAttributes[tile] = 1;
      if (tile > 127)
        tile += 0x180;
      else
        tile |= 0x80;
      *tiles = attr | tile;
      *mirror = attr | tile;
    }
    p = (u32 *)(0x06000000 + tile * 32);
    *p++ = ((u32 *)canvas)[i];
    *p++ = ((u32 *)canvas)[i + 0x40];
    *p++ = ((u32 *)canvas)[i + 0x80];
    *p++ = ((u32 *)canvas)[i + 0xC0];
    *p++ = ((u32 *)canvas)[i + 0x100];
    *p++ = ((u32 *)canvas)[i + 0x140];
    *p++ = ((u32 *)canvas)[i + 0x180];
    *p++ = ((u32 *)canvas)[i + 0x1C0];
    tiles++;
    mirror++;
  }
  Heap_Release(canvas);
  return count;
}
