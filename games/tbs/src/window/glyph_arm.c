/* The text canvas renderer, and the window edges of the screen transition,
   compiled as ARM code and copied to RAM to run.

   A text window draws its text into a canvas of 4bpp tiles in VRAM
   (TEXT_CANVAS_VRAM, mapped by mnWindowMapTextCanvasTiles). The canvas tiles
   run down columns: each column of tiles is as tall as the window's interior,
   which is the window height less its two border rows. mnGlyphDraw and
   mnScanlineWindowFillSpans copy these functions into RAM for each call, as the
   tree walker is copied: RAM_CODE_LOAD takes IWRAM when the copy fits, whose
   32-bit bus fetches an ARM instruction in one read, where the cartridge's
   16-bit bus takes two. */

#include "window/glyph_arm.h"
#include "edition.h"
#include "types.h"
#include "graphics/glyph.h"
#include "graphics/graphics.h"
#include "system/definitions.h"
#include "window/text_font.h"
#include "window/window.h"

/* The pixel rows text may reach: four tiles, the battle message window's
   interior; a glyph is cut off below it. */
#define TEXT_CANVAS_PIXEL_HEIGHT	32
/* The fonts start at the space. */
#define FONT_FIRST_CODE				' '
/* In English, the last code the main font has a glyph for. */
#define ENGLISH_FONT_LAST_CODE		143
/* A main-font glyph's bytes, as GLYPH lays one out: its width, then its rows, a
   halfword each. */
#define FONT_GLYPH_BYTES			((1 + FONT_GLYPH_ROWS) * 2)
/* The rows of a main-font glyph mnGlyphDrawArm draws. It reads the codes up to
   TEXT_LAST_FONT_CODE MAIN_FONT_GLYPH_BYTES apart in every edition, the
   Japanese layout of a width and 12 rows. */
#define MAIN_FONT_GLYPH_ROWS		12
#define MAIN_FONT_GLYPH_BYTES		((1 + MAIN_FONT_GLYPH_ROWS) * 2)
/* The Japanese small font's rows: a byte each. */
#define SMALL_FONT_ROWS				10
/* A halfword of four 4-bit pixels, all colour `color`: what
   mnTextSetCanvasPixel masks one pixel out of. */
#define PIXELS_4BPP_HALFWORD(color) ((color) * 0x1111)

#include "window/text_set_canvas_pixel.c"

/* Draws one character into a window's text canvas with its top-left corner at
   pixel (x, y) of the window's interior, and returns how far the text advances.

   With font set, the character comes from that small font: in Japanese
   SMALL_FONT_ROWS rows of a bit a pixel, elsewhere the 8x8 characters of the
   window tiles. With no font, it comes from the main font (FontGlyphs, and in
   Japanese FontKanjiGlyphs from KANJI_FIRST_CODE), drawn in four passes: the
   first three draw an outline in colour 5, offset left, right and down when the
   window has WINDOW_FLAG_OUTLINED_TEXT, and the last draws the glyph itself in
   colour 1. In Japanese a kana mark sits over the character before it and
   advances 0; in English a code past ENGLISH_FONT_LAST_CODE draws nothing and
   advances 1. */
Sint32 mnGlyphDrawArm(
	WINDOW *win, Uint32 character, Uint32 x, Uint32 y, const Uint8 *font)
{
	const Uint16 *glyphRows;
	Uint32		  glyphWidth;
	Uint32		  outlinePass;
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
	Uint32 advance;
#endif
	Uint32 winHeight;
	Uint32 row;
	Uint32 nextRow;
	Uint32 column;
#if LANGUAGE == LANGUAGE_ENGLISH
	Uint32 glyphIdx;
#endif
#if !GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED
	/* LEFTOVER(frame): an unused local the tbs-ja ROM's frame reserves and
	 * never touches. */
	UNUSED GLYPH_RECT unusedStackFrm[4];
#endif

#if LANGUAGE == LANGUAGE_ENGLISH
	glyphIdx = character - FONT_FIRST_CODE;
#endif
	winHeight = win->height;
#if LANGUAGE == LANGUAGE_ENGLISH
	if(glyphIdx > ENGLISH_FONT_LAST_CODE - FONT_FIRST_CODE)
		return 1;
#endif

	/* character - TEXT_MARK_VOICED <= 1, unsigned, is either kana mark. */
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
	if(character <= TEXT_LAST_FONT_CODE ||
		(font != NULL && character - TEXT_MARK_VOICED <= 1))
#else
	if(character <= TEXT_LAST_FONT_CODE || font != NULL)
#endif
	{
		if(font != NULL) {
			const Uint8 *src;
			Uint32		 col;
#ifdef MODERN
			/* A current GCC cannot see that the first column loads it. */
			Uint32 bits = 0;
#else
			Uint32 bits;
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
			Uint32 fontWidth;

			/* The small font: SMALL_FONT_ROWS rows of a byte, bit 0 the
			   leftmost pixel, in colour 1. */
			col = PIXELS_4BPP_HALFWORD(1);
			fontWidth = 8;
			x++;
			if(character - TEXT_MARK_VOICED <= 1) {
				character += FONT_FIRST_CODE;
				x -= 2;
				y--;
				fontWidth = 2;
			}
			character -= FONT_FIRST_CODE;
			src = font + character * SMALL_FONT_ROWS;
			row = 0;
			bits = *src++;
			while(y + row < TEXT_CANVAS_PIXEL_HEIGHT) {
				Uint32 tileY;
				Uint32 rowLine;
				Uint8 *vram;

				column = 0;
				if(bits == 0)
					nextRow = row + 1;
				else {
					nextRow = row + 1;
					tileY = (y + row) >> 3;
					vram = TEXT_CANVAS_VRAM;
					rowLine = ((y + row) & 7) << 1;
					do {
						if(bits & 1)
							mnTextSetCanvasPixel(winHeight, x + column, tileY,
								rowLine, vram, col);
						column++;
						bits >>= 1;
					} while(bits != 0);
				}
				row = nextRow;
				if(row >= SMALL_FONT_ROWS)
					break;
				bits = *src++;
			}
#else
			Uint32 foregroundCol;
			Uint32 secondaryCol;
			Uint32 fontWidth;

			/* The 8x8 font is window tiles of 4bpp pixels: a pixel of value 1
			   is drawn in colour 1, one of 3 in colour 3, any other is left
			   (bits & 0xd is 1 only for 1 and 3). */
			src = font + character * TILE_SIZE_4BPP;
			foregroundCol = PIXELS_4BPP_HALFWORD(1);
			secondaryCol = PIXELS_4BPP_HALFWORD(3);
			row = 0;
#if LANGUAGE == LANGUAGE_ENGLISH
			fontWidth = FontGlyphWidths[glyphIdx];
#else
			fontWidth = FontGlyphWidths[character - FONT_FIRST_CODE];
#endif
			if(y < TEXT_CANVAS_PIXEL_HEIGHT) {
				Uint8 *vram;

				vram = TEXT_CANVAS_VRAM;
				while(1) {
					Uint32 tileY;
					Uint32 rowLine;

					column = 0;
					nextRow = row + 1;
					tileY = (y + row) >> 3;
					rowLine = ((y + row) & 7) << 1;
					while(1) {
						if((column & 1) != 0)
							bits >>= 4;
						else
							bits = *src++;
						if((bits & 0xd) == 1) {
							col = secondaryCol;
							if((bits & 0xf) == 1)
								col = foregroundCol;
							if(((x + column) >> 3) < DISPLAY_TILE_WIDTH)
								mnTextSetCanvasPixel(winHeight, x + column,
									tileY, rowLine, vram, col);
						}
						column++;
						if(column > 7)
							break;
					}
					row = nextRow;
					if(nextRow > 7 || y + nextRow >= TEXT_CANVAS_PIXEL_HEIGHT)
						break;
				}
			}
#endif
			return fontWidth;
		}
		/* Main-font entries begin with their advance width, then rows of 16
		   pixels, the leftmost in the top bit. */
		glyphRows = (const Uint16 *)((const Uint8 *)FontGlyphs +
			(character - FONT_FIRST_CODE) * MAIN_FONT_GLYPH_BYTES);
		glyphWidth = *glyphRows++;
	} else {
#if LANGUAGE == LANGUAGE_JAPANESE
		glyphWidth = KANJI_WIDTH;
		glyphRows = (const Uint16 *)(FontKanjiGlyphs +
			(character - KANJI_FIRST_CODE) * KANJI_GLYPH_SIZE);
#else
		/* Its glyph's last MAIN_FONT_GLYPH_ROWS rows, after the width and the
		   first three. */
		glyphRows = (const Uint16 *)((const Uint8 *)FontGlyphs +
			(character - FONT_FIRST_CODE) * FONT_GLYPH_BYTES);
		glyphWidth = *glyphRows;
		glyphRows += 4;
#endif
	}
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
	if(character - TEXT_MARK_VOICED <= 1) {
		x -= 3;
		y -= 2;
		advance = 0;
	} else
		advance = glyphWidth;
#endif

	/* Draw the three outline offsets first, then the foreground pixels. */
	for(outlinePass = 0; outlinePass <= 3; outlinePass++) {
		Uint32		  col;
		Uint32		  bits;
		Uint8		 *vram;
		const Uint16 *nextGlyphRow;

		if(outlinePass == 3)
			col = PIXELS_4BPP_HALFWORD(1);
		else
			col = PIXELS_4BPP_HALFWORD(5);
		if(win->flg & WINDOW_FLAG_OUTLINED_TEXT) {
			if(outlinePass <= 1) {
				if(outlinePass == 1)
					x += 2;
				else
					x--;
			} else if(outlinePass == 2) {
				x--;
				y++;
			} else
				y--;
		}
		row = 0;
		bits = glyphRows[0];
		bits <<= 16;
		nextGlyphRow = glyphRows + 1;
		if(y < TEXT_CANVAS_PIXEL_HEIGHT) {
			vram = TEXT_CANVAS_VRAM;
			while(1) {
				Uint32 tileY;
				Uint32 rowLine;

				column = 0;
				nextRow = row + 1;
				tileY = (y + row) >> 3;
				rowLine = ((y + row) & 7) << 1;
				while(1) {
					if((Sint32)bits < 0)
						mnTextSetCanvasPixel(
							winHeight, x + column, tileY, rowLine, vram, col);
					bits <<= 1;
					if(bits == 0)
						break;
					column++;
					if(column > 13)
						break;
				}
				row = nextRow;
				if(nextRow >= MAIN_FONT_GLYPH_ROWS)
					break;
				bits = *nextGlyphRow++;
				bits <<= 16;
				if(y + nextRow >= TEXT_CANVAS_PIXEL_HEIGHT)
					break;
			}
		}
	}
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
	return advance;
#else
	return glyphWidth;
#endif
}

/* Builds the screen transition's window 1 edges for each of the screen's 160
   scanlines from the window borders in the UI tilemap at `tilemap`
   (BG_SCREEN_WIDTH tiles a row, DISPLAY_TILE_WIDTH shown), so the windows stay
   in view while the transition covers the rest: each line takes a pair of
   halfwords in `scanlineBounds`, the second (left << 8) | right, and a corner's
   line is a pixel narrower. */
void mnScanlineWindowBuildEdges(Uint16 *scanlineBounds, Uint16 *tilemap)
{
	Uint32 tileRow;
	Uint32 tileColumn;
	Sint32 left;
	Sint32 right;
	Sint32 topLeft;
	Sint32 bottomLeft;
	Sint32 topRight;
	Sint32 bottomRight;
	Sint32 foundLeftEdge;
	Sint32 horizontalBounds;

	for(tileRow = 0; tileRow < DISPLAY_TILE_HEIGHT; tileRow++) {
		bottomLeft = 0;
		left = bottomLeft;
		topLeft = bottomLeft;
		bottomRight = bottomLeft;
		right = bottomRight;
		topRight = bottomRight;
		foundLeftEdge = bottomRight;
		tileColumn = bottomRight;
		scanlineBounds++;
		for(; tileColumn < DISPLAY_TILE_WIDTH; tileColumn++) {
			Uint16 tile = *tilemap++;

			if(tile == 0)
				continue;
			/* The first border tile establishes the left edge and a one-tile
			   span. A later right border tile extends that span. */
			if(foundLeftEdge == 0) {
				switch(tile) {
				case WINDOW_TILE_BORDER_TOP_LEFT:
					left = tileColumn * 8;
					right = left + 8;
					topLeft = left + 1;
					bottomLeft = left;
					topRight = bottomLeft + 7;
					bottomRight = right;
					foundLeftEdge++;
					break;
				case WINDOW_TILE_BORDER_BOTTOM_LEFT:
					left = tileColumn * 8;
					right = left + 8;
					topLeft = left;
					bottomLeft = left + 1;
					topRight = right;
					bottomRight = left + 7;
					foundLeftEdge++;
					break;
				/* Outside Japanese the left edge meeting a divider counts as a
				   side too. XXX (no evidence): why tile 7 counts as a side; it
				   is blank in every edition's window tiles and no code writes
				   it into the UI tilemap, so no screen shows it. */
				case WINDOW_TILE(7):
				case WINDOW_TILE_BORDER_LEFT:
#if TRANSITION_MASK_COUNTS_SIDE_TEES
				case WINDOW_TILE_BORDER_LEFT_TEE:
#endif
					left = tileColumn * 8;
					right = left + 8;
					bottomLeft = left;
					topLeft = bottomLeft;
					bottomRight = right;
					topRight = bottomRight;
					foundLeftEdge++;
				}
			} else {
				switch(tile) {
				case WINDOW_TILE_BORDER_TOP_RIGHT:
					right = tileColumn * 8 + 8;
					topRight = tileColumn * 8 + 7;
					bottomRight = right;
					break;
				case WINDOW_TILE_BORDER_BOTTOM_RIGHT:
					right = tileColumn * 8 + 8;
					topRight = right;
					bottomRight = tileColumn * 8 + 7;
					break;
				case WINDOW_TILE(7):
				case WINDOW_TILE_BORDER_RIGHT:
#if TRANSITION_MASK_COUNTS_SIDE_TEES
				case WINDOW_TILE_BORDER_RIGHT_TEE:
#endif
					bottomRight = tileColumn * 8 + 8;
					right = bottomRight;
					topRight = bottomRight;
					break;
				}
			}
		}
		/* The tile row's eight scanlines, the second halfword of each pair: its
		   top line, six middle lines and its bottom line. */
		horizontalBounds = topRight + (topLeft << 8);
		*scanlineBounds++ = horizontalBounds;
		horizontalBounds = right + (left << 8);
		scanlineBounds++;
		*scanlineBounds++ = horizontalBounds;
		scanlineBounds++;
		*scanlineBounds++ = horizontalBounds;
		scanlineBounds++;
		*scanlineBounds++ = horizontalBounds;
		scanlineBounds++;
		tilemap += BG_SCREEN_WIDTH - DISPLAY_TILE_WIDTH;
		*scanlineBounds++ = horizontalBounds;
		scanlineBounds++;
		*scanlineBounds++ = horizontalBounds;
		scanlineBounds++;
		*scanlineBounds++ = horizontalBounds;
		horizontalBounds = bottomRight + (bottomLeft << 8);
		scanlineBounds++;
		*scanlineBounds++ = horizontalBounds;
	}
}
