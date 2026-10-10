/* ARM draft, stock agbcc_arm: JA differs 139, EN 170, DE/ES/FR/IT 85.
   Target: matching C with no compiler-steering devices or inline asm.
   The Japanese implementation currently linked from graphics/text/moji.c
   still has tagged devices; it does not satisfy this draft's clean target.

   2026-10-10: replaced the constrained draft with ordinary C shared by all
   six editions, including the Japanese one-bit custom-font path. Removed
   empty and one-pass blocks, redundant reads, forced copies, zero terms,
   delayed increments and the unused 64-byte Japanese stack reservation.
   The first clean form differed JA 188, EN 211, continental 159. Moving
   VRAM setup beside the pixel write gives the current 139/170/85 without
   dead calculations or artificial lifetimes.

   Staging's whole-tree permuter explored 31,149 English and 32,386 German
   candidates from the first clean form (three jobs, three minutes, search
   seeds 217/218, stack and branch differences enabled). Candidate scores
   that depended on new forced temporaries were not adopted. A separate
   32-choice placement search confirmed the useful pixel-write placement.
   Remaining differences include register allocation, loop order and the
   Japanese native frame's unexplained 64 bytes. */

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
	u32 pixel;

	tile = (x >> 3) * (stride - 2) + tile_y;
	line = row_line | ((x & 7) >> 2);
	shift = (x & 3) << 2;
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	mask = 15 << shift;
	pixel = (*(u16 *)tile & ~mask) | (mask & color);
	*(u16 *)tile = pixel;
}

s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character,
	u32 x, u32 y, const u8 *font)
{
	const u16 *rows;
	u32 width;
	u32 stride;
	u32 row;
	u32 col;
	u32 pass;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	u32 mark;
	u32 result;
#endif
#if defined(TBS_EDITION_EN)
	u32 index;

	index = character - 32;
#endif
	stride = window->height;
#if defined(TBS_EDITION_EN)
	if (index > 111)
		return 1;
#endif

#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	mark = character - 222;
	if (character > 255 && (font == NULL || mark > 1))
		goto glyph_wide;
#else
	if (character > 255 && font == NULL)
		goto glyph_wide;
#endif
	if (font != NULL)
	{
		const u8 *source;
		u32 font_width;
		u32 bits;
		u32 color;
		u32 tile_y;
		u32 row_line;
		u8 *vram;

#if defined(TBS_EDITION_JA)
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
		character -= 32;
		source = font + character * 10;
		row = 0;
		bits = *source++;
		if (y > 31)
			return font_width;
		while (1)
		{
			col = 0;
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
			row++;
			if (row > 9)
				break;
			bits = *source++;
			if (y + row > 31)
				break;
		}
#else
		{
			u32 color1;
			u32 color3;

			source = font + character * 32;
			color1 = 0x1111;
			color3 = 0x3333;
			row = 0;
			font_width = UiText_GlyphWidths[character - 32];
			if (y > 31)
				return font_width;
			vram = (u8 *)0x06002500;
			while (1)
			{
				col = 0;
				tile_y = (y + row) >> 3;
				row_line = ((y + row) & 7) << 1;
				while (1)
				{
					if ((col & 1) != 0)
						bits >>= 4;
					else
						bits = *source++;
					if ((bits & 13) == 1)
					{
						color = color3;
						if ((bits & 15) == 1)
							color = color1;
						if (((x + col) >> 3) <= 29)
							GlyphDot(stride, x + col, tile_y, row_line, vram, color);
					}
					col++;
					if (col > 7)
						break;
				}
				row++;
				if (row > 7 || y + row > 31)
					break;
			}
		}
#endif
		return font_width;
	}
	rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 26);
	width = *rows++;
	goto glyph_selected;
glyph_wide:
#if defined(TBS_EDITION_JA)
	width = 10;
	rows = (const u16 *)(UiText_KanjiGlyphs + (character - 256) * 24);
#else
	rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 32);
	width = rows[0];
	rows += 4;
#endif
glyph_selected:
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	if (mark <= 1)
	{
		x -= 3;
		y -= 2;
		result = 0;
	}
	else
		result = width;
#endif
	for (pass = 0; pass < 4; pass++)
	{
		const u16 *next;
		u32 color;
		u32 bits;
		u32 tile_y;
		u32 row_line;
		u8 *vram;

		color = pass == 3 ? 0x1111 : 0x5555;
		if (window->flags & 64)
		{
			if (pass == 0)
				x--;
			else if (pass == 1)
				x += 2;
			else if (pass == 2)
			{
				x--;
				y++;
			}
			else
				y--;
		}
		row = 0;
		bits = (u32)*rows << 16;
		next = rows + 1;
		if (y > 31)
			continue;
		while (1)
		{
			col = 0;
			tile_y = (y + row) >> 3;
			row_line = ((y + row) & 7) << 1;
			while (1)
			{
				if ((s32)bits < 0)
				{
					vram = (u8 *)0x06002500;
					GlyphDot(stride, x + col, tile_y, row_line, vram, color);
				}
				bits <<= 1;
				if (bits == 0)
					break;
				col++;
				if (col > 13)
					break;
			}
			row++;
			if (row > 11)
				break;
			bits = (u32)*next++ << 16;
			if (y + row > 31)
				break;
		}
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	return result;
#else
	return width;
#endif
}
