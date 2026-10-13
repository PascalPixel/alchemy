//--------------------------------------------------------------------------
// output_acquire_free.c
// 表示：スプライト出力（RenderOutput）のあきリスト
//--------------------------------------------------------------------------

#include "unit/window/mark_codes.h"
#include "unit/window/scales.h"
#include "unit/window/sideobjt.h"
#include "unit/window/spranim.h"
#include "unit/window/textspd.h"
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
#include "unit/runtime/oamlist.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
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

			/* FAKEMATCH: the size in a block of its own, so the slot number goes to r0 before the size's copy to r1 (differ 2 as RamCode_LoadSlot). */
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

			/* FAKEMATCH: the size in a block of its own, so the slot number goes to r0 before the size's copy to r1 (differ 2 as RamCode_LoadSlot). */
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
