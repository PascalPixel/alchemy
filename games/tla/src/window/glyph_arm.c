/* The text canvas renderer, compiled as ARM code and copied to RAM to run,
   entered in Thumb through mnGlyphDrawCode (glyph_arm_entries.s).

   A text window draws its text into a canvas of 4bpp tiles in VRAM
   (TEXT_CANVAS_VRAM). The canvas tiles run down columns: each column of tiles
   is as tall as the window's interior, which is the window height less its two
   border rows. */

#include "window/glyph_arm.h"
#include "types.h"
#include "graphics/uiwindow.h"
#include "graphics/glyph.h"
#include "graphics/text_font.h"
#include "edition.h"
#include "system/definitions.h"
#include "window/window.h"

/* The pixel rows text may reach: four tiles; a glyph is cut off below it. */
#define TEXT_CANVAS_PIXEL_HEIGHT	32
/* The fonts start at the space. */
#define FONT_FIRST_CODE				' '
/* The rows of a main-font glyph mnGlyphDrawArm draws. It reads the codes up to
   TEXT_LAST_FONT_CODE MAIN_FONT_GLYPH_BYTES apart in every edition, the
   Japanese layout of a width and 12 rows. */
#define MAIN_FONT_GLYPH_ROWS		12
/* A main-font glyph's bytes, as GLYPH lays one out: its width, then its rows, a
   halfword each. */
#define MAIN_FONT_GLYPH_BYTES		((1 + MAIN_FONT_GLYPH_ROWS) * 2)
/* The Japanese small font's rows: a byte each. */
#define SMALL_FONT_ROWS				10
/* A halfword of four 4-bit pixels, all colour `color`: what
   mnTextSetCanvasPixel masks one pixel out of. */
#define PIXELS_4BPP_HALFWORD(color) ((color) * 0x1111)

/* Sets one pixel in the column-major text canvas. winHeight includes the two
   border rows; doubledPixelRow is twice the row within a tile. repeatedCol
   holds the colour index in all four nibbles. */
static __inline__ void mnTextSetCanvasPixel(Uint32 winHeight, Uint32 x,
	Uint32 tileY, Uint32 doubledPixelRow, Uint8 *canvas, Uint32 repeatedCol)
{
	Uint16 *pixelHalfword;
	Uint32	pixelMask;

	/* The tile is column x / 8's, row tileY down it; within the tile a pixel
	   row is two halfwords of four pixels, the leftmost in the low nibble. */
	pixelHalfword =
		(Uint16 *)(((Uint32)canvas +
					   (((x >> 3) * (winHeight - 2) + tileY) << 5)) |
			((doubledPixelRow | ((x & 7) >> 2)) << 1));
	pixelMask = 15 << ((x & 3) << 2);
	*pixelHalfword = (*pixelHalfword & ~pixelMask) | (pixelMask & repeatedCol);
}

/* Draws one character into a window's text canvas with its top-left corner at
   pixel (x, y) of the window's interior, and returns how far the text advances.

   With font set, the character comes from that small font: the 10-row, 1bpp
   kana and numeral font in Japanese, or an 8x8 two-colour font elsewhere, 32
   bytes a character. With no font, it comes from the main font (Font_Glyphs,
   and in Japanese Font_KanjiGlyphs from KANJI_FIRST_CODE), drawn in four
   passes: the first three draw an outline in colour 5 (offset left, right and
   down when the window has WINDOW_FLAG_OUTLINED_TEXT) and the last draws the
   glyph itself in colour 1. Kana marks (TEXT_MARK_VOICED and
   TEXT_MARK_SEMIVOICED) sit over the previous character and advance by 0. */
Sint32 mnGlyphDrawArm(
	WINDOW *win, Uint32 character, Uint32 x, Uint32 y, const Uint8 *font)
{
	const Uint16 *glyphRows;
	Uint32		  glyphWidth;
	Uint32		  outlinePass;
	Uint32		  advance;
	Uint32		  winHeight;
	Uint32		  row;
	Uint32		  nextRow;
	Uint32		  column;
#if !GLYPH_DRAW_UNUSED_STACK_ARRAY_REMOVED
	/* Unused: sixteen words nothing reads or writes. */
	UNUSED Uint32 unusedStackFrm[16];
#endif

	winHeight = win->height;
	/* character - TEXT_MARK_VOICED <= 1, unsigned, is either kana mark. */
	if(character <= TEXT_LAST_FONT_CODE ||
		(font != NULL && character - TEXT_MARK_VOICED <= 1)) {
		if(font != NULL) {
			const Uint8 *src;
			Uint32		 col;
			Uint32		 bits;
#if (LANGUAGE == LANGUAGE_JAPANESE)
			Uint32 fontWidth;

			/* The small font: SMALL_FONT_ROWS rows of a byte, bit 0 the
			   leftmost pixel, in colour 1. A character is drawn a pixel in and
			   advances 8; a kana mark goes 2 further left and a row up, and
			   advances 2. */
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
			fontWidth = FontGlyphWidths[character - FONT_FIRST_CODE];
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
		glyphRows = (const Uint16 *)((const Uint8 *)Font_Glyphs +
			(character - FONT_FIRST_CODE) * MAIN_FONT_GLYPH_BYTES);
		glyphWidth = *glyphRows++;
	} else {
#if (LANGUAGE == LANGUAGE_JAPANESE)
		glyphWidth = KANJI_WIDTH;
		glyphRows = (const Uint16 *)(Font_KanjiGlyphs +
			(character - KANJI_FIRST_CODE) * KANJI_GLYPH_SIZE);
#else
		/* Its glyph's last MAIN_FONT_GLYPH_ROWS rows, after the width and the
		   first three. */
		glyphRows = (const Uint16 *)((const Uint8 *)Font_Glyphs +
			(character - FONT_FIRST_CODE) * sizeof(GLYPH));
		glyphWidth = *glyphRows;
		glyphRows += 4;
#endif
	}
	if(character - TEXT_MARK_VOICED <= 1) {
		x -= 3;
		y -= 2;
		advance = 0;
	} else
		advance = glyphWidth;

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
		bits <<= GLYPH_ROW_BITS;
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
				bits <<= GLYPH_ROW_BITS;
				if(y + nextRow >= TEXT_CANVAS_PIXEL_HEIGHT)
					break;
			}
		}
	}
	return advance;
}
