/// @file
/// Text drawn as tilemap tiles of its own (the menus' labels, names and
/// numbers), the text tiles taken and freed, divider lines, and the routines
/// that draw a message, string or number into a window.
// Part of window.c.

#include "window_internal.h"
#include "resource_ids.h"

/// A text tile's state (WindowWork.textTileState): taken by text, and shown on
/// the tilemap at the last look.
#define TEXT_TILE_TAKEN 1
#define TEXT_TILE_SHOWN 2
/// A text tile's place in WindowWork.textTileState: the TEXT_TILES tiles from
/// TEXT_TILE_FIRST take the first places, those from TEXT_EXTRA_TILE_FIRST the
/// next TEXT_TILES.
#define TEXT_TILE_STATE_INDEX(tile) (((tile) & 0xFF) ^ TEXT_TILE_FIRST)

/// Tile text is drawn into a canvas of one byte a pixel, a tile tall, then
/// packed into 4bpp tiles in place: row `row` of its tile `tile` is then a word
/// of eight pixels.
#define TILE_TEXT_CANVAS_WIDTH 256
#define TILE_TEXT_CANVAS_SIZE (TILE_TEXT_CANVAS_WIDTH * 8)
#define TILE_TEXT_PACKED_ROW(tile, row)                                        \
  (((u32 *)canvas)[(tile) + (row) * TILE_TEXT_CANVAS_WIDTH / 4])

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
  for (i = 0; i < TILE_SIZE_4BPP; i++) {
    n = *src++;
    *dst++ = n & (PALETTE_COLORS - 1);
    *dst++ = n >> 4;
  }

  dst = pixels;
  src = sheet + pattern * TILE_SIZE_4BPP;
  for (i = 0; i < TILE_SIZE_4BPP; i++) {
    u32 value = *src++;
    u32 color = remap[value & (PALETTE_COLORS - 1)];

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
    for (i = 0, p = dst; i < TILE_SIZE_4BPP; i++) {
      u32 value = p[0] | p[1] << 4;

      p += 2;
      *dst++ = value;
    }
  }

  if ((s8)slot >= 0) {
    n = 0;
    do {
      u32 value = work->nextTextTile;
      u32 following = (value + 1) % TEXT_TILES;

      slot = (u8)value;
      work->nextTextTile = following;
      if (work->textTileState[slot] == 0)
        break;
      n++;
    } while (n < TEXT_TILES);
    work->textTileState[slot] = TEXT_TILE_TAKEN;
    slot |= TEXT_TILE_FIRST;
    *entry = WINDOW_TILE(slot);
    *mirror = WINDOW_TILE(slot);
  }
  Dma_Set(pixels, BG_CHAR_BLOCK(0) + slot * TILE_SIZE_4BPP,
          DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
}

/// A new line in tile text advances by a space's width: tile text is one line.
#if LANGUAGE != LANGUAGE_JAPANESE
#define TILE_TEXT_NEWLINE_WIDTH gFont_GlyphWidths[0]
#else
#define TILE_TEXT_NEWLINE_WIDTH 8
#endif

/// In Japanese, the codes above TILE_TEXT_NARROW_LAST (the kana) are a whole
/// tile wide; a kana's mark is drawn from a tile glyph of its own,
/// TILE_TEXT_MARK_GLYPH for the voiced mark and the one after it for the
/// semi-voiced, three pixels wide and three pixels back over its kana.
#define TILE_TEXT_NARROW_LAST 128
#define TILE_TEXT_MARK_GLYPH 254
#define TILE_TEXT_MARK_WIDTH 3

/// A label's end tile advances three pixels: the HP label's in every font, and
/// the PP label's own in the French, Spanish and Italian fonts.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define TEXT_IS_NARROW_TILE(code)                                              \
  ((code) == WINDOW_TILE_HP_LABEL_END || (code) == WINDOW_TILE_PP_LABEL_END)
#else
#define TEXT_IS_NARROW_TILE(code) ((code) == WINDOW_TILE_HP_LABEL_END)
#endif

extern const u8 gFont_ColorPens[];

/// Draws a string of 16-bit character codes with the window font into tiles of
/// their own, starting x pixels into the first tile, and points tiles (and the
/// same entries in mirror) at them.
s32 Text_DrawStringAsTiles(u16 *text, u16 *tiles, u16 *mirror, s32 x) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
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
  canvas = Heap_Alloc(TILE_TEXT_CANVAS_SIZE);
  glyphs = Resource_GetTableEntry(RESOURCE_WINDOW_TILES);
  paletteBits = BG_TILE_PALETTE(work->textPalette);
  Iwram_ClearWords(pens, 16);
  // pens maps a font pixel, 1 or 3 (the font's two shades), to a colour; 0
  // leaves the background.
  if (paletteBits == BG_TILE_PALETTE(WINDOW_PALETTE)) {
    pens[1] = gFont_ColorPens[work->colour & (PALETTE_COLORS - 1)];
    pens[3] = 3;
    Iwram_FillWords(canvas, TILE_TEXT_CANVAS_SIZE,
                    PIXELS_8BPP(WINDOW_BACKGROUND_COLOR));
  } else {
    pens[1] = (u8)work->colour & (PALETTE_COLORS - 1);
    pens[3] = 1;
    Iwram_FillWords(canvas, TILE_TEXT_CANVAS_SIZE,
                    PIXELS_8BPP(TEXT_BACKGROUND_COLOR));
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
          pens[1] = gFont_ColorPens[work->colour & (PALETTE_COLORS - 1)];
          break;
        // TEXT_COLORS_RESET, TEXT_OUTLINE and TEXT_LINE_SPACING all reset the
        // colour here, and the last two leave their operand to be read as the
        // next code.
        case TEXT_COLORS_RESET:
        case TEXT_OUTLINE:
        case TEXT_LINE_SPACING:
          work->colour = TEXT_COLOR_WHITE;
          pens[1] = gFont_ColorPens[work->colour];
          break;
        case TEXT_NEWLINE:
          x += TILE_TEXT_NEWLINE_WIDTH;
          break;
        // The codes tile text does not act on are skipped with their
        // operands, as are codes 11, 12 and 14, whose meaning is open
        // (window_decode.c).
        case 14:
        case TEXT_WINDOW_STATE:
        case TEXT_ACTOR_NAME_START:
          text++;
          // Skip the second operand too.
          // Fall through.
        case 11:
        case 12:
        case TEXT_ACTOR_NAME:
#if LANGUAGE != LANGUAGE_JAPANESE
        case TEXT_ARTICLE_CLASS:
#endif
          text++;
          break;
        }
      } else {
        // A tilemap code (such as WINDOW_TILE_HP_LABEL) draws window tile
        // glyphCode & 0xff.
        glyphCode &= 0xFF;
#if LANGUAGE == LANGUAGE_JAPANESE
        if (glyphCode - TEXT_MARK_VOICED <= 1) {
          x -= TILE_TEXT_MARK_WIDTH;
          glyphCode += TILE_TEXT_MARK_GLYPH - TEXT_MARK_VOICED;
        }
#endif
        // Each glyph is an 8x8 tile of the window tiles, a word a row.
        src = (u32 *)(glyphs + glyphCode * TILE_SIZE_4BPP);
        dst = canvas + x;
        for (index = 0; index < 8; index++) {
          bits = *src++;
          for (column = 0; column < 4; column++) {
            u8 pen = pens[bits & (PALETTE_COLORS - 1)];

            if (pen != 0)
              *dst = pen;
            dst++;
            bits >>= 4;
            pen = pens[bits & (PALETTE_COLORS - 1)];
            if (pen != 0)
              *dst = pen;
            dst++;
            bits >>= 4;
          }
          dst += TILE_TEXT_CANVAS_WIDTH - 8;
        }
        // The sign tiles Text_DrawPrefixedNumberAtOffset writes (the HP and PP
        // labels' tiles) advance 8 for a first tile and 3 for a second, in
        // pixels; a character its font width, and any other tile below 32 one.
        if (code == WINDOW_TILE_HP_LABEL || code == WINDOW_TILE_PP_LABEL)
          x += 8;
        else if (TEXT_IS_NARROW_TILE(code))
          x += 3;
        else {
#if LANGUAGE == LANGUAGE_JAPANESE
          if (glyphCode > TILE_TEXT_NARROW_LAST)
            glyphWidth = glyphCode - TILE_TEXT_MARK_GLYPH <= 1
                             ? TILE_TEXT_MARK_WIDTH
                             : 8;
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
    dst = dst - tileCount * 4 + TILE_TEXT_CANVAS_WIDTH;
    src = (u32 *)((u8 *)src - tileCount * 8 + TILE_TEXT_CANVAS_WIDTH);
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
      work->textTileState[tile] = TEXT_TILE_TAKEN;
      if (tile > (TEXT_TILES - 1))
        tile += TEXT_EXTRA_TILE_FIRST - TEXT_TILES;
      else
        tile |= TEXT_TILE_FIRST;
      *tiles = paletteBits | tile;
      *mirror = paletteBits | tile;
    }
    vramTile = (u32 *)(BG_CHAR_BLOCK(0) + tile * TILE_SIZE_4BPP);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 0);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 1);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 2);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 3);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 4);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 5);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 6);
    *vramTile++ = TILE_TEXT_PACKED_ROW(index, 7);
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
        u32 index = TEXT_TILE_STATE_INDEX(tile);

        ((struct WindowWork *)base)->textTileState[index] &=
            ~(TEXT_TILE_TAKEN | TEXT_TILE_SHOWN);
      }
    }
    cursor += BG_SCREEN_WIDTH - width;
  }
}

/// Marks the text tiles the tilemap shows, and frees those it no longer shows:
/// a tile taken but not marked shown (its state exactly TEXT_TILE_TAKEN).
void Window_MarkVisibleTextTiles(void) {
  u8 *base = Ram_HeapSlots->windowWork;
  u32 alternate = ((struct WindowWork *)base)->extraTextTiles;
  u16 *cursor = ((struct WindowWork *)base)->tilemap[0];
  s32 width = DISPLAY_TILE_WIDTH;
  s32 row;
  s32 x;

  // BUG: the cursor never skips the two hidden columns that end each row of
  // BG_SCREEN_WIDTH entries, so the scan covers the tilemap's first
  // DISPLAY_TILE_WIDTH * DISPLAY_TILE_HEIGHT entries in order, and the text
  // tiles of the last row and a half are never marked shown: the loop below
  // frees them while they are still on the screen.
  for (row = DISPLAY_TILE_HEIGHT; row != 0; row--) {
    for (x = 0; x < width; x++) {
      u32 tile = *cursor++ & TILE_NUMBER_MASK;

      if ((tile - TEXT_TILE_FIRST) <= TEXT_TILES - 1 ||
          (alternate != 0 && tile > (TEXT_EXTRA_TILE_FIRST - 1) &&
           tile <= TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1)) {
        u32 index = TEXT_TILE_STATE_INDEX(tile);

        ((struct WindowWork *)base)->textTileState[index] |= TEXT_TILE_SHOWN;
      }
    }
#ifdef BUGFIX
    cursor += BG_SCREEN_WIDTH - width;
#endif
  }

  for (row = 0; row < 2 * TEXT_TILES; row++, base++) {
    if (((struct WindowWork *)base)->textTileState[0] == TEXT_TILE_TAKEN)
      ((struct WindowWork *)base)->textTileState[0] = 0;
  }
}

/// Frees the text tiles of the second range (the TEXT_TILES tiles from
/// TEXT_EXTRA_TILE_FIRST), and with allowAlternateTiles set lets text take that
/// range too; the first range's tiles keep their state.
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

/// Unused: does nothing.
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
      cursor += BG_SCREEN_WIDTH;
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

/// Sets the text colour (enum TextColor) of drawn text. The menus colour the
/// HP figures with it, and set TEXT_COLOR_WHITE back after.
void Text_SetColor(s32 color) {
  struct WindowWork *work = WINDOW_WORK;

  work->colour = color & (PALETTE_COLORS - 1);
}

/// Sets the text style (enum TextStyle) of drawn text: outlined, or a motion
/// of the glyph sprites. Reached only through a bare entry of the window
/// far-call table.
void Text_SetStyle(s32 style) {
  struct WindowWork *work = WINDOW_WORK;

  work->outline = style;
}

/// Decodes a message and draws it into a text canvas window at pixel (x, y),
/// all at once.
void Text_DrawMessage(s32 message, s32 window, s32 x, s32 y) {
  struct WindowWork *canvas = WINDOW_WORK;

  canvas->textBufferEnd = 0;
  Message_Decode(message, 1);
  canvas->textBuffer[canvas->textBufferEnd] = 0;
  canvas->textBufferEnd = (canvas->textBufferEnd + 1) & TEXT_BUFFER_MASK;
  Text_DrawWideStringAtOffset(canvas->textBuffer, (struct Window *)window, x,
                              y);
}

/// Decodes a message and draws it as tiles in a window at pixel (offsetX,
/// offsetY) of its interior. offsetY is rounded down to a tile row; offsetX
/// picks the tile, and its remainder is where the text starts in it.
void Text_DrawMessageAsTiles(s32 message, struct Window *window, u32 offsetX,
                             u32 offsetY) {
  s32 byteOffset;
  s32 vramAddress;
  u16 *text;
  u32 cell;
  struct WindowWork *canvas;

  canvas = WINDOW_WORK;
  canvas->textBufferEnd = 0;
  Message_Decode(message, 1);

  canvas->textBuffer[canvas->textBufferEnd] = 0;
  canvas->textBufferEnd = (canvas->textBufferEnd + 1) & TEXT_BUFFER_MASK;

  cell = (((s16)window->y + (offsetY >> 3) + 1) << 5) +
         ((s16)window->x + (offsetX >> 3)) + 1;
  if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT) {
    byteOffset = cell * 2;
    vramAddress = byteOffset + (s32)WINDOW_TILEMAP_VRAM;
    text = canvas->textBuffer;
    Text_DrawStringAsTiles(text, (u16 *)((u8 *)canvas->tilemap + byteOffset),
                           (u16 *)vramAddress, 7 & offsetX);
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

/// Draws a byte string as tiles in window at pixel (offsetX, offsetY) of its
/// interior.
void Text_DrawStringAtOffset(u8 *text, struct Window *window, s32 offsetX,
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

  cell = (((s16)window->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)window->x + ((u32)offsetX >> 3)) + 1;
  if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT) {
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

/// Draws a number (width as String_FormatNumber) into a text canvas window at
/// pixel (x, y).
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

#undef TILE_TEXT_NEWLINE_WIDTH
#undef TILE_TEXT_NARROW_LAST
#undef TILE_TEXT_MARK_GLYPH
#undef TILE_TEXT_MARK_WIDTH
#undef TEXT_IS_NARROW_TILE

/// Draws a four-digit number after a two-tile label as tiles in window at
/// pixel (offsetX, offsetY): the HP label for variant 0, the party status
/// window's HP; the PP label for variant 1, its PP.
void Text_DrawPrefixedNumberAtOffset(s32 value, struct Window *window,
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
  cell = (((s16)window->y + ((u32)offsetY >> 3) + 1) << 5) +
         ((s16)window->x + ((u32)offsetX >> 3)) + 1;
  if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT) {
    vram = (s32)WINDOW_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}

#undef TEXT_TILE_TAKEN
#undef TEXT_TILE_SHOWN
#undef TEXT_TILE_STATE_INDEX
#undef TILE_TEXT_CANVAS_WIDTH
#undef TILE_TEXT_CANVAS_SIZE
#undef TILE_TEXT_PACKED_ROW
