#include "unit/window/window.h"
#include "unit/window/notice.h"
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
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
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
