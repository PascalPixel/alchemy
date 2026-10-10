/* ARM draft, stock agbcc_arm: JA differs 0, EN 2, DE/ES/FR/IT 10.
   Target: matching C with tagged fakematches; no inline asm, register pins
   or asm barriers. Japanese already links the matching C in moji.c.

   2026-10-10: the device-free draft reached 66/107/65 after ordinary
   source reshaping. With C fakematches allowed again, sharing the current
   Japanese drawing loop gives 0/74/52. A one-pass packed-font pixel call
   gives 0/14/18; scoped source and row setup gives 0/8/14. Normalizing the
   continental height copy gives 0/8/12. Keeping a separate English width
   through the clipping return gives 0/6/12. Loading the English width-table
   address before the row scope gives 0/4/12. Separating the x copy, height
   copy and clipping return gives 0/2/12: only the English stack adjustment
   and y copy remain transposed. Both international pixel loops match;
   continental setup order remains.

   Searches: seed 249 found the pixel-call scope in 24 seconds. Finite
   setup scopes (32 and 1,024 choices), source/width forms (30), argument
   copies (48), unpinned register hints (16), height forms (7), and zero
   terms (125) did not close the remaining order differences. A whole-
   function inline wrapper regressed EN/DE to 104/142. Seed 251 searched
   the continental 12-difference form without finding an improvement.
   Earlier clean trials: row helper 219/263/202, passing VRAM explicitly
   181/218/190; whole-tree seeds 217/218 and 231/232/233 were also tried.
   Results with uninitialized temporaries were rejected.

   Follow-up: prefix searches 257/259 compiled 21,960/21,181 candidates;
   neither moved the first divergent entry instruction. Separate x copies,
   early argument tests (81), width helpers (4), normalized width reads (8),
   selector copies (12), width-table lifetimes (9) and equivalent row-zero
   expressions (9) did not improve this result. The width-return search
   (18) found the English clipping-copy form retained below. Further entry
   ranges, argument copies, cancelling arithmetic, height views and types
   did not improve the entry order. Reordering font initializers (3,840
   choices) also stalled. RTL showed the row scope ordered the table-address
   load after the row clear; separating the address and retaining the checked
   index fixes their order. Const-qualifying the font pointer did not help.
   Follow-up entry trials (equal-value branches, wide copies, aggregates,
   masked copies, cancelling operations, font copies and C89 parameter
   declarations) did not move y ahead of the stack adjustment. Recombining
   x's halves does retain its earlier copy; a repeated height read restores
   the load/spill order. Seed 263 compiled 21,961 candidates and found a
   clipping-return scope using an uninitialized mark; that condition was
   rejected. Testing initialized conditions (16) found the height condition
   below, which retains all native palette registers and gives EN differ 2.
   Prefix seed 269 compiled 22,473 candidates without moving the last pair.
   Const font/y inputs and a variadic entry stayed at 2; an inline C wrapper
   regressed to 85. Continental clipping forms, width/index normalization,
   palette scopes and 5,040 local declaration orders did not beat 12.
   Further continental trials: argument halves, selector forms and 2,240
   selector/height-copy combinations stayed at 12 or worse. Palette records,
   shared width ownership, repeated source copies and zero pointer terms
   did not help. Setup search 277 compiled 27,569 candidates: removing the
   source scope and retaining an empty final input-window test traded one
   reordered pair for eleven register differences (differ 28), so it was
   rejected. Initialized terminal references (44) and palette-lifetime
   placements (56) did not repair those registers. The saved draft was
   0/2/12 at staging's 3010b41dcd checkpoint. Separating the unchanged
   character copy before the continental selector then gives 0/2/10:
   the height load follows the complete selector calculation. Further height
   copies (560), branch copies/reads (64/16), early breaks (10), deferred
   pointer copies (12) and conditional stride copies (12) did not beat 10.
   Further selector-dependent height copies (24), paired normalizers (256)
   and palette/source copy scopes (512) also stayed at 10 or worse. Applying
   the English clipping-copy form (28 choices) regressed to 32: it swapped
   y and the tile-row register and moved the comparison before the byte load. */

#include "types.h"
#include "uiwindow.h"
#include "glyph.h"
#include "text_font.h"

extern const u8 UiText_GlyphWidths[];

static __inline__ void GlyphDot(u32 stride, u32 x, u32 tile_y, u32 row_line, u8 *vram, u32 color)
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
	do
	{
	} while (0);
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	mask = 15 << shift;
	pixel = (*(u16 *)tile & ~mask) | (mask & color);
	*(u16 *)tile = pixel;
}

s32 UiText_DrawGlyphArm(struct UiWindow *input_window, u32 character, u32 x, u32 y, const u8 *font)
{
	struct UiWindow *window;
	const u16		*rows;
	u32				 width;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	u32 mark;
	u32 result;
#endif
	u32 stride;
	u32 row;
	u32 rownext;
	u32 col;
#if !defined(TBS_EDITION_JA) && !defined(TBS_EDITION_EN)
	u32 copied_character;
	u32 narrow;
#endif
#if defined(TBS_EDITION_JA)
	/* FAKEMATCH: these records reserve the native frame's 64-byte unused area. */
	struct RenderInput frame[4];
#endif
#if defined(TBS_EDITION_EN)
	u32 index;
	u32 low;
	u32 high;

	/* FAKEMATCH: reconstruct the unchanged x value after a one-pass scope.
	   Combine removes the shifts; the copy stays before the window spill. */
	do
	{
		low = x << 16 >> 16;
		high = x >> 16 << 16;
	} while (0);
	x = low | high;
	index = character - 32;
#endif
	window = input_window;
#if defined(TBS_EDITION_JA)
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
#elif defined(TBS_EDITION_EN)
	/* FAKEMATCH: as in JA, keep the height copy through the range test.
	   With the x scope and clipping-return scope this gives EN differ 4 -> 2. */
	do
	{
		width = window->height;
	} while (0);
	if (index > 111)
		width = window->height;
	stride = (width << 16) >> 16;
#else
	/* FAKEMATCH: the unchanged character copy and explicit selector keep
	   the height load after the font/character tests. Combine cancels the
	   unsigned increment and decrement (continental differ 12 -> 10). */
	do
	{
		copied_character = character + 1;
	} while (0);
	character = copied_character - 1;
	narrow = character <= 255 || font != NULL;
	/* FAKEMATCH: normalize a separate height copy after its one-pass read.
	   This retains the native argument lifetimes (continental differ 14 -> 12). */
	do
	{
		width = window->height;
	} while (0);
	stride = (width << 16) >> 16;
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
	if (narrow)
#endif
	{
		if (font != NULL)
		{
			const u8 *source;
			u32		  color;
#if defined(TBS_EDITION_JA)
			u32 raw_color;
#else
#if defined(TBS_EDITION_EN)
			u32 raw_font_width;
			const u8 *widths;
#endif
			u32 color1;
			u32 color3;
#endif
			u32 font_width;
			u32 bits;

#if defined(TBS_EDITION_JA)
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
#else
			/* FAKEMATCH: separate source and row setup from the palette's
			   CSE block, retaining native spills (EN 14 -> 8, others 18 -> 14). */
			do
			{
				source = font + character * 32;
			} while (0);
			color1 = 0x1111;
			color3 = 0x3333;
#if !defined(TBS_EDITION_EN)
			do
			{
				row = 0;
			} while (0);
#endif
#if defined(TBS_EDITION_EN)
			/* FAKEMATCH: finish the palette's CSE block, then load the table
			   address before the row scope. Retaining the checked index keeps
			   it live through the row clear (EN differ 6 -> 4). */
			do
			{
			} while (0);
			widths = UiText_GlyphWidths;
			do
			{
				row = 0;
			} while (0);
			/* FAKEMATCH: the separate normalized width and repeated read
			   retain the clipping comparison before its spill. Both reads
			   become one LDRB; the width is a byte on either return path
			   (EN differ 8 -> 6). */
			raw_font_width = widths[index];
			do
			{
				if (y > 31)
					raw_font_width = widths[index];
			} while (0);
			font_width = (raw_font_width << 24) >> 24;
			if (y > 31)
			{
				/* FAKEMATCH: both returns agree. Keeping the initialized
				   height live through this empty branch preserves the native
				   palette registers; jump2 removes the branch (EN 20 -> 2). */
				if (width)
				{
					font_width = raw_font_width;
					goto packed_font_done;
				}
				else
				{
					font_width = raw_font_width;
					goto packed_font_done;
				}
			}
#else
			font_width = UiText_GlyphWidths[character - 32];
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
								/* FAKEMATCH: keep the pixel calculation in its own
								   CSE block; native color/pixel registers and load
								   order return (EN 74 -> 14, others 52 -> 18). */
								do
								{
									GlyphDot(stride, x + col, tile_y, row_line, vram, color);
								} while (0);
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
#if defined(TBS_EDITION_EN)
		packed_font_done:
#endif
			return font_width;
		}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
		/* FAKEMATCH: fold this difference in combine, after GCSE has run;
		   otherwise GCSE hoists the accent index (JA differ 2 -> 32). */
		mark = 0U - (222U - character);
#endif
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
		{
			const u16 *header = rows;

			rows += 4;
			/* FAKEMATCH: as for the narrow header above, retain the
			   native header-load sharing and pointer spill order. */
			do
			{
				width = *header;
			} while (0);
		}
#endif
	}
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
glyph_selected:
	if (mark <= 1)
	{
		x -= 3;
		y -= 2;
		/* FAKEMATCH: the boundary keeps the zero result after both coordinate changes. */
		do
		{
		} while (0);
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
