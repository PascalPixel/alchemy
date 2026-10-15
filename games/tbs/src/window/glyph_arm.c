#include "unit/window/glyph_arm.h"
#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"
#include "edition.h"
#include "graphics.h"

static __inline__ void GlyphDot(u32 stride, u32 x, u32 tile_y, u32 row_line, u8 *vram, u32 color)
{
	u16 *p;
	u32	 mask;

	p = (u16 *)((u32)vram + (((x >> 3) * (stride - 2) + tile_y) << 5)
				| ((row_line | ((x & 7) >> 2)) << 1));
	mask = 15 << ((x & 3) << 2);
	*p = (*p & ~mask) | (mask & color);
}

s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character, u32 x, u32 y, const u8 *font)
{
	const u16 *rows;
	u32		   width;
	u32		   pass;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	u32 result;
#endif
	u32 stride;
	u32 row;
	u32 rownext;
	u32 col;
#if defined(TBS_EDITION_EN)
	u32 index;
#endif
#if defined(TBS_EDITION_JA)
	/* FAKEMATCH: these records reserve the native frame's 64-byte unused area. Without them 6 of 193 differ in tbs-ja (sub sp, #36 for #100, and the stacked argument's offset). */
	struct RenderInput frame[4];
#endif

#if defined(TBS_EDITION_EN)
	index = character - 32;
#endif
	stride = window->height;
#if defined(TBS_EDITION_EN)
	if (index > 111)
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
			const u8 *source;
			u32		  color;
			u32		  bits;

#if defined(TBS_EDITION_JA)
			u32 font_width;

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
			while (y + row <= 31)
			{
				u32 tile_y;
				u32 row_line;
				u8 *vram;

				col = 0;
				/* FAKEMATCH: needed to match: the row increment is written in both arms, which puts ADD between CMP and BEQ; written once ahead of the test it is scheduled before CMP (JA differ 2). */
				if (bits == 0)
					rownext = row + 1;
				else
				{
					rownext = row + 1;
					tile_y = (y + row) >> 3;
					vram = (u8 *)0x06002500;
					row_line = ((y + row) & 7) << 1;
					do
					{
						if (bits & 1)
							GlyphDot(stride, x + col, tile_y, row_line, vram, color);
						col++;
						bits >>= 1;
					} while (bits != 0);
				}
				row = rownext;
				if (row > 9)
					break;
				bits = *source++;
			}
#else
			u32 color1;
			u32 color3;
			u32 font_width;

			source = font + character * 32;
			color1 = 0x1111;
			color3 = 0x3333;
			row = 0;
#if defined(TBS_EDITION_EN)
			font_width = gUiText_GlyphWidths[index];
#else
			font_width = gUiText_GlyphWidths[character - 32];
#endif
			if (y <= 31)
			{
				u8 *vram;

				vram = (u8 *)0x06002500;
				while (1)
				{
					u32 tile_y;
					u32 row_line;

					col = 0;
					rownext = row + 1;
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
					row = rownext;
					if (rownext > 7 || y + rownext > 31)
						break;
				}
			}
#endif
			return font_width;
		}
		rows = (const u16 *)((const u8 *)gUiText_Glyphs + (character - 32) * 26);
		width = *rows++;
	}
	else
	{
#if defined(TBS_EDITION_JA)
		width = 10;
		rows = (const u16 *)(gUiText_KanjiGlyphs + (character - 256) * 24);
#else
		rows = (const u16 *)((const u8 *)gUiText_Glyphs + (character - 32) * 32);
		width = *rows;
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
		result = width;
#endif

	for (pass = 0; pass <= 3; pass++)
	{
		u32		   color;
		u32		   bits;
		u8		  *vram;
		const u16 *next;

		if (pass == 3)
			color = 0x1111;
		else
			color = 0x5555;
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
		row = 0;
		bits = rows[0];
		bits <<= 16;
		next = rows + 1;
		if (y <= 31)
		{
			vram = (u8 *)0x06002500;
			while (1)
			{
				u32 tile_y;
				u32 row_line;

				col = 0;
				rownext = row + 1;
				tile_y = (y + row) >> 3;
				row_line = ((y + row) & 7) << 1;
				while (1)
				{
					if ((s32)bits < 0)
						GlyphDot(stride, x + col, tile_y, row_line, vram, color);
					bits <<= 1;
					if (bits == 0)
						break;
					col++;
					if (col > 13)
						break;
				}
				row = rownext;
				if (rownext > 11)
					break;
				bits = *next++;
				bits <<= 16;
				if (y + rownext > 31)
					break;
			}
		}
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	return result;
#else
	return width;
#endif
}

void Tile_BuildMetatiles(u16 *dst, u16 *src)
{
	u32 row;
	u32 col;
	s32 a;
	s32 b;
	s32 c;
	s32 d;
	s32 e;
	s32 f;
	s32 found;
	s32 v;

	for (row = 0; row < 20; row++)
	{
		d = 0;
		a = d;
		c = d;
		f = d;
		b = f;
		e = f;
		found = f;
		col = f;
		dst++;
		for (; col < 30; col++)
		{
			u16 cmd = *src++;

			if (cmd == 0)
				continue;
			if (found == 0)
			{
				switch (cmd)
				{
				case 0xf010:
					a = col * 8;
					b = a + 8;
					c = a + 1;
					d = a;
					e = d + 7;
					f = b;
					found++;
					break;
				case 0xf013:
					a = col * 8;
					b = a + 8;
					c = a;
					d = a + 1;
					e = b;
					f = a + 7;
					found++;
					break;
				case 0xf007:
				case 0xf016:
#if EDITION_INTERNATIONAL || GAME_TLA
				case 0xf01a:
#endif
					a = col * 8;
					b = a + 8;
					d = a;
					c = d;
					f = b;
					e = f;
					found++;
				}
			}
			else
			{
				switch (cmd)
				{
				case 0xf012:
					b = col * 8 + 8;
					e = col * 8 + 7;
					f = b;
					break;
				case 0xf015:
					b = col * 8 + 8;
					e = b;
					f = col * 8 + 7;
					break;
				case 0xf007:
				case 0xf017:
#if EDITION_INTERNATIONAL || GAME_TLA
				case 0xf01b:
#endif
					f = col * 8 + 8;
					b = f;
					e = f;
					break;
				}
			}
		}
		v = e + (c << 8);
		*dst++ = v;
		v = b + (a << 8);
		dst++;
		*dst++ = v;
		dst++;
		*dst++ = v;
		dst++;
		*dst++ = v;
		dst++;
		src += 2;
		*dst++ = v;
		dst++;
		*dst++ = v;
		dst++;
		*dst++ = v;
		v = f + (d << 8);
		dst++;
		*dst++ = v;
	}
}
