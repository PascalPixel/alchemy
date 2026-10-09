#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"

extern const u8 UiText_GlyphWidths[];

static __inline__ void GlyphPackedDot(u32 stride, u32 x, u32 tile_y, u32 row_line,
	u8 *vram, u32 color)
{
	u32 tile;
	u32 line;
	u32 shift;
	u32 mask;
	u32 pixel;

	/* FAKEMATCH: scalar reuse and the one-pass block give tile r0 and shift r1. */
	pixel = x >> 3;
	line = stride - 2;
	tile = pixel * line + tile_y;
	do
	{
		line = row_line | ((x & 7) >> 2);
		shift = (x & 3) << 2;
	} while (0);
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	mask = 15 << shift;
	pixel = (*(u16 *)tile & ~mask) | (mask & color);
	*(u16 *)tile = pixel;
}

static __inline__ void GlyphDot(u32 stride, u32 x, u32 tile_y, u32 row_line,
	u8 *vram, u32 color)
{
	u32 tile;
	u32 line;
	u32 shift;
	u32 mask;
	u32 pixel;

	/* FAKEMATCH: scalar reuse and the empty loop preserve the native dot sequence. */
	pixel = x >> 3;
	line = stride - 2;
	tile = pixel * line + tile_y;
	line = row_line | ((x & 7) >> 2);
	shift = (x & 3) << 2;
	do { } while (0);
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	mask = 15 << shift;
	pixel = (*(u16 *)tile & ~mask) | (mask & color);
	*(u16 *)tile = pixel;
}

s32 UiText_DrawGlyphArm(struct UiWindow *input_window, u32 character,
	u32 x, u32 y, const u8 *font)
{
	struct UiWindow *window;
	const u16 *rows;
#if !defined(TBS_EDITION_JA) && !defined(TBS_EDITION_EN)
	const u16 *header;
#endif
	u32 width;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	u32 mark;
	u32 result;
#endif
	u32 stride;
#if defined(TBS_EDITION_EN)
	u32 index;
#elif !defined(TBS_EDITION_JA)
	u32 use_font;
#endif
	u32 row;
	u32 rownext;
	u32 col;
#if defined(TBS_EDITION_JA)
	/* FAKEMATCH: these records reserve the native frame's 64-byte unused area. */
	struct RenderInput frame[4];
#endif
	window = input_window;
#if !defined(TBS_EDITION_JA) && !defined(TBS_EDITION_EN)
	/* FAKEMATCH: this boundary delays the character copy until after the font load. */
	do { } while (0);
	use_font = character <= 255 || font != NULL;
#endif
#if defined(TBS_EDITION_EN)
	/* FAKEMATCH: the early index test allocates index to ip and mark to lr. */
	index = character - 32;
	if (index > 111)
		return 1;
#endif
	/* FAKEMATCH: the one-pass height read preserves the load's scheduling boundary. */
	do
	{
		stride = window->height;
	} while (0);

#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	/* FAKEMATCH: separate mark assignments preserve character and header-load lifetimes. */
	if (character > 255)
	{
		if (font == NULL)
		{
			mark = character - 222;
			goto glyph_wide;
		}
		mark = character - 222;
		if (mark > 1)
			goto glyph_wide;
	}
#else
	if (use_font)
#endif
	{
		if (font != NULL)
		{
#if defined(TBS_EDITION_JA)
			const u8 *source;
			u32 color;
			u32 font_width;
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
			character -= 32;
			source = font + character * 10;
			row = 0;
			bits = *source++;
			if (y > 31)
				goto font_done;
			{
				while (1)
				{
					u32 tile_y;
					u32 row_line;
					u8 *vram;

					col = 0;
					/* FAKEMATCH: duplicated row increments produce the native CMP/ADD/BEQ order. */
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
					if (rownext > 9)
						break;
					/* FAKEMATCH: the early comparison orders CMP before LDRB (differ 12 -> 10). */
					do
					{
						if (y + rownext > 31)
							break;
					} while (0);
					bits = *source++;
					if (y + rownext > 31)
						break;
				}
			}
font_done:
			return font_width;
#else
			const u8 *source;
			u32 color1;
			u32 color3;
			u32 font_width;
			u32 packed;
			u32 color;
			u8 *vram;

			source = font + character * 32;
			/* FAKEMATCH: the empty loops preserve the native color initialization order. */
			do { } while (0);
			color1 = 0x1111;
			color3 = 0x3333;
			do { } while (0);
#if defined(TBS_EDITION_EN)
			{
				const u8 *glyph_widths = UiText_GlyphWidths;

				/* FAKEMATCH: the boundary orders the table load, row initialization and byte load. */
				do { } while (0);
				row = 0;
				font_width = glyph_widths[index];
			}
#else
			row = 0;
			font_width = UiText_GlyphWidths[character - 32];
#endif
			if (y > 31)
				goto font_done;
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
						packed >>= 4;
					else
						packed = *source++;
					if ((packed & 13) == 1)
					{
						color = color3;
						if ((packed & 15) == 1)
							color = color1;
						if (((x + col) >> 3) <= 29)
							GlyphPackedDot(stride, x + col, tile_y, row_line, vram, color);
					}
					col++;
					if (col > 7)
						break;
				}
				row = rownext;
				if (rownext > 7)
					break;
				if (y + rownext > 31)
					break;
			}
font_done:
			return font_width;
#endif
		}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
		mark = character - 222;
#endif
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 26);
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
		{
			const u16 *header = rows;

			rows++;
			/* FAKEMATCH: the one-pass header read preserves native load/store order and sharing. */
			do
			{
				width = *header;
			} while (0);
		}
#else
		header = rows;
		rows++;
		/* FAKEMATCH: the delayed width read merges the duplicate width spill. */
		do
		{
			width = *header;
		} while (0);
#endif
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	goto glyph_selected;
glyph_wide:
#else
	else
#endif
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
glyph_selected:
	if (mark <= 1)
	{
		x -= 3;
		y -= 2;
		/* FAKEMATCH: the boundary keeps the zero result after both coordinate changes. */
		do { } while (0);
		result = 0;
	}
	else
		result = width;
#endif

	{
		register u32 pass;
		const u16 *next;
		const u16 *rows1;

		rows1 = rows + 1;
		pass = 0;
		while (1)
		{
			u32 color;
			u32 bits;
			u8 *vram;
			/* FAKEMATCH: a separate next-pass lifetime avoids five repeated pass reloads. */
			u32 next_pass;

			if (pass == 3)
				color = 0x1111;
			else
				color = 0x5555;
			if (!(window->flags & 64))
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
			row = 0;
			bits = *rows;
			bits <<= 16;
			next = rows1;
			next_pass = pass + 1;
			if (y > 31)
				goto pass_next;
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
						goto row_next;
					col++;
					if (col > 13)
						break;
				}
row_next:
				row = rownext;
				if (rownext > 11)
					break;
				bits = *next++;
				bits <<= 16;
				if (y + rownext > 31)
					break;
			}
pass_next:
			pass = next_pass;
			if (pass > 3)
				break;
		}
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	return result;
#else
	return width;
#endif
}
