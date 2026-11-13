/// @file
/// Text drawn as tiles, text tile ownership and divider lines.
// Part of window.c.

#include "../window_internal.h"

extern char ResourceId_WindowTiles;

void Window_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern,
                               u8 *remap) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u8 pixels[128];
  u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  u32 slot = *(u8 *)entry;
  u8 *src;
  u8 *dst;
  u32 n;
  u32 i;

  dst = pixels;
  src = (u8 *)(0x06000000 + 32 * slot);
  for (i = 0; i < 32; i++) {
    n = *src++;
    *dst++ = n & 15;
    *dst++ = n >> 4;
  }

  dst = pixels;
  src = sheet + pattern * 32;
  for (i = 0; i < 32; i++) {
    u32 value = *src++;
    u32 color = remap[value & 15];

    if (color != 0)
      *dst = color;
    dst++;
    color = remap[value >> 4];
    if (color != 0)
      *dst = color;
    dst++;
  }

  {
    u8 *p;

    dst = pixels;
    for (i = 0, p = dst; i < 32; i++) {
      u32 value = p[0] | p[1] << 4;

      p += 2;
      *dst++ = value;
    }
  }

  if ((s8)slot >= 0) {
    n = 0;
    do {
      u32 value = work->tileCursor;
      u32 following = (value + 1) % 128;

      slot = (u8)value;
      work->tileCursor = following;
      if (work->tileAttributes[slot] == 0)
        break;
      n++;
    } while (n < 128);
    work->tileAttributes[slot] = 1;
    slot |= 0x80;
    *entry = slot | 0xf000;
    *mirror = slot | 0xf000;
  }
  Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, REG_DMA3);
}

// The Japanese font: control code 3 advances a fixed 8 pixels, codes 222 and
// 223 draw glyphs 254 and 255 three pixels to the left, and glyphs above 128
// have fixed widths; elsewhere code 3 advances by the space glyph's width.
#if LANGUAGE == LANGUAGE_JAPANESE
#define TEXT_JAPANESE_FONT 1
#define TEXT_CODE_3_WIDTH 8
#else
#define TEXT_JAPANESE_FONT 0
#define TEXT_CODE_3_WIDTH gFont_GlyphWidths[0]
#endif

// A label's end tile advances three pixels: the HP label's in every font, and
// the PP label's own in the French, Spanish and Italian fonts.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define TEXT_IS_NARROW_TILE(code)                                              \
  ((code) == WINDOW_TILE_HP_LABEL_END || (code) == WINDOW_TILE_PP_LABEL_END)
#else
#define TEXT_IS_NARROW_TILE(code) ((code) == WINDOW_TILE_HP_LABEL_END)
#endif

extern char ResourceId_WindowTiles;
extern const u8 gText_ColourPens[];

s32 Text_DrawStringAsTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x) {
  // Unused: gives the stack frame its 32 extra bytes.
  UNUSED u32 reserve[8];
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
  Iwram_ClearWords(pens, 16);
  if (attr == 0xF000) {
    pens[1] = gText_ColourPens[work->colour & 15];
    pens[3] = 3;
    Iwram_FillWords(canvas, 0x800, 0x04040404);
  } else {
    pens[1] = (u8)work->colour & 15;
    pens[3] = 1;
    Iwram_FillWords(canvas, 0x800, 0x0E0E0E0E);
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
          x += TEXT_CODE_3_WIDTH;
          break;
        case 14:
        case 15:
        case 28:
          script++;
        case 11:
        case 12:
        case 17:
#if !TEXT_JAPANESE_FONT
        case 29:
#endif
          script++;
          break;
        }
      } else {
        c &= 0xFF;
#if TEXT_JAPANESE_FONT
        if (c - 222 <= 1) {
          x -= 3;
          c += 32;
        }
#endif
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
        if (code == WINDOW_TILE_HP_LABEL || code == WINDOW_TILE_PP_LABEL)
          x += 8;
        else if (TEXT_IS_NARROW_TILE(code))
          x += 3;
        else {
#if TEXT_JAPANESE_FONT
          if (c > 128)
            w = c - 254 <= 1 ? 3 : 8;
          else
#endif
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

void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height) {
  u8 *base = Ram_HeapSlots->windowTiles;
  u16 *cursor = &((struct WindowWork *)base)->tilemap[y][x];
  u32 alternate = ((struct WindowWork *)base)->alt;
  u32 row;

  for (row = 0; row < height; row++) {
    u32 column;

    for (column = 0; column < width; column++) {
      u32 tile = *cursor++ & 0x3ff;

      if ((tile - 0x80) <= 0x7f ||
          (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
        u32 index = ((tile & 0xff) ^ 0x80);

        ((struct WindowWork *)base)->tileAttributes[index] &= 0xfc;
      }
    }
    cursor += 32 - width;
  }
}

void Window_MarkVisibleTextTiles(void) {
  u8 *base = Ram_HeapSlots->windowTiles;
  u32 alternate = ((struct WindowWork *)base)->alt;
  u16 *cursor = ((struct WindowWork *)base)->tilemap[0];
  s32 width = 30;
  s32 row;
  s32 x;

  for (row = 20; row != 0; row--) {
    for (x = 0; x < width; x++) {
      u32 tile = *cursor++ & 0x3ff;

      if ((tile - 0x80) <= 0x7f ||
          (alternate != 0 && tile > 0x1ff && tile <= 0x27f)) {
        u32 index = ((tile & 0xff) ^ 0x80);

        ((struct WindowWork *)base)->tileAttributes[index] |= 2;
      }
    }
  }

  for (row = 0; row < 256; row++, base++) {
    if (((struct WindowWork *)base)->tileAttributes[0] == 1)
      ((struct WindowWork *)base)->tileAttributes[0] = 0;
  }
}

struct WindowTileWork {
  u8 unk000[2];
  u8 alt;
  u8 unk003[0xe35];
  u8 table[256];
};

void Text_UseExtraTiles(s32 alt) {
  struct WindowTileWork *work =
      (struct WindowTileWork *)Ram_HeapSlots->windowTiles;
  s32 i;
  s32 fill;
  u8 *p;

  if (alt) {
    work->alt = 1;
    for (i = 128; i < 256; i++)
      work->table[i] = 0;
  } else {
    work->alt = 0;
    fill = 0;
    p = work->table + 128;
    for (i = 0; i < 128; i++)
      *p++ = fill;
  }
}

void Window_ReservedNoOp(void) {}

/// Draws a divider from (x1, y1) to (x2, y2), one of which must agree,
/// joining it to the frame and divider tiles already on the tilemap.
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2,
                            u32 y2) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *cursor;
  u32 pos;
  u32 tile;

  if (x1 == x2) {
    if (y1 == y2)
      return;
    if (y1 > y2) {
      pos = y1;
      y1 = y2;
      y2 = pos;
    }
    Window_ClearTextTilesInRect((s16)win->x + x2, (s16)win->y + y1, 1, y2 - y1);
    cursor = work->tilemap[(s16)win->y + y1] + (s16)win->x + x2;
    for (pos = y1; pos <= y2; pos++) {
      tile = *cursor;
      if (pos == y1) {
        switch (tile) {
        case 0xf009:
        case 0xf00b:
        case 0xf00c:
        case 0xf00d:
        case 0xf018:
          break;
        case 0xf011:
          tile = 0xf018;
          break;
        case 0xf00a:
          tile = 0xf00d;
          break;
        case 0xf00e:
          tile = 0xf009;
          break;
        default:
          tile = 0xf00f;
          break;
        }
      } else if (pos == y2) {
        switch (tile) {
        case 0xf00a:
        case 0xf00b:
        case 0xf00c:
        case 0xf00d:
        case 0xf019:
          break;
        case 0xf014:
          tile = 0xf019;
          break;
        case 0xf009:
          tile = 0xf00d;
          break;
        case 0xf00e:
          tile = 0xf00a;
          break;
        default:
          tile = 0xf00f;
          break;
        }
      } else {
        switch (tile) {
        case 0xf00b:
        case 0xf00c:
        case 0xf00d:
          break;
        case 0xf009:
        case 0xf00a:
        case 0xf00e:
          tile = 0xf00d;
          break;
        default:
          tile = 0xf00f;
          break;
        }
      }
      *cursor = tile;
      cursor += 32;
    }
  } else if (y1 == y2) {
    if (x1 == x2)
      return;
    if (x1 > x2) {
      pos = x1;
      x1 = x2;
      x2 = pos;
    }
    Window_ClearTextTilesInRect((s16)win->x + x1, (s16)win->y + y1, x2 - x1, 1);
    cursor = work->tilemap[(s16)win->y + y1] + (s16)win->x + x1;
    for (pos = x1; pos <= x2; pos++) {
      tile = *cursor;
      if (pos == x1) {
        switch (tile) {
        case 0xf009:
        case 0xf00a:
        case 0xf00b:
        case 0xf00d:
        case 0xf01a:
          break;
        case 0xf016:
          tile = 0xf01a;
          break;
        case 0xf00c:
          tile = 0xf00d;
          break;
        case 0xf00f:
          tile = 0xf00b;
          break;
        default:
          tile = 0xf00e;
          break;
        }
      } else if (pos == x2) {
        switch (tile) {
        case 0xf009:
        case 0xf00a:
        case 0xf00c:
        case 0xf00d:
        case 0xf01b:
          break;
        case 0xf017:
          tile = 0xf01b;
          break;
        case 0xf00b:
          tile = 0xf00d;
          break;
        case 0xf00f:
          tile = 0xf00c;
          break;
        default:
          tile = 0xf00e;
          break;
        }
      } else {
        switch (tile) {
        case 0xf009:
        case 0xf00a:
        case 0xf00d:
          break;
        case 0xf00b:
        case 0xf00c:
        case 0xf00f:
          tile = 0xf00d;
          break;
        default:
          tile = 0xf00e;
          break;
        }
      }
      *cursor = tile;
      cursor++;
    }
  }
}

/// Sets the text colour (0 to 15) of drawn text. The menus colour the HP
/// figures with it (menu/menu/shared.c, screens/party_status.c), and
/// set TEXT_COLOR_WHITE back after.
void Text_SetColor(s32 param) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  work->colour = param & 15;
}

/// Sets the text style (WindowWork.outline) of drawn text: 1 outlined, 2 to
/// 5 a motion of glyph sprites. Reached only through a bare entry of the
/// window far-call table.
void Text_SetStyle(s32 value) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  work->outline = value;
}

void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y);

void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct WindowWork *canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *counter = &canvas->count;
  s32 zero = 0;

  *counter = zero;
  Message_Decode(arg0, 1);
  canvas->entries[*counter] = zero;
  *counter = (*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1);
  Text_DrawWideStringAtOffset(canvas->entries, (struct Window *)arg1, arg2,
                              arg3);
}

/// Decodes a message and draws it as tiles in a window at pixel (offsetX,
/// offsetY) of its interior. offsetY is rounded down to a tile row; offsetX
/// picks the tile, and its remainder is where the text starts in it. As
/// Text_DrawMessage, it decodes from the ring's start. Nothing is drawn
/// when the place falls outside the tilemap's 32 by 20 entries.
void Text_DrawMessageAsTiles(s32 character, struct RenderInput *position,
                             u32 offsetX, u32 offsetY) {
  s32 byteOffset;
  s32 vramAddress;
  u16 *text;
  s32 zero;
  u32 cell;
  struct WindowWork *canvas;
  u16 *counter;

  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  counter = &canvas->count;
  zero = 0;
  *counter = zero;
  Message_Decode(character, 1);

  canvas->entries[*counter] = zero;
  *counter = (u16)((*counter + 1) & (UI_TEXT_ENTRY_COUNT - 1));

  cell = (((s16)position->y + (offsetY >> 3) + 1) << 5) +
         ((s16)position->x + (offsetX >> 3)) + 1;
  if (cell < 0x280U) {
    byteOffset = cell * 2;
    vramAddress = byteOffset + 0x06002000;
    text = canvas->entries;
    Text_DrawStringAsTiles(text, (s32)((u8 *)canvas->tilemap + byteOffset),
                           vramAddress, 7 & offsetX);
  }
}

void Text_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3) {
  u16 *buffer = Heap_AllocEwram(0x200);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  *output = 0;
  Text_DrawWideStringAtOffset(buffer, (struct Window *)arg1, arg2, arg3);
  Heap_Release(buffer);
}

void Text_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offsetX,
                             s32 offsetY) {
  u16 *buffer;
  u16 *output;
  struct WindowWork *canvas;
  u32 cell;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  buffer = Heap_AllocEwram(0x200);
  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  output = buffer;
  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  *output = 0;

  cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles(buffer, (u16 *)src, (u16 *)dst, phase);
    Heap_Release(buffer);
  }
}

u8 *String_FormatNumber(u8 *output, s32 value, s32 width);

void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y) {
  u16 *buffer = Heap_AllocEwram(0x200);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  x >>= 3;
  *output = 0;
  y >>= 3;
  Text_DrawWideStringInWindow((s16 *)buffer, (struct Window *)arg1, x, y);
  Heap_Release(buffer);
}

/// The number writers format the value in a 16-byte buffer and draw it with
/// the matching string writer.
void Text_DrawNumber(s32 value, s32 width, s32 output, s32 x, s32 y) {
  u8 data[16];
  s32 pinned = value;

  Text_DrawString(String_FormatNumber(data, pinned, width), output, x, y);
}

void Text_DrawNumberAtOffset(s32 value, s32 width, struct RenderInput *work,
                             s32 x, s32 y) {
  u8 data[16];
  s32 pinned = value;

  Text_DrawStringAtOffset(String_FormatNumber(data, pinned, width), work, x, y);
}

void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y) {
  u8 data[16];
  s32 pinned = value;

  Text_DrawStringInWindow(String_FormatNumber(data, pinned, width), output, x,
                          y);
}

#undef TEXT_JAPANESE_FONT
#undef TEXT_CODE_3_WIDTH
#undef TEXT_IS_NARROW_TILE

/// Draws a four-digit number after a two-tile label as tiles in window work at
/// pixel (offsetX, offsetY): the HP label for variant 0, the party status
/// window's HP; the PP label for variant 1, its PP.
void Text_DrawPrefixedNumberAtOffset(s32 value, struct RenderInput *work,
                                     s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[16];
  u16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  struct WindowWork *canvas;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  canvas = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  text = String_FormatNumber(formatted, value, 4);
  // Each label is two tiles; the PP label's second is the HP label's in
  // English, German and Japanese (WINDOW_TILE_PP_LABEL_END).
  if (variant == 0) {
    output[0] = WINDOW_TILE_HP_LABEL;
    output[1] = WINDOW_TILE_HP_LABEL_END;
  } else {
    output[0] = WINDOW_TILE_PP_LABEL;
    output[1] = WINDOW_TILE_PP_LABEL_END;
  }
  for (index = 0; index <= 4; index++) {
    output[index + 2] = text[index];
  }
  output[6] = 0;
  cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 0x280) {
    vram = 0x06002000;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}
