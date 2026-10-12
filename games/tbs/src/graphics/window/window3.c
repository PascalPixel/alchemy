#include "unit/graphics/font/mark_codes.h"
#include "unit/graphics/window/scales.h"
#include "unit/graphics/window/sideobjt.h"
#include "unit/graphics/window/spranim.h"
#include "unit/graphics/window/textspd.h"
#include "unit/menu/optdata.h"
#include "unit/graphics/window/window.h"
#include "unit/graphics/window/window2.h"
#include "unit/graphics/window/window3.h"
#include "unit/graphics/window/window4.h"
#include "unit/graphics/window/window5.h"
#include "unit/graphics/window/window6.h"
#include "unit/graphics/window/window7.h"
#include "unit/graphics/window/window8.h"
#include "unit/graphics/window/window9.h"
#include "unit/graphics/window/window10.h"
#include "unit/graphics/window/window11.h"
#include "unit/graphics/window/window12.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
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
