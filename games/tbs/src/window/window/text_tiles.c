/// @file
/// Text drawn as tiles, text tile ownership and divider lines.
// Part of window.c.

#include "../window_internal.h"

/// Draws window tile pattern over the tile of tilemap entry *entry, with its
/// colours mapped through remap (0 leaves a pixel as it was). A shared tile
/// first gets a free tile of its own, which entry and mirror then point to.
/// Only nonzero remap entries replace pixels; shared tiles become private
/// before upload. Nothing calls it.
UNUSED void Window_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern,
                                      u8 *remap) {
  struct WindowWork *work = WINDOW_WORK;
  u8 pixels[128];
  u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  u32 tile = *(u8 *)entry;
  u8 *src;
  u8 *dst;
  u32 n;
  u32 i;

  dst = pixels;
  src = BG_CHAR_BLOCK(0) + TILE_SIZE_4BPP * tile;
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

  if ((s8)tile >= 0) {
    for (n = 0; n < 128; n++) {
      u32 value = work->nextTextTile;
      u32 following = (value + 1) % 128;

      tile = (u8)value;
      work->nextTextTile = following;
      if (work->textTileState[tile] == 0)
        break;
    }
    work->textTileState[tile] = 1;
    tile |= 0x80;
    *entry = tile | 0xf000;
    *mirror = tile | 0xf000;
  }
  Dma_Set(pixels, BG_CHAR_BLOCK(0) + tile * TILE_SIZE_4BPP, DMA_COPY32(8),
          REG_DMA3);
}

/// Draws a string of 16-bit character codes with the window font into tiles
/// of their own, starting x pixels into the first tile, and points tiles (and
/// the same entries in mirror) at them. Colour codes change the text colour.
/// Returns how many tiles the text covers.
/// The canvas is eight rows of 256 pixels; the pen must stay within each row.
/// Text is one line: a new line code only moves the pen a space on. A tilemap
/// entry that already names a text tile keeps its tile, which is redrawn;
/// others get a new one, marked taken (attribute 1) until the tilemap stops
/// showing it (Window_MarkVisibleTextTiles) or its rectangle is cleared.
/// With every text tile taken, the search gives up and reuses the tile it
/// ends on. tiles is the work block's tilemap and mirror the same place in
/// VRAM, both written at once.
s32 Text_DrawStringAsTiles(u16 *text, u16 *tiles, u16 *mirror, s32 x) {
  /* FAKEMATCH(dead): an unused array for the frame's 32 extra bytes; as no
   * array, 2 of 402 instructions differ in tbs-en. */
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
  canvas = Alloc(0x800);
  glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  paletteBits = work->textPalette << 12;
  Iwram_ClearWords(pens, 16);
  // pens maps a font pixel, 1 or 3 (the font's two shades), to a colour; 0
  // leaves the background. In the UI palette (15) shade 1 takes the colour's
  // pen (gText_ColorPens) and shade 3 colour 3, over the window
  // background, colour 4; in another palette (WindowWork.textPalette, as the
  // party status window sets for its numbers) shade 1 takes the colour itself
  // and shade 3 colour 1, over colour 14.
  if (paletteBits == 0xF000) {
    pens[1] = gText_ColorPens[work->colour & 15];
    pens[3] = 3;
    Iwram_FillWords(canvas, 0x800, 0x04040404);
  } else {
    pens[1] = (u8)work->colour & 15;
    pens[3] = 1;
    Iwram_FillWords(canvas, 0x800, 0x0E0E0E0E);
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
          pens[1] = gText_ColorPens[work->colour & 15];
          break;
        // TEXT_COLORS_RESET, TEXT_OUTLINE and TEXT_LINE_SPACING all reset the
        // colour here, and the last two leave their operand to be read as the
        // next code.
        case TEXT_COLORS_RESET:
        case TEXT_OUTLINE:
        case TEXT_LINE_SPACING:
          work->colour = 15;
          pens[1] = gText_ColorPens[work->colour];
          break;
        // A new line is a space's width: tile text is one line.
        case TEXT_NEWLINE:
#if (LANGUAGE != LANGUAGE_JAPANESE)
          x += gFont_GlyphWidths[0];
#else
          x += 8;
#endif
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
        // A tilemap code (such as 0xf01d) draws window tile glyphCode & 0xff.
        // In Japanese a kana mark moves back 3 pixels onto the character
        // before it and draws from glyphs 254 and 255.
        glyphCode &= 0xFF;
#if (LANGUAGE == LANGUAGE_JAPANESE)
        if (glyphCode == 222 || glyphCode == 223) {
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
        // The sign tiles Text_DrawPrefixedNumberAtOffset writes (0xf01d to
        // 0xf01f, and 0xf005 in Spanish, French and Italian) advance 8 or 3
        // pixels; a character its font width, and any other tile below 32 one.
        if (code == 0xF01D || code == 0xF01F)
          x += 8;
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
        else if (code == 0xF01E || code == 0xF005)
#else
        else if (code == 0xF01E)
#endif
          x += 3;
        else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
          if (glyphCode > 128) {
            if (glyphCode == 254 || glyphCode == 255)
              glyphWidth = 3;
            else
              glyphWidth = 8;
          } else
#endif
              if (glyphCode > 31)
            glyphWidth = gFont_GlyphWidths[glyphCode - 32];
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
    dst = dst - tileCount * 4 + 0x100;
    src = (u32 *)((u8 *)src - tileCount * 8 + 0x100);
  }

  // Reuse private text tiles, allocate any new ones, then upload each tile.
  for (index = 0; index < tileCount; index++) {
    u32 mask = 127;
    u32 tile;
    u32 *vramTile;

    if (work->extraTextTiles != 0)
      mask = 255;
    // Attribute n is text tile 0x80 + n, and with alt set 128 to 255 are
    // tiles 0x200 to 0x27f.
    tile = *tiles & 0x3FF;
    if ((tile < 0x80 || tile > 0xFF) &&
        (work->extraTextTiles == 0 || tile < 0x200 || tile > 0x27F)) {
      tile = work->nextTextTile & mask;
      for (column = 0; column <= mask && work->textTileState[tile] != 0;
           column++)
        tile = (tile + 1) & mask;
      work->nextTextTile = (tile + 1) & mask;
      work->textTileState[tile] = 1;
      if (tile > 127)
        tile += 0x180;
      else
        tile |= 0x80;
      *tiles = paletteBits | tile;
      *mirror = paletteBits | tile;
    }
    vramTile = (u32 *)(BG_CHAR_BLOCK(0) + tile * TILE_SIZE_4BPP);
    *vramTile++ = ((u32 *)canvas)[index];
    *vramTile++ = ((u32 *)canvas)[index + 0x40];
    *vramTile++ = ((u32 *)canvas)[index + 0x80];
    *vramTile++ = ((u32 *)canvas)[index + 0xC0];
    *vramTile++ = ((u32 *)canvas)[index + 0x100];
    *vramTile++ = ((u32 *)canvas)[index + 0x140];
    *vramTile++ = ((u32 *)canvas)[index + 0x180];
    *vramTile++ = ((u32 *)canvas)[index + 0x1C0];
    tiles++;
    mirror++;
  }
  Free(canvas);
  return tileCount;
}

/// Frees the text tiles under a tilemap rectangle (x, y, width and height in
/// tiles) for reuse: their taken and shown bits are cleared. The callers do
/// this before overwriting the rectangle's entries.
/// Only the dynamic text-tile ranges have allocation attributes to clear;
/// the second range only while alt is set.
void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height) {
  u8 *base = gWindowWork[0];
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
  u32 alternateTiles = ((struct WindowWork *)base)->extraTextTiles;
  u32 row;

  for (row = 0; row < height; row++) {
    u32 column;

    for (column = 0; column < width; column++) {
      u32 tileId = *cursor++ & 0x3FF;

      if ((tileId - 0x80) <= 0x7F ||
          (alternateTiles != 0 && tileId > 0x1FF && tileId <= 0x27F)) {
        u32 attributeIndex = ((tileId & 0xFF) ^ 0x80);
        ((struct WindowWork *)base)->textTileState[attributeIndex] &= 0xFC;
      }
    }
    cursor += 32 - width;
  }
}

/// Marks the text tiles the tilemap shows, and frees those it no longer shows:
/// a tile taken but not marked shown (attribute exactly 1). The shown bit
/// stays set afterwards, until the tile's rectangle is cleared. The field
/// menus (menu/menu/djinn.c, djinn_preview.c, item.c, psynergy.c and status.c)
/// and the battle menus (screens/djinn_menu.c, party_commands.c and
/// status.c) call it.
void Window_MarkVisibleTextTiles(void) {
  u8 *base = gWindowWork[0];
  u16 *cursor = (u16 *)base;
  s32 width = 30;
  u32 alternateTiles = ((struct WindowWork *)base)->extraTextTiles;
  s32 row;
  s32 x;

  // The cursor never skips the two hidden columns that end each 32-entry
  // row, so the scan covers the tilemap's first 600 entries in order: row 18
  // from column 24 on and row 19 are never marked shown.
  for (row = 20; row != 0; row--) {
    for (x = 0; x < width; x++) {
      u32 tileId = *cursor++ & 0x3ff;

      if ((tileId - 0x80) <= 0x7f ||
          (alternateTiles != 0 && tileId > 0x1ff && tileId <= 0x27f)) {
        u32 attributeIndex = ((tileId & 0xff) ^ 0x80);

        ((struct WindowWork *)base)->textTileState[attributeIndex] |= 2;
      }
    }
  }

  for (row = 0; row < 256; row++, base++) {
    if (((struct WindowWork *)base)->textTileState[0] == 1)
      ((struct WindowWork *)base)->textTileState[0] = 0;
  }
}

/// Frees the text tiles of the second range (attributes 128 to 255, tiles
/// 0x200 to 0x27f), and with allowAlternateTiles set lets text tiles take that
/// range too; the first range's tiles keep their state. Both branches clear
/// the same 128 attributes.
/// The second range extends text capacity from 128 to 256 tiles. The field's
/// Djinn, item, Psynergy and status menus and the battle's Djinn menu set it
/// while open and clear it on leaving.
void Text_UseExtraTiles(s32 allowAlternateTiles) {
  s32 i;
  s32 j;
  s8 *attributes;
  s8 *tileAttributes;
  u8 *work;

  work = gWindowWork[0];
  if (allowAlternateTiles != 0) {
    ((struct WindowWork *)work)->extraTextTiles = 1;
    allowAlternateTiles = 0;
    for (i = 0x80,
        attributes = (s8 *)((struct WindowWork *)work)->textTileState + 0x80;
         i <= 0xFF; i += 1) {
      *attributes = allowAlternateTiles;
      attributes += 1;
    }
    return;
  }
  ((struct WindowWork *)work)->extraTextTiles = 0;
  allowAlternateTiles = 0;
  tileAttributes = (s8 *)((struct WindowWork *)work)->textTileState + 0x80;
  j = 0x7F;
  do {
    j -= 1;
    *tileAttributes = allowAlternateTiles;
    tileAttributes += 1;
  } while (j >= 0);
}

/// Nothing calls it.
UNUSED void Window_ReservedNoOp(void) {}

/// Draws a straight divider line inside a window from tile (x1, y1) to
/// (x2, y2), horizontal or vertical: its ends join the frame or the lines they
/// meet, and it crosses the lines it passes over.
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
    Window_ClearTextTilesInRect(win->x + x2, win->y + y1, 1, y2 - y1);
    cursor =
        (u16 *)((((win->y + y1) << 5) + win->x + x2) * 2 + (u32)work->tilemap);
    for (pos = y1; pos <= y2; pos++) {
      tile = *cursor;
      if (pos == y1) {
        switch (tile) {
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
        case WINDOW_TILE_DIVIDER_TEE_LEFT:
        case WINDOW_TILE_DIVIDER_CROSS:
        case WINDOW_TILE_FRAME_TOP_TEE:
          break;
        case WINDOW_TILE_FRAME_TOP:
          tile = WINDOW_TILE_FRAME_TOP_TEE;
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
        case WINDOW_TILE_FRAME_BOTTOM_TEE:
          break;
        case WINDOW_TILE_FRAME_BOTTOM:
          tile = WINDOW_TILE_FRAME_BOTTOM_TEE;
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
    Window_ClearTextTilesInRect(win->x + x1, win->y + y1, x2 - x1, 1);
    cursor =
        (u16 *)((((win->y + y1) << 5) + win->x + x1) * 2 + (u32)work->tilemap);
    for (pos = x1; pos <= x2; pos++) {
      tile = *cursor;
      if (pos == x1) {
        switch (tile) {
        case WINDOW_TILE_DIVIDER_TEE_DOWN:
        case WINDOW_TILE_DIVIDER_TEE_UP:
        case WINDOW_TILE_DIVIDER_TEE_RIGHT:
        case WINDOW_TILE_DIVIDER_CROSS:
        case WINDOW_TILE_FRAME_LEFT_TEE:
          break;
        case WINDOW_TILE_FRAME_LEFT:
          tile = WINDOW_TILE_FRAME_LEFT_TEE;
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
        case WINDOW_TILE_FRAME_RIGHT_TEE:
          break;
        case WINDOW_TILE_FRAME_RIGHT:
          tile = WINDOW_TILE_FRAME_RIGHT_TEE;
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
/// figures with it (menu/menu/common.c, screens/party_status.c), and
/// set colour 15 back after.
void Text_SetColor(s32 param) {
  struct WindowWork *work = WINDOW_WORK;

  work->colour = param & 15;
}

/// Sets the text style (WindowWork.outline) of drawn text: 1 outlined, 2 to
/// 5 a motion of glyph sprites. Reached only through a bare entry of the
/// window far-call table.
void Text_SetStyle(u16 value) {
  struct WindowWork *work = WINDOW_WORK;

  work->outline = value;
}

/// Decodes a message and draws it into a text canvas window at pixel (x, y),
/// all at once. It decodes from the start of the ring of decoded text (count
/// 0), overwriting whatever decoded text was there, and uses up the message
/// arguments.
void Text_DrawMessage(s32 message, struct Window *window, s32 x, s32 y) {
  struct WindowWork *canvas = WINDOW_WORK;
  u16 *counter = &canvas->textBufferEnd;
  s32 zero = 0;

  *counter = zero;
  Message_Decode(message, 1);
  canvas->textBuffer[*counter] = zero;
  *counter = (*counter + 1) & TEXT_BUFFER_MASK;
  Text_DrawWideStringAtOffset(canvas->textBuffer, window, x, y);
}

/// Decodes a message and draws it as tiles in a window at pixel (offsetX,
/// offsetY) of its interior. offsetY is rounded down to a tile row; offsetX
/// picks the tile, and its remainder is where the text starts in it. As
/// Text_DrawMessage, it decodes from the ring's start. Nothing is drawn
/// when the place falls outside the tilemap's 32 by 20 entries.
void Text_DrawMessageAsTiles(s32 message, const struct Window *window,
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
  Message_Decode(message, 1);

  canvas->textBuffer[*counter] = zero;
  *counter = (*counter + 1) & TEXT_BUFFER_MASK;

  cell = ((window->y + (offsetY >> 3) + 1) << 5) +
         (window->x + (offsetX >> 3)) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    byteOffset = cell * 2;
    vramAddress = byteOffset + (s32)WINDOW_TILEMAP_VRAM;
    text = canvas->textBuffer;
    Text_DrawStringAsTiles(text, (u16 *)((u8 *)canvas->tilemap + byteOffset),
                           (u16 *)vramAddress, 7 & offsetX);
  }
}

/// Copies a zero-terminated byte string into the text renderer's halfwords.
/// The destination needs one halfword per input byte, including the terminator.
static __inline__ void Text_WidenByteString(const u8 *input, u16 *output) {
  if (*input != 0) {
    do {
      *output = *input;
      input++;
      output++;
    } while (*input != 0);
  }
  *output = 0;
}

/// Draws a byte string into a text canvas window at pixel (x, y).
/// The 256-halfword scratch requires at most 255 input bytes before the
/// terminator. The scratch comes from EWRAM, or IWRAM when EWRAM is full
/// (AllocEwram), and is freed before returning.
void Text_DrawString(const u8 *text, struct Window *window, s32 x, s32 y) {
  s16 *buffer;
  const u8 *input;

  input = text;
  buffer = AllocEwram(TEXT_WIDE_STRING_BUFFER_SIZE);
  Text_WidenByteString(input, (u16 *)buffer);
  Text_DrawWideStringAtOffset((u16 *)buffer, window, x, y);
  Free(buffer);
}

/// Draws a byte string as tiles in window work at pixel (offsetX, offsetY) of
/// its interior.
/// The conversion scratch has the same 255-byte input limit as
/// Text_DrawString.
void Text_DrawStringAtOffset(const u8 *text, const struct Window *window,
                             s32 offsetX, s32 offsetY) {
  s16 *buffer;
  struct WindowWork *canvas;
  u32 cell;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  buffer = AllocEwram(TEXT_WIDE_STRING_BUFFER_SIZE);
  canvas = WINDOW_WORK;
  Text_WidenByteString(text, (u16 *)buffer);

  cell = ((window->y + ((u32)offsetY >> 3) + 1) << 5) +
         (window->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    vram = (s32)WINDOW_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)buffer, (u16 *)src, (u16 *)dst, phase);
    // The scratch is freed only here: a place outside the tilemap leaves it
    // allocated.
    Free(buffer);
  }
}

/// Draws a byte string into a window's tilemap at pixel (x, y) of its
/// interior, rounded down to tiles.
/// Bytes widen to character codes; pixel coordinates round down to tile
/// coordinates.
void Text_DrawStringInWindow(const u8 *text, struct Window *window, u32 x,
                             u32 y) {
  u16 *buffer = AllocEwram(TEXT_WIDE_STRING_BUFFER_SIZE);
  Text_WidenByteString(text, buffer);
  x >>= 3;
  y >>= 3;
  Text_DrawWideStringInWindow((s16 *)buffer, window, x, y);
  Free(buffer);
}

/// Draws a number (width as String_FormatNumber) into a text canvas window at
/// pixel (x, y).
void Text_DrawNumber(s32 value, s32 width, struct Window *window, s32 x,
                     s32 y) {
  u8 numberText[TEXT_NUMBER_BUFFER_SIZE];

  Text_DrawString(String_FormatNumber(numberText, value, width), window, x, y);
}

/// Draws a number (width as String_FormatNumber) as tiles in a window at pixel
/// (x, y) of its interior.
void Text_DrawNumberAtOffset(s32 value, s32 width, const struct Window *window,
                             s32 x, s32 y) {
  u8 numberText[TEXT_NUMBER_BUFFER_SIZE];

  Text_DrawStringAtOffset(String_FormatNumber(numberText, value, width), window,
                          x, y);
}

/// Draws a number (width as String_FormatNumber) into a window's tilemap at
/// pixel (x, y) of its interior.
void Text_DrawNumberInWindow(s32 value, s32 width, struct Window *window, s32 x,
                             s32 y) {
  u8 numberText[TEXT_NUMBER_BUFFER_SIZE];

  Text_DrawStringInWindow(String_FormatNumber(numberText, value, width), window,
                          x, y);
}

/// Draws a four-digit number after a two-glyph sign as tiles in window work at
/// pixel (offsetX, offsetY) of its interior: window tiles 0x1d and 0x1e for
/// variant 0, the party status window's HP; 0x1f and 0x1e for variant 1, its
/// PP (0x1f and 0x05 in Spanish, French and Italian). The number is padded
/// to four places with spaces.
void Text_DrawPrefixedNumberAtOffset(s32 value, const struct Window *window,
                                     s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[TEXT_NUMBER_BUFFER_SIZE];
  s16 output[8];
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
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
  if (variant == 0) {
    output[0] = 0xf01d;
    output[1] = 0xf01e;
  } else {
    output[0] = 0xf01f;
    output[1] = 0xf005;
  }
#else
  if (variant == 0) {
    output[0] = 0xf01d;
  } else {
    output[0] = 0xf01f;
  }
  output[1] = 0xf01e;
#endif
  for (index = 0; index <= 4; index++) {
    output[index + 2] = text[index];
  }
  output[6] = 0;

  cell = ((window->y + ((u32)offsetY >> 3) + 1) << 5) +
         (window->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    vram = (s32)WINDOW_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    Text_DrawStringAsTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}
