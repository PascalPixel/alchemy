/*
 * UiText_DrawGlyphArm: English ARM C draft, not linked or credited.
 * Re-derived from the current listing and our tbs-en ROM on 2026-10-09.
 * Stock agbcc_arm with K1's ARM options, measured with:
 *   alchemy diff recon/tbs/en/main/080155d0.c moji --game tbs
 *                --edition tbs-en --arm
 * Native: 196 instructions; draft: 205; common: 62; residual: 277.
 * The native 44-byte local frame becomes 60 bytes here. The first divergence
 * is register allocation and the frame after the push; remaining differences
 * include spills, conditional loads, multiply operands and loop scheduling.
 * No register pins, barriers, instruction asm or alternative compiler options.
 * Only English behavior is reconstructed here; the Japanese and continental
 * edition branches remain in the raw listing.
 * Trials: initial direct row geometry 325 residual instructions; hoisted row
 * geometry and reordered outline tests reduce the residual to 277.
 */

#include "types.h"
#include "uiwindow.h"
#include "glyph.h"

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
	u32 glyph;
	const u8 *source;
	const u16 *rows;
	u32 result;
	u32 pass;
	u32 row;
	u32 col;
	u32 bits;
	u32 color;
	const u16 *next;
	u8 *vram;

	stride = window->height;
	glyph = character - 32;
	if (glyph > 111)
		return 1;

	if (character <= 255 || (font != NULL && character - 222 <= 1))
	{
		if (font != NULL)
		{
			u32 color1;
			u32 color3;
			u32 font_width;
			u32 packed;

			source = font + character * 32;
			color1 = 0x1111;
			color3 = 0x3333;
			font_width = UiText_GlyphWidths[glyph];
			vram = (u8 *)0x06002500;
			for (row = 0; row <= 7 && y + row <= 31; row++)
			{
				u32 tile_y;
				u32 row_line;

				tile_y = (y + row) >> 3;
				row_line = ((y + row) & 7) << 1;
				for (col = 0; col <= 7; col++)
				{
					if ((col & 1) == 0)
						packed = *source++;
					else
						packed >>= 4;
					if ((packed & 13) == 1)
					{
						color = color3;
						if ((packed & 15) == 1)
							color = color1;
						if (((x + col) >> 3) <= 29)
							GlyphDot(stride, x + col, tile_y, row_line, vram, color);
					}
				}
			}
			return font_width;
		}
		rows = (const u16 *)((const u8 *)UiText_Glyphs + glyph * 26);
		result = *rows++;
	}
	else
	{
		rows = (const u16 *)((const u8 *)UiText_Glyphs + glyph * 32);
		result = rows[0];
		rows += 4;
	}
	if (character - 222 <= 1)
	{
		x -= 3;
		y -= 2;
		result = 0;
	}

	for (pass = 0; pass <= 3; pass++)
	{
		color = 0x5555;
		if (pass == 3)
			color = 0x1111;
		if (window->flags & 64)
		{
			if (pass <= 1)
			{
				if (pass == 1)
					x += 2;
				else
					x--;
			}
			else if (pass == 2)
			{
				x--;
				y++;
			}
			else
				y--;
		}
		next = rows + 1;
		bits = (u32)rows[0] << 16;
		vram = (u8 *)0x06002500;
		for (row = 0; row <= 11 && y + row <= 31; row++)
		{
			u32 tile_y;
			u32 row_line;

			tile_y = (y + row) >> 3;
			row_line = ((y + row) & 7) << 1;
			col = 0;
			do
			{
				if ((s32)bits < 0)
					GlyphDot(stride, x + col, tile_y, row_line, vram, color);
				bits <<= 1;
				if (bits == 0)
					break;
				col++;
			} while (col <= 13);
			if (row < 11)
				bits = (u32)*next++ << 16;
		}
	}
	return result;
}
