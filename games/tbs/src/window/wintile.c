#include "unit/window/window.h"
#include "unit/window/config.h"
#include "unit/window/wintile.h"
#include "unit/window/output.h"
#include "unit/window/ptystat.h"
#include "unit/window/save.h"
#include "unit/window/btlcmd.h"
#include "unit/window/debug.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evmes.h"
#include "unit/window/moji.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "edition.h"
#include "window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"
#include "btlwork.h"
#include "heap_state.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "btlpty.h"
#include "scene.h"
#include "text_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "btlsys.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "sound_ids.h"
#include "layout_guard.h"
#include "field_event.h"
#include "b5_context.h"
#include "btltype.h"
#include "uiflow.h"
#include "owner_state.h"
#include "workspace_options.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save_state.h"
#include "irq.h"
#include "character_menu.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "field_sprite.h"
#include "uiwindow.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "vram_tab.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "effect_step.h"
#include "resmenu.h"
#include "btlsmn.h"
#include "serial_runtime.h"
#include "battle_effect_work.h"
#include "shop.h"
#include "disptran.h"
#include "frame.h"

void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern, u8 *remap)
{
	struct UiRenderWork *work = UI_WORK;
	u8					 pixels[128];
	u8					*sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
	u32					 slot = *(u8 *)entry;
	u8					*src;
	u8					*dst;
	u32					 n;
	u32					 i;

	dst = pixels;
	src = (u8 *)(0x06000000 + 32 * slot);
	for (i = 0; i < 32; i++)
	{
		n = *src++;
		*dst++ = n & 15;
		*dst++ = n >> 4;
	}

	dst = pixels;
	src = sheet + pattern * 32;
	for (i = 0; i < 32; i++)
	{
		u32 value = *src++;
		u32 color = remap[value & 15];

		if (color != 0)
			*dst = color;
		dst++;
		color = remap[value >> 4];
		if (color != 0)
			*dst = color;
		dst++;
	}

	{
		u8 *p;

		dst = pixels;
		for (i = 0, p = dst; i < 32; i++)
		{
			u32 value = p[0] | p[1] << 4;

			p += 2;
			*dst++ = value;
		}
	}

	if ((s8)slot >= 0)
	{
		for (n = 0; n < 128; n++)
		{
			u32 value = work->tile_cursor;
			u32 following = (value + 1) % 128;

			slot = (u8)value;
			work->tile_cursor = following;
			if (work->tile_attributes[slot] == 0)
				break;
		}
		work->tile_attributes[slot] = 1;
		slot |= 0x80;
		*entry = slot | 0xf000;
		*mirror = slot | 0xf000;
	}
	Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, REG_DMA3);
}

s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x)
{
	/* FAKEMATCH: needed to match: an unread 32-byte reserve, the native frame's extra bytes (the frame is 36 bytes without it). */
	u32					 reserve[8];
	u8					 pens[16];
	struct UiRenderWork *work;
	u8					*canvas;
	u8					*glyphs;
	u32					 attr;
	u32					*src;
	u8					*dst;
	u32					 c;
	u32					 code;
	s32					 count;
	u32					 w;
	s32					 i;
	s32					 j;
	u32					 bits;

	work = UI_WORK;
	canvas = MemAlloc(0x800);
	glyphs = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
	attr = work->level << 12;
	Iwram_ClearWords(pens, 16);
	if (attr == 0xF000)
	{
		pens[1] = UiText_ColourPens[work->colour & 15];
		pens[3] = 3;
		Iwram_FillWords(canvas, 0x800, 0x04040404);
	}
	else
	{
		pens[1] = (u8)work->colour & 15;
		pens[3] = 1;
		Iwram_FillWords(canvas, 0x800, 0x0E0E0E0E);
	}
	if (script != 0)
	{
		for (;;)
		{
			c = *script++;
			code = c;
			if (c == 0)
				break;
			if (c <= 30)
			{
				switch (c)
				{
				case 8:
					work->colour = *script++;
					pens[1] = UiText_ColourPens[work->colour & 15];
					break;
				case 7:
				case 9:
				case 10:
					work->colour = 15;
					pens[1] = UiText_ColourPens[work->colour];
					break;
				case 3:
#if EDITION_INTERNATIONAL
					x += UiText_GlyphWidths[0];
#else
					x += 8;
#endif
					break;
				case 14:
				case 15:
				case 28:
					script++;
				case 11:
				case 12:
				case 17:
#if EDITION_INTERNATIONAL
				case 29:
#endif
					script++;
					break;
				}
			}
			else
			{
				c &= 0xFF;
#if !EDITION_INTERNATIONAL
				if (c == 222 || c == 223)
				{
					x -= 3;
					c += 32;
				}
#endif
				src = (u32 *)(glyphs + c * 32);
				dst = canvas + x;
				for (i = 0; i < 8; i++)
				{
					bits = *src++;
					for (j = 0; j < 4; j++)
					{
						u8 pen = pens[bits & 15];

						if (pen != 0)
							*dst = pen;
						dst++;
						bits >>= 4;
						pen = pens[bits & 15];
						if (pen != 0)
							*dst = pen;
						dst++;
						bits >>= 4;
					}
					dst += 248;
				}
				if (code == 0xF01D || code == 0xF01F)
					x += 8;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
				else if (code == 0xF01E || code == 0xF005)
#else
				else if (code == 0xF01E)
#endif
					x += 3;
				else
				{
#if !EDITION_INTERNATIONAL
					if (c > 128)
					{
						if (c == 254 || c == 255)
							w = 3;
						else
							w = 8;
					}
					else
#endif
						if (c > 31)
						w = UiText_GlyphWidths[c - 32];
					else
						w = 1;
					x += w;
				}
			}
		}
	}

	count = (u32)(x + 7) >> 3;
	src = (u32 *)canvas;
	dst = canvas;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < count; j++)
		{
			c = src[0];
			bits = src[1];
			c |= c << 4;
			bits |= bits >> 4;
			c &= 0x0FF00FF0;
			bits &= 0x00FF00FF;
			c |= c << 8;
			bits |= bits >> 8;
			src += 2;
			*(u32 *)dst = (c << 4) >> 16 | bits << 16;
			dst += 4;
		}
		dst = dst - count * 4 + 0x100;
		src = (u32 *)((u8 *)src - count * 8 + 0x100);
	}

	for (i = 0; i < count; i++)
	{
		u32	 mask = 127;
		u32	 tile;
		u32 *p;

		if (work->alt != 0)
			mask = 255;
		tile = *tiles & 0x3FF;
		if ((tile < 0x80 || tile > 0xFF) && (work->alt == 0 || tile < 0x200 || tile > 0x27F))
		{
			tile = work->tile_cursor & mask;
			for (j = 0; j <= mask && work->tile_attributes[tile] != 0; j++)
				tile = (tile + 1) & mask;
			work->tile_cursor = (tile + 1) & mask;
			work->tile_attributes[tile] = 1;
			if (tile > 127)
				tile += 0x180;
			else
				tile |= 0x80;
			*tiles = attr | tile;
			*mirror = attr | tile;
		}
		p = (u32 *)(0x06000000 + tile * 32);
		*p++ = ((u32 *)canvas)[i];
		*p++ = ((u32 *)canvas)[i + 0x40];
		*p++ = ((u32 *)canvas)[i + 0x80];
		*p++ = ((u32 *)canvas)[i + 0xC0];
		*p++ = ((u32 *)canvas)[i + 0x100];
		*p++ = ((u32 *)canvas)[i + 0x140];
		*p++ = ((u32 *)canvas)[i + 0x180];
		*p++ = ((u32 *)canvas)[i + 0x1C0];
		tiles++;
		mirror++;
	}
	MemFree(canvas);
	return count;
}

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height)
{
	u8	*base = gWindowWork[0];
	u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
	u32	 alternate = ((struct UiRenderWork *)base)->alt;
	u32	 row;

	for (row = 0; row < height; row++)
	{
		u32 column;

		for (column = 0; column < width; column++)
		{
			u32 tile = *cursor++ & 0x3FF;

			if ((tile - 0x80) <= 0x7F || (alternate != 0 && tile > 0x1FF && tile <= 0x27F))
			{
				u32 index = ((tile & 0xFF) ^ 0x80);
				((struct UiRenderWork *)base)->tile_attributes[index] &= 0xFC;
			}
		}
		cursor += 32 - width;
	}
}

void UiWindow_MarkVisibleTileAttributes(void)
{
	u8	*base = gWindowWork[0];
	u16 *cursor = (u16 *)base;
	s32	 width = 30;
	u32	 alternate = ((struct UiRenderWork *)base)->alt;
	s32	 row;
	s32	 x;

	for (row = 20; row != 0; row--)
	{
		for (x = 0; x < width; x++)
		{
			u32 tile = *cursor++ & 0x3ff;

			if ((tile - 0x80) <= 0x7f || (alternate != 0 && tile > 0x1ff && tile <= 0x27f))
			{
				u32 index = ((tile & 0xff) ^ 0x80);

				((struct UiRenderWork *)base)->tile_attributes[index] |= 2;
			}
		}
	}

	for (row = 0; row < 256; row++, base++)
	{
		if (((struct UiRenderWork *)base)->tile_attributes[0] == 1)
			((struct UiRenderWork *)base)->tile_attributes[0] = 0;
	}
}

void UiWork_SetAltFlagAndClearTable(s32 flag)
{
	s32 i;
	s32 j;
	s8 *p;
	s8 *q;
	u8 *work;

	work = gWindowWork[0];
	if (flag != 0)
	{
		((struct UiRenderWork *)work)->alt = 1;
		flag = 0;
		for (i = 0x80, p = (s8 *)((struct UiRenderWork *)work)->tile_attributes + 0x80; i <= 0xFF;
			i += 1)
		{
			*p = flag;
			p += 1;
		}
		return;
	}
	((struct UiRenderWork *)work)->alt = 0;
	flag = 0;
	q = (s8 *)((struct UiRenderWork *)work)->tile_attributes + 0x80;
	j = 0x7F;
	do
	{
		j -= 1;
		*q = flag;
		q += 1;
	} while (j >= 0);
}

void UiRender_ReservedNoOp(void)
{
}

void UiWindow_DrawDividerLine(struct RenderInput *win, u32 x1, u32 y1, u32 x2, u32 y2)
{
	struct UiRenderWork *work = UI_WORK;
	u16					*cursor;
	u32					 pos;
	u32					 tile;

	if (x1 == x2)
	{
		if (y1 == y2)
			return;
		if (y1 > y2)
		{
			pos = y1;
			y1 = y2;
			y2 = pos;
		}
		UiWindow_ClearTileAttributesInRect(win->x + x2, win->y + y1, 1, y2 - y1);
		cursor = (u16 *)((((win->y + y1) << 5) + win->x + x2) * 2 + (u32)work->tilemap);
		for (pos = y1; pos <= y2; pos++)
		{
			tile = *cursor;
			if (pos == y1)
			{
				switch (tile)
				{
				case 0xf009:
				case 0xf00b:
				case 0xf00c:
				case 0xf00d:
				case 0xf018:
					break;
				case 0xf011:
					tile = 0xf018;
					break;
				case 0xf00a:
					tile = 0xf00d;
					break;
				case 0xf00e:
					tile = 0xf009;
					break;
				default:
					tile = 0xf00f;
					break;
				}
			}
			else if (pos == y2)
			{
				switch (tile)
				{
				case 0xf00a:
				case 0xf00b:
				case 0xf00c:
				case 0xf00d:
				case 0xf019:
					break;
				case 0xf014:
					tile = 0xf019;
					break;
				case 0xf009:
					tile = 0xf00d;
					break;
				case 0xf00e:
					tile = 0xf00a;
					break;
				default:
					tile = 0xf00f;
					break;
				}
			}
			else
			{
				switch (tile)
				{
				case 0xf00b:
				case 0xf00c:
				case 0xf00d:
					break;
				case 0xf009:
				case 0xf00a:
				case 0xf00e:
					tile = 0xf00d;
					break;
				default:
					tile = 0xf00f;
					break;
				}
			}
			*cursor = tile;
			cursor += 32;
		}
	}
	else if (y1 == y2)
	{
		if (x1 == x2)
			return;
		if (x1 > x2)
		{
			pos = x1;
			x1 = x2;
			x2 = pos;
		}
		UiWindow_ClearTileAttributesInRect(win->x + x1, win->y + y1, x2 - x1, 1);
		cursor = (u16 *)((((win->y + y1) << 5) + win->x + x1) * 2 + (u32)work->tilemap);
		for (pos = x1; pos <= x2; pos++)
		{
			tile = *cursor;
			if (pos == x1)
			{
				switch (tile)
				{
				case 0xf009:
				case 0xf00a:
				case 0xf00b:
				case 0xf00d:
				case 0xf01a:
					break;
				case 0xf016:
					tile = 0xf01a;
					break;
				case 0xf00c:
					tile = 0xf00d;
					break;
				case 0xf00f:
					tile = 0xf00b;
					break;
				default:
					tile = 0xf00e;
					break;
				}
			}
			else if (pos == x2)
			{
				switch (tile)
				{
				case 0xf009:
				case 0xf00a:
				case 0xf00c:
				case 0xf00d:
				case 0xf01b:
					break;
				case 0xf017:
					tile = 0xf01b;
					break;
				case 0xf00b:
					tile = 0xf00d;
					break;
				case 0xf00f:
					tile = 0xf00c;
					break;
				default:
					tile = 0xf00e;
					break;
				}
			}
			else
			{
				switch (tile)
				{
				case 0xf009:
				case 0xf00a:
				case 0xf00d:
					break;
				case 0xf00b:
				case 0xf00c:
				case 0xf00f:
					tile = 0xf00d;
					break;
				default:
					tile = 0xf00e;
					break;
				}
			}
			*cursor = tile;
			cursor++;
		}
	}
}

void UiWork_SetParamNibble(s32 param)
{
	struct UiRenderWork *work = UI_WORK;

	work->colour = param & 15;
}

void UiWork_SetRenderWord(u16 value)
{
	struct UiRenderWork *work = UI_WORK;

	work->outline = value;
}

void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
	struct UiRenderWork *canvas = UI_WORK;
	u16					*counter = &canvas->count;
	s32					 zero = 0;

	*counter = zero;
	UiText_BuildRenderEntries(arg0, 1);
	canvas->entries[*counter] = zero;
	*counter = (*counter + 1) & RENDER_ENTRY_MASK;
	UiText_RenderWideStringAtOffset(canvas->entries, (struct UiWindow *)arg1, arg2, arg3);
}

void UiText_DrawCharacterAtOffset(
	s32 character, struct RenderInput *position, u32 offset_x, u32 offset_y)
{
	s32					 byte_offset;
	s32					 vram_address;
	u16					*text;
	s32					 zero;
	u32					 cell;
	struct UiRenderWork *canvas;
	u16					*counter;

	canvas = UI_WORK;
	counter = &canvas->count;
	zero = 0;
	*counter = zero;
	UiText_BuildRenderEntries(character, 1);

	canvas->entries[*counter] = zero;
	*counter = (*counter + 1) & RENDER_ENTRY_MASK;

	cell = ((position->y + (offset_y >> 3) + 1) << 5) + (position->x + (offset_x >> 3)) + 1;
	if (cell < 0x280U)
	{
		byte_offset = cell * 2;
		vram_address = byte_offset + 0x06002000;
		text = canvas->entries;
		UiText_RenderStringTiles(
			text, (u16 *)((u8 *)canvas->tilemap + byte_offset), (u16 *)vram_address, 7 & offset_x);
	}
}

void UiText_DrawString(u8 *text, s32 arg1, s32 arg2, s32 arg3)
{
	s16 *buffer;
	s16 *output;
	u8	*input;

	input = text;
	buffer = Runtime_BumpAllocateAlternatePool(0x200);
	output = buffer;
	if (*input != 0)
	{
		do
		{
			*output = *input;
			input++;
			output++;
		} while (*input != 0);
	}
	*output = 0;
	UiText_RenderWideStringAtOffset((u16 *)buffer, (struct UiWindow *)arg1, arg2, arg3);
	MemFree(buffer);
}

void UiText_DrawStringAtOffset(u8 *text, struct RenderInput *work, s32 offset_x, s32 offset_y)
{
	s16					*buffer;
	s16					*output;
	struct UiRenderWork *canvas;
	u32					 cell;
	s32					 vram;
	s32					 src;
	s32					 dst;
	s32					 phase;

	buffer = Runtime_BumpAllocateAlternatePool(0x200);
	canvas = UI_WORK;
	output = buffer;
	if (*text != 0)
	{
		do
		{
			*output = *text;
			text++;
			output++;
		} while (*text != 0);
	}
	*output = 0;

	cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5) + (work->x + ((u32)offset_x >> 3)) + 1;
	if (cell < 0x280)
	{
		vram = 0x06002000;
		cell *= 2;
		dst = vram + cell;
		src = (s32)canvas->tilemap + cell;
		phase = offset_x & 7;
		UiText_RenderStringTiles((u16 *)buffer, (u16 *)src, (u16 *)dst, phase);
		MemFree(buffer);
	}
}

void UiText_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y)
{
	u16 *buffer = Runtime_BumpAllocateAlternatePool(0x200);
	u16 *output = buffer;

	while (*text != 0)
	{
		*output = *text;
		text++;
		output++;
	}
	*output = 0;
	x >>= 3;
	y >>= 3;
	UiText_RenderWideStringInWindow((s16 *)buffer, (struct UiWindow *)arg1, x, y);
	MemFree(buffer);
}

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
	u8 data[16];

	UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberAtOffset(s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4)
{
	u8 data[16];

	UiText_DrawStringAtOffset(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
	u8 data[16];

	UiText_DrawStringInWindow(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}

void UiText_DrawPrefixedNumberAtOffset(
	s32 value, struct RenderInput *work, s32 offset_x, s32 offset_y, s32 variant)
{
	u8					 formatted[16];
	s16					 output[8];
	u8					*text;
	s32					 index;
	u32					 cell;
	struct UiRenderWork *canvas;
	s32					 vram;
	s32					 src;
	s32					 dst;
	s32					 phase;

	canvas = UI_WORK;
	text = UiText_FormatNumber(formatted, value, 4);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	if (variant == 0)
	{
		output[0] = 0xf01d;
		output[1] = 0xf01e;
	}
	else
	{
		output[0] = 0xf01f;
		output[1] = 0xf005;
	}
#else
	if (variant == 0)
	{
		output[0] = 0xf01d;
	}
	else
	{
		output[0] = 0xf01f;
	}
	output[1] = 0xf01e;
#endif
	for (index = 0; index <= 4; index++)
	{
		output[index + 2] = text[index];
	}
	output[6] = 0;

	cell = ((work->y + ((u32)offset_y >> 3) + 1) << 5) + (work->x + ((u32)offset_x >> 3)) + 1;
	if (cell < 0x280)
	{
		vram = 0x06002000;
		cell *= 2;
		dst = vram + cell;
		src = (s32)canvas->tilemap + cell;
		phase = offset_x & 7;
		UiText_RenderStringTiles((u16 *)output, (u16 *)src, (u16 *)dst, phase);
	}
}
