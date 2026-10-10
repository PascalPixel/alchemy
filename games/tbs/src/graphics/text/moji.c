#if defined(TBS_EDITION_JA)

#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"

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
	u32 width;
	u32 mark;
	u32 result;
	u32 stride;
	u32 row;
	u32 rownext;
	u32 col;
	/* FAKEMATCH: these records reserve the native frame's 64-byte unused area. */
	struct RenderInput frame[4];
	window = input_window;
	/* FAKEMATCH: the repeated read keeps the height copy after the character
	   comparison through combine. CSE removes the second read; jump2 removes
	   the empty branch. With the width setup below, JA differ 3 -> 2: the
	   redundant height spill disappears and the remaining registers match. */
	do
	{
		width = window->height;
	} while (0);
	if (character > 255)
		width = window->height;
	stride = (width << 16) >> 16;

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
	{
		if (font != NULL)
		{
			const u8 *source;
			u32 color;
			u32 raw_color;
			u32 font_width;
			u32 bits;

			/* FAKEMATCH: the height's zero high half keeps this constant and
			   its normalized copy separate until combine. The early accent
			   comparison places the color spill after CMP (JA differ 6 -> 4);
			   reload CSE removes the repeated assignment in the accent path. */
			raw_color = 0x1111 + (stride >> 16);
			{
				u32 raw_width;

				/* FAKEMATCH: stride is a zero-extended height, so the test is
				   false. The separate width copy and x boundary retain ADD
				   before its spill in sched2 (JA differ 2 -> 0). Testing
				   stride >> 16 instead leaves two instructions and differs 18. */
				raw_width = 8;
				do
				{
					if ((u16)stride != stride)
						raw_width += width & 0xffff0000U;
				} while (0);
				do
				{
					x++;
				} while (0);
				font_width = raw_width;
			}
			do
			{
				if (character - 222 <= 1)
					raw_color = 0x1111 + (stride >> 16);
			} while (0);
			color = (raw_color << 16) >> 16;
			if (character - 222 <= 1)
			{
				color = raw_color;
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
		}
		/* FAKEMATCH: fold this difference in combine, after GCSE has run;
		   otherwise GCSE hoists the accent index (JA differ 2 -> 32). */
		mark = 0U - (222U - character);
		rows = (const u16 *)((const u8 *)UiText_Glyphs + (character - 32) * 26);
		{
			const u16 *header = rows;

			rows++;
			/* FAKEMATCH: the one-pass header read preserves native load/store order and sharing. */
			do
			{
				width = *header;
			} while (0);
		}
	}
	goto glyph_selected;
glyph_wide:
	{
		width = 10;
		rows = (const u16 *)(UiText_KanjiGlyphs + (character - 256) * 24);
	}
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
	return result;
}

#endif
