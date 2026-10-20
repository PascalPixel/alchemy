/// @file
/// Text drawn as tiles, text tile ownership and divider lines.
/// Part of window.c, the window object, compiled only through it.

#include "../window_internal.h"

/// Draws window tile pattern over the tile of tilemap entry *entry, with its
/// colours mapped through remap (0 leaves a pixel as it was). A shared tile
/// first gets a free tile of its own, which entry and mirror then point to.
/// Only nonzero remap entries replace pixels; shared tiles become private
/// before upload. Nothing calls it.
UNUSED void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern,
                                        u8 *remap) {
  struct UiRenderWork *work = UI_WORK;
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
      u32 value = work->tileCursor;
      u32 following = (value + 1) % 128;

      tile = (u8)value;
      work->tileCursor = following;
      if (work->tileAttributes[tile] == 0)
        break;
    }
    work->tileAttributes[tile] = 1;
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
s32 UiText_RenderStringTiles(u16 *text, u16 *tiles, u16 *mirror, s32 x) {
  /* FAKEMATCH(dead): an unused array for the frame's 32 extra bytes; as no
   * array, 2 of 402 instructions differ in tbs-en. */
  UNUSED u32 reserve[8];
  u8 pens[16];
  struct UiRenderWork *work;
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

  work = UI_WORK;
  canvas = MemAlloc(0x800);
  glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
  paletteBits = work->level << 12;
  Iwram_ClearWords(pens, 16);
  if (paletteBits == 0xF000) {
    pens[1] = gUiText_ColourPens[work->colour & 15];
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
      if (glyphCode <= 30) {
        switch (glyphCode) {
        case 8:
          work->colour = *text++;
          pens[1] = gUiText_ColourPens[work->colour & 15];
          break;
        case 7:
        case 9:
        case 10:
          work->colour = 15;
          pens[1] = gUiText_ColourPens[work->colour];
          break;
        case 3:
#if (LANGUAGE != LANGUAGE_JAPANESE)
          x += gUiText_GlyphWidths[0];
#else
          x += 8;
#endif
          break;
        case 14:
        case 15:
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
        glyphCode &= 0xFF;
#if (LANGUAGE == LANGUAGE_JAPANESE)
        if (glyphCode == 222 || glyphCode == 223) {
          x -= 3;
          glyphCode += 32;
        }
#endif
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
            glyphWidth = gUiText_GlyphWidths[glyphCode - 32];
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

    if (work->alt != 0)
      mask = 255;
    tile = *tiles & 0x3FF;
    if ((tile < 0x80 || tile > 0xFF) &&
        (work->alt == 0 || tile < 0x200 || tile > 0x27F)) {
      tile = work->tileCursor & mask;
      for (column = 0; column <= mask && work->tileAttributes[tile] != 0;
           column++)
        tile = (tile + 1) & mask;
      work->tileCursor = (tile + 1) & mask;
      work->tileAttributes[tile] = 1;
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
  MemFree(canvas);
  return tileCount;
}

/// Marks the text tiles under a tilemap rectangle (x, y, width and height in
/// tiles) as no longer shown, for reuse.
/// Only the dynamic text-tile ranges have allocation attributes to clear.
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height) {
  u8 *base = gWindowWork[0];
  u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
  u32 alternateTiles = ((struct UiRenderWork *)base)->alt;
  u32 row;

  for (row = 0; row < height; row++) {
    u32 column;

    for (column = 0; column < width; column++) {
      u32 tileId = *cursor++ & 0x3FF;

      if ((tileId - 0x80) <= 0x7F ||
          (alternateTiles != 0 && tileId > 0x1FF && tileId <= 0x27F)) {
        u32 attributeIndex = ((tileId & 0xFF) ^ 0x80);
        ((struct UiRenderWork *)base)->tileAttributes[attributeIndex] &= 0xFC;
      }
    }
    cursor += 32 - width;
  }
}

/// Marks the text tiles the tilemap shows, and frees those it no longer shows.
void UiWindow_MarkVisibleTileAttributes(void) {
  u8 *base = gWindowWork[0];
  u16 *cursor = (u16 *)base;
  s32 width = 30;
  u32 alternateTiles = ((struct UiRenderWork *)base)->alt;
  s32 row;
  s32 x;

  for (row = 20; row != 0; row--) {
    for (x = 0; x < width; x++) {
      u32 tileId = *cursor++ & 0x3ff;

      if ((tileId - 0x80) <= 0x7f ||
          (alternateTiles != 0 && tileId > 0x1ff && tileId <= 0x27f)) {
        u32 attributeIndex = ((tileId & 0xff) ^ 0x80);

        ((struct UiRenderWork *)base)->tileAttributes[attributeIndex] |= 2;
      }
    }
  }

  for (row = 0; row < 256; row++, base++) {
    if (((struct UiRenderWork *)base)->tileAttributes[0] == 1)
      ((struct UiRenderWork *)base)->tileAttributes[0] = 0;
  }
}

/// Frees every text tile, and with flag set lets text tiles take the second
/// tile range (0x200 to 0x27f) too.
/// The second range extends text capacity from 128 to 256 tiles.
void UiWork_SetAltFlagAndClearTable(s32 allowAlternateTiles) {
  s32 i;
  s32 j;
  s8 *attributes;
  s8 *tileAttributes;
  u8 *work;

  work = gWindowWork[0];
  if (allowAlternateTiles != 0) {
    ((struct UiRenderWork *)work)->alt = 1;
    allowAlternateTiles = 0;
    for (i = 0x80,
        attributes = (s8 *)((struct UiRenderWork *)work)->tileAttributes + 0x80;
         i <= 0xFF; i += 1) {
      *attributes = allowAlternateTiles;
      attributes += 1;
    }
    return;
  }
  ((struct UiRenderWork *)work)->alt = 0;
  allowAlternateTiles = 0;
  tileAttributes = (s8 *)((struct UiRenderWork *)work)->tileAttributes + 0x80;
  j = 0x7F;
  do {
    j -= 1;
    *tileAttributes = allowAlternateTiles;
    tileAttributes += 1;
  } while (j >= 0);
}

/// Nothing calls it.
UNUSED void UiRender_ReservedNoOp(void) {}

/// Draws a straight divider line inside a window from tile (x1, y1) to
/// (x2, y2), horizontal or vertical: its ends join the frame or the lines they
/// meet, and it crosses the lines it passes over.
void UiWindow_DrawDividerLine(struct UiWindow *win, u32 x1, u32 y1, u32 x2,
                              u32 y2) {
  struct UiRenderWork *work = UI_WORK;
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
    UiWindow_ClearTileAttributesInRect(win->x + x2, win->y + y1, 1, y2 - y1);
    cursor =
        (u16 *)((((win->y + y1) << 5) + win->x + x2) * 2 + (u32)work->tilemap);
    for (pos = y1; pos <= y2; pos++) {
      tile = *cursor;
      if (pos == y1) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_TOP_TEE:
          break;
        case UI_TILE_FRAME_TOP:
          tile = UI_TILE_FRAME_TOP_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_UP:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_HORIZONTAL:
          tile = UI_TILE_DIVIDER_TEE_DOWN;
          break;
        default:
          tile = UI_TILE_DIVIDER_VERTICAL;
          break;
        }
      } else if (pos == y2) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_BOTTOM_TEE:
          break;
        case UI_TILE_FRAME_BOTTOM:
          tile = UI_TILE_FRAME_BOTTOM_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_DOWN:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_HORIZONTAL:
          tile = UI_TILE_DIVIDER_TEE_UP;
          break;
        default:
          tile = UI_TILE_DIVIDER_VERTICAL;
          break;
        }
      } else {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
          break;
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_HORIZONTAL:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        default:
          tile = UI_TILE_DIVIDER_VERTICAL;
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
    UiWindow_ClearTileAttributesInRect(win->x + x1, win->y + y1, x2 - x1, 1);
    cursor =
        (u16 *)((((win->y + y1) << 5) + win->x + x1) * 2 + (u32)work->tilemap);
    for (pos = x1; pos <= x2; pos++) {
      tile = *cursor;
      if (pos == x1) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_LEFT_TEE:
          break;
        case UI_TILE_FRAME_LEFT:
          tile = UI_TILE_FRAME_LEFT_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_LEFT:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_VERTICAL:
          tile = UI_TILE_DIVIDER_TEE_RIGHT;
          break;
        default:
          tile = UI_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      } else if (pos == x2) {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_CROSS:
        case UI_TILE_FRAME_RIGHT_TEE:
          break;
        case UI_TILE_FRAME_RIGHT:
          tile = UI_TILE_FRAME_RIGHT_TEE;
          break;
        case UI_TILE_DIVIDER_TEE_RIGHT:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        case UI_TILE_DIVIDER_VERTICAL:
          tile = UI_TILE_DIVIDER_TEE_LEFT;
          break;
        default:
          tile = UI_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      } else {
        switch (tile) {
        case UI_TILE_DIVIDER_TEE_DOWN:
        case UI_TILE_DIVIDER_TEE_UP:
        case UI_TILE_DIVIDER_CROSS:
          break;
        case UI_TILE_DIVIDER_TEE_RIGHT:
        case UI_TILE_DIVIDER_TEE_LEFT:
        case UI_TILE_DIVIDER_VERTICAL:
          tile = UI_TILE_DIVIDER_CROSS;
          break;
        default:
          tile = UI_TILE_DIVIDER_HORIZONTAL;
          break;
        }
      }
      *cursor = tile;
      cursor++;
    }
  }
}

/// Sets the text colour (0 to 15) of drawn text.
void UiWork_SetParamNibble(s32 param) {
  struct UiRenderWork *work = UI_WORK;

  work->colour = param & 15;
}

/// Sets the text style (UiRenderWork.outline) of drawn text.
void UiWork_SetRenderWord(u16 value) {
  struct UiRenderWork *work = UI_WORK;

  work->outline = value;
}

/// Decodes a message and draws it into a text canvas window at pixel (x, y).
void UiText_DrawResource(s32 message, struct UiWindow *window, s32 x, s32 y) {
  struct UiRenderWork *canvas = UI_WORK;
  u16 *counter = &canvas->count;
  s32 zero = 0;

  *counter = zero;
  UiText_BuildRenderEntries(message, 1);
  canvas->entries[*counter] = zero;
  *counter = (*counter + 1) & RENDER_ENTRY_MASK;
  UiText_RenderWideStringAtOffset(canvas->entries, window, x, y);
}

/// Decodes a message and draws it as tiles in a window at pixel (offsetX,
/// offsetY) of its interior.
void UiText_DrawCharacterAtOffset(s32 message, const struct UiWindow *window,
                                  u32 offsetX, u32 offsetY) {
  s32 byteOffset;
  s32 vramAddress;
  u16 *text;
  s32 zero;
  u32 cell;
  struct UiRenderWork *canvas;
  u16 *counter;

  canvas = UI_WORK;
  counter = &canvas->count;
  zero = 0;
  *counter = zero;
  UiText_BuildRenderEntries(message, 1);

  canvas->entries[*counter] = zero;
  *counter = (*counter + 1) & RENDER_ENTRY_MASK;

  cell = ((window->y + (offsetY >> 3) + 1) << 5) +
         (window->x + (offsetX >> 3)) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    byteOffset = cell * 2;
    vramAddress = byteOffset + (s32)UI_TILEMAP_VRAM;
    text = canvas->entries;
    UiText_RenderStringTiles(text, (u16 *)((u8 *)canvas->tilemap + byteOffset),
                             (u16 *)vramAddress, 7 & offsetX);
  }
}

/// Copies a zero-terminated byte string into the text renderer's halfwords.
/// The destination needs one halfword per input byte, including the terminator.
static __inline__ void UiText_WidenByteString(const u8 *input, u16 *output) {
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
/// terminator.
void UiText_DrawString(const u8 *text, struct UiWindow *window, s32 x, s32 y) {
  s16 *buffer;
  const u8 *input;

  input = text;
  buffer = Runtime_BumpAllocateAlternatePool(UI_BYTE_STRING_BUFFER_SIZE);
  UiText_WidenByteString(input, (u16 *)buffer);
  UiText_RenderWideStringAtOffset((u16 *)buffer, window, x, y);
  MemFree(buffer);
}

/// Draws a byte string as tiles in window work at pixel (offsetX, offsetY) of
/// its interior.
/// The conversion scratch has the same 255-byte input limit as
/// UiText_DrawString.
void UiText_DrawStringAtOffset(const u8 *text, const struct UiWindow *work,
                               s32 offsetX, s32 offsetY) {
  s16 *buffer;
  struct UiRenderWork *canvas;
  u32 cell;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  buffer = Runtime_BumpAllocateAlternatePool(UI_BYTE_STRING_BUFFER_SIZE);
  canvas = UI_WORK;
  UiText_WidenByteString(text, (u16 *)buffer);

  cell = ((work->y + ((u32)offsetY >> 3) + 1) << 5) +
         (work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    vram = (s32)UI_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    UiText_RenderStringTiles((u16 *)buffer, (u16 *)src, (u16 *)dst, phase);
    MemFree(buffer);
  }
}

/// Draws a byte string into a window's tilemap at pixel (x, y) of its
/// interior, rounded down to tiles.
/// Bytes widen to character codes; pixel coordinates round down to tile
/// coordinates.
void UiText_DrawStringInWindow(const u8 *text, struct UiWindow *window, u32 x,
                               u32 y) {
  u16 *buffer = Runtime_BumpAllocateAlternatePool(UI_BYTE_STRING_BUFFER_SIZE);
  UiText_WidenByteString(text, buffer);
  x >>= 3;
  y >>= 3;
  UiText_RenderWideStringInWindow((s16 *)buffer, window, x, y);
  MemFree(buffer);
}

/// Draws a number (width as UiText_FormatNumber) into a text canvas window at
/// pixel (x, y).
void UiText_DrawNumber(s32 value, s32 width, struct UiWindow *window, s32 x,
                       s32 y) {
  u8 numberText[UI_NUMBER_BUFFER_SIZE];

  UiText_DrawString(UiText_FormatNumber(numberText, value, width), window, x,
                    y);
}

/// Draws a number (width as UiText_FormatNumber) as tiles in a window at pixel
/// (x, y) of its interior.
void UiText_DrawNumberAtOffset(s32 value, s32 width,
                               const struct UiWindow *window, s32 x, s32 y) {
  u8 numberText[UI_NUMBER_BUFFER_SIZE];

  UiText_DrawStringAtOffset(UiText_FormatNumber(numberText, value, width),
                            window, x, y);
}

/// Draws a number (width as UiText_FormatNumber) into a window's tilemap at
/// pixel (x, y) of its interior.
void UiText_DrawNumberInWindow(s32 value, s32 width, struct UiWindow *window,
                               s32 x, s32 y) {
  u8 numberText[UI_NUMBER_BUFFER_SIZE];

  UiText_DrawStringInWindow(UiText_FormatNumber(numberText, value, width),
                            window, x, y);
}

/// Draws a four-digit number after a two-glyph sign (window tiles 0x1d and
/// 0x1e, or 0x1f and 0x1e with variant set) as tiles in window work at pixel
/// (offsetX, offsetY) of its interior.
void UiText_DrawPrefixedNumberAtOffset(s32 value, const struct UiWindow *work,
                                       s32 offsetX, s32 offsetY, s32 variant) {
  u8 formatted[UI_NUMBER_BUFFER_SIZE];
  s16 output[8];
  u8 *text;
  s32 index;
  u32 cell;
  struct UiRenderWork *canvas;
  s32 vram;
  s32 src;
  s32 dst;
  s32 phase;

  canvas = UI_WORK;
  text = UiText_FormatNumber(formatted, value, 4);
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

  cell = ((work->y + ((u32)offsetY >> 3) + 1) << 5) +
         (work->x + ((u32)offsetX >> 3)) + 1;
  if (cell < 32 * 20) {
    // Inside the tilemap.
    vram = (s32)UI_TILEMAP_VRAM;
    cell *= 2;
    dst = vram + cell;
    src = (s32)canvas->tilemap + cell;
    phase = offsetX & 7;
    UiText_RenderStringTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
  }
}
