// Text drawn as tilemap tiles of its own (the menus' labels, names and numbers), the text tiles
// taken and freed, divider lines, and the functions that draw a message, string or number into a
// window. Part of window.c.

// A text tile's state (WIN_STATE.textTileState): taken by text, and shown on the tilemap at the
// last look.
#define TEXT_TILE_TAKEN             1
#define TEXT_TILE_SHOWN             2
// A text tile's place in WIN_STATE.textTileState: the TEXT_TILES tiles from TEXT_TILE_FIRST take
// the first places, those from TEXT_EXTRA_TILE_FIRST the next TEXT_TILES.
#define TEXT_TILE_STATE_INDEX(tile) (((tile) & 0xFF) ^ TEXT_TILE_FIRST)

// Tile text is drawn into a canvas of one byte a pixel, a tile tall, then packed into 4bpp tiles in
// place: row `row` of its tile `tile` is then a word of eight pixels.
#define TILE_TEXT_CANVAS_WIDTH      256
#define TILE_TEXT_CANVAS_SIZE       (TILE_TEXT_CANVAS_WIDTH * 8)
#define TILE_TEXT_PACKED_ROW(tile, row)                                                            \
    (((Uint32 *)canvas)[(tile) + (row) * TILE_TEXT_CANVAS_WIDTH / 4])

// Draws window tile pattern over the tile of tilemap entry *entry, with its colours mapped through
// remap (0 leaves a pixel as it was).
void mnWindowOverlayTilePattern(Uint16 *entry, Uint16 *mirror, Sint32 pattern, Uint8 *remap)
{
    WIN_STATE *state = WINDOW_STATE;
    Uint8      pixels[128];
    Uint8     *sheet = syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
    Uint32     slot  = *(Uint8 *)entry;
    Uint8     *src;
    Uint8     *dest;
    Uint32     n;
    Uint32     i;

    dest = pixels;
    src  = BG_CHAR_BLOCK(0) + TILE_SIZE_4BPP * slot;
    for (i = 0; i < TILE_SIZE_4BPP; i++)
    {
        n       = *src++;
        *dest++ = n & (PALETTE_COLORS - 1);
        *dest++ = n >> 4;
    }

    dest = pixels;
    src  = sheet + pattern * TILE_SIZE_4BPP;
    for (i = 0; i < TILE_SIZE_4BPP; i++)
    {
        Uint32 value = *src++;
        Uint32 color = remap[value & (PALETTE_COLORS - 1)];

        if (color != 0)
            *dest = color;
        dest++;
        color = remap[value >> 4];
        if (color != 0)
            *dest = color;
        dest++;
    }

    {
        Uint8 *p;

        dest = pixels;
        for (i = 0, p = dest; i < TILE_SIZE_4BPP; i++)
        {
            Uint32 value  = p[0] | p[1] << 4;

            p            += 2;
            *dest++       = value;
        }
    }

    if ((Sint8)slot >= 0)
    {
        n = 0;
        do
        {
            Uint32 value        = state->nextTextTile;
            Uint32 following    = (value + 1) % TEXT_TILES;

            slot                = (Uint8)value;
            state->nextTextTile = following;
            if (state->textTileState[slot] == 0)
                break;
            n++;
        } while (n < TEXT_TILES);
        state->textTileState[slot]  = TEXT_TILE_TAKEN;
        slot                       |= TEXT_TILE_FIRST;
        *entry                      = WINDOW_TILE(slot);
        *mirror                     = WINDOW_TILE(slot);
    }
    syDMAXCopy(pixels,
               BG_CHAR_BLOCK(0) + slot * TILE_SIZE_4BPP,
               DMA_COPY_WORDS(TILE_SIZE_4BPP / 4),
               REG_DMA3);
}

// A new line in tile text advances by a space's width: tile text is one line.
#define TILE_TEXT_NEWLINE_WIDTH JAPANESE_OR(8, FontGlyphWidths[0])

// In Japanese, the codes above TILE_TEXT_NARROW_LAST (the kana) are a whole tile wide; a kana's
// mark is drawn from a tile glyph of its own, TILE_TEXT_MARK_GLYPH for the voiced mark and the one
// after it for the semi-voiced, three pixels wide and three pixels back over its kana.
#define TILE_TEXT_NARROW_LAST   128
#define TILE_TEXT_MARK_GLYPH    254
#define TILE_TEXT_MARK_WIDTH    3

// A label's end tile advances three pixels: the HP label's in every font, and the PP label's own in
// the French, Spanish and Italian fonts.
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)                                \
    || (LANGUAGE == LANGUAGE_ITALIAN)
#    define TEXT_IS_NARROW_TILE(code)                                                              \
        ((code) == WINDOW_TILE_HP_LABEL_END || (code) == WINDOW_TILE_PP_LABEL_END)
#else
#    define TEXT_IS_NARROW_TILE(code) ((code) == WINDOW_TILE_HP_LABEL_END)
#endif

extern const Uint8 FontColorPens[];

// Draws a string of 16-bit character codes with the window font into tiles of their own, starting x
// pixels into the first tile, and points tiles (and the same entries in mirror) at them.
Sint32             mnTextDrawStringAsTiles(Uint16 *text, Uint16 *tiles, Uint16 *mirror, Sint32 x)
{
    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    UNUSED Uint32 reserve[8];
    Uint8         pens[16];
    WIN_STATE    *state;
    Uint8        *canvas;
    Uint8        *glyphs;
    Uint32        paletteBits;
    Uint32       *src;
    Uint8        *dest;
    Uint32        glyphCode;
    Uint32        code;
    Sint32        tileCount;
    Uint32        glyphWidth;
    Sint32        index;
    Sint32        column;
    Uint32        bits;

    state       = WINDOW_STATE;
    canvas      = syHeapAllocate(TILE_TEXT_CANVAS_SIZE);
    glyphs      = syResourceGetTableEntry(RESOURCE_WINDOW_TILES);
    paletteBits = BG_TILE_PALETTE(state->textPalette);
    syWordsClear(pens, 16);
    // pens maps a font pixel, 1 or 3 (the font's two shades), to a colour; 0 leaves the background.
    if (paletteBits == BG_TILE_PALETTE(WINDOW_PALETTE))
    {
        pens[1] = FontColorPens[state->colour & (PALETTE_COLORS - 1)];
        pens[3] = 3;
        syWordsFill(canvas, TILE_TEXT_CANVAS_SIZE, PIXELS_8BPP(WINDOW_BACKGROUND_COLOR));
    }
    else
    {
        pens[1] = (Uint8)state->colour & (PALETTE_COLORS - 1);
        pens[3] = 1;
        syWordsFill(canvas, TILE_TEXT_CANVAS_SIZE, PIXELS_8BPP(TEXT_BACKGROUND_COLOR));
    }
    // Expand glyph pixels and control-code colour changes into a byte canvas.
    if (text != 0)
    {
        for (;;)
        {
            glyphCode = *text++;
            code      = glyphCode;
            if (glyphCode == 0)
                break;
            if (glyphCode <= TEXT_END_ALT)
            {
                switch (glyphCode)
                {
                    case TEXT_COLOR:
                        state->colour = *text++;
                        pens[1]       = FontColorPens[state->colour & (PALETTE_COLORS - 1)];
                        break;
                    // TEXT_COLORS_RESET, TEXT_OUTLINE and TEXT_LINE_SPACING all reset the colour
                    // here, and the last two leave their operand to be read as the next code.
                    case TEXT_COLORS_RESET:
                    case TEXT_OUTLINE:
                    case TEXT_LINE_SPACING:
                        state->colour = TEXT_COLOR_WHITE;
                        pens[1]       = FontColorPens[state->colour];
                        break;
                    case TEXT_NEWLINE: x += TILE_TEXT_NEWLINE_WIDTH; break;
                    // The codes tile text does not act on are skipped with their operands, as are
                    // codes 11, 12 and 14, whose meaning is open (window_decode.c).
                    case 14:
                    case TEXT_WINDOW_ANIMATION:
                    case TEXT_ACTOR_NAME_START:
                        text++;
                        // Skip the second operand too. Fall through.
                    case 11:
                    case 12:
                    case TEXT_ACTOR_NAME:
#if LANGUAGE != LANGUAGE_JAPANESE
                    case TEXT_ARTICLE_CLASS:
#endif
                        text++;
                        break;
                }
            }
            else
            {
                // A tilemap code (such as WINDOW_TILE_HP_LABEL) draws window tile glyphCode & 0xff.
                glyphCode &= 0xFF;
#if LANGUAGE == LANGUAGE_JAPANESE
                if (glyphCode - TEXT_MARK_VOICED <= 1)
                {
                    x         -= TILE_TEXT_MARK_WIDTH;
                    glyphCode += TILE_TEXT_MARK_GLYPH - TEXT_MARK_VOICED;
                }
#endif
                // Each glyph is an 8x8 tile of the window tiles, a word a row.
                src  = (Uint32 *)(glyphs + glyphCode * TILE_SIZE_4BPP);
                dest = canvas + x;
                for (index = 0; index < 8; index++)
                {
                    bits = *src++;
                    for (column = 0; column < 4; column++)
                    {
                        Uint8 pen = pens[bits & (PALETTE_COLORS - 1)];

                        if (pen != 0)
                            *dest = pen;
                        dest++;
                        bits >>= 4;
                        pen    = pens[bits & (PALETTE_COLORS - 1)];
                        if (pen != 0)
                            *dest = pen;
                        dest++;
                        bits >>= 4;
                    }
                    dest += TILE_TEXT_CANVAS_WIDTH - 8;
                }
                // The sign tiles mnTextDrawPrefixedNumberAtOffset writes (the HP and PP labels'
                // tiles) advance 8 for a first tile and 3 for a second, in pixels; a character its
                // font width, and any other tile below 32 one.
                if (code == WINDOW_TILE_HP_LABEL || code == WINDOW_TILE_PP_LABEL)
                    x += 8;
                else if (TEXT_IS_NARROW_TILE(code))
                    x += 3;
                else
                {
#if LANGUAGE == LANGUAGE_JAPANESE
                    if (glyphCode > TILE_TEXT_NARROW_LAST)
                        glyphWidth =
                            glyphCode - TILE_TEXT_MARK_GLYPH <= 1 ? TILE_TEXT_MARK_WIDTH : 8;
                    else
#endif
                        if (glyphCode >= TEXT_SPACE)
                        glyphWidth = FontGlyphWidths[glyphCode - TEXT_SPACE];
                    else
                        glyphWidth = 1;
                    x += glyphWidth;
                }
            }
        }
    }

    // Pack each eight-pixel span back into the tile's four-bit colour fields.
    tileCount = (Uint32)(x + 7) >> 3;
    src       = (Uint32 *)canvas;
    dest      = canvas;
    for (index = 0; index < 8; index++)
    {
        for (column = 0; column < tileCount; column++)
        {
            glyphCode        = src[0];
            bits             = src[1];
            glyphCode       |= glyphCode << 4;
            bits            |= bits >> 4;
            glyphCode       &= 0x0FF00FF0;
            bits            &= 0x00FF00FF;
            glyphCode       |= glyphCode << 8;
            bits            |= bits >> 8;
            src             += 2;
            *(Uint32 *)dest  = (glyphCode << 4) >> 16 | bits << 16;
            dest            += 4;
        }
        dest = dest - tileCount * 4 + TILE_TEXT_CANVAS_WIDTH;
        src  = (Uint32 *)((Uint8 *)src - tileCount * 8 + TILE_TEXT_CANVAS_WIDTH);
    }

    // Reuse private text tiles, allocate any new ones, then upload each tile.
    for (index = 0; index < tileCount; index++)
    {
        Uint32  mask = TEXT_TILES - 1;
        Uint32  tile;
        Uint32 *vramTile;

        if (state->extraTextTiles != 0)
            mask = 2 * TEXT_TILES - 1;
        tile = *tiles & TILE_NUMBER_MASK;
        if ((tile < TEXT_TILE_FIRST || tile > TEXT_TILE_FIRST + TEXT_TILES - 1)
            && (state->extraTextTiles == 0 || tile < TEXT_EXTRA_TILE_FIRST
                || tile > TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1))
        {
            tile = state->nextTextTile & mask;
            for (column = 0; column <= mask && state->textTileState[tile] != 0; column++)
                tile = (tile + 1) & mask;
            state->nextTextTile        = (tile + 1) & mask;
            state->textTileState[tile] = TEXT_TILE_TAKEN;
            if (tile > (TEXT_TILES - 1))
                tile += TEXT_EXTRA_TILE_FIRST - TEXT_TILES;
            else
                tile |= TEXT_TILE_FIRST;
            *tiles  = paletteBits | tile;
            *mirror = paletteBits | tile;
        }
        vramTile    = (Uint32 *)(BG_CHAR_BLOCK(0) + tile * TILE_SIZE_4BPP);
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
    syHeapRelease(canvas);
    return tileCount;
}

// Frees the text tiles under a tilemap rectangle (x, y, width and height in tiles) for reuse: their
// taken and shown bits are cleared.
void mnWindowClearTextTilesInRect(Sint32 x, Sint32 y, Uint32 width, Uint32 height)
{
    Uint8  *base      = RamHeapSlots->windowState;
    Uint16 *cursor    = &((WIN_STATE *)base)->tilemap[y][x];
    Uint32  alternate = ((WIN_STATE *)base)->extraTextTiles;
    Uint32  row;

    for (row = 0; row < height; row++)
    {
        Uint32 column;

        for (column = 0; column < width; column++)
        {
            Uint32 tile = *cursor++ & TILE_NUMBER_MASK;

            if ((tile - TEXT_TILE_FIRST) <= TEXT_TILES - 1
                || (alternate != 0 && tile > (TEXT_EXTRA_TILE_FIRST - 1)
                    && tile <= TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1))
            {
                Uint32 index                               = TEXT_TILE_STATE_INDEX(tile);

                ((WIN_STATE *)base)->textTileState[index] &= ~(TEXT_TILE_TAKEN | TEXT_TILE_SHOWN);
            }
        }
        cursor += BG_SCREEN_WIDTH - width;
    }
}

// Marks the text tiles the tilemap shows, and frees those it no longer shows: a tile taken but not
// marked shown (its state exactly TEXT_TILE_TAKEN).
void mnWindowMarkVisibleTextTiles(void)
{
    Uint8  *base      = RamHeapSlots->windowState;
    Uint32  alternate = ((WIN_STATE *)base)->extraTextTiles;
    Uint16 *cursor    = ((WIN_STATE *)base)->tilemap[0];
    Sint32  width     = DISPLAY_TILE_WIDTH;
    Sint32  row;
    Sint32  x;

    // BUG: the cursor never skips the two hidden columns that end each row of BG_SCREEN_WIDTH
    // entries, so the scan covers the tilemap's first DISPLAY_TILE_WIDTH * DISPLAY_TILE_HEIGHT
    // entries in order, and the text tiles of the last row and a half are never marked shown: the
    // loop below frees them while they are still on the screen.
    for (row = DISPLAY_TILE_HEIGHT; row != 0; row--)
    {
        for (x = 0; x < width; x++)
        {
            Uint32 tile = *cursor++ & TILE_NUMBER_MASK;

            if ((tile - TEXT_TILE_FIRST) <= TEXT_TILES - 1
                || (alternate != 0 && tile > (TEXT_EXTRA_TILE_FIRST - 1)
                    && tile <= TEXT_EXTRA_TILE_FIRST + TEXT_TILES - 1))
            {
                Uint32 index                               = TEXT_TILE_STATE_INDEX(tile);

                ((WIN_STATE *)base)->textTileState[index] |= TEXT_TILE_SHOWN;
            }
        }
#if FIX_TEXT_TILE_SCAN_ROW_STRIDE
        cursor += BG_SCREEN_WIDTH - width;
#endif
    }

    for (row = 0; row < 2 * TEXT_TILES; row++, base++)
    {
        if (((WIN_STATE *)base)->textTileState[0] == TEXT_TILE_TAKEN)
            ((WIN_STATE *)base)->textTileState[0] = 0;
    }
}

// Frees the text tiles of the second range (the TEXT_TILES tiles from TEXT_EXTRA_TILE_FIRST), and
// with allowAlternateTiles set lets text take that range too; the first range's tiles keep their
// state.
void mnTextUseExtraTiles(Sint32 allowAlternateTiles)
{
    WIN_STATE *state = WINDOW_STATE;
    Sint32     i;
    Uint8     *p;

    if (allowAlternateTiles)
    {
        state->extraTextTiles = 1;
        for (i = TEXT_TILES; i < 2 * TEXT_TILES; i++)
            state->textTileState[i] = 0;
    }
    else
    {
        state->extraTextTiles = 0;
        allowAlternateTiles   = 0;
        p                     = state->textTileState + TEXT_TILES;
        for (i = 0; i < TEXT_TILES; i++)
            *p++ = allowAlternateTiles;
    }
}

// Unused: does nothing.
UNUSED void mnWindowReservedNoOperation(void)
{
}

// Draws a divider from (x1, y1) to (x2, y2), one of which must agree, joining it to the tick and
// divider tiles already on the tilemap.
void mnWindowDrawDividerLine(WINDOW *window, Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2)
{
    WIN_STATE *state = WINDOW_STATE;
    Uint16    *cursor;
    Uint32     pos;
    Uint32     tile;

    if (x1 == x2)
    {
        if (y1 == y2)
            return;
        if (y1 > y2)
        {
            pos = y1;
            y1  = y2;
            y2  = pos;
        }
        mnWindowClearTextTilesInRect(window->x + x2, window->y + y1, 1, y2 - y1);
        cursor = state->tilemap[window->y + y1] + window->x + x2;
        for (pos = y1; pos <= y2; pos++)
        {
            tile = *cursor;
            if (pos == y1)
            {
                switch (tile)
                {
                    case WINDOW_TILE_DIVIDER_TEE_DOWN:
                    case WINDOW_TILE_DIVIDER_TEE_RIGHT:
                    case WINDOW_TILE_DIVIDER_TEE_LEFT:
                    case WINDOW_TILE_DIVIDER_CROSS:
                    case WINDOW_TILE_BORDER_TOP_TEE    : break;
                    case WINDOW_TILE_BORDER_TOP        : tile = WINDOW_TILE_BORDER_TOP_TEE; break;
                    case WINDOW_TILE_DIVIDER_TEE_UP    : tile = WINDOW_TILE_DIVIDER_CROSS; break;
                    case WINDOW_TILE_DIVIDER_HORIZONTAL: tile = WINDOW_TILE_DIVIDER_TEE_DOWN; break;
                    default                            : tile = WINDOW_TILE_DIVIDER_VERTICAL; break;
                }
            }
            else if (pos == y2)
            {
                switch (tile)
                {
                    case WINDOW_TILE_DIVIDER_TEE_UP:
                    case WINDOW_TILE_DIVIDER_TEE_RIGHT:
                    case WINDOW_TILE_DIVIDER_TEE_LEFT:
                    case WINDOW_TILE_DIVIDER_CROSS:
                    case WINDOW_TILE_BORDER_BOTTOM_TEE : break;
                    case WINDOW_TILE_BORDER_BOTTOM     : tile = WINDOW_TILE_BORDER_BOTTOM_TEE; break;
                    case WINDOW_TILE_DIVIDER_TEE_DOWN  : tile = WINDOW_TILE_DIVIDER_CROSS; break;
                    case WINDOW_TILE_DIVIDER_HORIZONTAL: tile = WINDOW_TILE_DIVIDER_TEE_UP; break;
                    default                            : tile = WINDOW_TILE_DIVIDER_VERTICAL; break;
                }
            }
            else
            {
                switch (tile)
                {
                    case WINDOW_TILE_DIVIDER_TEE_RIGHT:
                    case WINDOW_TILE_DIVIDER_TEE_LEFT:
                    case WINDOW_TILE_DIVIDER_CROSS     : break;
                    case WINDOW_TILE_DIVIDER_TEE_DOWN  :
                    case WINDOW_TILE_DIVIDER_TEE_UP    :
                    case WINDOW_TILE_DIVIDER_HORIZONTAL: tile = WINDOW_TILE_DIVIDER_CROSS; break;
                    default                            : tile = WINDOW_TILE_DIVIDER_VERTICAL; break;
                }
            }
            *cursor  = tile;
            cursor  += BG_SCREEN_WIDTH;
        }
    }
    else if (y1 == y2)
    {
        if (x1 == x2)
            return;
        if (x1 > x2)
        {
            pos = x1;
            x1  = x2;
            x2  = pos;
        }
        mnWindowClearTextTilesInRect(window->x + x1, window->y + y1, x2 - x1, 1);
        cursor = state->tilemap[window->y + y1] + window->x + x1;
        for (pos = x1; pos <= x2; pos++)
        {
            tile = *cursor;
            if (pos == x1)
            {
                switch (tile)
                {
                    case WINDOW_TILE_DIVIDER_TEE_DOWN:
                    case WINDOW_TILE_DIVIDER_TEE_UP:
                    case WINDOW_TILE_DIVIDER_TEE_RIGHT:
                    case WINDOW_TILE_DIVIDER_CROSS:
                    case WINDOW_TILE_BORDER_LEFT_TEE  : break;
                    case WINDOW_TILE_BORDER_LEFT      : tile = WINDOW_TILE_BORDER_LEFT_TEE; break;
                    case WINDOW_TILE_DIVIDER_TEE_LEFT : tile = WINDOW_TILE_DIVIDER_CROSS; break;
                    case WINDOW_TILE_DIVIDER_VERTICAL : tile = WINDOW_TILE_DIVIDER_TEE_RIGHT; break;
                    default                           : tile = WINDOW_TILE_DIVIDER_HORIZONTAL; break;
                }
            }
            else if (pos == x2)
            {
                switch (tile)
                {
                    case WINDOW_TILE_DIVIDER_TEE_DOWN:
                    case WINDOW_TILE_DIVIDER_TEE_UP:
                    case WINDOW_TILE_DIVIDER_TEE_LEFT:
                    case WINDOW_TILE_DIVIDER_CROSS:
                    case WINDOW_TILE_BORDER_RIGHT_TEE : break;
                    case WINDOW_TILE_BORDER_RIGHT     : tile = WINDOW_TILE_BORDER_RIGHT_TEE; break;
                    case WINDOW_TILE_DIVIDER_TEE_RIGHT: tile = WINDOW_TILE_DIVIDER_CROSS; break;
                    case WINDOW_TILE_DIVIDER_VERTICAL : tile = WINDOW_TILE_DIVIDER_TEE_LEFT; break;
                    default                           : tile = WINDOW_TILE_DIVIDER_HORIZONTAL; break;
                }
            }
            else
            {
                switch (tile)
                {
                    case WINDOW_TILE_DIVIDER_TEE_DOWN:
                    case WINDOW_TILE_DIVIDER_TEE_UP:
                    case WINDOW_TILE_DIVIDER_CROSS    : break;
                    case WINDOW_TILE_DIVIDER_TEE_RIGHT:
                    case WINDOW_TILE_DIVIDER_TEE_LEFT :
                    case WINDOW_TILE_DIVIDER_VERTICAL : tile = WINDOW_TILE_DIVIDER_CROSS; break;
                    default                           : tile = WINDOW_TILE_DIVIDER_HORIZONTAL; break;
                }
            }
            *cursor = tile;
            cursor++;
        }
    }
}

// Sets the text colour (TEXT_COL) of drawn text. The menus colour the HP figures with it, and set
// TEXT_COLOR_WHITE back after.
void mnTextSetColor(Sint32 color)
{
    WIN_STATE *state = WINDOW_STATE;

    state->colour    = color & (PALETTE_COLORS - 1);
}

// Sets the text style (TXT_STYLE) of drawn text: outlined, or a motion of the glyph sprites.
// Reached only through a bare entry of the window stub table.
void mnTextSetStyle(Sint32 style)
{
    WIN_STATE *state = WINDOW_STATE;

    state->outline   = style;
}

// Decodes a message and draws it into a text canvas window at pixel (x, y), all at once.
void mnTextDrawMessage(Sint32 msg, Sint32 window, Sint32 x, Sint32 y)
{
    WIN_STATE *canvas  = WINDOW_STATE;

    canvas->textBufEnd = 0;
    mnMessageDecode(msg, 1);
    canvas->textBuf[canvas->textBufEnd] = 0;
    canvas->textBufEnd                  = (canvas->textBufEnd + 1) & TEXT_BUFFER_MASK;
    mnTextDrawWideStringAtOffset(canvas->textBuf, (WINDOW *)window, x, y);
}

// Decodes a message and draws it as tiles in a window at pixel (offsetX, offsetY) of its interior.
// offsetY is rounded down to a tile row; offsetX picks the tile, and its remainder is where the
// text starts in it.
void mnTextDrawMessageAsTiles(Sint32 msg, WINDOW *window, Uint32 offsetX, Uint32 offsetY)
{
    Sint32     byteOffset;
    Sint32     vramAddr;
    Uint16    *text;
    Uint32     cell;
    WIN_STATE *canvas;

    canvas             = WINDOW_STATE;
    canvas->textBufEnd = 0;
    mnMessageDecode(msg, 1);

    canvas->textBuf[canvas->textBufEnd] = 0;
    canvas->textBufEnd                  = (canvas->textBufEnd + 1) & TEXT_BUFFER_MASK;

    cell = ((window->y + (offsetY >> 3) + 1) << 5) + (window->x + (offsetX >> 3)) + 1;
    if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
    {
        byteOffset = cell * 2;
        vramAddr   = byteOffset + (Sint32)WINDOW_TILEMAP_VRAM;
        text       = canvas->textBuf;
        mnTextDrawStringAsTiles(text,
                                (Uint16 *)((Uint8 *)canvas->tilemap + byteOffset),
                                (Uint16 *)vramAddr,
                                7 & offsetX);
    }
}

// Draws a byte string into a text canvas window at pixel (x, y). The 256-halfword scratch requires
// at most 255 input bytes before the terminator.
void mnTextDrawString(Uint8 *text, Sint32 window, Sint32 x, Sint32 y)
{
    Uint16 *buf    = syHeapAllocateEWRAM(TEXT_WIDE_STRING_BUFFER_SIZE);
    Uint16 *output = buf;

    while (*text != 0)
    {
        *output = *text;
        text++;
        output++;
    }
    *output = 0;
    mnTextDrawWideStringAtOffset(buf, (WINDOW *)window, x, y);
    syHeapRelease(buf);
}

// Draws a byte string as tiles in window at pixel (offsetX, offsetY) of its interior.
void mnTextDrawStringAtOffset(Uint8 *text, WINDOW *window, Sint32 offsetX, Sint32 offsetY)
{
    Uint16    *buf;
    Uint16    *output;
    WIN_STATE *canvas;
    Uint32     cell;
    Sint32     vram;
    Sint32     src;
    Sint32     dest;
    Sint32     phase;

    buf    = syHeapAllocateEWRAM(TEXT_WIDE_STRING_BUFFER_SIZE);
    canvas = WINDOW_STATE;
    output = buf;
    while (*text != 0)
    {
        *output = *text;
        text++;
        output++;
    }
    *output = 0;

    cell =
        ((window->y + ((Uint32)offsetY >> 3) + 1) << 5) + (window->x + ((Uint32)offsetX >> 3)) + 1;
    // BUG: the buffer is released only when the text starts on screen; text placed below the screen
    // keeps its buffer taken. BUGFIX releases it either way.
    if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
    {
        vram   = (Sint32)WINDOW_TILEMAP_VRAM;
        cell  *= 2;
        dest   = vram + cell;
        src    = (Sint32)canvas->tilemap + cell;
        phase  = offsetX & 7;
        mnTextDrawStringAsTiles(buf, (Uint16 *)src, (Uint16 *)dest, phase);
#if !FIX_STRING_BUFFER_LEAK
        syHeapRelease(buf);
#endif
    }
#if FIX_STRING_BUFFER_LEAK
    syHeapRelease(buf);
#endif
}

Uint8 *mnStringFormatNumber(Uint8 *output, Sint32 value, Sint32 width);

// Draws a byte string into a window's tilemap at pixel (x, y) of its interior, rounded down to
// tiles.
void   mnTextDrawStringInWindow(Uint8 *text, Sint32 window, Uint32 x, Uint32 y)
{
    Uint16 *buf    = syHeapAllocateEWRAM(TEXT_WIDE_STRING_BUFFER_SIZE);
    Uint16 *output = buf;

    while (*text != 0)
    {
        *output = *text;
        text++;
        output++;
    }
    x       >>= 3;
    *output   = 0;
    y       >>= 3;
    mnTextDrawWideStringInWindow((Sint16 *)buf, (WINDOW *)window, x, y);
    syHeapRelease(buf);
}

// Draws a number (width as mnStringFormatNumber) into a text canvas window at pixel (x, y).
void mnTextDrawNumber(Sint32 value, Sint32 width, Sint32 window, Sint32 x, Sint32 y)
{
    Uint8 numberText[TEXT_NUMBER_BUFFER_SIZE];

    mnTextDrawString(mnStringFormatNumber(numberText, value, width), window, x, y);
}

// Draws a number (width as mnStringFormatNumber) as tiles in a window at pixel (x, y) of its
// interior.
void mnTextDrawNumberAtOffset(Sint32 value, Sint32 width, WINDOW *window, Sint32 x, Sint32 y)
{
    Uint8 numberText[TEXT_NUMBER_BUFFER_SIZE];

    mnTextDrawStringAtOffset(mnStringFormatNumber(numberText, value, width), window, x, y);
}

// Draws a number (width as mnStringFormatNumber) into a window's tilemap at pixel (x, y) of its
// interior.
void mnTextDrawNumberInWindow(Sint32 value, Sint32 width, Sint32 window, Sint32 x, Sint32 y)
{
    Uint8 numberText[TEXT_NUMBER_BUFFER_SIZE];

    mnTextDrawStringInWindow(mnStringFormatNumber(numberText, value, width), window, x, y);
}

#undef TILE_TEXT_NEWLINE_WIDTH
#undef TILE_TEXT_NARROW_LAST
#undef TILE_TEXT_MARK_GLYPH
#undef TILE_TEXT_MARK_WIDTH
#undef TEXT_IS_NARROW_TILE

// Draws a four-digit number after a two-tile label as tiles in window at pixel (offsetX, offsetY):
// the HP label for variant 0, the party status window's HP; the PP label for variant 1, its PP.
void mnTextDrawPrefixedNumberAtOffset(
    Sint32 value, WINDOW *window, Sint32 offsetX, Sint32 offsetY, Sint32 variant)
{
    Uint8      formatted[TEXT_NUMBER_BUFFER_SIZE];
    Uint16     output[8];
    Uint8     *text;
    Sint32     index;
    Uint32     cell;
    WIN_STATE *canvas;
    Sint32     vram;
    Sint32     src;
    Sint32     dest;
    Sint32     phase;

    canvas = WINDOW_STATE;
    text   = mnStringFormatNumber(formatted, value, 4);
    // Each label is two tiles; the PP label's second is the HP label's in English, German and
    // Japanese (WINDOW_TILE_PP_LABEL_END).
    if (variant == 0)
    {
        output[0] = WINDOW_TILE_HP_LABEL;
        output[1] = WINDOW_TILE_HP_LABEL_END;
    }
    else
    {
        output[0] = WINDOW_TILE_PP_LABEL;
        output[1] = WINDOW_TILE_PP_LABEL_END;
    }
    for (index = 0; index <= 4; index++)
    {
        output[index + 2] = text[index];
    }
    output[6] = 0;
    cell =
        ((window->y + ((Uint32)offsetY >> 3) + 1) << 5) + (window->x + ((Uint32)offsetX >> 3)) + 1;
    if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
    {
        vram   = (Sint32)WINDOW_TILEMAP_VRAM;
        cell  *= 2;
        dest   = vram + cell;
        src    = (Sint32)canvas->tilemap + cell;
        phase  = offsetX & 7;
        mnTextDrawStringAsTiles((Uint16 *)output, (Uint16 *)src, (Uint16 *)dest, phase);
    }
}

#undef TEXT_TILE_TAKEN
#undef TEXT_TILE_SHOWN
#undef TEXT_TILE_STATE_INDEX
#undef TILE_TEXT_CANVAS_WIDTH
#undef TILE_TEXT_CANVAS_SIZE
#undef TILE_TEXT_PACKED_ROW
