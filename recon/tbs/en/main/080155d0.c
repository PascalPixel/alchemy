#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"

/* ARM draft: JA differs 2; EN differs 2; DE/ES/FR/IT differ 10. The tail matches.
   Registers and spill offsets match. Remaining differences:
   JA: STR r3,[sp,#12] precedes ADD r8,r8,#1; native reverses that pair.
       Sched2's loop boundary forces that order. Replacing the one-pass
       block with an if removes the boundary, but the store's anti-dependence
       on SUB r3,r1,#222 then produces the same swap. The width constant
       needs a separate lifetime from its spill while retaining r3.
   EN: MOV r7,r3 must precede SUB sp,sp,#44. Prologue threading inserts SUB
       first; sched2 gives both priority 5 through PUSH and keeps that order.
       A source form must change this dependency or the copy's RTL form.
   DE/ES/FR/IT:
       MOV r9,r2 and MOV r8,r3 belong before SUB sp,sp,#44. Prologue
       threading and sched2 give the copies the same PUSH priority as EN;
       y's incoming r3 also dies at the font-selection SUBS. Changing that
       lifetime must retain x=r9, y=r8 and font=r2.
       STR r0,[sp,#28] belongs after CMP r3,#0. Reload puts the height spill
       inside the one-pass read, whose end note makes sched2 keep it before
       CMP. A separate copy must survive combine outside that boundary.
       STR r2,[sp,#24] belongs after ADD r1,r1,#17. Reload similarly spills
       source before the color boundary; removing the boundary alone leaves
       sched2's store-to-MOV-r2 anti-dependence and changes palette ordering.
       STR r2,[sp,#12] belongs after CMP r8,#31. Sched2 delays the store
       before the block-ending branch and fills the delay with CMP. An early
       comparison and separate width copy can move it, but must preserve
       LDRB before CMP and y=r8; the repeated-read trials change both. */

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
	/* FAKEMATCH: the boundaries keep the parameter copies before the window
	   spill and the height load before the range comparison. The repeated
	   read leaves the height copy after CMP through combine; CSE removes
	   the read and jump2 the empty branch. With the palette setup below,
	   x stays beside the font load (EN differ 4 -> 2). */
	do { } while (0);
	window = input_window;
	index = character - 32;
	do
	{
		width = input_window->height;
	} while (0);
	if (index > 111)
		width = input_window->height;
	stride = width;
#elif defined(TBS_EDITION_JA)
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
			u32 raw_color;
			u32 font_width;
			u32 bits;

			/* FAKEMATCH: the height's zero high half keeps this constant and
			   its normalized copy separate until combine. The early accent
			   comparison places the color spill after CMP (JA differ 6 -> 4);
			   reload CSE removes the repeated assignment in the accent path. */
			raw_color = 0x1111 + (stride >> 16);
			font_width = 8;
			/* FAKEMATCH: both high halves are zero. Combine removes the width
			   addition but retains an unused height test through reload; flow2
			   then removes it. Its temporary reload preserves r3 for width 8
			   and r0 for width 2 (JA differ 8 -> 2 after delaying the height spill). */
			do
			{
				if (stride & 0xffff0000U)
					break;
				font_width += width & 0xffff0000U;
			} while (0);
			x++;
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
#if defined(TBS_EDITION_EN)
			/* FAKEMATCH: width and stride still hold the zero-extended height.
			   These zero terms retain width's lifetime through combine and an
			   unused stride test through reload. Flow2 removes the test; its
			   temporary reload leaves r1/r2 for the palette (EN differ 20 -> 2). */
			color1 = 0x1111 + (width & 0xffff0000U);
			if (!(stride >> 16))
				color1 += width & 0xffff0000U;
#else
			color1 = 0x1111;
#endif
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
#if defined(TBS_EDITION_JA)
		/* FAKEMATCH: fold this difference in combine, after GCSE has run;
		   otherwise GCSE hoists the accent index (JA differ 2 -> 32). */
		mark = 0U - (222U - character);
#elif defined(TBS_EDITION_EN)
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
