//--------------------------------------------------------------------------
// output_acquire_free.c
// 表示：スプライト出力（RenderOutput）のあきリスト
//--------------------------------------------------------------------------

#include "unit/window/scales.h"
#include "unit/window/spranim.h"
#include "unit/window/textspd.h"
#include "unit/window/window.h"
#include "unit/window/btlcmd.h"
#include "unit/object/object.h"
#include "unit/object/psyfx.h"
#include "unit/window/glyph_arm.h"
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
	for (i = 0; i != WINDOW_COUNT; i++, slot++)
	{
		if ((slot->flags & 1) == 0 && slot->duration == 0)
		{
			found = slot;
			break;
		}
	}
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
		UiWork_ResetCounters(x);
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
	/* FAKEMATCH: storing the new tail as a RenderOutput * keeps both stores in one alias set, so they stay in order. The typed list->tail_link = &node->next lets the scheduler put the tail store first (differ 2 in all six editions). */
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
			selected->work = work;
			selected->x = 0x300;
		}
		else if (preserve == 0)
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
			selected->x = 0x300;
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
						channel->work->state = 1;
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
						channel->work->state = 1;
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
	TEXT_VOICED_MARK = 222, // dakuten
	TEXT_SEMIVOICED_MARK = 223 // handakuten
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
			if (next == TEXT_VOICED_MARK)
			{
				code |= 0x4000;
				ch->entry = (ch->entry + 1) & RENDER_ENTRY_MASK;
			}
			else if (next == TEXT_SEMIVOICED_MARK)
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
	if (count > 0)
	{
		Dma_Fill16(value, dst, count * 2);
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
	/* FAKEMATCH: never read: the native frame keeps 8 bytes above the two outgoing arguments, exactly these four slots; without them the frame is 8 bytes, not 16 (differ 4 in tbs-en). */
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

	/* FAKEMATCH: the empty one-pass block ends a scheduling region, keeping the spill of out ahead of the work-address load. Without it the load is scheduled first (differ 2 in tbs-ja, the only edition with this body); assigning work after mark = 0 instead moves the zero too (differ 6). */
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
			/* FAKEMATCH: assigning digits in the mark argument computes buf + 7 in r1 and makes the loop's copy in r5, as the native does. A plain buf + 7 argument computes it in r5 and makes the copy in r1 (differ 6 in tbs-ja, the only edition with this body). */
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
			/* FAKEMATCH: the same assignment in the argument as the outlined path above; a plain buf + 7 argument differs the same way (differ 6 in tbs-ja). */
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
		{
			u32 size;

			/* FAKEMATCH: instruction order only: the size in a block of its own, so the slot number goes to r0 before the size's copy to r1. 2026-10-13, tbs-en: RamCode_LoadSlot, the size and the block as initialized locals, and the size written at both uses with no local all differ 2 (the copy to r1 first). */
			do
			{
				size = (u32)Text_DecodeSymbolCodeSize;
			} while (0);
			Dma_Set((const void *)Text_DecodeSymbol, Runtime_AllocateHeapBlock(50, size),
				0x84000000 | (size >> 2), REG_DMA3);
		}
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
						UiText_DecodeMessage(tmp + MsgClassName, name, 24);
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
							UiText_DecodeMessage(tmp + MsgItemPluralName, name, 24);
						else
							UiText_DecodeMessage(tmp + MsgItemName, name, 24);
#else
						UiText_DecodeMessage((tmp & RENDER_ENTRY_MASK) + MsgItemName, name, 24);
#endif
						pos = UiText_AppendArticleName(0, name, pos, entry, no, plural, &suffix);
						break;
					case 21:
						tmp = UiRender_LookupNamedValue(4, clear);
						UiText_DecodeMessage(tmp + MsgAbilityName, name, 24);
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
								+ MsgDebugEntryName,
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
						/* FAKEMATCH: value counts the letters, compared unsigned, keeping the allocation of Spanish and Italian. With the function's u32 j or i as the counter: differ 44 or 58 in tbs-es, 476 or 486 in tbs-it. */
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
		{
			u32 size;

			/* FAKEMATCH: instruction order only: the size in a block of its own, as in the international copy above; RamCode_LoadSlot differs 2 (the copy to r1 first). */
			do
			{
				size = (u32)Text_DecodeSymbolCodeSize;
			} while (0);
			Dma_Set((const void *)Text_DecodeSymbol, Runtime_AllocateHeapBlock(50, size),
				0x84000000 | (size >> 2), REG_DMA3);
		}
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
						UiText_DecodeMessage(tmp + MsgClassName, name, 24);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 20:
						tmp = UiRender_LookupNamedValue(2, clear);
						UiText_DecodeMessage((tmp & 511) + MsgItemName, name, 24);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 21:
						tmp = UiRender_LookupNamedValue(4, clear);
						UiText_DecodeMessage(tmp + MsgAbilityName, name, 24);
						pos = UiText_AppendArticleName(mode, name, pos, entry, 0, 0, 0);
						break;
					case 23:
						UiText_DecodeMessage(
							Scene_GetAreaNameIndexFar(UiRender_LookupNamedValue(6, clear), 1)
								+ MsgDebugEntryName,
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
							message = MsgMysteryPersonName;
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
				gap = ((gap << 8) / (counts[i] - 1));
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
#endif
		if (character != ' ')
		{
			RamCode_Load(draw, UiText_DrawGlyphArm, UiText_DrawGlyphCodeSize);
			result = draw(window, character, x, y, font);
#if !EDITION_INTERNATIONAL
			if ((u16)mark != 0)
				result += draw(window, (u16)mark, x + result, y, font);
#endif
			MemFree(draw);
			return result;
		}
	}
	else if (character == ' ')
	{
#if !EDITION_INTERNATIONAL
		width = 7;
#else
		width = 5;
#endif
	}
	else
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
				{
					sprite->affine_index = 0;
					sprite->affine = 0;
					sprite->x = output->x;
					sprite->y = output->y;
					break;
				}
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
				if (output->anim_step != 0)
				{
					effect.scale_x = 320;
					effect.scale_y = 320;
					effect.angle = 0;
					sprite->affine_index = AffineMatrix_BuildForEffect(&effect);
					sprite->affine = 3;
					sprite->x = output->x - 8;
					sprite->y = output->y - 8;
					output->anim_step--;
					break;
				}
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
	for (;;)
	{
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
		if (done)
			break;
		WaitFrames(1);
	}

	// 直接開いたウィンドウ
	for (;;)
	{
		if (i == WINDOW_COUNT)
			break;
		if (win->flags != 0)
			UiWork_Finalize(win, 0);
		win++;
		i++;
	}
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
	/* FAKEMATCH: three more names of the one table keep each case's own literal slot; with the one name in every case the switch folds away, differ 21 on tbs-en (9 instructions for 24). */
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

struct GlyphCursor
{
	u16 unused;
	u16 cursor;
};

static __inline__ void ResetCursor(struct GlyphCursor *cursor)
{
	/* FAKEMATCH: the helper takes the node's base field as its record, so each row store forms base + 372 and stores the cursor at #2, as the native does. A plain work->records[7].kind = 0 folds the offset into 374 and stores at #0 (differ 6). */
	cursor->cursor = 0;
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
			UiText_DrawCharacterAtOffset(MsgAbilityNotKnown, screen->window, 0, 0);
			break;
		case 2:
			UiText_DrawCharacterAtOffset(MsgItemNotHeld, screen->window, 0, 0);
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
		/* FAKEMATCH: the separate local loads the tile pointer early into r5 and makes its copy late, as the native does. Assigning Menu_CursorObjectTiles directly where right is read: differ 42; the same at the top of the branch: differ 16 (tbs-en). */
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
		u32 first = node->message - MsgCommandName;

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
		u32 id = node->message - MsgCommandName;

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
		node->message = base + MsgCommandName;
		break;
	case 2:
		if (reuse != 0)
			src = node->slot;
		UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
		node->message = base + MsgItemName;
		break;
	case 4:
		if (reuse != 0)
			src = node->slot;
		Ability_LoadGlyph(base, 1, &src, &tile, reuse);
		node->message = base + MsgAbilityName;
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

	for (;;)
	{
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
				continue;
			break;
		case 2:
			ret = ItemMenu_RunFar();
			if (ret != 0)
				continue;
			break;
		case 3:
			ret = RunAssetSelectionScreenFar();
			if (ret == -1)
				continue;
			break;
		case 4:
			ret = Menu_OpenActionFlow();
			if (ret == -1)
				continue;
			break;
		default:
			break;
		}
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

struct TimedNoticeWork
{
	u8				 unknown_000[0x230];
	struct UiWindow *window;
	u16				 countdown;
};

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
		/* FAKEMATCH: needed to match: the title message reuses the slot variable. The message id passed directly: differ 23. */
		slot = MsgPanelHpMaxLabel;
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
				UiText_DrawCharacterAtOffset(MsgPanelStatusLabel, win, 48, 0);
				UiText_DrawCharacterAtOffset(list[first].ability + MsgAbilityName, win, 56, 16);
				UiText_DrawCharacterAtOffset(list[second].ability + MsgAbilityName, win, 56, 32);
				UiText_DrawCharacterAtOffset(MsgPanelStatusLabel - 2, win, 16, 16);
				UiText_DrawCharacterAtOffset(MsgPanelStatusLabel - 1, win, 16, 32);
				UiText_DrawCharacterAtOffset(list[first].owner + MsgCharacterName, win, 104, 16);
				UiText_DrawCharacterAtOffset(list[second].owner + MsgCharacterName, win, 104, 32);
				RenderOutput_RedrawSavedRect(info);
				UiText_DrawCharacterAtOffset(MsgPsynergyNeedMorePp, info, 0, 16);
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
				UiText_DrawCharacterAtOffset(ability + MsgAbilityDescription, info, 0, 0);
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
			/* FAKEMATCH: the unread flag forces fresh zeros for both entries and the cursor argument instead of reusing the row register. Without the store, also without the flag: differ 4. */
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
	ShopCursor_AdvanceFar(&work->cursor.cursor);
	ShopCursor_MoveTowardTargetFar(&work->marker[0].cursor);
	ShopCursor_MoveTowardTargetFar(&work->marker[1].cursor);

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

	work = Runtime_AllocateBlock(20, 0x628);
	Dma_Fill32(0, work, 0x628);
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
	msg = MsgWindowColorLabel;
	UiText_DrawCharacterAtOffset(msg, win, 8, 0);
	msg++;
	UiText_DrawCharacterAtOffset(msg, win, 8, 16);
#if EDITION_INTERNATIONAL
	msg = MsgMessageSpeedLabel;
	UiText_DrawCharacterAtOffset(msg, win, 8, 32);
	msg++;
	UiText_DrawCharacterAtOffset(msg, win, 32, 40);
#else
	UiText_DrawCharacterAtOffset(MsgMessageSpeedLabel, win, 8, 40);
#endif
	UiText_DrawCharacterAtOffset(MsgSpeechLabel, win, 8, 64);
	UiText_DrawCharacterAtOffset(MsgAutoSleepLabel, win, 8, 88);

	x = Resource_FindFreeEntry();
	if (x < RESOURCE_SLOTS)
	{
		VramBlock_LoadCached(x, 128, Resource_FixedBlockBTiles);
		out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
		work->cursor.output = out;
		x = win->x * 8;
		y = win->y * 8 + 12;
		ShopCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
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
		ShopCursor_SetPositionImmediateFar(&work->marker[0].cursor, x, y);
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
		ShopCursor_SetPositionImmediateFar(&work->marker[1].cursor, x, y);
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
			Shop_SetCursorFar(&work->marker[0].cursor, x, y, 1);
			x = icon->x * 8 + OPTION_SLIDER_X;
			x += (*pB * 60) / work->option_count[1];
			y = icon->y * 8 + 20;
			Shop_SetCursorFar(&work->marker[1].cursor, x, y, 1);

			x = work->option[2] + MsgMessageSpeedSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 40, OPTION_SPEED_END, 48);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 40);
			x = work->option[3] + MsgSpeechSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 64, OPTION_SETTING_END, 72);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 64);
			x = work->option[4] + MsgAutoSleepSetting;
			UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 88, OPTION_SETTING_END, 96);
			UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 88);
			PaletteGlow_Update(*pA, *pB);

			x = icon->x * 8;
			y = (page * 3 + icon->y) * 8 + 4;
			if (page == 0)
				y += 8;
			Shop_SetCursorFar(&work->cursor.cursor, x, y, 3);
			RenderOutput_ClearList(win);
			UiText_DrawResource(page + MsgOptionHelp, win, 0, 0);
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

	ShopCursor_AdvanceFar(&work->cursor.cursor);
	index = work->page;
	Ui_ApplyTableScaleToObject(work->icon[index]);
}

void Menu_InitializeSelectedWorkspace(void)
{
	void *work;
	work = Runtime_AllocateBlock(20, 0x628);
	Dma_Fill32(0, work, 0x628);
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
		msg = MsgWorkspaceSaveQuest;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4);
		msg++;
		UiText_DrawResource(msg, win, WORKSPACE_TEXT_X, 4 + WORKSPACE_ROW_HEIGHT);
		y = 4 + 2 * WORKSPACE_ROW_HEIGHT;
	}
	UiText_DrawResource(MsgWorkspaceChangeSettings, win, WORKSPACE_TEXT_X, y);
	y += WORKSPACE_ROW_HEIGHT;
	if (gDebugMode != 0)
	{
		msg = MsgWorkspaceRoughMenu;
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
		ShopCursor_SetPositionImmediateFar(&work->cursor.cursor, x, y);
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
	for (;;)
	{
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
			Shop_SetCursorFar(&work->cursor.cursor, x, y, 3);
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
			continue;
		}
		break;
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
