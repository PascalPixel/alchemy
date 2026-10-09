/*
 * UiText_DrawGlyphArm: edition ARM C draft, not linked or credited.
 * Re-derived from the current listing and our ROMs on 2026-10-09.
 * Stock agbcc_arm with K1's ARM options, measured with:
 *   alchemy diff recon/tbs/en/main/080155d0.c moji --game tbs
 *                --edition tbs-en --arm
 * This checkpoint: Japanese residual 226 (193 native / 197 draft);
 * English residual 248 (196 / 206); continental residual 211 (187 / 194).
 * Japanese's native 100-byte local frame is reproduced with an unused
 * real-type local reserving the measured 56-byte frame difference. Remaining
 * differences include spill slots, multiply operands and loop scheduling.
 * No register pins, barriers, instruction asm or alternative compiler options.
 * All six edition behaviors are reconstructed; all builds keep their listing.
 * Trials: Japanese initial 233; frame local 227; separate next_pass 226.
 * Volatile local width 316; width pointer 228; volatile width pointer 313;
 * helper do/while block 270. These unsuccessful forms were removed.
 * The newer worker checkpoint improves this source further; this commit
 * preserves the measured trial as required by S4 before combining that work.
 */

#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"

extern const u8 UiText_GlyphWidths[];

static __inline__ void GlyphDot(u32 stride, u32 x, u32 tile_y, u32 row_line,
	u8 *vram, u32 color)
{
	u32 tile;
	u32 line;
	u32 shift;
	u32 mask;
	u16 *dst;

	tile = (stride - 2) * (x >> 3) + tile_y;
	line = row_line | ((x & 7) >> 2);
	dst = (u16 *)((u32)(vram + (tile << 5)) | (line << 1));
	shift = (x & 3) << 2;
	mask = 15 << shift;
	*dst = (*dst & ~mask) | (mask & color);
}

s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character,
	u32 x, u32 y, const u8 *font)
{
	u32 stride;
	const u16 *rows;
	u32 width;
	u32 result;
	struct UiWindow *saved_window;
#if defined(TBS_EDITION_JA)
	/* FAKEMATCH: two unused RenderOutput records reserve the measured 56-byte
	 * gap: without them the native 100-byte local frame becomes 44 bytes. */
	struct RenderOutput frame[2];
#endif

	saved_window = window;
	stride = window->height;
#if defined(TBS_EDITION_EN)
	if (character - 32 > 111)
		return 1;
#endif

#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	if (character <= 255 || (font != NULL && character - 222 <= 1))
#else
	if (character <= 255 || font != NULL)
#endif
	{
		if (font != NULL)
		{
#if defined(TBS_EDITION_JA)
			const u8 *source;
			u32 font_width;
			u32 color;
			u32 row;
			u32 rownext;
			u32 col;
			u32 bits;

			color = 0x1111;
			font_width = 8;
			x++;
			if (character - 222 <= 1)
			{
				character += 32;
				x -= 2;
				y--;
				font_width = 2;
			}
			source = font + (character - 32) * 10;
			row = 0;
			bits = *source++;
			if (y <= 31)
			{
				do
				{
					u32 tile_y;
					u32 row_line;
					u8 *vram;

					col = 0;
					rownext = row + 1;
					if (bits != 0)
					{
						tile_y = (y + row) >> 3;
						row_line = ((y + row) & 7) << 1;
						vram = (u8 *)0x06002500;
						do
						{
							if (bits & 1)
								GlyphDot(stride, x + col, tile_y, row_line, vram, color);
							col++;
							bits >>= 1;
						} while (bits != 0);
					}
					row = rownext;
					if (rownext > 9)
						break;
					bits = *source++;
				} while (y + rownext <= 31);
			}
			return font_width;
#else
			const u8 *source;
			u32 color1;
			u32 color3;
			u32 font_width;
			u32 packed;
			u32 row;
			u32 rownext;
			u32 col;
			u32 color;
			u8 *vram;

			source = font + character * 32;
			color1 = 0x1111;
			color3 = 0x3333;
			font_width = UiText_GlyphWidths[character - 32];
			row = 0;
			if (y > 31)
				return font_width;
			vram = (u8 *)0x06002500;
			do
			{
				u32 tile_y;
				u32 row_line;

				col = 0;
				rownext = row + 1;
				tile_y = (y + row) >> 3;
				row_line = ((y + row) & 7) << 1;
				do
				{
					packed >>= 4;
					if ((col & 1) == 0)
						packed = *source++;
					if ((packed & 13) == 1)
					{
						color = color3;
						if ((packed & 15) == 1)
							color = color1;
						if (((x + col) >> 3) <= 29)
							GlyphDot(stride, x + col, tile_y, row_line, vram, color);
					}
					col++;
				} while (col <= 7);
				row = rownext;
				if (rownext > 7)
					break;
			} while (y + rownext <= 31);
			return font_width;
#endif
		}
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 26);
		width = *rows++;
	}
	else
	{
#if defined(TBS_EDITION_JA)
		width = 10;
		rows = (const u16 *)(UiText_KanjiGlyphs + (character - 256) * 24);
#else
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 32);
		width = rows[0];
		rows += 4;
#endif
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	if (character - 222 <= 1)
	{
		x -= 3;
		y -= 2;
		result = 0;
	}
	else
#endif
		result = width;

	{
		register u32 pass;
		const u16 *rows1;

		rows1 = rows + 1;
		pass = 0;
		do
		{
			u32 color;
			const u16 *next;
			u32 bits;
			u32 row;
			u32 rownext;
			u32 col;
			u8 *vram;
#if defined(TBS_EDITION_JA)
			u32 next_pass;
#endif

			if (pass == 3)
				color = 0x1111;
			else
				color = 0x5555;
			if (!(saved_window->flags & 64))
				goto draw;
			if (pass <= 1)
			{
				if (pass == 1)
					x += 2;
				else
					x--;
				goto draw;
			}
			if (pass == 2)
			{
				x--;
				y++;
				goto draw;
			}
			y--;
draw:
			next = rows1;
			row = 0;
			bits = (u32)*rows << 16;
#if defined(TBS_EDITION_JA)
			next_pass = pass + 1;
#else
			pass++;
#endif
			if (y > 31)
				goto pass_next;
			vram = (u8 *)0x06002500;
			do
			{
				u32 tile_y;
				u32 row_line;

				col = 0;
				rownext = row + 1;
				tile_y = (y + row) >> 3;
				row_line = ((y + row) & 7) << 1;
				do
				{
					if ((s32)bits < 0)
						GlyphDot(stride, x + col, tile_y, row_line, vram, color);
					bits <<= 1;
					if (bits == 0)
						goto row_next;
					col++;
				} while (col <= 13);
row_next:
				row = rownext;
				if (rownext > 11)
					break;
				bits = (u32)*next++ << 16;
			} while (y + rownext <= 31);
pass_next:
#if defined(TBS_EDITION_JA)
			pass = next_pass;
#else
			;
#endif
		} while (pass <= 3);
	}
	return result;
}
