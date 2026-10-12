#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
#include "unit/system/scheduler/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "party_state.h"
#include "btlsys.h"
#include "object_runtime.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "shop.h"
#include "ui.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "keystate.h"
#include "node_chain.h"
#include "game_state.h"
#include "sound_ids.h"
#include "item.h"
#include "fixed_math.h"
#include "far_runtime.h"
#include "lib.h"
#include "fixed_point_position.h"
#include "inn_runtime.h"
#include "event_runtime.h"
#include "btlcalc.h"
#include "fieldobj.h"

// アイテム名の下の行の x（ドット）
#if EDITION_INTERNATIONAL
#define ITEM_NOTE_X 0
#else
#define ITEM_NOTE_X 8
#endif

s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selected_item)
{
	struct ShopRuntime	*shop;
	struct RenderOutput *cursor_anchor;
	s32					 list_window;
	s32					 selected_index = 0;
	s32					 redraw = 1;
	s32					 unit_id = 0;
	s32					 item_slot;
	s32					 result = 0;

	Shop_InitializeCursorWork();
	shop = gMenuWork;
	shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
	list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
	cursor_anchor = RenderOutput_CreateFar(
		shop->cursor_icon, 0x40000000, (struct RenderInput *)list_window, 0, result);
	cursor_anchor->active = 4;
	cursor_anchor->kind = result;
	ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursor_anchor;
	shop->mode = 12;
	PsynergyMenu_InitializeEntryObjectsFar(list_window, 2, 0, 8, result);

	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			selected_index = (selected_index + shop->party_member_count) % shop->party_member_count;
			unit_id = shop->party_member_ids[selected_index];
			Shop_PlaceCursor((void *)list_window, selected_index * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty(list_window, selected_index, 0);
			Shop_DrawUnitGrid(shop->item_window, unit_id);
		}

		WaitFrames(1);
		if ((gKeyState & KEY_A) != 0)
		{
			if (Inventory_CountFar(unit_id) == 0)
			{
				AudioCommand_PlayFar(0x71);
				continue;
			}

			AudioCommand_PlayFar(0x70);
			item_slot = Shop_SelUse(unit_id);
			if (item_slot == -1)
			{
				shop->cursor.anchor->active = 4;
				shop->mode = 12;
				redraw = 1;
				continue;
			}
			*selected_unit = unit_id;
			*selected_item = item_slot;
			result = 0;
			goto done;
		}

		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(0x71);
			*selected_unit = -1;
			*selected_item = -1;
			result = -1;
			goto done;
		}

		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(0x6f);
			selected_index--;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(0x6f);
			selected_index++;
			redraw = 1;
		}
	}

done:
	Menu_ReleaseEntryObjectsFar();
	UiWork_FinalizeFar(list_window, 2);
	UiWork_FinalizeFar(shop->item_window, 2);
	WaitFrames(1);
	Inn_Cleanup();
	return result;
}
s32 Shop_SelUse(s32 actor)
{
	struct ShopRuntime *shop = gMenuWork;

	s32				win2;
	s32				win1;
	struct BtlUnit *object;
	s32				selection;
	s32				redraw;
	s32				count;
	s32				result;
	s32				status;
	s32				flags;
	void		   *window;
	s32				x;
	s32				y;

	object = UnitGetFar(actor);
	count = 1;
	result = 0;
	redraw = 1;
#if EDITION_INTERNATIONAL
	win1 = UiWindow_CreateFar(14, 8, 16, 4, 2);
#else
	win1 = UiWindow_CreateFar(16, 8, 14, 4, 2);
#endif
	win2 = UiWindow_CreateFar(0, 5, 30, 3, 2);
	shop->cursor.anchor->active = 18;
	shop->mode = 12;
	selection = 0;

	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			count = Inventory_CountFar(actor);
			if (selection > count - 1)
				selection = count - 1;

			flags = object->inventory[selection] & 0x1ff;
			window = (void *)shop->item_window;
			x = selection % 5 << 4;
			y = (selection / 5 << 4) + 8;
			Shop_PlaceCursor(window, x, y);
			shop->mode = 3;
			Shop_DrawUseItem(win1, actor, selection);
			Shop_DrawMsg(win2, flags + (s32)MsgItemPlainName);
		}

		WaitFrames(1);

		if ((gKeyState & KEY_A) != 0)
		{
			status = Inventory_CheckDiscardFar(actor, selection);
			if (status == 0)
			{
				AudioCommand_PlayFar(112);
				result = selection;
				goto exit_loop;
			}
			if (status == -4)
			{
				UiText_OpenMessageWindowFar((s32)MsgCannotDropIt, 8, 1, 2);
			}
			else if (result == -3)
			{
				UiText_OpenMessageWindowFar((s32)MsgCannotRemoveIt, 8, 1, 2);
			}
			AudioCommand_PlayFar(113);
			while (UiWork_IsCompleteFar() == 0)
				WaitFrames(1);
			UiWork_FinalizePendingCoreFar();
			continue;
		}

		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(113);
			result = -1;
			goto exit_loop;
		}

		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(111);
			selection -= 1;
			selection = (selection + count) % count;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(111);
			selection += 1;
			selection = (selection + count) % count;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_UP) != 0)
		{
			selection -= 5;
			if (selection < 0)
				selection += 15;
			while (selection >= count)
				selection -= 5;
			AudioCommand_PlayFar(111);
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_DOWN) != 0)
		{
			selection += 5;
			if (selection >= count)
				selection -= 15;
			while (selection < 0)
				selection += 5;
			AudioCommand_PlayFar(111);
			redraw = 1;
		}
	}

exit_loop:
	UiWork_FinalizeFar(win2, 2);
	UiWork_FinalizeFar(win1, 2);
	WaitFrames(1);
	return result;
}

void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_id)
{
	struct BtlUnit *unit = UnitGetFar(unit_id);
	s32				masked = unit->inventory[item_id] & 0x1ff;
	u32				entry = unit->inventory[item_id];
	s32				mult = (entry >> 11) + 1;

	if (window != 0)
	{
		s32 result;

		RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
		UiText_DrawCharacterAtOffsetFar(masked + (s32)MsgItemName, window, 0, 0);

		result = Inventory_CheckDiscardFar(unit_id, item_id);
		if (result == -4)
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgCannotDrop, window, ITEM_NOTE_X, 8);
		}
		else if (result == -3)
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgCannotRemove, window, ITEM_NOTE_X, 8);
		}
		else
		{
			s32 qty;
			s32 total;

			qty = Shop_SalePrice(unit->inventory[item_id]);
			total = mult * qty;

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_IT)
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 80, 8);
#elif defined(TBS_EDITION_DE)
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 0, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 32, 8);
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 72, 8);
#elif defined(TBS_EDITION_ES)
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 88, 8);
#elif defined(TBS_EDITION_FR)
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 32, 8);
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 72, 8);
#else
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
			UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 1, window, 80, 8);
#endif
		}
	}
}
