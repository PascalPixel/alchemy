// Characters drawn into a glyph sprite's tiles, strings measured and drawn into windows, and
// numbers written as text. Part of window.c.

// The glyph's pixels, 16 to a line as its tiles are read: 16 lines for the Japanese font, 24 for
// the other languages' taller one.
#define GLYPH_BUFFER_WIDTH      16
#define GLYPH_PIXEL(x, y)       ((y) * GLYPH_BUFFER_WIDTH + (x))

// The second character of a pair, in bits 8 to 23 of its code.
#define GLYPH_PAIR_SECOND(code) (((code) << 8) >> 16)

// The glyph buffer's size: 16 lines of pixels for the Japanese font, 24 for the others.
#define GLYPH_PIXELS_SIZE       JAPANESE_OR((GLYPH_BUFFER_WIDTH * 16), (GLYPH_BUFFER_WIDTH * 24))

// Draws a character, and a second one when the code carries it in bits 8 to 15, into a 16x16-pixel
// cell of four 4bpp tiles at out: in the text colour over a shadow, or with an outline when
// WIN_STATE.outline is 1.
Sint32 mnTextDrawGlyphPair(Sint32 code, Uint32 *out)
{
    Uint8      pixels[GLYPH_PIXELS_SIZE];
    WIN_STATE *state;
#if (LANGUAGE == LANGUAGE_JAPANESE)
    const Uint16 *mark = NULL;
    const Uint16 *glyph;
    Sint32        markCode;
#else
    const Uint8 *glyph;
    Sint32       firstIndex;
    Sint32       secondIndex;
    Uint16       secondWidth;
#endif
    Sint32 width;
    Sint32 colour;
    Bool   shadowColour;
    Uint8 *pixel;
    Sint32 tileRow;
    Sint32 tileColumn;
    Sint32 line;
    Sint32 column;
    Uint32 tileLine;

    state = WINDOW_STATE;
#if (LANGUAGE == LANGUAGE_JAPANESE)
    // The voicing mark the code carries above TEXT_MARK_SHIFT, drawn over the glyph one pixel
    // wider.
    markCode = FontMarkGlyphCodes[(Uint32)code >> TEXT_MARK_SHIFT];
    if (markCode != 0)
    {
        markCode -= TEXT_SPACE;
        mark      = FontGlyphs[markCode].rows;
    }
    code &= (1 << TEXT_MARK_SHIFT) - 1;
    if ((Uint32)code < KANJI_FIRST_CODE)
    {
        Sint32 index = code - TEXT_SPACE;

        // The width, then the rows after it.
        glyph        = &FontGlyphs[index].width;
        width        = *glyph++;
    }
    else
    {
        Sint32 index = code - KANJI_FIRST_CODE;

        glyph        = (const Uint16 *)(FontKanjiGlyphs + index * (FONT_GLYPH_ROWS * 2));
        width        = KANJI_WIDTH;
    }
    syWordsClearEntry(pixels, GLYPH_PIXELS_SIZE);
#else
    secondIndex  = GLYPH_PAIR_SECOND(code);
    code        &= TEXT_LAST_FONT_CODE;
    firstIndex   = code - TEXT_SPACE;
    glyph        = (const Uint8 *)FontGlyphs + firstIndex * sizeof(GLYPH);
    width        = ((const GLYPH *)glyph)->width;
    syWordsClearEntry(pixels, GLYPH_PIXELS_SIZE);
    glyph = (const Uint8 *)((const GLYPH *)glyph)->rows;
#endif
    // While WIN_STATE.mindRead is set, the text is colour 8 with no shadow.
    if (state->mindRead != 0)
    {
        shadowColour = FALSE;
        colour       = 8;
    }
    else
    {
        shadowColour = TRUE;
        colour       = state->colour;
    }
#if (LANGUAGE == LANGUAGE_JAPANESE)
    if (state->outline == TEXT_STYLE_OUTLINED)
    {
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
        if (mark != NULL)
        {
            syDecompressBitRuns(mark, pixels + GLYPH_PIXEL(8, 1), shadowColour);
            syDecompressBitRuns(mark, pixels + GLYPH_PIXEL(9, 1), shadowColour);
        }
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
        if (mark != NULL)
        {
            pixel = pixels + 7;
            syDecompressBitRuns(mark, pixel, colour);
            syDecompressBitRuns(mark, pixel + 1, colour);
            width++;
        }
        width++;
    }
    else
    {
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
        if (mark != NULL)
            syDecompressBitRuns(mark, pixels + GLYPH_PIXEL(8, 1), shadowColour);
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
        if (mark != NULL)
        {
            pixel = pixels + 7;
            syDecompressBitRuns(mark, pixel, colour);
            width++;
        }
    }
#else
    if (state->outline == TEXT_STYLE_OUTLINED)
    {
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(2, 3), shadowColour);
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 2), colour);
        width++;
    }
    else
    {
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(1, 3), shadowColour);
        syDecompressBitRuns(glyph, pixels + GLYPH_PIXEL(0, 2), colour);
    }
    if ((Uint16)secondIndex != 0)
    {
        glyph        = (const Uint8 *)(&FontGlyphs[-TEXT_SPACE] + (Uint16)secondIndex);
        secondWidth  = *(const Uint16 *)glyph;
        glyph       += 2;
        if (state->outline == TEXT_STYLE_OUTLINED)
        {
            Uint8 *dest = pixels + width;
            syDecompressBitRuns(glyph, dest + GLYPH_PIXEL(1, 3), shadowColour);
            syDecompressBitRuns(glyph, dest + GLYPH_PIXEL(2, 3), shadowColour);
            syDecompressBitRuns(glyph, dest + GLYPH_PIXEL(0, 2), colour);
            syDecompressBitRuns(glyph, dest + GLYPH_PIXEL(1, 2), colour);
            secondWidth += 1;
        }
        else
        {
            Uint8 *dest = pixels + width;
            Uint8 *p;
            p = dest + GLYPH_PIXEL(1, 3);
            syDecompressBitRuns(glyph, p, shadowColour);
            p = dest + GLYPH_PIXEL(0, 2);
            syDecompressBitRuns(glyph, p, colour);
        }
        width += secondWidth;
    }
#endif

    // Pack the cell into its four tiles, left to right and top to bottom, each line read from its
    // eighth pixel back so the leftmost lands in the low nibble.
    pixel   = pixels + 7;
    tileRow = 0;
    do
    {
        tileColumn = 0;
        do
        {
            line = 0;
            do
            {
                tileLine = 0;
                for (column = 0; column < 8; column++)
                {
                    tileLine = (tileLine << 4) + *pixel--;
                }
                line++;
                // the next line's eighth pixel
                pixel  += GLYPH_BUFFER_WIDTH + 8;
                *out++  = tileLine;
            } while (line <= 7);
            tileColumn++;
            // up to the tile on the right
            pixel -= 8 * GLYPH_BUFFER_WIDTH - 8;
        } while (tileColumn <= 1);
        tileRow++;
        // down to the next row of tiles
        pixel += 8 * GLYPH_BUFFER_WIDTH - 2 * 8;
    } while (tileRow <= 1);
    return width;
}

// German, Spanish, French, and Italian count ten pixels for every character up to
// TEXT_LAST_FONT_CODE but the space, and nothing for those above it.
#if WIDE_STRING_FIXED_WIDTH
#    define WIDE_WIDTH_FIXED 1
#endif

// Glyph n in the font.
#define FONT_GLYPH(n) ((const GLYPH *)((n) * sizeof(GLYPH) + (const Uint8 *)FontGlyphs))

// Measures a string of 16-bit character codes in pixels: a space takes four, a character up to
// TEXT_LAST_FONT_CODE its glyph's width (TEXT_MARK_VOICED and TEXT_MARK_SEMIVOICED nothing) and a
// code above it KANJI_WIDTH; see WIDE_WIDTH_FIXED for the four editions that count otherwise.
Sint32 mnTextGetWideStringWidth(Uint16 *text)
{
    Sint32  width;
    Uint16 *cursor;
    Uint32  code;
#if !defined(WIDE_WIDTH_FIXED)
    Uint32 glyphIndex;
#endif

    code   = *text;
    width  = 0;
    cursor = text + 1;
    while (code != 0)
    {
        if (code == TEXT_SPACE)
        {
            width += 4;
        }
        else if (code <= TEXT_LAST_FONT_CODE)
        {
#if defined(WIDE_WIDTH_FIXED)
            width += 10;
#else
            glyphIndex = code - TEXT_MARK_VOICED;
            if (glyphIndex > 1U)
            {
                // The font starts at the space.
                glyphIndex += TEXT_MARK_VOICED - TEXT_SPACE;
                width      += FONT_GLYPH(glyphIndex)->width;
            }
        }
        else
        {
            width += KANJI_WIDTH;
#endif
        }
        code    = *cursor;
        cursor += 1;
    }
    return width;
}

// Draws a zero-terminated string of 16-bit character codes into a text canvas window at pixel (x,
// y), running its colour, outline, line spacing and new line codes; other control codes are skipped
// with their arguments.
void mnTextDrawWideStringAtOffset(Uint16 *text, WINDOW *window, Sint32 x, Sint32 y)
{
    WIN_STATE *state;
    Uint32     c;
    Uint32     next;
    Sint16     start;

    state = WINDOW_STATE;
    c     = 0;
    start = x;
    if (text == NULL)
    {
        text                              = state->textBuf;
        state->textBuf[state->textBufEnd] = c;
        state->textBufEnd                 = (state->textBufEnd + 1) & TEXT_BUFFER_MASK;
    }
    for (;;)
    {
        c = *text++;
#if (LANGUAGE != LANGUAGE_JAPANESE)
        // A code past the font draws as @.
        if (c > TEXT_LAST_FONT_CODE)
            c = '@';
#endif
        if (c == 0)
            break;
        // Colour, style, line spacing and new lines take effect; the other control codes are
        // skipped with their operands, as are codes 11, 12 and 14, whose meaning is open
        // (window_decode.c).
#if (LANGUAGE == LANGUAGE_JAPANESE)
        if (c < TEXT_COMMANDS)
        {
#else
        if (c < TEXT_COMMANDS || c == TEXT_EM_DASH)
        {
#endif
            switch (c)
            {
                case TEXT_COLOR:
                    state->colour = *text;
                    text++;
                    break;
                case TEXT_OUTLINE:
                    state->outline = *text;
                    text++;
                    break;
                case TEXT_LINE_SPACING:
                    state->lineSpacing = *text;
                    text++;
                    break;
                case TEXT_COLORS_RESET: mnTextResetStyle(); break;
                case TEXT_NEWLINE:
                    x  = start;
                    y += TEXT_LINE_HEIGHT;
                    break;
                case 14:
                case TEXT_WINDOW_ANIMATION:
                case TEXT_ACTOR_NAME_START:
                    text++;
                    // Skip the second operand too. Fall through.
                case 11:
                case 12:
                case TEXT_ACTOR_NAME:
#if (LANGUAGE != LANGUAGE_JAPANESE)
                case TEXT_ARTICLE_CLASS:
#endif
                    text++;
                    break;
            }
        }
        else
        {
#if (LANGUAGE == LANGUAGE_JAPANESE)
            // A kana followed by a voicing mark carries the mark in its code.
            next = *text;
            if (next == TEXT_MARK_VOICED)
            {
                c |= 1 << TEXT_MARK_SHIFT;
                text++;
            }
            else if (next == TEXT_MARK_SEMIVOICED)
            {
                c |= 2 << TEXT_MARK_SHIFT;
                text++;
            }
#else
            // Two characters narrow enough to share a glyph's 16-pixel cell are drawn as one
            // sprite.
            if ((window->flags & WINDOW_FLAG_TEXT_CANVAS) == 0)
            {
                next = *text;
#    if OUTLINED_GLYPH_PAIRS_FIT_CELL
                if (c > TEXT_SPACE && next > TEXT_SPACE)
                {
                    Sint16 pairWidth =
                        FontGlyphs[c - TEXT_SPACE].width + FontGlyphs[next - TEXT_SPACE].width;

                    // Outlined text joins only pairs one pixel narrower.
                    if (state->outline == TEXT_STYLE_OUTLINED)
                    {
                        if ((Uint16)pairWidth <= 14)
                        {
                            c |= next << 8;
                            text++;
                        }
                    }
                    else if ((Uint16)pairWidth <= 15)
                    {
                        c |= next << 8;
                        text++;
                    }
                }
#    else
                if (c > TEXT_SPACE && next > TEXT_SPACE
                    && (Uint16)(FontGlyphs[c - TEXT_SPACE].width
                                + FontGlyphs[next - TEXT_SPACE].width)
                           <= 15)
                {
                    c |= next << 8;
                    text++;
                }
#    endif
            }
#endif
            x += mnGlyphDraw(window, c, x, y, GLYPH_DRAW_CHARACTER);
        }
    }
}

// Unused: draws a string of 16-bit character codes straight into the UI tilemap, as tiles, inside a
// window at tile (offsetX, offsetY) of its interior, writing both the window state's copy and VRAM.
// A NULL buffer draws the decoded text in WIN_STATE.textBuf.
UNUSED void
mnTextDrawWideStringAsTiles(Uint16 *buf, const WINDOW *window, Sint32 offsetX, Sint32 offsetY)
{
    WIN_STATE *canvas = WINDOW_STATE;
    Uint32     cell;

    if (buf == NULL)
    {
        buf                                 = canvas->textBuf;
        canvas->textBuf[canvas->textBufEnd] = 0;
        canvas->textBufEnd                  = (canvas->textBufEnd + 1) & TEXT_BUFFER_MASK;
    }

    cell = ((window->y + offsetY + 1) << 5) + (window->x + offsetX) + 1;
    if (cell < BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT)
    { // inside the tilemap
        Sint32 src;
        Sint32 dest;

        cell *= 2;
        dest  = (Sint32)WINDOW_TILEMAP_VRAM + cell;
        src   = (Sint32)canvas->tilemap + cell;

        mnTextDrawStringAsTiles(buf, (Uint16 *)src, (Uint16 *)dest, offsetX & 7);
    }
}

// Draws a string of wide (16-bit) codes into the window, one glyph per cell, skipping the control
// codes and their arguments; TEXT_NEWLINE starts a new line. With no text, the decoded text in
// WIN_STATE.textBuf is drawn.
void mnTextDrawWideStringInWindow(Sint16 *text, WINDOW *window, Sint32 x, Sint32 y)
{
    WIN_STATE *state;
    Uint16     c;
    Sint16     start;

    state = WINDOW_STATE;
    start = x;
    if (text == NULL)
    {
        text                              = state->textBuf;
        state->textBuf[state->textBufEnd] = 0;
        state->textBufEnd                 = (state->textBufEnd + 1) & TEXT_BUFFER_MASK;
    }
    c = *text++;
    if (c != 0)
        do
        {
#if (LANGUAGE == LANGUAGE_JAPANESE)
            if (c < TEXT_COMMANDS)
            {
#else
            if (c < TEXT_COMMANDS || c == TEXT_EM_DASH)
            {
#endif
                switch (c)
                {
                    case TEXT_NEWLINE:
                        x = start;
                        y++;
                        break;
                    case 14:
                    case TEXT_WINDOW_ANIMATION:
                    case TEXT_ACTOR_NAME_START:
                        text++;
                        // Skip the second operand too. Fall through.
                    case TEXT_COLORS_RESET:
                    case TEXT_COLOR:
                    case TEXT_OUTLINE:
                    case TEXT_LINE_SPACING:
                    case 11:
                    case 12:
                    case TEXT_ACTOR_NAME:
#if (LANGUAGE != LANGUAGE_JAPANESE)
                    case TEXT_ARTICLE_CLASS:
#endif
                        text++;
                        break;
                }
            }
            else
            {
                mnWindowPutGlyph(window, c, x, y, GLYPH_DRAW_CHARACTER);
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH)                                \
    || (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
                x++;
#else
                if ((Uint16)(c - TEXT_MARK_VOICED) > 1)
                    x++;
#endif
            }
            c = *text++;
        } while (c != 0);
    state->dirty = 1;
}

// The digits mnStringFormatNumber writes, in buffer[1] to [12].
#define NUMBER_DIGITS 12

// Writes value as decimal text into buffer (NUMBER_DIGITS + 2 bytes) and returns where the text
// starts. With width 0 the number has no padding and a minus sign when negative; otherwise it is
// right-aligned in width characters (up to NUMBER_DIGITS) and unsigned.
Uint8 *mnStringFormatNumber(Uint8 *buf, Sint32 input, Sint32 width)
{
    Sint32 offset;
    Sint32 value;
    Sint32 negative;
    Sint32 spaceCharacter;
    Sint32 minusCharacter;
    Uint8 *leading;
    Uint8 *trim;

    value    = input;
    negative = 0;
    if (value < 0)
    {
        if (width == 0)
            negative = 1;
        value = 0U - value;
    }

    // The digits, then spaces for the leading zeros (the last digit stays) and the sign in front;
    // the byte after them ends the text.
    buf[0] = ' ';
    for (offset = NUMBER_DIGITS; offset != 0; offset--)
    {
        buf[offset] = syMathUnsignedMultiplyHighAdd(value, RECIPROCAL_OF(10), -10) + '0';
        value       = syMathUnsignedMultiplyHigh(value, RECIPROCAL_OF(10));
    }

    offset                 = 0;
    buf[NUMBER_DIGITS + 1] = offset;
    spaceCharacter         = ' ';
    offset                 = 1;
    minusCharacter         = '-';
    for (leading = buf; offset != NUMBER_DIGITS + 1; leading++, offset++)
    {
        if (leading[1] == '0')
        {
            if (offset != NUMBER_DIGITS)
                leading[1] = spaceCharacter;
        }
        else
        {
            if (negative != 0)
                leading[0] = minusCharacter;
            break;
        }
    }

    if (width == 0)
    {
        offset = 0;
        if (buf[0] == ' ')
        {
            trim = buf;
            do
            {
                offset++;
                if (offset == NUMBER_DIGITS)
                    break;
                trim++;
            } while (*trim == ' ');
        }
        return buf + offset;
    }

    if ((Uint32)width > NUMBER_DIGITS)
        width = NUMBER_DIGITS;
    return buf + NUMBER_DIGITS + 1 - width;
}

#undef syDecompressBitRuns
#undef GLYPH_PIXELS_SIZE
#undef GLYPH_PIXEL
#undef GLYPH_BUFFER_WIDTH
#undef GLYPH_PAIR_SECOND
#undef WIDE_WIDTH_FIXED
#undef FONT_GLYPH
