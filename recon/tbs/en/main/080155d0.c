#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"

/* ARM draft: JA differs 6; EN differs 4; DE/ES/FR/IT differ 10. The tail matches.
   Remaining sched2 ordering, with registers and spill offsets already correct:
   JA: height spill after the character CMP; width spill after x++; color spill
       after the accent CMP.
   EN: y copy before SUB sp; x copy immediately after the font load.
   DE/ES/FR/IT: coordinate copies before SUB sp and window spill; height spill
       after the font-selection CMP; source spill after color1's ADD;
       font-width spill after the y CMP. */

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
#if !defined(TBS_EDITION_EN)
	window = input_window;
#endif
#if !defined(TBS_EDITION_JA) && !defined(TBS_EDITION_EN)
	/* FAKEMATCH: this boundary delays the character copy until after the font load. */
	do { } while (0);
	use_font = character <= 255 || font != NULL;
#endif
#if defined(TBS_EDITION_EN)
	index = character - 32;
	/* FAKEMATCH: save window after forming index and leave the height read
	   outside a one-pass block; sched2 then orders LDRH/CMP/STR as native
	   (EN differ 12 -> 8). */
	window = input_window;
	stride = input_window->height;
#else
	/* FAKEMATCH: the one-pass height read preserves the load's scheduling boundary. */
	do
	{
		stride = window->height;
	} while (0);
#endif
#if defined(TBS_EDITION_EN)
	if (index > 111)
		return 1;
#endif

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
			{
				const u8 *first = source;

				/* FAKEMATCH: advance before the early clipping test to retain
				   the native ADD/CMP/LDRB order (JA differ 10 -> 8). */
				row = 0;
				source++;
				do
				{
					if (y > 31)
						break;
				} while (0);
				bits = *first;
				if (y > 31)
					goto font_done;
			}
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
#if defined(TBS_EDITION_EN)
			u32 raw_width;
#endif
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
				/* FAKEMATCH: keep the byte load separate from its spilled copy
				   through combine. The early clipping test and byte normalization
				   place CMP before STR in sched2 (EN differ 6 -> 4); the repeated
				   assignment below is removed by reload CSE. */
				raw_width = glyph_widths[index];
				do
				{
					if (y > 31)
						raw_width = glyph_widths[index];
				} while (0);
				font_width = (raw_width << 24) >> 24;
			}
#else
			row = 0;
			font_width = UiText_GlyphWidths[character - 32];
#endif
			if (y > 31)
				goto font_done;
#if defined(TBS_EDITION_EN)
			font_width = raw_width;
#endif
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
		const u16	*next;
		const u16	*rows1;

		rows1 = rows + 1;
		pass = 0;
		{
			u32 color;
			u32 bits;
			u8 *vram;
			/* FAKEMATCH: a separate next-pass lifetime avoids five repeated pass reloads. */
			u32 next_pass;

			/* FAKEMATCH: explicit back edges and one-pass bodies retain the
			   native allocation while the tail increment stays outside
			   both loops. This lets flow2 form its post-increment after reload,
			   retaining the store after the shift (JA/EN 8 -> 6, others 12 -> 10). */
		pass_start:
			do
			{
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
			row_start:
				do
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
				} while (0);
			} while (0);
		row_next:
			do
			{
				row = rownext;
				if (rownext > 11)
					goto pass_next;
			} while (0);
			/* FAKEMATCH: the repeated load preserves the early clipping
			   comparison through CSE; both loads become one LDRH. The two
			   boundaries retain y's native allocation priority in every edition. */
			do
			{
				do
				{
					bits = *next;
					if (y + rownext > 31)
						bits = *next;
				} while (0);
			} while (0);
			do
			{
				bits <<= 16;
			} while (0);
			/* FAKEMATCH: the low half is zero. Combine reduces this to
			   one after flow1, so the pointer spill remains after the shift. */
			next += 1 + (bits & 0xffff);
			if (y + rownext > 31)
				goto pass_next;
			goto row_start;
		pass_next:
			do
			{
				pass = next_pass;
				if (pass > 3)
					goto pass_end;
			} while (0);
			goto pass_start;
		pass_end:;
		}
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	return result;
#else
	return width;
#endif
}
