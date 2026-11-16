/// @file
// The sign tiles Text_DrawPrefixedNumberAtOffset writes (the HP and PP
// labels' tiles) advance 8 for a first tile and 3 for a second, in
// pixels; a character its font width, and any other tile below 32 one.
/// Text drawn as tiles, text tile ownership and divider lines.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"

/// Draws window tile pattern over the tile of tilemap entry *entry, with its
/// colours mapped through remap (0 leaves a pixel as it was).
void Window_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern,
                               u8 *remap) {
  struct WindowWork *work = WINDOW_WORK;
  u8 pixels[128];
  u8 *sheet = Resource_GetTableEntry(RESOURCE_WINDOW_TILES);
  u32 slot = *(u8 *)entry;
  u8 *src;
  u8 *dst;
  u32 n;
  u32 i;

  dst = pixels;
  src = BG_CHAR_BLOCK(0) + TILE_SIZE_4BPP * slot;
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
      u32 value = work->nextTextTile;
      u32 following = (value + 1) % 128;

      slot = (u8)value;
      work->nextTextTile = following;
      if (work->textTileState[slot] == 0)
        break;
      n++;
    } while (n < 128);
    work->textTileState[slot] = 1;
    slot |= 0x80;
    *entry = WINDOW_TILE(slot);
    *mirror = WINDOW_TILE(slot);
  }
  Dma_Set(pixels, BG_CHAR_BLOCK(0) + slot * TILE_SIZE_4BPP, DMA_COPY_WORDS(8),
          REG_DMA3);
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

extern const u8 gText_ColourPens[];

/// Draws a string of 16-bit character codes with the window font into tiles of
/// their own, starting x pixels into the first tile, and points tiles (and the
/// same entries in mirror) at them.
s32 Text_DrawStringAsTiles(u16 *text, u16 *tiles, u16 *mirror, s32 x) {
  // Unused: gives the stack frame its 32 extra bytes.
  UNUSED u32 reserve[8];
  u8 pens[16];
  struct WindowWork *work;
  u8 *canvas;
  u8 *glyphs;
  u32 paletteBits;
  u32 *src;
  u8 *dst;
  u32 glyphCode;
  u32 code;
  s32 tileCount;
  u32 glyphWidth;
  s32 index;
  s32 column;
  u32 bits;

  work = WINDOW_WORK;
  canvas = Heap_Alloc(2048);
  glyphs = Resource_GetTableEntry(RESOURCE_WINDOW_TILES);
  paletteBits = BG_TILE_PALETTE(work->textPalette);
  Iwram_ClearWords(pens, 16);
  // pens maps a font pixel, 1 or 3 (the font's two shades), to a colour; 0
  // leaves the background.
  if (paletteBits == BG_TILE_PALETTE(WINDOW_PALETTE)) {
    pens[1] = gText_ColourPens[work->colour & (PALETTE_COLORS - 1)];
    pens[3] = 3;
    Iwram_FillWords(canvas, 2048, PIXELS_8BPP(WINDOW_BACKGROUND_COLOR));
  } else {
    pens[1] = (u8)work->colour & (PALETTE_COLORS - 1);
    pens[3] = 1;
    Iwram_FillWords(canvas, 2048, PIXELS_8BPP(TEXT_BACKGROUND_COLOR));
  }
  // Expand glyph pixels and control-code colour changes into a byte canvas.
  if (text != 0) {
    for (;;) {
      glyphCode = *text++;
      code = glyphCode;
      if (glyphCode == 0)
        break;
      if (glyphCode <= TEXT_END_ALT) {
        switch (glyphCode) {
        case TEXT_COLOR:
          work->colour = *text++;
          pens[1] = gText_ColourPens[work->colour & (PALETTE_COLORS - 1)];
          break;
        // TEXT_COLORS_RESET, TEXT_OUTLINE and TEXT_LINE_SPACING all reset the
        // colour here, and the last two leave their operand to be read as the
        // next code.
        case TEXT_COLORS_RESET:
        case TEXT_OUTLINE:
        case TEXT_LINE_SPACING:
          work->colour = TEXT_COLOR_WHITE;
          pens[1] = gText_ColourPens[work->colour];
          break;
        case TEXT_NEWLINE:
          x += TEXT_CODE_3_WIDTH;
          break;
        case 14:
        case TEXT_WINDOW_STATE:
        case 28:
          text++;
          // Skip the second operand too.
          // Fall through.
        case 11:
        case 12:
        case TEXT_ACTOR_NAME:
#if !TEXT_JAPANESE_FONT
        case TEXT_ARTICLE_CLASS:
#endif
          text++;
          break;
        }
      } else {
        // A tilemap code (such as WINDOW_TILE_HP_LABEL) draws window tile
        // glyphCode & 0xff.
        glyphCode &= 0xFF;
#if TEXT_JAPANESE_FONT
        if (glyphCode - TEXT_MARK_VOICED <= 1) {
          x -= 3;
          glyphCode += 32;
        }
#endif
        // Each glyph is an 8x8 tile of the window tiles, a word a row.
        src = (u32 *)(glyphs + glyphCode * 32);
        dst = canvas + x;
        for (index = 0; index < 8; index++) {
          bits = *src++;
          for (column = 0; column < 4; column++) {
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
          if (glyphCode > 128)
            glyphWidth = glyphCode - 254 <= 1 ? 3 : 8;
          else
#endif
              if (glyphCode >= TEXT_SPACE)
            glyphWidth = gFont_GlyphWidths[glyphCode - TEXT_SPACE];
          else
            glyphWidth = 1;
          x += glyphWidth;
        }
      }
    }
  }

  // Pack each eight-pixel span back into the tile's four-bit colour fields.
  tileCount = (u32)(x + 7) >> 3;
  src = (u32 *)canvas;
  dst = canvas;
  for (index = 0; index < 8; index++) {
    for (column = 0; column < tileCount; column++) {
      glyphCode = src[0];
      bits = src[1];
      glyphCode |= glyphCode << 4;
      bits |= bits >> 4;
      glyphCode &= 0x0FF00FF0;
      bits &= 0x00FF00FF;
      glyphCode |= glyphCode << 8;
      bits |= bits >> 8;
      src += 2;
      *(u32 *)dst = (glyphCode << 4) >> 16 | bits << 16;
      dst += 4;
    }
    dst = dst - tileCount * 4 + 256;
    src = (u32 *)((u8 *)src - tileCount * 8 + 256);
  }

  // Reuse private text tiles, allocate any new ones, then upload each tile.
  for (index = 0; index < tileCount; index++) {
    u32 mask = TEXT_TILES - 1;
    u32 tile;
    u32 *vramTile;

    if (work->extraTextTiles != 0)
      mask = 2 * TEXT_TILES - 1;
    tile = *tiles & TILE_NUMBER_MASK;
    if ((tile < TEXT_TILE_FIRST || tile > TEXT_TILE_FIRST + TEXT_TILES - 1) &&
        (work->extraTextTiles == 0 || tile < TEXT_EXTRA_TILE_FIRST ||
         tile > TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1)) {
      tile = work->nextTextTile & mask;
      for (column = 0; column <= mask && work->textTileState[tile] != 0;
           column++)
        tile = (tile + 1) & mask;
      work->nextTextTile = (tile + 1) & mask;
      work->textTileState[tile] = 1;
      if (tile > (TEXT_TILES - 1))
        tile += TEXT_EXTRA_TILE_FIRST - TEXT_TILES;
      else
        tile |= 0x80;
      *tiles = paletteBits | tile;
      *mirror = paletteBits | tile;
    }
    vramTile = (u32 *)(BG_CHAR_BLOCK(0) + tile * TILE_SIZE_4BPP);
    *vramTile++ = ((u32 *)canvas)[index];
    *vramTile++ = ((u32 *)canvas)[index + 64];
    *vramTile++ = ((u32 *)canvas)[index + 128];
    *vramTile++ = ((u32 *)canvas)[index + 192];
    *vramTile++ = ((u32 *)canvas)[index + 256];
    *vramTile++ = ((u32 *)canvas)[index + 320];
    *vramTile++ = ((u32 *)canvas)[index + 384];
    *vramTile++ = ((u32 *)canvas)[index + 448];
    tiles++;
    mirror++;
  }
  Heap_Release(canvas);
  return tileCount;
}

/// Frees the text tiles under a tilemap rectangle (x, y, width and height in
/// tiles) for reuse: their taken and shown bits are cleared.
void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height) {
  u8 *base = Ram_HeapSlots->windowWork;
  u16 *cursor = &((struct WindowWork *)base)->tilemap[y][x];
  u32 alternate = ((struct WindowWork *)base)->extraTextTiles;
  u32 row;

  for (row = 0; row < height; row++) {
    u32 column;

    for (column = 0; column < width; column++) {
      u32 tile = *cursor++ & TILE_NUMBER_MASK;

      if ((tile - TEXT_TILE_FIRST) <= TEXT_TILES - 1 ||
          (alternate != 0 && tile > (TEXT_EXTRA_TILE_FIRST - 1) &&
           tile <= TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1)) {
        u32 index = ((tile & 0xFF) ^ 0x80);

        ((struct WindowWork *)base)->textTileState[index] &= 0xFC;
      }
    }
    cursor += BG_SCREEN_WIDTH - width;
  }
}

/// Marks the text tiles the tilemap shows, and frees those it no longer shows:
/// a tile taken but not marked shown (attribute exactly 1).
void Window_MarkVisibleTextTiles(void) {
  u8 *base = Ram_HeapSlots->windowWork;
  u32 alternate = ((struct WindowWork *)base)->extraTextTiles;
  u16 *cursor = ((struct WindowWork *)base)->tilemap[0];
  s32 width = DISPLAY_TILE_WIDTH;
  s32 row;
  s32 x;

  // The cursor never skips the two hidden columns that end each 32-entry
  // row, so the scan covers the tilemap's first 600 entries in order: row 18
  // from column 24 on and row 19 are never marked shown.
  for (row = DISPLAY_TILE_HEIGHT; row != 0; row--) {
    for (x = 0; x < width; x++) {
      u32 tile = *cursor++ & TILE_NUMBER_MASK;

      if ((tile - TEXT_TILE_FIRST) <= TEXT_TILES - 1 ||
          (alternate != 0 && tile > (TEXT_EXTRA_TILE_FIRST - 1) &&
           tile <= TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1)) {
        u32 index = ((tile & 0xff) ^ 0x80);

        ((struct WindowWork *)base)->textTileState[index] |= 2;
      }
    }
  }

  for (row = 0; row < 2 * TEXT_TILES; row++, base++) {
    if (((struct WindowWork *)base)->textTileState[0] == 1)
      ((struct WindowWork *)base)->textTileState[0] = 0;
  }
}

/// Frees the text tiles of the second range (attributes 128 to 255, tiles 512
/// to 639), and with allowAlternateTiles set lets text tiles take that range
/// too; the first range's tiles keep their state. The flag, once read, holds
/// the zero the tiles are cleared with.
void Text_UseExtraTiles(s32 allowAlternateTiles) {
  struct WindowWork *work = WINDOW_WORK;
  s32 i;
  u8 *p;

  if (allowAlternateTiles) {
    work->extraTextTiles = 1;
    for (i = TEXT_TILES; i < 2 * TEXT_TILES; i++)
      work->textTileState[i] = 0;
  } else {
    work->extraTextTiles = 0;
    allowAlternateTiles = 0;
    p = work->textTileState + TEXT_TILES;
    for (i = 0; i < TEXT_TILES; i++)
      *p++ = allowAlternateTiles;
  }
}

/// Unused: nothing calls it. Does nothing.
UNUSED void Window_ReservedNoOp(void) {}

/// Draws a divider from (x1, y1) to (x2, y2), one of which must agree,
/// joining it to the frame and divider tiles already on the tilemap.
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2,
                            u32 y2) {
  struct WindowWork *work = WINDOW_WORK;
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
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
        case WINDOW_TILE_DIVIDER_TEE_LEFT:
        case WINDOW_TILE_DIVIDER_CROSS:
        case WINDOW_TILE_BORDER_TOP_TEE:
          break;
        case WINDOW_TILE_BORDER_TOP:
          tile = WINDOW_TILE_BORDER_TOP_TEE;
          break;
        case WINDOW_TILE_DIVIDER_TEE_UP:
          tile = WINDOW_TILE_DIVIDER_CROSS;
          break;
        case WINDOW_TILE_DIVIDER_HORIZONTAL:
          tile = WINDOW_TILE_DIVIDER_TEE_DOWN;
          break;
        default:
          tile = WINDOW_TILE_DIVIDER_VERTICAL;
          break;
        }
      } else if (pos == y2) {
        switch (tile) {
        case WINDOW_TILE_DIVIDER_TEE_UP:
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
        case WINDOW_TILE_DIVIDER_TEE_LEFT:
        case WINDOW_TILE_DIVIDER_CROSS:
        case WINDOW_TILE_BORDER_BOTTOM_TEE:
          break;
        case WINDOW_TILE_BORDER_BOTTOM:
          tile = WINDOW_TILE_BORDER_BOTTOM_TEE;
          break;
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
          tile = WINDOW_TILE_DIVIDER_CROSS;
          break;
        case WINDOW_TILE_DIVIDER_HORIZONTAL:
          tile = WINDOW_TILE_DIVIDER_TEE_UP;
          break;
        default:
          tile = WINDOW_TILE_DIVIDER_VERTICAL;
          break;
        }
      } else {
        switch (tile) {
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
        case WINDOW_TILE_DIVIDER_TEE_LEFT:
        case WINDOW_TILE_DIVIDER_CROSS:
          break;
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
        case WINDOW_TILE_DIVIDER_TEE_UP:
        case WINDOW_TILE_DIVIDER_HORIZONTAL:
          tile = WINDOW_TILE_DIVIDER_CROSS;
          break;
        default:
          tile = WINDOW_TILE_DIVIDER_VERTICAL;
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
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
        case WINDOW_TILE_DIVIDER_TEE_UP:
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
        case WINDOW_TILE_DIVIDER_CROSS:
        case WINDOW_TILE_BORDER_LEFT_TEE:
          break;
        case WINDOW_TILE_BORDER_LEFT:
          tile = WINDOW_TILE_BORDER_LEFT_TEE;
          break;
        case WINDOW_TILE_DIVIDER_TEE_LEFT:
          tile = WINDOW_TILE_DIVIDER_CROSS;
          break;
        case WINDOW_TILE_DIVIDER_VERTICAL:
          tile = WINDOW_TILE_DIVIDER_TEE_RIGHT;
          break;
        default:
          tile = WINDOW_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      } else if (pos == x2) {
        switch (tile) {
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
        case WINDOW_TILE_DIVIDER_TEE_UP:
        case WINDOW_TILE_DIVIDER_TEE_LEFT:
        case WINDOW_TILE_DIVIDER_CROSS:
        case WINDOW_TILE_BORDER_RIGHT_TEE:
          break;
        case WINDOW_TILE_BORDER_RIGHT:
          tile = WINDOW_TILE_BORDER_RIGHT_TEE;
          break;
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
          tile = WINDOW_TILE_DIVIDER_CROSS;
          break;
        case WINDOW_TILE_DIVIDER_VERTICAL:
          tile = WINDOW_TILE_DIVIDER_TEE_LEFT;
          break;
        default:
          tile = WINDOW_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      } else {
        switch (tile) {
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
        case WINDOW_TILE_DIVIDER_TEE_UP:
        case WINDOW_TILE_DIVIDER_CROSS:
          break;
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
        case WINDOW_TILE_DIVIDER_TEE_LEFT:
        case WINDOW_TILE_DIVIDER_VERTICAL:
          tile = WINDOW_TILE_DIVIDER_CROSS;
          break;
        default:
          tile = WINDOW_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      }
      *cursor = tile;
      cursor++;
    }
  }
}

/// Sets the text colour (0 to 15) of drawn text. The menus colour the HP
/// figures with it (menu/menu_shared.c, window/screens_party_status.c), and
/// set TEXT_COLOR_WHITE back after.
void Text_SetColor(s32 param) {
  struct WindowWork *work = WINDOW_WORK;

  work->colour = param & 15;
}

/// Sets the text style (WindowWork.outline) of drawn text: 1 outlined, 2 to
/// 5 a motion of glyph sprites. Reached only through a bare entry of the
/// window far-call table.
void Text_SetStyle(s32 value) {
  struct WindowWork *work = WINDOW_WORK;

  work->outline = value;
}

void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y);

/// Decodes a message and draws it into a text canvas window at pixel (x, y),
/// all at once.
void Text_DrawMessage(s32 message, s32 window, s32 x, s32 y) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *counter = &canvas->textBufferEnd;
  s32 zero = 0;

  *counter = zero;
  Message_Decode(message, 1);
  canvas->textBuffer[*counter] = zero;
  *counter = (*counter + 1) & TEXT_BUFFER_MASK;
  Text_DrawWideStringAtOffset(canvas->textBuffer, (struct Window *)window, x,
                              y);
}

/// Decodes a message and draws it as tiles in a window at pixel (offsetX,
/// offsetY) of its interior. offsetY is rounded down to a tile row; offsetX
/// picks the tile, and its remainder is where the text starts in it. As
/// Text_DrawMessage, it decodes from the ring's start. Nothing is drawn
/// when the place falls outside the tilemap's 32 by 20 entries.
void Text_DrawMessageAsTiles(s32 character, struct Window *position,
                             u32 offsetX, u32 offsetY) {
  s32 byteOffset;
  s32 vramAddress;
  u16 *text;
  s32 zero;
  u32 cell;
  struct WindowWork *canvas;
  u16 *counter;

  canvas = WINDOW_WORK;
  counter = &canvas->textBufferEnd;
  zero = 0;
  *counter = zero;
  Message_Decode(character, 1);

  canvas->textBuffer[*counter] = zero;
  *counter = (u16)((*counter + 1) & TEXT_BUFFER_MASK);

  cell = (((s16)position->y + (offsetY >> 3) + 1) << 5) +
         ((s16)position->x + (offsetX >> 3)) + 1;
  if (cell < 640U) {
    byteOffset = cell * 2;
    vramAddress = byteOffset + (s32)WINDOW_TILEMAP_VRAM;
    text = canvas->textBuffer;
    Text_DrawStringAsTiles(text, (s32)((u8 *)canvas->tilemap + byteOffset),
                           vramAddress, 7 & offsetX);
  }
}

/// Draws a byte string into a text canvas window at pixel (x, y). The
/// 256-halfword scratch requires at most 255 input bytes before the terminator.
void Text_DrawString(u8 *text, s32 window, s32 x, s32 y) {
  u16 *buffer = Heap_AllocEwram(TEXT_WIDE_STRING_BUFFER_SIZE);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  *output = 0;
  Text_DrawWideStringAtOffset(buffer, (struct Window *)window, x, y);
  Heap_Release(buffer);
}

/// Draws a byte string as tiles in window work at pixel (offsetX, offsetY) of
/// its interior.
void Text_DrawStringAtOffset(u8 *text, struct Window *work, s32 offsetX,
                             s32 offsetY) {
  u16 *buffer;
  u16 *output;
  struct WindowWork *canvas;
  u32 cell;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  buffer = Heap_AllocEwram(TEXT_WIDE_STRING_BUFFER_SIZE);
  canvas = WINDOW_WORK;
  output = buffer;
  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  *output = 0;

  cell = (((s16)work->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 640) {
    vram = (s32)WINDOW_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles(buffer, (u16 *)src, (u16 *)dst, phase);
    Heap_Release(buffer);
  }
}

u8 *String_FormatNumber(u8 *output, s32 value, s32 width);

/// Draws a byte string into a window's tilemap at pixel (x, y) of its interior,
/// rounded down to tiles.
void Text_DrawStringInWindow(u8 *text, s32 window, u32 x, u32 y) {
  u16 *buffer = Heap_AllocEwram(TEXT_WIDE_STRING_BUFFER_SIZE);
  u16 *output = buffer;

  while (*text != 0) {
    *output = *text;
    text++;
    output++;
  }
  x >>= 3;
  *output = 0;
  y >>= 3;
  Text_DrawWideStringInWindow((s16 *)buffer, (struct Window *)window, x, y);
  Heap_Release(buffer);
}

/// The number writers format the value in a 16-byte buffer and draw it with
/// the matching string writer.
void Text_DrawNumber(s32 value, s32 width, s32 window, s32 x, s32 y) {
  u8 numberText[TEXT_NUMBER_BUFFER_SIZE];

  Text_DrawString(String_FormatNumber(numberText, value, width), window, x, y);
}

/// Draws a number (width as String_FormatNumber) as tiles in a window at pixel
/// (x, y) of its interior.
void Text_DrawNumberAtOffset(s32 value, s32 width, struct Window *window, s32 x,
                             s32 y) {
  u8 numberText[TEXT_NUMBER_BUFFER_SIZE];

  Text_DrawStringAtOffset(String_FormatNumber(numberText, value, width), window,
                          x, y);
}

/// Draws a number (width as String_FormatNumber) into a window's tilemap at
/// pixel (x, y) of its interior.
void Text_DrawNumberInWindow(s32 value, s32 width, s32 window, s32 x, s32 y) {
  u8 numberText[TEXT_NUMBER_BUFFER_SIZE];

  Text_DrawStringInWindow(String_FormatNumber(numberText, value, width), window,
                          x, y);
}

#undef TEXT_JAPANESE_FONT
#undef TEXT_CODE_3_WIDTH
#undef TEXT_IS_NARROW_TILE

/// Draws a four-digit number after a two-tile label as tiles in window work at
/// pixel (offsetX, offsetY): the HP label for variant 0, the party status
/// window's HP; the PP label for variant 1, its PP.
void Text_DrawPrefixedNumberAtOffset(s32 value, struct Window *work,
                                     s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[TEXT_NUMBER_BUFFER_SIZE];
  u16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  struct WindowWork *canvas;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  canvas = WINDOW_WORK;
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
  if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT) {
    vram = (s32)WINDOW_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}
