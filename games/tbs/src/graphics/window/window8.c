#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window5.h"
#include "unit/graphics/window/window6.h"
#include "unit/graphics/window/window7.h"
#include "unit/graphics/window/window8.h"
#include "unit/graphics/window/window9.h"
#include "unit/graphics/window/window10.h"
#include "unit/graphics/window/window12.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object11.h"
#include "unit/graphics/text/moji.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
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

#define TILE_ROW_FROM_BOTTOM(tile, up) ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

void UiWindow_BuildLayoutBounds(s32 flags)
{
	void					**slot = (void **)&gPartyStatusWork;
	struct PartyStatusWindow *state = *slot;
	struct UiRenderWork		 *work = slot[-1];
	s32						  height = 4;
	s32						  n;
	s32						  right;
	s32						  left;

	if (work->menu_state != 0)
	{
		n = BattleParty_PrepareActiveOwnersFar(0);
		height = 3;
	}
	else
	{
		n = Party_CountActiveOwnersFar();
	}
	if (flags & 1)
		height++;
	else
		flags &= -3;

	n *= 6;
	right = n + 1;
	if (flags & 2)
		right += 5;

	left = 30;
	left -= right;
	state->x = left;
	state->y = 0;
	state->width = right;
	state->height = height;
	state->flags = flags;
}

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
	s32						  zero;
	struct PartyStatusWindow *window;
	s8						 *busy;

	window = Runtime_AllocateBlock(16, sizeof(*window));
	busy = (s8 *)&UI_WORK->menu_busy;
	zero = 0;
	*busy = 1;
	UiWindow_BuildLayoutBounds(flags);
	window->window = UiWindow_Create(window->x, window->y, window->width, window->height, 6);
	PtyStDraw(flags);
	*busy = zero;
}

void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags)
{
	struct UiRenderWork *canvas = UI_WORK;
	s32					 first = 1;
	s32					 bias = 0;
	u32					 max = window->width - 1;
	s32					 i;
	u32					 rows = window->height;
	u32					 col;
	u32					 row;
	u16					*dest;

	if ((flags & 1) == 0)
		flags &= ~2;
	if (flags & 2)
	{
		bias = 5;
		first = 0;
	}
	i = first;
	while (UiWindow_PartyColumnOffsets[i] >= 0)
	{
		col = UiWindow_PartyColumnOffsets[i] + bias;
		if (col < max)
		{
			for (row = 0; row != rows; row++)
			{
				dest = (u16 *)(((window->y + row) * 32 + (window->x + col)) * 2
							   + (u32)canvas->tilemap);
				if (row == 0)
					*dest = 0xf018;
				else if (row == rows - 1)
					*dest = 0xf019;
				else
					*dest = 0xf00f;
			}
		}
		i++;
	}
	if (canvas->menu_state)
	{
		dest = canvas->tilemap + (window->y + window->height - 1) * 32 + window->x;
		col = 1;
		*dest++ = 0xf080;
		while (col < max)
		{
			col++;
			*dest++ = 0xf081;
		}
		*dest = 0xf082;
	}
	canvas->dirty = 1;
}

s32 UiWindow_DrawStatusBarTiles(struct RenderInput *window, s32 x, s32 y, s32 value)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	s32					 i;
	struct UiRenderWork *canvas = UI_WORK;
	s32					 filled = value;
	s32					 row;
	s32					 col;
	u32					 id;
	u32					 light;
	u32					 dark;
	u32					 pixels;
	u32					 result;

	if (canvas->menu_state == 0)
	{
		Dma_Set((const void *)Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(14, 0),
			0x80000010, REG_DMA3);
		BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
	}
	x += window->x;
	y += window->y;
	for (i = 0; i < 5; i++, x++)
	{
		id = canvas->tilemap[y * 32 + x];
		light = 0x22222222;
		dark = 0xcccccccc;
		id &= 0x3ff;
		if (value > 7)
		{
			light = 0x88888888;
			dark = 0xdddddddd;
		}
		else if (value >= 0)
		{
			light <<= value * 4;
			light |= 0x88888888U >> (32 - value * 4);
			dark <<= value * 4;
			dark |= 0xddddddddU >> (32 - value * 4);
		}
		for (row = 0; row <= (filled != 0 ? 2 : 0); row++)
		{
			pixels = *TILE_ROW_FROM_BOTTOM(id, row);
			result = 0;
			for (col = 0; col <= 7; col++)
			{
				u32 color = pixels & 15;

				if (color == 14)
					result |= light & (15U << (col * 4));
				else if (color == 1)
					result |= dark & (15U << (col * 4));
				else
					result |= color << (col * 4);
				pixels >>= 4;
			}
			*TILE_ROW_FROM_BOTTOM(id, row) = result;
		}
		value -= 8;
	}
}

s32 __divsi3(s32, s32);

void PtyStDraw(s32 flags)
{
	void					**slot = (void **)gPartyStatusWork;
	struct PartyStatusWindow *layout = slot[0];
	struct BtlWork			 *party = slot[-7];
	struct RenderInput		 *window = (struct RenderInput *)layout->window;
	struct UiRenderWork		 *work = slot[-1];
	s32						  x = 0;
	u32						  count;
	s32						  y = 0;
	u32						  i;
	u16						  owners[5];
	u8						  djinn[4];

	if (work->menu_state != 0)
	{
		count = BattleParty_PrepareActiveOwnersFar(0);
		y = -1;
		for (i = 0; i < count; i++)
		{
			owners[i] = party->party_units[i];
			if (owners[i] == 255)
				break;
		}
	}
	else
	{
		count = Party_CountActiveOwnersFar();
		for (i = 0; i < count; i++)
			owners[i] = gGameState.active_owners[i];
		owners[i] = 255;
	}
	count = i;
	if (flags == -1)
		flags = layout->flags;
	if (!(flags & 1))
		flags &= ~2;
	if (work->menu_state == 0 || BattlePlacement_CountValidEntriesFar(0, 0) == 0)
		flags &= ~2;
	if (flags == 9)
	{
		UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
		return;
	}
	work->menu_busy = 1;
	if (layout->flags == flags)
	{
		RenderOutput_RedrawSavedRect((struct UiWindow *)window);
		UiWindow_DrawColumnBorders(window, flags);
	}
	else
	{
		UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
		UiWindow_BuildLayoutBounds(flags);
		window->width = layout->width;
		window->height = layout->height;
		window->x = layout->x;
		UiWindow_DrawFrame(layout->x, layout->y, layout->width, layout->height);
		UiWindow_DrawColumnBorders(window, flags);
	}
	if (flags & 2)
		x = 5;
	for (i = 0; i != count; i++)
	{
		struct BtlUnit *unit = UnitGetFar(owners[i]);
		s32				hp = unit->hp;
		s32				maximum = unit->max_hp;
		s32				value;
		s32				current;

		if (hp == 0)
			UiWork_SetParamNibble(2);
		else if (hp <= maximum / 4)
			UiWork_SetParamNibble(4);
		else
			UiWork_SetParamNibble(15);
		work->level = 14;
		if (work->menu_state != 0)
			work->level = 5;
		UiText_DrawPrefixedNumberAtOffset(hp, window, (x + i * 6) * 8, y * 8 + 8, 0);
		work->level = 15;
		UiText_DrawStringAtOffset(unit->name, window, (x + i * 6) * 8, y * 8);
		UiWork_SetParamNibble(15);
		if (unit->max_hp != 0)
		{
			current = unit->hp;
			value = __divsi3(current * 40, unit->max_hp);
			if (value == 0 && current != 0)
				value = 1;
			UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 2, value);
		}
		if (flags & 1)
		{
			work->level = 14;
			if (work->menu_state != 0)
				work->level = 5;
			UiText_DrawPrefixedNumberAtOffset(unit->ep, window, (x + i * 6) * 8, y * 8 + 16, 1);
			if (unit->max_ep != 0)
			{
				current = unit->ep;
				value = __divsi3(current * 40, unit->max_ep);
				if (value == 0 && current != 0)
					value = 1;
				UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 3, value);
			}
		}
	}
	work->level = 15;
	if (work->menu_state != 0 && (flags & 2))
	{
		s32 row = y;

		if (flags & 1)
			row++;
		BattlePlacement_CountValidEntriesFar(0, djinn);
		UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5001, 0, row, 0);
		UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5002, 2, row, 0);
		UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5003, 0, row + 1, 0);
		UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5004, 2, row + 1, 0);
		UiWindow_PutGlyph(window, djinn[0] + '0', 1, row);
		UiWindow_PutGlyph(window, djinn[1] + '0', 3, row);
		UiWindow_PutGlyph(window, djinn[2] + '0', 1, row + 1);
		UiWindow_PutGlyph(window, djinn[3] + '0', 3, row + 1);
	}
	work->menu_busy = 0;
}

void UiWork_FinalizeAndReleaseBlock16(void)
{
	UiWork_Finalize((struct UiWindow *)**(s32 **)(u32)&gWindowWork[1], 1);
	Runtime_ReleaseHeapBlock(0x10);
}

void UiWindow_SetTileAttributeBitRect(
	const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
	struct UiRenderWork *work = UI_WORK;

	x += window->x + 1;
	y += window->y + 1;
	field &= 1;
	field <<= 12;
	if (x < 0)
	{
		width += x;
		x = 0;
	}
	if (x + width > 29)
	{
		width = 30 - x;
	}
	if (y < 0)
	{
		height += y;
		y = 0;
	}
	if (y + height > 29)
	{
		height = 20 - y;
	}
	if (width > 0 && height > 0)
	{
		y <<= 6;
		x = y + (x << 1);
		do
		{
			u16 *cell = (u16 *)(x + (u32)work->tilemap);
			s32	 remaining = width;
			while (remaining != 0)
			{
				u32 value = *cell;
				value &= 0xFFFFEFFF;
				value |= field;
				remaining--;
				*cell = value;
				cell++;
			}
			height--;
			x += 64;
		} while (height != 0);
		work->dirty = 1;
	}
}
