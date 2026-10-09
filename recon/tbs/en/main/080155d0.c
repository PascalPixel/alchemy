/*
 * UiText_DrawGlyphArm: all-edition ARM C draft, not linked or credited.
 * Re-derived from the current listing and our ROMs on 2026-10-09.
 * Stock agbcc_arm with K1's ARM options, measured with:
 *   alchemy diff recon/tbs/en/main/080155d0.c moji --game tbs
 *                --edition tbs-en --arm
 * Native / draft / common / residual:
 *   ja: 193 / 198 /  94 / 203
 *   en: 196 / 197 / 109 / 175
 *   de, es, fr, it: 187 / 186 / 110 / 153
 * Japanese reserves its native 100-byte local frame with a tagged unused
 * source type. International builds now reserve their native 44 bytes.
 * Differences remain in allocation, spills, multiply operands and scheduling.
 * No register pins, barriers, instruction asm or alternative compiler options.
 * Japanese decodes ten 1bpp rows and kanji rows; the other editions decode
 * eight packed rows. English alone rejects character indices outside 32-143.
 * Trials: English direct geometry 325; hoisted row geometry 277; shorter
 * local scopes and do-while loops 248; separate next-pass lifetime 229;
 * integer destination reuse 225. The ten-minute permute search compiled
 * 9,786 distinct candidates: weighted 9,110 -> 6,330, residual 248 -> 215.
 * Its explicit-exit loop forms, applied to the shared edition branches,
 * improve ja/en/other from 226/225/186 to 203/203/156.
 * Rejected trials: direct goto loops (en 270); volatile local stride
 * (en 268); volatile initial height read (ja/en/other 211/209/169);
 * one-pass helper block (en 241). None changes a compiler or its options.
 * packed = 0 added one instruction (en/other 206/157). Instead, the shift
 * is confined to odd columns, avoiding its initial uninitialized read with
 * the same residual counts (en/other 203/156).
 * Saved row predicate plus discarded label leaves residuals unchanged;
 * splitting the pending row pointer shrinks the international frame to 44
 * but worsens ja/en/other to 232/236/195. The label adds no sched2 boundary.
 * With explicit loop exits, a volatile international stride restores the
 * 44-byte frame and gives en/other 186/154 (ja 298, so left plain there).
 * Volatile saved window/rows/result place the four high spills at the native
 * offsets and give en/other 186/153; direct pass++ then worsens them to
 * 199/176 and Japanese to 214, so the next-pass lifetime is retained.
 * Writing the volatile result inside the English character branches restores
 * their separate stores and changes en 186 -> 175, ja/other unchanged.
 * Reintroducing a separate destination pointer internationally worsens
 * en/other to 277/271 with the volatile spill layout; scalar reuse remains.
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
#if defined(TBS_EDITION_JA)
	/* FAKEMATCH: retaining the destination pointer separately keeps the
	 * Japanese coordinate lifetimes; scalar address reuse increased the
	 * earlier Japanese residual from 228 to 254. */
	u16 *dst;
#endif

	tile = (stride - 2) * (x >> 3) + tile_y;
	line = row_line | ((x & 7) >> 2);
#if defined(TBS_EDITION_JA)
	dst = (u16 *)((u32)(vram + (tile << 5)) | (line << 1));
	shift = (x & 3) << 2;
	mask = 15 << shift;
	*dst = (*dst & ~mask) | (mask & color);
#else
	shift = (x & 3) << 2;
	tile = (u32)vram + (tile << 5);
	tile |= line << 1;
	mask = 15 << shift;
	*(u16 *)tile = (*(u16 *)tile & ~mask) | (mask & color);
#endif
}

s32 UiText_DrawGlyphArm(struct UiWindow *window, u32 character,
	u32 x, u32 y, const u8 *font)
{
#if defined(TBS_EDITION_JA)
	u32 stride;
#else
	/* FAKEMATCH: keeping the native window, rows and result spills restores
	 * their high frame slots (40, 36 and 32); the volatile stride below then
	 * occupies the native slot 28. The continental residual falls 154 -> 153
	 * and both international coordinate lifetimes use the native registers.
	 * Direct English result stores in the character branches retain their
	 * native branch shape, improving that edition's residual 186 -> 175. */
	struct UiWindow *volatile saved_window;
	const u16 *volatile saved_rows;
	volatile u32 saved_result;
	/* FAKEMATCH: the native width spill is reloaded for each dot. Keeping
	 * those loads with the explicit loop exits changes en/other residuals
	 * from 203/156 to 186/154, and restores the 44-byte local frame. */
	volatile u32 stride;
#endif
	const u16 *rows;
	u32 width;
	u32 result;
#if defined(TBS_EDITION_JA)
	struct UiWindow *saved_window;
	/* FAKEMATCH: two unused RenderOutput records reserve the measured
	 * 56-byte gap: without them the native 100-byte local frame becomes 44. */
	struct RenderOutput frame[2];

	saved_window = window;
#endif
#if !defined(TBS_EDITION_JA)
	saved_window = window;
#endif
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
#if !defined(TBS_EDITION_JA)
	saved_rows = rows;
#endif
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	if (character - 222 <= 1)
	{
		x -= 3;
		y -= 2;
#if defined(TBS_EDITION_JA)
		result = 0;
#else
		saved_result = 0;
#endif
	}
	else
#endif
#if defined(TBS_EDITION_JA)
		result = width;
#else
		saved_result = width;
#endif

	{
		register u32 pass;
		const u16 *rows1;

#if defined(TBS_EDITION_JA)
		rows1 = rows + 1;
#else
		rows1 = saved_rows + 1;
#endif
		pass = 0;
		while (1)
		{
			u32 color;
			const u16 *next;
			u32 bits;
			u32 row;
			u32 rownext;
			u32 col;
			u8 *vram;
			/* FAKEMATCH: a separate next-pass lifetime removes the repeated
			 * outline-test spill reloads: the earlier English residual fell
			 * from 234 to 229 before the address and loop-form rewrites. */
			u32 next_pass;

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
#if defined(TBS_EDITION_JA)
			bits = (u32)*rows << 16;
#else
			bits = (u32)*saved_rows << 16;
#endif
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
				bits = (u32)*next++ << 16;
				if (y + rownext > 31)
					break;
			}
pass_next:
			pass = next_pass;
			if (pass > 3)
				break;
		}
	}
#if defined(TBS_EDITION_JA)
	return result;
#else
	return saved_result;
#endif
}
