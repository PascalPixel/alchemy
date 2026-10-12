#include "unit/menu/item/menu.h"
#include "unit/menu/item/item.h"
#include "unit/menu/item/psynergy.h"
#include "unit/menu/item/status.h"
#include "unit/menu/item/useitem.h"
#include "unit/menu/item/compare.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/heap.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"
#include "uiwindow.h"
#include "mem.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "btlsys.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "fixed_math.h"
#include "lib.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "field_sprite.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"
#include "keystate.h"
#include "window.h"
#include "menu_result.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "sound_ids.h"
#include "iwram_call.h"
#include "heap_state.h"
#include "menu_list.h"
#include "irq.h"
#include "object_factory.h"
#include "far_runtime.h"
#include "equipment_menu.h"
#include "owner_state.h"
#include "inventory.h"
#include "layout_guard.h"
#include "btltype.h"
#include "btlcalc.h"
#include "eventwrk.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "battle_efx.h"
#include "four_object_motion.h"

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#define PROMPT_SAVES_TILES 1
#define PROMPT_TILES ((void *)0x06004000)
#define PROMPT_TILES_SIZE 0x2000
#endif

s32 Menu_OpenConfirmPrompt(void)
{
#if defined(PROMPT_SAVES_TILES)
	void *saved = Runtime_BumpAllocateAlternatePool(PROMPT_TILES_SIZE);
#endif
	struct PsynergyMenuState *state = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
	s32						  high;
	s32						  unused;
	s32						  low;
	s32						  result;

	((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
	UiWindow_InitializeWork(0);
	state->owner_count = Party_ListActiveOwnersFar(state->owner_table);
	ItemMenu_Init(0, 3, 0, 7);
	state->message_window = UiWindow_CreateFar(13, 0, 17, 3, 2);
	Palette_LightenBankHighlight(14);
	Link_DrawShiftedTilePairFar(0x06002500);
#if defined(PROMPT_SAVES_TILES)
	Scheduler_EnableOverlayCallbacksWithFlags();
	Iwram_CopyWords(saved, PROMPT_TILES, PROMPT_TILES_SIZE);
	Iwram_FillWords(PROMPT_TILES, PROMPT_TILES_SIZE, 0x33333333);
	UiWork_SetAltFlagAndClearTableFar(1);
#endif
	Menu_CancelSoundReset();
	result = Menu_ResolveSelectedAction(&high, &unused, &low);
	Menu_EnsureCancelSound();
	if (result == 1)
	{
		struct EventWork *target = WORK_SLOT(HEAP_SLOT_EVENT);
		u16				  flags;
		BtlActGet(OWNER_ACTION_ID_MASK & state->pane_action[0]);
		flags = low | (high << 10);
		target->psynergy_request = flags;
	}
	RenderOutput_ClearListFar((void *)state->status_window);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 1;
	ItemMenu_Close();
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	Runtime_ReleaseHeapBlock(0x37);
	((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
#if defined(PROMPT_SAVES_TILES)
	UiWindow_MarkVisibleTileAttributesFar();
	UiWork_SetAltFlagAndClearTableFar(0);
	Iwram_CopyWords(PROMPT_TILES, saved, PROMPT_TILES_SIZE);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 0;
	MemFree(saved);
	WaitFrames(1);
	Scheduler_DisableOverlayCallbacksWithFlags();
#endif
	WaitFrames(1);
	UiWindow_EraseBorderRectFar(0, 0, 30, 20);
	((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menu_busy = 0;
	Event_ClearInvalidPackedValuesFar();
	return result;
}

s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action)
{
	struct PsynergyMenuState *work;
	s32						  result;
	s32						  state;
	s32						  done;
	s32						  selection;
	s32						  classification;
	s32						  raw;
	s32						  self_flag;
	struct BtlAct			 *action;

	state = 0;
	done = 0;
	work = gMenuWork;
	result = 0;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		switch (state)
		{
		case 0:
		{
			s32 cancelled;
			s32 chosen;

			work->pane_row[0] = 0;
			ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseOwner);
			chosen = PsynergyMenu_SelectPartySlot(0);
			cancelled = -1;
			/* FAKEMATCH: the redundant cancellation copy ends the CSE block,
             * preserving the native cancellation/done register choices.
             * Without it, r2/r3 exchange and 12 instructions differ.
             * XOR equality keeps the cancellation copy into the result. */
			if (chosen == -1)
				cancelled = chosen;
			if (((u32)chosen ^ (u32)cancelled) == 0)
			{
				done = 1;
				result = cancelled;
			}
			RenderOutput_RedrawSavedRectFar(work->info_window);
			state = 1;
			break;
		}

		case 1:
			WaitFrames(1);
			UnitGetFar(work->owner_ids[0]);
			state = 0;
			if (work->psynergy_count != 0)
			{
				{
					struct PsynergyMenuState *saved_work = work;

					/* FAKEMATCH: the discarded conditional restoration
                     * ends the CSE block, so the mode offset is built from
                     * its own constant instead of adding 80 to the earlier
                     * pane offset. Both conditional arms disappear. */
					if (done)
						work = NULL;
					if (done)
						work = saved_work;
				}
				switch (work->mode)
				{
				case 0:
					ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseAbility);
					break;
				case 1:
					ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseForL);
					break;
				case 2:
					ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseForR);
					break;
				}
				ItemMenu_PosCategory();
				Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[0], 0, 0);
				selection = PsynergyMenu_RunList(0);
				state = 0;
				if (selection != -1)
				{
					state = 2;
					if (work->mode != 0)
					{
						if (work->mode == 1)
						{
							PsynergyMenu_SetShortcut(work->owner_ids[0], selection, 0);
							RenderOutput_ClearListFar((void *)work->info_window);
							InventoryMenu_ShowModalMessage((s32)MsgShortcutSetL, -1, -1);
#if !EDITION_INTERNATIONAL
							RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
						}
						else
						{
							PsynergyMenu_SetShortcut(work->owner_ids[0], selection, 1);
							RenderOutput_ClearListFar((void *)work->info_window);
							InventoryMenu_ShowModalMessage((s32)MsgShortcutSetR, -1, -1);
#if !EDITION_INTERNATIONAL
							RenderOutput_RedrawSavedRectFar(work->info_window);
#endif
						}
						state = 0;
					}
				}
			}
			break;

		case 3:
			ItemMenu_DrawMsg(0, (s32)MsgPsynergyChooseTarget);
			self_flag = PsynergyMenu_SelectTarget(0);
			state = 4;
			if (self_flag == -1)
			{
				work->flags |= 1;
				state = 1;
			}
			break;

		case 2:
			classification = PsynergyMenu_ClassifySelectedPsynergy();
			if (classification == 1)
			{
				state = 3;
				break;
			}
			if (classification == 2)
			{
				work->owner_ids[1] = 9;
				state = 4;
				break;
			}
			done = 1;
			result = 1;
			*out_owner = work->owner_ids[0];
			*out_action = work->pane_action[0] & OWNER_ACTION_ID_MASK;
			break;

		case 4:
			self_flag = 0;
			raw = work->pane_action[0];
			result = BattleEffect_ApplyToTargets(raw, work->owner_ids[0], work->owner_ids[1], 0);
			if (work->owner_ids[1] == 9)
			{
				work->owner_ids[1] = work->owner_ids[0];
				self_flag = 9;
			}
			if (result != -1)
			{
				action = BtlActGet(work->pane_action[0] & OWNER_ACTION_ID_MASK);
				UnitEpAddFar(work->owner_ids[0], -action->pp_cost);
			}
			UnitCalcFar(work->owner_ids[0]);
			if (result != -1)
			{
				Menu_DrawOwnerStatusPanel(work->status_window, work->owner_ids[1], 0, 0);
				Ability_PlayUseAnimation(work->pane_action[0] & OWNER_ACTION_ID_MASK);
				RenderOutput_ClearListFar((void *)work->info_window);
				InventoryMenu_ShowModalMessage(work->use_result + (s32)MsgItemUseResult, 0, -1);
			}
			else
			{
				AudioCommand_PlayFar(114);
				RenderOutput_ClearListFar((void *)work->info_window);
				InventoryMenu_ShowModalMessage(
					work->use_result + (s32)MsgItemUseResult, result, result);
			}
			if (result != -1)
			{
				result = 1;
				work->flags |= 1;
				state = 1;
			}
			else
			{
				work->skip_slide = 1;
				if (self_flag == 9)
				{
					work->flags |= 1;
					state = 1;
				}
				else
				{
					state = 3;
				}
			}
			break;

		default:
			done = 1;
			break;
		}
	}

	if (GameFlag_TestFar(0x150) != 0)
	{
		result = -1;
	}
	return result;
}

s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
	struct BtlAct *psynergy;
	s32			   diff;
	s32			   ret;

	psynergy =
		BtlActGet(OWNER_ACTION_ID_MASK & ((struct PsynergyMenuState *)gMenuWork)->pane_action[0]);
	if (BattleFx_HasMatchingEvent5Far(psynergy->type_0c) != 0)
	{
		return 0;
	}
	ret = 2;
	if (psynergy->rng != 0xff)
	{
		u8 kind = psynergy->target_mode;
		diff = kind ^ 2;
		ret = (0 - diff) | diff;
		ret = (u32)ret >> 0x1f;
		ret = 1 - ret;
	}
	return ret;
}

s32 PsynergyMenu_SelectPartySlot(s32 party_slot)
{
	/* FAKEMATCH: retain the original scalar tab/icon address lanes. Direct indexed owner fields change the 244-byte native object to 246 bytes and reorder loads around the cursor call. */
	struct PsynergyMenuState *menu = gMenuWork;
	s32						  tab_offset = party_slot + ((u8 *)menu->tab_index - (u8 *)menu);
	s32						  icon_offset =
		party_slot * sizeof(menu->pane_icon[0]) + ((u8 *)menu->pane_icon - (u8 *)menu);
	struct RenderOutput *icon;
	u8					 byte_val;
	s32					 tab;
	s32					 owner_offset;
	s32					 table_offset;
	s32					 owner_id;
	struct BtlUnit		*owner;
	u16					*actions;
	u8					*counts;
	s32					 badge;
	s32					 result;
	s32					 icon_offset2;

	result = 0;
	icon = *(struct RenderOutput **)((u8 *)menu + icon_offset);
	icon->active = 1;
	icon->anim_step = result;
	menu->cursor_icon->active = 13;
	counts = (u8 *)menu + ((u8 *)menu->tab_counts - (u8 *)menu->tab_index);
	tab = *(s8 *)((u8 *)menu + tab_offset);
	byte_val = menu->owner_count;
	counts[tab_offset] = byte_val;

	if (tab == -1)
	{
		*((u8 *)menu + tab_offset) = 0;
		owner_offset = 0;
	}
	else
	{
		owner_offset = tab * sizeof(menu->owner_table[0]);
		UiMenu_SlideCursor(tab * 24 - 10, 16);
	}

	table_offset = owner_offset + ((u8 *)menu->owner_table - (u8 *)menu);
	owner_id = *(u16 *)((u8 *)menu + table_offset);
	owner = UnitGetFar(owner_id);
	actions = menu->psynergies;
	badge = PsynergyMenu_CollectActions(owner, actions, 2);
	menu->psynergy_count = badge;
	result = PsynergyMenu_SetupActionIcons(menu->owner_table, actions);

	icon_offset2 = party_slot * sizeof(menu->pane_icon[0]) + ((u8 *)menu->pane_icon - (u8 *)menu);
	icon = *(struct RenderOutput **)((u8 *)menu + icon_offset2);
	UiIcon_PrepareObject(icon);
	WaitFrames(1);
	return result;
}

s32 PsynergyMenu_SetupActionIcons(u16 *owner_ids, u16 *unused)
{
	struct PsynergyMenuState *menu;
	s32						  selection;
	s32						  count;
	s32						  pending;
	s32						  result;
	struct BtlUnit			 *owner;
	s32						  window;
	s32						  shown;
	s32						  i;
	u16						 *actions;
	s32						  action_count;
	s8						  found;

	menu = gMenuWork;
	selection = menu->tab_index[0];
	count = menu->tab_counts[0];
	pending = 1;
	result = 0;
	shown = 0;
	menu->mode = result;
	owner = UnitGetFar(owner_ids[selection]);
	if (UiWindow_UpdateOrCreate(&menu->psynergy_window, 13, 3, 17, 10, 2))
		Menu_SpawnIconEntries((struct InventoryMenuState *)menu, menu->psynergy_window);
	if (UiWindow_UpdateOrCreate(&menu->shortcut_window, 13, 13, 17, 4, 2))
	{
		menu->cursor_icon = RenderOutput_CreateFromResourceFar(
			2, 0, (struct RenderInput *)menu->shortcut_window, 0, result);
		menu->cursor_icon->active = 13;
	}
	while (!GameFlag_TestFar(0x150))
	{
		if (pending)
		{
			pending = 0;
			selection = (count + selection) % count;
			window = menu->status_window;
			owner = UnitGetFar(owner_ids[selection]);
			PsynergyMenu_RefreshOwnerPsynergy(owner_ids[selection]);
			Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
			PsynergyMenu_DrawShortcuts(menu->shortcut_window, owner_ids[selection]);
			PsynergyMenu_CallIconRoutineWithValue((s32)menu, owner_ids[selection]);
			for (i = 3; i >= 0; i--)
				menu->row_positions[i] = 0x1e;
			menu->row_positions[selection] = 0x1a;
			if (!GameFlag_TestFar(0x151) && !shown)
			{
				RenderOutput_ClearListFar((void *)menu->info_window);
#if EDITION_INTERNATIONAL
				RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
#endif
				UiText_DrawWorkValueWithLabel((s32)menu->info_window);
				shown = 1;
			}
			else
			{
				GameFlag_ClearBitFar(0x151);
			}
		}
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			if (menu->psynergy_count)
			{
				AudioCommand_PlayFar(112);
				result = owner_ids[selection];
				break;
			}
			AudioCommand_PlayFar(114);
		}
		if ((gKeyState & KEY_L) || (gKeyState & KEY_R))
		{
			result = owner_ids[selection];
			if (gKeyState & KEY_L)
				menu->mode = 1;
			else
				menu->mode = 2;
			actions = MemAlloc(64);
			found = PsynergyMenu_CollectActions((struct BtlUnit *)owner, actions, 1);
			MemFree(actions);
			action_count = found;
			if (action_count == 0)
			{
				menu->mode = action_count;
				AudioCommand_PlayFar(114);
			}
			else
			{
				AudioCommand_PlayFar(112);
				break;
			}
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			selection--;
			pending = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			selection++;
			pending = 1;
		}
	}
	menu->tab_index[0] = selection;
	menu->selected_owner = owner_ids[selection];
	menu->owner_ids[0] = owner_ids[selection];
	return result;
}

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
	u16						 *psynergies;
	struct PsynergyMenuState *menu;
	struct BtlUnit			 *owner;

	menu = gMenuWork;
	owner = UnitGetFar(owner_id);
	psynergies = menu->psynergies;
	menu->psynergy_count = PsynergyMenu_CollectActions(owner, psynergies, 2);
	RenderOutput_RedrawSavedRectFar((void *)menu->psynergy_window);
	PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
	PsynergyMenu_DrawPsynergyIcons(psynergies);
	if (menu->psynergy_count == 0)
	{
		UiText_DrawCharacterAtOffsetFar(
			(s32)MsgPsynergyMenuEmpty, (s32)menu->psynergy_window, 0, 0x18);
	}
}

s32 PsynergyMenu_ReturnTrue(void)
{
	return 1;
}

void PsynergyMenu_NoOp(void)
{
}

s32 PsynergyMenu_SelectTarget(s32 mode)
{

	struct PsynergyMenuState *menu;
	s32						  selection;
	s32						  count;
	s32						  pending;
	s32						  result;
	s32						  shown;
	struct RenderOutput		 *marker;

	menu = gMenuWork;
	selection = menu->tab_index[1];
	count = menu->owner_count;
	pending = 1;
	result = 0;
	shown = 0;
	UnitGetFar(menu->owner_table[menu->tab_index[0]]);
	UiMenu_SlideCursor(selection * 24 - 10, 16);
	while (!GameFlag_TestFar(0x150))
	{
		if (pending)
		{
			pending = 0;
			selection = (selection + count) % count;
			UnitGetFar(menu->owner_table[selection]);
			marker = menu->pane_icon[1];
			marker->oam.x = marker->x =
				((((struct RenderInput *)menu->auxiliary_window)->x + selection * 3) << 3) - 2;
			if (mode == 0)
			{
				Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_table[selection], 0, 0);
				PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[selection]);
				if (!GameFlag_TestFar(0x151) && !shown)
				{
#if EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
					UiText_DrawCharacterAtOffsetFar(
						(menu->pane_action[0] & OWNER_ACTION_ID_MASK) + (s32)MsgAbilityDescription,
						(s32)menu->info_window, 0, 0);
#else
					RenderOutput_ClearListFar((void *)menu->info_window);
					UiText_DrawResourceFar(
						(menu->pane_action[0] & OWNER_ACTION_ID_MASK) + (s32)MsgAbilityDescription,
						(s32)menu->info_window, 0, 0);
#endif
					shown = 1;
				}
				else
				{
					GameFlag_ClearBitFar(0x151);
				}
			}
		}
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(112);
			result = menu->owner_table[selection];
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			AudioCommand_PlayFar(111);
			selection--;
			pending = 1;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			AudioCommand_PlayFar(111);
			selection++;
			pending = 1;
		}
	}
	marker = menu->pane_icon[1];
	menu->tab_index[1] = selection;
	UiIcon_PrepareObject(marker);
	marker->active = 13;
	WaitFrames(1);
	menu->tab_index[1] = selection;
	menu->selected_owner = menu->owner_table[selection];
	menu->owner_ids[1] = menu->owner_table[selection];
	return result;
}

s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut)
{
	s32 id = psynergy & OWNER_ACTION_ID_MASK;
	s32 code = (owner << 10) | id;

	if (shortcut == 0)
	{
		gGameState.first_shortcut = code;
	}
	else
	{
		gGameState.second_shortcut = code;
	}
	return 1;
}

#if EDITION_INTERNATIONAL

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner)
{
	s32 left;
	s32 top;
	s32 width;
	s32 height;
	s32 wide;

	if (gGameState.first_shortcut != 0 && gGameState.second_shortcut != 0)
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutChangeHelp, window, 0, -8);
	else
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutLabel, window, 0, -8);

	UiText_GetResourceDimensionsFar(
		(gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 10)
		wide = 1;
	else
		wide = 0;
	if (gGameState.first_shortcut != 0)
	{
		UiWork_PushValueSlotFar(gGameState.first_shortcut & 0x3ff, 4);
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutNameL, window, 0, 0);
		if (wide == 0)
			UiText_DrawStringAtOffsetFar(
				UnitGetFar(gGameState.first_shortcut >> 10), window, 80, 0);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyL, window, 0, 0);
	}

	UiText_GetResourceDimensionsFar(
		(gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 10)
		wide = 1;
	else
		wide = 0;
	if (gGameState.second_shortcut != 0)
	{
		UiWork_PushValueSlotFar(gGameState.second_shortcut & 0x3ff, 4);
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutNameR, window, 0, 8);
		if (wide == 0)
			UiText_DrawStringAtOffsetFar(
				UnitGetFar(gGameState.second_shortcut >> 10), window, 80, 8);
		UiWork_SetParamNibbleFar(15);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyR, window, 0, 8);
	}
	return 1;
}
#else

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner)
{
	s32 left;
	s32 top;
	s32 width;
	s32 height;
	u16 wide;

	if (gGameState.first_shortcut != 0 && gGameState.second_shortcut != 0)
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutChangeHelp, window, 0, -8);
	else
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutLabel, window, 0, -8);

	UiText_GetResourceDimensionsFar(
		(gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 11)
		wide = 1;
	else
		wide = 0;
	if (wide)
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutLString, window, 0, 0);
	else
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutLColonString, window, 0, 0);
	if (gGameState.first_shortcut != 0)
	{
		if (wide)
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName, window, 8, 0);
		else
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName, window, 16, 0);
		UiText_DrawStringAtOffsetFar(UnitGetFar(gGameState.first_shortcut >> 10), window, 80, 0);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyL, window, 24, 0);
	}

	UiText_GetResourceDimensionsFar(
		(gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName, &left, &top, &width, &height);
	if (width < 0 || width > 11)
		wide = 1;
	else
		wide = 0;
	if (wide)
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutRString, window, 0, 8);
	else
		UiText_DrawStringInWindowFar((u8 *)Menu_ShortcutRColonString, window, 0, 8);
	if (gGameState.second_shortcut != 0)
	{
		if (wide)
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName, window, 8, 8);
		else
			UiText_DrawCharacterAtOffsetFar(
				(gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName, window, 16, 8);
		UiText_DrawStringAtOffsetFar(UnitGetFar(gGameState.second_shortcut >> 10), window, 80, 8);
		UiWork_SetParamNibbleFar(15);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyR, window, 24, 8);
	}
	return 1;
}
#endif

void PsynergyMenu_CreateEntryGrid(void)
{
	s32						  window;
	struct PsynergyMenuState *menu;
	struct RenderOutput		 *cursor;
	s32						  index;
	s32						  x;
	s32						  y;
	struct RenderOutput		**output;

	menu = gMenuWork;
	window = UiMenu_CreateCursor(menu);
	PsynergyMenu_InitializeEntryObjects((void *)window, 2, 2, 8, 0);

	window = UiWindow_CreateFar(0, 5, 30, 15, 2);
	menu->psynergy_window = window;
	menu->selected_column = 0;
	menu->selected_row = 0;
	menu->column_count = 8;
	menu->row_count = 2;

	cursor =
		(struct RenderOutput *)UiIcon_CreateWithResourceVariant((struct RenderInput *)window, 0, 4);
	cursor->active = 13;
	menu->entry_grid_cursor = cursor;
	SideObject_CreateFar(0, 0, 0, window, 0, 0);

	y = 8;
	index = 0;
	output = &menu->entry_icons[0];
	x = 96;
	do
	{
		*output++ =
			RenderOutput_CreateFromResourceFar(4, index, (struct RenderInput *)window, x, y);
		index++;
		x += 16;
	} while (index <= 7);

	index = 8;
	y = 24;
	output = &menu->entry_icons[8];
	x = 96;
	do
	{
		*output++ =
			RenderOutput_CreateFromResourceFar(4, index, (struct RenderInput *)window, x, y);
		index++;
		x += 16;
	} while (index <= 15);
}

void PsynergyMenu_CloseWindows(void)
{
	struct PsynergyMenuState *menu;

	menu = gMenuWork;
	Menu_ReleaseEntryObjects();
	UiWork_FinalizeFar((void *)menu->auxiliary_window, 1);
	UiWork_FinalizeFar((void *)menu->psynergy_window, 1);
	UiWork_FinalizeFar((void *)menu->message_window, 1);
}

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
	s32					  remaining;
	struct RenderOutput **icons;
	u16					 *p;
	s32					  psynergy_id;

	icons = ((struct PsynergyMenuState *)gMenuWork)->entry_icons;
	p = psynergies;
	remaining = 31;
	do
	{
		psynergy_id = *p++;
		if (psynergy_id != 0)
		{
			Resource_LoadByModeIntoSlotFar(4, psynergy_id, (u8)(*icons)->index, 0);
		}
		icons++;
		remaining--;
	} while (remaining >= 0);
	Menu_HideEmptyEntryIcons(psynergies);
}

s32 PsynergyMenu_CollectActions(struct BtlUnit *owner, u16 *actions, s32 mode)
{
	/* FAKEMATCH: the doubled zero store through the byte-offset cursor makes loop.c replace the counter with the pointer (274 bytes); a single indexed clear keeps the counter (266). */
	s32 n;
	s32 passes;
	s32 count;
	s32 i;
	s32 j;

	passes = (mode != 2) ? 4 : 3;
	for (n = 62; n >= 0; n -= 2)
	{
		u16 *q = (u16 *)((u8 *)actions + n);
		*q = 0;
		*q = 0;
	}
	count = 0;

	if (mode == 1)
	{
		for (i = 0; i < 32; i++)
		{
			if (owner->action_slots[i].encoded_action != 0)
			{
				if (BtlActGet(owner->action_slots[i].encoded_action & OWNER_ACTION_ID_MASK)->type_0c
					!= 0)
				{
					actions[count] = owner->action_slots[i].encoded_action;
					count++;
				}
			}
		}
	}
	else
	{
		for (j = 0; j < passes; j++)
		{
			for (i = 0; i < 32; i++)
			{
				if (owner->action_slots[i].encoded_action != 0)
				{
					struct BtlAct *ability =
						BtlActGet(owner->action_slots[i].encoded_action & OWNER_ACTION_ID_MASK);

					if (j == 0 && (ability->type_0c != 0 || (ability->tgt_flg & 0x40) != 0))
					{
						actions[count] = owner->action_slots[i].encoded_action;
						count++;
					}
					else if (j == 1)
					{
					}
					else if (j == 2)
					{
					}
					else if (j == 3 && ability->type_0c == 0 && (ability->tgt_flg & 0x40) == 0)
					{
						actions[count] = owner->action_slots[i].encoded_action;
						count++;
					}
				}
			}
		}
	}

	return count;
}

s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
	s32						  owner_state;
	struct PsynergyMenuState *menu = gMenuWork;
	s32						  entry_count;
	s32						  row;
	s32						  page;
	s32						  page_count;
	s32						  selected_index;
	s32						  owner;

	owner_state = (s32)UnitGetFar(menu->owner_ids[index]);
	entry_count = menu->psynergy_count;
	owner = menu->owner_ids[index];
	selected_index = menu->selected_index_by_owner[owner];
	if ((selected_index + 1) > entry_count)
	{
		selected_index = entry_count - 1;
	}
	page = selected_index / 5;
	row = selected_index % 5;
	page_count = entry_count / 5;
	if (entry_count % 5 != 0)
	{
		page_count++;
	}
	result->owner_state = owner_state;
	result->page = page;
	result->page_count = page_count;
	result->row = row;
	result->entry_count = entry_count;
	result->selected_index = selected_index;
	return 1;
}

#if EDITION_INTERNATIONAL
#define DETAIL_ROW_X 0
#define DETAIL_ROW_WIDTH 15
#else
#define DETAIL_ROW_X 1
#define DETAIL_ROW_WIDTH 14
#endif

s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state)
{
	struct PsynergyMenuState *menu;
	s32						  combined;
	s32						  row;

	menu = gMenuWork;
	combined = state->page * 5;
	combined += state->row;
	state->selected_index = combined;

	if (GameFlag_TestFar(0x151) == 0)
	{
#if EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar(menu->info_window);
#else
		RenderOutput_ClearListFar(menu->info_window);
#endif
		WaitFrames(1);

		combined = state->selected_index;
		if (menu->psynergies[combined] != 0)
		{
			s32 masked = (menu->psynergies[combined] & 0x1ff) + (s32)MsgAbilityDescription;
#if EDITION_INTERNATIONAL
			UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
			UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
		}
	}
	else
	{
		GameFlag_ClearBitFar(0x2ff);
	}

	row = 0;
	do
	{
		if (row == state->row)
		{
			Render_SetTilemapFlagRect(
				(void *)menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 14);
		}
		else
		{
			Render_SetTilemapFlagRect(
				(void *)menu->psynergy_window, DETAIL_ROW_X, row * 2 + 1, DETAIL_ROW_WIDTH, 1, 15);
		}
		row++;
	} while (row <= 4);

	WaitFrames(1);
	return 1;
}

s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state)
{
	u32						  first_entry;
	u32						  visible_count;
	u8						  row;
	s32						  cursor;
	struct BtlUnit			 *owner;
	struct BtlAct			 *ability;
	struct PsynergyMenuState *menu = gMenuWork;

	(void)unused;

	RenderOutput_RedrawSavedRectFar((void *)window);
	UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

	if (2 & menu->flags)
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutHelp, window, 0, 88);
	}
	else
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
	}

	first_entry = state->page * 5;
	visible_count = (u8)(state->entry_count - first_entry);
	if (visible_count > 5)
	{
		visible_count = 5;
	}

#if EDITION_INTERNATIONAL
	Menu_SetPageIcons(5, first_entry, window, 0x70, 0x22);
#else
	Menu_SetPageIcons(5, first_entry, window, 0x78, 0x22);
#endif
	Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, 0x60, 0);
#else
	UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, 0x48, 0);
#endif

	row = 0;
	if (visible_count > row)
	{
		cursor = first_entry;
		do
		{
			owner = UnitGetFar(menu->owner_ids[0]);
			ability = BtlActGet(OWNER_ACTION_ID_MASK & menu->psynergies[cursor]);

			if (ability->pp_cost > owner->ep)
			{
				UiWork_SetParamNibbleFar(2);
			}
			else if (PsynergyMenu_IsActionRestricted(
						 OWNER_ACTION_ID_MASK & menu->psynergies[cursor])
					 != 0)
			{
				UiWork_SetParamNibbleFar(4);
			}
			else
			{
				UiWork_SetParamNibbleFar(15);
			}

			UiText_DrawCharacterAtOffsetFar(
				(OWNER_ACTION_ID_MASK & menu->psynergies[cursor]) + (s32)MsgAbilityName,
#if EDITION_INTERNATIONAL
				window, 16, row * 16 + 8);
#else
				window, 32, row * 16 + 8);
#endif
			UiText_DrawNumberAtOffsetFar(ability->pp_cost, 2, window, 104, row * 16 + 8);
			UiWork_SetParamNibbleFar(15);

			row++;
			cursor++;
		} while (visible_count > row);
	}

	return 1;
}

s32 PsynergyMenu_RunList(s32 pane)
{
	struct PsynergyMenuState *menu;
	struct BtlAct			 *ability;
	struct RenderOutput		 *icon;
	s32						  window;
	s32						  changed;
	s32						  nav;
	s8						  mode;
	s32						  tab;
	u8						  i;
	s32						  result;
	s32						  redraw;
	s32						  done;
	struct BtlUnit			 *owner;
	s32						  prev;
	s32						  prompt;
	s32						  work[5];
	struct MenuResult		  state;

	menu = gMenuWork;
	result = 0;
	prev = 0;
	prompt = 0;
	menu->pane_icon[pane]->active = 13;
	UiWindow_UpdateOrCreate(&menu->list_window, 13, 3, 17, 14, 2);
	window = menu->list_window;
	done = 0;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		owner = UnitGetFar(menu->owner_ids[pane]);
		if (menu->mode != 0)
		{
			menu->psynergy_count = PsynergyMenu_CollectActions(owner, menu->psynergies, 1);
		}
		else
		{
			menu->psynergy_count = PsynergyMenu_CollectActions(owner, menu->psynergies, 2);
		}
		PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
		PsynergyMenu_BuildPageResult(&state, pane);
		redraw = 1;
		changed = 1;
		menu->pane_icon[pane]->active = 1;

		while (GameFlag_TestFar(0x150) == 0)
		{
#if EDITION_INTERNATIONAL
			UiMenu_PositionCursor(88, state.row * 16 + 36);
#else
			UiMenu_PositionCursor(98, state.row * 16 + 36);
#endif

			if (changed != 0)
			{
				changed = 0;
				if (menu->psynergies[prev] != 0)
				{
					icon = menu->entry_icons[prev];
					UiIcon_PrepareObject(icon);
				}
				if (redraw != 0)
				{
					redraw = 0;
					WaitFrames(1);
					PsynergyMenu_DrawActionPage(window, 0, &state);
				}
				PsynergyMenu_DrawDetailPage(window, work, &state);
				menu->pane_action[pane] = menu->psynergies[state.selected_index];
				menu->cursor_icon->active = 13;
				if (menu->psynergies[state.selected_index] != 0)
				{
					icon = menu->entry_icons[state.selected_index];
					icon->active = 9;
					icon->anim_step = 0;
					icon->sentinel = 250;
				}
				for (i = 0; i < menu->owner_count; i++)
				{
					AnimationObjects_SelectAnimationFar(menu->tab_objects[i], 1);
				}
			}

			WaitFrames(1);
			prev = state.selected_index;

			if ((gKeysHeld & KEY_SELECT) == 0)
			{
				nav = Menu_HandlePageInput(
					0, state.entry_count, LIST_PAGE_SIZE, &state.row, &state.page);
			}
			else
			{
				nav = -1;
			}
			if (nav == 1)
			{
				redraw = 1;
				changed = 1;
			}
			if (nav == 0)
			{
				changed = 1;
			}
			if (nav == -1)
			{
				changed = 0;
			}

			if (menu->mode == 0)
			{
				if ((gKeyState & KEY_SELECT) != 0 && prompt == 0)
				{
					ability =
						BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
					if (ability->type_0c == 0)
					{
						AudioCommand_PlayFar(114);
					}
					else
					{
						AudioCommand_PlayFar(174);
						prompt = 1;
						menu->flags |= 2;
						UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
						UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutHelp, window, 0, 88);
					}
				}
				if ((gKeysHeld & KEY_SELECT) == 0 && prompt == 1)
				{
					prompt = 0;
					menu->flags &= 0xfffd;
					UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
					UiText_DrawCharacterAtOffsetFar(
						(s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
				}
			}

			if ((gKeyState & KEY_A) != 0)
			{
				if (menu->mode != 0)
				{
					AudioCommand_PlayFar(130);
					result = menu->psynergies[state.selected_index];
					done = 1;
					break;
				}
				if (menu->psynergies[state.selected_index] != 0)
				{
					if (PsynergyMenu_IsActionRestricted(menu->psynergies[state.selected_index])
						!= 0)
					{
						AudioCommand_PlayFar(114);
						continue;
					}
					ability =
						BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
					if (ability->pp_cost > owner->ep)
					{
						AudioCommand_PlayFar(114);
					}
					else
					{
						AudioCommand_PlayFar(173);
						result = menu->psynergies[state.selected_index];
						done = 1;
						break;
					}
				}
			}

			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(113);
				result = -1;
				done = 1;
				break;
			}

			if (((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0)
				&& (gKeysHeld & KEY_SELECT) == 0)
			{
				mode = menu->mode != 0 ? 1 : 2;
				AudioCommand_PlayFar(111);
				menu->selected_index_by_owner[menu->owner_ids[pane]] = state.selected_index;
				tab = menu->tab_index[pane];
				do
				{
					if ((gKeysRepeat & KEY_R) != 0)
					{
						tab++;
					}
					else
					{
						tab--;
					}
					tab = (tab + menu->owner_count) % menu->owner_count;
					menu->selected_owner = menu->owner_table[tab];
					menu->owner_ids[0] = menu->owner_table[tab];
					menu->psynergy_count = PsynergyMenu_CollectActions(
						UnitGetFar(menu->owner_ids[0]), menu->psynergies, mode);
				} while (menu->psynergy_count == 0);
				menu->tab_index[pane] = tab;
				for (i = 0; i < 4; i++)
				{
					menu->row_positions[i] = 30;
				}
				menu->row_positions[tab] = 26;
				Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_table[tab], 0, 0);
				PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->owner_table[tab]);
				break;
			}

			if ((gKeyState & KEY_L) != 0 && (gKeysHeld & KEY_SELECT) != 0)
			{
				ability = BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
				if (ability->type_0c == 0)
				{
					AudioCommand_PlayFar(114);
				}
				else
				{
					AudioCommand_PlayFar(130);
					if (PsynergyMenu_SetShortcut(
							menu->owner_ids[pane], menu->psynergies[state.selected_index], 0)
						!= 0)
					{
						result = menu->psynergies[state.selected_index];
						menu->mode = 1;
						done = 1;
						break;
					}
				}
			}

			if ((gKeyState & KEY_R) != 0 && (gKeysHeld & KEY_SELECT) != 0)
			{
				ability = BtlActGet(menu->psynergies[state.selected_index] & OWNER_ACTION_ID_MASK);
				if (ability->type_0c == 0)
				{
					AudioCommand_PlayFar(114);
				}
				else
				{
					if (PsynergyMenu_SetShortcut(
							menu->owner_ids[pane], menu->psynergies[state.selected_index], 1)
						!= 0)
					{
						AudioCommand_PlayFar(130);
						result = menu->psynergies[state.selected_index];
						menu->mode = 2;
						done = 1;
						break;
					}
				}
			}
		}
	}

	menu->flags &= 0xfffd;
	UiIcon_PrepareObject(menu->entry_grid_cursor);
	menu->pane_row[pane] = state.selected_index;
	menu->selected_index_by_owner[menu->owner_ids[pane]] = state.selected_index;
	menu->pane_action[pane] = result;
	if (GameFlag_TestFar(0x150) != 0)
	{
		result = -1;
	}
	WaitFrames(1);
	return result;
}

s32 PsynergyMenu_IsActionRestricted(s32 no)
{
	struct BtlAct *action = BtlActGet((u32)(no << 18) >> 18);

	if (action->type_0c != 0 || (action->tgt_flg & 0xc0) == 0xc0)
		return 0;
	return 1;
}
