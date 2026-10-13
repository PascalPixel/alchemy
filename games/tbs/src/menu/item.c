#include "unit/menu/menu.h"
#include "unit/menu/item.h"
#include "unit/menu/psynergy.h"
#include "unit/menu/status.h"
#include "unit/menu/useitem.h"
#include "unit/menu/compare.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
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

// イベントの作業領域（struct EventWork）の別の見え方。0x180 と 0x19a は共有ヘッダーにまだ名前がない
struct AssetSelectionResult
{
	s32 unknown_000[0x60];
	u16 unknown_180;
	u8	unknown_182[0x18];
	u16 unknown_19a;
};

s32 RunAssetSelectionScreen(void)
{
#if EDITION_INTERNATIONAL
	void *backup;
#endif
	struct InventoryMenuState *screen;
	s32						   index;
	s32						   value;
	s32						   category;
	s32						   result;
#if EDITION_INTERNATIONAL
	s32 size;
#endif
	void **process;
	void **cache;

#if EDITION_INTERNATIONAL
	size = 0x2000;
	backup = Runtime_BumpAllocateAlternatePool(size);
#endif
	screen = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
	cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_MENU_CONTROL];
	((struct ObjectSystemWork *)cache[0])->suspended = 1;
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	WaitFrames(1);
	UiWindow_InitializeWork(0);
	screen->party_count = Party_ListActiveOwnersFar(screen->owner_ids);
	ItemMenu_Init(0, 3, 0, 7);
	Resource_LoadPairedBlocks();
	Palette_LightenBankHighlight(14);
	Link_DrawShiftedTilePairFar(0x06002500);
	screen->message_window = UiWindow_CreateFar(13, 0, 17, 3, 2);
#if EDITION_INTERNATIONAL
	Scheduler_EnableOverlayCallbacksWithFlags();
	Iwram_CopyWords(backup, (void *)0x06004000, size);
	Iwram_FillWords((void *)0x06004000, size, 0x33333333);
	UiWork_SetAltFlagAndClearTableFar(1);
#endif
	Menu_CancelSoundReset();
	result = ItemMenu_RunCommands(&category, &value, &index);
	Menu_EnsureCancelSound();
	if (result == 1)
	{
		struct AssetSelectionResult *selection = cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL];

		selection->unknown_180 = (category << 10) | (index & ITEM_ID_MASK);
		selection->unknown_19a = screen->selected_slots[0];
	}
	RenderOutput_ClearListFar(screen->status_window);
	process = &cache[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL];
	((struct UiRenderWork *)*process)->menu_busy = 1;
	ItemMenu_Close();
	UiWindow_DrawFrameFar(0, 0, 30, 20);
	Menu_ResetTwoResourceEntries();
	Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
	((struct ObjectSystemWork *)cache[0])->suspended = 0;
#if EDITION_INTERNATIONAL
	UiWindow_MarkVisibleTileAttributesFar();
	UiWork_SetAltFlagAndClearTableFar(0);
	Iwram_CopyWords((void *)0x06004000, backup, size);
	((struct UiRenderWork *)*process)->menu_busy = 0;
	MemFree(backup);
	WaitFrames(1);
	Scheduler_DisableOverlayCallbacksWithFlags();
#endif
	WaitFrames(1);
	UiWindow_EraseBorderRectFar(0, 0, 30, 20);
	((struct UiRenderWork *)*process)->menu_busy = 0;
	Event_ClearInvalidPackedValuesFar();
	return result;
}

#if EDITION_INTERNATIONAL
#define MODAL_SHORT_X 14
#define MODAL_REMOVED_X 14
#else
#define MODAL_SHORT_X 15
#define MODAL_REMOVED_X 17
#endif

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

#define IWRAM_COPY_WORDS ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
	/* FAKEMATCH: needed to match: the copy routine as IwramIrqMain plus its offset, passed first; direct calls share the 332-byte size in r5 and move the routine to r6, and through Iwram_CopyWords the routine is loaded before the source and size (2 instructions reordered). */
	return copy(dst, src, size);
}

enum ItemMenuStep
{
	ITEM_STEP_OWNER = 0,
	ITEM_STEP_ITEM = 1,
	ITEM_STEP_USE = 2,
	ITEM_STEP_EQUIP = 3,
	ITEM_STEP_GIVE = 4,
	ITEM_STEP_DROP = 5,
	ITEM_STEP_TARGET = 6,
	ITEM_STEP_TRADE = 7,
	ITEM_STEP_IDLE = 8,
	ITEM_STEP_COMMAND = 9,
	ITEM_STEP_INSPECT = 10,
	ITEM_STEP_REMOVE = 11,
	ITEM_STEP_QUANTITY = 12
};

s32 ItemMenu_RunCommands(s32 *owner_out, s32 *target_out, s32 *item_out)
{
	s32 done;
#if !EDITION_INTERNATIONAL
	s32 owner;
	s32 chosen;
#endif
	s32						   sel;
	s32						   ret;
	struct BtlUnit			  *source;
	struct BtlUnit			  *target;
	struct InventoryMenuState *menu;
	struct ItemDefinition	  *item;
	struct BtlUnit			  *source_copy;
	struct BtlUnit			  *target_copy;
	u32						   state;
	s32						   command;
	s32						   result;
	s32						   qty;
	s32						   amount;
	s32						   aborted;
	s32						   work;
	s32						   n;
	u8						   moved_source;
	u8						   moved_target;
	s8						   stack;

	done = 0;
#if !EDITION_INTERNATIONAL
	owner = 0;
	chosen = 0;
#endif
	sel = 0;
	ret = 0;
	state = ITEM_STEP_OWNER;
	menu = gMenuWork;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		switch (state)
		{
		case ITEM_STEP_OWNER:
			menu->selected_slots[0] = 0;
			ItemMenu_SetMsgWin3();
			ItemMenu_SetItemWin3();
			menu->selected_item_icon->active = 13;
			ItemMenu_DrawMsg(0, (s32)MsgWhoseItem);
#if EDITION_INTERNATIONAL
			RenderOutput_RedrawSavedRectFar(menu->info_window);
			UiText_DrawWorkValueWithLabel((s32)menu->info_window);
			command = ItemMenu_PrepOwner(0);
			if (command == -1)
			{
				sel = command = 0;
				ret = -1;
				done = 1;
			}
#else
			UiText_DrawWorkValueWithLabel((s32)menu->info_window);
			owner = ItemMenu_PrepOwner(0);
			if (owner == -1)
			{
				chosen = sel = owner = 0;
				ret = -1;
				done = 1;
			}
#endif
			RenderOutput_RedrawSavedRectFar(menu->info_window);
			ItemMenu_HideAllIcons();
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_ITEM:
			if (ItemMenu_Count(menu->pane_owner[0]) == 0)
			{
				state = ITEM_STEP_OWNER;
				break;
			}
			ItemMenu_SetMsgWin3();
			ItemMenu_SetItemWin3();
			menu->selected_item_icon->active = 13;
			menu->pane_icons[0]->active = 1;
			ItemMenu_DrawMsg(0, (s32)MsgWhichItem);
			sel = ItemMenu_RunList(0);
			state = ITEM_STEP_OWNER;
			if (sel == -1)
			{
				break;
			}
			menu->list_mode = 0xff;
			state = ITEM_STEP_COMMAND;
			break;

		case ITEM_STEP_COMMAND:
			command = ItemMenu_ChooseCmd();
			if (command == -1)
			{
				state = ITEM_STEP_ITEM;
				menu->completion_flag = 1;
			}
			if (command == ITEM_COMMAND_USE)
			{
				if (BattleFx_HasTriggerFar(menu->selected_items[0] & ITEM_ID_MASK) != 0)
				{
					done = 1;
#if EDITION_INTERNATIONAL
					*owner_out = menu->pane_owner[0];
					*target_out = command;
					*item_out = menu->selected_items[0] & ITEM_ID_MASK;
#else
					*owner_out = owner;
					*target_out = chosen;
					*item_out = sel;
#endif
					ret = 1;
					break;
				}
				result = Item_ClassifyUseMode(menu->pane_owner[0], menu->selected_items[0]);
				if (result == 1)
				{
					state = ITEM_STEP_USE;
				}
				if (result == 2)
				{
					ItemMenu_Use();
					RenderOutput_ClearListFar(menu->info_window);
					InventoryMenu_ShowModalMessage(
						menu->message_offset + (s32)MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
					menu->pane_icons[0]->active = 13;
					menu->item_count =
						ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
					ItemMenu_DrawIcons(menu->items, 0);
					state = ITEM_STEP_OWNER;
				}
				if (result == -1 || result == 0)
				{
					done = 1;
					*owner_out = menu->pane_owner[0];
					*target_out = menu->pane_owner[1];
					*item_out = menu->selected_items[0] & ITEM_ID_MASK;
					ret = 1;
				}
			}
			if (command == ITEM_COMMAND_EQUIP)
			{
				state = ITEM_STEP_EQUIP;
			}
			if (command == ITEM_COMMAND_GIVE)
			{
				state = ITEM_STEP_TARGET;
			}
			if (command == ITEM_COMMAND_DROP)
			{
				state = ITEM_STEP_DROP;
			}
			if (command == ITEM_COMMAND_REMOVE)
			{
				state = ITEM_STEP_REMOVE;
			}
			if (command == ITEM_COMMAND_INSPECT)
			{
				state = ITEM_STEP_INSPECT;
			}
			break;

		case ITEM_STEP_USE:
			ItemMenu_HideAllIcons();
			ItemMenu_SetMsgWin5();
			ItemMenu_SetItemWin5();
			RenderOutput_RedrawSavedRectFar(menu->message_window);
			ItemMenu_DrawItemHead();
			UiText_DrawCharacterAtOffsetFar((s32)MsgUseOnWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
			if (ItemMenu_SelectTarget(0) != -1)
			{
#else
			chosen = ItemMenu_SelectTarget(0);
			if (chosen != -1)
			{
#endif
				command = 0;
				if (ItemMenu_IsSpecial(menu->selected_items[0] & ITEM_ID_MASK) != 0)
				{
					command = 8;
				}
				result = ItemMenu_Use();
				Menu_DrawOwnerStatusPanel(
					(s32)menu->status_window, menu->pane_owner[1], 0, command);
				if (result != -1)
				{
					RenderOutput_ClearListFar(menu->info_window);
					InventoryMenu_ShowModalMessage(
						menu->message_offset + (s32)MsgItemUseResult, 0, -1);
#if !EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
					menu->pane_icons[0]->active = 13;
					ItemMenu_TryBreak();
					state = ITEM_STEP_ITEM;
				}
				menu->item_count =
					ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
				ItemMenu_DrawIcons(menu->items, 0);
				menu->completion_flag = 1;
			}
			else
			{
				state = ITEM_STEP_COMMAND;
			}
			break;

		case ITEM_STEP_TARGET:
			ItemMenu_SetMsgWin5();
			ItemMenu_SetItemWin5();
			RenderOutput_RedrawSavedRectFar(menu->message_window);
			ItemMenu_DrawItemHead();
			UiText_DrawCharacterAtOffsetFar((s32)MsgGiveToWhom, (s32)menu->message_window, 16, 16);
#if EDITION_INTERNATIONAL
			n = ItemMenu_SelectTarget(1);
			state = ITEM_STEP_GIVE;
			if (n == -1)
			{
#else
			chosen = ItemMenu_SelectTarget(1);
			state = ITEM_STEP_GIVE;
			if (chosen == -1)
			{
#endif
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
				state = ITEM_STEP_COMMAND;
			}
			break;

		case ITEM_STEP_DROP:
			ItemMenu_HideAllIcons();
			item = Item_GetDirectFar(menu->selected_items[0] & ITEM_ID_MASK);
			result = 0;
			if ((item->flags & ITEM_STACKABLE) != 0)
			{
				qty = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if (qty > 1)
				{
					ItemMenu_DrawItemHead();
					result = ItemMenu_SelectGiveQuantity(0, qty, 1);
				}
			}
			if (result == -1)
			{
				state = ITEM_STEP_COMMAND;
				break;
			}
			menu->pane_owner[1] = 0;
			Resource_LoadByModeIntoSlotFar(2,
				(menu->selected_items[0] & ITEM_ID_MASK) | (result << INVENTORY_QUANTITY_SHIFT),
				(u8)menu->selected_item_icon->index, 0);
			menu->selected_item_icon->active = 1;
			menu->selected_item_icon->x = 120;
			menu->selected_item_icon->y = 28;
			UiIcon_PrepareObject(menu->selected_item_icon);
			UiWindow_ClearInteriorTilesFar((void *)menu->list_window, 0, 72, 120, 96);
			RenderOutput_RedrawSavedRectFar(menu->message_window);
			if (ItemMenu_ConfirmDrop(sel) == 0)
			{
#if EDITION_INTERNATIONAL
				command = menu->pane_owner[0];
				UnitGetFar(command);
				for (work = 0; work < result + 1; work++)
				{
					Inventory_RemoveFar(command, menu->selected_slots[0]);
					Item_AdjustCounterFar(menu->selected_items[0] & ITEM_ID_MASK, 1);
				}
				UnitCalcFar(command);
#else
				owner = menu->pane_owner[0];
				UnitGetFar(owner);
				for (work = 0; work < result + 1; work++)
				{
					Inventory_RemoveFar(owner, menu->selected_slots[0]);
					Item_AdjustCounterFar(menu->selected_items[0] & ITEM_ID_MASK, 1);
				}
				UnitCalcFar(owner);
#endif
				ItemMenu_SetItemWin3();
				ItemMenu_RefreshOwner(menu->pane_owner[0], 0);
				menu->selected_item_icon->active = 13;
				menu->pane_icons[0]->active = 13;
				WaitFrames(1);
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage((s32)MsgDroppedIt, MODAL_SHORT_X, 13);
				menu->completion_flag = 1;
				state = ITEM_STEP_ITEM;
			}
			else
			{
				state = ITEM_STEP_COMMAND;
			}
			UnitCalcFar(menu->pane_owner[0]);
			menu->selected_item_icon->active = 13;
			Event_ClearInvalidPackedValuesFar();
			break;

		case ITEM_STEP_GIVE:
			aborted = 0;
			item = Item_GetDirectFar(menu->selected_items[0] & ITEM_ID_MASK);
			if ((item->flags & ITEM_STACKABLE) != 0)
			{
				qty = InventoryMenu_GetItemQuantity(
					menu->pane_owner[1], menu->selected_items[0] & ITEM_ID_MASK);
				if (qty == 30)
				{
					aborted = 1;
				}
				if (ItemMenu_Count(menu->pane_owner[1]) == 15 && qty == 0)
				{
					state = ITEM_STEP_TRADE;
					break;
				}
				stack = (menu->selected_items[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
				if (aborted == 0)
				{
					if (qty + stack > 30)
					{
						amount = 30 - qty;
					}
					else
					{
						amount = stack;
					}
					if (stack > 1)
					{
						result = ItemMenu_SelectGiveQuantity(0, amount, 0);
					}
					else
					{
						result = 0;
					}
					if (result == -1)
					{
						state = ITEM_STEP_TARGET;
						break;
					}
					for (command = 0; command < result + 1; command++)
					{
						qty = Inventory_AddItemFar(
							menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
						if (qty != -1)
						{
							Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
							menu->selected_slots[1] = qty;
						}
						else
						{
							aborted = 1;
						}
					}
				}
			}
			else
			{
				result = Inventory_AddItemFar(menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
				if (result == -1)
				{
					state = ITEM_STEP_TRADE;
					break;
				}
				menu->selected_slots[1] = result;
				result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
				if (result == -1)
				{
					aborted = 1;
				}
			}
			UnitCalcFar(menu->pane_owner[0]);
			UnitCalcFar(menu->pane_owner[1]);
			Owner_RefreshClassActionsFar(menu->pane_owner[0]);
			Owner_RefreshClassActionsFar(menu->pane_owner[1]);
			result = 1;
			if (aborted == 0)
			{
				menu->pane_owner[0] = menu->pane_owner[1];
				menu->selected_items[0] &= ITEM_ID_MASK;
#if EDITION_INTERNATIONAL
				ItemMenu_SetMsgWin6();
#endif
				RenderOutput_RedrawSavedRectFar(menu->message_window);
				ItemMenu_DrawItemHead();
				result = ItemMenu_ConfirmEquip(0);
			}
			if (GameFlag_TestFar(0x150) != 0)
			{
				break;
			}
			ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
			menu->pane_icons[0]->active = 13;
			WaitFrames(1);
			if (aborted == 1)
			{
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage((s32)MsgCannotHoldMore, 15, 14);
			}
			else
			{
				RenderOutput_ClearListFar(menu->info_window);
				if (result == 1)
				{
					InventoryMenu_ShowModalMessage((s32)MsgGiven, 15, 14);
				}
				else
				{
					ItemMenu_DrawEquipPreview(
						menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
					InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 14);
					item = Item_GetDirectFar(menu->selected_items[0]);
					if ((item->flags & ITEM_CURSED) != 0)
					{
						AudioCommand_PlayFar(0x67);
						RenderOutput_ClearListFar(menu->info_window);
						InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7, MODAL_SHORT_X, 14);
					}
				}
			}
			Event_ClearInvalidPackedValuesFar();
			state = ITEM_STEP_OWNER;
			break;

		case ITEM_STEP_TRADE:
			aborted = 0;
			ItemMenu_SetMsgWin3();
			ItemMenu_SetItemWin3();
			ItemMenu_DrawMsg(0, (s32)MsgSwapForWhat);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			if (ItemMenu_RunList(1) == -1)
			{
#else
			sel = ItemMenu_RunList(1);
			if (sel == -1)
			{
#endif
				state = ITEM_STEP_TARGET;
				break;
			}
			source = UnitGetFar(menu->pane_owner[0]);
			target = UnitGetFar(menu->pane_owner[1]);
			source_copy = MemAlloc(BATTLE_UNIT_SIZE);
			target_copy = MemAlloc(BATTLE_UNIT_SIZE);
			CopyWords(IWRAM_COPY_WORDS, source_copy, source, BATTLE_UNIT_SIZE);
			CopyWords(IWRAM_COPY_WORDS, target_copy, target, BATTLE_UNIT_SIZE);
			moved_source = 0;
			while (moved_source <= 29)
			{
				result = Inventory_RemoveFar(menu->pane_owner[0], menu->selected_slots[0]);
				if (result == 2)
				{
					break;
				}
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				moved_source++;
			}
			moved_source++;
			moved_target = 0;
			while (moved_target <= 29)
			{
				if ((menu->selected_items[1] & INVENTORY_EQUIPPED) != 0)
				{
					item = Item_GetDirectFar(menu->selected_items[1] & ITEM_ID_MASK);
					if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0)
					{
						aborted = 1;
					}
				}
				result = Inventory_RemoveFar(menu->pane_owner[1], menu->selected_slots[1]);
				if (result == 2)
				{
					break;
				}
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				moved_target++;
			}
			moved_target++;
			while (moved_source != 0)
			{
				result = Inventory_AddItemFar(menu->pane_owner[1], menu->selected_items[0] & 0x5ff);
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				menu->selected_slots[1] = result;
				moved_source--;
			}
			while (moved_target != 0)
			{
				result = Inventory_AddItemFar(menu->pane_owner[0], menu->selected_items[1] & 0x5ff);
				if (result == -1)
				{
					aborted = 1;
					break;
				}
				menu->selected_slots[0] = result;
				moved_target--;
			}
			WaitFrames(1);
			if (aborted == 1)
			{
				CopyWords(IWRAM_COPY_WORDS, source, source_copy, BATTLE_UNIT_SIZE);
				CopyWords(IWRAM_COPY_WORDS, target, target_copy, BATTLE_UNIT_SIZE);
				RenderOutput_ClearListFar(menu->info_window);
#if defined(TBS_EDITION_FR)
				InventoryMenu_ShowModalMessage((s32)MsgCannotTrade, 11, 14);
				RenderOutput_RedrawSavedRectFar(menu->status_window);
#else
				InventoryMenu_ShowModalMessage((s32)MsgCannotTrade, 15, 14);
#endif
			}
			else
			{
				UnitCalcFar(menu->pane_owner[0]);
				UnitCalcFar(menu->pane_owner[1]);
				Owner_RefreshClassActionsFar(menu->pane_owner[0]);
				Owner_RefreshClassActionsFar(menu->pane_owner[1]);
				ItemMenu_SetMsgWin5();
#if EDITION_INTERNATIONAL
				ItemMenu_SetMsgWin6();
				ItemMenu_HidePageIcons();
#endif
				RenderOutput_RedrawSavedRectFar(menu->message_window);
				menu->pane_owner[0] = menu->pane_owner[1];
				menu->selected_items[0] &= ITEM_ID_MASK;
				ItemMenu_DrawItemHead();
				result = ItemMenu_ConfirmEquip(0);
				if (GameFlag_TestFar(0x150) == 0)
				{
					RenderOutput_ClearListFar(menu->info_window);
					ItemMenu_SetItemWin5();
					ItemMenu_RefreshOwner(menu->pane_owner[1], 1);
					if (result == 0)
					{
						ItemMenu_DrawEquipPreview(
							menu->pane_owner[1], menu->selected_slots[1], 0, menu->pane_owner[1]);
						InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 14);
						item = Item_GetDirectFar(menu->selected_items[0]);
						if ((item->flags & ITEM_CURSED) != 0)
						{
							AudioCommand_PlayFar(0x67);
							RenderOutput_ClearListFar(menu->info_window);
							InventoryMenu_ShowModalMessage(
								(s32)MsgEquippedIt + 7, MODAL_SHORT_X, 14);
						}
					}
					else
					{
#if defined(TBS_EDITION_IT)
						InventoryMenu_ShowModalMessage((s32)MsgTraded, 13, 14);
#else
						InventoryMenu_ShowModalMessage((s32)MsgTraded, 15, 14);
#endif
					}
				}
			}
			MemFree(target_copy);
			MemFree(source_copy);
			Event_ClearInvalidPackedValuesFar();
			state = ITEM_STEP_OWNER;
			break;

		case ITEM_STEP_EQUIP:
			result = Inventory_EquipFar(menu->pane_owner[0], menu->selected_slots[0]);
			if (result == -1)
			{
				state = ITEM_STEP_ITEM;
				break;
			}
			if (result == -2)
			{
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage((s32)MsgCannotRemoveItem, 0, -1);
#if !EDITION_INTERNATIONAL
				RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
				state = ITEM_STEP_ITEM;
				break;
			}
			UnitCalcFar(menu->pane_owner[0]);
			Owner_RefreshClassActionsFar(menu->pane_owner[0]);
			menu->pane_icons[0]->active = 13;
			menu->item_count = ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
			ItemMenu_DrawIcons(menu->items, 0);
			WaitFrames(1);
			ItemMenu_DrawEquipPreview(
				menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			RenderOutput_ClearListFar(menu->info_window);
			InventoryMenu_ShowModalMessage((s32)MsgEquippedIt, 15, 8);
			item = Item_GetDirectFar(menu->selected_items[0]);
			if ((item->flags & ITEM_CURSED) != 0)
			{
				AudioCommand_PlayFar(0x67);
				RenderOutput_ClearListFar(menu->info_window);
				InventoryMenu_ShowModalMessage((s32)MsgEquippedIt + 7, MODAL_SHORT_X, 8);
			}
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_REMOVE:
			UnitGetFar(menu->pane_owner[0])->inventory[menu->selected_slots[0]] &=
				~INVENTORY_EQUIPPED;
			UnitCalcFar(menu->pane_owner[0]);
			Owner_RefreshClassActionsFar(menu->pane_owner[0]);
			menu->pane_icons[0]->active = 13;
			menu->item_count = ItemMenu_Collect(UnitGetFar(menu->pane_owner[0]), menu->items, 0);
			ItemMenu_DrawIcons(menu->items, 0);
			WaitFrames(1);
			menu->equip_preview = 1;
			ItemMenu_DrawEquipPreview(
				menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			menu->equip_preview = 0;
			RenderOutput_ClearListFar(menu->info_window);
			InventoryMenu_ShowModalMessage((s32)MsgRemoved, MODAL_REMOVED_X, 8);
			Event_ClearInvalidPackedValuesFar();
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_INSPECT:
			menu->pane_icons[0]->active = 13;
			Menu_SelectQuantity(menu->selected_items[0]);
			RenderOutput_RedrawSavedRectFar(menu->status_window);
			ItemMenu_DrawEquipPreview(
				menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			menu->pane_icons[0]->active = 1;
			state = ITEM_STEP_COMMAND;
			break;

		case ITEM_STEP_QUANTITY:
			result = ItemMenu_SelectGiveQuantity(0, 30, 0);
			if (result != -1)
			{
				qty = result;
			}
			state = ITEM_STEP_ITEM;
			break;

		case ITEM_STEP_IDLE:
			break;

		default:
			done = 1;
			break;
		}
	}

	if (GameFlag_TestFar(0x150) != 0)
	{
		ret = -1;
	}
	return ret;
}

s32 ItemMenu_Use(void)
{
	struct InventoryMenuState *menu;
	s32						   result;

	menu = gMenuWork;
	result = Item_Use(menu->selected_slots[0], menu->pane_owner[0], menu->pane_owner[1]);

	if (result == -1)
	{
		AudioCommand_PlayFar(SOUND_MENU_ERROR);
		RenderOutput_ClearListFar(menu->info_window);
		InventoryMenu_ShowModalMessage(
			menu->message_offset + (s32)MsgItemUseResult, result, result);
#if !EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
		menu->completion_flag = 1;
		return result;
	}

	Item_PlayUseAnimation(menu->selected_items[0] & ITEM_ID_MASK);
	UnitCalcFar(menu->pane_owner[0]);
	UnitCalcFar(menu->pane_owner[1]);
	return 1;
}

void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   index;

	PsynergyMenu_InitializeEntryObjects((void *)UiMenu_CreateCursor(menu), 2, 2, 8, 0);
	for (index = 3; index >= 0; index--)
		menu->owner_y[index] = 30;

	{
		u8	zero = 0;
		s32 style;
		menu->help_window = NULL;
		menu->status_window = NULL;
		style = 2;
		menu->info_window = UiWindow_CreateFar(0, 17, 30, 3, style);
		menu->item_window = NULL;
		menu->selected_column = zero;
		menu->selected_row = zero;
		menu->column_count = 8;
		menu->row_count = style;
	}
}

void Menu_SpawnIconEntries(struct InventoryMenuState *state, s32 arg1)
{
	struct RenderOutput **output0;
	s32					  index0;
	s32					  fifth0;
	struct RenderOutput **output1;
	s32					  index1;
	s32					  fifth1;
	struct RenderOutput **output2;
	s32					  index2;
	s32					  fifth2;

	index0 = 0;
	fifth0 = 0xA8;
	output0 = &state->entry_icons[0];
	do
	{
		*output0++ =
			RenderOutput_CreateFromResourceFar(2, index0, (struct RenderInput *)arg1, 0xF8, fifth0);
		index0++;
	} while (index0 <= 7);

	index1 = 8;
	fifth1 = 0xA8;
	output1 = &state->entry_icons[8];
	do
	{
		*output1++ = RenderOutput_CreateFromResourceFar(
			2, index1, (struct RenderInput *)arg1, 0x100, fifth1);
		index1++;
	} while (index1 <= 15);

	index2 = 16;
	fifth2 = 0xA8;
	output2 = &state->entry_icons[16];
	do
	{
		*output2++ = RenderOutput_CreateFromResourceFar(
			2, index2, (struct RenderInput *)arg1, 0x100, fifth2);
		index2++;
	} while (index2 <= 31);
}

void ItemMenu_HideAllIcons(void)
{
	s32					  hidden_state = 13;
	struct RenderOutput **icons = ((struct InventoryMenuState *)gMenuWork)->entry_icons;
	s32					  slot;

	for (slot = 31; slot >= 0; slot--)
	{
		struct RenderOutput *icon = *icons++;
		if (icon != 0)
		{
			icon->active = hidden_state;
		}
	}
}

#if EDITION_INTERNATIONAL
void ItemMenu_HidePageIcons(void)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   slot = 0;
	s32						   hidden_state = 13;
	struct RenderOutput		 **icon_slot = menu->entry_icons;

	do
	{
		struct RenderOutput *icon = *icon_slot++;

		if (icon != 0 && slot % 5 == 0)
		{
			icon->active = hidden_state;
		}
		slot++;
	} while (slot <= 31);
}
#endif

void ItemMenu_Close(void)
{
	struct InventoryMenuState *menu;
	struct RenderOutput		  *cursor;

	menu = gMenuWork;
	Menu_ReleaseEntryObjects();
	ItemMenu_HideAllIcons();
	WaitFrames(1);
	cursor = menu->cursor;
	cursor->active = 0xD;
	UiWindow_CloseIfOpen(&menu->main_window, 1);
	UiWindow_CloseIfOpen(&menu->item_window, 1);
	UiWindow_CloseIfOpen(&menu->message_window, 1);
	UiWindow_CloseIfOpen(&menu->status_window, 1);
	UiWindow_CloseIfOpen(&menu->help_window, 1);
	UiWindow_CloseIfOpen(&menu->info_window, 1);
	UiWindow_CloseIfOpen(&menu->equip_window, 1);
	UiWindow_CloseIfOpen(&menu->list_window, 1);
	UiWindow_CloseIfOpen(&menu->unknown_window_38, 1);
	UiWindow_CloseIfOpen(&menu->modal_window, 1);
	UiWindow_CloseIfOpen(&menu->unknown_window_40, 1);
}

s32 ItemMenu_PrepOwner(s32 pane)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   result = 0;
	s32						   index;
	struct BtlUnit			  *owner;

	index = menu->pane_index[pane];
	RenderOutput_ClearListFar(menu->info_window);
	menu->pane_count[pane] = menu->party_count;
	if (index == -1)
	{
		menu->pane_index[pane] = result;
		index = 0;
	}
	else
	{
		UiMenu_SlideCursor(index * 24 - 10, 16);
	}
	owner = UnitGetFar(menu->owner_ids[index]);
	menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
	result = ItemMenu_RunOwnerSelection(menu->owner_ids, menu->items);
	UiIcon_PrepareObject(menu->pane_icons[pane]);
	WaitFrames(1);
	return result;
}

s32 ItemMenu_RunOwnerSelection(u16 *owner_ids, u16 *items)
{
	struct InventoryMenuState *menu;
	s32						   selection;
	s32						   count;
	s32						   result;
	struct BtlUnit			  *owner;
	u8						   sort_mode;
	s32						   by_category;
	s32						   pending;
	s32						   window;
	s32						   i;

	menu = gMenuWork;
	selection = menu->pane_index[0];
	count = menu->pane_count[0];
	pending = 1;
	result = 0;
	sort_mode = 0;
	by_category = 0;
	owner = UnitGetFar(owner_ids[selection]);
	if (UiWindow_UpdateOrCreate((s32 *)&menu->item_window, 13, 3, 17, 10, 2))
		Menu_SpawnIconEntries(menu, (s32)menu->item_window);
	if (UiWindow_UpdateOrCreate((s32 *)&menu->help_window, 13, 13, 17, 4, 2))
	{
		menu->selected_item_icon = RenderOutput_CreateFromResourceFar(
			2, 0, (struct RenderInput *)menu->help_window, 0, result);
		menu->selected_item_icon->active = 13;
	}
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, (s32)menu->help_window, 0, 0);
	UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1, (s32)menu->help_window, 0, 8);
#else
	UiText_DrawStringInWindowFar(ItemMenu_ArrangeKeysString, (s32)menu->help_window, 0, 0);
	UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, (s32)menu->help_window, 32, 0);
	UiText_DrawStringInWindowFar(ItemMenu_EquipmentKeyString, (s32)menu->help_window, 16, 8);
	UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1, (s32)menu->help_window, 32, 8);
#endif
	menu->pane_icons[0]->active = pending;
	while (!GameFlag_TestFar(0x150))
	{
		selection = (selection + count) % count;
		UiMenu_PositionCursor(selection * 24 - 10, 16);
		if (pending)
		{
			sort_mode = 0;
			pending = 0;
			window = (s32)menu->status_window;
			owner = UnitGetFar(owner_ids[selection]);
			if (by_category)
			{
				menu->item_count =
					ItemMenu_Collect(UnitGetFar(owner_ids[selection]), menu->items, 0);
				ItemMenu_OpenCategory(owner_ids[selection]);
				Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 8);
			}
			else
			{
				ItemMenu_RefreshOwner(owner_ids[selection], 0);
				Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
			}
			for (i = 3; i >= 0; i--)
				menu->owner_y[i] = 30;
			menu->owner_y[selection] = 26;
		}
		WaitFrames(1);
		if (gKeyState & KEY_A)
		{
			if (gKeysHeld & KEY_L)
			{
				sort_mode = (sort_mode + 4) % 4;
				InventoryMenu_SortByListOrder(owner->inventory, sort_mode);
				sort_mode++;
				ItemMenu_RefreshOwner(owner_ids[selection], 0);
				AudioCommand_PlayFar(112);
			}
			else if (ItemMenu_Count(owner_ids[selection]))
			{
				AudioCommand_PlayFar(112);
				result = owner_ids[selection];
				break;
			}
			else
			{
				AudioCommand_PlayFar(114);
			}
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			break;
		}
		if (gKeyState & KEY_R)
		{
			by_category = 1;
			pending = 1;
		}
		if (!(gKeysHeld & KEY_R) && by_category == 1)
		{
			by_category = 0;
			pending = 1;
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
	menu->pane_index[0] = selection;
	menu->selected_owner = owner_ids[selection];
	menu->pane_owner[0] = owner_ids[selection];
	return result;
}

void ItemMenu_OpenCategory(s32 owner_id)
{
	struct InventoryMenuState *menu;

	menu = gMenuWork;
	ItemMenu_PosCategory();
	RenderOutput_RedrawSavedRectFar(menu->item_window);
	ItemMenu_DrawCategory((s32)menu->item_window, owner_id, 0);
}

s32 ItemMenu_SelectTarget(s32 mode)
{

	struct InventoryMenuState *menu;
	s32						   window;
	s32						   count;
	s32						   selection;
	s32						   pending;
	u8						   result;
	s32						   shown;
	s32						   quantity;
	struct RenderOutput		  *marker;

	menu = gMenuWork;
	window = (s32)menu->item_window;
	selection = menu->pane_index[1];
	count = menu->party_count;
	pending = 1;
	result = 0;
	shown = 0;
	UiWindow_SetBounds((struct RenderInput *)window, 13, 5, 17, 12);
	RenderOutput_RedrawSavedRectFar((void *)menu->item_window);
	UnitGetFar(menu->owner_ids[menu->pane_index[0]]);
	Scheduler_AddOrUpdateCallback(EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
	while (!GameFlag_TestFar(0x150))
	{
		if (pending)
		{
			pending = 0;
			selection = (selection + count) % count;
			window = (s32)menu->item_window;
			UnitGetFar(menu->owner_ids[selection]);
			marker = menu->pane_icons[1];
			marker->oam.x = marker->x = ((menu->main_window->x + selection * 3) << 3) - 2;
			if (mode == 1)
			{
				ItemMenu_RefreshOwner(menu->owner_ids[selection], 1);
				UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
				UiWindow_ClearInteriorTilesFar((void *)window, 0, 72, 120, 80);
				if (selection != menu->pane_index[0])
				{
					quantity = InventoryMenu_GetItemQuantity(
						menu->owner_ids[selection], menu->selected_items[0] & ITEM_ID_MASK);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
					if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
						UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 0, 72);
					else if (quantity != 0)
					{
						UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
						UiText_DrawCharacterAtOffsetFar((s32)MsgInStock, window, STOCK_LABEL_X, 72);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar((s32)MsgNoneInStock, window, 16, 72);
					}
#else
					if (quantity != 0)
					{
						UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
						UiText_DrawCharacterAtOffsetFar((s32)MsgInStock, window, STOCK_LABEL_X, 72);
					}
					else
					{
						UiText_DrawCharacterAtOffsetFar((s32)MsgNoneInStock, window, 16, 72);
					}
					if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
#if EDITION_INTERNATIONAL
						UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 0, 72);
#else
						UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 16, 72);
#endif
#endif
				}
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->owner_ids[selection]);
			}
			if (mode == 0)
			{
				if (ItemMenu_IsSpecial(menu->selected_items[0] & ITEM_ID_MASK))
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection],
						menu->selected_slots[0], 8);
				else
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection],
						menu->selected_slots[0], 0);
				if (!GameFlag_TestFar(0x151) && !shown)
				{
#if EDITION_INTERNATIONAL
					RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
					UiText_DrawCharacterAtOffsetFar(
						(menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
						(s32)menu->info_window, 0, 0);
#else
					RenderOutput_ClearListFar((void *)menu->info_window);
					UiText_DrawResourceFar(
						(menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
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
			if (mode == 1 && selection == menu->pane_index[0])
			{
				AudioCommand_PlayFar(114);
				continue;
			}
			AudioCommand_PlayFar(112);
			result = menu->owner_ids[selection];
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			result = 255;
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
			pending = 1;
			selection++;
		}
	}
	marker = menu->pane_icons[1];
	menu->pane_index[1] = selection;
	UiIcon_PrepareObject(marker);
	marker->active = 13;
	EquipmentMenu_StartCompatibilityIndicators();
	WaitFrames(1);
	menu->pane_index[1] = selection;
	menu->selected_owner = menu->owner_ids[selection];
	menu->pane_owner[1] = menu->owner_ids[selection];
	return (s8)result;
}

void EquipmentMenu_UpdateCompatibilityIndicators(void)
{
	struct InventoryMenuState *menu;
	s8						   member_index;

	menu = gMenuWork;
	if ((gFrameCount & 31) == 0 && menu->party_count != 0)
	{
		member_index = 0;
		do
		{
			if (Item_CanOwnerEquip(
					menu->owner_ids[member_index], menu->selected_items[0] & ITEM_ID_MASK)
				!= 0)
			{
				struct AnimationObject *indicator = menu->owner_objects[member_index];
				AnimationObjects_SelectAnimationFar(indicator, 3);
			}
			else
			{
				struct AnimationObject *indicator = menu->owner_objects[member_index];
				AnimationObjects_SelectAnimationFar(indicator, 1);
			}
			member_index++;
		} while (member_index < menu->party_count);
	}
}

void EquipmentMenu_StartCompatibilityIndicators(void)
{
	struct InventoryMenuState *menu;
	s8						   member_index;

	menu = gMenuWork;
	if (menu->party_count != 0)
	{
		member_index = 0;
		do
		{
			struct AnimationObject *indicator = menu->owner_objects[member_index];
			AnimationObjects_SelectAnimationFar(indicator, 1);
			member_index++;
		} while (member_index < menu->party_count);
	}
	Scheduler_RemoveCallback(&EquipmentMenu_UpdateCompatibilityIndicators);
}

s32 ItemMenu_IsSpecial(s32 item_id)
{
	switch (item_id)
	{
	case 0xC1:
	case 0xC2:
	case 0xC3:
	case 0xC4:
		return 1;
	}
	return 0;
}

void ItemMenu_DrawMsg(s32 unused, s32 message)
{
	struct InventoryMenuState *menu;

	menu = gMenuWork;
	RenderOutput_RedrawSavedRectFar(menu->message_window);
	UiText_DrawCharacterAtOffsetFar(message, (s32)menu->message_window, 0, 0);
}

void Menu_HideEmptyEntryIcons(const u16 *items)
{
	struct InventoryMenuState *menu = gMenuWork;
	s32						   slot;

	for (slot = 0; slot < 32; slot++)
	{
		if (items[slot] == 0)
		{
			UiIcon_PrepareObject(menu->entry_icons[slot]);
			menu->entry_icons[slot]->active = 13;
		}
	}
}

s32 ItemMenu_Count(s32 owner_id)
{
	s32	 item_id;
	s32	 remaining;
	s32	 count;
	u16 *slots;

	count = 0;
	slots = UnitGetFar(owner_id)->inventory;
	remaining = 0xE;
	do
	{
		item_id = ITEM_ID_MASK & *slots;
		slots += 1;
		if (item_id != 0)
		{
			count += 1;
		}
		remaining -= 1;
	} while (remaining >= 0);
	return count;
}

s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item)
{
	struct BtlUnit *state;
	s32				i;
	s32				quantity;

	quantity = 0;
	state = UnitGetFar(owner);
	i = 0;
	do
	{
		if (state->inventory[i] != 0 && (state->inventory[i] & ITEM_ID_MASK) == item)
		{
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
			quantity += (state->inventory[i] & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
			quantity++;
#else
			quantity = (state->inventory[i] & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
			quantity++;
			break;
#endif
		}
		i++;
	} while (i <= 14);
	return quantity;
}

s32 ItemMenu_Collect(struct BtlUnit *owner, u16 *items, s32 mode)
{
	s32 count;
	s32 i;

	for (i = 0; i < 32; i++)
	{
		items[i] = 0;
	}
	count = 0;
	for (i = 0; i < 15; i++)
	{
		items[i] = 0;
		if (owner->inventory[i] != 0)
		{
			items[count] = owner->inventory[i];
			count++;
		}
	}
	return count;
}

void ItemMenu_DrawIcons(u16 *items, s32 style)
{
	s32					  remaining;
	u16					 *entries;
	struct RenderOutput **icons;
	s32					  item_id;

	icons = ((struct InventoryMenuState *)gMenuWork)->entry_icons;
	entries = items;
	remaining = 14;
	do
	{
		item_id = *entries++;
		if (item_id != 0)
		{
			if (style == 0)
			{
				Resource_LoadByModeIntoSlotFar(2, item_id, (u8)(*icons)->index, 0);
			}
			else
			{
				Resource_LoadByModeIntoSlotFar(7, item_id, (u8)(*icons)->index, 0);
			}
		}
		icons++;
		remaining--;
	} while (remaining >= 0);
	Menu_HideEmptyEntryIcons(items);
}

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
	struct InventoryMenuState *menu;
	struct BtlUnit			  *owner;
	u16						  *items;

	menu = gMenuWork;
	owner = UnitGetFar(owner_id);
	items = menu->items;
	menu->item_count = ItemMenu_Collect(owner, items, 0);
	RenderOutput_RedrawSavedRectFar(menu->item_window);
	ItemMenu_RefreshEntry(mode);
	ItemMenu_DrawIcons(items, 0);
	if (ItemMenu_Count(owner_id) == 0)
		UiText_DrawCharacterAtOffsetFar((s32)MsgItemMenuEmpty, (s32)menu->item_window, 8, 24);
}

void InventoryMenu_NoOp(void)
{
}

void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target)
{
	/* FAKEMATCH: Reusing the expired owner value for the saved unit keeps
       owner in r8 and style in r10; a separate saved pointer swaps those
       registers (44 aligned instructions differ). The type-6 buffer alias
       preserves the native shared draw tail; without it the body adds
       three instructions and seven aligned instructions differ. */
	struct InventoryMenuState *menu = gMenuWork;
	s32						   style = 0;
	struct BtlUnit			  *state = UnitGetFar(owner);
	s32						   item = state->inventory[slot];
	s32						   equipped;

	if (mode == 1)
		style = 0x100;
	switch (Item_GetDirectFar(item & ITEM_ID_MASK)->type)
	{
	case 0:
		Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 7:
	case 8:
	case 9:
		if (owner == target)
		{
			style |= 2;
			Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
		}
		else
		{
			s32 size;

			state = UnitGetFar(target);
			size = BATTLE_UNIT_SIZE;
			owner = (s32)MemAlloc(size);
			Iwram_CopyWords((void *)owner, state, size);
			equipped = Inventory_RemoveFirstUnflagged(target);
			if (equipped != 0)
			{
				item &= ~INVENTORY_EQUIPPED;
				equipped = Inventory_AddItemFar(target, item);
				if (equipped != -1)
				{
					style |= 2;
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
				}
				else
				{
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
				}
			}
			else
			{
				Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
			}
			Iwram_CopyWords(state, (void *)owner, size);
			MemFree((void *)owner);
		}
		break;
	case 6:
		if (target == owner)
		{
			style |= 4;
			Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
		}
		else
		{
			s32	  size;
			void *buffer;

			state = UnitGetFar(target);
			size = BATTLE_UNIT_SIZE;
			owner = (s32)MemAlloc(size);
			buffer = (void *)owner;
			Iwram_CopyWords(buffer, state, size);
			equipped = Inventory_RemoveFirstUnflagged(target);
			if (equipped != 0)
			{
				equipped = Inventory_AddItemFar(target, item);
				if (equipped != -1)
				{
					style |= 4;
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
				}
				else
				{
					Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
				}
			}
			else
			{
				Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
			}
			Iwram_CopyWords(state, (void *)owner, size);
			buffer = (void *)owner;
			MemFree(buffer);
		}
		break;
	}
}

s32 Inventory_RemoveFirstUnflagged(s32 owner)
{
	struct BtlUnit *state = UnitGetFar(owner);
	s32				result = 0;
	s32				i;

	for (i = 0; i < 15; i++)
	{
		if (state->inventory[i] == 0)
		{
			result = 1;
			break;
		}
		if ((state->inventory[i] & INVENTORY_EQUIPPED) == 0)
		{
			s32 quantity = state->inventory[i] >> INVENTORY_QUANTITY_SHIFT;
			s32 count = quantity + 1;
			s32 cnt;

			if (quantity == 0)
				count = 1;
			for (cnt = 0; cnt < count; cnt++)
				result = Inventory_RemoveFar(owner, i);
			if (result != 2)
				return 0;
			result = 1;
			break;
		}
	}
	return result;
}

#if EDITION_INTERNATIONAL

s32 ItemMenu_CmdCursorX(s32 column, s32 row)
{
	if (column > 2 || row > 2 || column < 0 || row < 0)
		return 0;
	return ItemMenu_CommandColumnXTable[row * 3 + column];
}

s32 ItemMenu_CmdCursorY(s32 column, s32 row)
{
	s32 y;

	(void)column;
	y = 0x1E;
	if (row != 0)
	{
		y = 0x26;
	}
	return y;
}
#endif

s32 ItemMenu_ChooseCmd(void)
{
	struct InventoryMenuState *menu;
	s8						   command_storage[6];
	u16						  *redraw_flag;
	s32						   col;
	s32						   saved;
	s32						   row;
	s32						   index;
	s32						   need_redraw;
	s8						  *command_states;
	s32						   x;
	s32						   y;

	index = 0;
	col = 0;
	row = 0;
	menu = gMenuWork;
	need_redraw = 1;
	command_states = command_storage;
	ItemMenu_BuildCmd(command_states);
	redraw_flag = &menu->flags;
	if (*redraw_flag != 1)
	{
		s32 message_window;

		ItemMenu_HideAllIcons();
		RenderOutput_RedrawSavedRectFar((void *)menu->list_window);
		message_window = (s32)menu->message_window;
		ItemMenu_SetMsgWin7();
		RenderOutput_RedrawSavedRectFar((void *)message_window);
		UiWindow_DrawDividerLineFar(message_window, 0, 3, 0x10, 3);
		ItemMenu_DrawItemHead();
		ItemMenu_DrawCmd(command_states, message_window);
#if EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar((void *)menu->info_window);
		UiText_DrawCharacterAtOffsetFar(
			(menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
			(s32)menu->info_window, 0, 0);
#else
		RenderOutput_ClearListFar((void *)menu->info_window);
		UiText_DrawResourceFar((menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName,
			(s32)menu->info_window, 0, 0);
#endif
	}
	*redraw_flag = 0;

	saved = (s8)menu->list_mode;
	if (saved == -1)
	{
		if (command_states[2] == 1)
		{
			col = 2;
			row = 0;
		}
		if (command_states[3] == 1)
		{
			col = 0;
			row = 1;
		}
		if (command_states[1] == 1)
		{
			col = 1;
			row = 0;
		}
		if (command_states[4] == 1)
		{
			col = 1;
			row = 1;
		}
		if (command_states[0] == 1)
		{
			col = 0;
			row = 0;
		}
	}
	else
	{
		col = (s8)(saved % 3);
		row = (s8)(saved / 3);
		index = row * 3 + col;
	}
#if EDITION_INTERNATIONAL
	x = ItemMenu_CmdCursorX(col, row);
	y = ItemMenu_CmdCursorY(col, row);
	UiMenu_SlideCursor(x, y);
#else
	UiMenu_SlideCursor(col * 40 + 86, row * 8 + 30);
#endif

	for (;;)
	{
		if (GameFlag_TestFar(0x150) != 0)
			break;
		if (need_redraw != 0)
		{
			need_redraw = 0;
			col = (col + 3) % 3;
			row = (row + 2) % 2;
			index = row * 3 + col;
			EquipmentMenu_StartCompatibilityIndicators();
			if (index > 2)
			{
				menu->equip_preview = 1;
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
				if (index == 3)
				{
					Scheduler_AddOrUpdateCallback(
						&EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
				}
			}
			else if (index != 0)
			{
				menu->equip_preview = 0;
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[0], menu->selected_slots[0], 0, menu->pane_owner[0]);
			}
			else
			{
				Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->pane_owner[0], 0, 0);
			}
		}
#if EDITION_INTERNATIONAL
		x = ItemMenu_CmdCursorX(col, row);
		y = ItemMenu_CmdCursorY(col, row);
		UiMenu_PositionCursor(x, y);
#else
		UiMenu_PositionCursor(col * 40 + 86, row * 8 + 30);
#endif
		WaitFrames(1);

		if ((gKeyState & KEY_A) != 0)
		{
			if (command_states[index] == -1)
			{
				AudioCommand_PlayFar(114);
			}
			else
			{
				switch (index)
				{
				case 0:
					AudioCommand_PlayFar(174);
					break;
				case 1:
					AudioCommand_PlayFar(175);
					break;
				case 2:
				case 3:
				case 5:
					AudioCommand_PlayFar(112);
					break;
				case 4:
					AudioCommand_PlayFar(117);
					break;
				default:
					AudioCommand_PlayFar(112);
					break;
				}
				menu->list_mode = index;
				break;
			}
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(113);
			index = -1;
			menu->list_mode = index;
			break;
		}

		if ((gKeysRepeat & KEY_UP) != 0)
		{
			row -= 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
		else if ((gKeysRepeat & KEY_DOWN) != 0)
		{
			row += 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
		else if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			col += 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
		else if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			col -= 1;
			need_redraw = 1;
			AudioCommand_PlayFar(111);
		}
	}

	menu->equip_preview = 0;
	EquipmentMenu_StartCompatibilityIndicators();
	return index;
}

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

void ItemMenu_BuildCmd(s8 *command_states)
{
	struct InventoryMenuState *menu;
	struct ItemDefinition	  *item;

	menu = gMenuWork;
	item = Item_GetDirectFar(ITEM_ID_MASK & menu->selected_items[0]);

	if (item->type == 0)
	{
		command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
	}
	else
	{
		command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_AVAILABLE;
	}

	if (Item_ClassifyUseMode(menu->pane_owner[0], menu->selected_items[0]) != -1)
		command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
	else
		command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;

	if (menu->selected_items[0] & INVENTORY_BROKEN)
		command_states[ITEM_COMMAND_USE] = COMMAND_DISABLED;

	if (Item_CanOwnerEquip(menu->pane_owner[0], menu->selected_items[0] & ITEM_ID_MASK) == 0)
	{
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
	}

	command_states[ITEM_COMMAND_GIVE] = COMMAND_AVAILABLE;
	command_states[ITEM_COMMAND_DROP] = COMMAND_AVAILABLE;
	command_states[ITEM_COMMAND_INSPECT] = COMMAND_AVAILABLE;

	if (menu->selected_items[0] & INVENTORY_EQUIPPED)
	{
		command_states[ITEM_COMMAND_REMOVE] = COMMAND_AVAILABLE;
		command_states[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
	}
	else
	{
		command_states[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
	}

	if (item->flags & ITEM_CANNOT_UNEQUIP)
	{
		command_states[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
		if (menu->selected_items[0] & INVENTORY_EQUIPPED)
		{
			command_states[ITEM_COMMAND_GIVE] = COMMAND_DISABLED;
			command_states[ITEM_COMMAND_DROP] = COMMAND_DISABLED;
		}
	}

	if (BattleFx_HasTriggerFar(menu->selected_items[0] & ITEM_ID_MASK) != 0)
		command_states[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;

	if (menu->party_count <= 1)
		command_states[ITEM_COMMAND_GIVE] = COMMAND_DISABLED;

	if (item->flags & ITEM_INDISPENSABLE)
		command_states[ITEM_COMMAND_DROP] = COMMAND_DISABLED;
}

#if EDITION_INTERNATIONAL
#define ITEM_TEXT_X 0x20
#else
#define ITEM_TEXT_X 0x28
#endif

void ItemMenu_DrawCmd(const s8 *command_states, s32 window)
{
	s32 disabled;
	s32 value;
	u32 message;

	UiWork_SetParamNibbleFar(0xf);
	value = command_states[ITEM_COMMAND_USE];
	disabled = -1;
	if (value == disabled)
		UiWork_SetParamNibbleFar(0xe);

	message = (u32)MsgItemCommandUse;
	UiText_DrawCharacterAtOffsetFar(message, window, 0, 0x18);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_EQUIP] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 1, window, ITEM_TEXT_X, 0x18);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_GIVE] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 2, window, 0, 0x20);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_DROP] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 3, window, 0x50, 0x20);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_INSPECT] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 4, window, 0x50, 0x18);
	UiWork_SetParamNibbleFar(0xf);
	if (command_states[ITEM_COMMAND_REMOVE] == disabled)
		UiWork_SetParamNibbleFar(0xe);

	UiText_DrawCharacterAtOffsetFar(message + 5, window, ITEM_TEXT_X, 0x20);
	UiWork_SetParamNibbleFar(0xf);
}

s32 Item_ClassifyUseMode(s32 owner, s32 itemId)
{
	s32					   masked = itemId;
	struct ItemDefinition *def;
	s32					   result = -1;

	masked &= ITEM_ID_MASK;
	def = Item_GetDirectFar(masked);

	if (BattleFx_HasTriggerFar(masked) != 0)
	{
		return 0;
	}

	{
		struct BtlAct *action = BtlActGet(def->action_id & OWNER_ACTION_ID_MASK);

		if (def->action_id != 0)
		{
			if (def->type != 0)
			{
				if (def->use_type != 3)
				{
					if (Item_CanOwnerEquip(owner, masked) != 0)
					{
						result = 1;
					}
				}
			}
			else
			{
				result = 1;
			}

			if (result == 1)
			{
				s32 mask = 0x80;
				u8	flags = action->tgt_flg;

				if ((flags & 0x40) != 0)
				{
					u8 range = action->rng;
					result = (range == 0xff) ? 2 : 1;
				}
				else
				{
					result = (flags & mask) ? -1 : 0;
				}
			}
		}
	}

	return result;
}

void ItemMenu_TryBreak(void)
{
	struct InventoryMenuState *menu;

	menu = gMenuWork;
	if ((Item_GetDirectFar(ITEM_ID_MASK & menu->selected_items[0])->use_type == 2)
		&& (Random16() < 0x2000U))
	{
		Inventory_BreakFar(menu->pane_owner[0], menu->selected_slots[0]);
		AudioCommand_PlayFar(SOUND_ITEM_BREAK);
		InventoryMenu_ShowModalMessage((s32)MsgItemBroke, 0, -1);
#if !EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar(menu->info_window);
#endif
	}
}

s32 ItemMenu_OpenDetail(s32 item_index)
{
	struct InventoryMenuState *menu = gMenuWork;

	UiWindow_UpdateOrCreate((s32 *)&menu->equip_window, 0, 0, 13, 10, 2);
	Palette_CopyObjectBankToBackground14();

	if (menu->selected_items[item_index] != 0)
		ItemMenu_DrawItemDetails((s32)menu->equip_window, menu->selected_items[item_index]);

	return 1;
}

s32 Menu_SelectQuantity(s32 value)
{
#if EDITION_INTERNATIONAL
	s32						   changed = 1;
	struct InventoryMenuState *menu = gMenuWork;
	struct RenderOutput		  *confirmState = menu->selected_item_icon;
#else
	struct InventoryMenuState *menu = gMenuWork;
	s32						   changed = 1;
#endif
	s32 window;
	s32 quantity = 0;

#if EDITION_INTERNATIONAL
	confirmState->active = 13;
	window = UiWindow_CreateFar(0, 0, 30, 10, 2);
#else
	window = UiWindow_CreateFar(0, 0, 13, 10, 2);
#endif
	Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);

	{
		struct RenderOutput *iconState = menu->cursor;
		iconState->active = 13;
	}
	Palette_CopyObjectBankToBackground14();
	WaitFrames(1);

	for (;;)
	{
		if (GameFlag_TestFar(336) != 0)
			break;

		if (changed != 0)
		{
			changed = 0;
			quantity = (quantity + 5) % 5;
			ItemMenu_DrawItemDetails(window, value);
		}

		if (gKeyState & 1)
			break;
		if (gKeyState & 2)
		{
			quantity = -1;
			break;
		}

		if (gKeysRepeat & 0x40)
		{
			quantity -= 1;
			changed = 1;
		}
		if (gKeysRepeat & 0x80)
		{
			quantity += 1;
			changed = 1;
		}
		WaitFrames(1);
	}

	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	WaitFrames(1);
	UiWork_FinalizeFar((void *)window, 1);
	RenderOutput_RedrawSavedRectFar(menu->main_window);
	Palette_LightenBankHighlight(14);
	{
		s32 delay = 0xc80;

		Scheduler_AddOrUpdateCallback((const void *)Menu_UpdateEntryObjectTransforms, delay);
	}

	{
		struct RenderOutput *iconState = menu->cursor;
#if EDITION_INTERNATIONAL
		iconState->active = 1;
#else
		u8 one = 1;

		iconState->active = one;
		menu->flags = one;
#endif
	}
#if EDITION_INTERNATIONAL
	UiWindow_DrawFrameFar(13, 0, 17, 10);
#endif

	return quantity;
}

void ItemMenu_DrawItemDetails(s32 window, s32 item)
{
	struct ItemDefinition *def;
	s32					   i;
	s32					   found;
	s32					   amount;
	s32					   consumable;
	s8					   row;

	row = 0;
	consumable = 0;
	def = Item_GetDirectFar(item & ITEM_ID_MASK);
	if (def->type != 0)
	{
		found = 0;
		if (def->primary_bonus == 0 && def->secondary_bonus == 0)
		{
			for (i = 0; i < 4; i++)
			{
				if (def->effects[i].kind != 0 || def->use_type == 3)
				{
					found = 1;
					break;
				}
			}
		}
		else
		{
			found = 1;
		}
		if (found == 1)
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgEquipEffectHeading, window, 16, 0);
			row = 1;
		}
		if (def->primary_bonus != 0)
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgStatLabel, window, 0, row * 8);
			amount = def->primary_bonus;
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			row++;
		}
		if (def->secondary_bonus != 0)
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgDefenseLabel, window, 0, row * 8);
			amount = def->secondary_bonus;
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			row++;
		}
	}
	for (i = 0; i < 4; i++)
	{
		if (def->effects[i].kind == 0)
			continue;
		amount = def->effects[i].amount;
		switch (def->effects[i].kind)
		{
		case 0:
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 26:
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			break;
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
			UiWindow_SetTilemapEntryFar(
				window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + (s32)MsgEquipEffectName, window, 8, row * 8);
			ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
#if EDITION_INTERNATIONAL
			UiText_DrawStringInWindowFar(Menu_TimesString, window, 64, row * 8);
			if (amount > 9)
			{
				UiText_DrawNumberInWindowFar(1, 1, window, 72, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 80, row * 8);
				UiText_DrawNumberInWindowFar(amount - 10, 1, window, 88, row * 8);
			}
			else
			{
				UiText_DrawNumberInWindowFar(0, 1, window, 72, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 80, row * 8);
				UiText_DrawNumberInWindowFar(amount, 1, window, 88, row * 8);
			}
#else
			if (amount > 9)
			{
				UiText_DrawNumberInWindowFar(1, 1, window, 48, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 56, row * 8);
				UiText_DrawNumberInWindowFar(amount - 10, 1, window, 64, row * 8);
			}
			else
			{
				UiText_DrawNumberInWindowFar(0, 1, window, 48, row * 8);
				UiText_DrawStringInWindowFar(Menu_PointString, window, 56, row * 8);
				UiText_DrawNumberInWindowFar(amount, 1, window, 64, row * 8);
			}
			UiText_DrawCharacterAtOffsetFar((s32)MsgEffectRateSuffix, window, 72, row * 8);
#endif
			break;
		case 23:
		case 25:
		case 27:
			UiText_DrawCharacterAtOffsetFar(
				def->effects[i].kind + (s32)MsgEquipEffectName, window, 0, row * 8);
			break;
		}
		row++;
	}
	if (def->flags & ITEM_CURSED)
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgItemCursed, window, 0, row * 8);
		row++;
	}
	if (def->use_type == 3)
	{
		UiText_DrawCharacterAtOffsetFar((s32)MsgBestowsPsynergy, window, 0, row * 8);
		consumable = 1;
		row++;
	}
	if (def->use_type != 4 && def->use_type != 0)
	{
		if (!consumable)
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgUsesHeading, window, 16, row * 8);
			row++;
		}
		switch (def->use_type)
		{
		case 0:
			break;
		case 1:
			UiText_DrawCharacterAtOffsetFar((s32)MsgSingleUse, window, 0, row * 8);
			row++;
			break;
		case 2:
		{
			s32 m;

			if (item & INVENTORY_BROKEN)
			{
				m = (s32)MsgBrokenNotice;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
				m++;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
			}
			else
			{
				m = (s32)MsgMightBreak;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
				m++;
				UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
				row++;
			}
			break;
		}
		}
	}
	if (def->flags & ITEM_STACKABLE)
	{
		if (row != 0)
			row++;
		i = (s32)MsgNumberHeading;
		UiText_DrawCharacterAtOffsetFar(i, window, 16, row * 8);
		row++;
		amount = (item & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
#if EDITION_INTERNATIONAL
		UiWork_PushValueSlotFar(amount + 1, 5);
		UiText_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
#else
		UiText_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
		UiText_DrawCharacterAtOffsetFar(i + 1, window, 16, row * 8);
#endif
		row++;
	}
	found = 0;
	if (row == 0)
	{
		if (def->flags & 4)
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgRareItem, window, 0, 0);
			found = 1;
		}
		if (!found)
		{
			if (def->flags & ITEM_INDISPENSABLE)
			{
				UiText_DrawCharacterAtOffsetFar((s32)MsgImportantItem, window, 0, 0);
				found = 1;
			}
			if (!found)
				UiText_DrawCharacterAtOffsetFar((s32)MsgDetailsUnknown, window, 0, 0);
		}
	}
}

void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y)
{
	s32 digits;
	s32 magnitude;

	UiText_DrawNumberInWindowFar(delta, 3, window, x, y);
	digits = 1;
	magnitude = delta;
	if (delta < 0)
	{
		magnitude = -delta;
	}
	if (magnitude > 9)
	{
		digits = 2;
	}
	magnitude = delta;
	if (delta < 0)
	{
		magnitude = -delta;
	}
	if (magnitude > 99)
	{
		digits = 3;
	}
	if (delta > 0)
	{
		UiText_DrawStringInWindowFar(&Menu_PlusSignString, window, x - digits * 8 + 16, y);
	}
	else
	{
		UiText_DrawStringInWindowFar(&Menu_MinusSignString, window, x - digits * 8 + 16, y);
	}
}

void ItemMenu_SetItemWin5(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->item_window, 13, 5, 17, 10);
}

void ItemMenu_SetItemWin3(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->item_window, 13, 3, 17, 10);
}

void ItemMenu_SetMsgWin5(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		5);
}

void ItemMenu_SetMsgWin6(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		6);
}

void ItemMenu_SetMsgWin7(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		7);
}

void ItemMenu_SetMsgWin3(void)
{
	UiWindow_SetBounds(
		(struct RenderInput *)((struct InventoryMenuState *)gMenuWork)->message_window, 13, 0, 17,
		3);
}

#if EDITION_INTERNATIONAL
#define GIVE_CURSOR_X 128
#define GIVE_ASK_X 32
#define GIVE_ASK_Y 0
#else
#define GIVE_CURSOR_X 88
#define GIVE_ASK_X 0
#define GIVE_ASK_Y 32
#endif

s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single)
{
	s32						   quantity = base;
	s32						   slot;
	s32						   changed;
	s32						   other_count;
	s32						   count;
	struct InventoryMenuState *menu = gMenuWork;
	void					  *tiles = Runtime_AllocateBlock(14, 0x400);
	s32						   window;
	struct RenderOutput		  *object;

	changed = 1;
	other_count = 0;
	window = (s32)menu->message_window;
	ItemMenu_SetMsgWin7();
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	if (single == 0)
		other_count = InventoryMenu_GetItemQuantity(
			menu->pane_owner[1], menu->selected_items[0] & ITEM_ID_MASK);
	count =
		InventoryMenu_GetItemQuantity(menu->pane_owner[0], menu->selected_items[0] & ITEM_ID_MASK);
	slot = Resource_FindFreeEntry();
	if (slot != 96)
	{
		VramBlock_LoadCached(slot, 256, 0);
		RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 48, 32);
		object = RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 80, 32);
		object->oam.index += 4;
		UiMenu_SlideCursor(GIVE_CURSOR_X, 40);
		while (!GameFlag_TestFar(0x150))
		{
			if (changed)
			{
				changed = 0;
				quantity = (range + quantity) % range;
				RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
				UiText_DrawCharacterAtOffsetFar(
					(s32)MsgGiveHowMany, window, GIVE_ASK_X, GIVE_ASK_Y);
				Dma_Set(Menu_QuantityTiles, tiles, 0x84000040, REG_DMA3);
				Shop_FillSelectorFar(30, 14, tiles);
				Shop_FillSelectorFar(range + base, 0, tiles);
				Shop_FillSelectorFar(base + quantity + 1, 10, tiles);
				Shop_FillSelectorFar(base, 2, tiles);
				VramBlock_LoadCached(slot, 256, tiles);
				UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
				UiText_DrawCharacterAtOffsetFar(
					(menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemName, window, 16, 8);
				UiText_DrawNumberInWindowFar(count - quantity - 1, 2, window, 16, 24);
#if EDITION_INTERNATIONAL
				if (single == 0)
					UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
#else
				UiText_DrawCharacterAtOffsetFar((s32)MsgItemCounter, window, 32, 24);
				if (single == 0)
				{
					UiText_DrawNumberInWindowFar(other_count + quantity + 1, 2, window, 80, 24);
					UiText_DrawCharacterAtOffsetFar((s32)MsgItemCounter, window, 96, 24);
				}
#endif
				UiText_DrawStringAtOffsetFar(UnitGetFar(menu->pane_owner[0])->name, window, 16, 16);
				if (single == 0)
					UiText_DrawStringAtOffsetFar(
						UnitGetFar(menu->pane_owner[1])->name, window, 80, 16);
			}
			if (gKeyState & KEY_A)
			{
				AudioCommand_PlayFar(112);
				break;
			}
			if (gKeyState & KEY_B)
			{
				quantity = -1;
				AudioCommand_PlayFar(113);
				break;
			}
			UiMenu_PositionCursor(GIVE_CURSOR_X, 40);
			if (gKeysRepeat & KEY_LEFT)
			{
				quantity--;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
			if (gKeysRepeat & KEY_RIGHT)
			{
				quantity++;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
			WaitFrames(1);
		}
	}
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	RenderOutput_ClearListFar((void *)window);
	Runtime_ReleaseHeapBlock(14);
	menu->selected_item_icon->active = 13;
	if (GameFlag_TestFar(0x150))
		quantity = -1;
	return quantity;
}

void ItemMenu_DrawItemHead(void)
{
	struct InventoryMenuState *menu = gMenuWork;

	Resource_LoadByModeIntoSlotFar(
		2, menu->selected_items[0], (u8)menu->selected_item_icon->index, 0);
	menu->selected_item_icon->active = 1;
	menu->selected_item_icon->x = 112;
	menu->selected_item_icon->y = 8;
	UiIcon_PrepareObject(menu->selected_item_icon);
	UiText_DrawStringAtOffsetFar(
		UnitGetFar(menu->pane_owner[0])->name, (s32)menu->message_window, 16, 0);
	UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemName,
		(s32)menu->message_window, 16, 8);
}

s32 ItemMenu_ConfirmDrop(s32 a0)
{
	s32 win;
	s32 slot;
	s32 text;
	s32 label;
	s32 rows = 2;
	s32 sel = 1;
	s32 changed = 1;

	win = UiWindow_CreateFar(13, 3, 17, 10, 2);
	slot = a0 & ITEM_ID_MASK;
	Item_GetDirectFar(slot);
	UiText_DrawCharacterAtOffsetFar(slot + (s32)MsgItemName, win, 24, 0);
	text = (s32)MsgConfirmDrop;
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar(text, win, 0, 16);
#else
	UiText_DrawCharacterAtOffsetFar(text, win, 8, 16);
#endif
	text++;
#if EDITION_INTERNATIONAL
	UiText_DrawCharacterAtOffsetFar(text, win, 0, 24);
#else
	UiText_DrawCharacterAtOffsetFar(text, win, 8, 24);
#endif
	label = (s32)MsgYes;
	UiText_DrawCharacterAtOffsetFar(label, win, 24, 40);
	label++;
	UiText_DrawCharacterAtOffsetFar(label, win, 24, 56);
	UiMenu_SlideCursor(104, 86);
	for (;;)
	{
		if (GameFlag_TestFar(0x150))
			break;
		if (changed)
		{
			changed = 0;
			sel = (sel + rows) % rows;
		}
		if (gKeyState & KEY_A)
		{
			AudioCommand_PlayFar(112);
			break;
		}
		if (gKeyState & KEY_B)
		{
			AudioCommand_PlayFar(113);
			sel = 1;
			break;
		}
		UiMenu_PositionCursor(104, (sel << 4) + 70);
		if (gKeysRepeat & KEY_UP)
		{
			sel--;
			changed = 1;
			AudioCommand_PlayFar(111);
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			sel++;
			changed = 1;
			AudioCommand_PlayFar(111);
		}
		WaitFrames(1);
	}
	if (GameFlag_TestFar(0x150))
	{
		sel = 1;
	}
	UiWork_FinalizeFar((void *)win, 1);
	return sel;
}

s32 ItemMenu_ConfirmEquip(s32 unused)
{
	s32						   rows = 2;
	s32						   sel = 0;
	s32						   changed = 1;
	struct InventoryMenuState *menu = gMenuWork;
	struct BtlUnit			  *state = UnitGetFar(menu->pane_owner[1]);
	struct BtlUnit			  *saved;
	s32						   win;
	s32						   item;
	s32						   owner;

	item = menu->selected_slots[1];
	owner = menu->pane_owner[1];
	ItemMenu_DrawEquipPreview(owner, item, 0, owner);
	saved = MemAlloc(BATTLE_UNIT_SIZE);
	Iwram_CopyWords(saved, state, BATTLE_UNIT_SIZE);
	win = (s32)menu->message_window;
	if ((u32)(Inventory_EquipFar(menu->pane_owner[1], menu->selected_slots[1]) + 2) <= 1)
	{
		sel = 1;
	}
	else
	{
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar((s32)MsgYes, win, 24, 24);
		UiText_DrawCharacterAtOffsetFar((s32)MsgYes + 1, win, 72, 24);
		UiWindow_ClearInteriorTilesFar((void *)win, 16, 16, 96, 24);
		UiText_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, win, 0, 16);
		UiMenu_SlideCursor(110, 32);
#else
		UiText_DrawCharacterAtOffsetFar((s32)MsgYes, win, 96, 0);
		UiText_DrawCharacterAtOffsetFar((s32)MsgYes + 1, win, 96, 16);
		UiWindow_ClearInteriorTilesFar((void *)win, 16, 16, 96, 24);
		UiText_DrawCharacterAtOffsetFar((s32)MsgEquipThisItem, win, 8, 16);
		UiMenu_SlideCursor(184, 5);
#endif
		for (;;)
		{
			if (GameFlag_TestFar(0x150))
				break;
			if (changed)
			{
				changed = 0;
				sel = (sel + rows) % rows;
			}
			if (gKeyState & KEY_A)
			{
				AudioCommand_PlayFar(175);
				break;
			}
			if (gKeyState & KEY_B)
			{
				AudioCommand_PlayFar(113);
				sel = 1;
				break;
			}
#if EDITION_INTERNATIONAL
			UiMenu_PositionCursor(sel * 48 + 110, 32);
			if (gKeysRepeat & KEY_LEFT)
			{
#else
			UiMenu_PositionCursor(184, (sel << 4) + 5);
			if (gKeysRepeat & KEY_UP)
			{
#endif
				sel--;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
#if EDITION_INTERNATIONAL
			if (gKeysRepeat & KEY_RIGHT)
			{
#else
			if (gKeysRepeat & KEY_DOWN)
			{
#endif
				sel++;
				changed = 1;
				AudioCommand_PlayFar(111);
			}
			WaitFrames(1);
		}
	}
	if (GameFlag_TestFar(0x150))
		sel = 1;
	if (sel == 1)
		Iwram_CopyWords(state, saved, BATTLE_UNIT_SIZE);
	MemFree(saved);
	UnitCalcFar(menu->pane_owner[1]);
	Owner_RefreshClassActionsFar(menu->pane_owner[1]);
	return sel;
}

void Resource_LoadPairedBlocks(void)
{
	struct InventoryMenuState *state = gMenuWork;
	s32						   value = Resource_FindFreeEntry();

	state->resource_slots[0] = value;
	VramBlock_LoadCached(value, 128, Menu_ArrowTiles);
	value = Resource_FindFreeEntry();
	state->resource_slots[1] = value;
	VramBlock_LoadCached(value, 128, Menu_SmallDownArrowTiles);
}

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
	s32						   encoded;
	struct InventoryMenuState *menu = gMenuWork;
	s32						   limit;
	s32						   remainder;
	s32						   quotient;
	s32						   groups;
	s32						   value;

	limit = ItemMenu_Count(menu->pane_owner[index]);
	encoded = (s32)UnitGetFar(menu->pane_owner[index]);
	value = menu->selected_index_by_owner[menu->pane_owner[index]];
	if ((value + 1) > limit)
	{
		value = limit - 1;
	}
	quotient = value / 5;
	remainder = value % 5;
	groups = limit / 5;
	if (limit % 5 != 0)
	{
		groups++;
	}
	result->owner_state = encoded;
	result->page = quotient;
	result->page_count = groups;
	result->row = remainder;
	result->entry_count = limit;
	result->selected_index = value;
	return 1;
}

s32 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state)
{
	struct InventoryMenuState *menu;
	s32						   page;
	s32						   combined;
	s32						   row;

	page = state->page;
	menu = gMenuWork;
	combined = page * 5;
	combined += state->row;
	state->selected_index = combined;

#if EDITION_INTERNATIONAL
	RenderOutput_RedrawSavedRectFar(menu->info_window);
#else
	RenderOutput_ClearListFar(menu->info_window);
#endif
	WaitFrames(1);

	combined = state->selected_index;
	if (menu->items[combined] != 0)
	{
		s32 masked = (menu->items[combined] & ITEM_ID_MASK) + (s32)MsgItemPlainName;
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar(masked, (s32)menu->info_window, 0, 0);
#else
		UiText_DrawResourceFar(masked, (s32)menu->info_window, 0, 0);
#endif
	}

	row = 0;
	do
	{
		if (row == state->row)
		{
			Render_SetTilemapFlagRect((void *)menu->item_window, 1, row * 2 + 1, 14, 1, 14);
		}
		else
		{
			Render_SetTilemapFlagRect((void *)menu->item_window, 1, row * 2 + 1, 14, 1, 15);
		}
		row++;
	} while (row <= 4);

	WaitFrames(1);
	return 1;
}

#if EDITION_INTERNATIONAL
#define PAGE_X 116
#define ENTRY_X 24
#else
#define PAGE_X 120
#define ENTRY_X 32
#endif

s32 ItemMenu_DrawNamePage(s32 window, s32 unused, const struct MenuResult *state)
{
	struct InventoryMenuState *menu = gMenuWork;
	u32						   page;
	u32						   first_entry;
	u32						   visible_count;
	u8						   row;
	const u16				  *item_id;

	(void)unused;

	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

	page = state->page;
	first_entry = page * 5;
	visible_count = (u8)(state->entry_count - first_entry);
	if (visible_count > 5)
	{
		visible_count = 5;
	}

	Menu_SetPageIcons(5, first_entry, window, PAGE_X, 34);
	Menu_DrawPageIndicator(window, state->entry_count, 5, state->page, 15);

	row = 0;
	if (visible_count > row)
	{
		item_id = &menu->items[first_entry];
		do
		{
			UiText_DrawCharacterAtOffsetFar((item_id[0] & ITEM_ID_MASK) + (s32)MsgItemName,
				(s32)menu->item_window, ENTRY_X, row * 16 + 8);
			row++;
			item_id++;
		} while (visible_count > row);
	}

	return 1;
}

s32 InventoryMenu_ItemNamePageReturnTrue(void)
{
	return 1;
}

void InventoryMenu_ItemNamePageNoOp(void)
{
}

s32 ItemMenu_RunList(s32 pane)
{
	struct InventoryMenuState *menu;
	s32						   window;
	s32						   changed;
	s32						   nav;
	s32						   tab;
	u8						   i;
	s32						   result;
	s32						   redraw;
	s32						   done;
	struct BtlUnit			  *owner;
	s32						   prev;
	struct RenderOutput		  *icon;
	s32						   work[5];
	struct MenuResult		   state;

	menu = gMenuWork;
	result = 0;
	prev = 0;
	menu->equip_preview = result;
	UiWindow_UpdateOrCreate((s32 *)&menu->list_window, 13, 3, 17, 14, 2);
	window = (s32)menu->list_window;
	done = 0;

	while (done == 0 && GameFlag_TestFar(0x150) == 0)
	{
		owner = UnitGetFar(menu->pane_owner[pane]);
		menu->item_count = ItemMenu_Collect(owner, menu->items, 0);
		ItemMenu_DrawIcons(menu->items, 0);
		menu->selected_item_icon->active = 13;
		ItemMenu_PageResult(&state, pane);
		UiMenu_PositionCursor(98, state.selected_index * 16 + 36);
		changed = 1;
		redraw = 1;

		while (GameFlag_TestFar(0x150) == 0)
		{
			UiMenu_PositionCursor(98, state.row * 16 + 36);

			if (changed != 0)
			{
				changed = 0;
				if (owner->inventory[prev] != 0)
				{
					icon = menu->entry_icons[prev];
					UiIcon_PrepareObject(icon);
				}
				if (redraw != 0)
				{
					WaitFrames(1);
					ItemMenu_DrawNamePage(window, 0, &state);
					if (pane == 0)
					{
						UiText_DrawCharacterAtOffsetFar(
							(s32)MsgChangeCharacterHelp, window, HELP_TEXT_X, 88);
					}
					redraw = 0;
				}
				ItemMenu_DrawItemDetailPage(window, work, &state);
				menu->selected_items[pane] = menu->items[state.selected_index];
				ItemMenu_DrawEquipPreview(
					menu->pane_owner[pane], state.selected_index, 0, menu->pane_owner[pane]);
				if (menu->items[state.selected_index] != 0)
				{
					icon = menu->entry_icons[state.selected_index];
					icon->active = 9;
					icon->anim_step = 0;
					icon->sentinel = 250;
				}
				for (i = 0; i < menu->party_count; i++)
				{
					AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 1);
				}
			}

			if ((gFrameCount & 31) == 0)
			{
				for (i = 0; i < menu->party_count; i++)
				{
					if (Item_CanOwnerEquip(
							menu->owner_ids[i], ITEM_ID_MASK & menu->items[state.selected_index])
						!= 0)
					{
						AnimationObjects_SelectAnimationFar(menu->owner_objects[i], 3);
					}
				}
			}

			WaitFrames(1);
			prev = state.selected_index;
			nav =
				Menu_HandlePageInput(0, state.entry_count, LIST_PAGE_SIZE, &state.row, &state.page);
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

			if ((gKeyState & KEY_A) != 0 && menu->items[state.selected_index] != 0)
			{
				AudioCommand_PlayFar(173);
				result = menu->items[state.selected_index];
				done = 1;
				break;
			}
			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(113);
				result = -1;
				done = 1;
				break;
			}
			if ((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0)
			{
				if (pane == 1)
				{
					AudioCommand_PlayFar(114);
					WaitFrames(1);
				}
				else
				{
					AudioCommand_PlayFar(111);
					menu->selected_index_by_owner[menu->pane_owner[pane]] = state.selected_index;
					tab = menu->pane_index[pane];
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
						tab = (tab + menu->party_count) % menu->party_count;
						menu->selected_owner = menu->owner_ids[tab];
						menu->pane_owner[pane] = menu->owner_ids[tab];
						menu->pane_index[pane] = tab;
						menu->item_count =
							ItemMenu_Collect(UnitGetFar(menu->pane_owner[pane]), menu->items, 0);
					} while (menu->item_count == 0);
					for (i = 0; i <= 3; i++)
					{
						menu->owner_y[i] = 30;
					}
					menu->owner_y[tab] = 26;
					break;
				}
			}
		}
	}

	UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
	UiIcon_PrepareObject(menu->grid_cursor);
	menu->selected_slots[pane] = state.selected_index;
	menu->selected_index_by_owner[menu->pane_owner[pane]] = state.selected_index;
	menu->selected_items[pane] = result;
	if (GameFlag_TestFar(0x150) != 0)
	{
		result = -1;
	}
	WaitFrames(1);
	return result;
}
