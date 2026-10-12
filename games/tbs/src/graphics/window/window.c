//--------------------------------------------------------------------------
// output_acquire_free.c
// 表示：スプライト出力（RenderOutput）のあきリスト
//--------------------------------------------------------------------------

#include "unit/graphics/font/mark_codes.h"
#include "unit/graphics/window/scales.h"
#include "unit/graphics/window/sideobjt.h"
#include "unit/graphics/window/spranim.h"
#include "unit/graphics/window/textspd.h"
#include "unit/menu/optdata.h"
#include "unit/graphics/window/window.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/graphics/text/moji.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
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

#define UI_WORK ((struct UiRenderWork *)gWindowWork[0])

//--------------------------------------------------------------------------
// あきリストの先頭をとり出す（なければ NULL）
//--------------------------------------------------------------------------
struct RenderOutput *RenderOutput_AcquireFree(void)
{
	struct UiRenderWork *work = UI_WORK;
	struct RenderOutput *out = work->free_outputs.head;

	if (out != NULL)
	{
		if (out->next == NULL)
			work->free_outputs.tail_link = &work->free_outputs.head; // 空になった
		work->free_outputs.head = out->next;
		out->next = NULL;
	}
	return out;
}

//--------------------------------------------------------------------------
// あきリストの最後に返す（outputs[] の外のものは返さない）
//--------------------------------------------------------------------------
void RenderOutput_ReleaseFree(struct RenderOutput *out)
{
	struct UiRenderWork *work = UI_WORK;

	if ((u32)out >= (u32)work->outputs && (u32)out < (u32)&work->free_outputs.head)
	{
		struct RenderOutput **tail = work->free_outputs.tail_link;

		work->free_outputs.tail_link = &out->next;
		*tail = out;
		out->next = NULL;
	}
}

//--------------------------------------------------------------------------
// outputs[] の 64 個をすべてつないで、あきリストにする
//--------------------------------------------------------------------------
void UiWork_InitFreeList(void)
{
	s32					 n;
	struct UiRenderWork *work;
	struct RenderOutput *out;
	struct RenderOutput *next;

	work = UI_WORK;
	out = work->outputs;
	work->free_outputs.head = out;
	n = UI_OUTPUT_COUNT - 2;
	do
	{
		next = out + 1;
		n--;
		out->next = next;
		out = next;
	} while (n >= 0);
	next->next = NULL;
	work->free_outputs.tail_link = &next->next;
}

void UiWork_InitializeWithResourceCounters(void)
{
	struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
	volatile u32 fill;
#endif

	work = Runtime_AllocateBlock(15, RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
	fill = 0;
	Dma_Set((const void *)&fill, work, 0x850004bf, REG_DMA3);
#else
	Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
	work->dirty = 1;
	work->glyph_resource = 99;
	work->level = 15;
#if EDITION_INTERNATIONAL
	fill = 0xf000f000;
	Dma_Set((const void *)&fill, work, 0x85000140, REG_DMA3);
#else
	Iwram_FillWords(work, 0x500, 0xf000f000);
#endif
	UiWork_InitFreeList();
	UiWork_SetTwoEntriesTo999();
	Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
	UiWork_InitCountersWithResourceAndScheduleRefresh();
}

void UiWork_Initialize(s32 kind)
{
	struct UiRenderWork *work;
#if EDITION_INTERNATIONAL
	volatile u32 fill;
#endif
	s32 i;
	s32 UiWork_CopyTileTail(u32 src, u32 dst)
	{
		src &= 0x3ff;
		dst &= 0x3ff;
		Dma_Set(
			(void *)(0x06000010 + src * 32), (void *)(0x06000000 + dst * 32), 0x80000008, REG_DMA3);
		return Iwram_ClearWords((void *)(0x0600000c + dst * 32), 20);
	}

	work = (struct UiRenderWork *)Runtime_AllocateBlock(15, RENDER_WORK_SIZE);
#if EDITION_INTERNATIONAL
	fill = 0;
	Dma_Set(&fill, work, (0x85000000 | RENDER_WORK_SIZE / 4), REG_DMA3);
#else
	Iwram_ClearWords(work, RENDER_WORK_SIZE);
#endif
	work->dirty = 1;
	work->glyph_resource = 99;
	work->menu_state = 1;
	work->level = 15;
#if EDITION_INTERNATIONAL
	fill = 0xf000f000;
	Dma_Set(&fill, work, 0x85000140, REG_DMA3);
#else
	Iwram_FillWords(work, 0x500, 0xf000f000);
#endif
	UiWork_InitFreeList();
	Scheduler_AddOrUpdateCallback(UiWork_UploadDirtyBlocks, 0x480);
	UiWork_InitCountersAndScheduleRefresh(kind);
	UiWork_CopyTileTail(0xf013, 128);
	UiWork_CopyTileTail(0xf014, 129);
	UiWork_CopyTileTail(0xf015, 130);
	{
		u8 mode = 4;

		for (i = 2; i >= 0; i--)
			work->tile_attributes[i] = mode;
	}
}

void UiWork_UploadDirtyBlocks(void)
{
	struct UiRenderWork *work = UI_WORK;
	u32					 flags;
	u8					*src;
	u8					*dst;
	if (!work->menu_busy)
	{
		flags = work->dirty;
		if (flags)
		{
			dst = (u8 *)0x06002000;
			src = (u8 *)work->tilemap;
			if (flags & 1)
				flags = 63;
			flags &= 63;
			flags >>= 1;
			do
			{
				if (flags & 1)
					Dma_Set(src, dst, 0x84000040, REG_DMA3);
				flags >>= 1;
				src += 256;
				dst += 256;
			} while (flags);
			work->dirty = flags;
		}
	}
}

void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height)
{
	struct UiRenderWork *canvas = UI_WORK;
	u16					*cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
	s32					 tile;
	u32					 bottom;
	u32					 row;
	u8					*mode;

	tile = 240;
	bottom = y + height;
	tile <<= 8;
	if (bottom > 20)
		height = 20 - y;
	if (width <= 1)
		width = 2;
	if (width > 30)
		width = 30;
	if (height <= 1)
		height = 2;
	if (height > 30)
		height = 30;

	UiWindow_ClearTileAttributesInRect(x, y, width, height);

	row = 0;
	if (row < height)
	{
		mode = &canvas->menu_state;
		do
		{
			u32 column;

			if (*mode != 0)
			{
				if ((y + row) > 16)
					tile = 0xF07F;
				else
					tile = 0xF000;
			}
			column = 0;
			if (column < width)
			{
				do
				{
					*cursor++ = tile;
					column++;
				} while (column < width);
			}
			row++;
			cursor += 32 - width;
		} while (row < height);
	}
	canvas->dirty = 1;
}

void UiWork_DrawByAttributes(struct UiWindow *window)
{
	u32					 flags;
	u32					 value;
	u32					 x;
	u32					 y;
	u32					 width;
	u32					 height;
	struct UiRenderWork *work;

	work = UI_WORK;
	value = window->height;
	flags = window->flags;
	height = value;
	value = 0;
	window->duration = value;
	x = window->x;
	y = window->y;
	width = window->width;
	if (8 & flags)
	{
		if (0x20 & flags)
		{
			UiWindow_DrawFrame(x, y, width, height);
			Iwram_FillWords((void *)0x06002500, 0xF00, 0x44444444);
		}
		else
		{
			Iwram_FillWords((void *)0x06002500, 0xF00, 0);
		}
		UiWindow_MapTextCanvasTiles(x, y, width, height, 0);
	}
	else
	{
		UiWindow_DrawFrame(x, y, width, height);
	}
	work->dirty = 1;
}

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs)
{
	struct UiWindow *slot;
	struct UiWindow *found;
	s32				 i;

	slot = UI_WORK->windows;
	found = 0;
	i = 0;
	while ((slot->flags & 1) != 0 || slot->duration != 0)
	{
		i++;
		slot++;
		if (i == WINDOW_COUNT)
		{
			goto done;
		}
	}
	found = slot;
done:
	if (found != 0)
	{
		found->y = y;
		found->width = width;
		found->height = height;
		found->x = x;
		found->output.head = NULL;
		found->state = 0;
		found->output.tail_link = &slot->output.head;
		found->unknown_10 = 1;
		found->flags = 1;
		/* FAKEMATCH: UiWork_ResetCounters is called as void (*)(); as void (*)(void), its prototype, UiWindow_Create does not compile */
		((void (*)())UiWork_ResetCounters)(x);
		if (attrs & 8)
		{
			found->flags |= 8;
		}
		if (attrs & 32)
		{
			found->flags |= 32;
		}
		if (attrs & 64)
		{
			found->flags |= 64;
		}
		if (attrs & 128)
		{
			found->flags |= 128;
		}
		if (attrs & 0x100)
		{
			found->flags |= 0x100;
		}
		if (attrs & 2)
		{
			found->flags |= 2;
			found->frame = 0;
			found->duration = 1;
			UiWork_DrawByAttributes(found);
		}
		else
		{
			found->duration = 8;
			found->frame = 7;
			UiWork_WaitWindowDuration(found);
			WaitFrames(1);
		}
	}
	return found;
}

void UiWork_WaitWindowDuration(struct UiWindow *window)
{
	if (!(2 & window->flags) && window->duration != 0)
	{
		do
		{
			WaitFrames(1);
		} while (window->duration != 0);
	}
}

void UiWork_Finalize(struct UiWindow *work, s32 release)
{
	u16 zero;

	if (work == 0)
		return;

	RenderOutput_PrepareForRedraw(work);
	work->previous_x = work->x;
	work->previous_y = work->y;
	work->previous_width = work->width;
	zero = 0;
	work->flags = zero;
	work->previous_height = work->height;

	if (release != 0)
	{
		UiWindow_EraseBorderRect(work->x, work->y, work->width, work->height);
		work->output.head = NULL;
		work->output.tail_link = NULL;
		work->width = zero;
		work->height = zero;
		work->x = zero;
		work->y = zero;
		work->unknown_10 = zero;
		work->effect = zero;
		work->state = zero;
		work->flags = zero;
		work->frame = zero;
		work->duration = zero;
		work->previous_x = zero;
		work->previous_y = zero;
		work->previous_width = zero;
		work->previous_height = zero;
	}
	else
	{
		work->frame = release;
		work->duration = 4;
	}
}

void RenderOutput_PrepareForRedraw(struct UiWindow *window)
{
	if (!(8 & window->flags))
	{
		RenderOutput_RedrawSavedRect(window);
		RenderOutput_ClearList(window);
	}
}

void RenderOutput_RedrawSavedRect(struct UiWindow *window)
{
	UiWindow_DrawFrame(window->x, window->y, window->width, window->height);
}

void RenderOutput_ClearList(void *work)
{
	struct RenderOutputList *list = work;
	struct RenderOutput		*node;
	struct RenderOutput		*next;

	next = NULL;
	if (list != NULL)
	{
		node = list->head;
		list->tail_link = &list->head;
		list->head = next;
		while (node != NULL)
		{
			next = node->next;
			RenderOutput_Release(node);
			node = next;
		}
	}
}

void UiWindow_ClearInteriorTiles(
	const struct RenderInput *window, u32 left, u32 top, u32 right, u32 bottom)
{
	struct UiRenderWork *work;
	u32					 x;
	u32					 y;
	u32					 width;
	u32					 height;
	u16					*tiles;
	u32					 row;

	right += 7;
	bottom += 7;
	right >>= 3;
	bottom >>= 3;
	x = left >> 3;
	work = UI_WORK;
	y = top >> 3;
	x += window->x;
	y += window->y;
	right += window->x;
	bottom += window->y;
	left = x + 1;
	top = y + 1;
	width = right - x;
	height = bottom - y;
	UiWindow_ClearTileAttributesInRect(left, top, width, height);
	tiles = work->tilemap + top * 32 + left;
	for (row = 0; row < height; row++)
	{
		for (left = 0; left < width; left++)
			*tiles++ = 0xf020;
		tiles += 32 - width;
	}
	work->dirty = 1;
}

void UiWork_UpdateListTail(struct RenderOutputList *list)
{
	struct RenderOutput	 *node = list->head;
	struct RenderOutput **tail = &list->head;

	while (node != NULL)
	{
		tail = &node->next;
		node = node->next;
	}
	list->tail_link = tail;
}

void RenderOutput_AppendToList(struct RenderOutputList *list, struct RenderOutput *node)
{
	/* FAKEMATCH: needed to match: the first-word tail store preserves the two stores in order; the typed tail-link field reverses them. */
	if (list != NULL)
	{
		*list->tail_link = node;
		*(struct RenderOutput **)&list->tail_link = node;
	}
}

void RenderOutput_Release(struct RenderOutput *entry)
{
	RenderOutput_ReleaseFree(entry);
	if ((u8)entry->kind != 0)
	{
		Resource_ResetEntry((u8)entry->index);
		if ((u8)entry->kind == 2)
		{
			u8 *dst = gWindowWork[0];
			s32 idx = entry->oam.palette * 2 + RENDER_PALETTE_TBL_OFS;
			*(u16 *)(dst + idx) = 0x3E7;
		}
	}
	entry->active = 0;
}

struct UiChannelSlot *UiText_QueueRenderEntries(
	struct UiWindow *window, s32 entry, s32 x, s32 y, u16 *spacing, s32 flags)
{
	struct UiChannelSlot *render = UI_WORK->channels;
	struct UiChannelSlot *found = NULL;
	u32					  i;

	for (i = 0; i != 3; i++, render++)
	{
		if (render->work == NULL)
		{
			found = render;
			break;
		}
	}
	if (found != NULL)
	{
		found->work = window;
		found->start_x = x << 8;
		found->x = x << 8;
		found->y = y << 8;
		found->entry = entry;
		found->colour = 15;
		found->line_spacing = 10;
		found->countdown = 0;
		found->outline = 0;
		found->ended = 0;
		found->flags = flags;
		if (spacing != NULL)
		{
			for (i = 0; i < 4; i++)
				found->spacing[i] = *spacing++;
		}
		else
		{
			for (i = 0; i < 4; i++)
				found->spacing[i] = 0;
		}
		found->line = 0;
	}
	return found;
}

struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *work, s32 value, s32 preserve)
{
	struct UiChannelSlot *slot;
	struct UiChannelSlot *selected;
	u16					 *destination;
	u16					  val04;
	u16					  zero;
	u32					  index;

	slot = UI_WORK->channels;
	selected = 0;
	for (index = 0; index != 3; slot++, index++)
	{
		if (slot->work == 0 || slot->work->state != 0)
		{
			selected = slot;
			break;
		}
	}

	if (selected != 0)
	{
		if (selected->work == 0)
		{
			selected->y = 0xA00;
			val04 = 0x300;
			selected->work = work;
			goto reset_x;
		}
		if (preserve == 0)
		{
			if (selected->y == 0)
			{
				selected->y = 0xA00;
			}
			else if (selected->y < 0xD00)
			{
				selected->y += 0xD00;
			}
			else
			{
				UiWork_ResetChannelTransition(selected);
			}
			val04 = 0x300;
		reset_x:
			selected->x = val04;
		}

		zero = 0;
		selected->start_x = 0x300;
		selected->work->state = zero;
		selected->colour = 15;
		selected->line_spacing = 10;
		selected->entry = value;
		selected->countdown = zero;
		selected->outline = zero;
		selected->line = zero;
		selected->ended = zero;
		index = 0;
		destination = selected->spacing;
		do
		{
			index++;
			*destination++ = zero;
		} while (index <= 3);
	}
	return selected;
}

s32 Ui_ClearVramBlock(void)
{
	return Iwram_FillWords((void *)0x06002500, 0xF00, 0);
}

s32 Ui_FillVramBlockPattern(void)
{
	return Iwram_FillWords((void *)0x06002500, 0xF00, 0x44444444);
}

void UiWork_ResetFreeChannel(void)
{
	struct UiChannelSlot *slot = UI_WORK->channels;
	struct UiChannelSlot *sel = 0;
	s32					  i;

	for (i = 0; i != 3; slot++, i++)
	{
		if (slot->work == 0 || slot->work->state != 0)
		{
			sel = slot;
			break;
		}
	}
	if (sel != 0)
	{
		if (sel->work != 0)
		{
			Ui_ClearVramBlock();
			sel->y = 0;
		}
		sel->x = 0;
		sel->countdown = 0;
		sel->colour = 0xF;
		sel->outline = 0;
		sel->line_spacing = 0xA;
	}
}

void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel)
{
	struct UiRenderWork *work = UI_WORK;

	work->colour = channel->colour;
	work->outline = channel->outline;
	work->line_spacing = channel->line_spacing;
}

void UiWork_ResetChannelTransition(struct UiChannelSlot *channel)
{

	channel->transition = 2;
}

#define PANEL_TILE_ROWS ((u32 *)0x06002500)
#define PANEL_ROW_COUNT 30
#define PANEL_ROW_WORDS 32
#define PANEL_ROW_LEAD 8

void UiWork_ShiftPanelRowsLeft(s32 step)
{
	s32	 shift = step * 6;
	u32 *base = PANEL_TILE_ROWS;
	s32	 n;

	for (n = 0; n < PANEL_ROW_COUNT; n++)
	{
		u32 *row = base + n * PANEL_ROW_WORDS;

		Dma_Set(row + PANEL_ROW_LEAD + shift, row + PANEL_ROW_LEAD,
			0x84000000 | (PANEL_ROW_WORDS - PANEL_ROW_LEAD - shift), REG_DMA3);
		Iwram_FillWords(row + (PANEL_ROW_WORDS - shift), shift * 4, 0);
	}
}

void UiWork_ProcessRenderChannels(void)
{
	struct UiChannelSlot *channel = UI_WORK->channels;
	s32					  channel_no = 0;
	s32					  one = 1;

	do
	{
		struct UiWindow *current = channel->work;

		if (current != 0 && *(s32 *)&current->frame == 0)
		{
			u16 flags = current->flags;

			if (flags == 0)
			{
				channel->work = NULL;
			}
			else
			{
				s32 pending = current->effect;
				s32 kind;

				if (pending != 0)
				{
					UiWork_AdvanceChannelTransition(channel);
				}
				else
				{
					kind = UiWork_StepChannelScript(channel);
					switch (kind)
					{
					case 8:
						channel->work->state = one;
						break;
					case 9:
					{
						struct UiWindow *entity = channel->work;

						UiWork_Finalize(entity, entity->flags & 2);
						channel->x = pending;
						channel->y = pending;
						channel->entry = pending;
						channel->countdown = pending;
						channel->colour = pending;
						channel->outline = pending;
						channel->line_spacing = pending;
						channel->work->state = one;
						break;
					}
					}
				}
			}
		}
		channel_no++;
		channel++;
	} while (channel_no != 3);
}

enum
{
	TEXT_END = 0,
	TEXT_PAGE = 1,
	TEXT_WAIT_KEY = 2,
	TEXT_NEWLINE = 3,
	TEXT_PAUSE_LONG = 4,
	TEXT_PAUSE_SHORT = 5,
	TEXT_PAUSE_LONGEST = 6,
	TEXT_COLOURS_RESET = 7,
	TEXT_COLOUR = 8,
	TEXT_OUTLINE = 9,
	TEXT_LINE_SPACING = 10,
	TEXT_WINDOW_STATE = 15,
	TEXT_END_ALT = 30,
	TEXT_COMMANDS = 31,
	TEXT_SPACE = 32,
	TEXT_DAKUTEN = 222,
	TEXT_HANDAKUTEN = 223
};

s32 UiWork_StepChannelScript(struct UiChannelSlot *ch)
{
	struct UiRenderWork *work = UI_WORK;
	u32					 keys = gKeysHeld;
	s32					 count = UiText_SpeedSteps[gGameState.message_speed];
	s32					 lag;
	u32					 code;
	u32					 next;
	struct UiWindow		*window;
	s32					 x;
	s32					 y;
	u32					 w;
	u32					 h;
	s32					 old_w;
	s32					 old_h;
	s32					 entry;
	s32					 pc;
	s32					 gx;
	s32					 gy;
	s32					 cue;
	s32					 advance;
	s32					 width;

	if (work->menu_state != 0)
	{
		lag = gLagFramesShown;
		if (lag < 0)
			lag = 0;
		if (lag > 2)
			lag = 2;
		count = lag * 5 + 3;
	}

	if (ch->transition != 0)
	{
		UiWork_ShiftPanelRowsLeft(1);
		ch->transition = (s16)ch->transition - 1;
		return 0;
	}
	if (gKeysHeld == 0 && ch->glyph_delay != 0)
	{
		ch->glyph_delay--;
		return 0;
	}

	do
	{
		code = TEXT_END;
		if (ch->ended == 0)
			code = work->entries[ch->entry];

		if (code < TEXT_COMMANDS)
		{
			switch (code)
			{
			case TEXT_NEWLINE:
				ch->x = ch->start_x;
				if (ch->work->flags & 8)
				{
					if (ch->y > 0xcff)
					{
						UiWork_ResetChannelTransition(ch);
						count = 1;
					}
					else
					{
						ch->y += 0xd00;
					}
				}
				else
				{
					ch->y += 0xf00;
					if (ch->line <= 2)
						ch->line++;
				}
				break;

			case TEXT_PAGE:
				if (work->mode != 0 && ch->countdown < 900)
					gKeysPressedLatch = 0;
				ch->countdown = 919;
				if (UiWork_CheckCancelByModeInput(ch) == 0)
				{
					struct UiWindow *pane = ch->work;

#if EDITION_INTERNATIONAL
					if (pane->width == 0 || pane->height == 0 || work->page_arrow_shown != 0)
						break;
#else
					if (work->page_arrow_shown != 0)
						break;
#endif
					UiText_DrawGlyph(pane, 1, pane->width * 4 - 8, pane->height * 8 - 16, 1);
					work->page_arrow_shown = 1;
					break;
				}
				window = ch->work;
				x = window->x;
				y = window->y;
				old_w = window->width;
				old_h = window->height;
				entry = ch->entry;
				work->page_arrow_shown = 0;
				RenderOutput_PrepareForRedraw(window);
				if (ch->flags == 0 && (window->width | window->height) != 0)
					UiWindow_EraseBorderRect(window->x, window->y, window->width, window->height);
				entry = (entry + 1) & RENDER_ENTRY_MASK;
				if (work->entries[entry] != 0 && (window->width | window->height) != 0)
				{
					if (ch->flags != 0)
					{
						UiWindow_EraseBorderRect(
							window->x, window->y, window->width, window->height);
					}
					else
					{
						UiWindow_FitOnScreen(entry, &x, &y, &w, &h, ch->spacing, 0);
						if (window->flags & 0x80)
						{
							if (old_h != h)
								y -= h - old_h;
							if (y < 0)
								y = 0;
						}
						if (!(window->flags & 0x100))
						{
							x += (s32)(old_w - w) / 4;
							UiWindow_FitOnScreen(entry, &x, &y, &w, &h, ch->spacing, 2);
						}
						window->x = x;
						window->y = y;
						window->width = w;
						window->height = h;
					}
					UiWindow_DrawFrame(window->x, window->y, window->width, window->height);
				}
				ch->x = ch->start_x;
				ch->y = 0;
				ch->line = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
				ch->colour = 15;
				ch->outline = 0;
				ch->line_spacing = 10;
				UiWork_CopyParamsToRenderWork(ch);
#endif
				Resource_ResetEntry(work->glyph_resource);
				work->glyph_resource = 99;
				break;

			case TEXT_WAIT_KEY:
				if (work->mode != 0 && ch->countdown < 900)
					gKeysPressedLatch = 0;
				if (UiWork_CheckCancelByModeInput(ch) != 0)
					return 9;
				ch->countdown = 919;
				break;

			case TEXT_PAUSE_SHORT:
				if (ch->countdown == 0)
					ch->countdown = 20;
				work->result[1] = 0;
				UiWork_CheckCancelByInput(ch);
				break;

			case TEXT_PAUSE_LONGEST:
				if (ch->countdown == 0)
					ch->countdown = 120;
				work->result[1] = 0;
				UiWork_CheckCancelByInput(ch);
				break;

			case TEXT_PAUSE_LONG:
				if (ch->countdown == 0)
					ch->countdown = 60;
				work->result[1] = 0;
				break;

			case TEXT_COLOUR:
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
				ch->colour = work->entries[ch->entry];
				UiWork_CopyParamsToRenderWork(ch);
				break;

			case TEXT_OUTLINE:
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
				ch->outline = work->entries[ch->entry];
				UiWork_CopyParamsToRenderWork(ch);
				break;

			case TEXT_LINE_SPACING:
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
				ch->line_spacing = work->entries[ch->entry];
				UiWork_CopyParamsToRenderWork(ch);
				break;

			case TEXT_COLOURS_RESET:
				ch->colour = 15;
				ch->outline = 0;
				ch->line_spacing = 10;
				UiWork_CopyParamsToRenderWork(ch);
				break;

			case TEXT_WINDOW_STATE:
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
				ch->work->effect = work->entries[ch->entry];
				ch->countdown = 10;
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
				break;

			case TEXT_END:
			case TEXT_END_ALT:
				ch->ended = 1;
				return 8;
			}

			if (work->menu_state == 0)
				count = 1;
		}
		else
		{
			gx = (ch->x + 128) / 256;
			gy = (ch->y + 128) / 256;
			cue = UiText_SpeedCueGaps[gGameState.message_speed];
			pc = ch->entry;
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
			if (work->mode != 0)
				gx += 8;
#endif
			pc = (pc + 1) & RENDER_ENTRY_MASK;
			next = work->entries[pc];
#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
			if (next == TEXT_DAKUTEN)
			{
				code |= 0x4000;
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
			}
			else if (next == TEXT_HANDAKUTEN)
			{
				code |= 0x8000;
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
			}
#endif
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_ES)
			if (!(ch->work->flags & 8) && code > TEXT_SPACE && next > TEXT_SPACE)
			{
				s16 pair =
					UiText_Glyphs[code - TEXT_SPACE].width + UiText_Glyphs[next - TEXT_SPACE].width;

				if (work->outline == 1)
				{
					if ((u16)pair <= 14)
					{
						code |= next << 8;
						ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
					}
				}
				else if ((u16)pair <= 15)
				{
					code |= next << 8;
					ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
				}
			}
#else
			if (!(ch->work->flags & 8) && code > TEXT_SPACE && next > TEXT_SPACE
				&& (u16)(UiText_Glyphs[code - TEXT_SPACE].width
						 + UiText_Glyphs[next - TEXT_SPACE].width)
					   <= 15)
			{
				code |= next << 8;
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
			}
#endif
#endif
			advance = UiText_DrawGlyph(ch->work, code, gx, gy, 0);
			ch->glyph_delay = UiText_SpeedDelays[gGameState.message_speed];
			if (advance != 0)
			{
				if (work->result[0] != 0)
				{
					if (work->result[1] == 0)
					{
						if (code != TEXT_SPACE)
						{
							AudioCommand_PlayFar(work->result[0] + (code & 3));
							work->result[1] = cue;
						}
					}
					else
					{
						work->result[1]--;
					}
				}
				width = advance << 8;
#if EDITION_INTERNATIONAL
				if (code == TEXT_SPACE)
					width += ch->spacing[ch->line];
#else
				width += ch->spacing[ch->line];
#endif
				ch->x += width;
			}
#if !EDITION_INTERNATIONAL
			else
				return 9;
#endif
			if (code == TEXT_SPACE && work->menu_state == 0)
				count = 1;
		}

		if (ch->countdown != 0)
		{
			ch->countdown--;
			if (ch->countdown != 0)
				continue;
		}
		ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
	} while (--count != 0);

	return 0;
}

struct UiWindowInterpolationScratch
{
	s32 scaled_part;
	s32 scaled_duration;
	s32 result;
};

void UiWork_ProcessDirectWork(void)
{
	struct UiRenderWork *state = UI_WORK;
	struct UiWindow		*work = state->windows;
	s32					 index = 0;
	u8					 dirty;

loop:
	if (work->flags != 0)
	{
		if ((s16)work->frame != 0)
		{
			UiWindow_UpdateInterpolatedGeometry(work, 0);
			work->frame = (s16)work->frame - 1;
		}
		else if (work->duration != 0)
		{
			UiWork_DrawByAttributes(work);
		}
	}
	else if (work->duration != 0)
	{
		if ((s16)work->frame != work->duration)
		{
			UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
				(s16)work->previous_width, (s16)work->previous_height);
			UiWindow_UpdateInterpolatedGeometry(work, 1);
			work->frame++;
			dirty = 1;
			state->dirty = dirty;
		}
		else
		{
			UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
				(s16)work->previous_width, (s16)work->previous_height);
			work->output.head = NULL;
			work->output.tail_link = NULL;
			work->width = 0;
			work->height = 0;
			work->x = 0;
			work->y = 0;
			work->unknown_10 = 0;
			work->effect = 0;
			work->state = 0;
			work->flags = 0;
			work->frame = 0;
			work->duration = 0;
			work->previous_x = 0;
			work->previous_y = 0;
			work->previous_width = 0;
			work->previous_height = 0;
			state->dirty = 1;
		}
	}
	index++;
	work++;
	if (index != WINDOW_COUNT)
		goto loop;
}

void UiWindow_UpdateInterpolatedGeometry(void *arg, s32 save_position)
{
	struct UiWindow					   *window = arg;
	struct UiWindowInterpolationScratch scratch;
	s32									frame;
	s32									duration;
	s32									remaining;
	s32									x;
	s32									y;
	s32									width;
	s32									height;

	frame = (s16)window->frame;
	duration = window->duration;
	remaining = duration - frame;
	scratch.scaled_part = (frame * window->width) << 16;
	scratch.scaled_duration = duration << 17;
	scratch.result = Iwram_RatioMulQ14(scratch.scaled_duration, scratch.scaled_part);
	x = (scratch.result >> 16) + window->x;

	scratch.scaled_part = (remaining * window->width) << 16;
	scratch.result = Iwram_RatioMulQ14(scratch.scaled_duration, scratch.scaled_part);
	width = scratch.result >> 15;

	scratch.scaled_part = (frame * window->height) << 16;
	scratch.scaled_duration = window->duration << 17;
	scratch.result = Iwram_RatioMulQ14(scratch.scaled_duration, scratch.scaled_part);
	y = (scratch.result >> 16) + window->y;

	scratch.scaled_part = (remaining * window->height) << 16;
	scratch.result = Iwram_RatioMulQ14(scratch.scaled_duration, scratch.scaled_part);
	height = scratch.result >> 15;

	UiWindow_DrawFrame(x, y, width, height);
	if (save_position != 0)
	{
		window->previous_x = x;
		window->previous_y = y;
		window->previous_width = width;
		window->previous_height = height;
	}
}

//--------------------------------------------------------------------------
// fill_halfwords_dma.c
// メモリ：DMA で 16 ビットずつ埋める
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// dst から count 個を value で埋め、埋め終えた次の位置を返す
//--------------------------------------------------------------------------
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count)
{
	volatile u16 fill;
	if (count > 0)
	{
		fill = value;
		Dma_Set(&fill, dst, count | (DMA_ENABLE32 | DMA_SRC_FIXED32), REG_DMA3);
		dst += count;
	}
	return dst;
}

void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height)
{
	struct UiRenderWork *canvas = UI_WORK;
	u16					*cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
	u32					 row;

	if (width <= 1 || height <= 1 || width > 30 || height > 30)
		return;
	UiWindow_ClearTileAttributesInRect(x, y, width, height);
	if (canvas->mode != 0)
		*cursor++ = 0xf01c;
	else
		*cursor++ = 0xf010;
	cursor = Memory_FillHalfwordsDma(cursor, 0xf011f011, width - 2);
	if (canvas->mode != 0)
		*cursor++ = 0xf41c;
	else
		*cursor++ = 0xf012;
	cursor += 32 - width;
	for (row = 1; row < height - 1; row++)
	{
		*cursor++ = 0xf016;
		if (width != 2)
			cursor = Memory_FillHalfwordsDma(cursor, 0xf020f020, width - 2);
		*cursor++ = 0xf017;
		cursor += 32 - width;
	}
	if (canvas->mode != 0)
		*cursor++ = 0xf81c;
	else
		*cursor++ = 0xf013;
	cursor = Memory_FillHalfwordsDma(cursor, 0xf014f014, width - 2);
	if (canvas->mode != 0)
		*cursor = 0xfc1c;
	else
		*cursor = 0xf015;
	canvas->dirty = 1;
}

void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width)
{
	struct UiRenderWork *canvas = UI_WORK;
	u16					*cursor = (u16 *)((y * 32 + x) * 2 + (u32)canvas->tilemap);
	u32					 row;
	u32					 col;

	if (width <= 1 || height <= 1 || width > 30 || height > 30)
		return;
	cursor += 32;
	if (full_width == 0)
	{
		for (row = 1; row < height - 1; row++)
		{
			cursor++;
			for (col = 1; col < width - 1; col++)
				*cursor++ = ((0x127 + row + (col - 1) * (height - 2)) & 0xfff) | 0xf000;
			cursor++;
			cursor += 32 - width;
		}
	}
	else
	{
		for (row = 1; row < height - 1; row++)
		{
			for (col = 0; col < width; col++)
				*cursor++ = ((0x127 + row + col * (height - 2)) & 0xfff) | 0xf000;
			cursor += 32 - width;
		}
	}
	canvas->dirty = 1;
}

//--------------------------------------------------------------------------
// is_complete.c
// 表示：メッセージのチャンネルがすべて書き終えたか
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 3 つのチャンネルのうち、ウィンドウがあって書き終えていない（state が 0）
// ものがあれば 0、なければ 1 を返す
//--------------------------------------------------------------------------
s32 UiWork_IsComplete(void)
{
	struct UiChannelSlot *ch;
	s32					  i;

	ch = UI_WORK->channels;
	for (i = 0; i != 3; i++, ch++)
	{
		if (ch->work != NULL && ch->work->state == 0)
			return 0;
	}
	return 1;
}

s32 UiWork_IsIdle(struct UiWindow *window)
{
#if !EDITION_INTERNATIONAL
	if (window == NULL)
		return 1;
#endif
	if (window->flags == 0 && window->duration == 0)
		return 1;
	return 0;
}

void UiWork_ResetCounters(void)
{
	struct UiRenderWork *state = UI_WORK;

	state->colour = 15;
	state->line_spacing = 10;
	state->unknown_before_count = 9;
	state->outline = 0;
	state->unknown_after_line_spacing = 1;
}

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
	struct UiRenderWork *state = UI_WORK;
	s32					 size;

	state->glyph_tiles = VramBlock_LoadCached(95, 128 << 6, 0);
	state->unknown_before_count = 9;
	state->line_spacing = 10;
	state->outline = 0;
	state->colour = 15;
	state->count = 0;
	size = 200;
	size <<= 4;
	Scheduler_AddOrUpdateCallback((void *)UiWork_ProcessAll, size);
}

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
	struct UiRenderWork *state = UI_WORK;
	s32					 size;

	if (initialize != 0)
		state->glyph_tiles = VramBlock_LoadCached(95, 128 << 6, 0);
	state->unknown_before_count = 9;
	state->line_spacing = 10;
	state->outline = 0;
	state->colour = 15;
	state->count = 0;
	size = 200;
	size <<= 4;
	Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, size);
}

void UiWork_FinalizeSharedSlot(void)
{
	struct BattleDisplayWork *display = gBattleDisplayWork;
	struct UiWindow			 *window = display->window;

	if (window != NULL)
	{
		UiWork_Finalize(window, 1);
		display->window = NULL;
	}
}

enum
{
	HEAP_SLOT_BATTLE_DISPLAY = 37
};

void UiText_PrepareMessageWork(s32 argument)
{
	s32						  index;
	struct UiChannelSlot	 *result;
	struct UiWindow			 *existing;
	struct UiWindow			 *work;
	struct UiRenderWork		 *state;
	struct BattleDisplayWork *control;

	state = UI_WORK;
	control = (struct BattleDisplayWork *)gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW];
	// result starts as NULL, though it is assigned again before it is read; the same zero clears marked below
	result = NULL;
	state->menu_state = 2;
	index = UiText_BuildRenderEntries(argument, 1);
	state->menu_state = 1;

	if (state->entries[index] != 0)
	{
		existing = control->window;
		if (existing == NULL)
		{
			work = UiWindow_Create(0, 15, 30, 6, 10);
			control->window = work;
			UiWindow_MapTextCanvasTiles(0, 15, 30, 6, 1);
			control->marked = 0;
		}
		else
		{
			work = existing;
		}
		if (work != NULL)
		{
			result = UiWork_ActivateChannel(work, index, control->marked);
			control->offset = (struct BattleDisplayOffset *)result;
			control->marked = 0;
			if (result == NULL)
			{
				UiWork_Finalize(work, 1);
			}
		}
	}
}

void UiText_ShowMessageAndWaitCore(s32 argument)
{
	UiText_PrepareMessageWork(argument);
	while (UiWork_IsComplete() == 0)
		WaitFrames(1);
	WaitFrames(1);
}

s32 UiText_OpenEntryMessage(s32 window, s32 argument)
{
	struct UiRenderWork *work = UI_WORK;
	s32					 entry;
	s32					 result = 0;
	/* FAKEMATCH: needed to match: four unread render entry slots, the native frame's eight extra bytes. */
	u16 entries[4];

	work->result[0] = 0;
	work->result[1] = 0;
	entry = UiText_BuildRenderEntries(argument, 1);
	if (work->entries[entry] == 0)
		return 0;
	if (window == 0)
		return 0;
	result = (s32)UiText_QueueRenderEntries((struct UiWindow *)window, entry, 0, 0, 0, 1);
	if (result == 0)
		return 0;
	return result;
}

void UiWork_SetBusyFlags(s32 flags)
{
	struct UiRenderWork *work;

	work = UI_WORK;
	if (work != NULL)
	{
		if (flags & 1)
		{
			work->spacing_before = 1;
		}
		if (2 & flags)
		{
			work->spacing_after = 1;
		}
	}
}

struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed)
{
	s32					 entry;
	u32					 height;
	u32					 width;
	u16					 bounds[4];
	s32					 flags;
	u16					*out;
	struct UiWindow		*window;
	struct UiRenderWork *work;

	work = UI_WORK;
	work->result[0] = (packed << 4) >> 20;
	// the window starts as NULL, though it is assigned before it is read; the same zero serves the zeros below
	window = NULL;
	work->result[1] = 0;
	packed &= 0xffff;
	flags = 0;
	entry = UiText_BuildRenderEntries(message, 1);
	if (work->entries[entry] == 0)
		return NULL;
	UiWindow_FitOnScreen(entry, &x, &y, &width, &height, (out = bounds), 0);
	if (width == 0 && height == 0)
		return NULL;
	if (!(packed & 1))
		flags |= 2;
	if (packed & 8)
		flags |= 8;
	if (packed & 16)
		flags |= 128;
	if (packed & 32)
		flags |= 256;
	window = UiWindow_Create(x, y, width, height, flags);
	if (window == NULL)
		return NULL;
	if (UiText_QueueRenderEntries(window, entry, 0, 0, out, 0) == 0)
	{
		UiWork_Finalize(window, 1);
		return NULL;
	}
	work->spacing_before = 0;
	work->spacing_after = 0;
	return window;
}

void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags)
{
	struct UiRenderWork *state = UI_WORK;
	s32					 release;
	s32					 pos[2] = {0, 0};
	s32					 height;
	s32					 width;
	s32					 y;
	s32					 x;
	struct UiWindow		*work = NULL;
	s32					 zero;

	y = x = 0;
	release = flags & 1;

	if (flags & 2)
	{
		state->busy = 1;
	}

	UiText_GetResourceDimensions(no, &x, &y, (u32 *)&width, (u32 *)&height);

	x = (30 - width) >> 1;
	y = (12 - height) >> 1;

	if (flags & 8)
	{
		y += 4;
	}
	else if (flags & 0x40)
	{
		y += 12;
	}
	else
	{
		s32 dy;

		Object_GetScreenPositionFar(gGameState.selected_actor, pos);
		dy = pos[1] >> 3;
		if (dy > 9)
		{
			y = dy - 5;
		}
		else
		{
			y = dy + 4;
		}
	}

	work = UiText_OpenMessageWindow(no, x, y, release);

	if (work != NULL)
	{
		while (UiWork_IsComplete() == 0)
		{
			WaitFrames(1);
		}

		if (flags & 0x20)
		{
			UI_WORK->menu_busy = 1;
		}

		if (!(flags & 4))
		{
			UiWork_Finalize(work, release);
			while (UiWork_IsIdle(work) == 0)
			{
				WaitFrames(1);
			}
		}
	}

	zero = 0;
	state->busy = zero;
	state->result[0] = zero;
	state->result[1] = zero;
	WaitFrames(3);
}

void UiWork_ProcessAll(void)
{
	UiWork_ProcessDirectWork();
	UiWork_ProcessRenderChannels();
	UiWork_AnimateSpriteSlots();
}

#if EDITION_INTERNATIONAL
s32 UiText_RenderGlyphPair(s32 code, u32 *out)
{
	u8					 buf[384];
	struct UiRenderWork *work;
	const u8			*input;
	s32					 length;
	s32					 colour;
	s32					 transparent;
	s32					 first_index;
	s32					 second_index;
	u16					 second_width;
	u8					*digits;
	s32					 group;
	s32					 row;
	s32					 column;
	s32					 nibble;
	u32					 value;

	work = UI_WORK;
	second_index = (code << 8) >> 16;
	code &= 255;
	Iwram_ClearWords(buf, 384);
	if (work->mode != 0)
	{
		transparent = 0;
		colour = 8;
	}
	else
	{
		transparent = 1;
		colour = work->colour;
	}
	first_index = code - 32;
	input = first_index * sizeof(struct FontGlyph) + (const u8 *)UiText_Glyphs;
	length = ((const struct FontGlyph *)input)->width;
	input = (const u8 *)((const struct FontGlyph *)input)->rows;
	if (work->outline == 1)
	{
		Iwram_ExpandBitRuns(input, buf + 49, transparent);
		Iwram_ExpandBitRuns(input, buf + 50, transparent);
		Iwram_ExpandBitRuns(input, buf + 32, colour);
		Iwram_ExpandBitRuns(input, buf + 33, colour);
		length++;
	}
	else
	{
		Iwram_ExpandBitRuns(input, buf + 49, transparent);
		Iwram_ExpandBitRuns(input, buf + 32, colour);
	}
	if ((u16)second_index != 0)
	{
		input = (const u8 *)&UiText_Glyphs[-32] + ((u16)second_index << 5);
		second_width = *(const u16 *)input;
		input += 2;
		if (work->outline == 1)
		{
			u8 *dst = buf + length;
			Iwram_ExpandBitRuns(input, dst + 49, transparent);
			Iwram_ExpandBitRuns(input, dst + 50, transparent);
			Iwram_ExpandBitRuns(input, dst + 32, colour);
			Iwram_ExpandBitRuns(input, dst + 33, colour);
			second_width += 1;
		}
		else
		{
			u8 *dst = buf + length;
			u8 *p;
			p = dst + 49;
			Iwram_ExpandBitRuns(input, p, transparent);
			p = dst + 32;
			Iwram_ExpandBitRuns(input, p, colour);
		}
		length += second_width;
	}

	digits = buf + 7;
	group = 0;
	do
	{
		row = 0;
		do
		{
			column = 0;
			do
			{
				value = 0;
				nibble = 7;
				do
				{
					value = (value << 4) + *digits--;
					nibble--;
				} while (nibble >= 0);
				*out++ = value;
				column++;
				digits += 24;
			} while (column <= 7);
			row++;
			digits -= 120;
		} while (row <= 1);
		group++;
		digits += 112;
	} while (group <= 1);
	return length;
}
#else
s32 UiText_RenderGlyphPair(u32 code, u32 *out)
{
	u8					 buf[256];
	struct UiRenderWork *work;
	const u8			*input;
	const u8			*mark;
	s32					 length;
	s32					 colour;
	s32					 transparent;
	u32					 mark_code;
	u8					*digits;
	s32					 group;
	s32					 row;
	s32					 column;
	s32					 nibble;
	u32					 value;

	/* FAKEMATCH: needed to match: the empty loop ends a scheduling region, keeping the spill of out ahead of the work-address load. */
	do
	{
	} while (0);
	work = UI_WORK;
	mark = 0;
	mark_code = UiText_MarkGlyphCodes[code >> 14];
	if (mark_code != 0)
		mark =
			(const u8 *)((const struct FontGlyph *)((const u8 *)UiText_Glyphs
													+ (mark_code - 32) * sizeof(struct FontGlyph)))
				->rows;
	code &= 0x3fff;
	if (code <= 0xff)
	{
		input = (const u8 *)UiText_Glyphs + (code - 32) * sizeof(struct FontGlyph);
		length = ((const struct FontGlyph *)input)->width;
		input = (const u8 *)((const struct FontGlyph *)input)->rows;
	}
	else
	{
		input = UiText_KanjiGlyphs + (code - 256) * 24;
		length = 10;
	}
	Iwram_ClearWords(buf, 256);
	if (work->mode != 0)
	{
		transparent = 0;
		colour = 8;
	}
	else
	{
		transparent = 1;
		colour = work->colour;
	}
	if (work->outline == 1)
	{
		Iwram_ExpandBitRuns(input, buf + 49, transparent);
		Iwram_ExpandBitRuns(input, buf + 50, transparent);
		if (mark != 0)
		{
			Iwram_ExpandBitRuns(mark, buf + 24, transparent);
			Iwram_ExpandBitRuns(mark, buf + 25, transparent);
		}
		Iwram_ExpandBitRuns(input, buf + 32, colour);
		Iwram_ExpandBitRuns(input, buf + 33, colour);
		if (mark != 0)
		{
			/* FAKEMATCH: needed to match: assigning digits in the mark argument makes the argument register compute buf + 7 before its loop copy. */
			Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
			Iwram_ExpandBitRuns(mark, buf + 8, colour);
		}
		length++;
	}
	else
	{
		Iwram_ExpandBitRuns(input, buf + 49, transparent);
		if (mark != 0)
			Iwram_ExpandBitRuns(mark, buf + 24, transparent);
		Iwram_ExpandBitRuns(input, buf + 32, colour);
		if (mark != 0)
		{
			/* FAKEMATCH: needed to match: this mark path needs the same argument-before-loop order as the outlined path. */
			Iwram_ExpandBitRuns(mark, digits = buf + 7, colour);
		}
	}

	digits = buf + 7;
	group = 0;
	do
	{
		row = 0;
		do
		{
			column = 0;
			do
			{
				value = 0;
				nibble = 7;
				do
				{
					value = (value << 4) + *digits--;
					nibble--;
				} while (nibble >= 0);
				*out++ = value;
				column++;
				digits += 24;
			} while (column <= 7);
			row++;
			digits -= 120;
		} while (row <= 1);
		group++;
		digits += 112;
	} while (group <= 1);
	return length;
}
#endif

//--------------------------------------------------------------------------
// get_wide_string_width.c
// 文字：16 ビット文字列の表示幅（ドット）を求める
//--------------------------------------------------------------------------
// 独・西・仏・伊版は 0xff 以下の文字をすべて 10 ドット、それより上を 0 で数える
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)    \
	|| defined(TBS_EDITION_IT) || defined(TLA_EDITION_DE) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define WIDE_WIDTH_FIXED 1
#endif

// フォントの n 番目の文字
#define FONT_GLYPH(n) \
	((const struct FontGlyph *)((n) * sizeof(struct FontGlyph) + (const u8 *)UiText_Glyphs))

//--------------------------------------------------------------------------
// 空白は 4 ドット、0xff 以下の文字はフォントの幅、それより上は 10 ドット
// 0xde・0xdf（濁点・半濁点）は幅に数えない
//--------------------------------------------------------------------------
s32 UiText_GetWideStringWidth(u16 *text)
{
	s32	 width;
	u16 *p;
	u32	 c;
#if !defined(WIDE_WIDTH_FIXED)
	u32 idx;
#endif

	c = *text;
	width = 0;
	p = text + 1;
	if (c != 0)
	{
		do
		{
			if (c == 0x20)
			{
				width += 4;
			}
			else if (c <= 0xffU)
			{
#if defined(WIDE_WIDTH_FIXED)
				width += 10;
#else
				idx = c - 0xde;
				if (idx > 1U)
				{
					idx += 0xbe; // フォントは 0x20 から
					width += FONT_GLYPH(idx)->width;
				}
			}
			else
			{
				width += 10;
#endif
			}
			c = *p;
			p += 1;
		} while (c != 0);
	}
	return width;
}

void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x, s32 y)
{
	u8					*base;
	struct UiRenderWork *work;
	u32					 c;
	u32					 next;
	s16					 start;

	base = gWindowWork[0];
	work = (struct UiRenderWork *)base;
	c = 0;
	start = x;
	if (text == NULL)
	{
		text = work->entries;
		work->entries[work->count] = c;
		work->count = (work->count + 1) & RENDER_ENTRY_MASK;
	}
#if EDITION_INTERNATIONAL
	for (;;)
#else
	while ((c = *text++) != 0)
#endif
	{
#if EDITION_INTERNATIONAL
		c = *text++;
		if (c > 0xff)
			c = 0x40;
		if (c == 0)
			break;
#endif
		if (c <= 30)
		{
			switch (c)
			{
			case 8:
				work->colour = *text;
				text++;
				break;
			case 9:
				work->outline = *text;
				text++;
				break;
			case 10:
				work->line_spacing = *text;
				text++;
				break;
			case 7:
				UiWork_ResetCounters();
				break;
			case 3:
				x = start;
				y += 15;
				break;
			case 14:
			case 15:
			case 28:
				text++;
			case 11:
			case 12:
			case 17:
#if EDITION_INTERNATIONAL
			case 29:
#endif
				text++;
				break;
			}
		}
		else
		{
#if EDITION_INTERNATIONAL
			if ((window->flags & 8) == 0)
			{
				next = *text;
#if defined(TBS_EDITION_ES)
				if (c > 32 && next > 32)
				{
					s16 width = UiText_Glyphs[c - 32].width + UiText_Glyphs[next - 32].width;

					if (work->outline == 1)
					{
						if ((u16)width <= 14)
						{
							c |= next << 8;
							text++;
						}
					}
					else if ((u16)width <= 15)
					{
						c |= next << 8;
						text++;
					}
				}
#else
				if (c > 32 && next > 32
					&& (u16)(UiText_Glyphs[c - 32].width + UiText_Glyphs[next - 32].width) <= 15)
				{
					c |= next << 8;
					text++;
				}
#endif
			}
#else
			if (*text == 0xde)
			{
				c |= 0x4000;
				text++;
			}
			else if (*text == 0xdf)
			{
				c |= 0x8000;
				text++;
			}
#endif
			x += UiText_DrawGlyph(window, c, x, y, 0);
		}
	}
}

void UiText_RenderGlyphTileAtWorkOffset(
	u16 *buffer, const struct RenderInput *work, s32 offset_x, s32 offset_y)
{
	struct UiRenderWork *canvas = UI_WORK;
	u32					 cell;

	if (buffer == NULL)
	{
		u16 *counter = &canvas->count;

		buffer = canvas->entries;
		canvas->entries[*counter] = 0;
		*counter = (*counter + 1) & RENDER_ENTRY_MASK;
	}

	cell = ((work->y + offset_y + 1) << 5) + (work->x + offset_x) + 1;
	if (cell < 0x280)
	{
		s32 src;
		s32 dst;

		cell *= 2;
		dst = 0x06002000 + cell;
		src = (s32)canvas->tilemap + cell;

		UiText_RenderStringTiles(buffer, (u16 *)src, (u16 *)dst, 0x06002000);
	}
}

void UiText_RenderWideStringInWindow(s16 *text, struct UiWindow *window, s32 x, s32 y)
{
	struct UiRenderWork *work;
	u16					 c;
	s16					 start;

	work = UI_WORK;
	start = x;
	if (text == NULL)
	{
		text = work->entries;
		work->entries[work->count] = 0;
		work->count = (work->count + 1) & RENDER_ENTRY_MASK;
	}
	c = *text++;
	if (c != 0)
		do
		{
			if (c <= 30)
			{
				switch (c)
				{
				case 3:
					x = start;
					y++;
					break;
				case 14:
				case 15:
				case 28:
					text++;
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 17:
#if EDITION_INTERNATIONAL
				case 29:
#endif
					text++;
					break;
				}
			}
			else
			{
				UiWindow_PutGlyph(window, c, x, y, 0);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
				x++;
#else
				if ((u16)(c - 0xde) > 1)
					x++;
#endif
			}
			c = *text++;
		} while (c != 0);
	work->dirty = 1;
}

u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width)
{
	s32 offset;
	s32 value;
	s32 negative;
	s32 space;
	s32 minus;
	u8 *leading;
	u8 *trim;

	value = input;
	negative = 0;
	if (value < 0)
	{
		if (width == 0)
			negative = 1;
		value = 0U - value;
	}

	buffer[0] = ' ';
	for (offset = 12; offset != 0; offset--)
	{
		buffer[offset] = value % 10 + '0';
		value = value / 10;
	}

	offset = 0;
	buffer[13] = offset;
	space = ' ';
	offset = 1;
	minus = '-';
	for (leading = buffer; offset != 13; leading++, offset++)
	{
		if (leading[1] == '0')
		{
			if (offset != 12)
				leading[1] = space;
		}
		else
		{
			if (negative != 0)
				leading[0] = minus;
			break;
		}
	}

	if (width == 0)
	{
		offset = 0;
		if (buffer[0] == ' ')
		{
			trim = buffer;
			do
			{
				offset++;
				if (offset == 12)
					break;
				trim++;
			} while (*trim == ' ');
		}
		return buffer + offset;
	}

	if (width < 0 || width > 12)
		width = 12;
	return buffer + 13 - width;
}

#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_IT)
#define ARTICLE_KINDS 11
#define ARTICLE_KIND_MASK 15
#elif defined(TBS_EDITION_ES)
#define ARTICLE_KINDS 5
#define ARTICLE_KIND_MASK 15
#else
#define ARTICLE_KINDS 8
#define ARTICLE_KIND_MASK 7
#endif

struct ArticleTable
{
	s8 *text[ARTICLE_KINDS];
};

#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
u32 UiText_AppendArticleName(
	s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural, s32 *suffix)
{
	struct ArticleTable tbl;
#if defined(TBS_EDITION_ES)
	struct ArticleTable indefinite_tbl;
	u16					count;
	s16					indefinite;
#endif
	s32 kind;
	u16 head;
	u16 c;
	s8 *p;
	s8	c8;
	s32 cnt;

#if defined(TBS_EDITION_ES)
	count = no & 7;
	indefinite = no & 8;
#endif
	if (mode != 0)
	{
		entry[pos] = ' ';
		pos = (pos + 1) & 0x1ff;
		entry[pos] = 0x0a;
		pos = (pos + 1) & 0x1ff;
		entry[pos] = 0x0a;
		pos = (pos + 1) & 0x1ff;
	}
#if defined(TBS_EDITION_ES)
	if (count == 0 || count == 4)
	{
#else
	if (no == 0 || no == 4)
	{
#endif
		head = name[0];
		if (head == 29)
			name += 2;
	}
	else
	{
		kind = 0;
		tbl = UiText_ArticleTable;
#if defined(TBS_EDITION_ES)
		indefinite_tbl = UiText_IndefiniteArticles;
#endif
		if (name[0] == 29)
		{
			kind = name[1];
			name += 2;
		}
#if defined(TBS_EDITION_ES)
		if (indefinite == 0)
			p = tbl.text[kind & ARTICLE_KIND_MASK];
		else
			p = indefinite_tbl.text[kind & ARTICLE_KIND_MASK];
#else
		p = tbl.text[kind & ARTICLE_KIND_MASK];
#endif
		for (cnt = 0; cnt < 8; cnt++)
		{
			if ((c8 = *p++) == 0)
				break;
			if (cnt == 0 && *suffix == 2)
				c8 -= 0x20;
			entry[pos] = c8;
			pos = (pos + 1) & 0x1ff;
		}
	}
	while (*name != 0)
	{
		c = *name++;
		entry[pos] = c;
		pos = (pos + 1) & 0x1ff;
#if defined(TBS_EDITION_DE)
		if (c == 's' || c == 'S' || c == 'z' || c == 'Z')
			*suffix = 1;
		else
			*suffix = 0;
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
		if (c == 's' || c == 'S')
			*suffix = 1;
		else
			*suffix = 0;
#endif
	}
	if (mode != 0)
	{
		entry[pos] = 0x0a;
		pos = (pos + 1) & 0x1ff;
		entry[pos] = 0x08;
		pos = (pos + 1) & 0x1ff;
		entry[pos] = ' ';
		pos = (pos + 1) & 0x1ff;
	}
	return pos;
}
#endif

#if defined(TBS_EDITION_EN)
u32 UiText_AppendArticleName(
	s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural, s32 *suffix)
{
	struct ArticleTable tbl;
	s32					kind;
	u16					head;
	u16					c;
	u8				   *p;
	s8					c8;
	s32					cnt;

	if (mode != 0)
	{
		entry[pos] = ' ';
		pos = (pos + 1) & 0x1ff;
		entry[pos] = 0x0a;
		pos = (pos + 1) & 0x1ff;
		entry[pos] = 0x0a;
		pos = (pos + 1) & 0x1ff;
	}
	if (no == 1 || (no == 3 && plural == 0))
	{
		kind = 0;
		tbl = UiText_ArticleTable;
		head = name[0];
		if (head == 29)
		{
			kind = name[1] - 1;
			name += 2;
		}
		if (kind == 0)
		{
			if (head == 'A' || head == 'I' || head == 'U' || head == 'E' || head == 'O')
				kind = 2;
			else
				kind = 1;
		}
		p = tbl.text[kind & 7];
		for (cnt = 0; cnt < 8; cnt++)
		{
			if ((c8 = *p++) == 0)
				break;
			entry[pos] = c8;
			pos = (pos + 1) & 0x1ff;
		}
	}
	else
	{
		head = name[0];
		if (head == 29)
			name += 2;
	}
	while (*name != 0)
	{
		c = *name++;
		entry[pos] = c;
		pos = (pos + 1) & 0x1ff;
		if (c == 'S' || c == 's')
			*suffix = 1;
		else
			*suffix = 0;
	}
	if (no == 2 || (no == 3 && plural != 0))
	{
		if (*suffix != 0)
		{
			entry[pos] = 'e';
			pos = (pos + 1) & 0x1ff;
		}
		entry[pos] = 's';
		pos = (pos + 1) & 0x1ff;
	}
	if (mode != 0)
	{
		entry[pos] = 0x0a;
		pos = (pos + 1) & 0x1ff;
		entry[pos] = 0x08;
		pos = (pos + 1) & 0x1ff;
		entry[pos] = ' ';
		pos = (pos + 1) & 0x1ff;
	}
	return pos;
}
#endif

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) \
	|| defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)

#define ENTRY_NARROW_GAP 5
#define ENTRY_WIDE_GAP 222

#define TEXT_WORK_BLOCK 50

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#if defined(TBS_EDITION_FR)
extern s8 UiText_ItemPronoun[];
#else
struct ItemPossessiveTable
{
#if defined(TBS_EDITION_ES)
	s8 *text[2];
#else
	s8 *text[4];
#endif
};
extern const struct ItemPossessiveTable UiText_ItemPossessives;
extern const s8							UiText_ItemGenders[];
extern u8							   *UiText_NumberWords[];
#endif
#endif

s32 UiText_BuildRenderEntries(s32 script, s32 clear)
{
	struct UiRenderWork *work = UI_WORK;
	u32					 ch = 0;
#if defined(TBS_EDITION_EN)
	s32 head = 1;
#endif
	s32 quote = 0;
	u32 start = work->count;
	u32 cnt = 0;
	s32 plural = 0;
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_IT)
	s32 running = 1;
#endif
#if defined(TBS_EDITION_EN)
	u32 pos = start;
	u32 prev;
	s32 suffix = 0;
#else
#if defined(TBS_EDITION_ES)
	struct ItemPossessiveTable possessives;
	s32						   form;
#endif
	s32 suffix = 2;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
	s32 running = 1;
#endif
	s8 *word;
	s8	c8;
	u32 pos = start;
#endif
	u16 *entry = work->entries;
	s32	 overflow = 0;
	s32 (*decode)(struct TextReader *);
	u32 next;
	s32 no;
	u32 j;
	u32 i;
	s32 value;
	s32 tmp;
#if defined(TBS_EDITION_IT)
	s32 ending;
#endif
	s32				  mag;
	u8				 *num;
	u8				 *src;
	u16				 *dst;
	u16				 *p;
	u16				  name[24];
	u8				  numbuf[16];
	struct TextReader st;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
	struct ItemPossessiveTable possessives;
	s32						   kind;
#endif

	if (script == -1)
	{
		start = work->message_start;
	}
	else
	{
		RamCode_LoadSlot(50, Text_DecodeSymbol, Text_DecodeSymbolCodeSize);
		decode = (s32 (*)(struct TextReader *))gWindowWork[TEXT_WORK_BLOCK - HEAP_SLOT_WINDOW];
		UiText_LookupMessage(&st, script);

		do
		{
#if defined(TBS_EDITION_EN)
			prev = ch;
#endif
			ch = (u32)decode(&st);
			if (ch > 255)
				ch = 64;

			if (overflow != 0)
			{
				if (ch < 32)
				{
					switch (ch)
					{
					case 19:
						decode(&st);
						UiRender_LookupNamedValue(3, clear);
						break;
					case 0:
					case 2:
					case 30:
						running = 0;
						break;
					case 22:
						UiRender_LookupNamedValue(5, clear);
						break;
					case 20:
						decode(&st);
						UiRender_LookupNamedValue(2, clear);
						break;
					case 21:
						UiRender_LookupNamedValue(4, clear);
						break;
					case 23:
						UiRender_LookupNamedValue(6, clear);
						break;
					case 8:
					case 9:
						decode(&st);
						break;
					case 17:
						decode(&st);
						break;
					case 18:
						decode(&st);
						break;
					case 29:
						decode(&st);
						break;
					case 1:
						running = 0;
						ch = 2;
						break;
					case 16:
						break;
					case -1:
						break;
					default:
						break;
					}
				}
			}
			else
			{
				if (work->spacing_before != 0
#if defined(TBS_EDITION_EN)
					&& head == 0 && ch != 222 && ch != 223
#endif
				)
				{
					entry[pos] = ENTRY_NARROW_GAP;
					pos = (pos + 1) & RENDER_ENTRY_MASK;
				}

#if defined(TBS_EDITION_EN)
				if (work->spacing_after != 0 && head == 0 && ch != 222 && ch != 223 && prev <= 256
					&& prev > 127 && prev != 222 && prev != 223 && prev != 32 && prev != 165
					&& prev != 161 && prev != 164)
				{
					entry[pos] = ENTRY_WIDE_GAP;
					pos = (pos + 1) & RENDER_ENTRY_MASK;
				}

#endif
				if (ch > 31)
				{
					if (work->spacing_before != 0 && (ch == 32 || cnt > 10))
					{
						entry[pos] = '.';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = '.';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = '.';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						overflow = 1;
						if (cnt > 10)
							ch = 32;
					}
					if (ch == 34)
					{
						quote ^= 1;
						if (quote != 0)
							ch = 142;
					}
					entry[pos] = (u16)ch;
					pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_EN)
					head = 0;
#else
					if (ch != ' '
#if defined(TBS_EDITION_ES)
						&& ch != 161 && ch != 191
#endif
					)
					{
						suffix = 0;
						if (ch == '.' || ch == '!' || ch == '?')
							suffix = 2;
						if (ch == 's' || ch == 'S'
#if defined(TBS_EDITION_DE)
							|| ch == 'z' || ch == 'Z'
#endif
						)
							suffix = 1;
					}
#endif
				}
				else
				{
					switch (ch)
					{
					case 0:
					case 2:
					case 30:
						running = 0;
						break;
					case 8:
					case 9:
					case 29:
						entry[pos] = (u16)ch;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = decode(&st) + 0xFFFFU;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						break;
					case 22:
						value = (s32)UiRender_LookupNamedValue(5, clear);
						mag = value;
						if (value < 0)
							mag = -value;
						plural = 1;
						if (mag <= 1)
							plural = 0;
						num = UiText_FormatNumber(numbuf, value, 0);
						i = num - numbuf;
						while (i != 16 && numbuf[i] != 0)
						{
							entry[pos] = numbuf[i];
							pos = (pos + 1) & RENDER_ENTRY_MASK;
							i++;
						}
						break;
					case 19:
						no = decode(&st) - 1;
						tmp = UiRender_LookupNamedValue(3, clear);
						UiText_DecodeMessage(tmp + (s32)&MsgClassName, name, 24);
						pos = UiText_AppendArticleName(0, name, pos, entry, no, plural, &suffix);
						break;
					case 20:
						no = decode(&st) - 1;
#if defined(TBS_EDITION_ES)
						form = no & 7;
#endif
						tmp = UiRender_LookupNamedValue(2, clear);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
						tmp &= RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_ES)
						if (form == 2 || (form == 3 && plural != 0) || form == 4)
#else
						if (no == 2 || (no == 3 && plural != 0) || no == 4)
#endif
							UiText_DecodeMessage(tmp + (s32)MsgItemPluralName, name, 24);
						else
							UiText_DecodeMessage(tmp + (s32)&MsgItemName, name, 24);
#else
						UiText_DecodeMessage(
							(tmp & RENDER_ENTRY_MASK) + (s32)&MsgItemName, name, 24);
#endif
						pos = UiText_AppendArticleName(0, name, pos, entry, no, plural, &suffix);
						break;
					case 21:
						tmp = UiRender_LookupNamedValue(4, clear);
						UiText_DecodeMessage(tmp + (s32)&MsgAbilityName, name, 24);
						p = name;
						next = pos;
						while (*p != 0)
						{
							entry[next] = *p;
							p++;
							next = (next + 1) & RENDER_ENTRY_MASK;
						}
						pos = next;
						break;
					case 23:
						UiText_DecodeMessage(
							Scene_GetAreaNameIndexFar(UiRender_LookupNamedValue(6, clear), 1)
								+ (s32)&MsgDebugEntryName,
							name, 24);
						p = name;
						next = pos;
						while (*p != 0)
						{
							entry[next] = *p;
							p++;
							next = (next + 1) & RENDER_ENTRY_MASK;
						}
						pos = next;
						break;
					case 16:
						src = UnitGetFar(gGameState.selected_actor)->name;
						dst = name;
						i = 0;
						do
						{
							*dst = *src;
							i++;
							src++;
							dst++;
						} while (i <= 14);
						pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0, &suffix);
						break;
					case 18:
						no = decode(&st) - 1;
						src = UnitGetFar(UiRender_LookupNamedValue(1, clear))->name;
						dst = name;
						j = 0;
						do
						{
							*dst = *src;
							j++;
							src++;
							dst++;
						} while (j <= 14);
						pos = UiText_AppendArticleName(0, name, pos, entry, no, plural, &suffix);
						break;
					case 17:
						tmp = decode(&st) - 1;
						src = UnitGetFar(tmp)->name;
						dst = name;
						j = 0;
						do
						{
							*dst = *src;
							j++;
							src++;
							dst++;
						} while (j <= 14);
						pos = UiText_AppendArticleName(0, name, pos, entry, 0, 0, &suffix);
						break;
					case 26:
						tmp = ((decode(&st) - 1) << 1) + 128;
						entry[pos] = tmp;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = tmp + 1;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						break;
					case 24:
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
						entry[pos] = ' ';
#else
						entry[pos] = 143;
#endif
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = '-';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
						entry[pos] = ' ';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
#endif
						break;
#if defined(TBS_EDITION_DE)
					case 13:
						no = decode(&st) - 1;
						src = UiText_NumberWords[(no & 7) * 2 + plural];
						while (*src != 0)
						{
							entry[pos] = *src++;
							pos = (pos + 1) & RENDER_ENTRY_MASK;
						}
						break;
#endif
#if defined(TBS_EDITION_IT)
					case 13:
						ending = decode(&st) - 1;
						if (ending == 0)
						{
							if (plural == 0)
								entry[pos] = 'o';
							else
								entry[pos] = 'i';
						}
						else if (plural == 0)
							entry[pos] = 'a';
						else
							entry[pos] = 'e';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						break;
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
					case 13:
#elif defined(TBS_EDITION_EN)
					case 25:
#endif
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_IT)
						if (plural != 0)
						{
							if (suffix != 0)
							{
								entry[pos] = 'e';
								pos = (pos + 1) & RENDER_ENTRY_MASK;
							}
							entry[pos] = 's';
							pos = (pos + 1) & RENDER_ENTRY_MASK;
						}
						break;
#endif
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
					case 25:
#if defined(TBS_EDITION_ES)
						UiRender_LookupNamedValue(2, 0);
						possessives = UiText_ItemPossessives;
						word = possessives.text[plural];
#else
						tmp = UiRender_LookupNamedValue(2, 0);
						tmp &= RENDER_ENTRY_MASK;
						possessives = UiText_ItemPossessives;
						kind = UiText_ItemGenders[tmp];
						word = possessives.text[kind & 3];
#endif
						/* FAKEMATCH: needed to match: value counts the letters, compared unsigned; a counter of its own loses r1 in Spanish and Italian. */
						for (value = 0; (u32)value < 16; value++)
						{
							if ((c8 = *word++) == 0)
								break;
							if (value == 0 && suffix == 2)
								c8 -= 0x20;
							entry[pos] = c8;
							pos = (pos + 1) & RENDER_ENTRY_MASK;
						}
						break;
#endif
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
					case 25:
#if defined(TBS_EDITION_DE)
						tmp = UiRender_LookupNamedValue(2, 0);
						tmp &= RENDER_ENTRY_MASK;
						possessives = UiText_ItemPossessives;
						kind = UiText_ItemGenders[tmp];
						word = possessives.text[kind & 3];
#else
						UiRender_LookupNamedValue(2, 0);
						word = UiText_ItemPronoun;
#endif
						for (i = 0; i < 16; i++)
						{
							if ((c8 = *word++) == 0)
								break;
							if (i == 0 && suffix == 2)
								c8 -= 0x20;
							entry[pos] = c8;
							pos = (pos + 1) & RENDER_ENTRY_MASK;
						}
						suffix = 0;
						break;
#endif
					case 27:
#if defined(TBS_EDITION_DE)
						if (suffix == 1)
							entry[pos] = '\'';
						else
							entry[pos] = 's';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
#elif defined(TBS_EDITION_EN)
						entry[pos] = '\'';
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						if (suffix == 0)
						{
							entry[pos] = 's';
							pos = (pos + 1) & RENDER_ENTRY_MASK;
						}
#endif
						break;
					case -1:
						break;
					case 1:
					case 3:
#if defined(TBS_EDITION_EN)
						head = 1;
#endif
					default:
						entry[pos] = (u16)ch;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
#if defined(TBS_EDITION_EN)
						if (ch == 's' || ch == 'S')
							suffix = 1;
						else
							suffix = 0;
#endif
						break;
					}
				}
			}
			cnt++;
		} while (running != 0 && cnt <= RENDER_ENTRY_MASK);

		entry[pos] = (u16)ch;
		pos = (pos + 1) & RENDER_ENTRY_MASK;
		entry[pos] = 0;
		next = (pos + 1) & RENDER_ENTRY_MASK;
		work->count = (u16)next;
		Runtime_ReleaseHeapBlock(TEXT_WORK_BLOCK);
		work->message_start = (u16)start;
	}

	if (clear != 0)
		UiWork_ClearValueNameTables();

	return start;
}

#endif

#if defined(TBS_EDITION_JA)

extern __inline__ u32 UiText_AppendArticleName(
	s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural, s32 *suffix)
{
	u16 c;

	if (mode != 0)
	{
		entry[pos] = ' ';
		pos = (pos + 1) & RENDER_ENTRY_MASK;
		entry[pos] = 10;
		pos = (pos + 1) & RENDER_ENTRY_MASK;
		entry[pos] = 10;
		pos = (pos + 1) & RENDER_ENTRY_MASK;
	}
	while (*name != 0)
	{
		c = *name++;
		entry[pos] = c;
		pos = (pos + 1) & RENDER_ENTRY_MASK;
	}
	if (mode != 0)
	{
		entry[pos] = 10;
		pos = (pos + 1) & RENDER_ENTRY_MASK;
		entry[pos] = 8;
		pos = (pos + 1) & RENDER_ENTRY_MASK;
		entry[pos] = ' ';
		pos = (pos + 1) & RENDER_ENTRY_MASK;
	}
	return pos;
}

s32 UiText_BuildRenderEntries(s32 script, s32 clear)
{
	struct UiRenderWork *work = UI_WORK;
	u32					 ch = 0;
	s32					 head = 1;
	u32					 start = work->count;
	u32					 cnt = 0;
	s32					 running = 1;
	u32					 pos = start;
	u16					*entry = work->entries;
	u8					 mode = work->menu_state & 2;
	s32 (*decode)(struct TextReader *);
	s32				  overflow = 0;
	u32				  prev;
	u32				  next;
	u32				  i;
	u32				  j;
	s32				  tmp;
	s32				  message;
	s32				  value;
	u8				 *num;
	u8				 *src;
	u16				 *dst;
	struct BtlUnit	 *actor;
	u16				  name[24];
	u8				  numbuf[16];
	struct TextReader st;

	if (script == -1)
	{
		start = work->message_start;
	}
	else
	{
		RamCode_LoadSlot(50, Text_DecodeSymbol, Text_DecodeSymbolCodeSize);
		decode = (s32 (*)(struct TextReader *))gWindowWork[50 - HEAP_SLOT_WINDOW];
		UiText_LookupMessage(&st, script);
		do
		{
			prev = ch;
			ch = (u32)decode(&st);
			if (overflow != 0)
			{
				if (ch < 32)
				{
					switch (ch)
					{
					case 19:
						UiRender_LookupNamedValue(3, clear);
						break;
					case 0:
					case 2:
					case 30:
						running = 0;
						break;
					case 8:
					case 9:
						decode(&st);
						break;
					case 22:
						UiRender_LookupNamedValue(5, clear);
						break;
					case 20:
						UiRender_LookupNamedValue(2, clear);
						break;
					case 21:
						UiRender_LookupNamedValue(4, clear);
						break;
					case 23:
						UiRender_LookupNamedValue(6, clear);
						break;
					case 17:
						decode(&st);
						break;
					case 18:
						break;
					case 1:
						running = 0;
						ch = 2;
						break;
					case 16:
						break;
					case -1:
						break;
					default:
						break;
					}
				}
			}
			else
			{
				if (work->spacing_before != 0 && head == 0 && ch != 222 && ch != 223)
				{
					entry[pos] = 5;
					pos = (pos + 1) & RENDER_ENTRY_MASK;
				}
				if (work->spacing_after != 0 && head == 0 && ch != 222 && ch != 223 && prev <= 256
					&& prev > 127 && prev != 222 && prev != 223 && prev != 32 && prev != 165
					&& prev != 161 && prev != 164)
				{
					entry[pos] = 222;
					pos = (pos + 1) & RENDER_ENTRY_MASK;
				}
				if (ch > 31)
				{
					if (mode != 0 && ch == ' ')
					{
						entry[pos] = ch;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
					}
					if (work->spacing_before != 0 && (ch == 32 || cnt > 10))
					{
						entry[pos] = 165;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = 165;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = 165;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = 161;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						overflow = 1;
						if (cnt > 10)
							ch = 32;
					}
					entry[pos] = (u16)ch;
					pos = (pos + 1) & RENDER_ENTRY_MASK;
					head = 0;
				}
				else
				{
					switch (ch)
					{
					case 0:
					case 2:
					case 30:
						running = 0;
						break;
					case 8:
					case 9:
						entry[pos] = (u16)ch;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						entry[pos] = decode(&st) + 0xffffU;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						break;
					case 22:
						value = UiRender_LookupNamedValue(5, clear);
						num = UiText_FormatNumber(numbuf, value, 0);
						i = num - numbuf;
						while (i != 16 && numbuf[i] != 0)
						{
							entry[pos] = numbuf[i];
							pos = (pos + 1) & RENDER_ENTRY_MASK;
							i++;
						}
						break;
					case 19:
						tmp = UiRender_LookupNamedValue(3, clear);
						UiText_DecodeMessage(tmp + (s32)&MsgClassName, name, 24);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 20:
						tmp = UiRender_LookupNamedValue(2, clear);
						UiText_DecodeMessage((tmp & 511) + (s32)&MsgItemName, name, 24);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 21:
						tmp = UiRender_LookupNamedValue(4, clear);
						UiText_DecodeMessage(tmp + (s32)&MsgAbilityName, name, 24);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 23:
						UiText_DecodeMessage(
							Scene_GetAreaNameIndexFar(UiRender_LookupNamedValue(6, clear), 1)
								+ (s32)&MsgDebugEntryName,
							name, 24);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 16:
						src = UnitGetFar(gGameState.selected_actor)->name;
						dst = name;
						i = 0;
						do
						{
							*dst = *src;
							i++;
							src++;
							dst++;
						} while (i <= 14);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 18:
						actor = UnitGetFar(UiRender_LookupNamedValue(1, clear));
						if (work->menu_state != 0
							&& (actor->class_id == 122 || actor->class_id == 125))
						{
							message = (s32)&MsgMysteryPersonName;
							if (actor->class_id == 125)
								message++;
							UiText_DecodeMessage(message, name, 10);
							pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						}
						else
						{
							src = actor->name;
							dst = name;
							j = 0;
							do
							{
								*dst = *src;
								j++;
								src++;
								dst++;
							} while (j <= 14);
							pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						}
						break;
					case 17:
						tmp = decode(&st) - 1;
						src = UnitGetFar(tmp)->name;
						dst = name;
						j = 0;
						do
						{
							*dst = *src;
							j++;
							src++;
							dst++;
						} while (j <= 14);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case -1:
						break;
					case 1:
					case 3:
						head = 1;
					default:
						entry[pos] = (u16)ch;
						pos = (pos + 1) & RENDER_ENTRY_MASK;
						break;
					}
				}
			}
			cnt++;
		} while (running != 0 && cnt <= RENDER_ENTRY_MASK);
		entry[pos] = (u16)ch;
		pos = (pos + 1) & RENDER_ENTRY_MASK;
		entry[pos] = 0;
		next = (pos + 1) & RENDER_ENTRY_MASK;
		work->count = (u16)next;
		Runtime_ReleaseHeapBlock(50);
		work->message_start = (u16)start;
	}
	if (clear != 0)
		UiWork_ClearValueNameTables();
	return start;
}

#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)

#define FIT_FIXED_LIMIT 1
#endif

void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, u16 *spacing, u32 flags)
{
#if !defined(FIT_FIXED_LIMIT)
	u8 *base;
#endif
	s32 x;
	s32 y;
	s32 limit;
	s32 right;
	s32 bottom;
	s32 over;
	s32 pos;

#if !defined(FIT_FIXED_LIMIT)
	base = gWindowWork[0];
#endif
	x = *px;
	y = *py;
	limit = 30;

	if (!(flags & 2))
	{
		if (flags & 1)
			UiText_MeasureStringVariant(no, pw, ph, spacing);
		else
			UiText_MeasureEntryDimensions(no, pw, ph, spacing);
	}

	if (*pw == 0 && *ph == 0)
		return;

	if (!(flags & 2))
	{
		*pw = (*pw + 19) >> 3;
		*ph = (*ph + 15) >> 3;
#if !defined(FIT_FIXED_LIMIT)
		if (((struct UiRenderWork *)base)->mode != 0)
		{
			*pw += 2;
			limit = 29;
		}
#endif
	}

	right = x + *pw;
	if (right > limit)
	{
		over = right - limit;
		pos = x - over;
		if (pos >= 0)
			x = pos;
		else
			x = 0;
	}
	bottom = y + *ph;
	if (bottom > 20)
	{
		over = bottom - 20;
		pos = y - over;
		if (pos >= 0)
			y = pos;
		else
			y = 0;
	}
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x > limit - *pw)
		x = limit - *pw;
	if (y > 20 - *ph)
		y = 20 - *ph;
	*px = x;
	*py = y;
}

void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y)
{
	UiText_MeasureEntryDimensions(UiText_BuildRenderEntries(no, 0), (u32 *)x, (u32 *)y, 0);
}

s32 __divsi3(s32 numerator, s32 denominator);

// no の表示エントリを作り、UiWindow_FitOnScreen に flags をつけてわたす。
// エントリがなければ 0 を返す
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)               \
	s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)     \
	{                                                             \
		struct UiRenderWork *work;                                \
		s32					 idx;                                 \
                                                                  \
		work = UI_WORK;                                           \
		idx = UiText_BuildRenderEntries(no, 0);                   \
		if (work->entries[idx] == 0)                              \
		{                                                         \
			return 0;                                             \
		}                                                         \
		UiWindow_FitOnScreen(idx, x, y, width, height, 0, flags); \
		return 1;                                                 \
	}

DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensions, 0)
DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensionsAlt, 1)

void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height, u16 *spacing)
{
	struct UiRenderWork *work;
	u32					 lines;
	u32					 width;
	u32					 height;
	u32					 line_width;
	u32					 count;
	u32					 c;
	u32					 glyph;
	u32					 i;
	u16					 counts[4];
	u16					 widths[4];
	s32					 gap;
	u32					 style;
#if !EDITION_INTERNATIONAL
	s32 trailing;
#endif

	work = UI_WORK;
#if !EDITION_INTERNATIONAL
	trailing = 0;
#endif
	width = 0;
	height = 15;
	lines = 0;
	line_width = 0;
	count = 0;
	for (;;)
	{
		c = work->entries[pos];
		pos = (pos + 1) & RENDER_ENTRY_MASK;
		if (c > 31)
		{
#if EDITION_INTERNATIONAL
			if (c == 32)
			{
				line_width += 5;
				count++;
			}
			else
			{
				glyph = UiText_Glyphs[c - 32].width;
				style = work->outline;
				if (style == 1 || style == 5)
					glyph++;
				line_width += glyph;
			}
#else
			if (c != 0xde && c != 0xdf)
			{
				if (c == 32)
				{
					line_width += 7;
				}
				else
				{
					if (c == 0xa5)
						trailing = -1;
					else if (c == 0x21)
						trailing = -3;
					else if (c == 0xa1 || c == 0xa4)
						trailing = -6;
					else
						trailing = 0;
					glyph = 10;
					if (c <= 0xff)
						glyph = UiText_Glyphs[c - 32].width;
					style = work->outline;
					if (style == 1 || style == 5)
						glyph++;
					line_width += glyph;
				}
			}
			count++;
#endif
			continue;
		}
		switch (c)
		{
		case 0:
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_FR) && !defined(TBS_EDITION_IT)
		case 1:
#endif
			goto done;
		case 3:
#if EDITION_INTERNATIONAL
			count++;
#else
			line_width += trailing;
#endif
			counts[lines] = count;
			widths[lines] = line_width;
			if (width < line_width)
				width = line_width;
			if (lines < 3)
				lines++;
			count = 0;
			line_width = 0;
			height += 15;
			break;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
		case 1:
			work->outline = 0;
			goto done;
#endif
		case 14:
		case 28:
			pos = (pos + 1) & RENDER_ENTRY_MASK;
		case 8:
		case 10:
		case 15:
		case 17:
			pos = (pos + 1) & RENDER_ENTRY_MASK;
			break;
		case 9:
			work->outline = work->entries[pos];
			pos = (pos + 1) & RENDER_ENTRY_MASK;
			break;
		}
	}
done:
#if EDITION_INTERNATIONAL
	count++;
#else
	line_width += trailing;
#endif
	counts[lines] = count;
	widths[lines] = line_width;
	if (width < line_width)
		width = line_width;
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	if (work->mode)
		width = width + 2;
#endif
	*out_width = width;
	*out_height = height;
	width = (width + 19) >> 3;
	width = (width << 3) - 16;
	if (spacing != NULL)
	{
		i = 0;
		while (i <= lines)
		{
			if (counts[i] <= 1)
			{
				*spacing++ = 0;
			}
			else
			{
				gap = width - widths[i];
				gap -= 4;
				if (gap < 0)
					gap = 0;
				gap = __divsi3(gap << 8, counts[i] - 1);
#if EDITION_INTERNATIONAL
				if (gap < 0 || gap > 0xc00)
					gap = 0x200;
#else
				if (gap < 0 || gap > 0x800)
					gap = 0;
				if (gap < 0 || gap > 0x100)
					gap = 0x100;
#endif
				*spacing++ = gap;
			}
			i++;
		}
	}
}

void UiText_MeasureStringVariant(s32 start, u32 *out_width, u32 *out_height, u16 *spacing)
{
	u32					 heights[16];
	u16					 counts[4];
	u16					 widths[4];
	struct UiRenderWork *work;
	u32					 max_width;
	u32					 line;
	u32					 width;
	u32					 count;
	u32					 variant;
	u32					 code;
	u32					 glyph;
	u32					 outline;
	u32					 i;
	u32					 j;
	s32					 gap;
#if !EDITION_INTERNATIONAL
	s32 trailing;

	trailing = 0;
#endif
	work = UI_WORK;
	heights[0] = 15;
	heights[1] = 15;
	heights[2] = 15;
	heights[3] = 15;
	heights[4] = 15;
	heights[5] = 15;
	heights[6] = 15;
	heights[7] = 15;
	heights[8] = 15;
	heights[9] = 15;
	heights[10] = 15;
	heights[11] = 15;
	heights[12] = 15;
	max_width = 0;
	heights[13] = 15;
	heights[14] = 15;
	heights[15] = 15;
	line = 0;
	width = 0;
	count = 0;
	variant = 0;
	for (;;)
	{
		code = work->entries[start];
		start = (start + 1) & RENDER_ENTRY_MASK;
		if (code >= 32)
		{
#if EDITION_INTERNATIONAL
			if (code == 32)
			{
				width += 5;
				count++;
			}
			else
			{
				glyph = UiText_Glyphs[code - 32].width;
				outline = work->outline;
				if (outline == 1 || outline == 5)
					glyph++;
				width += glyph;
			}
#else
			if (code != 0xde && code != 0xdf)
			{
				if (code == 32)
				{
					width += 7;
				}
				else
				{
					if (code == 0xa5)
						trailing = -1;
					else if (code == 0x21)
						trailing = -3;
					else if (code == 0xa1 || code == 0xa4)
						trailing = -6;
					else
						trailing = 0;
					glyph = 10;
					if (code <= 0xff)
						glyph = UiText_Glyphs[code - 32].width;
					outline = work->outline;
					if (outline == 1 || outline == 5)
						glyph++;
					width += glyph;
				}
			}
			count++;
#endif
			continue;
		}
		switch (code)
		{
		case 3:
#if EDITION_INTERNATIONAL
			counts[line] = ++count;
#else
			width += trailing;
			counts[line] = count;
#endif
			widths[line] = width;
			if (variant == 0 && max_width < width)
				max_width = width;
			if (line < 3)
				line++;
			count = 0;
			width = 0;
			heights[variant] += 15;
			break;
		case 0:
#if EDITION_INTERNATIONAL
			counts[line] = ++count;
#else
			width += trailing;
			counts[line] = count;
#endif
			widths[line] = width;
			if (variant == 0 && max_width < width)
				max_width = width;
			variant++;
			goto done;
		case 1:
#if EDITION_INTERNATIONAL
			counts[line] = ++count;
#else
			width += trailing;
			counts[line] = count;
#endif
			widths[line] = width;
			if (variant == 0 && max_width < width)
				max_width = width;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			work->outline = 0;
#endif
			variant++;
			break;
		case 14:
		case 28:
			start = (start + 1) & RENDER_ENTRY_MASK;
		case 8:
		case 10:
		case 15:
		case 17:
			start = (start + 1) & RENDER_ENTRY_MASK;
			break;
		case 9:
			work->outline = work->entries[start];
			start = (start + 1) & RENDER_ENTRY_MASK;
			break;
		}
	}
done:
#if !defined(TBS_EDITION_DE) && !defined(TBS_EDITION_FR)
	if (work->mode != 0)
		max_width += 2;
#endif
	for (i = 0; i < variant; i++)
	{
		if (i == 0)
			*out_height = heights[0];
		else if (*out_height < heights[i])
			*out_height = heights[i];
	}
	*out_width = max_width;
	max_width = (((max_width + 19) >> 3) << 3) - 16;
	if (spacing != NULL)
	{
		for (j = 0; j <= line; j++)
		{
			if (counts[j] <= 1)
			{
				*spacing++ = 0;
			}
			else
			{
				gap = max_width - widths[j];
				gap -= 4;
				if (gap < 0)
					gap = 0;
				gap = (gap << 8) / (counts[j] - 1);
#if !EDITION_INTERNATIONAL
				if (gap < 0 || gap > 0x800)
					gap = 0;
				if (gap < 0 || gap > 0x100)
					gap = 0x100;
#endif
				*spacing++ = gap;
			}
		}
	}
}

typedef s32 (*GlyphRoutine)(struct UiWindow *, s32, s32, s32, u8 *);

// RenderOutput の 0x10 からの 12 バイト：リンクと OAM の属性（スプライトのセル）
#define SPRITE_CELL(output) ((struct CursorSprite *)(output)->unknown_10)

s32 UiText_DrawGlyph(struct UiWindow *window, s32 character, s32 x, s32 y, s32 mode)
{
	struct UiRenderWork *work = UI_WORK;
	u32					 spacing_x = work->unknown_before_count;
	s32					 width;
	u32					 spacing_y = work->line_spacing;

	if (mode != 1 && (window->flags & 8) != 0)
	{
		u8			*font;
		GlyphRoutine draw;
		s32			 result;
#if !EDITION_INTERNATIONAL
		s32 mark;

		font = NULL;
		if (((struct BattleDisplayWork *)gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW])
				->window
			== window)
		{
			font = Resource_GetTableEntry((s32)&ResourceId_SmallFont);
			width = 3;
			if (character == ' ')
				goto done;
		}
		mark = (s16)UiText_MarkGlyphCodes[(u32)character >> 14];
		character &= 0x3fff;
		width = 4;
		if (character == ' ')
			goto done;
#else
		if (((struct BattleDisplayWork *)gWindowWork[HEAP_SLOT_BATTLE_DISPLAY - HEAP_SLOT_WINDOW])
				->window
			== window)
		{
			Resource_GetTableEntry((s32)&ResourceId_SmallFont);
			Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
			width = 3;
			if (character == ' ')
				goto done;
		}
		font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
		width = 4;
		if (character == ' ')
			goto done;
#endif
		RamCode_Load(draw, UiText_DrawGlyphArm, UiText_DrawGlyphCodeSize);
		result = draw(window, character, x, y, font);
#if !EDITION_INTERNATIONAL
		if ((u16)mark != 0)
			result += draw(window, (u16)mark, x + result, y, font);
#endif
		MemFree(draw);
		return result;
	}

	if (character == ' ')
	{
#if !EDITION_INTERNATIONAL
		width = 7;
#else
		width = 5;
#endif
		goto done;
	}
	{
		u32					 tiles[32];
		struct RenderOutput *output;
		struct CursorSprite *sprite;
		s32					 index;

		output = RenderOutput_AcquireFree();
		if (output == NULL)
			return 0;
		index = (output - work->outputs) * 4;
		output->active = 1;
		output->kind = 0;
		if (mode == 1)
		{
			width = 1;
			output->active = 2;
		}
		else
		{
			switch (work->outline)
			{
			case 3:
				output->active = 5;
				break;
			case 4:
				output->active = 6;
				output->anim_step = 8;
				break;
			case 5:
				output->active = 7;
				output->anim_step = 0;
				break;
			case 2:
				output->active = 4;
				output->anim_step = 0;
				break;
			}
#if !EDITION_INTERNATIONAL
			width = UiText_RenderGlyphPair(character, tiles);
#else
			{
				s32 drawn = UiText_RenderGlyphPair(character, tiles);

				if (drawn == 0)
					drawn = 1;
				width = drawn;
			}
#endif
		}

		if ((u8)output->active == 2)
		{
			sprite = SPRITE_CELL(output);
			if (work->glyph_resource == 99)
				work->glyph_resource = Resource_FindFreeEntry();
			sprite->x = (window->x << 3) + ((window->width - 2) << 3) + 4;
#if !EDITION_INTERNATIONAL
			sprite->y = (window->y << 3) + ((window->height - 2) << 3) + 1;
#else
			sprite->y = (window->y << 3) + ((window->height - 2) << 3) - 1;
#endif
		}
		else
		{
			Dma_Set(tiles, (void *)(0x06010000 + (work->glyph_tiles + index) * 32), 0x84000020,
				REG_DMA3);
#if !EDITION_INTERNATIONAL
			((u16 *)&output->oam)[0] = y + (spacing_y >> 1) + window->y * 8;
#else
			((u16 *)&output->oam)[0] = y + (spacing_y >> 1) + window->y * 8 - 2;
#endif
			((u16 *)&output->oam)[1] = (x + (spacing_x >> 1) + window->x * 8 + 2) | 0x4000;
			((u16 *)&output->oam)[2] = work->glyph_tiles + index;
			sprite = SPRITE_CELL(output);
		}
		output->sentinel = 254;
		output->x = sprite->x;
		output->y = sprite->y;
		output->index = index;
		output->next = NULL;
		RenderOutput_AppendToList(&window->output, output);
	}
done:
	return width;
}

// RenderOutput の見方：0x10 からのスプライトのセルをメンバー attr として持つ。
// RenderOutput の unknown_10 をキャストして読むと UiWork_AnimateSpriteSlots が合わない (578 -> 605 命令)
struct SpriteOutput
{
	struct SpriteOutput *next;
	u8					 kind;
	u8					 active;
	u16					 x;
	u16					 y;
	u16					 unknown_0a;
	u16					 anim_step;
	u8					 index;
	u8					 sentinel;
	struct CursorSprite	 attr;
};

LAYOUT_OFFSET_GUARD(SpriteOutput_X, struct SpriteOutput, x, (u32) & ((struct RenderOutput *)0)->x);
LAYOUT_OFFSET_GUARD(SpriteOutput_Y, struct SpriteOutput, y, (u32) & ((struct RenderOutput *)0)->y);
LAYOUT_OFFSET_GUARD(
	SpriteOutput_Active, struct SpriteOutput, active, (u32) & ((struct RenderOutput *)0)->active);
LAYOUT_OFFSET_GUARD(SpriteOutput_AnimStep, struct SpriteOutput, anim_step,
	(u32) & ((struct RenderOutput *)0)->anim_step);
LAYOUT_OFFSET_GUARD(
	SpriteOutput_Index, struct SpriteOutput, index, (u32) & ((struct RenderOutput *)0)->index);
LAYOUT_OFFSET_GUARD(
	SpriteOutput_Attr, struct SpriteOutput, attr, (u32) & ((struct RenderOutput *)0)->unknown_10);

#if EDITION_INTERNATIONAL

void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
	void				*work = gWindowWork[0];
	u8					*base = work;
	struct SpriteOutput *out;
	s32					 idx;
	u16					*slot;
	struct CursorSprite *attr;
	u32					 pos;
	u16					 row;

	if (y > (win->height - 2))
		return;
	if (x > (win->width - 2))
		return;
	if (mode == 1)
	{
		s32 column;
		work = RenderOutput_AcquireFree();
		if (work == NULL)
			return;
		out = work;
		idx = ((struct RenderOutput *)out - ((struct UiRenderWork *)base)->outputs) * 4;
		out->active = 2;
		attr = &out->attr;
		slot = &((struct UiRenderWork *)base)->glyph_resource;
		if (*slot == 99)
			*slot = Resource_FindFreeEntry();
		column = 0xfffe;
		attr->x = (win->x + (column + *(u16 *)&win->width)) * 8 + 4;
		row = (u8)win->y + (row = (u8)win->height + 254);
		attr->y = row * 8 - 1;
		/* FAKEMATCH: needed to match: the embedded OAM view preserves packed/logical position store order changed by byte-span and scalar views. */
		out->x = attr->x;
		out->y = attr->y;
		out->next = NULL;
		out->index = idx;
		if (out->active == 0)
			out->active = mode;
		RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
	}
	else if (tile <= 0xff)
	{
		x++;
		y++;
		pos = (win->y + y) * 32 + (win->x + x);
		if (pos < 640)
			((struct UiRenderWork *)work)->tilemap[pos] = tile | 0xf000;
	}
}

#else

void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
	u8 *work = gWindowWork[0];
	/* FAKEMATCH: needed to match: this XOR pair changes no pointer bits and folds without instructions, preserving work and base as independent lifetimes. A direct copy merges both into r8, omits the native r8/r12 copy, and differs 17 aligned instructions (0x144 versus 0x148 bytes). */
	u8					*base = (u8 *)((unsigned long)work ^ tile);
	struct SpriteOutput *out;
	s32					 idx;
	u16					*slot;
	struct CursorSprite *attr;
	u32					 pos;
	u16					 row;

	base = (u8 *)((unsigned long)base ^ tile);

	if (y > (win->height - 2))
		return;
	if (x > (win->width - 2))
		return;
	if (mode == 1)
	{
		s32 column;
		u16 left;
		out = (struct SpriteOutput *)RenderOutput_AcquireFree();
		if (out == NULL)
			return;
		idx = ((struct RenderOutput *)out - ((struct UiRenderWork *)base)->outputs) * 4;
		out->active = 2;
		attr = &out->attr;
		slot = &((struct UiRenderWork *)base)->glyph_resource;
		if (*slot == 99)
			*slot = Resource_FindFreeEntry();
		column = 0xfffe;
		left = win->x;
		attr->x = (left + (column + *(u16 *)&win->width)) * 8 + 4;
		row = (u8)win->y + (row = (u8)win->height + 254);
		attr->y = row * 8 + 1;
		/* FAKEMATCH: needed to match: the embedded OAM view preserves packed/logical position store order changed by byte-span and scalar views. */
		out->x = attr->x;
		out->y = attr->y;
		out->next = NULL;
		out->index = idx;
		if (out->active == 0)
			out->active = mode;
		RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
	}
	else if (tile <= 0xff)
	{
		if (tile - 0xde <= 1)
		{
			u32 line;

			line = (win->y + y) * 32;
			switch (*(((struct UiRenderWork *)base)->tilemap + (line + (win->x + x))))
			{
			case 0xf011:
				tile -= 0xc0;
				break;
			case 0xf00e:
				tile -= 0xd0;
				break;
			}
		}
		else
		{
			x++;
			y++;
		}
		pos = (win->y + y) * 32 + (win->x + x);
		if (pos < 640)
			((struct UiRenderWork *)work)->tilemap[pos] = tile | 0xf000;
	}
}

#endif

enum
{
	TILEMAP_ENTRY_PLAIN,
	TILEMAP_ENTRY_NONE,
	TILEMAP_ENTRY_PALETTE_14,
	TILEMAP_ENTRY_PALETTE_15,
	TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode)
{
	struct UiRenderWork *work = UI_WORK;
	s32					 palette;
	s32					 index;

	y += 1;
	x += 1;
	if (y > (u32)(window->height - 1))
		return;
	if (x > (u32)(window->width - 1))
		return;

	switch (mode)
	{
	case TILEMAP_ENTRY_PALETTE_15:
		palette = 0xf000;
		break;
	case TILEMAP_ENTRY_PALETTE_14:
		palette = 0xe000;
		break;
	case TILEMAP_ENTRY_PALETTE_1:
		palette = 0x1000;
		break;
	default:
		palette = 0;
		break;
	}

	switch (mode)
	{
	case TILEMAP_ENTRY_NONE:
		return;
	case TILEMAP_ENTRY_PALETTE_14:
	case TILEMAP_ENTRY_PALETTE_15:
	case TILEMAP_ENTRY_PALETTE_1:
		index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
		if ((u32)index >= TILEMAP_ENTRIES)
			return;
		work->tilemap[index] = palette | value;
		return;
	case TILEMAP_ENTRY_PLAIN:
	default:
		index = (window->y + y) * TILEMAP_WIDTH + (window->x + x);
		if ((u32)index >= TILEMAP_ENTRIES)
			return;
		work->tilemap[index] = value;
		return;
	}
}

void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output)
{
	s32					   scale = 256;
	struct CursorSprite	  *sprite = SPRITE_CELL(output);
	struct AffineTransform effect;

	switch ((u8)output->active)
	{
	case 9:
		scale = Ui_BounceScales[output->anim_step++ & 31];
		break;
	case 10:
		scale = Ui_BounceScales[output->anim_step++ & 31] >> 1;
		break;
	case 11:
		if (output->anim_step <= 7)
			scale = Ui_BounceScales[output->anim_step++ * 2 + 16];
		break;
	case 12:
		if (output->anim_step <= 7)
			scale = Ui_BounceScales[output->anim_step++ * 2 + 16] >> 1;
		break;
	}
	if (scale == 256)
	{
		sprite->affine_index = 0;
		sprite->affine = 0;
		sprite->x = output->x;
		sprite->y = output->y;
	}
	else
	{
		effect.scale_x = scale;
		effect.scale_y = scale;
		effect.angle = 0;
		sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
		if (scale > 256)
		{
			sprite->affine = 3;
			/* FAKEMATCH: needed to match: the unsigned halfword read of the x cell; a cast of the signed member swaps the x load and the -8 pool load. */
			sprite->x = *(u16 *)&output->x - 8;
			sprite->y = (u16)output->y - 8;
		}
		else
		{
			sprite->affine = 1;
			sprite->x = output->x;
			sprite->y = output->y;
		}
	}
}

void UiWork_AnimateSpriteSlots(void)
{
	struct UiRenderWork	  *work = UI_WORK;
	struct UiWindow		  *window = work->windows;
	s32					   no;
	struct AffineTransform effect;

	for (no = 0; no != WINDOW_COUNT; window++, no++)
	{
		struct SpriteOutput *output;
		u32					 phase;

		if (!(window->flags & 1))
			continue;
		output = (struct SpriteOutput *)window->output.head;
		phase = (gFrameTick >> 2) & 7;
		while (output != NULL)
		{
			struct CursorSprite *sprite = &output->attr;

			if (window->effect == 4)
			{
				output->anim_step = 2;
				output->active = 8;
			}
			switch (output->active)
			{
			case 2:
				if (work->glyph_resource != 0x60)
				{
					const u8 *tiles = &UiSprite_CursorFrameTiles[phase * 128];

					sprite->tile = VramBlock_LoadCached(work->glyph_resource, 128, tiles);
					output->index = sprite->tile;
					sprite->mode = 0;
					sprite->mosaic = 0;
					sprite->colors = 1;
					sprite->size = 0;
					sprite->shape = 2;
					sprite->y = output->y + UiSprite_CursorBobOffsets[gFrameTick % 80] + 2;
					sprite->affine = 0;
					sprite->affine_index = 0;
				}
				break;
			case 5:
				if (gFrameTick & 1)
				{
					sprite->x = output->x
								+ (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
					sprite->y = output->y
								+ (s32)(((Random16() * 3 >> 16) + (Random16() * 3 >> 16)) >> 1) - 1;
				}
				break;
			case 6:
				if (output->anim_step == 0)
					goto reset;
				effect.scale_x = 512;
				effect.scale_y = 512;
				effect.angle = 0;
				sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
				sprite->affine = 3;
				sprite->x = output->x - 5;
				sprite->y = output->y - 5;
				output->anim_step--;
				break;
			case 7:
				effect.scale_x = 256;
				effect.scale_y = 256;
				output->anim_step += 768;
				effect.angle = output->anim_step;
				sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
				sprite->affine = 1;
				sprite->x = output->x - (Trig_Sin(effect.angle + 0xe800) >> 14) - 2;
				sprite->y = output->y - (Trig_Cos(effect.angle + 0x6800) >> 14) - 2;
				break;
			case 4:
				if (gFrameTick & 1)
					output->anim_step++;
				sprite->x = output->x + UiSprite_WobbleOffsets[output->anim_step % 20][0];
				sprite->y = output->y + UiSprite_WobbleOffsets[output->anim_step % 20][1] - 2;
				break;
			case 17:
				output->anim_step++;
				sprite->y = output->y - UiSprite_BounceOffsets[output->anim_step & 15];
				break;
			case 14:
			case 15:
			case 16:
				output->anim_step++;
				sprite->y = output->y + UiSprite_BounceOffsets[output->anim_step & 15];
				break;
			case 18:
				output->anim_step++;
				sprite->x = output->x - UiSprite_BounceOffsets[output->anim_step & 15];
				sprite->y = output->y + UiSprite_BounceOffsets[output->anim_step & 15];
				break;
			case 8:
				if (output->anim_step == 0)
					goto reset;
				effect.scale_x = 320;
				effect.scale_y = 320;
				effect.angle = 0;
				sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
				sprite->affine = 3;
				sprite->x = output->x - 8;
				sprite->y = output->y - 8;
				output->anim_step--;
				break;
			reset:
				sprite->affine_index = 0;
				sprite->affine = 0;
				sprite->x = output->x;
				sprite->y = output->y;
				break;
			case 9:
			case 10:
			case 11:
			case 12:
				RenderOutput_UpdateScaleAnimation((struct RenderOutput *)output);
				break;
			}
			if (output->active == 2)
			{
				if (work->glyph_resource != 0x60)
					Runtime_PushSlotEntry(sprite, output->sentinel);
			}
			else if (output->active != 13)
			{
				Runtime_PushSlotEntry(sprite, output->sentinel);
			}
			output = output->next;
			phase = (gFrameTick >> 2) & 7;
		}
	}
}

//--------------------------------------------------------------------------
// adjust_value_without_limit.c
// ウィンドウ：メッセージを長さの制限なしに文字列にする
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// メッセージ mes を buf に展開する（長さの上限なし）
//--------------------------------------------------------------------------
void Ui_AdjustValueWithoutLimit(s32 mes, s32 buf)
{
	UiText_DecodeMessage(mes, buf, 0x7fffffff);
}

s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity)
{
	struct UiRenderWork *work;
	u32					 count;

	work = UI_WORK;
	work->count = 0;
	UiText_BuildRenderEntries(key, 1);
	for (count = 0; count < capacity - 1 && (destination[count] = work->entries[count]) != 0;
		count++)
	{
	}
	destination[count] = 0;
	return count;
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#define DECODE_EUROPEAN
#endif

void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity)
{
	struct TextReader reader;
	void			 *original;
	s32 (*decode)(struct TextReader *);
	u32 value;
#if !defined(DECODE_EUROPEAN)
	u16 delta;
#endif

	original = ((union HeapState *)&gWorkSlot)->slots[50];
	if (original == NULL)
		RamCode_LoadSlot(50, Text_DecodeSymbol, Text_DecodeSymbolCodeSize);
	decode = (s32 (*)(struct TextReader *))((union HeapState *)&gWorkSlot)->slots[50];
	UiText_LookupMessage(&reader, message);
#if !defined(DECODE_EUROPEAN)
	delta = 0xffff;
#endif
	while ((value = decode(&reader)) != 0)
	{
		switch (value)
		{
		case 14:
			capacity -= 3;
			if (capacity <= 0)
				goto done;
			*text++ = value;
#if defined(DECODE_EUROPEAN)
			value = decode(&reader) + 0xffff;
			*text++ = value;
			value = decode(&reader) + 0xffff;
#else
			*text++ = decode(&reader) + delta;
			value = decode(&reader) + delta;
#endif
			break;
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 15:
#if defined(DECODE_EUROPEAN)
		case 29:
#endif
			capacity--;
			if (capacity <= 0)
				goto done;
			*text++ = value;
			value = decode(&reader) + 0xffff;
			break;
		default:
			capacity--;
			if (capacity <= 0)
				goto done;
			break;
		}
		*text++ = value;
	}
done:
	if (original == NULL)
		Runtime_ReleaseHeapBlock(50);
	*text = 0;
}

//--------------------------------------------------------------------------
// clear_word_if_set.c
// メモリ：0 でないワードを 0 にする
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// *p が 0 でなければ 0 にして、p を返す
//--------------------------------------------------------------------------
s32 *Runtime_ClearWordIfSet(s32 *p)
{
	if (*p != 0)
	{
		*p = 0;
	}
	return p;
}

//--------------------------------------------------------------------------
// uiwork.c
// ウィンドウ：メッセージのチャンネルの後始末と、メッセージに埋める値の表
//--------------------------------------------------------------------------

#if !GAME_TLA

//--------------------------------------------------------------------------
// ウィンドウをすべて閉じる
// 3 つのチャンネルのウィンドウを閉じ、開閉の動きが終わるまで待ってから、
// 直接開いたウィンドウを閉じる
//--------------------------------------------------------------------------
void UiWork_DrainPending(s32 unused)
{
	struct UiRenderWork	 *uw;
	struct UiChannelSlot *ch;
	struct UiWindow		 *win;
	u32					  done;
	struct UiWindow		 *work;
	struct UiWindow		 *poll_work;
	s32					  i;
	u16					  flag;

	uw = UI_WORK;
	ch = uw->channels;
	win = uw->windows;
	i = 0;
	do
	{
		work = ch->work;
		if (work != 0 && work->flags != 0)
			UiWork_Finalize(work, 0);
		i++;
		ch++;
	} while (i != 3);

	// 閉じ終えたチャンネルを空ける。動いているものがあれば 1 フレーム待つ
poll:
	done = 1;
	ch = uw->channels;
	i = 0;
	do
	{
		poll_work = ch->work;
		if (poll_work != 0)
		{
			if ((*(s32 *)&poll_work->frame) == 0) // frame と duration がともに 0
			{
				flag = poll_work->flags;
				if (flag == 0)
					ch->work = (struct UiWindow *)(u32)flag;
				else
					done = 0;
			}
			else
			{
				done = 0;
			}
		}
		i++;
		ch++;
	} while (i != 3);
	i = 0;
	if (!done)
	{
		WaitFrames(1);
		goto poll;
	}

	// 直接開いたウィンドウ
	goto win_test;
win_loop:
	if (win->flags != 0)
		UiWork_Finalize(win, 0);
	win++;
	i++;
win_test:
	if (i != WINDOW_COUNT)
		goto win_loop;
}

//--------------------------------------------------------------------------
// チャンネルのウィンドウの効果 4 を 1 フレーム進める
// countdown のあいだ 1 タイル大きい枠を描き、終わったら元の枠に戻す
//--------------------------------------------------------------------------
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *ch)
{
	struct UiWindow *work = ch->work;
	s32				 effect = work->effect;
	s32				 x = work->x;
	s32				 y = work->y;
	s32				 w = work->width;
	s32				 h = work->height;

	if (effect != 4)
		return;

	UiWindow_DrawFrame(x - 1, y - 1, w + 2, h + 2);
	ch->countdown--;
	if (ch->countdown != 0)
		return;

	ch->work->effect = 0;
	UiWindow_EraseBorderRect(x - 1, y - 1, w + 2, h + 2);
	UiWindow_DrawFrame(x, y, w, h);
}

//--------------------------------------------------------------------------
// メッセージに埋める値の表（8 つ）を空にする
//--------------------------------------------------------------------------
void UiWork_ClearValueNameTables(void)
{
	s32					 no;
	struct UiRenderWork *work;

	work = UI_WORK;
	no = 0;

	do
	{
		work->values[no] = 0;
		work->names[no] = 0;
		no++;
	} while (no != 8);
}

//--------------------------------------------------------------------------
// 表のあいているところに、種類 kind の値 value を入れる
// （いっぱいなら入れない）
//--------------------------------------------------------------------------
void UiWork_PushValueSlot(u32 value, u32 kind)
{
	struct UiRenderWork *work = UI_WORK;
	u32					 no = 0;
	u32					 limit = 8;

	do
	{
		if (work->names[no] == 0)
		{
			work->values[no] = value;
			work->names[no] = kind;
			break;
		}
		no++;
	} while (no != limit);
}

//--------------------------------------------------------------------------
// 表から種類 kind の値をさがす（なければ 0）
// clear : 0 でなければ、見つけたものを表から消す
//--------------------------------------------------------------------------
u32 UiRender_LookupNamedValue(u32 kind, u32 clear)
{
	struct UiRenderWork *work = UI_WORK;
	u32					 i;
	u32					 value = 0;

	for (i = 0; i < 8; i++)
	{
		if (work->names[i] == kind)
		{
			value = work->values[i];
			if (clear != 0)
			{
				work->values[i] = 0;
				work->names[i] = 0;
			}
			break;
		}
	}
	return value;
}

//--------------------------------------------------------------------------
// メッセージの待ちを打ち切るか
// busy で音の状態が 0 のとき、または A・B・L・R を押しているとき、
// countdown を 0 にして 1 を返す
//--------------------------------------------------------------------------
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *ch)
{
	struct UiRenderWork *work = UI_WORK;
	s32					 cancel = 0;
	s32					 zero;

	if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
		cancel = 1;
	zero = 0;
	if (gKeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancel = 1;
	if (cancel != zero)
	{
		ch->countdown = zero;
		return 1;
	}
	return zero;
}

//--------------------------------------------------------------------------
// メッセージの待ちを打ち切るか（押したキーで見る）
// mode が 0 でなければ、ためておいたキーで見る
//--------------------------------------------------------------------------
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *ch)
{
	struct UiRenderWork *work = UI_WORK;
	s32					 key;
	s32					 cancel = 0;
	u8					 zero;

	if (work->busy != 0 && AudioCommand_GetStateByteFar() == 0)
		cancel = 1;
	key = gKeyState;
	zero = 0;
	if (work->mode != zero)
		key = gKeysPressedLatch;
	if (key & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancel = 1;
	if (cancel != 0)
	{
		ch->countdown = zero;
		return 1;
	}
	return 0;
}

//--------------------------------------------------------------------------
// 書き終えたチャンネルのウィンドウを閉じ、10 フレーム待つ
//--------------------------------------------------------------------------
void UiWork_FinalizePendingCore(void)
{
	s32					  i;
	struct UiChannelSlot *ch;
	struct UiWindow		 *work;

	ch = UI_WORK->channels;
	i = 0;
	do
	{
		work = ch->work;
		if (work != NULL && (*(s32 *)&work->frame) == 0 && work->flags != 0 && work->state != 0)
		{
			UiWork_Finalize(work, 2 & work->flags);
		}
		i++;
		ch++;
	} while (i != 3);
	WaitFrames(10);
}
#endif

void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset)
{
	struct UiRenderWork *work;
	struct UiWindow		*window;
	s32					 x;
	s32					 y;
	s32					 width;
	s32					 height;
	s32					 entry;

	work = UI_WORK;
	x = 8;
	y = 8;
	// the window starts as NULL, though every path that reads it assigns it first; the same zero is the style argument below
	window = NULL;
	entry = UiText_BuildRenderEntries(message, 1);
	if (work->entries[entry] != 0)
	{
		UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width, (u32 *)&height);
		x = (30 - width) >> 1;
		y = ((15 - height) >> 1) + y_offset;
		if (mode != 0)
			window = UiWindow_Create(x, y, width, height, 0);
		else
		{
			window = UiWindow_Create(x, y, 0, 0, 2);
			window->width = mode;
			window->height = mode;
		}
		if (UiText_QueueRenderEntries(window, entry, 0, 0, 0, 0) == 0)
			UiWork_Finalize(window, 1);
		else
		{
			while (UiWork_IsComplete() == 0)
				WaitFrames(1);
			if (mode != 0)
			{
				UiWork_Finalize(window, 0);
				while (UiWork_IsIdle(window) == 0)
					WaitFrames(1);
			}
			else
				UiWork_Finalize(window, 1);
			work->result[0] = 0;
			work->result[1] = 0;
		}
	}
}

s32 UiText_BuildRenderEntriesMode1(s32 arg0)
{
	return UiText_BuildRenderEntries(arg0, 1);
}

void UiText_LookupMessage(struct TextReader *reader, s32 message)
{
	void (*routine)(struct TextReader *, s32);

	RamCode_Load(routine, UiText_LookupMessageArm, UiText_LookupMessageCodeSize);
	routine(reader, message);
	MemFree(routine);
}

extern const void *const Text_MessageContexts[][2];

s32 UiText_DecodeSymbol(struct TextReader *reader)
{
	u32		  code;
	u32		  hi;
	const u8 *base;
	const u8 *tree;
	const u8 *leaves;
	const u8 *text;
	s32		  text_bits;
	s32		  tree_bits;
	s32		  rank;
	s32		  depth;
	s32		  bit;
	u32		  index;
	u32		  half;

	code = reader->previous_code;
	text = (const u8 *)reader->words;
	text_bits = reader->bits;
	tree_bits = 1;
	hi = code >> 8;
	code &= 0xff;
	base = Text_MessageContexts[hi][0];
	base += ((const u16 *)Text_MessageContexts[hi][1])[code];
	tree = base;
	leaves = base - 1;
	rank = 0;
	for (;;)
	{
		bit = tree_bits & 1;
		tree_bits >>= 1;
		if (bit != 0 && tree_bits == 0)
		{
			tree_bits = *tree++;
			bit = tree_bits & 1;
			tree_bits = (tree_bits >> 1) | 0x80;
		}
		if (bit != 0)
			break;
		bit = text_bits & 1;
		text_bits >>= 1;
		if (bit != 0 && text_bits == 0)
		{
			text_bits = *text++;
			bit = text_bits & 1;
			text_bits = (text_bits >> 1) | 0x80;
		}
		if (bit == 0)
			continue;
		depth = 0;
		do
		{
			bit = tree_bits & 1;
			tree_bits >>= 1;
			if (bit != 0 && tree_bits == 0)
			{
				tree_bits = *tree++;
				bit = tree_bits & 1;
				tree_bits = (tree_bits >> 1) | 0x80;
			}
			if (bit == 0)
				depth++;
			else
			{
				rank++;
				depth--;
			}
		} while (depth >= 0);
	}
	index = rank * 3;
	if (((index * 4) & 7) == 0)
	{
		bit = leaves[-(index >> 1)];
		code = bit << 4;
		bit = leaves[-(index >> 1) - 1];
		code |= (u32)bit >> 4;
	}
	else
	{
		half = leaves[-(index >> 1)];
		half &= 0xf;
		code = leaves[-(index >> 1) - 1] | (half << 8);
	}
	reader->bits = text_bits;
	reader->previous_code = code;
	reader->words = (const u32 *)text;
	return code;
}

void UiWork_SetTwoEntriesTo999(void)
{
	struct UiRenderWork *work = UI_WORK;
	s32					 index = 0;

	do
	{
		work->resource_ids[index] = 999;
		index++;
	} while (index != 2);
}

s32 Localization_LookupEntryId(u32 value)
{
	s32 result = -1;
	s32 i = 0;

	if (value < 20)
	{
		for (;;)
		{
			s32 key = Portrait_CharacterMap[i];

			if (key == -1)
				break;
			if (key == value)
			{
				i++;
				result = Portrait_CharacterMap[i];
				break;
			}
			i += 2;
		}
	}
	else
	{
		for (;;)
		{
			s32 key = Portrait_SpriteMap[i];

			if (key == -1)
				break;
			if (key == value)
			{
				i++;
				result = Portrait_SpriteMap[i];
				result += 128;
				break;
			}
			i += 2;
		}
	}
	return result;
}

s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y)
{
	s32	  win;
	s32	  minus_four;
	s32	  ofs;
	void *work;

	work = gWindowWork[0];
	if (Localization_LookupEntryId(arg0) == -1)
	{
		return 0;
	}
	minus_four = -4;
	ofs = minus_four;
	if (((struct UiRenderWork *)work)->mode != 0)
	{
		win = (s32)UiWindow_Create(x, y, 6, 5, 2);
		ofs = 0;
	}
	else
	{
		win = (s32)UiWindow_Create(x, y, 5, 5, 2);
	}
	if (win != 0)
	{
		CreateSideObject(arg0, arg1, -1, win, ofs, minus_four);
	}
	return win;
}

void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry)
{
	struct UiRenderWork *state;
	struct UiWindow		*work;
	s32					 id;
	s32					 index;
	s32					 i;
	struct RenderOutput *entity;

	state = UI_WORK;
	work = state->windows;
	id = Localization_LookupEntryId(entry);
	if (id == -1)
		return;

	if (state->resource_ids[1] == id)
	{
		index = 1;
	}
	else if (state->resource_ids[0] == id)
	{
		index = 0;
	}
	else
	{
		return;
	}

	id = state->output_ids[index];

	for (i = 0; i != WINDOW_COUNT; i++, work++)
	{
		entity = work->output.head;
		if ((u8)entity->kind == 2 && (u8)entity->index == id)
		{
			UiWork_Finalize(work, 2);
			return;
		}
	}
}

s32 Ui_CountIconTableEntries(void)
{
	return UiIcon_ItemIconPointersEnd - UiIcon_ItemIconPointers;
}

s32 Ui_CountSecondTableEntries(void)
{
	return UiIcon_PsynergyIconPointersEnd - UiIcon_PsynergyIconPointers;
}

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
	GlyphTransfer *work;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	work->encoded = UiIcon_FramePointerTable[no1];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, 0);
	work->encoded = UiIcon_OverlayPointerTable[no0];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, 1);
	if (flag == 0)
	{
		*slot = Resource_FindFreeEntry();
	}
	*ret = VramBlock_LoadCached(*slot, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
}

void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
	UiIcon_BuildItemIconTiles(BtlActGet(action)->unknown_04[0], with_base, slot, tile, reuse);
}

void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse)
{
	GlyphTransfer *work;
	s32			   slot;

	slot = 0;
	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));

	if (glyph >= Ui_CountIconTableEntries())
		glyph = 0;

	if (with_base != 0)
	{
		work->encoded = UiIcon_FramePointerTable[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 0);
		slot = 1;
	}

	work->encoded = UiIcon_ItemIconPointers[glyph];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, slot);

	if (reuse == 0)
		*src = Resource_FindFreeEntry();

	*dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
}

extern GlyphTransfer *gGlyphWork;

#define ICON_FRAME 1
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

s32 ItemIcon_Compose(u32 code, u32 layers)
{
	s32					   overlay;
	struct ItemDefinition *item;
	GlyphTransfer		  *work;
	s32					   count;

	overlay = 0;
	count = 0;
	item = Item_GetDirectFar(code & 0x1ff);
	work = gGlyphWork;
	if (work == NULL)
		return -1;
	if (layers & ICON_FRAME)
	{
		work->encoded = UiIcon_FramePointerTable[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 0);
		overlay = 1;
	}
	work->encoded = UiIcon_ItemIconPointers[item->icon];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, overlay);
	if ((layers & ICON_EQUIPPED) && (code & 0x400))
	{
		work->encoded = UiIcon_MarkPointers[1];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
	}
	if ((layers & ICON_BROKEN) && (code & 0x200))
	{
		work->encoded = UiIcon_MarkPointers[0];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
	}
	if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) && (item->flags & 2))
	{
		work->encoded = UiIcon_MarkPointers[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
	}
	if (layers & ICON_COUNT_ABOVE_ONE)
	{
		count = (code & 0xf800) >> 11;
		count++;
		if (count <= 1)
			count = 0;
	}
	if (layers & ICON_COUNT)
	{
		count = (code & 0xf800) >> 11;
		count++;
	}
	if (count != 0 && count <= 30)
	{
		s32 ones = count % 10;

		work->encoded = UiIcon_DigitPointers[ones];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 1);
		if (count / 10 != 0)
		{
			work->encoded = UiIcon_DigitPointers[count / 10 + 9];
			work->width = 2;
			work->height = 2;
			UiGlyph_DecodeWithHeapRoutines(work, 1);
		}
	}
	return 256;
}

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
	GlyphTransfer *work;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	ItemIcon_Compose(arg0, arg1);
	VramBlock_LoadCached(arg2, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
	return 1;
}

s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot)
{
	GlyphTransfer *work;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	Ui_PrepareTransferFromTableEntry(icon);
	VramBlock_LoadCached(slot, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
	return 1;
}
extern const u8 *RenderResource_PairSourceTable[];

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot)
{
	/* FAKEMATCH: three more names of the one table keep each case's own literal slot; one name folds the switch to a single load. */
	extern const u8 *PairSource1[] __asm__("RenderResource_PairSourceTable"), *PairSource2[] __asm__("RenderResource_PairSourceTable"), *PairSource3[] __asm__("RenderResource_PairSourceTable");
	void *source;
	switch (value)
	{
	case 1:
		source = PairSource1;
		break;
	case 2:
		source = PairSource2;
		break;
	case 3:
		source = PairSource3;
		break;
	case 0:
	default:
		source = RenderResource_PairSourceTable;
		break;
	}
	VramBlock_LoadCached(slot, 32, source);
	return 1;
}

void Ui_PrepareTransferForItem(s32 code)
{
	GlyphTransfer		  *state = gGlyphWork;
	struct ItemDefinition *item = Item_GetDirectFar(0x1ff & code);

	if (code != 0)
	{
		state->encoded = UiIcon_ItemIconPointers[item->icon];
	}
	else
	{
		state->encoded = UiIcon_ItemIconPointers[0];
	}
	state->width = 2;
	state->height = 2;
	UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse)
{
	UiIcon_BuildAbilityIconTiles(BtlActGet(action)->unknown_04[0], with_base, slot, tile, reuse);
}

#define ROUTINE_BLOCK 49

struct GlyphCursor
{
	u16 unused;
	u16 cursor;
};

static __inline__ void ResetCursor(struct GlyphCursor *cursor)
{
	/* FAKEMATCH: needed to match: keep the cursor-subrecord base for the paired row stores. */
	cursor->cursor = 0;
}

void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse)
{
	GlyphTransfer *work;
	s32			   slot;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	slot = 0;

	if (glyph >= Ui_CountSecondTableEntries())
		glyph = 0;

	if (with_base != 0)
	{
		work->encoded = UiIcon_FramePointerTable[2];
		work->width = 2;
		work->height = 2;
		UiGlyph_DecodeWithHeapRoutines(work, 0);
		slot = 1;
	}

	work->encoded = UiIcon_PsynergyIconPointers[glyph];
	work->width = 2;
	work->height = 2;
	UiGlyph_DecodeWithHeapRoutines(work, slot);

	if (reuse == 0)
		*src = Resource_FindFreeEntry();

	*dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
}

void Ui_PrepareTransferFromTableEntry(u32 index)
{
	GlyphTransfer *state = gGlyphWork;

	state->encoded = UiIcon_MiscIconPointers[index];
	state->width = 2;
	state->height = 2;
	UiGlyph_DecodeWithHeapRoutines(state, 0);
}

void UiGlyph_LoadEntryWithPalette(
	u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse)
{
	GlyphTransfer *work;
	u16			  *table;
	u8			  *entry;
	u32			   index;

	work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	table = Resource_GetTableEntry((s32)&ResourceId_Icons);
	if (icon <= 127)
		index = icon;
	else
		index = icon - 112;
	entry = (u8 *)table + table[index];
	work->encoded = entry + 32;
	work->width = 4;
	work->height = 4;
	UiGlyph_DecodeWithHeapRoutines(work, 0);
	if (reuse == 0)
		*slot = Resource_FindFreeEntry();
	*tile = VramBlock_LoadCached(*slot, 0x200, work->tiles);
	Runtime_ReleaseHeapBlock(17);
	Dma_Set(entry, (void *)&OBJ_PLTT_COLOR(palette, 0), 0x80000010, REG_DMA3);
}

void UiGlyph_ReservedNoOp(void)
{
}

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *glyph, s32 outlined)
{
	RamCode_LoadSlot(ROUTINE_BLOCK, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
	((void (*)(const void *, u8 *))gWorkSlot[ROUTINE_BLOCK])(glyph->encoded, glyph->input);
	Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
	if (outlined)
		RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandMasked, Tile_ExpandMaskedCodeSize);
	else
		RamCode_LoadSlot(ROUTINE_BLOCK, Tile_ExpandOpaque, Tile_ExpandOpaqueCodeSize);
	((void (*)(u8 *, u8 *, u32, u32))gWorkSlot[ROUTINE_BLOCK])(
		glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
	Runtime_ReleaseHeapBlock(ROUTINE_BLOCK);
}

void UiGlyph_ResetWorkState(void)
{
	struct SelectionScreen *work;
	struct SelectionSprite *visual;
	const u8			   *tbl;
	s32						i;
	u16						clear = 0;

	work = Runtime_AllocateBlock(18, sizeof(struct SelectionScreen));
	work->head = NULL;
	work->path = NULL;
	work->window = NULL;
	work->unknown_39a = clear;
	work->top = clear;
	work->cursor_index = 128;
	work->vertical_scroll = 32;
	work->count = clear;
	work->locked = 999;
	i = 0;
	do
	{
		work->records[i + 2].kind = 0;
		work->records[i + 9].kind = 0;
		i++;
	} while (i != 5);
	ResetCursor((struct GlyphCursor *)&work->records[7].base);
	ResetCursor((struct GlyphCursor *)&work->records[8].base);
	work->records[0].kind = 0;
	work->records[1].kind = 0;
	work->records[0].y = 0;
	work->records[1].y = 0;
	tbl = Menu_AnimatedCursorTiles;
	work->records[14].slot = Resource_FindFreeEntry();
	work->records[14].tile = VramBlock_LoadCached(work->records[14].slot, 256, tbl);
	work->records[14].kind = 0;
	work->records[14].scale = 0;
	work->records[15].kind = 0;
	visual = &work->records[14].sprite;
	visual->mode = 0;
	visual->mosaic = 0;
	visual->colors = 1;
	visual->affine = 0;
	visual->param = 0;
	visual->size = 1;
	visual->shape = 0;
	visual->priority = 0;
}

void Resource_ClearOwnerListAndCounters(void)
{
	struct SelectionScreen *screen;

	screen = gResQueueWork;
	screen->head = NULL;
	screen->unknown_39a = 0;
	if (screen->cursor_index & 0x80)
	{
		screen->top = 0;
		screen->cursor_index = 0;
	}
	screen->vertical_scroll = 0;
	screen->count = 0;
}

void Resource_PushPendingPair(u32 first, u32 second)
{
	struct SelectionScreen *state = gResQueueWork;
	u16						cnt = state->count;

	if (cnt != 16)
	{
		state->kinds[cnt] = first;
		state->bases[cnt] = second;
		state->count++;
	}
}

void MenuSelection_BuildEntries(u32 unused)
{
	struct SelectionScreen *screen = gResQueueWork;
	u32						count = screen->count;
	u32						index = screen->top;
	struct SelectionNode   *prev = 0;
	struct SelectionNode   *node;
	s32						cnt = 0;

	while (index < count)
	{
		s32 base = screen->bases[index];
		u32 kind = screen->kinds[index];

		node = Resource_FindFreeTransferEntry(0);
		if (node == 0)
			break;
		MenuSelection_SetupEntry(kind, base, node, 0);
		if (screen->head == 0)
		{
			screen->head = node;
			node->prev = 0;
		}
		else
		{
			prev->next = node;
			node->prev = prev;
		}
		node->next = 0;
		cnt++;
		prev = node;
		if (cnt == 5)
			break;
		index++;
	}

	screen->base_x = 100 - cnt * 8;
	screen->base_y = 140;
	for (prev = screen->head, cnt = 0; prev != 0; prev = prev->next)
	{
		s32 x = screen->base_x + cnt;
		s32 y;

		prev->x = x;
		y = screen->base_y;
		prev->y = y;
		prev->target_x = x;
		prev->target_y = y;
		if (prev->kind == 6 && screen->locked == 0)
		{
			prev->y = 6;
			prev->target_y = 6;
		}
		prev->dx = 0;
		prev->dy = 0;
		cnt += 16;
	}
	Menu_LoadSelectedResource();
}

void UiWork_ReservedNoOp(void)
{
}

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind)
{
	struct SelectionScreen *state = gResQueueWork;
	s32						i;

	if (kind != 0)
	{
		for (i = 0; i != 5; i++)
		{
			if (state->records[i + 9].kind == 0)
				return &state->records[i + 9];
		}
	}
	else
	{
		for (i = 0; i != 7; i++)
		{
			if (state->records[i + 2].kind == 0)
				return &state->records[i + 2];
		}
	}
	return NULL;
}

void Resource_ScheduleOwnerResetDelayed(void)
{
	Scheduler_AddOrUpdateCallback(MenuSelection_DrawFrame, 0xC80);
}

void Resource_ScheduleOwnerReset(void)
{
	Scheduler_RemoveCallback(MenuSelection_DrawFrame);
}

void MenuSelection_DrawFrame(void)
{
	struct SelectionScreen *state = gResQueueWork;
	struct SelectionSprite *cursor_entry = &state->records[14].sprite;
	struct SelectionNode   *cursor = &state->records[14];
	struct SelectionNode   *transfer = &state->records[15];
	struct SelectionNode   *node;
	struct SelectionSprite *e;
	struct SelectionSprite *te;
	s32						cnt = 0;
	s32						slot;
	s32						tmp;
	struct EffectPosition	pos;

	node = state->head;
	while (node != 0)
	{
		e = &node->sprite;
		e->affine = 0;
		e->param = 0;
		e->x = node->x;
		e->y = node->y;
		slot = 240;
		if (state->vertical_scroll != 0)
		{
			e->y += state->vertical_scroll;
		}
		else if (node->x != node->target_x)
		{
			if (node->dx > 0)
			{
				if (node->x + node->dx > node->target_x)
				{
					node->x = node->target_x;
				}
				else
				{
					node->x += node->dx;
				}
			}
			else
			{
				if (node->x + node->dx < node->target_x)
				{
					node->x = node->target_x;
				}
				else
				{
					node->x += node->dx;
				}
			}
			e->x = node->x;
		}
		else if (cnt == state->cursor_index)
		{
			slot = 241;
			if (cursor->kind != 0)
			{
				if (BattleMotion_ProjectScaledPositionFar(node->base, &pos) != -1)
				{
					cursor->target_x = pos.x;
					cursor->target_y = pos.y;
					if (cursor->scale == 0)
					{
						cursor->x = pos.x;
						cursor->y = pos.y;
						cursor->scale = 1;
					}
				}
			}
		}
		if (node->scale != 0)
		{
			if (GameFlag_TestFar(0x103) != 0)
			{
				if (state->records[14].kind == 1)
				{
					e->mode = 1;
				}
				else
				{
					e->mode = 0;
				}
				if (node->kind == 1)
				{
					e->mode = 1;
				}
			}
			Runtime_PushSlotEntry((s32 *)e, slot);
		}
		node = node->next;
		cnt++;
	}
	if (transfer->kind != 0)
	{
		node = NodeChain_GetNodeAtCount(state);
		te = &transfer->sprite;
		te->mode = 0;
		te->affine = 0;
		te->param = 0;
		te->mosaic = 0;
		te->colors = 1;
		te->size = 2;
		te->shape = 0;
		te->priority = 0;
		te->tile = transfer->tile;
		te->x = node->x - 4;
		te->y = node->y + (Menu_CursorBobOffsets[(gFrameTick >> 1) & 15] >> 1) - 4;
		if (transfer->scale != transfer->scale_end)
		{
			state->matrix[0] = transfer->scale;
			state->matrix[1] = transfer->scale;
			state->matrix[2] = 0;
			te->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
			te->affine = 3;
			te->x += 0xfff0;
			te->y += 0xf0;
			transfer->scale += transfer->scale_step;
		}
		if (GameFlag_TestFar(0x103) != 0)
		{
			te->mode = 1;
		}
		Runtime_PushSlotEntry((s32 *)te, 248);
	}
	MenuSelection_DrawSideMarker(state, 0);
	MenuSelection_DrawSideMarker(state, 1);
	node = state->path;
	while (node != 0)
	{
		e = &node->sprite;
		if (node->x != node->target_x)
		{
			node->x += node->dx;
		}
		if (node->y != node->target_y)
		{
			node->y += node->dy;
		}
		e->x = node->x;
		e->y = node->y;
		if (node->scale != node->scale_end)
		{
			node->scale += node->scale_step;
			state->matrix[0] = node->scale;
			state->matrix[1] = node->scale;
			state->matrix[2] = 0;
			e->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
			e->affine = 3;
			e->x += 0xfff8;
			e->y += 0xf8;
		}
		else
		{
			e->affine = 0;
			e->param = 0;
		}
		if (GameFlag_TestFar(0x103) != 0)
		{
			if (state->records[14].kind == 1)
			{
				e->mode = 1;
			}
			else
			{
				e->mode = 0;
			}
			if (node->kind == 1)
			{
				e->mode = 1;
			}
		}
		Runtime_PushSlotEntry((s32 *)e, 240);
		node = node->next;
	}
	if (cursor->kind != 0)
	{
		cnt = (s32)Menu_AnimatedCursorTiles;
		{
			void *src = (void *)(cnt + (((gFrameTick >> 2) & 15) << 8));

			cursor_entry->tile = VramBlock_LoadCached(cursor->slot, 0x100, src);
		}
		if (cursor->target_x != cursor->x)
		{
			tmp = (cursor->target_x - cursor->x) >> 1;
			if (tmp != 0)
			{
				cursor->x += tmp;
			}
			else
			{
				cursor->x = cursor->target_x;
			}
		}
		if (cursor->target_y != cursor->y)
		{
			if ((tmp = (cursor->target_y - cursor->y) >> 1) != 0)
			{
				cursor->y += tmp;
			}
			else
			{
				cursor->y = cursor->target_y;
			}
		}
		cursor_entry->y = cursor->y + Menu_CursorBobOffsets[(gFrameTick >> 2) & 15] - 32;
		cursor_entry->x = cursor->x - 4;
		if (GameFlag_TestFar(0x103) != 0)
		{
			if (state->records[14].kind == 1)
			{
				cursor_entry->mode = 1;
			}
			else
			{
				cursor_entry->mode = 0;
			}
		}
		Runtime_PushSlotEntry((s32 *)cursor_entry, 248);
	}
	state->status += 1;
}

void MenuSelection_DrawSideMarker(struct SelectionScreen *state, s32 index)
{
	u32						frame = (gFrameTick >> 2) & 7;
	struct SelectionSprite *entry;
	u8					   *frames;
	u8					   *tmp;

	if (state->records[index].kind == 0)
		return;
	entry = &state->records[index].sprite;
	entry->x = state->records[index].x;
	entry->y = state->records[index].y;
	if (index != 0)
	{
		frames = Menu_CursorObjectTiles;
		if (state->records[1].base != 0)
			entry->x = entry->x + state->records[1].base;
	}
	else
	{
		frames = Menu_CursorLeftObjectTiles;
		if (state->records[0].base != 0)
			entry->x = entry->x - state->records[0].base;
	}
	tmp = frames + frame * 128;
	entry->tile = VramBlock_LoadCached(state->records[index].slot, 128, tmp);
	if (GameFlag_TestFar(0x103))
	{
		if (state->records[14].kind == 1)
			entry->mode = 1;
		else
			entry->mode = 0;
	}
	Runtime_PushSlotEntry((s32 *)entry, 238);
	if (state->records[index].base != 0)
		state->records[index].base--;
}

void Menu_OpenSelectionWindow(s32 mode, u32 count)
{
	struct SelectionScreen *screen;
	struct SelectionNode   *node;
	struct UiWindow		  **slot;
	struct UiWindow		   *window;

	screen = gResQueueWork;
	node = NodeChain_GetNodeAtCount(screen);
	slot = &screen->window;
	window = *slot;
	if (window == 0)
	{
		if (mode == 6)
		{
			if (screen->locked != 0)
			{
				*slot = UiWindow_Create(17, 17, 5, 3, mode);
			}
			else
			{
				*slot = UiWindow_Create(17, 0, 5, 3, mode);
			}
			screen->vertical_scroll = 0;
			screen->locked = 999;
		}
		else
		{
			*slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
		}
		RenderOutput_PrepareForRedraw(screen->window);
	}
	else
	{
		if (count != 0 && window->width != count + 2)
		{
			UiWork_Finalize(window, 2);
			*slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
		}
		RenderOutput_PrepareForRedraw(screen->window);
	}
	if (screen->count != 0)
	{
		UiText_DrawCharacterAtOffset(node->message, screen->window, 0, 0);
	}
	else
	{
		switch (mode)
		{
		case 4:
			UiText_DrawCharacterAtOffset((s32)MsgAbilityNotKnown, screen->window, 0, 0);
			break;
		case 2:
			UiText_DrawCharacterAtOffset((s32)MsgItemNotHeld, screen->window, 0, 0);
			break;
		}
	}
}

void Resource_ResetOwnerEntries(void)
{
	struct SelectionScreen *state = gResQueueWork;
	struct SelectionNode   *node;

	Resource_ScheduleOwnerReset();
	UiWork_Finalize(state->window, 2);
	WaitFrames(1);
	node = state->head;
	while (node != 0)
	{
		if (node->kind != 0)
		{
			Resource_ResetEntry(node->slot);
			node->kind = 0;
		}
		node = node->next;
	}
	node = state->path;
	while (node != 0)
	{
		if (node->kind != 0)
		{
			Resource_ResetEntry(node->slot);
			node->kind = 0;
		}
		node = node->next;
	}
	Resource_ResetPendingTransfer();
	if (state->records[0].y != 0)
	{
		Resource_ResetEntry(state->records[0].slot);
		if (state->records[0].y != 0)
		{
			Resource_ResetEntry(state->records[1].slot);
		}
	}
	Resource_ResetEntry(state->records[14].slot);
	Runtime_ReleaseHeapBlock(18);
}

void Menu_SetNodeCoordinates(u32 first, u32 second)
{
	struct SelectionScreen *state = gResQueueWork;
	struct SelectionNode   *node;

	state->base_x = first;
	state->base_y = second;
	node = state->head;
	while (node != 0)
	{
		node->x = first;
		node->target_x = first;
		node->y = second;
		node->target_y = second;
		node = node->next;
		first += 16;
	}
}

void Menu_SetupSelectionBothSides(void)
{
	struct SelectionScreen *state;

	state = gResQueueWork;
	Menu_SetupSelectionSide(state, 0);
	Menu_SetupSelectionSide(state, 1);
}

void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index)
{
	struct SelectionSprite *entry = &state->records[index].sprite;
	u8					   *frames = 0;
	u32						count;

	state->records[index].kind = 0;
	if (index != 0)
	{
		/* FAKEMATCH: the separate local preserves the early r5 load and late copy; direct assignment changes later node registers. */
		u8 *right = Menu_CursorObjectTiles;

		count = state->count;
		if (state->top != 0)
			count -= state->top;
		if (count > 5)
		{
			state->records[index].kind = 1;
			count = 5;
		}
		frames = right;
		state->records[1].x = state->base_x + 16 * (count - 1) + 17;
	}
	else
	{
		frames = Menu_CursorLeftObjectTiles;
		state->records[0].x = state->base_x - 9;
		if (state->top != 0)
			state->records[0].kind = 1;
	}
	if (state->records[index].y == 0)
	{
		state->records[index].slot = Resource_FindFreeEntry();
		state->records[index].tile = VramBlock_LoadCached(state->records[index].slot, 128, frames);
		state->records[index].y = state->base_y;
		state->records[index].base = 0;
		entry->mode = 0;
		entry->mosaic = 0;
		entry->colors = 1;
		entry->affine = 0;
		entry->param = 0;
		entry->size = 0;
		entry->shape = 2;
		entry->priority = 0;
	}
}

//--------------------------------------------------------------------------
// get_node_at_count.c
// メニュー：つながったノードの count 番目をさがす
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// st->node から next を st->count 回たどったノードを返す
//--------------------------------------------------------------------------
struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *st)
{
	struct NodeChainNode *node = st->node;
	s32					  i;

	for (i = 0; i != st->count; ++i)
	{
		node = node->next;
	}
	return node;
}

#define SELECTION_FIXED 999

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

#define NODE_KIND_YES_NO 6

#define SELECTION_ROWS 4
#define ARROW_SCROLL_FRAMES 8

s32 Menu_SelectionLoop(s32 mode)
{
	struct SelectionScreen *screen = gResQueueWork;

	Menu_LoadSelectionNodeResource(screen, 0);
	for (;;)
	{
		WaitFrames(1);
		if (screen->vertical_scroll != 0)
		{
			continue;
		}
		if (mode != SELECTION_FIXED)
		{
			if (gKeysRepeat & KEY_RIGHT)
			{
				Menu_StepRight(screen);
			}
			else if (gKeysRepeat & KEY_LEFT)
			{
				Menu_StepLeft(screen);
			}
			else if (gKeyState & KEY_A)
			{
				return Menu_ConfirmSelection(screen);
			}
		}
		if (mode != 0 && (gKeyState & KEY_B))
		{
			return -1;
		}
	}
}

u32 Menu_WaitForSelectionInput(u32 mode)
{
	struct SelectionScreen *screen = gResQueueWork;
	u32						result;

	for (;;)
	{
		WaitFrames(1);
		if (screen->vertical_scroll != 0)
		{
			continue;
		}

		if (mode != SELECTION_FIXED)
		{
			if (gKeysRepeat & KEY_RIGHT)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				Menu_StepRight(screen);
			}
			else if (gKeysRepeat & KEY_LEFT)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				Menu_StepLeft(screen);
			}

			if (gKeyState & KEY_A)
			{
				result = screen->top + screen->cursor_index;
				if (screen->head->kind == NODE_KIND_YES_NO)
				{
					if (result == 0)
						AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
					else
						AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				}
				else
				{
					AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
				}
				return result;
			}
		}

		if (mode != 0 && (gKeyState & KEY_B))
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			return -1;
		}
	}
}

void Menu_MoveSelectionForward(struct SelectionScreen *screen)
{
	u32 end = screen->top + screen->cursor_index + 1;

	if (end == screen->count)
	{
		return;
	}
	Menu_ReloadNodeResource(screen, screen->cursor_index);
	screen->status = NODE_STATUS_SCROLLING;
	WaitFrames(1);
	screen->cursor_index++;
	if (screen->cursor_index == SELECTION_ROWS && end + 1 < screen->count)
	{
		screen->cursor_index--;
		screen->records[1].base = ARROW_SCROLL_FRAMES;
		screen->top++;
		Menu_ScrollSelectionList(screen, TRUE);
		if (screen->top + screen->cursor_index + 2 == screen->count)
		{
			screen->records[1].kind = FALSE;
		}
		screen->records[0].kind = TRUE;
	}
	screen->status = NODE_STATUS_SHOWN;
	Menu_LoadSelectionNodeResource(screen, screen->cursor_index);
	WaitFrames(1);
	Menu_OpenSelectionWindow(screen->head->kind, 0);
	WaitFrames(1);
}

void Menu_MoveSelectionBackward(struct SelectionScreen *screen)
{
	u32 cursor;

	if (screen->top == 0 && screen->cursor_index == 0)
	{
		return;
	}
	Menu_ReloadNodeResource(screen, screen->cursor_index);
	screen->status = NODE_STATUS_SCROLLING;
	WaitFrames(1);
	cursor = screen->cursor_index;
	if (cursor == 1 && screen->top != 0)
	{
		screen->records[0].base = ARROW_SCROLL_FRAMES;
		screen->top--;
		Menu_ScrollSelectionList(screen, FALSE);
		if (screen->top == 0)
		{
			screen->records[0].kind = FALSE;
		}
		screen->records[1].kind = cursor;
	}
	else
	{
		screen->cursor_index--;
	}
	screen->status = NODE_STATUS_SHOWN;
	Menu_LoadSelectionNodeResource(screen, screen->cursor_index);
	WaitFrames(1);
	Menu_OpenSelectionWindow(screen->head->kind, 0);
	WaitFrames(1);
}

void Menu_StepRight(struct SelectionScreen *state)
{
	struct SelectionNode *node;
	u16					 *ids;
	s32					  y;
	u32					  end;

	Menu_ReloadNodeResource(state, state->cursor_index);
	state->status = 33;
	WaitFrames(1);
	state->cursor_index++;
	if (state->count > 5)
	{
		end = state->top + state->cursor_index;
		if (end == state->count)
		{
			node = state->head;
			state->records[0].kind = 0;
			while (node->next != NULL)
			{
				node->target_x = state->base_x;
				node->target_y = state->base_y;
				node->dx = -12;
				node = node->next;
			}
			node->target_x = state->base_x;
			node->target_y = state->base_y;
			node->dx = -12;
			while (node->target_x != node->x)
				WaitFrames(1);
			node = state->head;
			if (node != NULL)
			{
				ids = state->kinds;
				do
				{
					MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
					node = node->next;
					ids++;
				} while (node != NULL);
			}
			state->cursor_index = 0;
			state->top = 0;
			node = state->head;
			y = node->x + 16;
			node = node->next;
			while (node != NULL)
			{
				node->target_x = y;
				node->dx = 12;
				node = node->next;
				y += 16;
			}
			state->records[1].kind = 1;
		}
		else if (state->cursor_index == 4 && end + 1 < state->count)
		{
			state->cursor_index--;
			state->records[1].base = 8;
			state->top++;
			Menu_ScrollSelectionList(state, 1);
			if (state->top + state->cursor_index + 2 == state->count)
				state->records[1].kind = 0;
			state->records[0].kind = 1;
		}
	}
	else if (state->cursor_index == state->count)
	{
		state->cursor_index = 0;
	}
	state->status = 1;
	Menu_LoadSelectionNodeResource(state, state->cursor_index);
	WaitFrames(1);
	Menu_OpenSelectionWindow(state->head->kind, 0);
	WaitFrames(1);
}

void Menu_StepLeft(struct SelectionScreen *state)
{
	struct SelectionNode *node;
	s32					  i;

	Menu_ReloadNodeResource(state, state->cursor_index);
	state->status = 33;
	WaitFrames(1);
	if (state->count > 5)
	{
		if ((state->cursor_index | state->top) != 0)
		{
			if (state->cursor_index == 1 && state->top != 0)
			{
				state->records[0].base = 8;
				state->top--;
				Menu_ScrollSelectionList(state, 0);
				if (state->top == 0)
					state->records[0].kind = 0;
				state->records[1].kind = 1;
			}
			else
			{
				state->cursor_index--;
			}
		}
		else
		{
			node = state->head;
			i = 64;
			{ register s32 z asm("r0") = 0; /* FAKEMATCH: the zero goes through r0. 2026-10-11: CSE follows the beq into this block and reuses the known-zero OR register for a plain store (differ 28); only a loop-end note before the else label stops it, and a fresh zero then takes r3 (differ 4); r0 needs the zero live across the first loop, which no instruction-free second use gives (a second store after the loop: differ 13). */
				state->records[1].kind = z;
			}
			while (node->next != NULL)
			{
				node->target_x = node->x + i;
				node->dx = 12;
				node = node->next;
				i -= 16;
			}
			node = state->head;
			while (node->x != node->target_x)
				WaitFrames(1);
			i = 0;
			while (i != state->count - 5)
				i++;
			node = state->head;
			state->top = i;
			state->cursor_index = 4;
			while (node != NULL)
			{
				MenuSelection_SetupEntry(state->kinds[i], state->bases[i], node, 1);
				node = node->next;
				i++;
			}
			node = state->head;
			i = state->base_x;
			while (node->next != NULL)
			{
				node->target_x = i;
				node->dx = -12;
				node = node->next;
				i += 16;
			}
			state->records[0].kind = 1;
		}
	}
	else if (state->cursor_index != 0)
	{
		state->cursor_index--;
	}
	else
	{
		state->cursor_index = state->count - 1;
	}
	state->status = 1;
	Menu_LoadSelectionNodeResource(state, state->cursor_index);
	WaitFrames(1);
	Menu_OpenSelectionWindow(state->head->kind, 0);
	WaitFrames(1);
}

void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index)
{
	struct SelectionNode *node = state->head;
	u32					  output;
	u32					  value;

	while (index != 0)
	{
		index--;
		node = node->next;
	}
	if (node->kind == 1 || node->kind == 6)
	{
		u32 first = node->message - (u32)MsgCommandName;

		value = node->slot;
		Ui_BuildPairedPatternsToSlot(first, 0, &value, &output, 1);
	}
}

void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index)
{
	struct SelectionNode *node = state->head;
	u32					  res;
	u32					  value;

	while (index != 0)
	{
		index--;
		node = node->next;
	}
	if (node->kind == 1 || node->kind == 6)
	{
		u32 id = node->message - (u32)MsgCommandName;

		value = node->slot;
		Ui_BuildPairedPatternsToSlot(id, 0, &value, &res, 1);
		Menu_LoadSelectedResource();
	}
}

void Menu_SendNodeCountList(struct SelectionScreen *screen)
{
	u16					  data[6];
	struct SelectionNode *node = screen->head;
	s32					  count = 0;

	while (node != 0)
	{
		node = node->next;
		count++;
	}
	data[count] = 0xff;
	BattlePres_SetActorModesFar(data, 0);
}

void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward)
{
	struct SelectionNode *p;
	struct SelectionNode *last;
	struct SelectionNode *first;
	s32					  base;
	s32					  kind;
	u32					  index;
	s32					  y;
	s32					  z;

	if (forward != 0)
	{
		index = screen->top + SELECTION_ROWS;
		base = screen->bases[index];
		kind = screen->kinds[index];
		p = Resource_FindFreeTransferEntry(0);
		if (p == NULL)
			return;
		MenuSelection_SetupEntry(kind, base, p, 0);
		y = screen->base_x;
		p->x = y + 80;
		z = screen->base_y;
		p->target_x = y + 64;
		p->y = z;
		p->target_y = z;
		p->scale_step = 32;
		p->scale = 32;
		p->scale_end = 256;
		p->dx = -2;
		last = p;
		p = screen->head;
		p->scale_step = -32;
		p->scale_end = 0;
		for (;;)
		{
			p->target_x = p->x - 16;
			p->dx = -2;
			if (p->next == NULL)
				break;
			p = p->next;
		}
		p->next = last;
		last->next = NULL;
		last->prev = p;
		p = screen->head;
		do
		{
			WaitFrames(1);
		} while (p->scale != 0);
		screen->head = p->next;
		Resource_ResetEntry(p->slot);
		p->kind = 0;
		p = p->next;
		p->prev = NULL;
	}
	else
	{
		index = screen->top;
		base = screen->bases[index];
		kind = screen->kinds[index];
		p = Resource_FindFreeTransferEntry(0);
		if (p == NULL)
			return;
		MenuSelection_SetupEntry(kind, base, p, 0);
		p->x = screen->base_x - 16;
		p->y = screen->base_y;
		p->target_y = p->y;
		p->dx = 2;
		p->scale = 32;
		p->scale_step = 32;
		p->target_x = p->x + 16;
		p->scale_end = 256;
		first = p;
		p = screen->head;
		p->prev = first;
		first->next = p;
		first->prev = NULL;
		screen->head = first;
		p = first;
		for (;;)
		{
			p->target_x = p->x + 16;
			p->dx = 2;
			if (p->next == NULL)
				break;
			p = p->next;
		}
		p->scale_end = 0;
		p->scale_step = -32;
		p = screen->head;
		do
		{
			WaitFrames(1);
		} while (p->scale != 256);
		while (p->next != NULL)
			p = p->next;
		Resource_ResetEntry(p->slot);
		p->kind = 0;
		p->prev->next = NULL;
	}
}

s32 Resource_LoadByMode(s32 mode, s32 value)
{
	s32 output;
	s32 result = -1;

	switch (mode)
	{
	case 1:
	case 6:
		Ui_BuildPairedPatternsToSlot(value, 0, &result, &output, 0);
		break;
	case 2:
		result = Resource_FindFreeEntry();
		if (result == VRAM_CACHE_ENTRY_COUNT)
			return -1;
		UiIcon_CopyResourceToSlot(value, 0x1a, result);
		break;
	case 9:
		result = Resource_FindFreeEntry();
		if (result == VRAM_CACHE_ENTRY_COUNT)
			return -1;
		RenderResource_LoadTableEntry(value, 0, result);
		break;
	case 4:
		Ability_LoadGlyph(value, 1, &result, &output, 0);
		break;
	}
	return result;
}

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option)
{
	s32 output;
	s32 original = result;

	if (result == -1)
	{
		result = Resource_FindFreeEntry();
		if (result == VRAM_CACHE_ENTRY_COUNT)
			return original;
	}

	switch (mode)
	{
	case 1:
	case 6:
		Ui_BuildPairedPatternsToSlot(value, option, &result, &output, 1);
		break;
	case 2:
		UiIcon_CopyResourceToSlot(value, 58, result);
		break;
	case 7:
		UiIcon_CopyResourceToSlot(value, 42, result);
		break;
	case 4:
		Ability_LoadGlyph(value, option, &result, &output, 1);
		break;
	case 8:
		Ui_BuildPatternToSlot(value, 0, result);
		break;
	case 9:
		RenderResource_LoadTableEntry(value, 0, result);
		break;
	}

	return result;
}

void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse)
{
	s32						src;
	s32						tile;
	struct SelectionSprite *obj;

	switch (kind)
	{
	case 1:
	case 6:
		if (reuse != 0)
			src = node->slot;
		Ui_BuildPairedPatternsToSlot(base, 0, &src, &tile, reuse);
		node->message = base + (s32)MsgCommandName;
		break;
	case 2:
		if (reuse != 0)
			src = node->slot;
		UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
		node->message = base + (s32)MsgItemName;
		break;
	case 4:
		if (reuse != 0)
			src = node->slot;
		Ability_LoadGlyph(base, 1, &src, &tile, reuse);
		node->message = base + (s32)MsgAbilityName;
		break;
	}

	node->base = base;
	node->slot = src;
	node->tile = tile;
	node->kind = kind;
	node->scale = 0x100;
	node->scale_end = 0x100;

	obj = &node->sprite;
	obj->mode = 0;
	obj->colors = 0;
	obj->mosaic = 0;

	obj->size = 1;
	obj->shape = 0;
	obj->palette = 0;
	obj->tile = tile;
	obj->priority = 0;
}

u32 Menu_ConfirmSelection(struct SelectionScreen *screen)
{
	struct SelectionNode   *node;
	struct SelectionNode   *other = NULL;
	u32						cnt = 0;
	u32						index;
	struct SelectionSprite *sprite;

	index = screen->top + screen->cursor_index;
	Menu_SendNodeCountList(screen);
	Menu_ReloadNodeResource(screen, screen->cursor_index);
	screen->status = 33;
	WaitFrames(1);
	screen->records[0].kind = 0;
	screen->records[1].kind = 0;
	screen->records[14].kind = 0;
	screen->records[14].scale = 0;
	Resource_ResetPendingTransfer();
	node = screen->head;
	while (node != NULL && screen->cursor_index != cnt)
	{
		node = node->next;
		cnt++;
	}
	node->home_x = node->x;
	node->home_y = node->y;
	for (other = screen->head; other != NULL; other = other->next)
	{
		if (other != node)
		{
			other->target_x = node->x;
			other->dx = (node->x - other->x) >> 1;
		}
	}
	WaitFrames(2);
	for (other = screen->head; other != NULL; other = other->next)
	{
		if (other != node)
		{
			Resource_ResetEntry(other->slot);
			other->kind = 0;
		}
	}
	screen->head = node;
	node->prev = NULL;
	node->next = NULL;
	node->target_x = 4;
	for (other = screen->path, cnt = 0; other != NULL; other = other->next)
	{
		node->target_x += 16;
		cnt++;
	}
	screen->path_top[cnt] = screen->top;
	screen->path_cursor[cnt] = screen->cursor_index;
	node->dx = (node->target_x - node->x) >> 1;
	screen->unknown_39a = 0;
	screen->cursor_index |= 0x80;
	WaitFrames(2);
	other = Resource_FindFreeTransferEntry(1);
	other->kind = node->kind;
	other->message = node->message;
	other->base = node->base;
	other->slot = node->slot;
	other->tile = node->tile;
	other->x = node->x;
	other->y = node->y;
	other->target_x = other->x;
	other->target_y = other->y;
	other->home_x = node->home_x;
	other->home_y = node->home_y;
	other->dx = 0;
	other->dy = 0;
	other->scale = 0x100;
	other->scale_end = 0x100;
	sprite = &other->sprite;
	sprite->mode = 0;
	sprite->colors = 0;
	sprite->mosaic = 0;
	sprite->size = 1;
	sprite->shape = 0;
	sprite->palette = 0;
	sprite->tile = other->tile;
	node->kind = 0;
	screen->head = NULL;
	if (screen->path != NULL)
	{
		node = screen->path;
		while (node->next != NULL)
			node = node->next;
		node->next = other;
		other->prev = node;
		other->next = NULL;
	}
	else
	{
		screen->path = other;
		other->prev = NULL;
		other->next = NULL;
	}
	return index;
}

void UiTextResource_NoOpCallback(void)
{
}

void UiTextResource_NoOpCallbackBank(void)
{
}

void UiTextResource_NoOpCallback1(void)
{
}

void UiTextResource_NoOpCallback2(void)
{
}

void UiTextResource_NoOpCallback3(void)
{
}

void UiTextResource_NoOpCallback4(void)
{
}

void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot)
{
	const void *data = Menu_CursorObjectTiles;
	s32			value = Resource_FindFreeEntry();

	*slot = value;
	object->oam.f.tile = VramBlock_LoadCached(value, 0x80, data);
	object->oam.f.mode = 0;
	object->oam.f.mosaic = 0;
	object->oam.f.colors = 1;
	object->oam.f.affine = 0;
	object->oam.f.affine_index = 0;
	object->oam.f.size = 0;
	object->oam.f.shape = 2;
	object->oam.f.priority = 0;
}

void UiTextResource_SetPosition(struct MenuSprite *obj, s32 x, s32 y)
{
	obj->oam.f.x = x;
	obj->oam.f.y = y;
	Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}

void UiTextResource_Release(s32 value)
{
	Resource_ResetEntry(value);
}

void Menu_LoadSelectedResource(void)
{
	struct SelectionScreen *state = gResQueueWork;
	struct SelectionNode   *selection = NodeChain_GetNodeAtCount(state);
	struct SelectionNode   *transfer;
	GlyphTransfer		   *buffer;
	u8					   *tbl;
	void				   *resource;
	s32						no;

	if (selection->kind != 1 && selection->kind != 6)
		return;

	buffer = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));
	transfer = &state->records[15];
	no = selection->base;
	tbl = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
	{
		u8 **destination = &buffer->encoded;
		resource = tbl + *(u16 *)(tbl + selection->base * 2);
		*destination = resource;
	}
	Resource_DecodeByteLz(resource, buffer);

	if (transfer->kind == 0)
		transfer->slot = Resource_FindFreeEntry();
	transfer->tile = VramBlock_LoadCached(transfer->slot, 0x400, buffer);
	transfer->kind = 1;
	transfer->base = no;
	transfer->scale = 40;
	transfer->scale_step = 40;
	transfer->scale_end = 240;
	Runtime_ReleaseHeapBlock(17);
}

void Resource_ResetPendingTransfer(void)
{
	struct SelectionNode *transfer = &gResQueueWork->records[15];

	if (transfer->kind != 0)
	{
		Resource_ResetEntry(transfer->slot);
		transfer->kind = 0;
	}
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)

#define HAS_LOCALIZED_MENU_GUARD 1
#endif

s32 Menu_RunTopSelection(void)
{
	s32 ret;
	s32 sel;
	u8 *state;

	state = (u8 *)gEventWork;
	sel = 0;

loop:
	UiWindow_OpenMode1AndWaitFrame();
	sel = Menu_SelectTopEntry(sel);
#if defined(HAS_LOCALIZED_MENU_GUARD)
	state[0xcca] = 1;
	if (*(s16 *)(state + 0xcb8) != 0)
	{
		BattleFx_ClearRandomParticlesFar();
		WaitFrames(1);
	}
#endif
	ret = UiWork_CloseAndRelease();

	switch (sel)
	{
	case 0:
		ret = Object_GetTriggerTileAheadOfCurrentFar();
		if (ret == 0)
			ret = 0xff;
		((struct EventWork *)state)->facing_trigger = ret;
		break;
	case 1:
		ret = Menu_OpenConfirmPromptFar();
		if (ret == -1)
			goto loop;
		break;
	case 2:
		ret = ItemMenu_RunFar();
		if (ret != 0)
			goto loop;
		break;
	case 3:
		ret = RunAssetSelectionScreenFar();
		if (ret == -1)
			goto loop;
		break;
	case 4:
		ret = Menu_OpenActionFlow();
		if (ret == -1)
			goto loop;
		break;
	default:
		break;
	}

#if defined(HAS_LOCALIZED_MENU_GUARD)
	state[0xcca] = 0;
#endif
	return ret;
}

void UiWindow_OpenMode1AndWaitFrame(void)
{
	UiWindow_CreateWithLayoutBounds(1);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// runtime_close_and_release.c
// ウィンドウ：ヒープ 16 番のウィンドウを閉じてブロックを返す
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ヒープ 16 番のウィンドウを閉じ、ブロックを返す
//--------------------------------------------------------------------------
void UiWork_CloseAndRelease(void)
{
	UiWork_FinalizeAndReleaseBlock16();
}

struct TimedNoticeWork
{
	u8				 unknown_000[0x230];
	struct UiWindow *window;
	u16				 countdown;
};

void Ui_ClearWorkStateAndWaitFrame(void)
{
	UiGlyph_ResetWorkState();
	Resource_ClearOwnerListAndCounters();
	WaitFrames(1);
}

u32 Menu_RunSelectionForValue(u32 value)
{
	struct SelectionScreen *state = gResQueueWork;
	u32						result;

	state->cursor_index = value;
	state->locked = 1;
	MenuSelection_BuildEntries(value);
	Menu_SetupSelectionBothSides();
	Menu_OpenSelectionWindow(0, 5);
	Resource_ScheduleOwnerResetDelayed();
	result = Menu_WaitForSelectionInput(1);
	Resource_ResetOwnerEntries();
	return result;
}

void UiTimedNotice_Create(void)
{
	struct TimedNoticeWork *work;
	s32						height;
	s32						width;
	s32						y;
	s32						x;
	s32						resource;
	struct UiWindow		   *window;

	work = (struct TimedNoticeWork *)gEventWork;
	x = 8;
	y = 8;
	resource =
		Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance) + RENDER_RESOURCE_BASE;
	UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
	x = (30 - width) >> 1;
	y = (10 - height) >> 1;
	window = UiWindow_Create(x, y, width, height, 2);
	work->window = window;
	UiText_DrawResource(resource, (s32)window, 0, 0);
	work->countdown = 90;
	Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0xc80);
}

void UiTimedNotice_Tick(void)
{
	struct TimedNoticeWork *work;
	u16						count;

	work = (struct TimedNoticeWork *)gEventWork;
	work->countdown = (count = work->countdown + 0xffff);
	if ((count << 16) == 0)
	{
		UiWork_Finalize(work->window, 2);
		Scheduler_RemoveCallback(UiTimedNotice_Tick);
	}
}

void UiTimedNotice_CloseIfActive(void)
{
	struct UiWindow *window;

	window = ((struct TimedNoticeWork *)gEventWork)->window;
	if (window != NULL && window->flags != 0)
	{
		UiWork_Finalize(window, 2);
		Scheduler_RemoveCallback(UiTimedNotice_Tick);
	}
}

s32 Item_CallHandler48(s32 arg0, s32 arg1)
{
	PartyInventory_RemoveFar(arg1);
	return 0;
}

s32 Item_ReturnTrue(void)
{
	return 1;
}

void Party_AdjustPaletteGlowPhase(s32 arg0)
{
	u8 value = gGameState.palette_glow[0];
	if (arg0 & 0x20)
		value += 0xff;
	else
		value += 1;
	gGameState.palette_glow[0] = value;
}

struct AbilityListEntry
{
	u16 owner;
	u16 ability;
};

void Debug_SelectAbilityPair(void)
{
	struct AbilityListEntry *list;
	struct RenderInput		*win;
	struct RenderInput		*title;
	struct RenderInput		*info;
	struct RenderOutput		*cursor;
	u32						 first;
	u32						 second;
	u32						 count;
	s32						 done = 0;
	s32						 row = 0;
	s32						 redraw;
	s32						 slot;
	s32						 y;
	s32						 cost;
	s32						 ability;

	cursor = NULL;
	redraw = 1;
	list = MemAlloc(0x700);
	OwnerAction_AddFar(0, 140);
	OwnerAction_AddFar(1, 140);
	OwnerAction_AddFar(2, 140);
	OwnerAction_AddFar(2, 141);
	OwnerAction_AddFar(2, 78);
	OwnerAction_AddFar(3, 93);
	OwnerAction_AddFar(5, 140);
	second = 0;
	first = 0;
	count = Object_CollectResources(list);
	if (count != 0)
	{
		Menu_FindShortcutEntries(&first, &second, list);
		win = UiWindow_Create(4, 6, 20, 7, 2);
		title = UiWindow_Create(4, 3, 20, 3, 2);
		info = UiWindow_Create(4, 14, 20, 5, 2);
		slot = Resource_FindFreeEntry();
		if (slot != 0)
		{
			VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
			cursor = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
		}
		/* FAKEMATCH: needed to match: the title message reuses the slot variable */
		slot = (s32)MsgPanelHpMaxLabel;
		UiText_DrawCharacterAtOffset(slot, title, 16, 0);
		for (;;)
		{
			if (redraw != 0)
			{
				redraw = 0;
				first = (first + count) % count;
				second = (second + count) % count;
				row = (row + 2) % 2;
				y = (row << 4) + (win->y << 3) + 28;
				cursor->y = y;
				cursor->oam.y = y;
				RenderOutput_RedrawSavedRect(win);
				UiWindow_DrawDividerLine(win, 1, 2, 17, 2);
				UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel, win, 48, 0);
				UiText_DrawCharacterAtOffset(
					list[first].ability + (s32)MsgAbilityName, win, 56, 16);
				UiText_DrawCharacterAtOffset(
					list[second].ability + (s32)MsgAbilityName, win, 56, 32);
				UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel - 2, win, 16, 16);
				UiText_DrawCharacterAtOffset((s32)MsgPanelStatusLabel - 1, win, 16, 32);
				UiText_DrawCharacterAtOffset(
					list[first].owner + (s32)MsgCharacterName, win, 104, 16);
				UiText_DrawCharacterAtOffset(
					list[second].owner + (s32)MsgCharacterName, win, 104, 32);
				RenderOutput_RedrawSavedRect(info);
				UiText_DrawCharacterAtOffset((s32)MsgPsynergyNeedMorePp, info, 0, 16);
				if (row != 0)
				{
					cost = BtlActGet(list[second].ability)->pp_cost;
					ability = list[second].ability;
				}
				else
				{
					cost = BtlActGet(list[first].ability)->pp_cost;
					ability = list[first].ability;
				}
				UiText_DrawNumberAtOffset(cost, 2, info, 64, 16);
				UiText_DrawCharacterAtOffset(ability + (s32)MsgAbilityDescription, info, 0, 0);
			}
			WaitFrames(1);
			if (gKeysRepeat & KEY_LEFT)
			{
				AudioCommand_PlayFar(111);
				if (row != 0)
					second--;
				else
					first--;
				redraw = 1;
			}
			if (gKeysRepeat & KEY_RIGHT)
			{
				AudioCommand_PlayFar(111);
				if (row != 0)
					second++;
				else
					first++;
				redraw = 1;
			}
			if (gKeysRepeat & KEY_UP)
			{
				AudioCommand_PlayFar(111);
				row--;
				redraw = 1;
			}
			if (gKeysRepeat & KEY_DOWN)
			{
				AudioCommand_PlayFar(111);
				row++;
				redraw = 1;
			}
			if (gKeyState & KEY_A)
				AudioCommand_PlayFar(112);
			else if (gKeyState & KEY_B)
				AudioCommand_PlayFar(113);
			else if (gKeyState & KEY_START)
				AudioCommand_PlayFar(113);
			else
				continue;
			/* FAKEMATCH: the unread flag forces fresh zeros for both entries and the cursor argument instead of reusing the row register. */
			done = 1;
			break;
		}
		gGameState.first_shortcut = (list[first].owner << 10) | list[first].ability;
		gGameState.second_shortcut = (list[second].owner << 10) | list[second].ability;
		UiWork_Finalize(win, 1);
		UiWork_Finalize(title, 1);
		UiWork_Finalize(info, 1);
		WaitFrames(1);
	}
	MemFree(list);
}

#define ACTION_ID_MASK 0x3fff

s32 Object_CollectResources(struct AbilityListEntry *output)
{
	u16 object_ids[14];
	s32 output_count = 0;
	s32 object_count = Party_ListActiveOwnersFar(object_ids);
	s32 i;

	for (i = 0; i < object_count; i++)
	{
		struct BtlUnit *object;
		u32				id;
		s32				index;
		u32				resource_id;

		id = object_ids[i];
		object = UnitGetFar(id);
		for (index = 0;
			index < (s32)(sizeof(object->action_slots) / sizeof(object->action_slots[0])); index++)
		{
			resource_id = object->action_slots[index].encoded_action & ACTION_ID_MASK;
			if (resource_id == 0)
				break;
			BtlActGet(resource_id);
			output[output_count].owner = id;
			output[output_count].ability = resource_id;
			output_count++;
		}
	}

	return output_count;
}

void Menu_FindShortcutEntries(
	u32 *first_index, u32 *second_index, const struct AbilityListEntry *entries)
{
	s32 i;

	*first_index = 0;
	*second_index = 0;

	for (i = 0; i <= 447; i++)
	{
		if (entries[i].ability == (gGameState.first_shortcut & 0x3ff)
			&& entries[i].owner == (gGameState.first_shortcut >> 10))
		{
			*first_index = i;
			break;
		}
	}

	for (i = 0; i <= 447; i++)
	{
		if (entries[i].ability == (gGameState.second_shortcut & 0x3ff)
			&& entries[i].owner == (gGameState.second_shortcut >> 10))
		{
			*second_index = i;
			break;
		}
	}
}

void Menu_RunSelectionHook(void)
{
	Debug_SelectAbilityPair();
}

struct SelectionWorkspace
{
	u8				 unknown_000[0x46];
	u16				 first_active;
	u16				 first_resource;
	u8				 unknown_04a[0x352 - 0x4a];
	u16				 second_active;
	u16				 second_resource;
	u8				 unknown_356[0xff4 - 0x356];
	struct UiWindow *window;
	u8				 unknown_ff8[12];
};

void Menu_AllocateSelectionWorkspace(void)
{
	struct SelectionWorkspace *work;
	u32						   zero;

	work = Runtime_AllocateBlock(0x13, 0x1004);
	zero = 0;
	work->first_active = zero;
	work->second_active = zero;
}

void Ui_FinalizeWorkAndReleaseHeap13(void)
{
	struct SelectionWorkspace *state;

	state = *(struct SelectionWorkspace **)gSelectionWorkspace;
	UiWork_Finalize(state->window, 0);
	while (UiWork_IsIdle(state->window) == 0)
	{
		WaitFrames(1);
	}
	if (state->first_active != 0)
	{
		Resource_ResetEntry(state->first_resource);
	}
	if (state->second_active != 0)
	{
		Resource_ResetEntry(state->second_resource);
	}
	Runtime_ReleaseHeapBlock(0x13);
}

void GraphicsPalette_ReservedNoOpC9BC(void)
{
}

void GraphicsPalette_ReservedNoOpC9C0(void)
{
}

void GraphicsPalette_ReservedNoOpC9C4(void)
{
}

struct MenuWorkspaceEntry
{
	u8	unknown_00[10];
	u16 state;
	u8	unknown_0c[40];
};

void Menu_ResetWorkspaceSelection(struct WorkspaceWork *work)
{
	struct MenuWorkspaceEntry *entry = (struct MenuWorkspaceEntry *)((u8 *)work + 0x400);
	u16						   zero = 0;

	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	entry++;
	entry->state = zero;
	work->page = zero;
	work->tint[0] = zero;
	work->tint[1] = zero;
	work->tint[2] = zero;
	work->cursor_frame = zero;
}

void GraphicsPalette_SetTintChannelsFromCounters(struct WorkspaceWork *work)
{
	s16 phase;
	s32 bias;
	s32 blue;
	s32 red;
	s32 green;

	phase = (gGameState.palette_glow[0] + 12) % 24 * 4;
	bias = gGameState.palette_glow[1] - 7;
	red = PaletteGlow_WaveTable[(phase % 96)];
	green = PaletteGlow_WaveTable[(phase + 32) % 96];
	blue = PaletteGlow_WaveTable[(phase + 64) % 96];
	red += bias;
	green += bias;
	blue += bias;
	if (red < 0)
		red = 0;
	if (green < 0)
		green = 0;
	if (blue < 0)
		blue = 0;
	if (red > 31)
		red = 31;
	if (green > 31)
		green = 31;
	if (blue > 31)
		blue = 31;
	work->tint[0] = red;
	work->tint[1] = green;
	work->tint[2] = blue;
}

void Palette_WriteBlendedBank15Entries(struct WorkspaceWork *work)
{
	*(s16 *)0x050001E8 = Palette_ScaleTintChannels(work, 0xEEEE, 0xCCCC, 0x11110);
	*(s16 *)0x050001EA = Palette_ScaleTintChannels(work, 0xD555, 0xBBBB, 0xEEEE);
	*(s16 *)0x050001EC = Palette_ScaleTintChannels(work, 0xBBBB, 0xAAAA, 0xCCCC);
	*(s16 *)0x050001EE = Palette_ScaleTintChannels(work, 0xA221, 0x9999, 0xAAAA);
	*(s16 *)0x050001F0 = Palette_ScaleTintChannels(work, 0x10888, 0xDDDD, 0x13333);
	*(s16 *)0x050001F2 = Palette_ScaleTintChannels(work, 0x12221, 0xEEEE, 0x15555);
	*(s16 *)0x050001F4 = Palette_ScaleTintChannels(work, 0x13BBB, 0x10000, 0x17777);
}

u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b)
{
	s32 r;
	s32 g;
	s32 b;

	r = Iwram_MulQ16(work->tint[0] << 16, scale_r) >> 16;
	g = Iwram_MulQ16(work->tint[1] << 16, scale_g) >> 16;
	b = Iwram_MulQ16(work->tint[2] << 16, scale_b) >> 16;
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	if (r > 31)
		r = 31;
	if (g > 31)
		g = 31;
	if (b > 31)
		b = 31;
	return r + ((b << 10) + (g << 5));
}

u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale)
{
	s32 red;
	s32 green;
	s32 blue;

	red = Iwram_MulQ16(rgb[0] << 16, red_scale) >> 16;
	green = Iwram_MulQ16(rgb[1] << 16, green_scale) >> 16;
	blue = Iwram_MulQ16(rgb[2] << 16, blue_scale) >> 16;
	if (red < 0)
		red = 0;
	if (red > 31)
		red = 31;
	if (green < 0)
		green = 0;
	if (green > 31)
		green = 31;
	if (blue < 0)
		blue = 0;
	if (blue > 31)
		blue = 31;
	return red + ((blue << 10) + (green << 5));
}

void PaletteGlow_Update(s32 phase, s32 brightness)
{
	s16 rgb[3];
	s16 step;
	s16 red;
	s16 green;
	s16 blue;
	s16 offset;

	step = ((phase + 12) % 24) * 4;
	offset = brightness - 7;
	red = PaletteGlow_WaveTable[(step % 96)] + offset;
	green = PaletteGlow_WaveTable[(step + 32) % 96] + offset;
	blue = PaletteGlow_WaveTable[(step + 64) % 96] + offset;
	if (red < 0)
		red = 0;
	if (red > 31)
		red = 31;
	if (green < 0)
		green = 0;
	if (green > 31)
		green = 31;
	if (blue < 0)
		blue = 0;
	if (blue > 31)
		blue = 31;
	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
	BG_PLTT_COLOR(15, 4) = Color_ScaleComponents(rgb, 0xeeee, 0xcccc, 0x11110);
	BG_PLTT_COLOR(15, 5) = Color_ScaleComponents(rgb, 0xd555, 0xbbbb, 0xeeee);
	BG_PLTT_COLOR(15, 6) = Color_ScaleComponents(rgb, 0xbbbb, 0xaaaa, 0xcccc);
	BG_PLTT_COLOR(15, 7) = Color_ScaleComponents(rgb, 0xa221, 0x9999, 0xaaaa);
	BG_PLTT_COLOR(15, 8) = Color_ScaleComponents(rgb, 0x10888, 0xdddd, 0x13333);
	BG_PLTT_COLOR(15, 9) = Color_ScaleComponents(rgb, 0x12221, 0xeeee, 0x15555);
	BG_PLTT_COLOR(15, 10) = Color_ScaleComponents(rgb, 0x13bbb, 0x10000, 0x17777);
}

void GraphicsPalette_DecrementSelectionWrap(struct WorkspaceWork *work)
{
	if (work->page == 0)
		work->page = 2;
	else
		work->page--;
}

void Menu_AdvanceWorkspaceIndexModulo3(struct WorkspaceWork *work)
{
	if (++work->page > 2)
		work->page = 0;
}

void GraphicsPalette_DecrementSelectedCounter(struct WorkspaceWork *work)
{
	switch (work->page)
	{
	case 0:
		if (gGameState.message_speed != 0)
			gGameState.message_speed--;
		break;
	case 1:
		if (gGameState.palette_glow[0] != 0)
			gGameState.palette_glow[0]--;
		break;
	case 2:
		if (gGameState.palette_glow[1] != 0)
			gGameState.palette_glow[1]--;
		break;
	}
}

void GraphicsPalette_AdjustSelectionCounter(struct WorkspaceWork *work)
{
	switch (work->page)
	{
	case 0:
		if (gGameState.message_speed <= 1)
			gGameState.message_speed++;
		break;
	case 1:
		if (gGameState.palette_glow[0] <= 23)
			gGameState.palette_glow[0]++;
		break;
	case 2:
		if (gGameState.palette_glow[1] <= 14)
			gGameState.palette_glow[1]++;
		break;
	}
}

void GraphicsPalette_SelectionNoOp(void)
{
}

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
	s32					  src0;
	s32					  src1;
	s32					  sel;
	struct WorkspaceWork *work;

	work = gSelectionWork;
	sel = work->page;
	ShopCursor_AdvanceFar(&work->cursor);
	ShopCursor_MoveTowardTargetFar(&work->marker[0]);
	ShopCursor_MoveTowardTargetFar(&work->marker[1]);

	if (sel == 0)
	{
		src0 = (work->cursor_frame & 7) + (s32)&ResourceId_ShopCursorFrames;
	}
	else
	{
		src0 = (s32)&ResourceId_ShopCursorFrames;
	}
	VramBlock_LoadCached((u8)work->marker[0].output->index, 0x100, Resource_GetTableEntry(src0));

	if (sel == 1)
	{
		src1 = (work->cursor_frame & 7) + (s32)&ResourceId_ShopCursorFrames;
	}
	else
	{
		src1 = (s32)&ResourceId_ShopCursorFrames;
	}
	VramBlock_LoadCached((u8)work->marker[1].output->index, 0x100, Resource_GetTableEntry(src1));

	if (sel > 1)
		Ui_ApplyTableScaleToObject(work->frame[sel - 2][work->option[sel]]);
	work->cursor_frame++;
}

void OptionMenu_InitializeWork(void)
{
	struct WorkspaceWork *work;
	volatile u32		  zero;

	work = Runtime_AllocateBlock(20, 0x628);
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x8500018a, REG_DMA3);
	work->option[0] = gGameState.palette_glow[0];
	work->option_count[0] = 24;
	work->option[1] = gGameState.palette_glow[1];
	work->option_count[1] = 15;
	work->option[2] = gGameState.message_speed;
	work->option_count[2] = 3;
	work->option[3] = gGameState.speech;
	work->option_count[3] = 2;
	work->option[4] = gGameState.saved_options;
	work->option_count[4] = 2;
	Scheduler_AddOrUpdateCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance, 0xc80);
}

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
	Scheduler_RemoveCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance);
	Runtime_ReleaseHeapBlock(0x14);
}

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96

struct RenderInput *Menu_OpenWorkspaceOptions(void)
{
	/* FAKEMATCH: the shared cursor union keeps pointer stores before window reads; an equal-size struct reorders these moves in all six editions. */
	struct WorkspaceWork *work;
	struct RenderInput	 *win;
	struct RenderOutput	 *out;
	s32					  x;
	s32					  y;
	s32					  msg;

	work = gSelectionWork;
	win = UiWindow_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, 2);
	UiWindow_DrawDividerLine(win, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
	UiWindow_DrawDividerLine(win, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
	UiWindow_DrawDividerLine(win, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
	UiWindow_DrawDividerLine(win, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
	msg = (s32)MsgWindowColorLabel;
	UiText_DrawCharacterAtOffset(msg, win, 8, 0);
	msg++;
	UiText_DrawCharacterAtOffset(msg, win, 8, 16);
#if EDITION_INTERNATIONAL
	msg = (s32)MsgMessageSpeedLabel;
	UiText_DrawCharacterAtOffset(msg, win, 8, 32);
	msg++;
	UiText_DrawCharacterAtOffset(msg, win, 32, 40);
#else
	UiText_DrawCharacterAtOffset((s32)MsgMessageSpeedLabel, win, 8, 40);
#endif
	UiText_DrawCharacterAtOffset((s32)MsgSpeechLabel, win, 8, 64);
	UiText_DrawCharacterAtOffset((s32)MsgAutoSleepLabel, win, 8, 88);

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(x, 128, Resource_FixedBlockBTiles);
		out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
		work->cursor.output = out;
		x = win->x * 8;
		y = win->y * 8 + 12;
		ShopCursor_SetPositionImmediateFar(&work->cursor, x, y);
	}

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		Dma_Set(WorkspaceOptions_SliderPalette, (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0),
			0x80000020, REG_DMA3);
		VramBlock_LoadCached(x, 256, WorkspaceOptions_SliderTiles);
		y = 0;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
		out->oam.palette = SLIDER_PALETTE;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
		out->oam.index += 4;
		out->oam.palette = SLIDER_PALETTE;
		y = 16;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
		out->oam.palette = 15;
		out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
		out->oam.index += 4;
		out->oam.palette = 15;
	}

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(x, 256, 0);
		out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
		out->oam.palette_256 = 1;
		work->marker[0].output = out;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += work->option[0] * 60 / work->option_count[0];
		y = win->y * 8 + 4;
		ShopCursor_SetPositionImmediateFar(&work->marker[0], x, y);
	}

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(x, 256, 0);
		out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
		out->oam.palette_256 = 1;
		work->marker[1].output = out;
		x = win->x * 8 + OPTION_SLIDER_X;
		x += work->option[1] * 60 / work->option_count[1];
		y = win->y * 8 + 20;
		ShopCursor_SetPositionImmediateFar(&work->marker[1], x, y);
	}

	y = 28;
	work->frame[0][0] =
		RenderResource_CreateFrame(OptionMenu_SpeedFrames[0], 0, win, OPTION_FRAME_X, y);
	work->frame[0][1] =
		RenderResource_CreateFrame(OptionMenu_SpeedFrames[1], 0, win, OPTION_FRAME_X + 24, y);
	work->frame[0][2] =
		RenderResource_CreateFrame(OptionMenu_SpeedFrames[2], 0, win, OPTION_FRAME_X + 48, y);
	y = 52;
	work->frame[1][0] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[0], 0, win, 100, y);
	work->frame[1][1] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[1], 0, win, 124, y);
	y = 76;
	work->frame[2][0] = RenderResource_CreateFrame(OptionMenu_SleepFrames[0], 0, win, 100, y);
	work->frame[2][1] = RenderResource_CreateFrame(OptionMenu_SleepFrames[1], 0, win, 124, y);
	return win;
}

s32 Menu_RunWorkspaceOptions(void)
{
	struct WorkspaceWork *work;
	struct RenderInput	 *icon;
	struct RenderInput	 *win;
	struct RenderOutput	 *pair;
	s8					 *pA;
	s8					 *pB;
	s32					  redraw;
	s32					  page;
	s32					  i;
	s32					  dim;
	s32					  index;
	s32					  x;
	s32					  y;
	s32					  result;
	struct RenderOutput	 *frame;

	redraw = 1;
	page = 0;
	OptionMenu_InitializeWork();
	work = gSelectionWork;
	win = UiWindow_Create(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3, 2);
	icon = Menu_OpenWorkspaceOptions();
	pair = RenderResource_CreatePair(7, icon, 64, -48);
	WaitFrames(1);
	pA = &work->option[0];
	pB = &work->option[1];

	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			page = (page + 5) % 5;
			work->option[page] =
				(work->option[page] + work->option_count[page]) % work->option_count[page];
			work->page = page;
			if (gSleepDisabled != 0)
				work->option[4] = 0;

			for (i = 0; i <= 2; i++)
			{
				frame = work->frame[0][i];
				frame->sentinel = 0xfb;
				UiIcon_PrepareObjectFar(frame);
				index = (u8)work->frame[0][i]->index;
				dim = 0;
				if (i != work->option[2])
					dim = 1;
				RenderResource_LoadFrame(OptionMenu_SpeedFrames[i], index, dim);
			}
			for (i = 0; i <= 1; i++)
			{
				frame = work->frame[1][i];
				frame->sentinel = 0xfb;
				UiIcon_PrepareObjectFar(frame);
				index = (u8)work->frame[1][i]->index;
				dim = 0;
				if (i != work->option[3])
					dim = 1;
				RenderResource_LoadFrame(OptionMenu_SpeechFrames[i], index, dim);
			}
			for (i = 0; i <= 1; i++)
			{
				frame = work->frame[2][i];
				frame->sentinel = 0xfb;
				UiIcon_PrepareObjectFar(frame);
				index = (u8)work->frame[2][i]->index;
				dim = 0;
				if (i != work->option[4])
					dim = 1;
				RenderResource_LoadFrame(OptionMenu_SleepFrames[i], index, dim);
			}

			x = icon->x * 8 + OPTION_SLIDER_X;
			x += (*pA * 60) / work->option_count[0];
			y = icon->y * 8 + 4;
			Shop_SetCursorFar(&work->marker[0], x, y, 1);
			x = icon->x * 8 + OPTION_SLIDER_X;
			x += (*pB * 60) / work->option_count[1];
			y = icon->y * 8 + 20;
			Shop_SetCursorFar(&work->marker[1], x, y, 1);

			x = work->option[2] + (s32)MsgMessageSpeedSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 40, OPTION_SPEED_END, 48);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 40);
			x = work->option[3] + (s32)MsgSpeechSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 64, OPTION_SETTING_END, 72);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 64);
			x = work->option[4] + (s32)MsgAutoSleepSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 88, OPTION_SETTING_END, 96);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 88);
			PaletteGlow_Update(*pA, *pB);

			x = icon->x * 8;
			y = (page * 3 + icon->y) * 8 + 4;
			if (page == 0)
				y += 8;
			Shop_SetCursorFar(&work->cursor, x, y, 3);
			RenderOutput_ClearList(win);
			UiText_DrawResource(page + (s32)MsgOptionHelp, win, 0, 0);
		}
		Ui_ApplyTableOffsetToPair(pair);
		WaitFrames(1);

		if (gKeyState & KEY_SELECT)
		{
			AudioCommand_PlayFar(112);
			work->preset++;
			redraw = 1;
			if (work->preset > 5)
				work->preset = 0;
			*pA = OptionMenu_FirstColorPresets[work->preset];
			*pB = OptionMenu_SecondColorPresets[work->preset];
			continue;
		}
		if (gKeyState & (KEY_A | KEY_START))
		{
			result = 0;
			AudioCommand_PlayFar(112);
			break;
		}
		if (gKeyState & KEY_B)
		{
			result = -1;
			AudioCommand_PlayFar(113);
			break;
		}
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			page--;
			redraw = 1;
		}
		else if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			page++;
			redraw = 1;
		}
		else
		{
			if (gKeysRepeat & KEY_LEFT)
			{
				AudioCommand_PlayFar(111);
				work->option[page]--;
				redraw = 1;
			}
			if (gKeysRepeat & KEY_RIGHT)
			{
				AudioCommand_PlayFar(111);
				work->option[page]++;
				redraw = 1;
			}
		}
	}

	UiWork_Finalize(win, 2);
	UiWork_Finalize(icon, 2);
	if (result == 0)
	{
		gGameState.palette_glow[0] = work->option[0];
		gGameState.palette_glow[1] = work->option[1];
		gGameState.message_speed = work->option[2];
		gGameState.speech = work->option[3];
		gGameState.saved_options = work->option[4];
		gOptionMirror = gGameState.saved_options;
	}
	else
	{
		PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	}
	Runtime_ScheduleCallbackAndReleaseBlock20B();
	WaitFrames(1);
	return result;
}

#define FLAG_WORKSPACE_SETTINGS_ONLY 0x17e
#define WORKSPACE_ROWS 3
#define WORKSPACE_DEBUG_ROWS 3
#define WORKSPACE_ROW_HEIGHT 24
#define WORKSPACE_TEXT_X 48

void Menu_RunSelectedWorkspaceEntry(void)
{
	struct WorkspaceWork *work = gSelectionWork;
	u32					  index;

	ShopCursor_AdvanceFar(&work->cursor);
	index = work->page;
	Ui_ApplyTableScaleToObject(work->icon[index]);
}

void Menu_InitializeSelectedWorkspace(void)
{
	void		*work;
	volatile u32 zero;
	work = Runtime_AllocateBlock(20, 0x628);
	zero = 0;
	Dma_Set(&zero, work, 0x8500018a, REG_DMA3);
	Scheduler_AddOrUpdateCallback(Menu_RunSelectedWorkspaceEntry, 3200);
}

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
	Scheduler_RemoveCallback(&Menu_RunSelectedWorkspaceEntry);
	Runtime_ReleaseHeapBlock(0x14);
}

struct RenderInput *Menu_CreateWorkspaceWindows(void)
{
	struct WorkspaceWork *work;
	s32					  rows;
	s32					  first;
	s32					  settings_only;
	s32					  top;
	s32					  height;
	struct RenderInput	 *win;
	s32					  y;
	s32					  slot;
	s32					  msg;
	s32					  i;
	s32					  x;

	rows = WORKSPACE_ROWS;
	work = gSelectionWork;
	settings_only = GameFlag_TestFar(FLAG_WORKSPACE_SETTINGS_ONLY);
	first = 0;
	if (settings_only != 0)
	{
		rows = 1;
		first = 2;
	}
	if (gDebugMode != 0)
		rows += WORKSPACE_DEBUG_ROWS;
	top = 8 - rows;
	height = rows * 3 + 1;
	if (top + height > 19)
	{
		top = 1;
		height = 19;
	}
	win = UiWindow_Create(5, top, 20, height, 2);
	for (i = 1; i < rows; i++)
		UiWindow_DrawDividerLine(win, 0, i * 3, 19, i * 3);
	y = 4;
	if (settings_only == 0)
	{
		msg = (s32)MsgWorkspaceSaveQuest;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4);
		msg++;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4 + WORKSPACE_ROW_HEIGHT);
		y = 4 + 2 * WORKSPACE_ROW_HEIGHT;
	}
	UiText_DrawResource((s32)MsgWorkspaceChangeSettings, win, WORKSPACE_TEXT_X, y);
	y += WORKSPACE_ROW_HEIGHT;
	if (gDebugMode != 0)
	{
		msg = (s32)MsgWorkspaceRoughMenu;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
		y += WORKSPACE_ROW_HEIGHT;
		UiText_DrawResource(msg + 1, win, WORKSPACE_TEXT_X, y);
		y += WORKSPACE_ROW_HEIGHT;
		msg += 2;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, y);
	}
	slot = Resource_FindFreeEntry();
	if (slot < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		work->cursor.output = RenderOutput_Create(slot, 0x40000000, win, 0, 0);
		x = win->x * 8;
		y = win->y * 8 + 16;
		ShopCursor_SetPositionImmediateFar(&work->cursor, x, y);
	}
	y = -4;
	for (i = 0; i < rows; i++)
	{
		work->icon[i] =
			RenderResource_CreateFrame(Menu_WorkspaceIconFrames[first + i], 0, win, 12, y);
		y += WORKSPACE_ROW_HEIGHT;
	}
	return win;
}

s32 Menu_RunWorkspaceSelectionLoop(void)
{
	struct RenderInput	 *window;
	s32					  redraw;
	struct RenderOutput	 *pair;
	s32					  first;
	s32					  i;
	s32					  j;
	s32					  rows;
	s32					  settings_only;
	s32					  dim;
	s32					  y;
	s32					  result;
	s32					  page;
	s32					  index;
	s32					  x;
	s32					  top;
	s8					 *tbl;
	struct RenderOutput	 *icon;
	struct WorkspaceWork *work;

	redraw = 1;
	rows = 3;
	settings_only = GameFlag_TestFar(0x17E);
	first = 0;
	Menu_InitializeSelectedWorkspace();
	work = gSelectionWork;
	window = Menu_CreateWorkspaceWindows();
#if EDITION_INTERNATIONAL
	y = -0x18;
	if (gDebugMode != 0)
	{
		y = -0x10;
	}
	pair = RenderResource_CreatePair(6, window, 0x28, y);
#else
	pair = RenderResource_CreatePair(6, window, 0x28, -0x18);
#endif
	WaitFrames(1);
	page = work->page;
	if (settings_only != 0)
	{
		rows = 1;
		first = 2;
	}
	if (gDebugMode != 0)
	{
		rows += 3;
	}
redraw_menu:
	if (redraw != 0)
	{
		redraw = 0;
		page = (page + rows) % rows;
		work->page = page;
		i = 0;
		if (i < rows)
		{
			j = first;
			tbl = Menu_WorkspaceIconFrames;
			do
			{
				icon = work->icon[i];
				icon->sentinel = 0xFB;
				UiIcon_PrepareObjectFar(icon);
				index = (u8)icon->index;
				dim = 0;
				if (i != work->page)
				{
					dim = 1;
				}
				RenderResource_LoadFrame(*(s8 *)(j + (s32)tbl), index, dim);
				i++;
				j++;
			} while (i < rows);
		}
		top = window->y;
		x = window->x * 8;
		y = (((page * 3) + top) * 8) + 0x10;
		Shop_SetCursorFar(&work->cursor, x, y, 3);
	}
	Ui_ApplyTableOffsetToPair(pair);
	WaitFrames(1);
	if (gKeyState & KEY_A)
	{
		result = page;
		AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
	}
	else if (gKeyState & (KEY_B | KEY_START))
	{
		result = -1;
		AudioCommand_PlayFar(SOUND_MENU_CANCEL);
	}
	else
	{
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			page -= 1;
			redraw = 1;
		}
		else if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			page += 1;
			redraw = 1;
		}
		goto redraw_menu;
	}
	UiWork_Finalize(window, 2);
	Runtime_ScheduleCallbackAndReleaseBlock20A();
	WaitFrames(1);
	if (result >= 0)
	{
		result += first;
	}
	return result;
}

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

struct RenderOutput *RenderOutput_Create(
	s32 slot, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y)
{
	s32					 x;
	struct RenderOutput *output;
	s32					 y;

	output = RenderOutput_AcquireFree();
	if (output == NULL)
	{
		Resource_ResetEntry(slot);
		return 0;
	}
	x = offset_x + (input->x * 8) + 8;
	y = offset_y + (input->y * 8) + 8;
	x &= 0x1ff;
	y &= 0xff;
	*(u32 *)&output->oam = (x << 16) | y | flags;
	((u32 *)&output->oam)[1] = ((struct VramBlockCacheEntry *)Ram_VramBlockCache)[slot].offset >> 5;
	output->sentinel = 0xff;
	output->next = 0;
	output->x = x;
	output->y = y;
	output->index = slot;
	output->kind = 1;
	output->active = 1;
	/* FAKEMATCH: RenderOutput_AppendToList is called as s32 (*)(); as void (*)(struct RenderOutputList *, struct RenderOutput *), its prototype, RenderOutput_Create has 9 of 136 bytes different */
	((s32 (*)())RenderOutput_AppendToList)(input, output);
	return output;
}

struct RenderOutput *RenderOutput_CreateFromResource(
	s32 mode, s32 resource, struct RenderInput *input, s32 x, s32 y)
{
	s32 no;

	no = Resource_LoadByMode(mode, resource);
	if (no < 0)
	{
		return NULL;
	}
	return RenderOutput_Create(no, 0x40000000, input, x, y);
}

struct RenderOutput *RenderOutput_CreateLoaded(
	s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y)
{
	s32					 no;
	struct RenderOutput *result;

	no = Resource_FindFreeEntry();
	result = NULL;
	if (no != 0x60)
	{
		UiIcon_CopyResourceToSlot(code, layers, no);
		result = RenderOutput_Create(no, 0x40000000, input, x, y);
	}
	return result;
}

struct RenderOutput *RenderOutput_CreateWithTransform(
	s32 icon, struct RenderInput *input, s32 x, s32 y)
{
	s32					 count;
	s32					 unused;
	struct RenderOutput *result;

	count = Resource_FindFreeEntry();
	if (count == 0x60)
	{
		return NULL;
	}
	Ui_BuildPairedPatternsToSlot(icon, 1, &count, &unused, 1);
	result = RenderOutput_Create(count, 0x40000000, input, x, y);
	result->sentinel = 251;
	return result;
}

struct RenderOutput *RenderOutput_CreateFromTable(
	s32 table_entry, struct RenderInput *input, s32 x, s32 y)
{
	s32					 slot;
	struct RenderOutput *output;

	slot = Resource_FindFreeEntry();
	output = NULL;
	if (slot != 0x60)
	{
		RenderResource_LoadTableEntry(table_entry, 0, slot);
		output = RenderOutput_Create(slot, 0x40000000, input, x, y);
	}
	return output;
}

struct RenderOutput *CreateSideObject(
	s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
	struct UiRenderWork *state = UI_WORK;
	struct RenderOutput *object = 0;
	s32					 first;
	s32					 second;
	s32					 id;
	s32					 slot;

	if (GameFlag_TestFar(32) != 0)
	{
		if (object_kind == 0)
			object_kind = 18;
		if (object_kind == 1)
			object_kind = 19;
	}

	id = Localization_LookupEntryId(object_kind);
	if (id == -1)
		return object;

	if ((u32)side > 1)
	{
		side = 1;
		if (state->resource_ids[1] != 999)
		{
			side = 0;
			if (state->resource_ids[0] != 999)
				return object;
		}
	}

	slot = 14 + side;
	UiGlyph_LoadEntryWithPalette(id, position, &first, &second, slot, 0);
	object = RenderOutput_Create(first, 0x80000000, (struct RenderInput *)arg3, arg4, arg5);
	if (object != 0)
	{
		object->oam.palette = slot;
		object->kind = 2;
	}

	state->resource_ids[side] = id;
	state->output_ids[side] = first;
	return object;
}

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 character, s32 value)
{
	s32					 result;
	s32					 current;
	u32					 character_id;
	struct UiRenderWork *state;

	state = UI_WORK;

	if (GameFlag_TestFar(0x20) != 0)
	{
		if (character == 0)
			character = 0x12;
		if (character == 1)
			character = 0x13;
	}

	character_id = Localization_LookupEntryId(character);
	if (character_id != -1U)
	{
		if (slot > 1U)
		{
			if (state->resource_ids[1] == character_id)
			{
				slot = 1;
			}
			else if (state->resource_ids[0] == character_id)
			{
				slot = 0;
			}
			else
			{
				return;
			}
		}
		current = state->output_ids[slot];
		UiGlyph_LoadEntryWithPalette(character_id, value, &current, &result, slot + 0xe, 1);
	}
}

//--------------------------------------------------------------------------
// runtime_set_byte5_and_reset_halfword_c.c
// 表示：スプライト出力のアニメを切りかえる
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// active を mode にして、アニメの段（anim_step）を 0 に戻す
// out : スプライト出力（NULL なら何もしない）
//--------------------------------------------------------------------------
void RenderOutput_SetAnimation(struct RenderOutput *out, s32 mode)
{
	if (out != NULL)
	{
		out->active = mode;
		out->anim_step = 0;
	}
}

void RenderOutput_SetInvertedSentinel(void *arg0, s32 arg1)
{
	if (arg0 != 0)
		((struct RenderOutput *)arg0)->sentinel = ~arg1;
}

#define TILE_ROW_FROM_BOTTOM(tile, up) ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

typedef void (*RamRoutine)(void *dst, void *scene);

void UiWindow_FillFromScene(void *dst)
{
	void *scene = gWindowWork[0];

	if (scene == 0)
	{
		volatile u16 fill = 0xe0e0;

		Dma_Set((void *)&fill, dst, 0x810000a0, REG_DMA3);
	}
	else
	{
		RamRoutine code;

		RamCode_Load(code, Tile_BuildMetatiles, Tile_BuildMetatilesCodeSize);
		code(dst, scene);
		MemFree(code);
	}
}

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height, s32 value)
{
	u32	 row = 0;
	s16 *dst = (s16 *)0x06002000;

	if (row < height)
	{
		do
		{
			u32 column = 0;
			if (column < width)
			{
				do
				{
					column++;
					*dst = value;
					dst++;
				} while (column < width);
			}
			row++;
			dst += 32 - width;
		} while (row < height);
	}
}

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
		/* FAKEMATCH: UiText_DrawPrefixedNumberAtOffset is called as s32 (*)(); as void (*)(s32, struct RenderInput *, s32, s32, s32), its prototype, PtyStDraw has 7 of 980 bytes different */
		((s32 (*)())UiText_DrawPrefixedNumberAtOffset)(hp, window, (x + i * 6) * 8, y * 8 + 8, 0);
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
		/* FAKEMATCH: needed to match: these four calls pass no mode argument (an unprototyped call of the five-argument function). */
		((s32 (*)())UiWindow_PutGlyph)(window, djinn[0] + '0', 1, row);
		((s32 (*)())UiWindow_PutGlyph)(window, djinn[1] + '0', 3, row);
		((s32 (*)())UiWindow_PutGlyph)(window, djinn[2] + '0', 1, row + 1);
		((s32 (*)())UiWindow_PutGlyph)(window, djinn[3] + '0', 3, row + 1);
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

void *Text_FormatPlayTime(s32 value, u8 *out)
{
	u8	buf[64];
	u32 time;
	u32 minutes;
	s32 seconds;
	u8 *s;
	u8 *p;

	time = (u32)value / 0xe10;
	if (time > 0xea5f)
		time = 0xea5f;

	minutes = time / 60;
	seconds = time % 60;

	s = UiText_FormatNumber(buf, minutes, 3);
	*out = *s;
	s++;
	p = out + 1;
	*p = *s;
	p++;
	*p = s[1];
	seconds += 100;
	p++;
	*p = ':';

	s = UiText_FormatNumber(buf, seconds, 2);
	p++;
	*p = s[0];
	p++;
	*p = s[1];
	p[1] = 0;

	return out;
}

u32 SaveState_FindFreeSummarySlot(void)
{
	u32					i;
	struct SaveSummary *p;

	p = gSaveWorkspace->summary;
	for (i = 0; i < 3; i++, p++)
	{
		if (p->level == 0)
			return i;
	}
	return 0x3E7;
}

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
	s32					i;
	s32					cnt;
	struct SaveSummary *summary;

	if (SaveState_InitializeWorkspace() != 0)
	{
		cnt = -9;
	}
	else
	{
		cnt = SaveState_LoadSummaryRecords();
		if (flag != 0)
		{
			summary = gSaveWorkspace->summary;
			for (i = 0; i < 3; i++)
			{
				if ((s8)summary[i].send_flag != 0)
					cnt--;
			}
		}
	}
	SaveState_ReleaseWorkspace();
	return cnt;
}

s32 SaveState_ScanRecordFlags(void)
{
	/* FAKEMATCH: the signed cursor from the party tail preserves the literal base changed by direct flag fields. */
	s32 err;
	s32 cnt;
	s32 ret;

	err = SaveState_InitializeWorkspace();
	cnt = 0;
	ret = -9;
	if (err == 0)
	{
		s32 i;
		s8 *flags;

		ret = SaveState_LoadSummaryRecords();
		flags = (s8 *)&gSaveWorkspace->summary[0].party[4];
		gTitleSendOptionEnabled = 0;
		(*(s16 *)&gTitleExtraOptionEnabled) = 0;
		for (i = 0; i < 3; i++)
		{
			if (flags[i * sizeof(struct SaveSummary)
					  + ((u32) & ((struct SaveSummary *)0)->send_flag - (u32)
						  & ((struct SaveSummary *)0)->party[4])]
				!= 0)
			{
				gTitleSendOptionEnabled = 1;
				cnt++;
			}
			if (flags[i * sizeof(struct SaveSummary)
					  + ((u32) & ((struct SaveSummary *)0)->flag_count - (u32)
						  & ((struct SaveSummary *)0)->party[4])]
				!= 0)
			{
				(*(s16 *)&gTitleExtraOptionEnabled) = 1;
			}
		}

		if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT))
		{
			gTitleSendOptionEnabled = 0;
		}
	}
	SaveState_ReleaseWorkspace();
	if (ret != 0 && cnt == ret)
	{
		return ret + 100;
	}
	return ret;
}

u32 SaveState_BuildSummaryHeader(void)
{
	u16					owners[14];
	struct SaveSummary *summary;
	struct BtlUnit	   *owner;
	u8				   *source;
	u8				   *destination;
	u32				   *word;
	s32					i;
	u32					sum = 0;

	summary = (struct SaveSummary *)(gSaveBuffer - 16);
	/* FAKEMATCH: Runtime_GetBuildStampTimeFar is called as u32 (*)(s32); as u32 (*)(void), its prototype, SaveState_BuildSummaryHeader does not compile */
	gGameState.build_stamp = ((u32 (*)(s32))Runtime_GetBuildStampTimeFar)(0);
	gGameState.play_time = gLoadedStateWord;
	{
		u32 *copy = (u32 *)gSceneState;
		copy[64] = gLoadedStateWord;
	}
	gGameState.saved_options = gOptionMirror;
	owner = UnitGetFar(gGameState.selected_actor);
	destination = summary->name;
	source = owner->name;
	for (i = 11; i >= 0; i--)
		*destination++ = *source++;
	summary->level = owner->level;
	summary->play_time = gGameState.play_time;
	summary->area = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
	summary->class_id = owner->class_index;
	summary->coins = gGameState.coins;
	summary->djinn[0] = Party_SumDjinnCountsFar(0);
	summary->djinn[1] = Party_SumDjinnCountsFar(1);
	summary->djinn[2] = Party_SumDjinnCountsFar(2);
	summary->djinn[3] = Party_SumDjinnCountsFar(3);
	Party_ListActiveOwnersFar(owners);
	for (i = 0; i <= 3 && owners[i] != 255; i++)
		summary->party[i] = owners[i];
	summary->party[i] = -1;
	summary->palette_glow = gGameState.palette_glow[0];
	summary->palette_glow_sub = gGameState.palette_glow[1];
	summary->send_flag = gGameState.unknown_20f;
	summary->flag_count = 0;
	for (i = 48; i <= 127; i++)
	{
		if (GameFlag_TestFar(i))
			summary->flag_count++;
	}
	summary->has_flag_20 = GameFlag_TestFar(32) != 0;
	{
		s32	 n;
		u32 *frame = &gGameState.build_stamp;
		u32	 frames;

		n = 242;
		/* FAKEMATCH: the one-pass null arm and duplicate frame read end
         * CSE with the bound constant ahead of the frame load. Folding
         * the arm reverses two rows; the conditional and duplicate read
         * disappear, preserving all 174 instructions in every edition. */
		if (!frame)
		{
			do
			{
				n = 242;
			} while (0);
		}
		else
		{
			n = 242;
			frames = *frame;
		}
		frames = *frame;
		word = (u32 *)GameFlagBytes;
		i = 0;
		n <<= 2;
		summary->frames = frames;
		goto test;
		do
		{
			sum += *word++;
			i++;
		test:;
		} while (i < n);
	}
	summary->checksum = sum;
	return sum;
}

s16 SaveState_WriteCurrentSlotPair(void)
{
	s16 value;
	s16 result;
	s32 found;
	s32 error;

	result = 0;
	value = gSaveSlot;
	if (value != -1)
	{
		found = SaveState_InitializeWorkspace();
		if (found != 0)
		{
			UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
			error = 9;
			goto set_error;
		}
		SaveState_BuildSummaryHeader();
		{
			void *base = &gSaveBuffer;
			s32	  next;

			found = SaveState_WriteRecord(gSaveSlot, base);
			next = gSaveSlot;
			base = (char *)base + 0x1000;
			found |= SaveState_WriteRecord(next + 3, base);
			if (found != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
				error = 3;
			set_error:
				result = 0 - error;
			}
		}
		SaveState_ReleaseWorkspace();
		value = result;
	}
	return value;
}

s32 SaveState_WriteSlotPair(s32 arg0)
{
	s32 found;
	s16 result = 0;

	found = SaveState_InitializeWorkspace();
	if (found != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		void *base = &gSaveBuffer;

		found = SaveState_WriteRecord(arg0, base);
		base = (char *)base + 0x1000;
		found |= SaveState_WriteRecord(arg0 + 3, base);
		if (found != 0)
		{
			UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
			result = -3;
		}
	}
	SaveState_ReleaseWorkspace();
	return result;
}

s32 SaveState_ProcessSelectedSlot(void)
{
	void *buffer;
	s32	  value;
	s32	  result;
	s32	  found;
	s32	  error;

	buffer = Runtime_BumpAllocateAlternatePool(0x1000);
	result = 0;
	value = gSaveSlot;
	if (value != -1)
	{
		found = SaveState_InitializeWorkspace();
		if (found != 0)
		{
			error = 9;
			UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
			/* FAKEMATCH: this eliminated self-assignment ends CSE before
             * the shared negate block. Without it, the unbound error uses
             * r6 for both constants and needs a copy to r7 (differ 6).
             * Scoping, a slot pointer and an inline return leave that
             * allocation; duplicating the negate tail still differs 4. */
			gSaveSlot += 0;
			goto negate;
		}
		else
		{
			u8 *dst;

			found = SaveState_ReadRecordPayload(gSaveSlot, buffer);
			if (found != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
				result = -2;
			}
			dst = (u8 *)buffer + (s32)gSaveStamp;
			dst -= (s32)gSaveBuffer;
			Iwram_CopyWords(dst, gSaveStamp, 16);
			found = SaveState_WriteRecord(gSaveSlot, buffer);
			if (found != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
				error = 3;
			negate:
				result = -error;
			}
		}
		SaveState_ReleaseWorkspace();
		MemFree(buffer);
		value = result;
	}
	return value;
}

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
	s32 result;

	UiText_OpenMessageWindow((s32)MsgOverwriteConfirm, 8, 12, 2);
	while (UiWork_IsComplete() == 0)
	{
		WaitFrames(1);
	}
	if (Menu_RunConfirmSelection(1, 0, 0, 1))
	{
		UiWork_FinalizePendingCore();
	}
	else
	{
		UiWork_FinalizePendingCore();
		AudioCommand_PlayFar(85);
		result = SaveState_ProcessSelectedSlot();
		if (result >= 0)
		{
			UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 1);
		}
	}
	return result;
}

s32 SaveState_CopySlotPair(void)
{
	u32 found;
	s32 value;
	s32 result;

	result = 0;
	found = SaveState_InitializeWorkspace();
	if (found != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		SaveState_LoadSummaryRecords();
		value = SaveMenu_SelectSlot(0, 2);
		if (value == -1)
		{
			result = value;
		}
		else
		{
			void *lower = &gSaveBuffer;
			void *upper;

			found = SaveState_ReadRecordPayload(value, lower);
			upper = (u8 *)lower + 0x1000;
			found |= SaveState_ReadRecordPayload(value + 3, upper);
			if (found != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
				result = -2;
			}
			else
			{
				value = SaveState_FindFreeSummarySlot();
				if (value == 999)
				{
					UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
					result = -5;
				}
				else
				{
					found = SaveState_WriteRecord(value, lower);
					found |= SaveState_WriteRecord(value + 3, upper);
					if (found != 0)
					{
						UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
						result = -3;
					}
					else
					{
						UiText_ShowPositionedMessageAndWait((s32)MsgFileCopied, 1);
					}
				}
			}
		}
	}

	SaveState_ReleaseWorkspace();
	return result;
}

s32 SaveState_DeleteSelectedSlot(void)
{
	s32 found;
	s32 value;
	s32 result = 0;

	found = SaveState_InitializeWorkspace();
	if (found != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		SaveState_LoadSummaryRecords();
		value = SaveMenu_SelectSlot(0, 3);
		if (value == -1)
		{
			result = value;
		}
		else
		{
			UiText_OpenMessageWindow((s32)MsgEraseConfirm, 8, 1, 2);
			while (UiWork_IsComplete() == 0)
			{
				WaitFrames(1);
			}
			if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0)
			{
				UiWork_FinalizePendingCore();
			}
			else
			{
				UiWork_FinalizePendingCore();
				found = SaveState_DeleteRecord(value);
				found |= SaveState_DeleteRecord(value + 3);
				if (found != 0)
				{
					UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
					result = -4;
				}
				else
				{
					UiText_ShowPositionedMessageAndWait((s32)MsgFileErased, 1);
				}
			}
		}
	}
	SaveState_ReleaseWorkspace();
	return result;
}

//--------------------------------------------------------------------------

// glow_update_sine.c

// パレット：BG パレット 14 の色 8〜11 を sin で明滅させる

//--------------------------------------------------------------------------

#if EDITION_INTERNATIONAL || defined(TLA_EDITION_JA)

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、4 色を位相をずらした sin で明滅させる
// s = sin / 16384 として RGB555 で 赤 22 + 2s・緑 16 + s・青 20 + s
//--------------------------------------------------------------------------
void PaletteGlow_UpdateSine(void)
{
	s32			 *tick;
	volatile u16 *pal;
	s32			  col;
	s32			  s;
	s32			  i;

	tick = (s32 *)&gFrameTick;
	pal = (volatile u16 *)0x050001D0; // BG パレット 14 の色 8
	i = 0;
	do
	{
		s = Trig_Sin((*tick + i * 8) * 0x300); // 1 色ごとに 8 フレームずらす
		col = s / 16384;
		{
			u32 r = col << 1;
			u32 g = col;

			r += 22;
			g += 16;
			col += 20;
			/* FAKEMATCH: the do-while fixes the shift's place in the schedule. */
			do
			{
				col <<= 10;
			} while (0);
			g <<= 5;
			col |= g;
			col |= r;
		}
		/* FAKEMATCH: likewise for the store. */
		do
		{
			*pal++ = col;
		} while (0);
	} while (++i <= 3);
}

#endif

#if EDITION_INTERNATIONAL

void Scheduler_ScheduleCallbackAAfterFrames(void)
{
	Scheduler_AddOrUpdateCallback(PaletteGlow_UpdateSine, 0xC80);
}

void Scheduler_ScheduleCallbackA(void)
{
	Scheduler_RemoveCallback(PaletteGlow_UpdateSine);
}

void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height)
{
	u16 *map = (u16 *)gWindowWork[0];
	s32	 sum;

	sum = x + window->x;
	x = sum + 1;
	sum = y + ((struct UiWindow *)window)->y;
	y = sum + 1;

	if (x < 0)
	{
		width += x;
		x = 0;
	}
	if (x + width > 29)
		width = 30 - x;

	if (y < 0)
	{
		height += y;
		y = 0;
	}
	if (y + height > 29)
		height = 20 - y;

	if (width > 0 && height > 0)
	{
		s32 y_ofs = y << 6;
		s32 x_ofs = x << 1;
		s32 offset = y_ofs + x_ofs;
		do
		{
			s32	 remaining = width;
			u16 *cell = (u16 *)(offset + (s32)map);
			if (remaining != 0)
			{
				do
				{
					remaining--;
					*cell++ = 0xE006;
				} while (remaining != 0);
			}
			height--;
			offset += 64;
		} while (height != 0);
		((struct UiRenderWork *)map)->dirty = 1;
	}
}

#endif

void ObjectPlacement_CreateGroup(
	struct RenderInput *origin, s32 x, s32 y, struct SaveSummary *descriptor)
{
	struct ShopRuntime *state = gMenuWork;
	s32					i;
	s32					duration;

	for (i = 0; i < 4 && descriptor->party[i] != -1; i++)
	{
		struct AnimationObject *object = ResourceObject_CreateFar(
			Party_RemapCharacterIdIfEnabledFar(descriptor->party[i], (s8)descriptor->has_flag_20));

		if (object != 0)
		{
			AnimationObjects_SelectAnimationFar(object, 1);
			((struct FieldSprite *)object)->flags = 0;
			((struct FieldSprite *)object)->priority = 0;
		}

		state->party_member_icons[i] = object;
		state->party_member_x[i] = (x + origin->x + i * 3) * 8 + 0x10;
		state->party_member_y[i] = (y + origin->y) * 8 + 0x10;
		state->party_member_scale[i] = 0x10000;
	}

	duration = 200;
	duration <<= 4;
	Scheduler_AddOrUpdateCallback(Menu_UpdateFirstObjectRowPositions, duration);
}

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
	struct ShopRuntime *state = gMenuWork;
	s32					i;

	Scheduler_RemoveCallback(Menu_UpdateFirstObjectRowPositions);
	for (i = 0; i < 4; i++)
	{
		if (state->party_member_icons[i] != 0)
		{
			ResourceObject_ReleaseFar(state->party_member_icons[i]);
			state->party_member_icons[i] = 0;
		}
	}
}

void Menu_UpdateFirstObjectRowPositions(void)
{
	struct ShopRuntime *state = gMenuWork;
	s32					source[2];
	s32					request[4];
	s32					index = 0;
	s32					handle;

	while (1)
	{
		handle = (s32)state->party_member_icons[index];
		if (handle != 0)
		{
			source[0] = state->party_member_scale[index];
			source[1] = state->party_member_scale[index];
			request[0] = state->party_member_x[index] << 16;
			request[1] = 0x01F40000;
			request[2] = (state->party_member_y[index] << 16) + 0x01F40000;
			request[3] = 0;
			Object_ApplyProjectedPlacementFar(handle, request, source, 0x4000);
		}
		index += 1;
		if (index > 3)
		{
			break;
		}
	}
}

void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  i;

	if (origin != 0)
	{
		for (i = 0; i < 4; i++)
		{
			struct AnimationObject *object =
				(struct AnimationObject *)ResourceObject_CreateFar(Menu_PartySpriteResourceIds[i]);

			if (object != 0)
			{
				AnimationObjects_SelectAnimationFar(object, 2);
				((struct FieldSprite *)object)->flags = 0;
				((struct FieldSprite *)object)->priority = 0;
			}

			state->objects[i] = object;
			state->positions_x[i] = (origin->x + x + i * 3) * 8 + 0x10;
			state->positions_y[i] = (origin->y + y) * 8 + 0x10;
		}

		Scheduler_AddOrUpdateCallback(Menu_UpdateSecondObjectRowPositions, 200 << 4);
	}
}

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  i;

	Scheduler_RemoveCallback(Menu_UpdateSecondObjectRowPositions);
	for (i = 0; i < 4; i++)
	{
		if (state->objects[i] != 0)
		{
			ResourceObject_ReleaseFar(state->objects[i]);
			state->objects[i] = 0;
		}
	}
}

void Menu_UpdateSecondObjectRowPositions(void)
{
	struct FourObjectMotionState *root = gMenuWork;
	s32							  scale[2];
	s32							  position[4];
	s32							  index;

	index = 0;
	do
	{
		void *object = root->objects[index];

		if (object != 0)
		{
			s32 unit = 0x10000;
			s32 bias = 0x1f40000;

			scale[0] = unit;
			scale[1] = unit;
			position[0] = root->positions_x[index] << 16;
			position[1] = bias;
			position[2] = (root->positions_y[index] << 16) + bias;
			position[3] = 0;
			Object_ApplyProjectedPlacementFar((s32)object, position, scale, 0x4000);
		}
		index++;
	} while (index <= 3);
}

//--------------------------------------------------------------------------
// draw_four_numbers_in_row.c
// セーブ：記録のジンの数を属性ごとに横に並べて書く
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 4 つの数を 2 けたで、24 ドットおきに y 16 に書く
// win : 書き込み先（0 なら何もしない）
//--------------------------------------------------------------------------
void UiText_DrawFourNumbersInRow(struct RenderInput *win, struct SaveSummary *row)
{
	s32 i;

	if (win != 0)
	{
		for (i = 0; i < 4; i++)
			UiText_DrawNumberAtOffset((s8)row->djinn[i], 2, win, i * 24, 16);
	}
}

//--------------------------------------------------------------------------
// draw_character_summary.c
// セーブ：記録の名前・レベル・クラス・コイン・プレイ時間を書く
//--------------------------------------------------------------------------

// 版ごとの「Lv」と「コイン」の x（ドット）
#if defined(TBS_EDITION_ES) || defined(TLA_EDITION_ES)
#define SUMMARY_LEVEL_X 64
#define SUMMARY_COINS_X 56
#elif defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
#define SUMMARY_LEVEL_X 59
#define SUMMARY_COINS_X 56
#else
#define SUMMARY_COINS_X 48
#endif

//--------------------------------------------------------------------------
// win : 書き込み先（0 なら何もしない）
// rec : セーブの記録
//--------------------------------------------------------------------------
void StatusMenu_DrawCharacterSummary(s32 win, struct SaveSummary *rec)
{
	u32 buf[4]; // プレイ時間の文字列
	s32 y;

	if (win != 0)
	{
		RenderOutput_PrepareForRedraw((struct UiWindow *)win);
		UiWindow_DrawDividerLine(win, 0, 4, 13, 4);
		UiText_DrawStringAtOffset(rec->name, win, 0, 0);
		y = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR)
		UiText_DrawStringAtOffset(StatusMenu_LevelLetterString, win, SUMMARY_LEVEL_X, 0);
#else
		UiText_DrawStringInWindow((s32)StatusMenu_LevelLetterString, win, 72, 0);
#endif
		UiText_DrawNumberAtOffset(rec->level, 2, win, 80, y);
		UiText_DrawCharacterAtOffset(rec->class_id + (s32)MsgClassName, win, 0, 16);
#if !EDITION_INTERNATIONAL
		y = 32;
		UiText_DrawNumberInWindow(rec->coins, 6, win, 24, y);
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, 72, 32);
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 48);
		UiText_DrawStringInWindow((s32)Text_FormatPlayTime(rec->play_time, (u8 *)buf), win, 48, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgStatusLabel, win, 0, 32);
		UiText_DrawStringInWindow((s32)Text_FormatPlayTime(rec->play_time, (u8 *)buf), win, 48, 40);
		y = 48;
		UiText_DrawNumberInWindow(rec->coins, 6, win, 0, y);
#if defined(TBS_EDITION_FR) || defined(TLA_EDITION_FR)
		// フランス語版は 1 枚以下なら単数形
		if (rec->coins > 1)
			UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
		else
			UiText_DrawCharacterAtOffset((s32)MsgPiece, win, SUMMARY_COINS_X, 48);
#else
		UiText_DrawCharacterAtOffset((s32)MsgCoins, win, SUMMARY_COINS_X, 48);
#endif
#endif
	}
}

#if EDITION_INTERNATIONAL

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
	struct UiWindow		 *list;
	struct SaveWorkspace *save;
	struct UiWindow		 *status;
	struct UiWindow		 *djinn;
	struct UiWindow		 *counts;
	struct SaveSummary	 *summary;
	struct RenderOutput	 *cursor;
	s32					  redraw;
	struct UiRenderWork	 *render;
	u32					  stamp;
	void				 *work;
	volatile u32		  zero;
	s32					  result;
	s32					  i;
	s32					  y;

	work = Runtime_AllocateHeapBlock(55, 0xa70);
	save = *(gWorkSlot + HEAP_SLOT_SAVE);
	status = NULL;
	djinn = NULL;
	counts = NULL;
	redraw = 1;
	render = *(gWorkSlot + HEAP_SLOT_WINDOW);
	stamp = Runtime_GetBuildStampTimeFar();
	if (slot < 0)
		slot = 0;
	if (mode == 1)
	{
		for (i = 0;
			i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag != 0);
			i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	else if (mode == 4)
	{
		for (i = 0;
			i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].flag_count == 0);
			i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	else if (mode == 5)
	{
		for (i = 0;
			i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag == 0);
			i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	else if (mode != 0)
	{
		for (i = 0; i < 3 && (save->summary[slot].level == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}

	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
	Scheduler_ScheduleCallbackAAfterFrames();
	list = UiWindow_Create(1, 2, 28, 7, 2);
	summary = save->summary;
	y = 0;
	for (i = 0; i < 3; i++, y += 16, summary++)
	{
		if (summary->level == 0)
		{
			UiText_DrawResource((s32)MsgSaveNoData, list, 10, y);
		}
		else if (summary->frames < stamp)
		{
			UiText_DrawResource((s32)MsgSaveContinue, list, 10, y);
		}
		else if (summary->play_time != *(u32 *)summary->unknown_38)
		{
			UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, y);
		}
		else if (mode == 5 && (s8)summary->send_flag == 0)
		{
			UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, y);
		}
		else
		{
			UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
			UiText_DrawResource(summary->area + (s32)MsgDebugEntryName, list, 62, y);
			render->dirty = 1;
		}
	}
	UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
	UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
	cursor = RenderResource_CreatePair(mode, list, 72, -24);

	while (1)
	{
		if (redraw != 0)
		{
			redraw = 0;
			if (save->summary[slot].level != 0)
			{
				PaletteGlow_Update(
					save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
				if (status == NULL)
					status = UiWindow_Create(1, 10, 14, 9, 2);
				StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
				WaitFrames(1);
				if (djinn == NULL)
					djinn = UiWindow_Create(16, 10, 13, 3, 2);
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
				WaitFrames(1);
				if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1]
						+ (s8)save->summary[slot].djinn[2] + (s8)save->summary[slot].djinn[3]
					!= 0)
				{
					if (counts == NULL)
						counts = UiWindow_Create(16, 14, 13, 5, 2);
					UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
				}
				else
				{
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					UiWork_Finalize(counts, 2);
					counts = NULL;
				}
			}
			else
			{
				PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
				Menu_ClearSecondObjectRowAndScheduleUpdate();
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				UiWork_Finalize(counts, 2);
				UiWork_Finalize(djinn, 2);
				UiWork_Finalize(status, 2);
				counts = NULL;
				djinn = NULL;
				status = NULL;
			}
			RenderOutput_RedrawSavedRect(list);
			UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
			UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
			UiWindow_FillTilemapRect(list, 0, slot * 2, 26, 1);
		}
		Ui_ApplyTableOffsetToPair(cursor);
		WaitFrames(1);
		if (gKeysRepeat & 0x40)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 2) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeysRepeat & 0x80)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 4) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeyState & 2)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		else if (gKeyState & 1)
		{
			AudioCommand_PlayFar(112);
			result = slot;
			break;
		}
	}
	Menu_ClearSecondObjectRowAndScheduleUpdate();
	Menu_ClearFirstObjectRowAndScheduleUpdate();
	UiWork_Finalize(counts, 2);
	UiWork_Finalize(djinn, 2);
	UiWork_Finalize(status, 2);
	UiWork_Finalize(list, 2);
	Scheduler_ScheduleCallbackA();
	Runtime_ReleaseHeapBlock(55);
	PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	WaitFrames(1);
done:
	return result;
}
#elif defined(TBS_EDITION_JA)

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
	struct UiWindow		 *list;
	struct UiWindow		 *status;
	struct SaveWorkspace *save;
	struct UiWindow		 *djinn;
	struct UiWindow		 *counts;
	struct SaveSummary	 *summary;
	struct RenderOutput	 *cursor;
	s32					  kind;
	s32					  redraw;
	struct UiRenderWork	 *render;
	u32					  stamp;
	void				 *work;
	struct ShopCursor	  marker;
	volatile u32		  zero;
	s32					  result;
	s32					  i;
	s32					  x;
	s32					  y;
	s32					  tile;
	struct RenderOutput	 *anchor;

	work = Runtime_AllocateHeapBlock(55, 0xa70);
	save = *(gWorkSlot + HEAP_SLOT_SAVE);
	status = NULL;
	djinn = NULL;
	counts = NULL;
	redraw = 1;
	render = *(gWorkSlot + HEAP_SLOT_WINDOW);
	stamp = Runtime_GetBuildStampTimeFar();
	if (slot < 0)
		slot = 0;
	if (mode != 0)
	{
		for (i = 0; i < 3 && (save->summary[slot].level == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	if (mode == 1)
	{
		for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag != 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	if (mode == 4)
	{
		for (i = 0; i < 3 && ((s8)save->summary[slot].flag_count == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}
	if (mode == 5)
	{
		for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag == 0); i++)
		{
			slot++;
			if (slot == 3)
				slot = 0;
		}
		if (i == 3)
		{
			result = -2;
			goto done;
		}
	}

	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
	list = UiWindow_Create(1, 2, 28, 7, 2);

	for (i = 0, summary = save->summary; i < 3; i++, summary++)
	{
		if (save->summary[i].level == 0)
		{
			UiText_DrawResource((s32)MsgSaveNoData, list, 10, i * 16);
		}
		else if (save->summary[i].frames < stamp)
		{
			UiText_DrawResource((s32)MsgSaveContinue, list, 10, i * 16);
		}
		else if (save->summary[i].play_time != *(u32 *)save->summary[i].unknown_38)
		{
			UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, i * 16);
		}
		else if (mode == 5 && (s8)save->summary[i].send_flag == 0)
		{
			UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, i * 16);
		}
		else
		{
			y = i * 16;
			UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
			UiText_DrawResource(save->summary[i].area + (s32)MsgDebugEntryName, list, 72, y);
			render->dirty = 1;
		}
	}
	UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
	UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
	tile = Resource_FindFreeEntry();
	if (tile < 96)
	{
		VramBlock_LoadCached(tile, 128, Resource_FixedBlockBTiles);
		x = -12;
		y = slot * 16 + 4;
		anchor = RenderOutput_Create(tile, 0x40000000, (struct RenderInput *)list, x, y);
		anchor->active = 1;
		marker.anchor = anchor;
	}
	kind = 1;
	cursor = RenderResource_CreatePair(mode, list, 72, -24);

	while (1)
	{
		if (redraw != 0)
		{
			redraw = 0;
			if (save->summary[slot].level != 0)
			{
				PaletteGlow_Update(
					save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
				if (status == NULL)
					status = UiWindow_Create(1, 10, 14, 9, 2);
				StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
				WaitFrames(1);
				if (djinn == NULL)
					djinn = UiWindow_Create(16, 10, 13, 3, 2);
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
				WaitFrames(1);
				if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1]
						+ (s8)save->summary[slot].djinn[2] + (s8)save->summary[slot].djinn[3]
					!= 0)
				{
					if (counts == NULL)
						counts = UiWindow_Create(16, 14, 13, 5, 2);
					UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
				}
				else
				{
					Menu_ClearSecondObjectRowAndScheduleUpdate();
					UiWork_Finalize(counts, 2);
					counts = NULL;
				}
			}
			else
			{
				PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
				Menu_ClearSecondObjectRowAndScheduleUpdate();
				Menu_ClearFirstObjectRowAndScheduleUpdate();
				UiWork_Finalize(counts, 2);
				UiWork_Finalize(djinn, 2);
				UiWork_Finalize(status, 2);
				counts = NULL;
				djinn = NULL;
				status = NULL;
			}
			x = list->x * 8 - 4;
			y = (list->y << 3) + slot * 16 + 12;
			Shop_SetCursorFar(&marker, x, y, kind);
			kind = 3;
		}
		ShopCursor_AdvanceFar(&marker);
		Ui_ApplyTableOffsetToPair(cursor);
		WaitFrames(1);
		if (gKeysRepeat & 0x40)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 2) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeysRepeat & 0x80)
		{
			AudioCommand_PlayFar(111);
			redraw = 1;
			do
			{
				slot = (slot + 4) % 3;
				if (mode == 0)
					break;
			} while ((save->summary[slot].level == 0
					  || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
					  || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
					  || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
		}
		else if (gKeyState & 2)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		else if (gKeyState & 1)
		{
			AudioCommand_PlayFar(112);
			result = slot;
			break;
		}
	}
	Menu_ClearSecondObjectRowAndScheduleUpdate();
	Menu_ClearFirstObjectRowAndScheduleUpdate();
	UiWork_Finalize(counts, 2);
	UiWork_Finalize(djinn, 2);
	UiWork_Finalize(status, 2);
	UiWork_Finalize(list, 2);
	Runtime_ReleaseHeapBlock(55);
	PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
	WaitFrames(1);
done:
	return result;
}
#endif

s32 Save_WriteSelectedSlot(void)
{
	s32					  result;
	s32					  slot;
	s32					  flag;
	struct SaveWorkspace *work;

	result = 0;
	flag = SaveState_InitializeWorkspace();
	if (flag != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		result = -9;
	}
	else
	{
		SaveState_LoadSummaryRecords();
		work = gSaveWorkspace;
		slot = SaveMenu_SelectSlot(gSaveSlot, 0);
		if (slot == -1)
		{
			result = slot;
		}
		else
		{
			if (work->summary[slot].level != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgOverwriteConfirm, 13);
				while (UiWork_IsComplete() == 0)
				{
					WaitFrames(1);
				}
				if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0)
				{
					UiWork_FinalizePendingCore();
					goto skip;
				}
				UiWork_FinalizePendingCore();
			}
			gSaveSlot = slot;
			AudioCommand_PlayFar(85);
			UiText_ShowPositionedMessageAndWait((s32)MsgSaving, 13);
			while (UiWork_IsComplete() == 0)
			{
				WaitFrames(1);
			}
			SaveState_BuildSummaryHeader();
			SaveState_CaptureObjectTableFar();
			flag = SaveState_WriteRecord(slot, gSaveBuffer);
			flag |= SaveState_WriteRecord(slot + 3, gSaveBuffer + 0x1000);
			UiWork_FinalizePendingCore();
			if (flag != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
				result = -3;
			}
			else
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 9);
			}
		}
	}
skip:
	SaveState_ReleaseWorkspace();
	return result;
}

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
	s32		   ret = 0;
	s32		   err = SaveState_InitializeWorkspace();

	if (err != 0)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
		ret = -9;
	}
	else
	{
		s32 value;

		SaveState_LoadSummaryRecords();
		value = SaveMenu_SelectSlot(gSaveSlot, arg);
		if (value == -1)
		{
			ret = value;
		}
		else
		{
			void *base = &gSaveBuffer;

			err = SaveState_ReadRecordPayload(value, base);
			base = (char *)base + 0x1000;
			err |= SaveState_ReadRecordPayload(value + 3, base);
			if (err != 0)
			{
				UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
				ret = -2;
			}
			else
			{
				gLoadedStateWord = gGameState.play_time;
				gOptionMirror = gGameState.saved_options;
				gPostLoadCounter = 0;
				gSaveSlot = value;
			}
		}
	}
	SaveState_ReleaseWorkspace();
	return ret;
}

s32 Resource_LoadFixedBlockBIntoFreeSlot(void)
{
	s32 slot;

	slot = Resource_FindFreeEntry();
	VramBlock_LoadCached(slot, 0x80, Resource_FixedBlockBTiles);
	return slot;
}

#if EDITION_INTERNATIONAL
s32 Resource_ReturnTrue(void)
{
	return 1;
}
#else

s32 NameEntry_AcceptsMark(s32 mark, s32 letter)
{
	const u8 *range;

	if (mark < 0xde || mark > 223)
		return 1;
	if (mark == 0xde)
		range = NameEntry_DakutenRanges;
	else
		range = NameEntry_HandakutenRanges;
	for (; *range != 0; range += 2)
	{
		if (letter >= range[0] && letter <= range[1])
			return 1;
	}
	return 0;
}

s32 NameEntry_CountLetters(const u8 *name)
{
	s32 count;

	count = 0;
	for (; *name != 0; name++)
	{
		if ((u8)(*name - 0xde) > 1)
			count++;
	}
	return count;
}
#endif

void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source)
{
	struct UiRenderWork *work = UI_WORK;
	s16					*mirror = (s16 *)work->tilemap;
	s16					*buffer = Runtime_BumpAllocateAlternatePool(0x300);
	s16					*input = buffer;
	u32					 cell;
	s16					*vram;
	s32					 row;

	Resource_DecodeType01(source, buffer);
	cell = window->y * 32 + window->x;
	vram = (s16 *)0x06002000 + cell;
	mirror += cell;

	for (row = 0; row < window->height; row++)
	{
		s32 column;

		for (column = 0; column < window->width; column++)
		{
			s16 value = *input++;

			*vram++ = value;
			*mirror++ = value;
		}
		vram += 32 - window->width;
		mirror += 32 - window->width;
	}
	MemFree(buffer);
}

void UiWindow_SetTileAttributeRect(
	const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
	u8 *base = gWindowWork[0];

	x += window->x + 1;
	y += window->y + 1;
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
			u16 *cell = (u16 *)(x + (u32)base);
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
		((struct UiRenderWork *)base)->dirty = 1;
	}
}

void Resource_CopyFixedBlockB(s32 arg0)
{
	VramBlock_LoadCached(arg0, 0x80, Resource_FixedBlockBTiles);
}

void Resource_CopyFixedBlockA(s32 arg0)
{
	VramBlock_LoadCached(arg0, 0x80, Resource_FixedBlockATiles);
}

#if EDITION_INTERNATIONAL
s32 UiText_SetRenderString(const u8 *str)
{
	struct UiRenderWork *work;
	u16					*dst;
	s32					 count;
	u32					 width;
	u32					 height;

	work = UI_WORK;
	count = 0;
	if (*str != 0)
	{
		dst = work->entries;
		do
		{
			*dst = *str;
			str++;
			dst++;
			count++;
		} while (*str != 0);
	}
	work->entries[count] = 0;
	UiText_MeasureEntryDimensions(0, &width, &height, 0);
	return width;
}

void UiText_DrawPaddedLabel(s32 output, u8 *input)
{
	u8	text[20];
	s32 length = 0;

	if (*input != 0)
	{
		do
		{
			text[length] = *input;
			input++;
			length++;
		} while (*input != 0);
	}

	text[length++] = 8;
	text[length++] = 2;

	while (length <= 6)
	{
		text[length++] = 95;
	}

	text[length++] = 8;
	text[length++] = 15;
	text[length] = 0;
	UiText_DrawString(text, output, 0, -2);
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define NAME_LABEL_WIDTH 9
#else
#define NAME_LABEL_WIDTH 8
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
#define NAME_KEY_LABELS 1
#else
#define NAME_KEY_LABELS 0
#endif
#if defined(TBS_EDITION_DE)
#define NAME_END_KEY_WIDTH 4
#else
#define NAME_END_KEY_WIDTH 3
#endif

s32 NameEntry_EditOwnerName(s32 entry)
{
	struct RenderInput	*label;
	s32					 result = 0;
	s32					 original_length = 0;
	s32					 length = 0;
	u8					 text[16];
	u8					*input = text + 1;
	u8					*saved = (u8 *)UnitGetFar(entry);
	u8					*work = gWindowWork[0];
	s32					 text_dirty = 1;
	s32					 cursor_dirty = 1;
	struct RenderInput	*window;
	union MenuCursor	 cursor;
	union MenuCursor	 caret;
	s32					 column, row, slot;
	struct RenderOutput *output;
	u8					*end;
	u16					*pal;

	Ui_LoadWindowGraphics();
	window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 9, 2);
	label = (struct RenderInput *)UiWindow_Create(8, 3, NAME_LABEL_WIDTH, 3, 2);
	UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
	UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
#if NAME_KEY_LABELS
	UiText_DrawStringInWindow(NameEntry_DeleteKeyLabel, window, 144, 32);
	UiText_DrawStringInWindow(NameEntry_EndKeyLabel, window, 144, 40);
#endif
	UiWindow_DrawDividerLine(window, 18, 0, 18, 7);
	((struct UiRenderWork *)work)->dirty = text_dirty;
	text[0] = result;
	{
		s32 i;

		for (i = 0; i < 14; i++)
		{
			u32 c = saved[i];

			input[i] = c;
			if (c != 0)
			{
				original_length++;
				length++;
			}
		}
	}
	input[14] = 0;
	UiText_DrawPaddedLabel((s32)label, saved);
	slot = Resource_FindFreeEntry();
	column = 18;
	row = 5;
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		cursor.output = output;
		ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 + 140, window->y * 8 + 52);
	}
	slot = Resource_FindFreeEntry();
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockATiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		caret.output = output;
		output->sentinel = 255;
		((struct FieldSprite *)output->unknown_10)->priority = 0;
		ShopCursor_SetPositionImmediateFar(&caret, UiText_SetRenderString(input) + 70, 22);
	}
	pal = (u16 *)0x050001c0;
	Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
	pal[4] = 0x6318;
	end = (u8 *)(original_length + (s32)input);
	for (;;)
	{
		s32 width = 1;

		if (column == 18)
		{
			if (row == 4)
				width = 3;
			if (row == 5)
				width = NAME_END_KEY_WIDTH;
		}
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
		WaitFrames(1);
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
		if (cursor_dirty)
		{
			cursor_dirty = 0;
			Shop_SetCursorFar(&cursor, (window->x + column) * 8 - 7, (window->y + row) * 8 + 15, 3);
		}
		if (text_dirty)
		{
			text_dirty = 0;
			Shop_SetCursorFar(&caret, UiText_SetRenderString(input) + 70, 22, 3);
		}
		ShopCursor_AdvanceFar(&cursor);
		ShopCursor_MoveTowardTargetFar(&caret);
		{
			u32 frame = (gFrameTick >> 1) & 7;

			output = caret.output;
			((struct FieldSprite *)output->unknown_10)->x = output->x + NameEntry_CaretWave[frame];
			((struct FieldSprite *)output->unknown_10)->y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row--;
			if (column != 18)
			{
				if (row == -1)
					row = 5;
			}
			else
			{
				row = (row != 3);
				row = 5 - row;
			}
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row++;
			if (column != 18)
			{
				if (row == 6)
					row = 0;
			}
			else
			{
				row = (row != 6);
				row += 4;
			}
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column--;
			if (column == -1)
			{
				column = 18;
				if (row < 4 || row > 5)
					column = 16;
			}
			else if (column == 5 || column == 11 || column == 17)
			{
				column--;
			}
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column++;
			if (column == 19)
				column = 0;
			else if (column == 5 || column == 11 || column == 17)
				column++;
			if (column == 18 && (row < 4 || row > 5))
				column = 0;
		}
		if (gKeyState & KEY_START)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column = 18;
			row = 5;
		}
		if (gKeysRepeat & KEY_B)
		{
			AudioCommand_PlayFar(113);
		remove_character:
			if (length != 0)
			{
				length--;
				*--end = 0;
				RenderOutput_PrepareForRedraw(label);
				UiText_DrawPaddedLabel((s32)label, input);
				text_dirty = 1;
				continue;
			}
			result = -1;
			goto finish;
		}
		if (!(gKeysRepeat & KEY_A))
			continue;
		AudioCommand_PlayFar(112);
		if (column == 18)
		{
			if (row == 5)
			{
				if (length == 0)
				{
					caret.output->active = 13;
					RenderOutput_PrepareForRedraw(label);
					UiText_DrawPaddedLabel((s32)label, saved);
					WaitFrames(10);
				}
				else
				{
					u8 *dst = saved;
					u8 *src = input;
					s32 i = 0;
					u8 *dst_save = dst;
					u8 *src_save = src;

					/* FAKEMATCH: the split restores end the pointer setup's
                     * CSE blocks. They disappear, preserving destination/
                     * source/index in r2/r1/r0; removing the barrier alone
                     * changes them to r1/r0/r2 (14 instructions differ). */
					if (saved)
						src = NULL;
					if (saved)
						src = src_save;
					if (saved)
						dst = NULL;
					if (saved)
						dst = dst_save;
					while (i < 15)
					{
						*dst++ = *src++;
						i++;
					}
				}
				goto finish;
			}
			if (row == 4)
				goto remove_character;
			continue;
		}
		{
			u32 tx = window->x + column + 1;
			u32 cell;
			u32 character;

			cell = ((window->y + row + 1) << 5) + tx;
			character = work[cell * 2];
			if (length == 5)
				continue;
			*end++ = character;
			*end = 0;
			length++;
			if (length == 5)
			{
				column = 18;
				row = 5;
				cursor_dirty = 1;
			}
			RenderOutput_PrepareForRedraw(label);
			UiText_DrawPaddedLabel((s32)label, input);
			text_dirty = 1;
		}
	}
finish:
	UiWork_Finalize((struct UiWindow *)window, 2);
	UiWork_Finalize((struct UiWindow *)label, 2);
	UiWork_FinalizeEntityMatchingLocalizedId(entry);
	WaitFrames(1);
	return result;
}
#else

s32 NameEntry_EditOwnerName(s32 entry)
{
	struct RenderInput	*label;
	s32					 result = 0;
	s32					 keyboard = 0;
	s32					 bytes = 0;
	s32					 length = 0;
	u8					 text[16];
	u8					*input = text + 1;
	u8					*saved = (u8 *)UnitGetFar(entry);
	u8					*work = gWindowWork[0];
	s32					 text_dirty = 1;
	s32					 cursor_dirty = 1;
	struct RenderInput	*window;
	union MenuCursor	 cursor;
	union MenuCursor	 caret;
	s32					 column, row, slot, x;
	struct RenderOutput *output;
	u32					 character;
	u16					*pal;

	Ui_LoadWindowGraphics();
	window = (struct RenderInput *)UiWindow_Create(3, 6, 24, 13, 2);
	label = (struct RenderInput *)UiWindow_Create(8, 3, 8, 3, 2);
	UiWindow_CreateWithSideObject(Localization_LookupEntryId(entry), 0, 3, 1);
	UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
	UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
	((struct UiRenderWork *)work)->dirty = text_dirty;
	text[0] = result;
	{
		s32 i;

		for (i = 0; i < 14; i++)
		{
			character = saved[i];

			input[i] = character;
			if (character != 0)
			{
				bytes++;
				length++;
				if ((u8)(character - 0xde) <= 1)
					length--;
			}
		}
	}
	input[14] = 0;
	UiText_DrawString(saved, (s32)label, 0, 0);
	column = 18;
	slot = Resource_FindFreeEntry();
	row = 10;
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		cursor.output = output;
		ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 + 140, window->y * 8 + 92);
	}
	slot = Resource_FindFreeEntry();
	if (slot <= 95)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockATiles);
		output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		caret.output = output;
		output->sentinel = 255;
		((struct FieldSprite *)output->unknown_10)->priority = 0;
		x = 72 + NameEntry_CountLetters(input) * 10;
		ShopCursor_SetPositionImmediateFar(&caret, x, 22);
	}
	pal = (u16 *)0x050001c0;
	Dma_Set((void *)0x050001e0, pal, 0x84000008, REG_DMA3);
	pal[4] = 0x6318;
	for (;;)
	{
		s32 width = 1;

		if (column == 18)
		{
			if (row == 0)
				width = 4;
			if (row == 8)
				width = 3;
			if (row == 10)
				width = 3;
		}
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 14);
		WaitFrames(1);
		UiWindow_SetTileAttributeRect(window, column, row, width, 1, 15);
		if (cursor_dirty)
		{
			cursor_dirty = 0;
			Shop_SetCursorFar(&cursor, (window->x + column) * 8 - 7, (window->y + row) * 8 + 15, 3);
		}
		if (text_dirty)
		{
			text_dirty = 0;
			x = 72 + NameEntry_CountLetters(input) * 10;
			Shop_SetCursorFar(&caret, x, 22, 3);
		}
		ShopCursor_AdvanceFar(&cursor);
		ShopCursor_MoveTowardTargetFar(&caret);
		{
			u32 frame = (gFrameTick >> 1) & 7;

			output = caret.output;
			((struct FieldSprite *)output->unknown_10)->x = output->x + NameEntry_CaretWave[frame];
			((struct FieldSprite *)output->unknown_10)->y =
				output->y + NameEntry_CaretWave[(frame + 5) & 7];
		}
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row--;
			if (column != 18)
			{
				if (row == -1)
					row = 10;
				else if (row == 5)
					row = 4;
			}
			else
			{
				if (row == -1)
				{
					row = 10;
				}
				else
				{
					row = (row != 7);
					row *= 8;
				}
			}
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			row++;
			if (column != 18)
			{
				if (row == 11)
					row = 0;
				if (row == 5)
					row = 6;
			}
			else
			{
				if (row == 1)
					row = 8;
				else if (row == 9)
					row = 10;
				else
					row = 0;
			}
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column--;
			if (column == -1)
			{
				if (row == 0 || row == 8 || row == 10)
					column = 18;
				else
					column = 16;
			}
			else if (column == 5 || column == 11 || column == 17)
			{
				column--;
			}
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column++;
			if (column == 19)
				column = 0;
			else if (column == 5 || column == 11 || column == 17)
				column++;
			if (column == 18)
			{
				if (row == 0 || row == 8 || row == 10)
					column = 18;
				else
					column = 0;
			}
		}
		if (gKeyState & KEY_START)
		{
			AudioCommand_PlayFar(111);
			cursor_dirty = 1;
			column = 18;
			row = 10;
		}
		if (gKeyState & KEY_SELECT)
		{
			AudioCommand_PlayFar(111);
		change_keyboard:
			keyboard ^= 1;
			if (keyboard)
				UiWindow_CopyTilemapRegion(window, NameEntry_AlternateKeyboardTilemap);
			else
				UiWindow_CopyTilemapRegion(window, NameEntry_KeyboardTilemap);
			UiWindow_DrawDividerLine(window, 18, 0, 18, 12);
			((struct UiRenderWork *)work)->dirty = 1;
			continue;
		}
		if (gKeysRepeat & KEY_B)
		{
			AudioCommand_PlayFar(113);
		remove_character:
			if (length != 0)
			{
				length--;
				bytes--;
				character = input[bytes];
				input[bytes] = 0;
				if ((u8)(character - 0xde) <= 1)
				{
					bytes--;
					input[bytes] = 0;
				}
			}
			else
			{
				result = -1;
				goto finish;
			}
			RenderOutput_PrepareForRedraw(label);
			UiText_DrawString(input, (s32)label, 0, 0);
			text_dirty = 1;
			continue;
		}
		if (!(gKeysRepeat & KEY_A))
			continue;
		AudioCommand_PlayFar(112);
		if (column == 18)
		{
			if (row == 10)
			{
				if (length == 0)
				{
					caret.output->active = 13;
					RenderOutput_PrepareForRedraw(label);
					UiText_DrawString(saved, (s32)label, 0, 0);
					WaitFrames(10);
				}
				else
				{
					u8 *dst;
					u8 *src;
					s32 i;
					u8 *dst_save;
					u8 *src_save;

					dst = saved;
					src = input;
					i = 0;
					/* FAKEMATCH: the split restores end the pointer setup's
                     * CSE blocks. They disappear, preserving destination/
                     * source/index in r1/r2/r0; removing the barrier alone
                     * changes them to r2/r0/r1 (14 instructions differ). */
					dst_save = dst;
					src_save = src;
					if (window->height)
						dst = NULL;
					if (window->height)
						dst = dst_save;
					if (window->height)
						src = NULL;
					if (window->height)
						src = src_save;
					while (i < 15)
					{
						*dst++ = *src++;
						i++;
					}
				}
				goto finish;
			}
			if (row == 8)
				goto remove_character;
			if (row == 0)
				goto change_keyboard;
			continue;
		}
		{
			u32 tx = window->x + column + 1;
			u32 cell;
			u8 *dst;

			cell = ((window->y + row + 1) << 5) + tx;
			character = work[cell * 2];
			if (character == 0xfe)
				character = 0xde;
			if (character == 0xff)
				character = 0xdf;
			if (length == 5 && (u8)(character - 0xde) > 1)
				continue;
			dst = input + bytes;
			if (!NameEntry_AcceptsMark(character, input[bytes - 1]))
			{
				AudioCommand_PlayFar(113);
				continue;
			}
			*dst = character;
			bytes++;
			input[bytes] = 0;
			length++;
			if ((u8)(character - 0xde) <= 1)
				length--;
			if (length == 5)
			{
				column = 18;
				row = 10;
				cursor_dirty = 1;
			}
			RenderOutput_PrepareForRedraw(label);
			UiText_DrawString(input, (s32)label, 0, 0);
			text_dirty = 1;
		}
	}
finish:
	UiWork_Finalize((struct UiWindow *)window, 2);
	UiWork_Finalize((struct UiWindow *)label, 2);
	UiWork_FinalizeEntityMatchingLocalizedId(entry);
	WaitFrames(1);
	return result;
}
#endif

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#define TALK_WINDOW_X 7
#define TALK_WINDOW_WIDTH 18
#elif defined(TBS_EDITION_ES)
#define TALK_WINDOW_X 9
#define TALK_WINDOW_WIDTH 12
#else
#define TALK_WINDOW_X 8
#define TALK_WINDOW_WIDTH 16
#endif

s32 PartyTalkMenu_Choose(s32 unused)
{
	/* FAKEMATCH: the shared cursor union preserves pointer-store ordering; an equal-size struct changes stack-address construction in all six editions. */
	struct RenderInput *window;
	union MenuCursor	cursor;
	s32					slot;
	s32					row;
	s32					moved;
	u16				   *palette;

	moved = 1;
	window = UiWindow_Create(TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7, 2);
	UiText_DrawCharacterAtOffset((s32)MsgTalkChoice, window, 8, 0);
	UiText_DrawCharacterAtOffset((s32)MsgTalkChoice + 1, window, 8, 16);
	UiText_DrawCharacterAtOffset((s32)MsgTalkChoice + 2, window, 8, 32);
	slot = Resource_FindFreeEntry();
	row = 0;
	if (slot < 96)
	{
		VramBlock_LoadCached(slot, 128, Resource_FixedBlockBTiles);
		cursor.output = RenderOutput_Create(slot, 0x40000000, window, 0, 0);
		ShopCursor_SetPositionImmediateFar(&cursor, window->x * 8 - 3, window->y * 8 + 9);
	}
	palette = (u16 *)0x050001c0;
	Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
	palette[4] = 0x6318;
	do
	{
		UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 14);
		WaitFrames(1);
		UiWindow_SetTileAttributeRect(window, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, 15);
		if (moved)
		{
			moved = 0;
			Shop_SetCursorFar(&cursor, window->x * 8 - 3, (window->y + row * 2) * 8 + 9, 3);
		}
		ShopCursor_AdvanceFar(&cursor);
		if (gKeysRepeat & KEY_UP)
		{
			AudioCommand_PlayFar(111);
			row--;
			moved = 1;
			if (row == -1)
				row = 2;
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			AudioCommand_PlayFar(111);
			row++;
			moved = 1;
			if (row == 3)
				row = 0;
		}
		if (gKeysRepeat & KEY_B)
		{
			AudioCommand_PlayFar(113);
			row = -1;
			goto close;
		}
	} while (!(gKeysRepeat & KEY_A));
	AudioCommand_PlayFar(112);
close:
	UiWork_Finalize(window, 2);
	WaitFrames(1);
	Resource_ResetEntry(slot);
	return row;
}

void Djinn_ShowJoinedMessage(s32 pc, s32 element, s32 djinn)
{
	struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
	s32					 name;
	struct UiWindow		*window;
	s32					 sprite[3];
	s32					*entry;
	s32					 palette, handle;
	s32					 id;
	s32					 text;
	u32					 zero;
	s32					*p;

	window = NULL;
	zero = 0;
	name = Djinn_ElementSprites[element & 3];
	entry = sprite;
	window = UiWindow_Create(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

	if (window != NULL)
	{
		UiWindow_DrawDividerLine(window, 4, 0, 4, 4);

		work->dirty = 1;

		id = Localization_LookupEntryId(name);
		UiGlyph_LoadEntryWithPalette(id, zero, &handle, &palette, 14, zero);

		p = entry;
		*p++ = zero;
		*p++ = 0x8000000c | ((DJINN_JOIN_X * 8 + 4) << 16);
		*p = palette | 0xe000;

		work->result[0] = zero;
		work->result[1] = zero;

		UiWork_PushValueSlot(pc, 1);
		UiWork_PushValueSlot(element * 20 + djinn + 300, 4);

		text = UiText_BuildRenderEntriesMode1(element + (s32)MsgVenusDjinnJoined);

		((struct UiChannelSlot *(*)())UiText_QueueRenderEntries)(window, text, 36, 2, zero); /* FAKEMATCH: needed to match: called without the flags argument. */

		AudioCommand_PlayFar(81);

		do
		{
			Runtime_PushSlotEntry(entry, 250);
			WaitFrames(1);
		} while (AudioCommand_GetStateByteFar() != 0
				 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		UiWork_Finalize(window, 2);
		WaitFrames(1);
		Resource_ResetEntry(handle);
	}
}

s32 Party_LookupCharacterValueByFlag32(u32 index)
{
	if (index > 8)
	{
		return 0;
	}
	if (GameFlag_TestFar(32) == 0)
	{
		return Character_SpriteTable[index];
	}
	return Character_ChildSpriteTable[index];
}

void Party_ShowJoinedMessage(s32 member)
{
	struct UiRenderWork *work = (struct UiRenderWork *)*gWindowWork;
	struct UiWindow		*window = NULL;
	s32					 sprite[3];
	s32					*entry = sprite;
	s32					 palette, handle;
	s32					*p;

	window = UiWindow_Create(2, 1, 26, 5, 0);
	if (window != NULL)
	{
		UiWindow_DrawDividerLine(window, 4, 0, 4, 4);
		work->dirty = 1;
		UiGlyph_LoadEntryWithPalette(
			Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(member)), 0, &handle,
			&palette, 14, 0);
		p = entry;
		*p++ = 0;
		*p++ = 0x8000000c | ((2 * 8 + 4) << 16);
		*p = palette | 0xe000;
		work->result[0] = 0;
		work->result[1] = 0;
		UiWork_PushValueSlot(member, 1);
		((struct UiChannelSlot *(*)())UiText_QueueRenderEntries)(window, UiText_BuildRenderEntriesMode1((s32)MsgJoinedParty), 36, 2, 0); /* FAKEMATCH: needed to match: called without the flags argument. */
		AudioCommand_PlayFar(81);
		do
		{
			Runtime_PushSlotEntry(entry, 250);
			WaitFrames(1);
		} while (AudioCommand_GetStateByteFar() != 0
				 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
		UiWork_Finalize(window, 2);
		WaitFrames(1);
		Resource_ResetEntry(handle);
	}
}

void Party_ShowPairJoinedMessage(s32 first, s32 second)
{
	u8				*work = gWindowWork[0];
	struct UiWindow *window;
	s32				 sprite2[3];
	s32				 sprite1[3];
	s32				*entry1;
	s32				*entry2;
	s32				 handle1, palette, handle2;
	u32				 zero;
	s32				*p;

	// the window starts as NULL, though it is assigned before it is read; the same zero is the style argument below
	window = NULL;
	entry1 = sprite1;
	window = UiWindow_Create(1, 1, 28, 5, 0);
	zero = 0;

	if (window != NULL)
	{
		UiWindow_DrawDividerLine(window, 8, 0, 4, 4);

		((struct UiRenderWork *)work)->dirty = 1;

		UiGlyph_LoadEntryWithPalette(
			Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(first)), 0, &handle1,
			&palette, 14, zero);
		p = entry1;
		*p++ = zero;
		*p++ = 0x800c000c;
		*p = palette | 0xe000;

		entry2 = sprite2;
		UiGlyph_LoadEntryWithPalette(
			Localization_LookupEntryId(Party_LookupCharacterValueByFlag32(second)), 0, &handle2,
			&palette, 15, zero);
		p = entry2;
		*p++ = zero;
		*p++ = 0x802c000c;
		*p = palette | 0xf000;

		((struct UiRenderWork *)work)->result[0] = zero;
		((struct UiRenderWork *)work)->result[1] = zero;

		UiWork_PushValueSlot(first, 1);
		UiWork_PushValueSlot(second, 1);

		((struct UiChannelSlot *(*)())UiText_QueueRenderEntries)(window, UiText_BuildRenderEntriesMode1((s32)MsgPairJoinedParty), 68, 2, zero); /* FAKEMATCH: needed to match: called without the flags argument. */

		AudioCommand_PlayFar(81);

		do
		{
			Runtime_PushSlotEntry(entry1, 250);
			Runtime_PushSlotEntry(entry2, 250);
			WaitFrames(1);
		} while (AudioCommand_GetStateByteFar() != 0
				 && (gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		UiWork_Finalize(window, 2);
		WaitFrames(1);
		Resource_ResetEntry(handle1);
		Resource_ResetEntry(handle2);
	}
}

void RenderResource_LoadPair(s32 group_index, s32 resource_index)
{
	void	 *staging_buffer = Runtime_AllocateBlock(14, 0x400);
	const u8 *resource = RenderResource_PairSourceTable[group_index];

	if (resource_index <= 0x5F)
	{
		Resource_DecodeByteLz(resource, staging_buffer);
		VramBlock_LoadCached(resource_index, 0x200, staging_buffer);
		Runtime_ReleaseHeapBlock(14);
	}
}

struct RenderOutput *RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y)
{
	s32					 index;
	struct RenderOutput *first;
	struct RenderOutput *second;

	index = Resource_FindFreeEntry();
	if (index > 95)
		return NULL;

	RenderResource_LoadPair(group, index);
	first = RenderOutput_Create(index, 0x80004000, input, x, y);
	first->sentinel = 0xFD;
	second = RenderOutput_Create(index, 0x80004000, input, x + 32, y);
	second->sentinel = 0xFD;
	second->oam.index += 8;
	return first;
}

static __inline__ u32 XorWord(u32 word, u32 mask)
{
	return word ^ mask;
}

void Ui_ApplyTableOffsetToPair(struct RenderOutput *obj)
{
	s32 value;

	value = (u8)obj->y + Ui_PairBobOffsets[(gFrameTick >> 2) & 7];
	obj->oam.y = value;
	obj = obj->next;
	value = (u8)obj->y + Ui_PairBobOffsets[(*(volatile u32 *)&gFrameTick >> 2) & 7];
	obj->oam.y = value;
}

void RenderResource_LoadFrame(s32 index, s32 value, s32 flag)
{
	s32	  size = 1024;
	void *buffer = Runtime_AllocateBlock(14, size);
	u16	 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

	if (value <= 95)
	{
		Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
		if (flag != 0)
			Runtime_RemapBytesByTableFar(buffer, 768);
		VramBlock_LoadCached(value, size, buffer);
		Runtime_ReleaseHeapBlock(14);
	}
}

struct RenderOutput *RenderResource_CreateFrame(
	s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4)
{
	s32					 index;
	struct RenderOutput *entity;

	index = Resource_FindFreeEntry();
	entity = NULL;
	if (index != 0x60)
	{
		RenderResource_LoadFrame(arg0, index, arg1);
		entity = RenderOutput_Create(index, 0x80000000, arg2, arg3, arg4);
		entity->oam.palette_256 = 1;
		entity->sentinel = 0xfb;
	}
	return entity;
}

void Ui_ApplyTableScaleToObject(struct RenderOutput *obj)
{
	s32					   v = Ui_ObjectPulseScales[(gFrameTick >> 1) & 7];
	struct AffineTransform efx;

	if (v < 0)
		v += 255;
	v >>= 8;

	if (obj != 0)
	{
		efx.scale_x = v;
		efx.scale_y = v;
		efx.angle = 0;
		obj->oam.matrix = AffineMatrix_BuildForEffect(&efx);
		obj->oam.affine_mode = 3;
		obj->oam.x = obj->x + 0xfff0;
		obj->oam.y = (u8)obj->y + 0xf0;
		obj->sentinel = 0xfc;
	}
}

s32 Menu_BuildLocalizedPatternTiles(void)
{
	u32 *vram = (u32 *)0x06006280;
	s32	 set;
	s32	 n;

	for (set = 0; set < 2; set++)
	{
		for (n = 0; n < 6; n++)
		{
			u32 *tile = vram + set * 0x60 + n * 0x10;
			s32	 x;

			Iwram_FillWords(tile, 64, 0x44444444);
			for (x = 1; x <= 7; x++)
			{
				s32 mi = n;

				if (set == 1 && x <= 1)
				{
					continue;
				}
				if (set == 0 && n > x - 2)
				{
					mi = x - 2;
					if (mi < 0)
					{
						mi = 0;
					}
				}
				tile[x] = XorWord(tile[x], Menu_BarPatternRows[mi].word0);
				tile[x + 8] = XorWord(tile[x + 8], Menu_BarPatternRows[mi].word1);
			}
		}
	}
}

int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 unused)
{
	// 値を返さない int 関数 (pop {r1}; bx r1)
	s32 tile_offset = arg3 * 2;
	s32 tile = tile_offset + 0xF315;

	UiWindow_SetTilemapEntry((struct UiWindow *)arg0, 0x400 | tile, arg1, arg2, 0);
	UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile_offset + 0xF314, arg1 + 1, arg2, 0);
	UiWindow_SetTilemapEntry((struct UiWindow *)arg0, tile, arg1 + 2, arg2, 0);
}

void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift)
{
	s32 neg;
	s32 rshift;
	s32 lshift;
	s32 i;

	neg = -shift;
	i = 0;
	rshift = neg * 4;
	lshift = shift * 4;

	do
	{
		u32 result;
		u32 shifted;
		u32 fallback;
		s32 j;

		shifted = *second++;
		result = 0;
		fallback = *first++;
		if (shift < 0)
			shifted >>= rshift;
		else
			shifted <<= lshift;

		j = 7;
		do
		{
			result <<= 4;
			if (shifted > 0x0FFFFFFF)
				result += shifted >> 28;
			else
				result += fallback >> 28;
			j--;
			shifted <<= 4;
			fallback <<= 4;
		} while (j >= 0);

		*output++ = result;
		i++;
	} while (i <= 7);
}

void Link_DrawShiftedTilePair(s32 offset)
{
	s32 phase = (gFrameCount >> 2) & 3;

	if (phase > 2)
	{
		phase = 2;
	}
	if (phase <= 0)
	{
		phase = 1;
	}
	phase++;
	Graphics_MergeShiftedTileRows(
		(u32 *)0x06000220, (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
	Graphics_MergeShiftedTileRows(
		(u32 *)0x06000240, (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}

void Graphics_ExpandVramTilesByColorTable(u16 *dst)
{
	s32 bank = 0;
	s32 pal_ofs = 0;
	s32 dst_bank = 0;

	do
	{
		s32 row = 0;
		s32 dst_ofs = (dst_bank + bank) << 6;

		do
		{
			u16 *out = (u16 *)((u8 *)dst + dst_ofs);
			u16 *src = (u16 *)((u8 *)0x06000600 + (row << 5));
			s32	 col = 0;

			do
			{
				u32 packed = *src++;
				u32 dec = 0;
				s32 nibble = 0;

				do
				{
					u32 color = Graphics_ExpandNibbleTable[(packed & 15) + pal_ofs] << (nibble * 4);
					nibble++;
					packed >>= 4;
					dec |= color;
				} while (nibble <= 3);

				col++;
				*out++ = dec;
			} while (col <= 15);

			row++;
			dst_ofs += 32;
		} while (row <= 9);

		pal_ofs += 16;
		dst_bank += 4;
		bank++;
	} while (bank <= 1);
}

s32 Resource_LoadTableEntryToBuffer(s32 resource, s32 index)
{
	s32			   result;
	GlyphTransfer *work;

	work = Runtime_AllocateBlock(17, sizeof(*work));
	Ui_PrepareTransferFromTableEntry(resource);
	result = Resource_GetBuffer(index, (s32)work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
	return result;
}

s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index)
{
	s32			   result;
	GlyphTransfer *work;

	work = Runtime_AllocateBlock(17, sizeof(*work));
	ItemIcon_Compose(resource, 0x1a);
	result = Resource_GetBuffer(index, (s32)work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
	return result;
}

s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index)
{
	s32			   out;
	s32			   cur;
	s32			   ret;
	GlyphTransfer *work;

	work = Runtime_AllocateBlock(17, sizeof(*work));
	cur = index;
	Ability_LoadGlyph(resource, 0, &cur, &out, 1);
	ret = Resource_GetBuffer(index, (s32)work->tiles);
	Runtime_ReleaseHeapBlock(0x11);
	return ret;
}

s32 Ui_LoadEntryForKind(u32 kind, s32 value)
{
	s32 out;
	s32 cur;
	u32 no;

	no = kind;
	cur = value;
	if (no > 7U)
	{
		no = 0;
	}
	if (GameFlag_TestFar(0x20) != 0)
	{
		switch (no)
		{
		case 0:
			no = 0x38;
			break;
		case 1:
			no = 0x39;
			break;
		}
	}
	UiGlyph_LoadEntryWithPalette(no, 0, &cur, &out, 0xE, 1);
	return out;
}

//--------------------------------------------------------------------------
// get_table_word_zero.c
// ウィンドウ：固定のタイルの表の先頭を返す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Ui_FixedTileBlocks[0] を返す（0 以外の no も 0 として読む）
//--------------------------------------------------------------------------
s32 Ui_GetTableWordZero(s32 no)
{
	if (no != 0)
		no = 0;
	return Ui_FixedTileBlocks[no];
}

void Ui_RunCopiedRoutine(GlyphTransfer *work)
{

	RamCode_LoadSlot(49, Tile_Decompress4bpp, Tile_Decompress4bppCodeSize);
	((void (*)(s32, u8 *))((union HeapState *)gWorkSlot)->slots[49])(
		(s32)work->encoded, (u8 *)work);
	Runtime_ReleaseHeapBlock(49);
}

//--------------------------------------------------------------------------
// create_countdown_label_window.c
// 通信：残り時間の見出しのウィンドウを開く
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 左上に 6x4 のウィンドウを開いて見出しを書き、そのハンドルを返す
//--------------------------------------------------------------------------
s32 Link_CreateCountdownLabelWindow(void)
{
	s32 win = UiWindow_Create(0, 0, 6, 4, 6);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TLA_EDITION_ES) \
	|| defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
	UiText_DrawStringAtOffset(Link_TimeLabelString, win, 0, 0);
#else
	UiText_DrawStringInWindow(Link_TimeLabelString, win, 0, 0);
#endif
	return win;
}

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
	GlyphTransfer *buffer = Runtime_AllocateHeapBlock(0x11, sizeof(GlyphTransfer));
	u16			  *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);
	u8			 **slot = &buffer->encoded;
	void		  *target = (void *)((u32)base + base[arg1]);
	s32			   ret;

	*slot = target;
	Resource_DecodeByteLz(target, buffer);
	ret = Resource_GetBuffer(arg0, (s32)buffer);
	Runtime_ReleaseHeapBlock(0x11);
	return ret;
}

void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile)
{
	GlyphTransfer *decoded;
	u8			  *font;
	u8			  *remapped;
	u8			  *src;
	u8			  *dst;
	s32			   i;
	u32			   index;

	decoded = Runtime_AllocateHeapBlock(17, sizeof(*decoded));
	font = Resource_GetTableEntry((u32)&ResourceId_CommandIcons);
	decoded->encoded = font + ((u16 *)font)[glyph];
	Resource_DecodeByteLz(decoded->encoded, decoded);
	remapped = MemAlloc(0x400);
	src = decoded->input;
	dst = remapped;
	for (i = 0; i < 0x400; i++)
	{
		index = *src++;
		if (palette->slot[index] == 255)
		{
			palette->slot[index] = palette->count;
			if (palette->count < 64)
			{
				((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
				palette->count++;
			}
		}
		*dst++ = palette->slot[index];
	}
	Dma_Set(remapped, (void *)(0x06004000 + tile * 64), 0x84000100, REG_DMA3);
	MemFree(remapped);
	Runtime_ReleaseHeapBlock(17);
}

struct SelectionEntry
{
	struct MenuSprite sprite;
	u16				  resource, unused;
	s32				  pos_x, pos_y;
	s32				  kind;
};

struct SelectionWork
{
	struct GlyphPalette	   palette;
	struct SelectionEntry  entries[6];
	struct UiWindow		  *window;
	s32					   count;
	struct AffineTransform affine;
	s32					   matrix, selection, previous;
	s32					   kinds[6];
};

void UiText_DrawCharacter(struct SelectionWork *work, s32 index, u32 kind)
{
	struct SelectionEntry *entry = &work->entries[index];

	UiText_LoadRemappedGlyph(&work->palette, kind, index * 16);
	work->entries[index].kind = kind;
	MenuSprite_Init(entry->sprite, 0x80002000);
	entry->sprite.oam.f.tile = Resource_LoadIndexedIntoBuffer(work->entries[index].resource, kind);
}

void Graphics_SetBg1Priority3(void)
{
	u32 value = REG_BG1CNT;

	value &= ~3;
	value |= 3;
	REG_BG1CNT = value;
}

void Graphics_ClearBg1ControlBit2(void)
{
	u32 value = REG_BG1CNT;

	value &= ~3;
	REG_BG1CNT = value;
}

void Graphics_InstallBg1VCountHandler(void)
{
	u32 address = 0x04000012;
	s32 zero = 0;

	*(volatile u16 *)address = zero;
	/* FAKEMATCH: needed to match: the do-while keeps the handler load ahead of the index. */
	do
	{
		address = (u32)Graphics_ClearBg1ControlBit2;
	} while (0);
	Runtime_SetIrqHandler(2, 136, (InterruptHandler)address);
}

struct UiWindow *UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y)
{
	struct UiWindow *result;

	result = UiText_OpenMessageWindow(message, x, y, 1);
	while (UiWork_IsComplete() == 0)
		WaitFrames(1);
	return result;
}

#if EDITION_INTERNATIONAL
#define MADO_X 22 /* 窓の X */
#else
#define MADO_X 23
#endif

s32 Ui_RunSelectionScreen(s32 mode)
{
	u8					  party[4];
	struct UiRenderWork	 *render;
	struct SelectionWork *work;
	struct MenuCell		 *runtime;
	s32					  rebuild, trade_count, scale, result;
	s32					  cnt;
	s32					  keys, repeat;
	s32					  slot;
	s32					  i, num;

	render = UI_WORK;
	slot = Resource_LoadIntoFreeSlot(0x400);
	rebuild = 1;
	trade_count = 0;
	work = Runtime_BumpAllocateAlternatePool(sizeof(*work));
	for (i = 255; i >= 0; i--)
		work->palette.slot[i] = 255;
	work->palette.count = 1;
	if (mode == 0)
		work->window = UiWindow_Create(20, 17, 10, 3, 6);
	else
		work->window = UiWindow_Create(MADO_X, 17, 30 - MADO_X, 3, 6);
	work->previous = -1;
	if (mode == 0)
	{
		num = 0;
		work->kinds[num++] = 14;
		work->kinds[num++] = 4;
		work->kinds[num++] = 7;
		work->kinds[num] = -1;
		work->selection = gLinkCountdownWork->command_selection[0];
	}
	else
	{
		work->selection = gLinkCountdownWork->command_selection[1];
		num = 0;
		work->kinds[num++] = 0;
		work->kinds[num++] = 1;
		if (*((s32 *)Trade_GetOfferStateFar(0)) != 0)
		{
			BattlePlacement_CountValidEntriesFar(0, party);
			for (i = 0; i <= 3; i++)
				trade_count += party[i];
			work->kinds[num++] = 15;
			if (trade_count != 0)
				work->kinds[num++] = 16;
		}
		work->kinds[num++] = 2;
		work->kinds[num++] = 3;
		work->kinds[num] = -1;
	}
	for (cnt = 0; cnt <= 5; cnt++)
	{
		work->entries[cnt].resource = slot;
		if (work->kinds[cnt] == -1)
			break;
		UiText_DrawCharacter(work, cnt, work->kinds[cnt]);
	}
	work->count = cnt;
	work->affine.scale_x = 320;
	work->affine.scale_y = 304;
	work->affine.angle = 0;
	cnt = 0;
	if (cnt < work->count)
	{
		do
		{
			s32 x = (cnt - work->count) * 24;
			work->entries[cnt].pos_x = x + 155;
			if (mode != 0)
				work->entries[cnt].pos_x = x + MADO_X * 8 - 5;
			work->entries[cnt].pos_y = 136;
			cnt++;
		} while (cnt < work->count);
	}
	Scheduler_AddOrUpdateCallback(Graphics_SetBg1Priority3, 0x480);
	Runtime_SetIrqHandler(2, 136, Graphics_ClearBg1ControlBit2);

	for (;;)
	{
		scale = Ui_BounceScales[(gFrameCount * 2) & 31];
		scale = (scale - 256) / 4 + 304;
		work->affine.scale_y = work->affine.scale_x = scale;
		gBgScroll[1].x = 0;
		gBgScroll[1].y = 32;
		if (rebuild != 0)
		{
			rebuild = 0;
			render->menu_busy = 1;
			if (mode)
			{
				s32 panel;
				s32 count;
				for (panel = 0; panel < 6 - work->count; panel++)
				{
					for (cnt = 0; cnt <= 2; cnt++)
					{
						s32 col;
						for (col = 0; col <= 2; col++)
							*(render->tilemap + (cnt + 17) * 32 + panel * 3 + MADO_X - 18
								+ (col & 3)) = 0xf07f;
					}
				}
				for (panel = 0; panel < work->count; panel++)
				{
					count = work->count;
					for (cnt = 0; cnt <= 2; cnt++)
					{
						s32 col;
						for (col = 0; col <= 2; col++)
						{
							s32 tmp;
							/* FAKEMATCH: vram, ofs and mask restore two forms the bytes prove. Set ahead of the index, they are three more moves out of the column loop in loop.c's first pass, which drops its threshold so the tile pattern's row part stays in the column loop until the second pass: the row loop reduces the index giv first and copies it first (inits index r6, pattern r5; per row mov ip, r6 and str r5, [sp, #4]). volatile keeps the VRAM base add apart from the store it addresses. Without the three 24, without ofs 564, without mask 470, without volatile 112. */
							volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + MADO_X;
							s32			  ofs = 0x100;
							s32			  mask = 3;
							/* FAKEMATCH: needed to match: tmp, set twice (the tile, then the column's index), is no giv, so the tile is recomputed in the column loop as the bytes show (adds r3, r7, r0; adds r3, r3, r7); 267 differ without it, 846 in both modes. */
							vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp =
								panel * 16 + cnt * 4 + col + ofs;
							tmp = col & mask;
							*(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp - count * 3
								+ MADO_X) = 0;
						}
					}
				}
			}
			else
			{
				s32 panel;
				s32 count;
				for (panel = 0; panel < 6 - work->count; panel++)
				{
					for (cnt = 0; cnt <= 2; cnt++)
					{
						s32 col;
						for (col = 0; col <= 2; col++)
							*(render->tilemap + (cnt + 17) * 32 + panel * 3 + 2 + (col & 3)) =
								0xf07f;
					}
				}
				for (panel = 0; panel < work->count; panel++)
				{
					do { } while (0); /* FAKEMATCH: needed to match: the empty loop restores the CSE block boundary at the panel body's head, so the exit test compares count instead of reloading work->count (223 differ without it). */
					count = work->count;
					for (cnt = 0; cnt <= 2; cnt++)
					{
						s32 col;
						for (col = 0; col <= 2; col++)
						{
							s32 tmp;
							/* FAKEMATCH: as in mode 1: vram, ofs and mask keep the tile pattern in the column loop through loop.c's first pass, volatile keeps the VRAM add apart from its store. */
							volatile u16 *vram = (volatile u16 *)0x0600f800 + 21 * 32 + 20;
							s32			  ofs = 0x100;
							s32			  mask = 3;
							/* FAKEMATCH: needed to match: tmp, as in mode 1 (271 differ without it, 846 in both modes). */
							vram[panel * 3 + cnt * 32 + (col & mask) - count * 3] = tmp =
								panel * 16 + cnt * 4 + col + ofs;
							tmp = col & mask;
							*(render->tilemap + (cnt + 17) * 32 + panel * 3 + tmp - count * 3
								+ 20) = 0;
						}
					}
				}
			}
		}
		if (work->previous != work->selection)
		{
			RenderOutput_PrepareForRedraw(work->window);
			UiText_DrawCharacterAtOffset(work->entries[work->selection].kind + (s32)&MsgCommandName,
				(struct RenderInput *)work->window, 0, 0);
			work->previous = work->selection;
			UiText_DrawCharacter(work, work->selection, work->kinds[work->selection]);
		}
		work->matrix = AffineMatrix_BuildForEffect(&work->affine);
		for (cnt = 0; cnt < work->count; cnt++)
		{
			struct SelectionEntry *entry = &work->entries[cnt];
			if (cnt == work->selection)
			{
				entry->sprite.oam.f.affine_index = work->matrix;
				entry->sprite.oam.f.affine = 3;
				entry->sprite.oam.f.x = entry->pos_x + scale * 7 / 512 - 14;
				entry->sprite.oam.f.y = entry->pos_y + scale * 3 / 256 - 20;
				Runtime_PushSlotEntry(entry, 0xf1);
			}
			else
			{
				entry->sprite.oam.f.x = entry->pos_x;
				entry->sprite.oam.f.y = entry->pos_y;
				entry->sprite.oam.f.affine_index = 0;
				entry->sprite.oam.f.affine = 0;
			}
		}
		keys = gKeyState;
		repeat = gKeysRepeat;
		runtime = gLinkCountdownWork;
		if (runtime->auto_enabled != 0)
		{
			keys = repeat = 0;
			if (runtime->auto_delay == 0)
			{
				if (runtime->auto_mode == 1)
				{
					if (work->kinds[work->selection] == 3)
						keys = repeat = 1;
					else
						keys = repeat = 32;
					runtime->auto_delay = 30;
				}
				else if (runtime->auto_mode == 0)
				{
					s32 kind = work->kinds[work->selection];
					if (kind == 16 || (trade_count == 0 && kind == 15))
					{
						if (runtime->auto_enabled == 1)
						{
							struct UiWindow *message;
							if (kind == 15)
								message = UiText_ShowMessageAndWaitComplete(
									(s32)&MsgDjinnTutorialUnleashCommand, 15, 8);
							else if (kind == 16)
								message = UiText_ShowMessageAndWaitComplete(
									(s32)&MsgDjinnTutorialSummon, 15, 8);
							BattlePresentation_WaitForPromptAtFar(102, 155);
							UiWork_Finalize(message, 1);
							runtime->auto_enabled++;
							runtime->auto_delay = 45;
						}
						else
						{
							keys = repeat = 1;
							runtime->auto_delay = 200;
						}
					}
					else
					{
						keys = repeat = 16;
						runtime->auto_delay = 40;
					}
				}
				else
				{
					runtime->auto_delay = 60;
					keys = repeat = 1;
				}
			}
			else
			{
				runtime->auto_delay--;
			}
		}
		if ((keys & 0x300) && mode != 0)
		{
			result = -2;
			break;
		}
		if (keys & 1)
		{
			result = work->kinds[work->selection];
			break;
		}
		if (mode != 0 && (keys & 2))
		{
			AudioCommand_PlayFar(0x71);
			result = -1;
			break;
		}
		if (repeat & 0x90)
		{
			AudioCommand_PlayFar(0x6f);
			work->selection = (work->selection + 1) % work->count;
		}
		else if (repeat & 0x60)
		{
			AudioCommand_PlayFar(0x6f);
			work->selection = (work->selection + work->count - 1) % work->count;
		}
		else if (gLinkCountdownWork->active == 0)
		{
			result = work->kinds[0];
			break;
		}
		QueueIoWriteDelay2(0x04000000, 0x1741);
		render->menu_busy = 0;
		WaitFrames(1);
	}
	if (mode != 0)
		gLinkCountdownWork->command_selection[1] = work->selection;
	else
		gLinkCountdownWork->command_selection[0] = work->selection;
	for (cnt = 0; cnt < work->count; cnt++)
		Resource_ResetEntry(work->entries[cnt].resource);
	render->menu_busy = 1;
	UiWork_Finalize(work->window, 1);
	{
		s32 panel;
		for (panel = 0; panel <= 6; panel++)
		{
			for (cnt = 0; cnt <= 2; cnt++)
			{
				s32 col;
				for (col = 0; col <= 2; col++)
					*(volatile u16 *)(render->tilemap + (cnt + 17) * 32 + panel * 3 + 5 + (col & 3)) = 0xf07f; /* FAKEMATCH: the add of 0x44a stays apart from the store it addresses (adds r3, r7; strh r6, [r3]); as a plain store combine folds it into strh r6, [r3, r7], 0 -> 33 */
			}
		}
	}
	render->dirty = 1;
	WaitFrames(1);
	Scheduler_RemoveCallback(Graphics_SetBg1Priority3);
	Runtime_SetIrqHandler(2, 0, 0);
	IO_WRITE_QUEUE_PUSH(0x04000000, 0x1541, 0x20000);
	MemFree(work);
	render->menu_busy = 0;
	WaitFrames(1);
	return result;
}

void Ui_SetRectHighlight(s32 x, s32 y, s32 width, s32 height, s32 alt)
{
	struct UiRenderWork *render = UI_WORK;

	alt &= 1;
	alt <<= 12;

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
		do
		{
			u16 *p = &((u16(*)[32])render->tilemap)[y][x];
			s32	 col = width;

			if (col != 0)
			{
				do
				{
					u32 tile = *p;
					tile = (tile & ~0x1000) | alt;
					col--;
					*p = tile;
					p++;
				} while (col != 0);
			}

			UiRender_MarkRow(render, y);
			height--;
			y++;
		} while (height != 0);
	}
}

s32 BattleLayout_HighlightPartyPanels(u16 *ids)
{
	struct BtlWork *battle = BtlWk;
	s32				count;
	s32				i;
	s32				j;

	count = BattleParty_ListActorIdsFar(1, 0);
	Ui_SetRectHighlight(29 - count * 6, 0, 25, 5, 15);
	i = 0;
	if (ids[0] != 0xff)
	{
	next_id:
		for (j = 0; j < 4 && battle->party_units[j] != ids[i]; j++)
		{
			if (battle->party_units[j] == 0xff)
			{
				j = 4;
				break;
			}
		}
		if (j != 4)
			Ui_SetRectHighlight(29 - (count - j) * 6, 0, 7, 5, 14);
		if (++i < 4 && ids[i] != 0xff)
			goto next_id;
	}
	return 0;
}

s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
	// 値を返さない int 関数 (pop {r1}; bx r1)
	s16 *dst;
	s32	 n;
	u8	*src;

	dst = out;
	Text_FormatSignedDecimalToWork(value);
	src = (u8 *)((u32)&gNumberTextBuffer);
	n = 0xD;
	do
	{
		n -= 1;
		*dst = *src;
		src += 1;
		dst += 1;
	} while (n >= 0);
}

s32 DjinnMenu_ListChangedDjinn(const struct OwnerActionSlot *oldGrid,
	const struct OwnerActionSlot *newGrid, u16 *out, s32 *gained, s32 *lost)
{
	s32 i;
	s32 j;
	s32 count;
	s32 added;
	s32 removed;

	added = 0;
	removed = 0;
	count = 0;
	for (i = 0; i < 32 && newGrid[i].encoded_action != 0; i++)
	{
		if (BtlActGet(newGrid[i].encoded_action)->tgt_flg & 0x80)
		{
			out[count] = newGrid[i].encoded_action & ACTION_ID_MASK;
			count++;
			for (j = 0; j < 32; j++)
			{
				if (((newGrid[i].encoded_action ^ oldGrid[j].encoded_action) & ACTION_ID_MASK) == 0)
					break;
			}
			if (j == 32)
			{
				added++;
				out[count - 1] |= 0x8000;
			}
		}
	}
	for (i = 0; i < 32 && oldGrid[i].encoded_action != 0; i++)
	{
		if (BtlActGet(oldGrid[i].encoded_action)->tgt_flg & 0x80)
		{
			for (j = 0; j < 32; j++)
			{
				if (((oldGrid[i].encoded_action ^ newGrid[j].encoded_action) & ACTION_ID_MASK) == 0)
					break;
			}
			if (j == 32)
			{
				removed++;
				out[count] = (oldGrid[i].encoded_action & ACTION_ID_MASK) | 0x4000;
				count++;
			}
		}
	}
	*gained = added;
	*lost = removed;
	return count;
}

//--------------------------------------------------------------------------
// create_output_from_resource_slot.c
// 表示：わざのアイコンをスプライトとしてウィンドウに置く
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// あいている VRAM キャッシュにアイコンを読み、16x16 のスプライトで出す
// （あきがなければ何もしない）
// input : 置く先のウィンドウ
// x, y  : ウィンドウの中の位置（ドット）
// abil  : わざの番号
//--------------------------------------------------------------------------
s32 Ui_CreateOutputFromResourceSlot(struct RenderInput *input, s32 x, s32 y, s32 abil)
{
	s32 slot;

	slot = Resource_FindFreeEntry();
	if (slot != VRAM_CACHE_ENTRY_COUNT)
	{
		Resource_LoadIndexedEntryToBuffer(abil, slot);
		RenderOutput_Create(slot, 0x40000000, input, x, y); // OBJ サイズ 16x16
	}
}

struct RenderInput *DjinnMenu_ShowChangePreview(
	struct RenderInput *win, s32 owner, s32 code, s32 page, s32 *pageCount)
{
	struct BtlUnit *unit;
	struct BtlUnit *snap;
	struct BtlAct  *info;
	s16			   *buf;
	s32				oldHp;
	s32				oldPp;
	s32				oldAtk;
	s32				oldDef;
	s32				oldAgi;
	u16			   *list;
	s32				total;
	s32				n;
	s32				marks;
	s32				rows;
	s32				col;
	s32				row;
	s32				pages;
	s32				gained;
	s32				lost;
	s32				tile;
	s32				val;
	s32				i;

	void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising)
	{
		struct RenderOutput	 *output = RenderOutput_AcquireFree();
		struct PreviewSprite *entry;
		u32					  zero;
		if (output)
		{
			output->active = 1;
			output->kind = 1;
			/* FAKEMATCH: a char store (alias set 0) keeps the index store ahead of the attribute word in sched2 */
			*(s8 *)&output->index = Resource_LoadIntoFreeSlot(128);
			entry = (struct PreviewSprite *)((u8 *)output + 16);
			output->sentinel = 240;
			output->x = 120;
			output->y = 120;
			/* FAKEMATCH: typed like the window pointer, the word store waits for the window load but not for the x and y stores, so sched2 places the entry copy before them */
			*(struct RenderInput **)&entry->attributes = (struct RenderInput *)0x40000400;
			zero = 0;
			/* FAKEMATCH: a pointer-typed clear does not wait for the x and y stores (sched2 order) */
			*(void **)&entry->tile = (void *)zero;
			entry->attributes.bits.x = win->x * 8 + x;
			entry->attributes.bits.y = win->y * 8 + y;
			/* FAKEMATCH: advancing and restoring the captured pointer keeps
             * the window chain in r9. Without the pair, r8/r9 allocation
             * changes and two extra instructions remain (80 -> 82, 22 rows). */
			win++;
			win--;
			entry->tile.bits.index = Resource_GetBuffer((u8)output->index,
				(s32)(rising ? DjinnMenu_ArrowUpTiles : DjinnMenu_ArrowDownTiles));
			RenderOutput_AppendToList((struct RenderOutputList *)win, output);
		}
	}

	unit = UnitGetFar(owner);
	if (win != 0)
		UiWork_Finalize(win, 1);

	if (page == 0)
		win = UiWindow_Create(0, 8, 21, 11, 6);
	else
		win = UiWindow_Create(0, 5, 21, 14, 6);
	if (win == 0)
		return 0;

	buf = MemAlloc(128);
	snap = MemAlloc(332);
	list = Runtime_BumpAllocateAlternatePool(96);
	Iwram_CopyWords(snap, unit, 332);

	col = (code >> 8) & 15;
	row = code & 255;
	if (Djinn_IsActiveFar(owner, col, row) != 0)
		Djinn_DeactivateFar(owner, col, row);
	else
		Djinn_ActivateFar(owner, col, row);
	UnitCalcFar(owner);

	total =
		DjinnMenu_ListChangedDjinn(snap->action_slots, unit->action_slots, list, &gained, &lost);
	pages = __divsi3(total - 1, 5) + 1;
	*pageCount = pages;
	if (page * 5 - 5 >= total)
		page = pages;

	if (page == 0)
	{
		s32 label = (s32)MsgPreviewStatLabel;

		UiText_DrawCharacterAtOffset(label, win, 0, 8);
		UiText_DrawCharacterAtOffset(label + 1, win, 0, 16);
		UiText_DrawCharacterAtOffset(label + 2, win, 0, 24);
		UiText_DrawCharacterAtOffset(label + 3, win, 0, 32);
		UiText_DrawCharacterAtOffset(label + 4, win, 0, 40);
		UiText_DrawCharacterAtOffset(label + 5, win, 0, 48);

		oldHp = snap->hp;
		UiText_FormatNumberToHalfwords(buf, oldHp);
#if !EDITION_INTERNATIONAL
		UiText_RenderWideStringInWindow(buf + 7, win, 4, 1);
#else
		/* FAKEMATCH: UiText_RenderWideStringInWindow is called as s32 (*)(s16 *, void *, s32, s32); as void (*)(s16 *, struct UiWindow *, s32, s32), its prototype, DjinnMenu_ShowChangePreview has 4 of 1588 bytes different */
		((s32 (*)(s16 *, void *, s32, s32))UiText_RenderWideStringInWindow)(buf + 7, win, 5, 1);
#endif
		oldPp = snap->ep;
		UiText_FormatNumberToHalfwords(buf, oldPp);
#if !EDITION_INTERNATIONAL
		/* FAKEMATCH: UiText_RenderWideStringInWindow is called as s32 (*)(s16 *, void *, s32, s32); as void (*)(s16 *, struct UiWindow *, s32, s32), its prototype, DjinnMenu_ShowChangePreview has 4 of 1584 bytes different */
		((s32 (*)(s16 *, void *, s32, s32))UiText_RenderWideStringInWindow)(buf + 7, win, 4, 2);
#else
		UiText_RenderWideStringInWindow(buf + 7, win, 5, 2);
#endif
		oldAtk = snap->atk;
		UiText_FormatNumberToHalfwords(buf, oldAtk);
#if !EDITION_INTERNATIONAL
		UiText_RenderWideStringInWindow(buf + 7, win, 4, 3);
#else
		UiText_RenderWideStringInWindow(buf + 8, win, 6, 3);
#endif
		oldDef = snap->def;
		UiText_FormatNumberToHalfwords(buf, oldDef);
#if !EDITION_INTERNATIONAL
		/* FAKEMATCH: UiText_RenderWideStringInWindow is called as s32 (*)(s16 *, void *, s32, s32); as void (*)(s16 *, struct UiWindow *, s32, s32), its prototype, DjinnMenu_ShowChangePreview has 4 of 1584 bytes different */
		((s32 (*)(s16 *, void *, s32, s32))UiText_RenderWideStringInWindow)(buf + 7, win, 4, 4);
#else
		/* FAKEMATCH: UiText_RenderWideStringInWindow is called as s32 (*)(s16 *, void *, s32, s32); as void (*)(s16 *, struct UiWindow *, s32, s32), its prototype, DjinnMenu_ShowChangePreview has 4 of 1588 bytes different */
		((s32 (*)(s16 *, void *, s32, s32))UiText_RenderWideStringInWindow)(buf + 8, win, 6, 4);
#endif
		oldAgi = snap->agi;
		UiText_FormatNumberToHalfwords(buf, oldAgi);
#if !EDITION_INTERNATIONAL
		/* FAKEMATCH: UiText_RenderWideStringInWindow is called as s32 (*)(s16 *, void *, s32, s32); as void (*)(s16 *, struct UiWindow *, s32, s32), its prototype, DjinnMenu_ShowChangePreview has 4 of 1584 bytes different */
		((s32 (*)(s16 *, void *, s32, s32))UiText_RenderWideStringInWindow)(buf + 7, win, 4, 5);
#else
		/* FAKEMATCH: UiText_RenderWideStringInWindow is called as s32 (*)(s16 *, void *, s32, s32); as void (*)(s16 *, struct UiWindow *, s32, s32), its prototype, DjinnMenu_ShowChangePreview has 4 of 1588 bytes different */
		((s32 (*)(s16 *, void *, s32, s32))UiText_RenderWideStringInWindow)(buf + 8, win, 6, 5);
#endif
		UiText_FormatNumberToHalfwords(buf, snap->luck);
		/* FAKEMATCH: UiText_RenderWideStringInWindow is called as s32 (*)(s16 *, void *, s32, s32); as void (*)(s16 *, struct UiWindow *, s32, s32), its prototype, DjinnMenu_ShowChangePreview has 6 of 1584 bytes different */
		((s32 (*)(s16 *, void *, s32, s32))UiText_RenderWideStringInWindow)(buf + 8, win, 5, 6);

		UiWindow_DrawDividerLine(win, 0, 8, 19, 8);
		if (gained != 0 || lost != 0)
			UiWork_SetParamNibble(2);
#if !EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffset((s32)MsgBattleAdditionalPsynergyHelp, win, 40, 64);
#else
		UiText_DrawCharacterAtOffset((s32)MsgBattleAdditionalPsynergyHelp, win, 24, 64);
#endif
		UiWork_SetParamNibble(15);
	}

	if (page > 0)
	{
		marks = 0;
		rows = total;
		if (total > 4)
			rows = 5;
		i = page * 5 - 5;
		for (n = 0; n < rows && i < total; n++, i++)
		{
			info = BtlActGet(list[i]);
			if (info->damage_class != 4)
				UiWindow_SetTilemapEntry(win, 0x5001 + info->damage_class, 15, n * 2, 0);

			tile = info->rng;
			if (tile == 255)
				tile = 11;
			else
				tile--;
			UiWindow_DrawThreeTileColumn((s32)win, 16, n * 2, tile, 0);

			Ui_CreateOutputFromResourceSlot(win, 0, n * 16 - 4, list[i] & 0x3fff);
			if ((list[i] & 0x8000) != 0)
				UiWork_SetParamNibble(4);
			else if ((list[i] & 0x4000) != 0)
				UiWork_SetParamNibble(2);
			else
				UiWork_SetParamNibble(15);

			UiText_DrawCharacterAtOffset((list[i] & 0x3fff) + (s32)MsgAbilityName, win, 16, n * 16);
			UiWindow_SetTilemapEntry(win, 0xf01f, 11, n * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			UiWindow_SetTilemapEntry(win, 0xf005, 12, n * 2, 0);
#else
			UiWindow_SetTilemapEntry(win, 0xf01e, 12, n * 2, 0);
#endif
			UiText_DrawNumberInWindow(BtlActGet(list[i])->pp_cost, 2, win, 104, n * 16);
		}

		if (gained != 0)
		{
			UiWork_SetParamNibble(4);
			UiText_DrawCharacterAtOffset((s32)MsgPsynergyGained, win, 32, 80);
			marks = 1;
		}
		if (lost != 0)
		{
			UiWork_SetParamNibble(2);
			UiText_DrawCharacterAtOffset((s32)MsgPsynergyLost, win, 32, marks * 8 + 80);
			marks++;
		}
		if (marks == 0)
			UiText_DrawCharacterAtOffset((s32)MsgPsynergyUnchanged, win, 32, 80);
		UiWork_SetParamNibble(15);
		UiWork_SetParamNibble(15);
		UiWindow_DrawDividerLine(win, 0, 10, 19, 10);
	}

	if (page == 0)
	{
		UiText_DrawCharacterAtOffset((s32)MsgClassName + snap->class_index, win, 0, 0);
		UiText_DrawCharacterAtOffset((s32)MsgClassName + unit->class_index, win, 80, 0);
		if (snap->class_index != unit->class_index)
			UiWindow_SetTilemapEntry(win, 0xf728, 9, 0, page);
		else
			UiWindow_SetTilemapEntry(win, 0xf729, 9, 0, page);

		UiText_FormatNumberToHalfwords(buf, unit->hp);
		UiText_RenderWideStringInWindow(buf + 7, win, 11, 1);
		if (unit->hp != oldHp)
		{
			val = 0;
			if (unit->hp > oldHp)
				val = 1;
			DjinnMenu_DrawStatArrow(80, 14, val);
		}

		UiText_FormatNumberToHalfwords(buf, unit->ep);
		UiText_RenderWideStringInWindow(buf + 7, win, 11, 2);
		if (unit->ep != oldPp)
		{
			val = 0;
			if (unit->ep > oldPp)
				val = 1;
			DjinnMenu_DrawStatArrow(80, 22, val);
		}

		UiText_FormatNumberToHalfwords(buf, unit->atk);
		UiText_RenderWideStringInWindow(buf + 7, win, 11, 3);
		if (unit->atk != oldAtk)
		{
			val = 0;
			if (unit->atk > oldAtk)
				val = 1;
			DjinnMenu_DrawStatArrow(80, 30, val);
		}

		UiText_FormatNumberToHalfwords(buf, unit->def);
		UiText_RenderWideStringInWindow(buf + 7, win, 11, 4);
		if (unit->def != oldDef)
		{
			val = 0;
			if (unit->def > oldDef)
				val = 1;
			DjinnMenu_DrawStatArrow(80, 38, val);
		}

		UiText_FormatNumberToHalfwords(buf, unit->agi);
		UiText_RenderWideStringInWindow(buf + 7, win, 11, 5);
		if (unit->agi != oldAgi)
		{
			val = 0;
			if (unit->agi > oldAgi)
				val = 1;
			DjinnMenu_DrawStatArrow(80, 46, val);
		}

		UiText_FormatNumberToHalfwords(buf, unit->luck);
		UiText_RenderWideStringInWindow(buf + 8, win, 12, 6);
		if (unit->luck != snap->luck)
		{
			val = 0;
			if (unit->luck > snap->luck)
				val = 1;
			DjinnMenu_DrawStatArrow(80, 54, val);
		}
	}

	Iwram_CopyWords(unit, snap, 332);
	MemFree(list);
	MemFree(snap);
	MemFree(buf);
	return win;
}

#if EDITION_INTERNATIONAL
#define OWNER_STATUS_TEXT_MAX 0x80
#define OWNER_STATUS_VALUE_X 184
#else
#define OWNER_STATUS_TEXT_MAX 0x40
#define OWNER_STATUS_VALUE_X 176
#endif

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
#define OWNER_STATUS_LABEL(row) ((s32)MsgOwnerStatusAttack + (row))
#else
#define OWNER_STATUS_LABEL(row) ((s32)MsgOwnerStatusExp - 10 + (row))
#endif

struct StatusCursor
{
	s8	pos[5];
	s32 entry;
	s32 lastEntry;
	s32 mode;
	s32 limit;
	s32 col;
	s32 row;
};

s32 Ui_RunOwnerStatusScreen(u16 *list, s32 listCount, s32 owner)
{
	struct MenuSprite					objFace;
	struct MenuSprite					objCursor;
	s32									tbl[11];
	s8									sel[11];
	struct MenuSprite					objIcon[11];
	struct MenuSprite					objName;
	struct StatusCursor					cur;
	struct UiRenderWork				   *base;
	struct UiWindow					   *winMain;
	struct UiWindow					   *winDesc;
	s32									idx;
	s32									handleCursor;
	struct MenuPoint					pix;
	s32									handleFrame;
	s32									cnt;
	s32									extended;
	s32									keys;
	u16								   *text;
	s32									iconIdx;
	s32								   *q;
	s32									defDelta;
	s32									i;
	s32									j;
	s32									atkDelta;
	s32									redraw;
	struct BtlUnit					   *object;
	struct BtlUnit					   *saved;
	const struct OwnerStatusStatCursor *stat;
	struct MenuSprite				   *p;
	s32									agiDelta;
	s32									code;
	s32									n;
	struct MenuCell					   *screen;
	s32									x;

	base = UI_WORK;
	redraw = 1;
	idx = -1;
	handleFrame = Resource_LoadIntoFreeSlot(0x200);
	cnt = 0;
	extended = Party_SumDjinnCountsFar(-1);

	screen = ((struct MenuCell *)gWindowWork[42]);
	screen->busy = 1;
	if (screen->work != NULL)
	{
		UiWork_Finalize(screen->work, 1);
		screen->work = NULL;
	}
	AudioCommand_PlayFar(0x70);

	for (i = 4; i >= 0; i--)
	{
		cur.pos[i] = 0;
	}
	cur.entry = 0;
	cur.lastEntry = 0;
	cur.mode = 0;

	handleCursor = Resource_LoadIntoFreeSlot(0x80);
	for (i = 0; i <= 10; i++)
	{
		tbl[i] = Resource_LoadIntoFreeSlot(0x80);
		sel[i] = -1;
	}

	if (listCount != 0)
	{
		for (i = 0; i <= 5; i++)
		{
			s32 kind = list[i];

			if (kind == 0xFF)
			{
				break;
			}
			if (kind != 0xFE)
			{
				if (kind == owner)
				{
					idx = i;
					break;
				}
			}
		}
	}

	pix.x = 0;
	pix.y = 0;
	winMain = UiWindow_Create(0, 0, 30, 20, 6);
	winDesc = UiWindow_Create(0, 14, 30, 6, 10);
	UiWindow_MarkVisibleTileAttributes();

	for (;;)
	{
		keys = gKeysRepeat;

		objName.oam.word.attr01 = 0x80000400;
		objName.oam.word.attr23 = 0;
		objName.oam.f.tile = Ui_LoadEntryForKind(owner, handleFrame);
		objName.oam.f.x = 8;
		objName.oam.f.y = 24;
		objName.oam.f.palette = 14;
		Runtime_PushSlotEntry((s32 *)&objName, 240);

		switch (cur.entry)
		{
		case 0:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		default:
			cur.mode = 0;
			if (extended != 0)
			{
				cur.limit = 9;
			}
			else
			{
				cur.limit = 7;
			}
			break;
		case 10:
		case 11:
		case 12:
		case 13:
			cur.mode = 1;
			cur.limit = 4;
			break;
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
			cur.mode = 2;
			cur.limit = cnt;
			break;
		}

		Ui_SetRectHighlight(winMain->x, winMain->y, winMain->width, winMain->height, 15);

		if (cnt != 0)
		{
			if ((u32)cur.mode <= 1)
			{
				n = cur.pos[cur.mode];
				if ((keys & 0x80) != 0)
				{
					keys = 0;
					AudioCommand_PlayFar(0x6F);
					n++;
					if (n >= cur.limit)
					{
						n = 0;
						if (cur.mode == 1)
						{
							cur.mode = 2;
							n = cur.pos[cur.mode];
						}
					}
				}
				else if ((keys & 0x40) != 0)
				{
					keys = 0;
					AudioCommand_PlayFar(0x6F);
					n--;
					if (n < 0)
					{
						n = cur.limit - 1;
						if (cur.mode == 1)
						{
							cur.mode = 2;
							n = cur.pos[cur.mode];
						}
					}
				}
				else if ((keys & 0x31) != 0)
				{
					keys = 0;
					AudioCommand_PlayFar(0x6F);
					cur.mode ^= 2;
					if ((gKeysRepeat & KEY_A) != 0)
					{
						n = cur.pos[cur.mode];
					}
					else if ((gKeysRepeat & KEY_LEFT) != 0)
					{
						n = cnt - 1;
					}
					else
					{
						n = 0;
					}
				}
				cur.pos[cur.mode] = (s8)n;
			}
			else if (cur.mode == 2)
			{
				n = cur.pos[cur.mode];
				if (n >= cur.limit)
				{
					n = cur.limit - 1;
				}
				if (n < 0)
				{
					cur.mode = 0;
					n = cur.pos[0];
				}
				else if ((keys & 0x10) != 0)
				{
					keys = 0;
					n++;
					if (n >= cur.limit)
					{
						cur.mode = 0;
						n = cur.pos[0];
					}
					AudioCommand_PlayFar(0x6F);
				}
				else if ((keys & 0x20) != 0)
				{
					keys = 0;
					n--;
					if (n < 0)
					{
						cur.mode = 0;
						n = cur.pos[0];
					}
					AudioCommand_PlayFar(0x6F);
				}
				else if ((keys & 0xC1) != 0)
				{
					keys = 0;
					cur.mode = 0;
					n = cur.pos[0];
					AudioCommand_PlayFar(0x6F);
				}
				cur.pos[cur.mode] = (s8)n;
			}
		}

		if (cur.mode == 0)
		{
			s32 k = cur.pos[cur.mode];

			if (extended == 0)
			{
				k += 9;
			}
			stat = &OwnerStatus_StatCursors[k];
			cur.entry = stat->entry;
			cur.col = stat->column;
			cur.row = stat->row;
			Ui_SetRectHighlight(winMain->x + stat->highlight_column + 1,
				winMain->y + stat->highlight_row + 1, stat->highlight_width, 1, 14);
		}
		else if (cur.mode == 1)
		{
			cur.entry = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].entry;
			cur.col = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].column;
			cur.row = OwnerStatus_EquipmentCursors[cur.pos[cur.mode]].row;
		}
		else if (cur.mode == 2)
		{
			cur.entry = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].entry;
			cur.col = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].column;
			cur.row = OwnerStatus_ConditionCursors[cur.pos[cur.mode]].row;
		}

		if (cur.lastEntry != cur.entry)
		{
			cur.lastEntry = cur.entry;
			redraw = 2;
		}
		pix.x = cur.col * 8;
		pix.y = cur.row * 8;

		{
			struct MenuSprite *face = GetBattleObjectSlotFar(owner)->object->records;

			objFace.oam.word.attr01 = 0xC0002400;
			objFace.oam.word.attr23 = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			/* FAKEMATCH: the zero lives in sl here; sched2 puts the attr23 store before the move into sl and the 0xFC00 load before the face tile load, which may alias that store; native moves into sl between the two stores and loads the tile first (4 instructions) */
			asm("");
#endif
			objFace.oam.f.tile = face->oam.f.tile;
			objFace.oam.f.x = 0xAC;
			objFace.oam.f.y = 56;
			Runtime_PushSlotEntry((s32 *)&objFace, 240);
		}

		objCursor.oam.word.attr01 = 0x40000400;
		objCursor.oam.word.attr23 = 0;
		/* FAKEMATCH: arm_reorg counts this empty asm as 4 bytes, so the gFrameCount literal falls past the face x literal's 64-byte ldrh range and opens the next pool, as native; the pool and its branch move two instructions earlier, 85 -> 63. */
		/* FAKEMATCH: removing both barriers gives EN 1392/1392, common 1390, differ 4: gFrameCount joins the earlier pool and the branch follows two extra loads. ES 1458/1458, common 1455, differ 6: attr23/mov sl order and the 0x3ff load also move. 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. In ES and IT both asms are scheduling barriers only (differ 8 without them). */
		asm("");
		objCursor.oam.f.tile = Resource_GetBuffer(handleCursor, (s32)Resource_FixedBlockBTiles);
		objCursor.oam.f.x = (pix.x + winMain->x * 8 - ((gFrameCount & 4) >> 2)) + 16;
		objCursor.oam.f.y = (u8)((pix.y + winMain->y * 8 - ((gFrameCount & 4) >> 2)) + 16);
		objCursor.oam.f.affine_index = 8;
		Runtime_PushSlotEntry((s32 *)&objCursor, 241);

		if (redraw != 0)
		{
			object = UnitGetFar(owner);
#if EDITION_INTERNATIONAL
			UiWindow_MarkVisibleTileAttributes();
#endif
			Ui_FillVramBlockPattern();

			if ((redraw & 1) != 0)
			{
				RenderOutput_RedrawSavedRect(winMain);
				UiWindow_MapTextCanvasTiles(
					winDesc->x, winDesc->y, winDesc->width, winDesc->height, 0);
				UiWindow_DrawDividerLine((struct RenderInput *)winMain, 0, 14, 29, 14);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
				switch (System_GetLanguage())
				{
				case 2:
					UiText_DrawStringAtOffset(
						(u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
					UiText_DrawStringAtOffset(
						(u8 *)OwnerStatus_NivString, (struct RenderInput *)winMain, 51, 0);
					UiText_DrawNumberInWindow(
						object->level, 2, (struct RenderInput *)winMain, 72, 0);
					break;
				case 3:
					UiText_DrawStringAtOffset(
						(u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
					UiText_DrawStringAtOffset(
						(u8 *)OwnerStatus_NvString, (struct RenderInput *)winMain, 56, 0);
					UiText_DrawNumberInWindow(
						object->level, 2, (struct RenderInput *)winMain, 72, 0);
					break;
				default:
					UiText_DrawStringAtOffset(
						(u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
					UiText_DrawStringAtOffset(
						(u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
					UiText_DrawNumberInWindow(
						object->level, 2, (struct RenderInput *)winMain, 72, 0);
					break;
				}
#else
				UiText_DrawStringAtOffset((u8 *)object->name, (struct RenderInput *)winMain, 0, 0);
#if defined(TBS_EDITION_FR)
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 51, 0);
#else
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_LevelString, (struct RenderInput *)winMain, 56, 0);
#endif
				UiText_DrawNumberInWindow(object->level, 2, (struct RenderInput *)winMain, 72, 0);
#endif
				UiText_DrawCharacterAtOffset(
					(s32)MsgOwnerStatusExp, (struct RenderInput *)winMain, 0, 8);
				UiText_DrawNumberInWindow(
					object->experience, 8, (struct RenderInput *)winMain, 40, 8);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
				switch (System_GetLanguage())
				{
				case 1:
					UiText_DrawStringAtOffset(
						(u8 *)OwnerStatus_KpString, (struct RenderInput *)winMain, 40, 24);
					break;
				case 2:
					UiText_DrawStringAtOffset(
						(u8 *)OwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
					break;
				case 3:
					UiText_DrawStringAtOffset(
						(u8 *)OwnerStatus_PvString, (struct RenderInput *)winMain, 40, 24);
					break;
				case 4:
					UiText_DrawStringAtOffset(
						(u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
					break;
				}
#else
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_HpString, (struct RenderInput *)winMain, 40, 24);
#endif
				UiText_DrawNumberInWindow(object->hp, 4, (struct RenderInput *)winMain, 56, 24);
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 24);
				UiText_DrawNumberInWindow(object->max_hp, 4, (struct RenderInput *)winMain, 96, 24);
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_PpString, (struct RenderInput *)winMain, 40, 32);
				UiText_DrawNumberInWindow(object->ep, 4, (struct RenderInput *)winMain, 56, 32);
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_SlashString, (struct RenderInput *)winMain, 88, 32);
				UiText_DrawNumberInWindow(object->max_ep, 4, (struct RenderInput *)winMain, 96, 32);

				UiText_DrawCharacterAtOffset(
					OWNER_STATUS_LABEL(0), (struct RenderInput *)winMain, 136, 16);
				UiText_DrawNumberInWindow(
					object->atk, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 16);
				UiText_DrawCharacterAtOffset(
					OWNER_STATUS_LABEL(1), (struct RenderInput *)winMain, 136, 24);
				UiText_DrawNumberInWindow(
					object->def, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 24);
				UiText_DrawCharacterAtOffset(
					OWNER_STATUS_LABEL(2), (struct RenderInput *)winMain, 136, 32);
				UiText_DrawNumberInWindow(
					object->agi, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 32);
				UiText_DrawCharacterAtOffset(
					OWNER_STATUS_LABEL(3), (struct RenderInput *)winMain, 136, 40);
				UiText_DrawNumberInWindow(
					object->luck, 3, (struct RenderInput *)winMain, OWNER_STATUS_VALUE_X, 40);

				UiText_DrawCharacterAtOffset(
					(s32)MsgClassName + object->class_index, (struct RenderInput *)winMain, 0, 48);
				if (extended != 0)
				{
					UiText_DrawCharacterAtOffset(
						OWNER_STATUS_LABEL(9), (struct RenderInput *)winMain, 0, 72);
				}
				UiText_DrawCharacterAtOffset(
					OWNER_STATUS_LABEL(5), (struct RenderInput *)winMain, 0, 80);
				UiText_DrawCharacterAtOffset(
					OWNER_STATUS_LABEL(6), (struct RenderInput *)winMain, 0, 88);
				UiText_DrawCharacterAtOffset(
					OWNER_STATUS_LABEL(7), (struct RenderInput *)winMain, 0, 96);

				for (j = 0; j <= 3; j++)
				{
					s32 pal;

					pal = 1;
					if (extended != 0)
					{
						pal = 0;
					}
					UiWindow_SetTilemapEntry(
						(struct RenderInput *)winMain, 0x5001 + j, 7 + j * 4, pal + 8, 0);
					if (extended != 0)
					{
						UiText_DrawNumberInWindow(object->djinn_active_counts[j], 1,
							(struct RenderInput *)winMain, 40 + j * 32, 72);
						UiText_DrawStringAtOffset((u8 *)OwnerStatus_SlashString,
							(struct RenderInput *)winMain, 48 + j * 32, 72);
						UiText_DrawNumberInWindow(object->djinn_owned_counts[j], 1,
							(struct RenderInput *)winMain, 48 + j * 32 + 8, 72);
					}
					UiText_DrawNumberInWindow(Owner_GetResistanceValueFar(owner, j), 2,
						(struct RenderInput *)winMain, 48 + j * 32, 80);
					UiText_DrawNumberInWindow(
						object->elm[j].pow, 3, (struct RenderInput *)winMain, 40 + j * 32, 88);
					UiText_DrawNumberInWindow(
						object->elm[j].res, 3, (struct RenderInput *)winMain, 40 + j * 32, 96);
				}

				i = 0;
				if (object->hp == 0)
				{
					sel[0] = 16;
					i = 1;
				}
				do
				{
					if (object->restraint != 0)
					{
						sel[i++] = 15;
					}
					if (i > 7)
					{
						break;
					}
					if (object->shiryo != 0)
					{
						sel[i++] = 8;
					}
					if (i > 7)
					{
						break;
					}
					if (object->doku == 1)
					{
						sel[i++] = 1;
					}
					if (i > 7)
					{
						break;
					}
					if (object->doku == 2)
					{
						sel[i++] = 2;
					}
					if (i > 7)
					{
						break;
					}
					if (object->fuuji != 0)
					{
						sel[i++] = 4;
					}
					if (i > 7)
					{
						break;
					}
					if (object->shibire != 0)
					{
						sel[i++] = 3;
					}
					if (i > 7)
					{
						break;
					}
					if (object->nemuri != 0)
					{
						sel[i++] = 5;
					}
					if (i > 7)
					{
						break;
					}
					if (object->akuryo != 0)
					{
						sel[i++] = 7;
					}
					if (i > 7)
					{
						break;
					}
					if (object->maboroshi != 0)
					{
						sel[i++] = 6;
					}
					if (i > 7)
					{
						break;
					}
					if (object->atk_cnt != 0)
					{
						if (object->atk_lv > 0)
						{
							sel[i++] = 9;
						}
						if (object->atk_lv < 0)
						{
							sel[i++] = 10;
						}
					}
					if (i > 7)
					{
						break;
					}
					if (object->def_cnt != 0)
					{
						if (object->def_lv > 0)
						{
							sel[i++] = 11;
						}
						if (object->def_lv < 0)
						{
							sel[i++] = 12;
						}
					}
					if (i > 7)
					{
						break;
					}
					if (object->res_cnt != 0)
					{
						if (object->res_lv > 0)
						{
							sel[i++] = 13;
						}
						if (object->res_lv < 0)
						{
							sel[i++] = 14;
						}
					}
					if (i > 7)
					{
						break;
					}
					if (object->agi_lv > 0)
					{
						sel[i++] = 17;
					}
					if (object->agi_lv < 0)
					{
						sel[i++] = 18;
					}
				} while (0);

				for (j = 0; j < i; j++)
				{
					Resource_LoadTableEntryToBuffer(sel[j], tbl[j]);
				}
				if (i == 0)
				{
					sel[0] = 0;
					i = 1;
				}
				if (i <= 10)
				{
					for (j = i; j < 11; j++)
					{
						sel[j] = -1;
					}
				}
				cnt = i;
				if (sel[0] == 0)
				{
					UiText_DrawCharacterAtOffset(
						object->hp != 0 ? (s32)MsgOwnerStatusNormal : (s32)MsgOwnerStatusDowned,
						(struct RenderInput *)winMain, 112, 0);
				}
			}

			text = (u16 *)MemAlloc(OWNER_STATUS_TEXT_MAX * sizeof(*text));
			if ((u32)cur.entry > 13)
			{
				iconIdx = sel[cur.entry - 14];
				if (iconIdx == 0 && object->hp == 0)
				{
					iconIdx = 16;
				}

				saved = (struct BtlUnit *)MemAlloc(sizeof(*object));
				code = 0;
				Iwram_CopyWords(saved, object, sizeof(*object));

				atkDelta = object->atk;
				defDelta = object->def;
				agiDelta = object->agi;
				object->atk_lv = 0;
				object->def_lv = 0;
				object->agi_lv = 0;
				UnitCalcFar(owner);
				atkDelta -= object->atk;
				defDelta -= object->def;
				agiDelta -= object->agi;
				Iwram_CopyWords(object, saved, sizeof(*object));
				MemFree(saved);

				switch (iconIdx)
				{
				case 8:
					code = object->shiryo;
					break;
				case 9:
					code = atkDelta;
					break;
				case 10:
					code = -atkDelta;
					break;
				case 11:
					code = defDelta;
					break;
				case 12:
					code = -defDelta;
					break;
				case 17:
					code = agiDelta;
					break;
				case 18:
					code = -agiDelta;
					break;
				case 13:
				case 14:
					code = object->res_lv * 20;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
					if (code < 0)
					{
						code = -code;
					}
#endif
					break;
				default:
					break;
				}
				UiWork_PushValueSlot(code, 5);
				UiText_CopyMessageString(
					iconIdx + (s32)MsgOwnerStatusConditionHelp, text, OWNER_STATUS_TEXT_MAX);
			}
			else if (cur.entry == 2 && object->level <= 98)
			{
				UiWork_PushValueSlot(
					Owner_GetLevelThresholdFar(owner, object->level + 1) - object->experience, 5);
				UiText_CopyMessageString((s32)MsgOwnerStatusNextLevel, text, OWNER_STATUS_TEXT_MAX);
			}
			else
			{
				UiText_CopyMessageString(
					cur.entry + (s32)MsgOwnerStatusEntryHelp, text, OWNER_STATUS_TEXT_MAX);
			}

			UiText_RenderWideStringAtOffset(text, (struct UiWindow *)winDesc, 0, 4);
			MemFree(text);
			redraw = 0;
			base->dirty = 1;
		}

		p = objIcon;
		q = tbl;
		{
			struct VramBlockCacheEntry *vc;

			for (i = 0, vc = gVramBlockCache, x = 112; i <= 10; i++)
			{
				p->oam.word.attr01 = 0x40000400;
				p->oam.word.attr23 = 0;
				p->oam.f.tile = vc[*q++].offset >> 5;
				p->oam.f.x = x;
				p->oam.f.y = (u8)(winMain->y * 8 + 8);
				if (sel[i] > 0)
				{
					Runtime_PushSlotEntry((s32 *)p, 240);
				}
				x += 15;
				p++;
			}
		}

		if (gLinkCountdownWork->active == 0)
		{
			break;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			break;
		}

		if (listCount != 0)
		{
			if ((keys & 0x100) != 0)
			{
				idx++;
				if (idx >= listCount)
				{
					idx = 0;
				}
				owner = list[idx];
				redraw = 1;
				AudioCommand_PlayFar(0x6F);
			}
			else if ((keys & 0x200) != 0)
			{
				idx--;
				if (idx < 0)
				{
					idx = listCount - 1;
				}
				owner = list[idx];
				redraw = 1;
				AudioCommand_PlayFar(0x6F);
			}
		}
		WaitFrames(1);
	}

	{
		s32 *h = tbl;

		for (i = 10; i >= 0; i--)
		{
			Resource_ResetEntry(*h++);
		}
	}
	Resource_ResetEntry(handleFrame);
	Resource_ResetEntry(handleCursor);
	WaitFrames(1);
#if EDITION_INTERNATIONAL
	UiWindow_MarkVisibleTileAttributes();
#endif
	UiWork_Finalize(winMain, 1);
	UiWork_Finalize(winDesc, 1);
	PtyStDraw(BtlWk->pty_st);
	((struct MenuCell **)&BtlWk)[HEAP_SLOT_WINDOW + 42 - HEAP_SLOT_BATTLE]->busy = 0;
	WaitFrames(1);
	return 0;
}

#if EDITION_INTERNATIONAL
#define TEXT_COUNT 52
#define TEXT_LOAD UiText_CopyMessageString
#define TURNS_DIGITS 1
#define TURNS_X 48
#else
#define TEXT_COUNT 32
#define TEXT_LOAD UiText_DecodeMessage
#define TURNS_DIGITS 2
#define TURNS_X 40
#endif

// gLinkCountdownWork のブロック全体。0x28 から 0xe4 までは menu_list.h の MenuCell が読むところ
struct LinkRoundState
{
	struct MenuSprite entries[3];
	u8				  entryActive[3];
	u8				  unk_27;
	s32				  targetOffset;
	s32				  currentOffset;
	s32				  unk_30;
	s32				  unk_34;
	s32				  unk_38;
	s32				  unk_3c;
	s32				  unk_40;
	s32				  displayHandle;
	s32				  secondaryHandle;
	s32				  timer;
	s32				  enabled;
	u8				  cost[8][4];
	u8				  pick[34];
	u16				  list[33];
	s32				  hintCount;
	s32				  hintTimer;
	s32				  actorId;
	u32				  selected[8];
};

#define COUNTDOWN ((struct LinkRoundState *)gLinkCountdownWork)
#define CURSOR_OWNER (*(struct PartyStatusWindow **)gPartyStatusWork)

extern const u8 DjinnMenu_BlinkTile[32];
extern const u8 DjinnMenu_ElementXOffsets[4];
extern const u8 DjinnMenu_ElementYOffsets[4];

s32 Battle_SelectAbility(s32 owner, s32 unused)
{
	struct CursorSprite		  slot;
	u16						  buf[64];
	struct UiRenderWork		 *ctx;
	struct UiWindow			 *detail_win;
	s32						  prev_row;
	s32						  res;
	u32						 *tbl;
	s32						  cnt;
	s32						  prev_page;
	struct UiWindow			 *msg_win;
	s32						  mask;
	s32						  saved_row;
	s32						  sub_sel;
	s32						  redraw;
	s32						  is_used;
	s32						  sub_cnt;
	struct MenuCell			 *menu;
	struct BtlUnit			 *unit;
	struct DjinnRecoveryList *list;
	struct UiWindow			 *list_win;
	struct UiWindow			 *icon_win;
	struct PartyStatusWindow *cursor;
	u32						 *pos;
	u32						  cur;
	u32						  blink;
	s32						  page;
	s32						  row;
	u32						  code;
	s32						  pages;
	s32						  pal;
	struct MenuPoint		  cursor_pos;
	s32						  trig;
	s32						  rep;
	s32						  opts;
	s32						  x;
	s32						  y;
	s32						  i;
	s32						  j;
	struct CursorSprite		 *cur_spr;

	ctx = UI_WORK;
	detail_win = 0;
	prev_row = -1;
	res = Resource_LoadIntoFreeSlot(0x80);
	tbl = Runtime_BumpAllocateAlternatePool(0xa8 * 2);
	cnt = 0;
	prev_page = -1;
	code = 0;
	mask = 0;
	sub_sel = 0;
	sub_cnt = 0;
	redraw = 0;
	is_used = 0;
	msg_win = UiWindow_Create(0, 4, 30, 4, 42);
	UiWork_SetAltFlagAndClearTable(1);
	list_win = UiWindow_Create(21, 9, 9, 11, 6);
	MenuCell_GetCursor(page, row, saved_row);
	unit = UnitGetFar(owner);

	for (x = 0; x <= 3; x++)
	{
		for (y = 0, pos = (u32 *)((cnt << 2) + (u32)tbl); y <= 19; y++)
		{
			if (unit->djinn_active[x] & (u32)(1 << y))
			{
				*pos = (u32)((x << 8) | y);
				pos++;
				cnt++;
			}
			else if (unit->djinn_available[x] & (u32)(1 << y))
			{
				u32 v;
				s32 k;
				list =
					&((struct DjinnRecoveryTable *)Trade_GetOfferStateFar((u32)owner > 7 ? 1 : 0))
						 ->list;
				for (k = 0; k < list->count; k++)
				{
					if (list->entries[k].unit_id == owner && list->entries[k].element == x
						&& list->entries[k].index == y)
					{
						break;
					}
				}
				v = (u32)((x << 8) | y | 0x10000);
				*pos = v;
				if (list->entries[k].turns > 0)
				{
					v |= (u32)(list->entries[k].turns << 17);
					*pos = v;
				}
				cnt++;
				pos++;
			}
		}
	}
	tbl[cnt] = 0x80000000;
	ctx->dirty = 1;

	cur_spr = &slot;
	for (;;)
	{
		if (page != prev_page || row != prev_row || redraw != 0)
		{
			cur = tbl[page + row];
			mask = 0;
			ctx->menu_busy = 1;
			Ui_SetRectHighlight(
				list_win->x + 1, list_win->y + prev_row * 2 + 1, list_win->width - 2, 1, 15);
			if (sub_sel != 0)
			{
				UiWork_Finalize(msg_win, 1);
				msg_win = UiWindow_Create(0, 4, 30, 4, 42);
				Ui_FillVramBlockPattern();
			}
			redraw = 0;
			if (cnt != 0)
			{
				s32 k;
				u32 selected;
				UiWork_ClearValueNameTables();
				is_used = 0;
				/* FAKEMATCH: this one-pass loop retains the native redraw
                 * reload after the selected-entry load. A plain assignment
                 * folds k to zero (1201 -> 1200 instructions, 27 rows differ);
                 * reading selected in the comparison moves the reload early
                 * (2 rows differ). No loop instructions survive. */
				selected = COUNTDOWN->selected[redraw];
				for (k = redraw; k == redraw;)
				{
					k = redraw;
					break;
				}
				if (selected == cur)
				{
					is_used = 1;
				}
				else
				{
					for (;;)
					{
						/* FAKEMATCH: needed to match: is_used is zero on every search-loop continuation. Its step dependency prevents loop.c from strength-reducing k into a pointer; CSE later removes the flag term. Plain k differs 35 instructions in EN; this form restores the native reload and indexing (differ 0). */
						k += 1 - is_used;
						if (k > 7)
							break;
						if ((u32)COUNTDOWN->selected[k] == cur)
						{
							is_used = 1;
							break;
						}
					}
				}
				if (is_used != 0)
				{
					TEXT_LOAD((s32)MsgBattleDjinnTwice, buf, TEXT_COUNT);
					if (detail_win != 0)
					{
						UiWork_Finalize(detail_win, 1);
						detail_win = 0;
						sub_sel = 0;
					}
				}
				else if (cur & 0x10000)
				{
					if (cur & 0x3e0000)
					{
						UiWork_PushValueSlot((s32)((cur & 0x3e0000) >> 17), 5);
						TEXT_LOAD(
							(s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + (s32)MsgDjinnEffect,
							buf, TEXT_COUNT);
						if (detail_win != 0)
						{
							UiWork_Finalize(detail_win, 1);
							detail_win = 0;
							sub_sel = 0;
						}
					}
					else
					{
						detail_win = (struct UiWindow *)DjinnMenu_ShowChangePreview(
							(struct RenderInput *)detail_win, owner, (s32)cur, sub_sel, &sub_cnt);
						UiText_CopyMessageString((s32)MsgBattleSummonStandby, buf, TEXT_COUNT);
						mask = 1 << ((cur & 0xf00) >> 8);
					}
				}
				else
				{
					detail_win = (struct UiWindow *)DjinnMenu_ShowChangePreview(
						(struct RenderInput *)detail_win, owner, (s32)cur, sub_sel, &sub_cnt);
					TEXT_LOAD((s32)(((cur & 0xf00) >> 8) * 20 + (cur & 0xff)) + (s32)MsgDjinnEffect,
						buf, TEXT_COUNT);
				}
			}
			else
			{
				TEXT_LOAD((s32)MsgBattleNoDjinn, buf, TEXT_COUNT);
			}
			ctx->menu_busy = 0;
			if (sub_sel == 0)
			{
				ctx->menu_busy = 1;
				UiWork_Finalize(msg_win, 1);
				msg_win = UiWindow_Create(0, 4, 30, 4, 42);
				ctx->menu_busy = (u8)sub_sel;
			}
			UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
			prev_row = row;
			if (page != prev_page)
			{
				RenderOutput_RedrawSavedRect(list_win);
				j = 0;
				while ((code = tbl[page + j]) != 0x80000000)
				{
					UiWindow_SetTilemapEntry((struct RenderInput *)list_win,
						(s32)((code & 0xf00) >> 8) + 0x5001, 0, j * 2, 0);
					if (code & 0x3e0000)
					{
						UiWork_SetParamNibble(4);
					}
					else if (code & 0x10000)
					{
						UiWork_SetParamNibble(2);
					}
					UiText_DrawCharacterAtOffset(
						(s32)(((code & 0xf00) >> 8) * 20 + (code & 0xff)) + (s32)MsgDjinnName,
						(struct RenderInput *)list_win, 8, j * 16);
					if (code & 0x3e0000)
					{
						UiText_DrawNumberAtOffset((s32)((code & 0x3e0000) >> 17), TURNS_DIGITS,
							(struct RenderInput *)list_win, TURNS_X, j * 16);
					}
					UiWork_SetParamNibble(15);
					j++;
					if (j > 4)
					{
						break;
					}
				}
				prev_page = page;
			}
			if (cnt > 5)
			{
				for (j = 0; j < (pages = ((cnt + 4) / 5)); j++)
				{
					s32 tile = j + 0xf301;
					if (j == (page / 5))
					{
						tile = j + 0xf30b;
					}
					UiWindow_SetTilemapEntry((struct RenderInput *)list_win, tile,
						list_win->width - pages + j - 2, -1, 0);
				}
			}
			Ui_SetRectHighlight(
				list_win->x + 1, list_win->y + row * 2 + 1, list_win->width - 2, 1, 14);
			ctx->dirty = 1;
			ctx->menu_busy = 0;
		}

		if (cnt > 5)
		{
			for (j = 0; j < (cnt + 4) / 5; j++)
			{
				s32 tile = j + 0xf301;
				if ((gKeysHeld & 0x100) || (gFrameCount & 15) <= 11)
				{
					if (j == (page / 5))
					{
						tile = j + 0xf30b;
					}
				}
				UiWindow_SetTilemapEntry((struct RenderInput *)list_win, tile,
					list_win->width - ((cnt + 4) / 5) + j - 2, -1, 0);
			}
			if ((gKeysHeld & 0x100) == 0)
			{
				UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf334,
					list_win->width - (cnt + 4) / 5 - 3, -1, 0);
				UiWindow_SetTilemapEntry(
					(struct RenderInput *)list_win, 0xf335, list_win->width - 2, -1, 0);
			}
			else
			{
				UiWindow_SetTilemapEntry((struct RenderInput *)list_win, 0xf011,
					list_win->width - (cnt + 4) / 5 - 3, -1, 0);
				UiWindow_SetTilemapEntry(
					(struct RenderInput *)list_win, 0xf012, list_win->width - 2, -1, 0);
			}
			UiRender_MarkRow(ctx, list_win->y - 1);
		}

		trig = gKeyState;
		rep = gKeysRepeat;
		opts = gKeysHeld;
		menu = gLinkCountdownWork;
		if (menu->auto_enabled != 0)
		{
			rep = 0;
			trig = 0;
			opts = 0;
			if (menu->auto_delay == 0)
			{
				menu->auto_delay = 60;
				rep = 1;
				trig = 1;
			}
			else
			{
				menu->auto_delay--;
			}
		}
		menu = gLinkCountdownWork;
		if (menu->active == 0 || (trig & 2))
		{
			AudioCommand_PlayFar(0x71);
			code = -1;
			break;
		}
		if (trig & 1)
		{
			u32 sel;
			u32 turns;
			if (cnt != 0)
			{
				sel = tbl[page + row];
				turns = sel & 0x3e0000;
				if (turns == 0)
				{
					if (is_used == 0)
					{
						menu->page = page;
						menu->row = row;
						menu->preferred_row = saved_row;
						code = sel;
						break;
					}
				}
				else if (is_used == 0)
				{
					Ui_FillVramBlockPattern();
					UiWork_ClearValueNameTables();
					UiWork_PushValueSlot((s32)((turns & 0x3e0000) >> 17), 5);
					UiText_CopyMessageString((s32)MsgBattleDjinnRecovering, buf, TEXT_COUNT);
					UiText_RenderWideStringAtOffset(buf, msg_win, 0, 4);
				}
				AudioCommand_PlayFar(0x72);
			}
			else
			{
				code = -1;
				break;
			}
		}

		if (cnt != 0)
		{
			if (rep & 0x80)
			{
				AudioCommand_PlayFar(0x6f);
				row++;
				if (row == 5 || page + row == cnt)
				{
					row = 0;
				}
				saved_row = row;
			}
			else if (rep & 0x40)
			{
				AudioCommand_PlayFar(0x6f);
				row--;
				if (row < 0)
				{
					if (page == ((cnt - 1) / 5) * 5)
					{
						row = cnt - page - 1;
					}
					else
					{
						row = 4;
					}
				}
				saved_row = row;
			}
			else if (opts & 0x100)
			{
				if (detail_win != 0)
				{
					for (i = 0; i < sub_cnt; i++)
					{
						s32 tile = i + 0xf301;
						if ((gFrameCount & 15) <= 11 && i == sub_sel - 1)
						{
							tile = sub_sel - 1 + 0xf30b;
						}
						UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, tile,
							detail_win->width - sub_cnt + i - 2, -1, 0);
					}
					UiWindow_SetTilemapEntry((struct RenderInput *)detail_win, 0xf334,
						detail_win->width - sub_cnt - 3, -1, 0);
					UiWindow_SetTilemapEntry(
						(struct RenderInput *)detail_win, 0xf335, detail_win->width - 2, -1, 0);
					UiRender_MarkRow(ctx, detail_win->y);
				}
				if (sub_sel == 0 && sub_cnt != 0)
				{
					if (detail_win != 0)
					{
						RenderOutput_ClearList(detail_win);
					}
					sub_sel = 1;
					redraw = 1;
				}
				else
				{
					if (sub_sel > sub_cnt)
					{
						sub_sel = sub_cnt;
					}
					if (sub_sel != 0)
					{
						if (rep & 16)
						{
							AudioCommand_PlayFar(0x6f);
							sub_sel++;
							if (sub_sel > sub_cnt)
							{
								sub_sel = 1;
							}
							redraw = 1;
						}
						else if (rep & 32)
						{
							AudioCommand_PlayFar(0x6f);
							sub_sel--;
							if (sub_sel <= 0)
							{
								sub_sel = sub_cnt;
							}
							redraw = 1;
						}
					}
				}
			}
			else if (sub_sel != 0)
			{
				if (detail_win != 0)
				{
					RenderOutput_ClearList(detail_win);
				}
				sub_sel = 0;
				redraw = 1;
			}
			else if (rep & 16)
			{
				AudioCommand_PlayFar(0x6f);
				Runtime_SetMainState19();
				if (page + 5 >= cnt)
				{
					if (page != 0)
					{
						page = 0;
						row = saved_row;
					}
				}
				else
				{
					page += 5;
					row = saved_row;
					if (page == ((cnt - 1) / 5) * 5)
					{
						row = cnt - page - 1;
						if (row > saved_row)
						{
							row = saved_row;
						}
					}
				}
			}
			else if (rep & 32)
			{
				AudioCommand_PlayFar(0x6f);
				Runtime_SetMainState19();
				if (page != 0)
				{
					page -= 5;
					row = saved_row;
				}
				else
				{
					page = ((cnt - 1) / 5) * 5;
					row = saved_row;
					if (page != 0)
					{
						row = cnt - page - 1;
						if (row > saved_row)
						{
							row = saved_row;
						}
					}
				}
			}
		}

		{
			s32 cx;

			/* FAKEMATCH: the one-pass calculation keeps row's reload after
             * the x shift. Without the block, mov r2, r10 fills the ldrh
             * delay slot before lsls (2 differing instruction rows). */
			do
			{
				cx = list_win->x * 8;
			} while (0);
			/* FAKEMATCH: instruction order: row read in a low register here is reloaded before the subtract, and row * 2 inherits that reload, so mov r2, r10 precedes subs r3, #2 as native (2 rows without; equal priority, and the reload otherwise has the later luid) */
			asm("" : : "l"(row));
			cursor_pos.x = cx - 2;
		}
		cursor_pos.y = (row * 2 + list_win->y) * 8 + 20;
		cur_spr = &slot;
		((u32 *)cur_spr)[1] = MENU_SPRITE_SIZE_16;
		((u32 *)cur_spr)[2] = 0;
		cur_spr->tile = Resource_GetBuffer(res, (s32)Resource_FixedBlockBTiles);
		cur_spr->x = cursor_pos.x + ((gFrameCount & 4) >> 1) - 6;
		cur_spr->y = cursor_pos.y - ((gFrameCount & 4) >> 2) - 8;
		if (cnt != 0)
		{
			Runtime_PushSlotEntry(cur_spr, 242);
		}
		cursor = CURSOR_OWNER;
		blink = gFrameCount & 4;
		icon_win = cursor->window;
		if (cursor->flags & 2)
		{
			for (i = 0; i <= 3; i++)
			{
				pal = 15 - (blink != 0);
				if (((1 << i) & mask) == 0)
				{
					pal = 15;
				}
				Ui_SetRectHighlight(icon_win->x + DjinnMenu_ElementXOffsets[i] + 1,
					icon_win->y + DjinnMenu_ElementYOffsets[i] + 1, 2, 2, pal);
			}
		}
		if (gFrameCount & 4)
		{
			Iwram_CopyWords((void *)0x06006500, DjinnMenu_BlinkTile, 32);
			Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
		}
		else
		{
			Iwram_FillWords((void *)0x06006500, 32, 0x44444444);
			Iwram_CopyWords((void *)0x06006520, DjinnMenu_BlinkTile, 32);
		}
		WaitFrames(1);
	}

	if (CURSOR_OWNER->flags & 2)
	{
		Ui_SetRectHighlight(CURSOR_OWNER->window->x + 1, CURSOR_OWNER->window->y + 1, 4, 4, 15);
	}
	Resource_ResetEntry(res);
	UiWork_Finalize(msg_win, 1);
	UiWork_Finalize(detail_win, 1);
	UiWork_Finalize(list_win, 1);
	UiWindow_MarkVisibleTileAttributes();
	UiWork_SetAltFlagAndClearTable(0);
	MemFree(tbl);
	WaitFrames(1);
	return (s32)code;
}

//--------------------------------------------------------------------------
// classify_use_ability.c
// アイテム：召喚えらびのメニューと、戦闘でアイテムを使えるかの判定
//--------------------------------------------------------------------------

#define SENTOU_OK 0x80 // tgt_flg：戦闘で使える

#if !GAME_TLA

#if EDITION_INTERNATIONAL
#define SUMMON_COST_X 13 // 必要ジン数の列
#else
#define SUMMON_COST_X 10
#endif

enum
{
	SUMMON_ELEMENT_TILE = 0x5001,		// 属性アイコン
	LIST_TILE_PAGE_NUMBER = 0xf301,		// ページ番号
	LIST_TILE_PAGE_NUMBER_LIT = 0xf30b, // 表示中のページ番号
	LIST_TILE_PAGES_LEFT = 0xf334,		// 左矢印
	LIST_TILE_PAGES_RIGHT = 0xf335		// 右矢印
};

#define SUMMON_END 32 // 召喚リストの終わり

//--------------------------------------------------------------------------
// 召喚えらび
// 覚えた召喚を 1 ページ 4 つずつ並べ、えらんだ召喚の番号を返す
// （キャンセルは -1）
// standby : 属性ごとのスタンバイ中のジンの数
//--------------------------------------------------------------------------
s32 SummonMenu_SelectSummon(s32 unused0, s32 unused1, const u8 *standby)
{
	struct MenuSprite			   cursor;
	struct MenuSprite			   sprites[4];
	u8							   available[33]; // 覚えた召喚
	u8							   ordered[33];	  // 呼べるものを先にした並び
	u16							   text[64];
	s32							   handles[4];
	u8							   visible[4];
	s32							   glyph;
	s32							   count;
	struct UiWindow				  *window;
	struct UiRenderWork			  *render;
	s32							   drawn_row, drawn_page, row, page;
	s32							   cursor_handle;
	struct UiWindow				  *description, *counts;
	s32							   mask, preferred;
	struct MenuPoint			   cursor_position;
	const struct SummonDefinition *summon;
	struct MenuCell				  *nav;
	struct MenuSprite			  *sprite, *cur;
	s32							   used;
	s32							   id, tile, affordable;
	s32							   pressed, repeated, result;
	s32							   index;

	/* FAKEMATCH: register choice (spill slots) only: a dead store. GCSE
	   sizes its hash table from the function's length and PRE numbers its
	   reaching registers in bucket order; with this store the table has
	   379 buckets and &ordered and &sprites take their spill slots before
	   count - 1, as in the native frame (377 buckets without it: 80 rows
	   differ, every one a spill slot's offset). */
	result = 0;
	render = UI_WORK;
	drawn_row = -1;
	drawn_page = -1;
	cursor_handle = (s16)Resource_LoadIntoFreeSlot(128); // 16x16 スプライト 1 枚分
	description = UiWindow_Create(0, 4, 30, 4, 42);
	counts = UiWindow_Create(20, 8, 10, 3, 6);
	mask = 0;
	MenuCell_GetCursor(page, row, preferred);
#if EDITION_INTERNATIONAL
	window = UiWindow_Create(13, 11, 17, 9, 6);
#else
	window = UiWindow_Create(16, 11, 14, 9, 6);
#endif

	// 行ごとのアイコンのスプライト
	for (index = 0; index < 4; index++)
	{
		s32 y = index * 2;

		sprite = &sprites[index];
		MenuSprite_Init(sprites[index], MENU_SPRITE_SIZE_16);
		sprite->oam.f.x = window->x * 8 + 8;
		sprite->oam.f.y = (y + window->y) * 8 + 4;
	}
	for (index = 0; index < 4; index++)
	{
		s32 slot = Resource_LoadIntoFreeSlot(128);
		handles[index] = slot;
		sprites[index].oam.f.tile = Resource_GetBuffer(slot, -1);
	}

	// 今のスタンバイ数で呼べる召喚を後ろから順に先頭へ、残りをもとの順で後ろへ
	count = Trade_ListFlaggedEntriesFar(available);
	/* FAKEMATCH: instruction order only (block placement and schedule):
	   with the first scan's exit inside this one-pass block, loop.c's
	   find_and_verify_loops finds no barrier at depth 0 and leaves the
	   main loop's select block in place, as the native has it (without
	   the block it moves after the scan: differ 224, 6 branches fewer);
	   inside it, count and used are stored before count - 1 is built
	   (differ 2 with used = 0 before the block). */
	do
	{
		s32 i;

		used = 0;
		for (i = count - 1; i >= 0; i--)
		{
			const struct SummonDefinition *s;
			s32							   n, k;

			k = available[i];
			s = SummonDefinition_Get(k);
			for (n = 0; n <= 3; n++)
			{
				if (s->djinn_required[n] > standby[n])
					break;
			}
			if (n == 4)
			{
				ordered[used++] = k;
				available[i] = SUMMON_END;
			}
		}
	} while (0);
	{
		s32 i, k;

		for (i = 0; i < count; i++)
		{
			k = available[i];
			if (k != SUMMON_END)
				ordered[used++] = k;
		}
	}
	ordered[used] = SUMMON_END;

	for (;;)
	{
		cur = &cursor;

		//------------------------------------------------------------------
		// ページか行が変わったら描きなおす
		//------------------------------------------------------------------
		if (page != drawn_page || row != drawn_row)
		{
			render->menu_busy = 1;
			Ui_SetRectHighlight(
				window->x + 1, window->y + drawn_row * 2 + 1, window->width - 2, 1, 15);
			Ui_FillVramBlockPattern();

			// 説明文と、必要なジンの属性
			{
				const struct SummonDefinition *s;
				s32							   element;

				s = SummonDefinition_Get(ordered[page + row]);
#if EDITION_INTERNATIONAL
				UiText_CopyMessageString(
					s->name_message_id + (s32)&MsgAbilityDescription, text, 52);
#else
				UiText_DecodeMessage(s->name_message_id + (s32)&MsgAbilityDescription, text, 32);
#endif
				UiText_RenderWideStringAtOffset(text, (struct UiWindow *)description, 0, 4);
				drawn_row = row;
				mask = 0;
				for (element = 0; element <= 3; element++)
				{
					if (s->djinn_required[element] != 0)
						mask |= 1 << element;
				}
			}

			// スタンバイ数と、ページの召喚 4 つ
			if (page != drawn_page)
			{
				RenderOutput_RedrawSavedRect(window);
				{
					s32 element;

					for (element = 0; element <= 3; element++)
					{
						UiWindow_SetTilemapEntry(
							counts, element + SUMMON_ELEMENT_TILE, element * 2, 0, 0);
						UiWindow_PutGlyph(counts, standby[element] + '0', element * 2 + 1, 0, 0);
					}
				}
				{
					s32 element, col, y;

					index = 0;
					if ((id = ordered[page + index]) != SUMMON_END)
					{
					next:
						summon = SummonDefinition_Get(id);
						col = 0;
						for (element = 0; element <= 3; element++)
						{
							if (summon->djinn_required[element] > standby[element])
								break;
						}
						affordable = element == 4;
						Ability_LoadGlyph(
							summon->name_message_id & 0x3fff, 0, &handles[index], &glyph, 1);
						y = index * 2;
						sprites[index].oam.f.tile = glyph;
						if (!affordable)
							UiWork_SetParamNibble(2); // 呼べない召喚は色 2
						UiText_DrawCharacterAtOffset(
							SummonDefinition_Get(id)->name_message_id + (s32)&MsgAbilityName,
							(struct RenderInput *)window, 16, index * 16);
						for (element = 0; element <= 3; element++)
						{
							if (summon->djinn_required[element] != 0)
							{
								UiWindow_SetTilemapEntry(window, element + SUMMON_ELEMENT_TILE,
									col * 2 + SUMMON_COST_X, y, 0);
								UiWindow_PutGlyph(window, summon->djinn_required[element] + '0',
									col * 2 + SUMMON_COST_X + 1, y, 0);
								col++;
							}
						}
						UiWork_SetParamNibble(15);
						visible[index++] = 1;
						if (index <= 3 && (id = ordered[page + index]) != SUMMON_END)
							goto next;
					}
					while (index <= 3)
						visible[index++] = 0;
				}
				drawn_page = page;
			}

			// ページ番号
			if (count > 4)
			{
				for (index = 0; index < (count + 3) / 4; index++)
				{
					tile = index + LIST_TILE_PAGE_NUMBER;
					if (index == page / 4)
						tile = index + LIST_TILE_PAGE_NUMBER_LIT;
					UiWindow_SetTilemapEntry(
						window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
				}
			}
			Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
			render->dirty = 1;
			render->menu_busy = 0;
		}

		//------------------------------------------------------------------
		// 毎フレームの表示
		//------------------------------------------------------------------
		for (index = 0; index <= 3; index++)
		{
			if (visible[index] != 0)
				Runtime_PushSlotEntry((s32 *)&sprites[index], 240);
		}

		// カーソルは 4 フレームごとに揺れる
		cursor_position.x = window->x * 8 - 2;
		cursor_position.y = (row * 2 + window->y) * 8 + 20;
		/* FAKEMATCH: instruction order only: as u32 words, not through the
		   union, these stores do not alias cursor_position, so cur is
		   reloaded before cursor_position.y is stored (differ 2). */
		((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
		((u32 *)cur)[2] = 0;
		cur->oam.f.tile = Resource_GetBuffer((u16)cursor_handle, (s32)Resource_FixedBlockBTiles);
		cur->oam.f.x = cursor_position.x + ((gFrameCount & 4) >> 1) - 4;
		cur->oam.f.y = cursor_position.y - ((gFrameCount & 4) >> 2) + 248;
		sprite = &cursor;
		Runtime_PushSlotEntry((s32 *)sprite, 242);

		// 必要な属性のスタンバイ数を 8 フレームごとに点滅させる
		{
			s32 element, tile, blink;

			blink = gFrameCount & 8;
			for (element = 0; element <= 3; element++)
			{
				tile = 15 - (blink != 0);
				if ((1 << element & mask) == 0)
					tile = 15;
				Ui_SetRectHighlight(counts->x + element * 2 + 1, counts->y + 1, 2, 1, tile);
			}
		}

		// ページ番号（表示中のものは点滅）と左右の矢印
		if (count > 4)
		{
			for (index = 0; index < (count + 3) / 4; index++)
			{
				tile = index + LIST_TILE_PAGE_NUMBER;
				if ((gFrameCount & 15) <= 11 && index == page / 4)
					tile = index + LIST_TILE_PAGE_NUMBER_LIT;
				UiWindow_SetTilemapEntry(
					window, tile, window->width - (count + 3) / 4 + index - 2, -1, 0);
			}
			UiWindow_SetTilemapEntry(
				window, LIST_TILE_PAGES_LEFT, window->width - (count + 3) / 4 - 3, -1, 0);
			UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2, -1, 0);
			UiRender_MarkRow(render, window->y - 1);
		}

		//------------------------------------------------------------------
		// 入力
		//------------------------------------------------------------------
		nav = gLinkCountdownWork;
		/* FAKEMATCH: instruction order only: page, row and preferred are
		   stored in source order before the keys are read (differ 4). */
		do
		{
			nav->page = page;
			nav->row = row;
			nav->preferred_row = preferred;
		} while (0);
		pressed = gKeyState;
		repeated = gKeysRepeat;

		// 自動入力中は 120 フレームごとに決定する
		if (nav->auto_enabled != 0)
		{
			repeated = 0;
			pressed = 0;
			if (nav->auto_delay == 0)
			{
				nav->auto_delay = 120;
				repeated = KEY_A;
				pressed = KEY_A;
			}
			else
			{
				nav->auto_delay--;
			}
		}
		if (pressed & KEY_A)
		{
			result = ordered[page + row];
			break;
		}
		if (gLinkCountdownWork->active == 0 || (pressed & KEY_B))
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			result = -1;
			break;
		}
		if (repeated & KEY_DOWN)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			row++;
			if (row == 4 || page + row == count)
				row = 0;
			preferred = row;
		}
		else if (repeated & KEY_UP)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			row--;
			if (row < 0)
			{
				if (page == ((count - 1) / 4) * 4)
					row = count - page - 1; // 最後のページ
				else
					row = 3;
			}
			preferred = row;
		}
		else if (repeated & KEY_RIGHT)
		{
			// 次のページ（最後からは先頭へ）
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			Runtime_SetMainState19();
			if (page + 4 >= count)
			{
				if (page != 0)
				{
					page = 0;
					row = preferred;
				}
			}
			else
			{
				page += 4;
				row = preferred;
				if (page == ((count - 1) / 4) * 4)
				{
					row = count - page - 1;
					if (row > preferred)
						row = preferred;
				}
			}
		}
		else if (repeated & KEY_LEFT)
		{
			// 前のページ（先頭からは最後へ）
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			Runtime_SetMainState19();
			if (page != 0)
			{
				page -= 4;
				row = preferred;
			}
			else
			{
				page = ((count - 1) / 4) * 4;
				row = preferred;
				if (page != 0)
				{
					row = count - page - 1;
					if (row > preferred)
						row = preferred;
				}
			}
		}
		WaitFrames(1);
	}

	// 後かたづけ
	WaitFrames(1);
	for (index = 0; index < 4; index++)
		Resource_ResetEntry(handles[index]);
	Resource_ResetEntry((u16)cursor_handle);
	UiWork_Finalize(counts, 1);
	UiWork_Finalize(description, 1);
	UiWork_Finalize(window, 1);
	WaitFrames(1);
	return result;
}
#endif

//--------------------------------------------------------------------------
// 戦闘でアイテムを使えるか
// 戻り値 : 0 使える / 1 使えない / 2 戦闘では使えない
// unit   : キャラクター番号
// item   : 持ち物の値
//--------------------------------------------------------------------------
s32 Item_ClassifyUseAbility(s32 unit, s32 item)
{
	s32					   ret;
	struct ItemDefinition *def;

	if (item == 0)
	{
		return 1;
	}
	def = Item_GetDirectFar(item);
	ret = 1;
	if (def->use_type == 3) // エナジーを授ける装備
	{
		return ret;
	}
	if (def->action_id == 0)
	{
		return ret;
	}
	if ((def->type != 0) && (Item_CanOwnerEquip(unit, item) == 0))
	{
		return ret;
	}
	if ((SENTOU_OK & BtlActGet(def->action_id)->tgt_flg) == 0)
	{
		return 2;
	}
	return 0;
}

void Vram_CopyTile(u32 source_tile, u32 destination_tile)
{
	Dma_Set((u8 *)0x06000000 + (source_tile & 0x3ff) * 32,
		(u8 *)0x06000000 + (destination_tile & 0x3ff) * 32, 0x84000008, REG_DMA3);
}

s32 ItemList_SelectEntry(s32 owner, u16 *entries, s32 count)
{
	struct UiRenderWork *render;
	s32					 drawn_page;
	s32					 drawn_row;
	s32					 cursor_handle;
	struct UiWindow		*message_window;
	s32					 icon_count;
	s32					 preferred_row;
	struct MenuPoint	 pos;
	struct MenuCell		*cell;
	struct UiWindow		*window;
	s32					 page;
	s32					 row;
	s32					 entry;
	s32					 tile;
	s32					 edge;
	s32					 reason;
	s32					 result;
	s32					 i;
	struct CursorSprite	 cursor;
	struct MenuSprite	*spr;
	struct CursorSprite *cur;
	u16					 buffer[64];
	s32					 handles[BATTLE_LIST_ROWS];
	struct MenuSprite	 sprites[BATTLE_LIST_ROWS];

	render = UI_WORK;
	drawn_page = -1;
	drawn_row = -1;
	cursor_handle = Resource_LoadIntoFreeSlot(128);
	message_window = UiWindow_Create(0, 5, 30, 4, 42);
	icon_count = 0;
	MenuCell_GetCursor(page, row, preferred_row);
	window = UiWindow_Create(15, 9, 15, BATTLE_LIST_ROWS * 2 + 1, 6);

	for (i = 0; i < BATTLE_LIST_ROWS; i++)
	{
		s32 y = i * 2;

		spr = &sprites[i];
		MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
		spr->oam.f.x = window->x * 8 + 8;
		spr->oam.f.y = (y + window->y) * 8 + 4;
	}

	for (i = 0; i < BATTLE_LIST_ROWS; i++)
	{
		handles[i] = Resource_LoadIntoFreeSlot(128);
		sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
	}

	Vram_CopyTile(0xf018, 0x200);
	Vram_CopyTile(0xf018, 0x201);
	Vram_CopyTile(0xf019, 0x210);
	Vram_CopyTile(0xf019, 0x211);

	cur = &cursor;
	for (;;)
	{
		if (page != drawn_page || row != drawn_row)
		{
			render->menu_busy = 1;
			Ui_SetRectHighlight(
				window->x + 1, window->y + drawn_row * 2 + 1, window->width - 2, 1, 15);
			Ui_FillVramBlockPattern();
			if (count != 0)
			{
				if (Item_ClassifyUseAbility(owner, entries[page + row]) == 2)
				{
					TEXT_LOAD((s32)MsgBattleItemUnavailable, buffer, TEXT_COUNT);
				}
				else
				{
#if EDITION_INTERNATIONAL
					UiText_CopyMessageString(
						(entries[page + row] & ITEM_ID_MASK) + (s32)MsgItemPlainName, buffer, 52);
#else
					UiText_DecodeMessage(
						(entries[page + row] & ITEM_ID_MASK) + (s32)MsgItemPlainName, buffer, 32);
#endif
				}
			}
			else
			{
				TEXT_LOAD((s32)MsgBattleNoItems, buffer, TEXT_COUNT);
			}
			UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
			drawn_row = row;

			if (page != drawn_page)
			{
				RenderOutput_RedrawSavedRect(window);
				for (i = 0; i < BATTLE_LIST_ROWS; i++)
				{
					entry = entries[page + i];
					if (entry == 0)
					{
						break;
					}
					Item_GetDirectFar(entry);
					UiWork_SetParamNibble(15);
					if (Item_ClassifyUseAbility(owner, entry) != 0)
					{
						UiWork_SetParamNibble(4);
					}
					else if ((entry & INVENTORY_BROKEN) != 0)
					{
						UiWork_SetParamNibble(2);
					}
					UiText_DrawCharacterAtOffset((entry & ITEM_ID_MASK) + (s32)MsgItemName,
						(struct RenderInput *)window, 16, i * 16);
					UiWork_SetParamNibble(15);
					sprites[i].oam.f.tile = Resource_LoadKind26EntryToBuffer(entry, handles[i]);
				}
				icon_count = i;
				drawn_page = page;
			}

			if (count > BATTLE_LIST_ROWS)
			{
				for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
				{
					tile = i + LIST_TILE_PAGE_NUMBER;
					if (i == page / BATTLE_LIST_ROWS)
					{
						tile = i + LIST_TILE_PAGE_NUMBER_LIT;
					}
					UiWindow_SetTilemapEntry(window, tile,
						window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
				}
			}

			Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
			render->dirty = 1;
			render->menu_busy = 0;
		}

		if (count > BATTLE_LIST_ROWS)
		{
			for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
			{
				tile = i + LIST_TILE_PAGE_NUMBER;
				if ((gFrameCount & 15) <= 11)
				{
					if (i == page / BATTLE_LIST_ROWS)
					{
						tile = i + LIST_TILE_PAGE_NUMBER_LIT;
					}
				}
				UiWindow_SetTilemapEntry(
					window, tile, window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
			}
			edge = -1;
			UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
				window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
			UiWindow_SetTilemapEntry(window, LIST_TILE_PAGES_RIGHT, window->width - 2, edge, 0);
			UiRender_MarkRow(render, window->y - 1);
			/* FAKEMATCH: the cursor pointer pair preserves the native
             * spill slots (42 rows differ without the pair); it keeps
             * all 770 instructions in every edition. */
			cur++;
			cur--;
		}

		for (i = 0; i < icon_count; i++)
		{
			spr = &sprites[i];
			Runtime_PushSlotEntry(spr, 240);
		}

		pos.x = window->x * 8 - 2;
		pos.y = (row * 2 + window->y) * 8 + 20;
		/* FAKEMATCH: re-pointing cur each frame orders the spill slots of the cursor address and row*2 as the native frame does. */
		cur = &cursor;
		((u32 *)cur)[1] = MENU_SPRITE_SIZE_16;
		((u32 *)cur)[2] = 0;
		cur->tile = Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
		cur->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
		cur->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
		if (count != 0)
		{
			Runtime_PushSlotEntry(cur, 242);
		}

		cell = gLinkCountdownWork;
		cell->page = page;
		cell->row = row;
		/* FAKEMATCH: a one-pass block finishes these stores before input is read. The duplicate final store is removed by flow but preserves the native spill-slot order; with one store, 42 instructions differ only in their stack offsets. */
		cell->preferred_row = preferred_row;
		cell->preferred_row = preferred_row;
		do
		{
		} while (0);

		if ((gKeyState & KEY_A) != 0)
		{
			if (count != 0)
			{
				result = page + row;
				Item_GetDirectFar(entries[result]);
				reason = 0;
				if ((entries[result] & INVENTORY_BROKEN) == 0)
				{
					reason = Item_ClassifyUseAbility(owner, entries[result]);
					if (reason == 0)
					{
						break;
					}
				}
				AudioCommand_PlayFar(114);
				if (reason == 2)
				{
					TEXT_LOAD((s32)MsgBattleItemUnavailable, buffer, TEXT_COUNT);
				}
				else if ((entries[result] & INVENTORY_BROKEN) != 0)
				{
					TEXT_LOAD((s32)MsgBattleItemBroken, buffer, TEXT_COUNT);
				}
				else
				{
					TEXT_LOAD((s32)MsgBattleItemNoEffect, buffer, TEXT_COUNT);
				}
				Ui_FillVramBlockPattern();
				UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
			}
			else
			{
				result = -1;
				break;
			}
		}
		else if (cell->active == 0 || (gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}

		if (count != 0)
		{
			if ((gKeysRepeat & KEY_DOWN) != 0)
			{
				AudioCommand_PlayFar(111);
				row++;
				if (row == BATTLE_LIST_ROWS || page + row == count)
				{
					row = 0;
				}
				preferred_row = row;
			}
			else if ((gKeysRepeat & KEY_UP) != 0)
			{
				AudioCommand_PlayFar(111);
				row--;
				if (row < 0)
				{
					if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
					{
						row = count - page - 1;
					}
					else
					{
						row = BATTLE_LIST_ROWS - 1;
					}
				}
				preferred_row = row;
			}
			else if ((gKeysRepeat & KEY_RIGHT) != 0)
			{
				AudioCommand_PlayFar(111);
				Runtime_SetMainState19();
				if (page + BATTLE_LIST_ROWS >= count)
				{
					if (page != 0)
					{
						page = 0;
						row = preferred_row;
					}
				}
				else
				{
					page += BATTLE_LIST_ROWS;
					row = preferred_row;
					if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
					{
						row = count - page - 1;
						if (row > preferred_row)
						{
							row = preferred_row;
						}
					}
				}
			}
			else if ((gKeysRepeat & KEY_LEFT) != 0)
			{
				AudioCommand_PlayFar(111);
				Runtime_SetMainState19();
				if (page != 0)
				{
					page -= BATTLE_LIST_ROWS;
					row = preferred_row;
				}
				else
				{
					page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
					row = preferred_row;
					if (page != 0)
					{
						row = count - page - 1;
						if (row > preferred_row)
						{
							row = preferred_row;
						}
					}
				}
			}
		}
		WaitFrames(1);
	}

	UiWork_Finalize(message_window, 1);
	UiWork_Finalize(window, 1);
	WaitFrames(1);
	Resource_ResetEntry(cursor_handle);
	for (i = 0; i < BATTLE_LIST_ROWS; i++)
	{
		Resource_ResetEntry(handles[i]);
	}
	WaitFrames(1);
	return result;
}

s32 BattleMenu_RunActionSelection(s32 unit_id, u16 *actions, s32 count)
{
	struct UiRenderWork *render;
	s32					 drawn_page;
	s32					 page;
	s32					 drawn_row;
	s32					 cursor_handle;
	struct BtlUnit		*unit;
	struct UiWindow		*message_window;
	s32					 icon_count;
	s32					 preferred_row;
	struct MenuPoint	 pos;
	struct MenuCell		*nav;
	struct UiWindow		*window;
	s32					 row;
	s32					 entry;
	s32					 tile;
	s32					 edge;
	s32					 range;
	s32					 result;
	s32					 i;
	struct MenuSprite	 cursor;
	struct MenuSprite	*spr;
	u16					 buffer[64];
	s32					 handles[BATTLE_LIST_ROWS];
	struct MenuSprite	 sprites[BATTLE_LIST_ROWS];
	s32					 glyph;

	render = UI_WORK;
	drawn_row = -1;
	drawn_page = -1;
	cursor_handle = Resource_LoadIntoFreeSlot(128);
	unit = UnitGetFar(unit_id);
	message_window = UiWindow_Create(0, 5, 30, 4, 42);
	icon_count = BATTLE_LIST_ROWS;
	MenuCell_GetCursor(page, row, preferred_row);
	window = UiWindow_Create(9, 9, 21, BATTLE_LIST_ROWS * 2 + 1, 6);

	for (i = 0; i < BATTLE_LIST_ROWS; i++)
	{
		s32 y = i * 2;

		spr = &sprites[i];
		MenuSprite_Init(sprites[i], MENU_SPRITE_SIZE_16);
		spr->oam.f.x = window->x * 8 + 8;
		spr->oam.f.y = (y + window->y) * 8 + 4;
	}

	for (i = 0; i < BATTLE_LIST_ROWS; i++)
	{
		handles[i] = Resource_LoadIntoFreeSlot(128);
		sprites[i].oam.f.tile = Resource_GetBuffer(handles[i], -1);
	}

	Vram_CopyTile(0xf018, 0x200);
	Vram_CopyTile(0xf018, 0x201);
	Vram_CopyTile(0xf019, 0x210);
	Vram_CopyTile(0xf019, 0x211);

	spr = &cursor;
	for (;;)
	{
		if (page != drawn_page || row != drawn_row)
		{
			render->menu_busy = 1;
			Ui_SetRectHighlight(
				window->x + 1, window->y + drawn_row * 2 + 1, window->width - 2, 1, 15);
			Ui_FillVramBlockPattern();
			if (count != 0)
			{
#if EDITION_INTERNATIONAL
				UiText_CopyMessageString(
					(actions[page + row] & ACTION_ID_MASK) + (s32)MsgAbilityDescription, buffer,
					52);
#else
				UiText_DecodeMessage(
					(actions[page + row] & ACTION_ID_MASK) + (s32)MsgAbilityDescription, buffer,
					32);
#endif
			}
			else
			{
				TEXT_LOAD(MSGNO(MsgBattleNoPsynergy), buffer, TEXT_COUNT);
			}
			UiText_RenderWideStringAtOffset(buffer, message_window, 0, 4);
			drawn_row = row;

			if (page != drawn_page)
			{
				RenderOutput_RedrawSavedRect(window);
				for (i = 0; i < BATTLE_LIST_ROWS; i++)
				{
					struct BtlAct *action;

					entry = actions[page + i];
					if (entry == 0)
					{
						break;
					}
					action = BtlActGet(entry);
					UiWindow_SetTilemapEntry(window, 0xf01f, 11, i * 2, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
					UiWindow_SetTilemapEntry(window, 0xf005, 12, i * 2, 0);
#else
					UiWindow_SetTilemapEntry(window, 0xf01e, 12, i * 2, 0);
#endif
					Ability_LoadGlyph(entry & ACTION_ID_MASK, 0, &handles[i], &glyph, 1);
					sprites[i].oam.f.tile = glyph;

					if ((action->tgt_flg & ACTION_USABLE_IN_BATTLE) == 0)
					{
						UiWork_SetParamNibble(4);
					}
					else if (action->pp_cost > unit->ep)
					{
						UiWork_SetParamNibble(2);
					}
					else if (unit->fuuji != 0)
					{
						UiWork_SetParamNibble(9);
					}

					render->level = 5;
					UiText_DrawCharacterAtOffset(
						entry + (s32)MsgAbilityName, (struct RenderInput *)window, 16, i * 16);
					UiText_DrawNumberAtOffset(
						action->pp_cost, 2, (struct RenderInput *)window, 104, i * 16);
					UiWork_SetParamNibble(15);
					render->level = 15;

					if (action->damage_class != 4)
					{
						UiWindow_SetTilemapEntry(
							window, action->damage_class + 0x5001, 15, i * 2, 0);
					}

					range = action->rng;
					if (range == 255)
					{
						range = 11;
					}
					else
					{
						range--;
					}
					UiWindow_DrawThreeTileColumn((s32)window, 16, i * 2, range, 0);
				}
				icon_count = i;
				drawn_page = page;
			}

			if (count > BATTLE_LIST_ROWS)
			{
				for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
				{
					tile = i + 0xf301;
					if (i == page / BATTLE_LIST_ROWS)
					{
						tile = i + 0xf30b;
					}
					UiWindow_SetTilemapEntry(window, tile,
						window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
				}
			}

			Ui_SetRectHighlight(window->x + 1, window->y + row * 2 + 1, window->width - 2, 1, 14);
			render->dirty = 1;
			render->menu_busy = 0;
		}

		if (count > BATTLE_LIST_ROWS)
		{
			for (i = 0; i < (count + 4) / BATTLE_LIST_ROWS; i++)
			{
				tile = i + 0xf301;
				if ((gFrameCount & 15) <= 11)
				{
					if (i == page / BATTLE_LIST_ROWS)
					{
						tile = i + 0xf30b;
					}
				}
				UiWindow_SetTilemapEntry(
					window, tile, window->width - (count + 4) / BATTLE_LIST_ROWS + i - 2, -1, 0);
			}
			edge = -1;
			UiWindow_SetTilemapEntry(
				window, 0xf334, window->width - (count + 4) / BATTLE_LIST_ROWS - 3, edge, 0);
			UiWindow_SetTilemapEntry(window, 0xf335, window->width - 2, edge, 0);
			UiRender_MarkRow(render, window->y - 1);
		}

		for (i = 0; i < icon_count; i++)
		{
			Runtime_PushSlotEntry(&sprites[i], 240);
		}

		pos.x = window->x * 8 - 4;
		pos.y = (row * 2 + window->y) * 8 + 20;
		((u32 *)spr)[1] = 0x40000000;
		((u32 *)spr)[2] = 0;
		/* FAKEMATCH: this dead duplicate is needed to match the native spill
         * slots; removing it changes 42 aligned instructions. */
		((u32 *)spr)[2] = 0;
		spr->oam.f.tile = Resource_GetBuffer(cursor_handle, (s32)Resource_FixedBlockBTiles);
		spr->oam.f.x = pos.x + ((gFrameCount & 4) >> 1) - 4;
		spr->oam.f.y = pos.y - ((gFrameCount & 4) >> 2) - 8;
		if (count != 0)
		{
			Runtime_PushSlotEntry(spr, 242);
		}

		nav = gLinkCountdownWork;
		nav->page = page;
		nav->row = row;
		nav->preferred_row = preferred_row;

		if ((gKeyState & KEY_A) != 0)
		{
			if (count != 0)
			{
				result = page + row;
				if ((BtlActGet(actions[result])->tgt_flg & ACTION_USABLE_IN_BATTLE) != 0)
				{
					break;
				}
				/* FAKEMATCH: a dead store here keeps the test above from being threaded past the break. */
				result = -1;
			}
			else
			{
				result = -1;
				break;
			}
		}
		else if (nav->active == 0 || (gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}

		if (count != 0)
		{
			if ((gKeysRepeat & KEY_DOWN) != 0)
			{
				AudioCommand_PlayFar(111);
				row++;
				if (row == BATTLE_LIST_ROWS || page + row == count)
				{
					row = 0;
				}
				preferred_row = row;
			}
			else if ((gKeysRepeat & KEY_UP) != 0)
			{
				AudioCommand_PlayFar(111);
				row--;
				if (row < 0)
				{
					if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
					{
						row = count - page - 1;
					}
					else
					{
						row = BATTLE_LIST_ROWS - 1;
					}
				}
				preferred_row = row;
			}
			else if ((gKeysRepeat & KEY_RIGHT) != 0)
			{
				AudioCommand_PlayFar(111);
				Runtime_SetMainState19();
				if (page + BATTLE_LIST_ROWS >= count)
				{
					if (page != 0)
					{
						page = 0;
						row = preferred_row;
					}
				}
				else
				{
					page += BATTLE_LIST_ROWS;
					row = preferred_row;
					if (page == (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS)
					{
						row = count - page - 1;
						if (row > preferred_row)
						{
							row = preferred_row;
						}
					}
				}
			}
			else if ((gKeysRepeat & KEY_LEFT) != 0)
			{
				AudioCommand_PlayFar(111);
				Runtime_SetMainState19();
				if (page != 0)
				{
					row = preferred_row;
					page -= BATTLE_LIST_ROWS;
				}
				else
				{
					page = (count - 1) / BATTLE_LIST_ROWS * BATTLE_LIST_ROWS;
					row = preferred_row;
					if (page != 0)
					{
						row = count - page - 1;
						if (row > preferred_row)
						{
							row = preferred_row;
						}
					}
				}
			}
		}
		WaitFrames(1);
	}

	UiWork_Finalize(message_window, 1);
	UiWork_Finalize(window, 1);
	WaitFrames(1);
	for (i = 0; i < BATTLE_LIST_ROWS; i++)
	{
		Resource_ResetEntry(handles[i]);
	}
	Resource_ResetEntry(cursor_handle);
	WaitFrames(1);
	return result;
}

struct CursorSlot
{
	u8 x;
	u8 y;
	u8 flags;
	s8 index;
};

s32 BattleTarget_RunSelection(s32 sel, s32 mode, u32 spread, u32 kind)
{
	u16					   ids[8];
	struct SelectionSprite entries[6];
	struct CursorSlot	   tbl[6];
	struct EffectPosition  markPos;
	u16					   selIds[14];
	u8					   selSlot[8];
	struct EffectPosition  pos;
	u16					   name[15];
	struct EffectPosition  namePos;
	struct EffectPosition  targetPos;
	struct AffineTransform efx;
	struct BtlUnit		  *unit;
	struct BtlWork		  *runtime;
	struct UiWindow		  *window;
	/* FAKEMATCH: the earlier combined pin/barrier removal gave EN 1585/1585, common 1343, differ 484: infoWin spills and the frame shrinks 324 -> 320. ES 1684/1684, common 1569, differ 230: infoWin/sel swap r10/r9 and spill slots permute. Both extents stayed unchanged. */
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	struct UiWindow *infoWin;
#else
	/* FAKEMATCH: register choice only: as an ordinary local the info window is spilled and the frame shrinks to 320 bytes (differ 492); the native keeps it in r9. 2026-10-11: its live length is doubled by update_equiv_regs (the first set carries a constant REG_EQUAL), giving priority 2104 against &pos 2171 and the hoisted bit-field zero 2142; one duplicated no-effect block reaches 2143 against 2149 (differ 202), two push &pos below the zero (differ 449), and an initializer without the note sends it to r10 as in ES/IT (differ 96). */
	register struct UiWindow *infoWin asm("r9");
#endif
	s32						cursor;
	s32						cnt;
	s32						total;
	s32						redraw;
	s32						slotId;
	s32						pending;
	s32						matrix;
	s32						found;
	s32						column;
	s32						rows;
	s32						count;
	s32						i;
	s32						j;
	s32						n;
	s32						nx;
	s32						ny;
	s32						pressed;
	s32						y;
	s32						width;
	s32						repeat;
	s32						result;
	struct CursorSlot	   *slot;
	struct SelectionSprite *entry;
	struct SelectionSprite *head;
	struct BtlUnit		   *who;

	runtime = BtlWk;
	cnt = 0;
	redraw = 0xFFFF;
	slotId = Resource_LoadIntoFreeSlot(256);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	/* FAKEMATCH: this value-preserving null initializer is needed to match
     * infoWin in r10; a direct zero swaps high registers and spill slots. */
	infoWin = (struct UiWindow *)((u32)(u16)sel >> 16);
#else
	infoWin = 0;
#endif
	if (spread == 0)
		spread = 1;
	if (mode == 2 || mode == 4)
		((struct MenuCell **)&BtlWk)[48]->slide_offset = -2;
	else
		((struct MenuCell **)&BtlWk)[48]->slide_offset = 16;
	for (i = 0; i < 6; i++)
		tbl[i].flags = 0;
	cursor = -1;
	if (mode == 2)
	{
		for (i = 0; i <= 5 && runtime->party_units[i] != 0xFF; i++)
		{
			ids[cnt] = runtime->party_units[i];
			cnt++;
		}
	}
	else if (mode == 4)
	{
		ids[0] = (u16)sel;
		cnt = 1;
	}
	else
	{
		for (i = 0; i <= 5 && runtime->enm_no[i] != 0xFF; i++)
		{
			ids[cnt] = runtime->enm_no[i];
			cnt++;
		}
	}
	ids[cnt] = 0xFF;
	total = cnt;
	if (mode == 2)
	{
		if (spread != 0xFF && kind != 0)
		{
			found = 0;
			for (j = 0; j < cnt; j++)
			{
				struct BtlUnit *st;

				if (ids[j] == 0xFE)
					continue;
				st = UnitGetFar(ids[j]);
				switch (kind)
				{
				case 5:
					if (st->hp == 0)
					{
						sel = ids[j];
						found = 1;
					}
					break;
				case 3:
					if (st->doku != 0)
						found = 1;
					break;
				case 4:
					if (st->maboroshi != 0 || st->shibire != 0 || st->nemuri != 0 || st->fuuji != 0
						|| st->shiryo != 0)
						found = 1;
					break;
				case 6:
					if (st->maboroshi != 0 || st->shibire != 0 || st->nemuri != 0 || st->fuuji != 0
						|| st->shiryo != 0 || st->doku != 0 || st->akuryo != 0)
						found = 1;
					break;
				}
				if (found != 0)
				{
					sel = ids[j];
					break;
				}
			}
		}
		for (j = 0; j < cnt; j++)
		{
			if (ids[j] == sel)
				break;
		}
		if (j != cnt)
			cursor = j;
	}
	if (cursor < 0)
		cursor = (cnt - 1) / 2;
	while (ids[cursor] == 0xFE
		   || (GameFlag_TestFar(0x16C) && mode == 1 && UnitGetFar(ids[cursor])->hp == 0))
	{
		cursor = cursor + cnt - 1;
		cursor %= cnt;
	}
	if (mode != 2)
	{
		BattleMotion_ProjectConditionalPositionFar(sel, (s32 *)&markPos);
		tbl[0].flags = 8;
		tbl[0].x = (u8)markPos.x;
		tbl[0].y = 0x80;
	}
	window = UiWindow_Create(0, 12, 30, 4, 74);
	while (1)
	{
		head = entries;
		pending = 0;
		{
			s32 t = cursor * 2;

			/* FAKEMATCH: the separate switch arms end CSE before the
             * projection call, keeping the scaled offset reload ahead of
             * the ids load. Sharing their block reverses two rows; no
             * switch branch remains in any edition. */
			switch (t)
			{
			case 0:
				t = cursor * 2;
				break;
			default:
				t = cursor * 2;
				break;
			}
			BattleMotion_ProjectConditionalPositionFar(*(u16 *)((u8 *)ids + t), (s32 *)&pos);
		}
		((s32 *)head)[1] = 0x40002000;
		((s32 *)head)[2] = 0;
		head->tile = Resource_GetBuffer(
			slotId, (((gFrameCount >> 2) & 31) << 8) + (s32)Menu_AnimatedCursorTiles);
		pos.y += Trig_Sin(gFrameCount << 12) / 32768;
		y = pos.y;
		if (tbl[0].flags & 1)
		{
			nx = (pos.x + tbl[0].x) / 2;
			ny = (y + tbl[0].y) / 2;
			if ((pos.x - nx < 0 ? -(pos.x - nx) : pos.x - nx) <= 7)
				pending = 1;
			pos.x = nx;
			tbl[0].x = (u8)nx;
			tbl[0].y = (u8)ny;
			pos.y = ny;
		}
		else if ((u8)tbl[0].flags <= 3)
		{
			tbl[0].x = (u8)pos.x;
			pos.y = y;
			tbl[0].y = (u8)y;
			tbl[0].flags = 1;
		}
		else
		{
			pos.x = tbl[0].x;
			pos.y = tbl[0].y;
			tbl[0].flags = (u8)(tbl[0].flags - 4);
			if (tbl[0].flags <= 3)
				tbl[0].flags = 1;
		}
		head->x = pos.x - 8;
		head->y = (u8)(pos.y - 16);
		Runtime_PushSlotEntry((s32 *)head, 240);
		if (spread == 0xFF)
		{
			efx.scale_x = 256;
			efx.scale_y = 256;
		}
		else
		{
			efx.scale_x = 176;
			efx.scale_y = 176;
		}
		efx.angle = 0;
#if !defined(TBS_EDITION_ES) && !defined(TBS_EDITION_IT)
		/* FAKEMATCH: register choice (spill slots) only: seventeen dead
           stores, two insns each until flow deletes them. GCSE sizes its
           expression table from the insn count (n / 2 | 1 buckets) and
           PRE numbers its reaching registers in bucket order, which is the
           order their spill slots are assigned; with these the count is
           1451 and the table 725 buckets, so tbl, entries, ids and
           cursor * 2 take sp+36, 32, 28 and 24 before &efx (20) and
           selSlot (16) as in the native frame (1417 and 709 buckets
           without them: every remaining row is a spill slot's offset).
           2026-10-11: ES and IT match without any of these stores (and
           without the two more they had), so only the other four editions
           compile them. */
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
		efx.angle = 0;
#endif
		matrix = AffineMatrix_BuildForEffect(&efx);
		if ((redraw & 1) == 0)
			goto frame_end;
		cnt = 0;
		for (i = 0; i < 6; i++)
			tbl[i].flags &= (u8)~2;
		for (i = 0; (u32)i < spread; i++)
		{
			{
				n = cursor + i;
				if (n < total && ids[n] != 0xFE)
				{
					selIds[cnt] = ids[n];
					tbl[i].flags |= 2;
					if (tbl[i].index != i)
					{
						tbl[i].flags &= (u8)~1;
						tbl[i].index = (s8)i;
					}
					selSlot[cnt] = (u8)i;
					cnt++;
				}
				if (i != 0 && (n = cursor - i) >= 0 && ids[n] != 0xFE)
				{
					selIds[cnt] = ids[n];
					tbl[6 - i].flags |= 2;
					if (tbl[6 - i].index != -i)
					{
						tbl[6 - i].flags &= (u8)~1;
						tbl[6 - i].index = (s8)-i;
					}
					selSlot[cnt] = (u8)(6 - i);
					cnt++;
				}
			}
		}
		for (i = 0; i < 6; i++)
		{
			if ((tbl[i].flags & 2) == 0)
				tbl[i].index = 6;
		}
		selIds[cnt] = 0xFF;
		BattlePres_SetActorModesFar(selIds, 1);
		if (ids[cursor] > 7)
			goto draw_name;
		if (spread == 0xFF)
			goto frame_tail;
		if (kind == 0)
			goto frame_tail;
		unit = UnitGetFar(ids[cursor]);
		BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&pos);
		if (infoWin != 0)
			UiWork_Finalize(infoWin, 1);
		switch (kind)
		{
		case 7:
		{
			s32 anchor;

			anchor = pos.x / 8;
			column = anchor - 4;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			if (System_GetLanguage() == 4)
			{
				if (anchor + 6 > 29)
					column = 20;
				infoWin = UiWindow_Create(column, 8, 10, 3, 6);
			}
			else if (System_GetLanguage() == 3)
			{
				if (anchor + 7 > 29)
					column = 19;
				infoWin = UiWindow_Create(column, 8, 11, 3, 6);
			}
			else
			{
				if (anchor + 5 > 29)
					column = 21;
				infoWin = UiWindow_Create(column, 8, 9, 3, 6);
			}
#elif defined(TBS_EDITION_FR)
			if (anchor + 5 > 29)
				column = 21;
			infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#elif defined(TBS_EDITION_JA)
			if (anchor + 4 > 29)
				column = 22;
			infoWin = UiWindow_Create(column, 8, 8, 3, 6);
#else
			if (anchor + 4 > 29)
				column = 22;
			infoWin = UiWindow_Create(column, 8, 9, 3, 6);
#endif
		}
			UiWork_SetParamNibble(2);
			UiText_DrawCharacterAtOffset(
				(s32)MsgTargetSetDjinni, (struct RenderInput *)infoWin, 0, 0);
			UiWork_SetParamNibble(15);
			goto frame_tail;
		case 1:
		{
			s32 anchor;

			anchor = pos.x / 8;
			column = anchor - 7;
			if (anchor + 6 > 29)
				column = 17;
			infoWin = UiWindow_Create(column, 8, 13, 3, 6);
		}
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			switch (System_GetLanguage())
			{
			case 1:
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_KpString, (struct RenderInput *)infoWin, 0, 0);
				break;
			case 2:
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
				break;
			case 3:
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_PvString, (struct RenderInput *)infoWin, 0, 0);
				break;
			case 4:
				UiText_DrawStringAtOffset(
					(u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
				break;
			}
#else
			UiText_DrawStringAtOffset(
				(u8 *)OwnerStatus_HpString, (struct RenderInput *)infoWin, 0, 0);
#endif
			UiText_DrawNumberInWindow(unit->hp, 4, (s32)infoWin, 16, 0);
			UiText_DrawStringAtOffset(
				(u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
			UiText_DrawNumberInWindow(unit->max_hp, 4, (s32)infoWin, 56, 0);
			goto frame_tail;
		case 2:
		{
			s32 anchor;

			anchor = pos.x / 8;
			column = anchor - 7;
			if (anchor + 6 > 29)
				column = 17;
			infoWin = UiWindow_Create(column, 8, 13, 3, 6);
		}
			UiText_DrawStringAtOffset(
				(u8 *)OwnerStatus_PpString, (struct RenderInput *)infoWin, 0, 0);
			UiText_DrawNumberInWindow(unit->ep, 4, (s32)infoWin, 16, 0);
			UiText_DrawStringAtOffset(
				(u8 *)OwnerStatus_SlashString, (struct RenderInput *)infoWin, 48, 0);
			UiText_DrawNumberInWindow(unit->max_ep, 4, (s32)infoWin, 56, 0);
			goto frame_tail;
		case 5:
		{
			s32 anchor;

			anchor = pos.x / 8;
			column = anchor - 7;
			if (anchor + 5 > 29)
				column = 18;
			infoWin = UiWindow_Create(column, 8, 12, 3, 6);
		}
			if (unit->hp != 0)
				goto no_condition;
			UiText_DrawCharacterAtOffset((s32)MsgTargetDowned, (struct RenderInput *)infoWin, 0, 0);
			goto frame_tail;
		case 3:
		{
			s32 anchor;

			anchor = pos.x / 8;
			column = anchor - 7;
			if (anchor + 5 > 29)
				column = 18;
			infoWin = UiWindow_Create(column, 8, 12, 3, 6);
		}
			if (unit->doku == 0)
				goto no_condition;
			UiText_DrawCharacterAtOffset(
				(s32)MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
			goto frame_tail;
		case 4:
			count = 0;
			if (unit->maboroshi != 0)
				count = 1;
			if (unit->shibire != 0)
				count++;
			if (unit->nemuri != 0)
				count++;
			if (unit->fuuji != 0)
				count++;
			if (unit->shiryo != 0)
				count++;
			if (count == 0)
				count = 1;
			rows = 9 - count;
			if (rows <= 3)
				rows = 4;
			{
				s32 anchor;

				anchor = pos.x / 8;
				column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
				if (System_GetLanguage() == 3)
				{
					if (anchor + 10 > 29)
						column = 13;
					infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
				}
				else
				{
					if (anchor + 9 > 29)
						column = 14;
					infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
				}
#elif defined(TBS_EDITION_JA)
				if (anchor + 7 > 29)
					column = 16;
				infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
				if (anchor + 9 > 29)
					column = 14;
				infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
			}
			count = 0;
			if (unit->maboroshi != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, 0);
				count = 1;
			}
			if (unit->shibire != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (unit->nemuri != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (unit->fuuji != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
				++count;
			}
			if (unit->shiryo != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (count == 0)
				goto no_condition;
			goto frame_tail;
		case 6:
			count = 0;
			if (unit->doku != 0)
				count = 1;
			if (unit->maboroshi != 0)
				count++;
			if (unit->shibire != 0)
				count++;
			if (unit->nemuri != 0)
				count++;
			if (unit->fuuji != 0)
				count++;
			if (unit->shiryo != 0)
				count++;
			if (unit->akuryo != 0)
				count++;
			if (count == 0)
				count = 1;
			rows = 9 - count;
			if (rows <= 3)
				rows = 4;
			{
				s32 anchor;

				anchor = pos.x / 8;
				column = anchor - 7;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
				if (System_GetLanguage() == 3)
				{
					if (anchor + 10 > 29)
						column = 13;
					infoWin = UiWindow_Create(column, rows, 17, count + 2, 6);
				}
				else
				{
					if (anchor + 9 > 29)
						column = 14;
					infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
				}
#elif defined(TBS_EDITION_JA)
				if (anchor + 7 > 29)
					column = 16;
				infoWin = UiWindow_Create(column, rows, 14, count + 2, 6);
#else
				if (anchor + 9 > 29)
					column = 14;
				infoWin = UiWindow_Create(column, rows, 16, count + 2, 6);
#endif
			}
			count = 0;
			if (unit->doku != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCurePoison, (struct RenderInput *)infoWin, 0, 0);
				count = 1;
			}
			if (unit->maboroshi != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureDelusion, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (unit->shibire != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureStun, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (unit->nemuri != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureSleep, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (unit->fuuji != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureSeal, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (unit->shiryo != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureCurse, (struct RenderInput *)infoWin, 0, count * 8);
				count++;
			}
			if (unit->akuryo != 0)
			{
				UiText_DrawCharacterAtOffset(
					(s32)MsgTargetCureHaunt, (struct RenderInput *)infoWin, 0, 8 * count);
				count++;
			}
			if (count == 0)
				goto no_condition;
			goto frame_tail;
		default:
			break;
		}
		goto frame_tail;
	no_condition:
		UiWork_SetParamNibble(2);
		UiText_DrawCharacterAtOffset((s32)MsgTargetNoEffect, (struct RenderInput *)infoWin, 0, 0);
		UiWork_SetParamNibble(15);
		goto frame_tail;
	draw_name:
		if (spread == 0xFF)
			goto frame_tail;
		who = UnitGetFar(ids[cursor]);
		BattleMotion_ProjectConditionalPositionFar(ids[cursor], (s32 *)&namePos);
		namePos.y += Trig_Sin(gFrameCount << 12) / 32768;
		if (who->class_id == 125 || who->class_id == 122)
		{
			width = (s32)MsgMysteryPersonName;
			if (who->class_id == 125)
				width++;
#if defined(TBS_EDITION_JA)
			UiText_DecodeMessage(width, name, 10);
#else
			UiText_CopyMessageString(width, name, 14);
#endif
		}
		else
		{
			for (i = 0; i <= 13; i++)
			{
				name[i] = n = who->name[i];
				if (!n)
					break;
			}
			name[i] = 0;
		}
		width = UiText_GetWideStringWidth(name);
		namePos.x -= width / 2;
		namePos.x -= 8;
		if (namePos.x + width > 224)
			namePos.x = 224 - width;
		if (namePos.x < 0)
			namePos.x = 0;
		Ui_ClearVramBlock();
		UiText_RenderWideStringAtOffset(name, window, namePos.x, 4);
	frame_tail:
		redraw &= ~1;
	frame_end:
		if (pending != 0)
		{
			for (i = 1; i < cnt; i++)
			{
				entry = &head[i];
				slot = &tbl[selSlot[i]];
				BattleMotion_ProjectConditionalPositionFar(selIds[i], (s32 *)&targetPos);
				targetPos.y = targetPos.y + Trig_Sin(gFrameCount << 12) / 32768;
				*entry = *head;
				if (slot->flags & 1)
				{
					s32 x = (targetPos.x + slot->x) / 2;
					s32 y = (targetPos.y + slot->y) / 2;

					targetPos.x = x;
					slot->x = x;
					targetPos.y = y;
					slot->y = y;
				}
				else
				{
					s32 x = entry->x;
					s32 y = entry->y + 8;

					slot->flags = 1;
					targetPos.x = x;
					slot->x = x;
					targetPos.y = y;
					slot->y = y;
				}
				entry->mode = 1;
				entry->x = targetPos.x - 8;
				entry->y = targetPos.y - 12;
				if (spread == 0xFF)
					entry->affine = 0;
				else
					entry->affine = 1;
				entry->param = matrix;
				Runtime_PushSlotEntry((s32 *)entry, 240);
			}
		}
		pressed = gKeyState;
		repeat = gKeysRepeat;
		if (gLinkCountdownWork->auto_enabled)
		{
			repeat = 0;
			pressed = 0;
			if (gLinkCountdownWork->auto_delay == 0)
			{
				gLinkCountdownWork->auto_delay = 60;
				repeat = 1;
				pressed = 1;
			}
			else
			{
				gLinkCountdownWork->auto_delay--;
			}
		}
		if (pressed & 1)
		{
			s32 id;
			s32 k;

			id = ids[cursor];
			redraw = 0;
			result = -1;
			for (k = 0; k <= 5 && runtime->party_units[k] != 0xFF; k++)
			{
				if (runtime->party_units[k] == id)
				{
					result = 0x100 | k;
					break;
				}
			}
			if (result < 0)
			{
				for (k = 0; k <= 5 && runtime->enm_no[k] != 0xFF; k++)
				{
					if (runtime->enm_no[k] == id)
					{
						result = 0x180 | k;
						break;
					}
				}
			}
			cursor = result;
		}
		else if (spread != 0xFF)
		{
			if (repeat & 0x90)
			{
				AudioCommand_PlayFar(111);
				do
				{
					cursor++;
					cursor %= total;
				} while (ids[cursor] == 0xFE);
				redraw |= 1;
			}
			if (repeat & 0x60)
			{
				AudioCommand_PlayFar(111);
				do
				{
					cursor = cursor + total - 1;
					cursor %= total;
				} while (ids[cursor] == 0xFE);
				redraw |= 1;
			}
		}
		if (gLinkCountdownWork->active == 0 || (pressed & 2))
		{
			AudioCommand_PlayFar(113);
			cursor = -1;
			break;
		}
		WaitFrames(1);
		if (redraw == 0)
			break;
	}
	WaitFrames(1);
	Resource_ResetEntry(slotId);
	if (infoWin != 0)
		UiWork_Finalize(infoWin, 1);
	UiWork_Finalize(window, 1);
	BattlePres_SetActorModesFar(ids, 0);
	gLinkCountdownWork->slide_offset = 0;
	WaitFrames(1);
	return cursor;
}

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct LinkSignature
{
	u8	padding_000[8];
	u16 e;
	u16 d;
	u16 v;
	u16 s;
	u8	padding_010[8];
};

void UpdateLinkSessionCountdown(void)
{
	struct LinkRoundState	   *state = COUNTDOWN;
	struct BtlWork			   *runtime;
	struct MenuSprite		   *entry;
	u8						   *active;
	struct LinkSignature	   *signature;
	s32							target;
	s32							current;
	s32							difference;
	s32							step;
	s32							newOffset;
	s32							timer;
	s32							nextTimer;
	s32							seconds;
	s32							i;

	if (state != 0)
	{
		target = state->targetOffset;
		current = state->currentOffset;
		if (target != current)
		{
			difference = target - current;
			step = difference / 3;
			if (step == 0)
			{
				step--;
				if (difference >= 0)
					step = 1;
			}
			newOffset = current + step;
			state->currentOffset = newOffset;
			Camera_ConfigureSceneFar(newOffset);
		}

		entry = state->entries;
		active = state->entryActive;
		i = 2;
		do
		{
			if (*active++ != 0)
				Runtime_PushSlotEntry(entry, 240);
			i--;
			entry++;
		} while (i >= 0);
		Link_DrawShiftedTilePair(0x06006680);

		if (state->enabled == 0)
			goto done;

		runtime = BtlWk;
		if (runtime->link_paused == 0)
			goto load_timer;
		state->timer = 0;
		goto done;

	signature_tail:
		if (signature->v != 'V')
			goto signature_done;
		if (signature->s != 'S')
			goto signature_done;
		state->timer = COUNTDOWN_START_FRAMES;
		timer = state->timer;
		goto signature_done;

	load_timer:
		timer = state->timer;
		if (timer >= 0)
			goto timer_ready;
		signature = (struct LinkSignature *)gLinkPeerSignatures[runtime->link_side ^ 1];
		if (signature->e != 'E')
			goto signature_done;
		if (signature->d == 'D')
			goto signature_tail;

	signature_done:
		if (timer < 0)
			goto done;

	timer_ready:
		if (state->displayHandle == 0 && state->secondaryHandle == 0)
		{
			state->displayHandle = Link_CreateCountdownLabelWindow();
			timer = state->timer;
		}

		if (timer > 0)
		{
			nextTimer = timer - 1;
			state->timer = nextTimer;
			timer = nextTimer;
		}
		if (timer < 0)
			goto done;

		seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
		if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
			AudioCommand_PlayFar(SOUND_TRIPLE_TONE_LOW);

		if (state->displayHandle != 0)
			UiText_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
	}

done:
	return;
}

s32 UiText_ShowLocalizedMessageAndWait(void)
{
	s16				buffer[64];
	s32				work;
	s32				result;
	struct BtlWork *battle;
	s32				mode;

	battle = BtlWk;
	mode = gGameState.battle_start;
	if (gGameState.battle_start == 2 || mode <= 2 || (result = 1, mode > 4))
	{
		result = 0;
	}
	if (result == 0)
	{
		if ((s8)battle->chouhatsu_flg != 0)
		{
			result = 1;
		}
		if (result != 0)
		{
			goto active;
		}
	}
	else
	{
	active:
		work = UiWindow_Create(0, 7, 30, 4, 42);
		Ui_FillVramBlockPattern();
		TEXT_LOAD((s32)MsgNoTimeToRun, buffer, TEXT_COUNT);
		UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
		do
		{
			WaitFrames(1);
		} while ((gKeyState & (KEY_A | KEY_B)) == 0 && COUNTDOWN->timer != 0);
		UiWork_Finalize(work, 1);
	}
	return result;
}

//--------------------------------------------------------------------------
// classify_entry_kind.c
// バトル：わざの対象選びの種類
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// わざの効果から、対象選びで使う種類を返す
// 0 その他、1 HPかいふく、2 EPかいふく、3 毒がきえる、4 状態異常をなおす、
// 5 ふっき、6 状態異常をすべてなおす
// act : わざのデータ
//--------------------------------------------------------------------------
s32 Battle_ClassifyEntryKind(const struct BtlAct *act)
{
	s32 val;
	s32 type;
	s32 kind;
	u8	efx;

	kind = 0;
	type = act->tgt_flg & 15;
	if (type == 1) // HPかいふく
		kind = 1;
	if (type == 11) // EPかいふく
		kind = 2;
	val = (s8)act->efx;
	efx = val;
	if (efx == 3) // 毒がきえる
		kind = 3;
	if (efx == 4) // 状態異常をなおす
		kind = 4;
	if (efx == 64) // 状態異常をすべてなおす
		kind = 6;
	if (BtlEfxRevChkFar(act->efx) != 0) // ふっき
		kind = 5;
	return kind;
}

struct WindowLine
{
	u16 left;
	u16 span;
};

struct WindowBuffer
{
	u8				  unknown_000[4];
	struct WindowLine lines[160];
};

s32 Battle_CollectPartyCommands(struct BattleActionRecord *out, u16 *in, s32 count)
{
	struct LinkRoundState	   *state;
	struct BtlWork			   *link;
	struct BtlWork			   *session;
	u16						   *peer;
	u16						   *sync;
	struct BattleActionRecord  *ent;
	struct BtlUnit			   *actor;
	struct UiWindow			   *win;
	struct MenuSprite		   *slot;
	u16							hdr[2];
	s32							aux;
	s32							actorId;
	s32							mode;
	s32							kind;
	s32							param;
	s32							res1024;
	s32							res512;
	s32							miss;
	s32							i;
	s32							k;
	s32							order;

	void DrawPartyPanels(void)
	{
		u16 data[2];
		data[0] = 0xff;
		BattleLayout_HighlightPartyPanels(data);
		BattlePres_SetActorModesFar(data, 1);
	}

	aux = 256;
	kind = 0;
	param = 0;
	res1024 = Resource_LoadIntoFreeSlot(1024);
	res512 = Resource_LoadIntoFreeSlot(512);
	state = Runtime_AllocateBlock(57, 260);
	Resource_LoadIndexedIntoBuffer(res1024, 0);
	Graphics_ExpandVramTilesByColorTable(0x06006000);
	Link_DrawShiftedTilePair(0x06006680);
	Menu_BuildLocalizedPatternTiles();
	link = BtlWk;
	for (k = 0; k < 8; k++)
	{
		state->selected[k] = 0x80000000;
	}
	for (k = 0; k < 3; k++)
	{
		state->entryActive[k] = 0;
	}
	state->targetOffset = 0;
	state->currentOffset = 0;
	state->timer = -1;
	state->unk_3c = 0;
	state->unk_40 = 0;
	state->enabled = 0;
	state->secondaryHandle = 0;
	state->displayHandle = 0;
	if (link->two_sided != 0)
	{
		state->enabled = 1;
		sync = (u16 *)gSerialTransfer.payload;
		sync[4] = 'V';
		sync[5] = 'S';
		sync[6] = 'S';
		sync[7] = 'T';
		miss = 0;
		/* FAKEMATCH: instruction order only: the one-pass block's loop
           notes before the jump end the schedule there, so miss = 0 and the
           'T' store go out before the copies of the side and pause
           pointers that PRE puts before the jump, as native; its LOOP_CONT
           note keeps jump from copying the check in front (differ 4). */
		do
		{
			goto handshake_check;
		handshake_loop:
			if ((gLinkStatus & 3) != 3)
			{
				if (++miss > 24)
				{
					count = -1;
					state->enabled = 0;
					goto handshake_done;
				}
			}
			else
			{
				miss = 0;
				if (peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T')
				{
					goto handshake_done;
				}
				if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S')
				{
					goto handshake_done;
				}
				if (!(peer[0] == 'E' && peer[1] == 'X' && peer[2] == 'E' && peer[3] == 'C'))
				{
					count = -1;
					state->enabled = 0;
					goto handshake_done;
				}
			}
			WaitFrames(1);
		handshake_check:
			peer = (u16 *)&gLinkPeerSignatures[link->link_side ^ 1];
			if (link->link_paused == 0)
			{
				goto handshake_loop;
			}
		} while (0);
		count = -1;
		state->enabled = 0;
	}
handshake_done:
	Scheduler_AddOrUpdateCallback(UpdateLinkSessionCountdown, 3200);

	for (;;)
	{
		DrawPartyPanels();
		state->entryActive[2] = 0;
		state->actorId = -1;
		state->hintCount = 0;
		if (GameFlag_TestFar(366) != 0)
		{
			state->hintCount = 1;
			state->hintTimer = 60;
		}
		if (count > 0)
		{
			mode = Ui_RunSelectionScreen(0);
		}
		else
		{
			mode = 14;
		}
		if (mode == 7)
		{
			void *block;
			s32	  sel;
			s32	  handle;

			block = MemAlloc(12);
			if (gDebugMode != 0 && (gKeysHeld & KEY_START) != 0)
			{
				sel = 2;
			}
			else
			{
				sel = 1;
			}
			handle = BattleParty_ListActorIdsFar(sel, block);
			WaitFrames(1);
			Ui_RunOwnerStatusScreen(block, handle, *(u16 *)block);
			MemFree(block);
			continue;
		}
		if (mode == 4)
		{
			if (UiText_ShowLocalizedMessageAndWait() != 0)
			{
				continue;
			}
			count = 1;
			ent = out;
			ent->unit_id = in[0];
			ent->priority = 0x7ffe;
			ent->command = 99;
			ent->parameter = 0;
			ent->target = 256;
			goto finish;
		}
		if (mode != 14)
		{
			goto finish;
		}
		AudioCommand_PlayFar(154);
		i = 0;
		if (i >= count)
		{
			goto finish;
		}

		for (;;)
		{
			if (i == 0)
			{
				BattlePlacement_CountValidEntriesFar(0, state->cost[0]);
			}
			else
			{
				s32 k;

				for (k = 0; k < 4; k++)
				{
					state->cost[i][k] = state->cost[i - 1][k];
				}
			}

			ent = &out[i];
			actorId = in[i];
			/* FAKEMATCH: instruction order only: called as returning a
               doubleword, the call also sets r1, so the r1 reload for
               class_id no longer depends on the in reload and sched2 loads
               i * 2 before in, as native (differ 2). */
			actor = (struct BtlUnit *)(s32)((s64 (*)(s32))UnitGetFar)(actorId);
			COUNTDOWN->actorId = actorId;
			COUNTDOWN->unk_40 = 0;
			slot = &COUNTDOWN->entries[2];
			slot->oam.word.attr01 = 0x80000400;
			slot->oam.word.attr23 = 0;
			slot->oam.f.tile = Ui_LoadEntryForKind(actor->class_id, res512);
#if EDITION_INTERNATIONAL
			slot->oam.f.x = 0;
#else
			slot->oam.f.x = 4;
#endif
			slot->oam.f.y = 0x80;
			slot->oam.f.palette = 14;
			state->entryActive[2] = 1;

		mark_visible:
			UiWindow_MarkVisibleTileAttributes();
			COUNTDOWN->entryActive[0] = 0;
			COUNTDOWN->selected[i] = 0x80000000;
			/* FAKEMATCH: register choice (spill slots) only: twelve dead
               stores, one insn each until flow deletes them. GCSE sizes its
               expression table from the insn count (n / 2 | 1 buckets) and
               PRE numbers its reaching registers in bucket order, which is
               the order their spill slots are assigned. &hdr (sfp - 4)
               hashes to 13922 and i << 1, i << 2, i << 4 to 13966, 13967,
               13969: with these stores the table has 775 buckets (747
               against 16, 17, 19), so the givs take sp+32, 28 and 24
               before &hdr (sp+20) as in the native frame; without them 769
               (80 against 124, 125, 127) puts &hdr above the givs. 775
               needs 11 or 12 stores in every edition (Japanese is two
               insns longer). Without them differ 50 in every edition (en
               96 with the nested helpers' 46); every row they remove is a
               spill slot's offset. */
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = 0;
			hdr[0] = (u16)actorId;
			hdr[1] = 255;
			BattlePres_SetActorModesFar(hdr, 1);
			BattleLayout_HighlightPartyPanels(hdr);
			WaitFrames(1);
			mode = Ui_RunSelectionScreen(1);
			WaitFrames(1);
			if (mode == -2)
			{
				void *block;
				s32	  handle;

				block = MemAlloc(12);
				handle = BattleParty_ListActorIdsFar(1, block);
				state->entryActive[2] = 0;
				WaitFrames(1);
				Ui_RunOwnerStatusScreen(block, handle, actorId);
				state->entryActive[2] = 1;
				MemFree(block);
				continue;
			}
			BattlePres_SetActorModesFar(hdr, 0);
			if (mode == -1)
			{
				if (i == 0)
				{
					goto next_round;
				}
				i--;
				continue;
			}

			if (COUNTDOWN->timer == 0)
			{
				mode = 3;
			}
			slot = &COUNTDOWN->entries[0];
			slot->oam.word.attr23 = 0;
			slot->oam.word.attr01 = 0x80002400;
			slot->oam.f.tile = Resource_LoadIndexedIntoBuffer(res1024, mode);
			if (mode == 15)
			{
				slot->oam.f.x = 128;
			}
			else
			{
				slot->oam.f.x = 96;
			}
			slot->oam.f.y = 136;
			state->entryActive[0] = 1;
			switch (mode)
			{
			case 0:
				win = UiWindow_Create(11, 17, 11, 3, 6);
#if defined(TBS_EDITION_FR)
				UiText_DrawCharacterAtOffset((s32)MsgCommandName, (struct RenderInput *)win, 14, 0);
#else
				UiText_DrawCharacterAtOffset((s32)MsgCommandName, (struct RenderInput *)win, 16, 0);
#endif
				state->entries[0].oam.f.x = 64;
				AudioCommand_PlayFar(112);
				mode = BattleTarget_RunSelection(actorId, 1, 1, 0);
				UiWork_Finalize(win, 1);
				if (mode == -1)
				{
					goto mark_visible;
				}
				kind = 0;
				aux = mode;
				ent->rng = 1;
				break;

			case 1:
			{
				struct MenuSprite	*slot;
				struct BtlAct		*ability;
				struct UiRenderWork *ui;
				u16					*list;
				u8					*pick;
				s32					 cost;
				s32					 handle;
				s32					 cnt;
				s32					 k;
				s32					 cur;

				AudioCommand_PlayFar(112);
				mode = 0;
				/* FAKEMATCH: instruction order only: the one-pass block's loop
                   notes end the schedule here, so mode = 0 goes out before the
                   countdown base is loaded, as native (differ 4). */
				do
				{
				} while (0);
				COUNTDOWN->unk_34 = mode;
				COUNTDOWN->unk_30 = mode;
				COUNTDOWN->unk_38 = mode;
			psynergy_menu:
				list = state->list;
				pick = state->pick;
				state->entries[0].oam.f.x = 48;
				cnt = 0;
				k = 0;
				cur = actor->action_slots[k].encoded_action & ACTION_ID_MASK;
				if (cur != 0)
				{
					do
					{
						if ((BtlActGet(cur)->tgt_flg & 0x80) != 0)
						{
							pick[cnt] = k;
							list[cnt] = cur;
							cnt++;
						}
						k++;
						if (k == 32)
						{
							break;
						}
						cur = actor->action_slots[k].encoded_action & ACTION_ID_MASK;
					} while (cur != 0);
				}
				pick[cnt] = 0;
				list[cnt] = 0;
				mode = BattleMenu_RunActionSelection(actorId, list, cnt);
				if (mode == -1)
				{
					goto mark_visible;
				}
				param = actor->action_slots[pick[mode]].encoded_action & ACTION_ID_MASK;
				ability = BtlActGet(param);
				cost = ability->rng;
				slot = &state->entries[1];
				handle = Resource_LoadIntoFreeSlot(128);
				ui = UI_WORK;
				win = UiWindow_Create(8, 17, 18, 3, 6);
				state->entries[0].oam.f.x = 40;
				slot->oam.word.attr01 = 0x40000400;
				slot->oam.word.attr23 = 0;
				slot->oam.f.x = win->x * 8 + 8;
				slot->oam.f.y = win->y * 8 + 4;
				/* FAKEMATCH: instruction order only (pool placement): arm_reorg counts this empty asm as a 4-byte insn, so the pool goes in after mov r1, r10 as native, whose constant 40 acts as a range-60 (zero_extendhisi2) pool load (differ 14). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
				__asm__ volatile ("");
				slot->oam.f.tile = Resource_LoadIndexedEntryToBuffer(param, handle);
				state->entryActive[1] = 1;
				ui->level = 5;
				if ((s32)ability->pp_cost > (s32)actor->ep)
				{
					UiWork_SetParamNibble(2);
				}
				else if (actor->fuuji != 0)
				{
					UiWork_SetParamNibble(9);
				}
				UiText_DrawCharacterAtOffset(
					param + (s32)MsgAbilityName, (struct RenderInput *)win, 16, 0);
				UiText_DrawNumberAtOffset(ability->pp_cost, 2, (struct RenderInput *)win, 104, 0);
				ui->level = 15;
				UiWork_SetParamNibble(15);
				UiWindow_SetTilemapEntry(win, 61471, 11, 0, 0);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
				UiWindow_SetTilemapEntry(win, 61445, 12, 0, 0);
#else
				UiWindow_SetTilemapEntry(win, 61470, 12, 0, 0);
#endif
				if (ability->damage_class != 4)
				{
					UiWindow_SetTilemapEntry(win, ability->damage_class + 20481, 15, 0, 0);
				}
				ent->rng = cost;
				AudioCommand_PlayFar(112);
				mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
					Battle_ClassifyEntryKind((const struct BtlAct *)ability));
				state->entryActive[1] = 0;
				Resource_ResetEntry(handle);
				UiWork_Finalize(win, 1);
				if (mode == -1)
				{
					goto psynergy_menu;
				}
				kind = 1;
				aux = mode;
				break;
			}

			case 16:
			{
				const struct SummonDefinition *djinn;
				struct BtlAct				  *ability;
				struct MenuSprite			  *slot;
				s32							   handle;
				s32							   ok;
				s32							   j;
				s32							   n;

				AudioCommand_PlayFar(112);
				COUNTDOWN->unk_34 = 0;
				COUNTDOWN->unk_30 = 0;
				COUNTDOWN->unk_38 = 0;
			djinn_menu:
#if EDITION_INTERNATIONAL
				state->entries[0].oam.f.x = 80;
#else
				state->entries[0].oam.f.x = 104;
#endif
				BattleLayout_HighlightPartyPanels(hdr);
				mode = SummonMenu_SelectSummon(0, 0, state->cost[i]);
				BattleLayout_HighlightPartyPanels(hdr);
				if (mode == -1)
				{
					goto mark_visible;
				}
				kind = 6;
				param = mode;
				WaitFrames(1);
				slot = &state->entries[1];
				djinn = SummonDefinition_Get(mode);
				ability = BtlActGet(djinn->name_message_id);
				handle = Resource_LoadIntoFreeSlot(128);
#if EDITION_INTERNATIONAL
				win = UiWindow_Create(10, 17, 17, 3, 6);
#else
				win = UiWindow_Create(10, 17, 14, 3, 6);
#endif
				n = 0;
				for (j = 0; j <= 3; j++)
				{
					if (djinn->djinn_required[j] > state->cost[i][j])
					{
						break;
					}
				}
				ok = (j == 4);
				state->entries[0].oam.f.x = 56;
				slot->oam.word.attr01 = 0x40000400;
				slot->oam.word.attr23 = 0;
				/* FAKEMATCH: instruction order only (pool placement, schedule): counted as 4 bytes, the empty asm puts the pool after mov r2, r9 as native (constant 56) and keeps ok = 1 - (j != 4) among the slot stores (differ 12). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
				__asm__ volatile ("");
				slot->oam.f.x = win->x * 8 + 8;
				slot->oam.f.y = win->y * 8 + 4;
				slot->oam.f.tile =
					Resource_LoadIndexedEntryToBuffer(djinn->name_message_id & 0x3fff, handle);
				state->entryActive[1] = 1;
				if (ok == 0)
				{
					UiWork_SetParamNibble(2);
				}
				UiText_DrawCharacterAtOffset(
					SummonDefinition_Get(mode)->name_message_id + (s32)MsgAbilityName, win, 16, 0);
				for (j = 0; j <= 3; j++)
				{
					if (djinn->djinn_required[j] != 0)
					{
#if EDITION_INTERNATIONAL
						UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 13, 0, 0);
						UiWindow_PutGlyph(win, djinn->djinn_required[j] + 48, n * 2 + 14, 0, 0);
#else
						UiWindow_SetTilemapEntry(win, j + 20481, n * 2 + 10, 0, 0);
						UiWindow_PutGlyph(win, djinn->djinn_required[j] + 48, n * 2 + 11, 0, 0);
#endif
						n++;
					}
				}
				if (ok != 0)
				{
					AudioCommand_PlayFar(112);
				}
				else
				{
					AudioCommand_PlayFar(114);
				}
				mode = BattleTarget_RunSelection(actorId, ability->target_mode, ability->rng,
					Battle_ClassifyEntryKind((const struct BtlAct *)ability));
				ent->rng = ability->rng;
				state->entryActive[1] = 0;
				Resource_ResetEntry(handle);
				UiWork_Finalize(win, 1);
				if (mode == -1)
				{
					goto djinn_menu;
				}
				for (j = 0; j <= 3; j++)
				{
					if (djinn->djinn_required[j] > state->cost[i][j])
					{
						state->cost[i][j] = 0;
						break;
					}
					state->cost[i][j] -= djinn->djinn_required[j];
				}
				aux = mode;
				break;
			}

			case 15:
			{
				struct BtlAct	*ability;
				struct UiWindow *hint;
				s32				 cost;
				s32				 element;
				s32				 index;

				AudioCommand_PlayFar(112);
				COUNTDOWN->unk_34 = 0;
				COUNTDOWN->unk_30 = 0;
				COUNTDOWN->unk_38 = 0;
			summon_menu:
				state->entries[0].oam.f.x = 144;
				COUNTDOWN->selected[i] = 0x80000000;
				mode = Battle_SelectAbility(actorId, 1);
				ent->rng = 0;
				if (mode == -1)
				{
					goto mark_visible;
				}
				kind = 5;
				param = mode;
				COUNTDOWN->selected[i] = mode;
				element = (param >> 8) & 15;
				index = param & 255;
				if (Djinn_IsActiveFar(actorId, element, index) != 0)
				{
					ability = BtlActGet(Djinn_GetDefinitionHeaderFar(element, index));
					cost = ability->rng;
					win = UiWindow_Create(11, 17, 10, 3, 6);
					state->entries[0].oam.f.x = 64;
					UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
					UiText_DrawCharacterAtOffset(
						element * 20 + index + (s32)MsgDjinnName, win, 16, 0);
					ent->rng = cost;
					WaitFrames(1);
					AudioCommand_PlayFar(112);
					mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
						Battle_ClassifyEntryKind((const struct BtlAct *)ability));
					if (state->hintCount != 0)
					{
						hint = UiText_ShowMessageAndWaitComplete((s32)MsgDjinnUnleashHint, 15, 8);
						while (UiWork_IsComplete() == 0)
						{
							WaitFrames(1);
						}
						UiWork_Finalize(hint, 1);
						state->hintCount++;
						state->hintTimer = 45;
					}
					UiWork_Finalize(win, 1);
					if (mode == -1)
					{
						goto summon_menu;
					}
					aux = mode;
					break;
				}
				win = UiWindow_Create(11, 17, 10, 3, 6);
				state->entries[0].oam.f.x = 64;
				UiWork_SetParamNibble(2);
				UiWindow_SetTilemapEntry(win, element + 20481, 0, 0, 0);
				UiText_DrawCharacterAtOffset(element * 20 + index + (s32)MsgDjinnName, win, 16, 0);
				UiWork_SetParamNibble(15);
				ent->rng = 1;
				WaitFrames(1);
				AudioCommand_PlayFar(112);
				mode = BattleTarget_RunSelection(actorId, 4, 0, 7);
				UiWork_Finalize(win, 1);
				if (mode == -1)
				{
					goto summon_menu;
				}
				aux = mode;
				break;
			}

			case 2:
			{
				struct MenuSprite *slot;
				struct BtlAct	  *ability;
				u16				  *list;
				u8				  *pick;
				s32				   cost;
				s32				   handle;
				s32				   cnt;
				s32				   k;
				s32				   cur;
				s32				   sel;

				AudioCommand_PlayFar(112);
				mode = 0;
				/* FAKEMATCH: instruction order only: as in the psynergy case,
                   mode = 0 before the countdown base (differ 2). */
				do
				{
				} while (0);
				COUNTDOWN->unk_34 = mode;
				COUNTDOWN->unk_30 = mode;
				COUNTDOWN->unk_38 = mode;
			item_menu:
				state->entries[0].oam.f.x = 96;
				pick = state->pick;
				list = state->list;
				cnt = 0;
				k = 0;
				cur = actor->inventory[k];
				if (cur != 0)
				{
					do
					{
						Item_GetDirectFar(cur);
						if (Item_ClassifyUseAbility(actorId, cur) == 0)
						{
							list[cnt] = cur;
							pick[cnt++] = k;
						}
						k++;
						if (k == 15)
						{
							break;
						}
						cur = actor->inventory[k];
					} while (cur != 0);
				}
				k = 0;
				cur = actor->inventory[k];
				if (cur != 0)
				{
					do
					{
						Item_GetDirectFar(cur);
						if (Item_ClassifyUseAbility(actorId, cur) != 0)
						{
							list[cnt] = cur;
							pick[cnt++] = k;
						}
						k++;
						if (k == 15)
						{
							break;
						}
						cur = actor->inventory[k];
					} while (cur != 0);
				}
				list[cnt] = 0;
				mode = ItemList_SelectEntry(actorId, list, cnt);
				if (mode == -1)
				{
					goto mark_visible;
				}
				sel = pick[mode];
				param = sel;
				ability = BtlActGet(Item_GetDirectFar(actor->inventory[sel])->action_id);
				cost = ability->rng;
				slot = &state->entries[1];
				handle = Resource_LoadIntoFreeSlot(128);
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
				win = UiWindow_Create(9, 17, 16, 3, 6);
#else
				win = UiWindow_Create(9, 17, 15, 3, 6);
#endif
				state->entries[0].oam.f.x = 48;
				slot->oam.word.attr01 = 0x40000400;
				slot->oam.word.attr23 = 0;
				/* FAKEMATCH: instruction order only (pool placement): counted as 4 bytes, the empty asm puts the pool after ldr r1, [sp, #72] as native (constant 48) (differ 10). 2026-10-11: arm_reorg runs before shorten_branches and counts the empty asm at its 4-byte default length while it emits nothing; the halfword constant fix needs that much more estimated length after it, and no C statement or single pattern-length change (strh, str, orr, and tested over all 413 English objects) supplies it. */
				__asm__ volatile ("");
				slot->oam.f.x = win->x * 8 + 8;
				slot->oam.f.y = win->y * 8 + 4;
				slot->oam.f.tile = Resource_LoadKind26EntryToBuffer(actor->inventory[sel], handle);
				state->entryActive[1] = 1;
				UiText_DrawCharacterAtOffset((actor->inventory[sel] & 0x1ff) + (s32)MsgItemName,
					(struct RenderInput *)win, 24, 0);
				ent->rng = cost;
				AudioCommand_PlayFar(112);
				mode = BattleTarget_RunSelection(actorId, ability->target_mode, cost,
					Battle_ClassifyEntryKind((const struct BtlAct *)ability));
				state->entryActive[1] = 0;
				Resource_ResetEntry(handle);
				UiWork_Finalize(win, 1);
				if (mode == -1)
				{
					goto item_menu;
				}
				kind = 2;
				aux = mode;
				break;
			}

			case 3:
				kind = 3;
				break;
			}

			AudioCommand_PlayFar(110);
			ent->unit_id = (u16)actorId;
			order = actor->agi;
			if (order != 0)
			{
				order += (u32)(actor->agi * Random16()) >> 20;
			}
			ent->priority = order;
			if (i != 0 && in[i] == in[i - 1])
			{
				ent->priority = (s16)order / 2;
			}
			if ((s16)ent->priority < 0)
			{
				ent->priority = 2000;
			}
			ent->command = kind;
			ent->parameter = param;
			ent->target = aux;
			i++;
			if (i < count)
			{
				continue;
			}
			break;
		}

	next_round:
		if (i < count)
		{
			continue;
		}
		break;
	}

finish:
	if (state->enabled != 0)
	{
		sync = (u16 *)gSerialTransfer.payload;
		sync[4] = 'E';
		sync[5] = 'D';
		sync[6] = 'V';
		sync[7] = 'S';
	}
	if (state->displayHandle != 0)
	{
		UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
	}
	Resource_ResetEntry(res512);
	Resource_ResetEntry(res1024);
	Scheduler_RemoveCallback(UpdateLinkSessionCountdown);
	if (state->enabled != 0)
	{
		s32 DrawLocalizedResource(void)
		{
			u16 data[64];
			TEXT_LOAD((s32)MsgWaitingForOpponent, data, TEXT_COUNT);
			UiText_RenderWideStringAtOffset(data, (struct UiWindow *)state->displayHandle, 0, 4);
		}

		session = BtlWk;
		mode = 0;
		if (state->displayHandle == 0 && session->link_paused == 0)
		{
			state->displayHandle = (s32)UiWindow_Create(0, 16, 30, 4, 42);
			Ui_FillVramBlockPattern();
			DrawLocalizedResource();
		}
		else
		{
			state->displayHandle = 0;
		}
		for (;;)
		{
			peer = (u16 *)&gLinkPeerSignatures[session->link_side ^ 1];
			if (session->link_paused != 0)
			{
				count = -1;
				break;
			}
			if ((gLinkStatus & 3) != 3)
			{
				mode++;
				if (mode > 24)
				{
					count = -1;
					break;
				}
			}
			else
			{
				mode = 0;
				if (peer[4] == 'E' && peer[5] == 'D' && peer[6] == 'V' && peer[7] == 'S')
				{
					break;
				}
				if (!(peer[4] == 'V' && peer[5] == 'S' && peer[6] == 'S' && peer[7] == 'T'))
				{
					mode = 1;
				}
			}
			WaitFrames(1);
		}
		if (state->displayHandle != 0)
		{
			UiWork_Finalize((struct UiWindow *)state->displayHandle, 1);
		}
	}
	Camera_ConfigureSceneFar(0);
	Runtime_ReleaseHeapBlock(57);
	return count;
}

void AffineEffect_UpdateFrame(void)
{
	struct ResourceMenuWork	 *work;
	struct ResourceMenuEntry *sprite;
	struct AffineTransform	  effect;
	s32						  scale;
	s32						  matrix;
	s32						  edge;

	s32							   y;
	u32							   end;
	u32							   i;
	u16							  *line;
	struct DisplayTransitionState *buffers;

	work = gMenuSelectWork;
	sprite = work->entries;
	scale = Ui_BounceScales[(gFrameCount * 2) & 31];
	scale = (scale - 256) / 4 + 304;
	effect.scale_x = scale;
	effect.scale_y = scale;
	effect.angle = 0;
	matrix = AffineMatrix_BuildForEffect(&effect);
	for (i = 0; i < work->count; i++, sprite++)
	{
		u32 *attr;
		s32	 slot;
		s32	 px;

		px = sprite->x;
		if (px == 0)
			continue;
		if (i == work->selection)
		{
			s32 left = px + scale * 7 / 512 - 20;

			if (sprite->y != 0)
				y = sprite->y + scale * 3 / 256 - 20;
			else
				y = (scale * 15 / 256 - 30) & 255;
			attr = &sprite->link;
			*attr++ = 0;
			*attr++ = (matrix << 25) | y | (left << 16) | 0x80002300;
			*attr = gVramBlockCache[sprite->slot].offset >> 5;
			slot = 246;
		}
		else
		{
			u32 base;
			u32 value;

			attr = &sprite->link;
			/* FAKEMATCH: needed to match: reading the y halfword through attr
             * keeps the attribute-pointer copy before its load; sprite->y
             * moves the copy two instructions later. */
			y = ((s16 *)attr)[7];
			*attr++ = 0;
			value = y | (px << 16) | 0x80002000;
			base = gVramBlockCache[sprite->slot].offset;
			*attr++ = value;
			*attr = base >> 5;
			slot = 245;
		}
		Runtime_PushSlotEntry(sprite, slot);
	}
	if (work->row == 0)
	{
		struct WindowBuffer *window;

		s32 span;

		buffers = (struct DisplayTransitionState *)gDisplayTransition;
		if (buffers == 0 || work->count == 0)
			return;
		window = (struct WindowBuffer *)buffers->pages[buffers->page];
		edge = work->entries[work->selection].x;
		span = (scale * 12 - 2817) / 256;
		edge = ((edge - span) << 8) + (edge + span) + 23;
		end = work->entries[work->selection].y + 24;
		end += (scale * 32 - 7937) / 512 + 1;
		line = &window->lines[24].span;
		i = 24;
		if (i < end)
		{
			do
			{
				*line = (*line & 0xff) | edge;
				i++;
				line += 2;
			} while (i < end);
		}
		edge = work->entries[0].x;
		if (work->selection == 0)
			edge -= (scale * 12 - 2817) / 256;
		edge <<= 8;
		line = &window->lines[0].span;
		for (i = 0; i <= 23; i++)
		{
			*line = (*line & 0xff) | edge;
			line += 2;
		}
	}
	else
	{
		struct WindowBuffer *window;
		s32					 start;

		s32 span;

		buffers = (struct DisplayTransitionState *)gDisplayTransition;
		if (buffers == 0 || work->count == 0)
			return;
		window = (struct WindowBuffer *)buffers->pages[buffers->page];
		edge = work->entries[work->selection].x;
		span = (scale * 12 - 2817) / 256;
		edge = ((edge - span) << 8) + (edge + span) + 23;
		start = work->entries[work->selection].y;
		start -= (scale * 32 - 7937) / 512 + 1;
		line = &window->lines[start].span;
		i = start;
		if (i <= 135)
		{
			do
			{
				*line = (*line & 0xff) | edge;
				i++;
				line += 2;
			} while (i <= 135);
		}
		edge = work->entries[0].x;
		if (work->selection == 0)
			edge -= (scale * 12 - 2817) / 256;
		edge <<= 8;
		line = &window->lines[136].span;
		for (i = 136; i <= 159; i++)
		{
			*line = (*line & 0xff) | edge;
			line += 2;
		}
	}
}

void *AffineEffect_InitializeWork(void)
{
	void		*work;
	volatile u32 zero;
	work = Runtime_AllocateBlock(58, 152);
	zero = 0;
	Dma_Set(&zero, work, 0x85000026, REG_DMA3);
	Scheduler_AddOrUpdateCallback(AffineEffect_UpdateFrame, 0xc76);
	return work;
}

void Menu_EndResourceSelection(void)
{
	struct UiWindow			*child;
	s32						 i;
	struct ResourceMenuWork *work;

	work = gMenuSelectWork;
	Scheduler_RemoveCallback(AffineEffect_UpdateFrame);
	child = work->window;
	if (child != 0)
	{
		UiWork_Finalize(child, 2);
	}
	i = 0;
	while (i < work->count)
	{
		Resource_ResetEntry(work->entries[i].slot);
		i += 1;
	}
	Runtime_ReleaseHeapBlock(0x3A);
	WaitFrames(1U);
}

s32 Menu_RunResourceSelectionLoop(s32 initial)
{
	struct ResourceMenuWork *work = gMenuSelectWork;
	s32						 resource;

	work->selection = initial;
redraw:
	RenderOutput_PrepareForRedraw(work->window);
	if (work->resource_base != 0)
		resource = work->resource_base + work->selection;
	else
		resource = work->resource_ids[work->selection] + (s32)MsgCommandName;
	UiText_DrawCharacterAtOffset(resource, work->window, 0, 0);
	for (;;)
	{
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(112);
			return work->selection;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			return -1;
		}
		if (gKeyState & KEY_START)
		{
			AudioCommand_PlayFar(113);
			return -1;
		}
		if ((gKeysRepeat & KEY_LEFT) || (gKeysRepeat & KEY_UP))
		{
			AudioCommand_PlayFar(111);
			work->selection--;
			if (work->selection < 0)
				work->selection = work->count - 1;
			goto redraw;
		}
		if ((gKeysRepeat & KEY_RIGHT) || (gKeysRepeat & KEY_DOWN))
		{
			AudioCommand_PlayFar(111);
			work->selection++;
			if (work->selection >= work->count)
				work->selection = 0;
			goto redraw;
		}
	}
}

static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs)
{
	if (diff >= 0)
		return diff;
	return rhs - lhs;
}

s32 Menu_SelectResource(s32 start, s32 goal)
{
	s16						 resource_base;
	s16						 cur;
	s32						 diff;
	s32						 resource_id;
	s32						 dist;
	s32						 pos;
	s32						 step;
	s32						 delay;
	const u8				*tbl;
	struct ResourceMenuWork *state;

	state = gMenuSelectWork;
	step = 1;
	delay = 12;
	state->selection = start;
	if (goal < start)
		step = -1;
	pos = start;

	for (;;)
	{
		RenderOutput_PrepareForRedraw(state->window);
		resource_base = state->resource_base;
		if (resource_base != 0)
		{
			resource_id = resource_base + state->selection;
		}
		else
		{
			resource_id = state->resource_ids[state->selection] + (s32)MsgCommandName;
		}
		UiText_DrawCharacterAtOffset(resource_id, state->window, 0, 0);

		cur = state->selection;
		tbl = Menu_SelectionStepDelays;
		diff = cur - goal;
		dist = AbsoluteDifference(diff, cur, goal);
		WaitFrames(tbl[dist] + delay);

		if (pos == goal)
			break;

		state->selection += step;
		AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
		delay = 0;
		pos += step;
	}

	WaitFrames(48);
	AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
	return goal;
}

void Menu_LoadResourceSlot(s32 slot, s32 index)
{
	s32	  size = 1024;
	void *buffer = MemAlloc(size);
	u16	 *base = Resource_GetTableEntry((s32)&ResourceId_CommandIcons);

	Resource_DecodeByteLz((void *)((u32)base + base[index]), buffer);
	VramBlock_LoadCached(slot, size, buffer);
	MemFree(buffer);
}

void Menu_AppendResourceEntry(s32 no)
{
	struct ResourceMenuWork	 *work;
	struct ResourceMenuEntry *entry;
	s16						  index;
	s32						  slot;
	s32						  flags;

	work = gMenuSelectWork;
	index = work->count;
	if (index <= 5)
	{
		work->count++;
		entry = &work->entries[index];
		slot = Resource_FindFreeEntry();
		Menu_LoadResourceSlot(slot, no);
		entry->x = index * 24 + 32;
		flags = 136;
		entry->y = flags;
		entry->slot = slot;
		work->resource_ids[index] = no;
	}
}

void Menu_CenterResourceEntries(s32 row, s32 width, s32 resource_base)
{
	struct ResourceMenuWork *menu = gMenuSelectWork;
	s32						 x;
	s32						 i;
	s32						 count;

	menu->width = width + 2;
	menu->resource_base = resource_base;
	menu->row = row;
	count = menu->count;
	x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
	for (i = 0; i < menu->count; i++)
	{
		struct ResourceMenuEntry *entry = &menu->entries[i];

		entry->x = x * 8;
		entry->y = row * 8;
		x += 3;
	}
	menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}

void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base)
{
	struct ResourceMenuWork	 *work;
	s32						  i;
	struct ResourceMenuEntry *entry;

	work = gMenuSelectWork;
	work->width = width + 2;
	work->resource_base = resource_base;
	work->row = y;
	for (i = 0; i < work->count; i++)
	{
		entry = &work->entries[i];
		entry->y = y << 3;
		entry->x = x << 3;
		x += 3;
	}
	work->window = UiWindow_Create(x, y, work->width, 3, 2);
}

static __inline__ s32 TblGet(s8 *tbl, s32 index)
{
	return tbl[index];
}

s32 Menu_SelectTopEntry(s32 sel)
{
	s32 group = 0;
	s32 ofs;
	s32 triple;
	s32 ret;
	s8 *tbl;

	if (Party_SumDjinnCountsFar(-1) == 0)
	{
		group = 1;
	}

	triple = group * 3;
	tbl = Menu_TopEntryPositionByCommand;
	ofs = triple << 1;
	sel = TblGet(tbl, sel + ofs) - 1;
	if (sel < 0)
	{
		sel = 0;
	}

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(1);
	if (group == 0)
	{
		Menu_AppendResourceEntry(15);
	}
	Menu_AppendResourceEntry(2);
	Menu_AppendResourceEntry(7);
	Menu_CenterResourceEntries(17, SELECT_MENU_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();

	if (ret >= 0)
	{
		ret = Menu_TopEntryCommandByPosition[ret + ofs + 1];
	}

	return ret;
}

#if EDITION_INTERNATIONAL
#define ANIMATE_ENTRIES_ROW 7
#else
#define ANIMATE_ENTRIES_ROW 5
#endif

s32 Menu_AnimateSelectionToEntry(s32 arg0, s32 arg1)
{
	UiWindow_OpenMode1AndWaitFrame();
	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(1);
	Menu_AppendResourceEntry(0xF);
	Menu_AppendResourceEntry(2);
	Menu_AppendResourceEntry(7);
	Menu_CenterResourceEntries(0x11, ANIMATE_ENTRIES_ROW, 0);
	arg1 = Menu_SelectResource(arg0, arg1 - 1);
	Menu_EndResourceSelection();
	UiWork_CloseAndRelease();
	return arg1;
}

s32 Menu_SelectSaveSlotAction(void)
{
	s32 type;
	s32 ret;
	s32 initial;
	u32 group;

	group = 0;
	initial = 0;
	type = SaveState_ScanRecordFlags();
	if (type < 0)
	{
		return -1;
	}
	if (type == 0)
	{
		return 0;
	}
	if (type == 3)
	{
		group = 1;
	}
	else if (type == 0x67)
	{
		group = 2;
	}
	else if (type > 0x64)
	{
		group = 3;
	}
	else
	{
		initial = 1;
	}
	AffineEffect_InitializeWork();
	if ((group == 0) || (group == 3))
	{
		Menu_AppendResourceEntry(0x15);
	}
	if (group <= 1U)
	{
		Menu_AppendResourceEntry(0x16);
	}
	if ((group == 0) || (group == 3))
	{
		Menu_AppendResourceEntry(0x17);
	}
	Menu_AppendResourceEntry(0x18);
	if ((*(s16 *)gTitleExtraOptionEnabled) != 0)
	{
		Menu_AppendResourceEntry(0x1D);
	}
	if (gTitleSendOptionEnabled != 0)
	{
		Menu_AppendResourceEntry(0x1E);
	}
	Menu_CenterResourceEntries(0x11, TYPE_MENU_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(initial);
	Menu_EndResourceSelection();
	if (ret >= 0)
	{
		ret = Menu_SaveSlotActionByPosition[ret + (group * 6)];
	}
	return ret;
}

#if EDITION_INTERNATIONAL
#define MODE_LABEL_X 18
#define MODE_LABEL_RIGHT 144
#else
#define MODE_LABEL_X 8
#define MODE_LABEL_RIGHT 80
#endif

void Menu_DrawModeLabel(void)
{
	struct ResourceMenuWork *state = gMenuSelectWork;

	if (state->previous_mode != state->selection)
	{
		state->previous_mode = state->selection;
		UiWindow_ClearInteriorTiles(state->lower_window, 8, 40, MODE_LABEL_RIGHT, 80);

		if (state->selection != 1)
		{
			if (state->selection > 1)
				goto mode_other;
			if (state->selection != 0)
				goto mode_other;

			{
				s32 text = (s32)MsgPasswordLevel;

				UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
				UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
				UiText_DrawCharacterAtOffset(text + 2, state->lower_window, MODE_LABEL_X, 56);
				UiText_DrawCharacterAtOffset(text + 3, state->lower_window, MODE_LABEL_X, 64);
				text += 4;
				UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 72);
				goto done;
			}
		}

		{
			s32 text = (s32)MsgPasswordLevel;

			UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 40);
			UiText_DrawCharacterAtOffset(text + 1, state->lower_window, MODE_LABEL_X, 48);
			text += 2;
			UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 56);
			goto done;
		}

	mode_other:
		{
			s32 text = (s32)MsgPasswordLevel;

			UiText_DrawCharacterAtOffset(text++, state->lower_window, MODE_LABEL_X, 40);
			UiText_DrawCharacterAtOffset(text, state->lower_window, MODE_LABEL_X, 48);
		}
	done:;
	}
}

#if EDITION_INTERNATIONAL
#define PASSWORD_HELP_X 16
#else
#define PASSWORD_HELP_X 0
#endif

#if defined(TBS_EDITION_FR)

void Menu_DrawModeIndicator(void)
{
	struct ResourceMenuWork *state = gMenuSelectWork;

	if (state->previous_mode != state->selection)
	{
		if (state->selection == 0)
		{
			RenderOutput_ClearList(state->lower_window);
			UiText_DrawResource((s32)MsgPasswordTransferHelp, (s32)state->lower_window, 16, 4);
			UiText_DrawResource((s32)MsgPasswordTransferHelp + 1, (s32)state->lower_window, 16, 16);
		}
		else if (state->previous_mode == 0)
		{
			RenderOutput_ClearList(state->lower_window);
			UiText_DrawResource((s32)MsgCableTransferHelp, (s32)state->lower_window, 0, 4);
			UiText_DrawResource((s32)MsgCableTransferHelp + 1, (s32)state->lower_window, 0, 16);
			UiText_DrawResource((s32)MsgCableTransferHelp + 2, (s32)state->lower_window, 0, 28);
		}
		state->previous_mode = state->selection;
	}
}
#else
void Menu_DrawModeIndicator(void)
{
	struct ResourceMenuWork *state = gMenuSelectWork;
	s16						*shown = &state->previous_mode;
	s16						*current = &state->selection;

	if (*shown != *current)
	{
		*shown = *current;
		RenderOutput_PrepareForRedraw(state->lower_window);
		if (*current == 0)
		{
#if defined(TBS_EDITION_DE)
			UiText_DrawCharacterAtOffset((s32)MsgPasswordTransferHelp, state->lower_window, 32, 8);
			UiText_DrawCharacterAtOffset(
				(s32)MsgPasswordTransferHelp + 1, state->lower_window, 32, 24);
#else
			UiText_DrawResource(
				(s32)MsgPasswordTransferHelp, state->lower_window, PASSWORD_HELP_X, 4);
			UiText_DrawResource(
				(s32)MsgPasswordTransferHelp + 1, state->lower_window, PASSWORD_HELP_X, 16);
#endif
		}
		else
		{
#if defined(TBS_EDITION_DE)
			UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp, state->lower_window, 0, 0);
			UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 1, state->lower_window, 0, 16);
			UiText_DrawCharacterAtOffset((s32)MsgCableTransferHelp + 2, state->lower_window, 0, 32);
#else
			UiText_DrawResource((s32)MsgCableTransferHelp, state->lower_window, 0, 4);
			UiText_DrawResource((s32)MsgCableTransferHelp + 1, state->lower_window, 0, 16);
			UiText_DrawResource((s32)MsgCableTransferHelp + 2, state->lower_window, 0, 28);
#endif
		}
	}
}
#endif

s32 Menu_SelectResourceLayout(s32 mode)
{
	struct ResourceMenuWork *work;
	struct UiWindow			*window;
	s32						 msg;
	struct UiWindow		   **upper;
	s32						 result;

	AffineEffect_InitializeWork();
	work = gMenuSelectWork;
	if (mode == 0)
	{
		Menu_AppendResourceEntry(44);
		Menu_AppendResourceEntry(45);
	}
	else
	{
		Menu_AppendResourceEntry(46);
		Menu_AppendResourceEntry(47);
		Menu_AppendResourceEntry(48);
#if defined(TBS_EDITION_ES)
		Menu_CenterResourceEntries(17, 5, 0);
#endif
	}
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
	Menu_CenterResourceEntries(17, 7, 0);
#elif defined(TBS_EDITION_ES)
#elif defined(TBS_EDITION_FR)
	Menu_CenterResourceEntries(17, 9, 0);
#else
	Menu_CenterResourceEntries(17, 5, 0);
#endif
	if (mode != 0)
	{
		Scheduler_AddOrUpdateCallback(Menu_DrawModeLabel, 0xc76);
		work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) || defined(TBS_EDITION_IT)
		window = UiWindow_Create(7, 0, 17, 4, 2);
#elif defined(TBS_EDITION_ES)
		window = UiWindow_Create(5, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR)
		window = UiWindow_Create(5, 0, 22, 4, 2);
#else
		window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
		msg = (s32)MsgPasswordSelection;
		upper = &work->upper_window;
		*upper = window;
		UiText_DrawResource(msg, (s32)window, 0, 4);
#if EDITION_INTERNATIONAL
#if defined(TBS_EDITION_DE)
		window = UiWindow_Create(1, 4, 28, 12, 2);
#else
		window = UiWindow_Create(3, 4, 25, 12, 2);
#endif
		work->lower_window = window;
		UiText_DrawResource(msg + 1, (s32)window, 8, 0);
		UiText_DrawResource(msg + 2, (s32)work->lower_window, 8, 11);
		msg += 3;
		UiText_DrawResource(msg, (s32)work->lower_window, 8, 22);
#else
		window = UiWindow_Create(5, 4, 21, 12, 2);
		work->lower_window = window;
		UiText_DrawResource(msg + 1, (s32)window, 0, 4);
		msg += 2;
		UiText_DrawResource(msg, (s32)work->lower_window, 0, 16);
#endif
	}
	else
	{
		Scheduler_AddOrUpdateCallback(Menu_DrawModeIndicator, 0xc76);
		work->previous_mode = 0xffff;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
		window = UiWindow_Create(6, 0, 18, 4, 2);
#elif defined(TBS_EDITION_ES)
		window = UiWindow_Create(4, 0, 21, 4, 2);
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
		window = UiWindow_Create(5, 0, 20, 4, 2);
#else
		window = UiWindow_Create(9, 0, 13, 4, 2);
#endif
		upper = &work->upper_window;
		*upper = window;
#if EDITION_INTERNATIONAL
		UiText_DrawResource((s32)MsgTransferMethod, (s32)window, 2, 4);
#else
		UiText_DrawResource((s32)MsgTransferMethod, (s32)window, 0, 4);
#endif
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
		work->lower_window = UiWindow_Create(1, 5, 28, 7, 2);
#elif defined(TBS_EDITION_DE)
		work->lower_window = UiWindow_Create(0, 5, 30, 7, 2);
#else
		work->lower_window = UiWindow_Create(5, 5, 21, 7, 2);
#endif
	}
	result = Menu_RunResourceSelectionLoop(0);
	if (mode != 0)
		Scheduler_RemoveCallback(Menu_DrawModeLabel);
	else
		Scheduler_RemoveCallback(Menu_DrawModeIndicator);
	RenderOutput_PrepareForRedraw(*upper);
	RenderOutput_PrepareForRedraw(work->lower_window);
	UiWork_Finalize(*upper, 2);
	UiWork_Finalize(work->lower_window, 2);
	WaitFrames(1);
	Menu_EndResourceSelection();
	return result;
}

//--------------------------------------------------------------------------
// confirm_selection.c
// メニュー：コマンドのアイコンを並べてえらばせる（はい・いいえ など）
//--------------------------------------------------------------------------
// アイコンは ResourceId_CommandIcons の番号。
// えらんでいるコマンドの名前をウィンドウに出す
//--------------------------------------------------------------------------
// アイコン 0x11〜0x14 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry11To14(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x11);
	Menu_AppendResourceEntry(0x12);
	Menu_AppendResourceEntry(0x13);
	Menu_AppendResourceEntry(0x14);
#if defined(TBS_EDITION_DE) || defined(TLA_EDITION_DE)
	Menu_CenterResourceEntries(17, 8, 0); // 名前のウィンドウの幅
#elif EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 7, 0);
#else
	Menu_CenterResourceEntries(17, 6, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// アイコン 0x19〜0x1c を画面の中央に並べてえらぶ（行 17）
//--------------------------------------------------------------------------
s32 Menu_SelectEntry19To1c(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x19);
	Menu_AppendResourceEntry(0x1A);
	Menu_AppendResourceEntry(0x1B);
	Menu_AppendResourceEntry(0x1C);
#if EDITION_INTERNATIONAL
	Menu_CenterResourceEntries(17, 10, 0);
#else
	Menu_CenterResourceEntries(17, 8, 0);
#endif
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

//--------------------------------------------------------------------------
// はい・いいえ（アイコン 5・6）をえらぶ。キャンセルは 1（いいえ）
// low  : 0 なら行 0、0 でなければ行 17 に出す
// mes  : 名前のメッセージの先頭（0 ならコマンド名）
// w    : 名前のウィンドウの幅（0 なら 3）
// sel  : はじめにえらんでいる位置
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelection(s32 low, s32 mes, s32 w, s32 sel)
{
	s32 row;

	row = 0;

	AffineEffect_InitializeWork();

	if (w == 0)
	{
		w = 3;
	}
	if (low != 0)
	{
		row = 17;
	}

	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_CenterResourceEntries(row, w, mes);

	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}

//--------------------------------------------------------------------------
// はい・いいえを (x, y) に出してえらぶ。キャンセルは 1（いいえ）
//--------------------------------------------------------------------------
s32 Menu_RunConfirmSelectionAt(s32 x, s32 y, s32 sel)
{
	s32 mes = (s32)MsgCommandYes;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(5);
	Menu_AppendResourceEntry(6);
	Menu_LayoutResourceEntries(x, y, 3, mes);
	sel = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	if (sel == -1)
	{
		sel = 1;
	}
	return sel;
}

//--------------------------------------------------------------------------
// select_entry_20_to_21.c
// メニュー：コマンドのアイコン 0x20・0x21 の 2 つからえらばせる
//--------------------------------------------------------------------------
// 版ごとの名前のウィンドウの幅
#if !EDITION_INTERNATIONAL
#define PAIR_WIDTH 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || defined(TLA_EDITION_FR) \
	|| defined(TLA_EDITION_IT)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

//--------------------------------------------------------------------------
// アイコン 0x20・0x21 を画面の中央に並べてえらぶ（行 17）
// sel : はじめにえらんでいる位置。戻り値はえらんだ位置
//--------------------------------------------------------------------------
s32 Menu_SelectEntry20To21(s32 sel)
{
	s32 ret;

	AffineEffect_InitializeWork();
	Menu_AppendResourceEntry(0x20);
	Menu_AppendResourceEntry(0x21);
	Menu_CenterResourceEntries(17, PAIR_WIDTH, 0);
	ret = Menu_RunResourceSelectionLoop(sel);
	Menu_EndResourceSelection();
	return ret;
}

void Scheduler_ScheduleCallbackCAfterFrames(void)
{
	Scheduler_AddOrUpdateCallback((SchedulerCallback)Menu_RunSelection, 0xC80);
}

void Menu_DrawSelectionRow(struct UiWindow *work, s32 first, const s16 *second)
{
	s16 selected = first;
	s32 label = Scene_GetAreaNameIndexFar(selected, *second) + (s32)MsgDebugEntryName;
	RenderOutput_PrepareForRedraw(work);
	UiText_DrawNumber(selected, 3, (s32)work, 0, 14);
	UiText_DrawNumber(*second, 3, (s32)work, MENU_LABEL_X, 14);
	UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 0);
	UiText_DrawResource(selected + (s32)MsgDebugCategoryName, (s32)work, 0, 0);
	UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 14);
	UiText_DrawResource(label, (s32)work, MENU_LABEL_X, 0);
}

s16 Menu_RunSelection(void)
{
	s32				  resource;
	s16				  mode;
	s16				  secondary;
	struct MenuSprite object;
	s16				  primary;
	struct UiWindow	 *work;
	s16				  result;

	work = 0;
	mode = 0;
	primary = gGameState.default_scene;
	secondary = gGameState.default_entrance;
	work = UiWindow_Create(0, 7, 30, 5, 2);
	Menu_DrawSelectionRow(work, primary, &secondary);
	UiTextResource_Initialize(&object, &resource);

	while (gKeysHeld != 0)
		WaitFrames(1);

	for (;;)
	{
		result = Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
		if (result == -1)
		{
			UiTextResource_Release(resource);
			UiWork_Finalize(work, 2);
			Event_SetDestinationFar(primary, secondary);
			return result;
		}
		if (result == -2)
		{
			UiTextResource_Release(resource);
			UiWork_Finalize(work, 2);
			return result;
		}

		UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
		primary = result;
		WaitFrames(1);
	}
}

s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode)
{
	if (gKeysRepeat & KEY_A)
		return -1;
	if (gKeysRepeat & KEY_B)
		return -2;
	if (gKeysRepeat & KEY_DOWN)
	{
		*mode ^= 1;
	}
	else if (gKeysRepeat & KEY_UP)
	{
		*mode ^= 1;
	}
	else if (gKeysRepeat & KEY_RIGHT)
	{
		if (*mode == 0)
		{
			value++;
		}
		else
		{
			(*sub)++;
			if (*sub > 99)
				*sub = 0;
		}
		if (value > 200)
			value = 0;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	else if (gKeysRepeat & KEY_LEFT)
	{
		if (*mode == 0)
		{
			value--;
		}
		else
		{
			(*sub)--;
			if (*sub < 0)
				*sub = 99;
		}
		if (value < 0)
			value = 200;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	else if (gKeysRepeat & KEY_R)
	{
		if (*mode == 0)
		{
			*sub = 0;
			value += 10;
		}
		else
		{
			*sub += 10;
			if (*sub > 99)
				*sub -= 99;
		}
		if (value > 200)
			value = 0;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	else if (gKeysRepeat & KEY_L)
	{
		if (*mode == 0)
		{
			*sub = 0;
			value -= 10;
		}
		else
		{
			*sub -= 10;
			if (*sub < 0)
				*sub += 99;
		}
		if (value < 0)
			value = 200;
		Menu_DrawSelectionRow((struct UiWindow *)arg, value, sub);
	}
	return value;
}

//--------------------------------------------------------------------------
// run_selection_with_cursor_object.c
// メニュー：フラグの一覧画面（カーソルで選んで A で切りかえる）
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 1 ページ 16x16 のフラグを表示し、閉じるまで入力を受ける
// ページ 8 から始める。戻り値はいつも 0
//--------------------------------------------------------------------------
s32 Menu_RunSelectionWithCursorObject(void)
{
	struct MenuSprite obj;	  // カーソルのオブジェクト
	s32				  cur[2]; // カーソルの列・行
	s32				  res;
	s32				  page;
	s32				  win;
	s32				  ret;

	page = 8;
	win = 0;
	cur[0] = win;
	cur[1] = win;
	win = UiWindow_Create(1, 0, 28, 20, 2);
	Menu_DrawFlagBitTable(win, page);
	UiTextResource_Initialize(&obj, &res);
	for (;;)
	{
		WaitFrames(1);
		ret = Menu_HandleFlagGridInput(win, &page, cur);
		if (ret == -1)
			break; // 閉じる
		if (ret == 1)
			Menu_DrawFlagBitTable(win, page); // フラグかページが変わった
		UiTextResource_SetPosition(&obj, cur[0] * 8 + 58, cur[1] * 8 + 20);
	}
	UiTextResource_Release(res);
	UiWork_Finalize(win, 2);
	return 0;
}

//--------------------------------------------------------------------------
// format_hex.c
// 文字：数を 16 進の文字列にする
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// value の下から count けた（5 けたまで）を、上のけたから大文字の
// 16 進で out に書く（終わりの 0 は書かない）
//--------------------------------------------------------------------------
void Text_FormatHex(u32 value, u32 count, u8 *out)
{
	u8	buf[8];
	u32 i;

	if (count > 5)
		count = 5;

	for (i = 0; i != count; i++)
	{
		u32 d = value & 15;
		if (d <= 9)
			buf[i] = d + '0';
		else
			buf[i] = d + 'A' - 10;
		value >>= 4;
	}

	for (i = count - 1; (s32)i >= 0; i--)
		*out++ = buf[i];
}

void Menu_DrawFlagBitTable(s32 window, s32 start_flag)
{
	s32	 row;
	s32	 y;
	s32	 flag;
	char label[5];
	char bits[17];

	RenderOutput_PrepareForRedraw((struct UiWindow *)window);
	UiText_DrawStringInWindow((u8 *)Menu_HexDigitsString, window, 48, 0);

	flag = start_flag << 8;
	for (row = 0; row != 16; row++)
	{
		s32 i;

		y = row * 8 + 16;

		for (i = 0; i != 5; i++)
		{
			label[i] = 0;
		}
		Text_FormatHex(flag, 3, (s32)label);
		UiText_DrawStringInWindow((u8 *)label, window, 0, y);
		UiText_DrawStringInWindow((u8 *)Menu_ColonString, window, 32, y);

		for (i = 0; i < 16; i++)
		{
			s32 val = GameFlag_TestFar(flag);
			bits[i] = (val != 0) + 48;
			flag++;
		}
		bits[i] = 0;
		UiText_DrawStringInWindow((u8 *)bits, window, 48, y);
	}
}

s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor)
{
	s32 *row = &cursor[1];

	if (gKeysRepeat & KEY_A)
	{
		s32 flag = ((*page << 4) + *row << 4) + cursor[0];

		if (GameFlag_TestFar(flag))
			GameFlag_ClearBitFar(flag);
		else
			GameFlag_SetBitFar(flag);
		return 1;
	}
	if ((gKeyState & KEY_B) || (gKeysRepeat & KEY_SELECT))
		return -1;
	if (gKeysRepeat & KEY_UP)
	{
		if (--*row < 0)
			*row = 15;
	}
	else if (gKeysRepeat & KEY_DOWN)
	{
		if (++*row > 15)
			*row = 0;
	}
	else if (gKeysRepeat & KEY_LEFT)
	{
		if (--cursor[0] < 0)
			cursor[0] = 15;
	}
	else if (gKeysRepeat & KEY_RIGHT)
	{
		if (++cursor[0] > 15)
			cursor[0] = 0;
	}
	else if ((gKeysRepeat & KEY_L) && (gKeysRepeat & KEY_START))
	{
		*page -= 10;
		if (*page < 0)
			*page = 15;
		return 1;
	}
	else if ((gKeysRepeat & KEY_R) && (gKeysRepeat & KEY_START))
	{
		*page += 10;
		if (*page > 15)
			*page = 0;
		return 1;
	}
	else if (gKeysRepeat & KEY_L)
	{
		if (--*page < 0)
			*page = 15;
		return 1;
	}
	else if (gKeysRepeat & KEY_R)
	{
		if (++*page > 15)
			*page = 0;
		return 1;
	}
	return 0;
}

void Menu_ReservedNoOp(void)
{
}

void Djinn_SetFoundFlagAndAdd(s32 owner, s32 element, s32 index)
{
	GameFlag_SetBitFar((element * 0x14) + index + 0x30);
	Djinn_AddToOwnerFar(owner, element, index);
}

s32 Menu_RunWorkspaceResultLoop(void)
{
	s32 result;

retry:
	result = Menu_RunWorkspaceSelectionLoop();
	if (result == -1)
	{
		return -1;
	}
	if (result == 0)
	{
		if (Save_WriteSelectedSlot() == -1)
		{
			goto retry;
		}
	}
	else if (result == 1)
	{
		UiText_ShowPositionedMessageAndWait((s32)MsgSleepMode, 1);
		gSleepRequested = result;
	}
	else if (result == 2)
	{
		if (Menu_RunWorkspaceOptions() == -1)
		{
			goto retry;
		}
	}
	return 0;
}

s32 DebugMenu_BrowseIcons(void)
{
	s32					redraw;
	s32					tile;
	s32					page;
	s32					mode;
	struct RenderInput *window;
	s32					slot;
	s32					base;
	s32					i;
	s32					row;
	s32					x;
	s32					y;

	window = NULL;
	page = 0;
	mode = 0;
	redraw = 1;
	gMenuCtrlWork->suspended = redraw;
	WaitFrames(1);

	do
	{
		if (gKeysRepeat & KEY_LEFT)
		{
			page--;
			redraw = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			redraw = 1;
			page++;
		}
		if (gKeysRepeat & KEY_L)
		{
			mode--;
			redraw = 1;
		}
		if (gKeysRepeat & KEY_R)
		{
			redraw = 1;
			mode++;
		}
		if (gKeysRepeat & KEY_A)
			goto close;
		if (gKeysRepeat & KEY_B)
			goto close;
		if (redraw != 0)
		{
			redraw = 0;
			page = (page + 8) % 8;
			mode = (mode + 3) % 3;
			UiWork_Finalize(window, 2);
			window = UiWindow_Create(10, 0, 18, 12, 2);
			if (mode == 0)
				UiText_DrawStringInWindow(Workspace_ItemTitle, window, 0, 0);
			else if (mode == 1)
				UiText_DrawStringInWindow(Workspace_EnergyTitle, window, 0, 0);
			else
				UiText_DrawStringInWindow(Workspace_StatusTitle, window, 0, 0);
			UiText_DrawStringInWindow(Workspace_PageLabel, window, 0, 8);
			UiText_DrawNumberInWindow(page, 0, window, 40, 8);
			base = page << 5;
			UiText_DrawNumberInWindow(base, 3, window, 64, 8);
			UiText_DrawStringInWindow(Workspace_RangeDash, window, 88, 8);
			UiText_DrawNumberInWindow(base + 31, 3, window, 96, 8);
			for (i = 0; i <= 31; i++)
			{
				slot = -1;
				row = i / 8;
				x = (i - row * 8) * 16;
				y = row * 16 + 16;
				if (mode == 0)
				{
					UiIcon_BuildItemIconTiles(base + i, 1, &slot, &tile, mode);
					RenderOutput_Create(slot, 0x40000000, window, x, y);
				}
				else if (mode == 1)
				{
					UiIcon_BuildAbilityIconTiles(base + i, 1, &slot, &tile, 0);
					RenderOutput_Create(slot, 0x40000000, window, x, y);
				}
				else
				{
					slot = Resource_FindFreeEntry();
					Ui_BuildPatternToSlot(i, 0, slot);
					RenderOutput_Create(slot, 0x40000000, window, x, y);
				}
			}
		}
		WaitFrames(1);
	} while (1);
close:
	UiWork_Finalize(window, 2);
	gMenuCtrlWork->suspended = 0;
	return 0;
}

#if !EDITION_INTERNATIONAL || defined(TBS_EDITION_EN)
#define DEBUG_GLYPH_NAME ((s32)MsgDebugGlyphName)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_GLYPH_NAME ((s32)MsgEnd)
#else
#define DEBUG_GLYPH_NAME ((s32)MsgDebugSecondForm)
#endif

s32 DebugMenu_BrowseEntryGlyphs(void)
{
	struct UiRenderWork *work;
	struct RenderInput	*portrait;
	s32					 redraw;
	struct RenderInput	*window;
	s32					 count;
	s32					 total;
	s32					 index;
	s32					 i;
	s32					 glyph;
	s32					 tile;
	s32					 slot;
	s32					 cancel;
	u32					 slot_offset;

	work = UI_WORK;
	redraw = 1;
	portrait = UiWindow_CreateWithSideObject(0, 0, 10, 5);
	window = UiWindow_Create(10, 10, 14, 3, 2);
	index = 0;
	for (i = 0; Portrait_CharacterMap[i * 2] != -1; i++)
	{
	}
	count = i;
	for (i = 0; Portrait_SpriteMap[i * 2] != -1; i++)
	{
	}
	total = count + i;

	/* FAKEMATCH: the count-derived mask is needed to match the native mask
     * in r9 and work in fp; a direct constant swaps them (four instructions). */
	cancel = ((u32)(u16)count >> 16) | KEY_B;
	/* FAKEMATCH: defining the offset before the loop keeps its load in r1; defining it inside redraw uses r3 (4 instructions differ in all six editions). */
	slot_offset = RENDER_SLOT_VALUE_OFS + 2;
	while (1)
	{
		if (gKeysRepeat & KEY_LEFT)
		{
			redraw = 1;
			index--;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			redraw = 1;
			index++;
		}
		if (gKeysRepeat & KEY_L)
		{
			redraw = 1;
			index -= 10;
		}
		if (gKeysRepeat & KEY_R)
		{
			redraw = 1;
			index += 10;
		}
		if (gKeysRepeat & KEY_A)
			break;
		if (gKeysRepeat & cancel)
			break;
		if (redraw)
		{
			redraw = 0;
			index = (index + total) % total;
			RenderOutput_PrepareForRedraw(window);
			if (index < count)
				glyph = Portrait_CharacterMap[index * 2 + 1];
			else
				glyph = Portrait_SpriteMap[(index - count) * 2 + 1] + 128;
			{
				/* FAKEMATCH: the word temporary is needed to match palette's
                 * r2 through the slot store; direct assignment chooses r3. */
				s32 pal;
				s32 loaded = *(u16 *)((u8 *)work + slot_offset);
				pal = 15;
				slot = loaded;
				UiGlyph_LoadEntryWithPalette(glyph, 0, &slot, &tile, pal, 1);
			}
			UiText_DrawNumberInWindow(index, 2, window, 0, 0);
			UiText_DrawCharacterAtOffset(index + DEBUG_GLYPH_NAME, window, 24, 0);
		}
		WaitFrames(1);
	}
	UiWork_Finalize(window, 2);
	UiWork_Finalize(portrait, 2);
	WaitFrames(1);
	return 0;
}
