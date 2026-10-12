#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop2.h"
#include "unit/menu/shop/shop3.h"
#include "unit/menu/shop/shop4.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime2.h"
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

s32 Shop_SelBuy(void)
{
	struct ShopRuntime	  *shop = gMenuWork;
	s32					   price_window = 0;
	s32					   stock_window;
	s32					   message_window;
	s32					   party_window;
	s32					   selected = 0;
	s32					   quantity;
	struct ItemDefinition *item;
	s32					   ask_again;
	s32					   item_window;
	s32					   redraw;
	s32					   cursor;
	s32					   count;
	s32					   item_id;
	struct ItemDefinition *stock_item;
	s32					   result;
	s32					   prev;
	s32					   member;
	s32					   slot;
	s32					   i;
	s16					  *ids;

	shop->item_window = 0;
	shop->money_window = UiWindow_CreateFar(30 - SHOP_MONEY_WIDTH, 7, SHOP_MONEY_WIDTH, 4, 2);
	Shop_DrawMoney();
	price_window = UiWindow_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, 2);

next_page:
	cursor = selected;
	stock_window = UiWindow_CreateFar(0, 12, 30, 4, 2);
	shop->cursor.anchor->active = 18;
	shop->mode = 12;
	message_window = UiWindow_CreateFar(0, 17, 30, 3, 2);
	redraw = 1;

	for (;;)
	{
		count = shop->stock_count;
		if (redraw != 0)
		{
			ids = shop->stock_item_ids;
			item_id = ids[cursor];
			stock_item = Item_GetDirectFar(item_id);
			redraw = 0;
			Shop_PlaceCursor((void *)stock_window, cursor % 7 * 32 - 8, 8);
			shop->mode = 4;
			Shop_DrawStock(stock_window, cursor);
			Shop_DrawMsg(message_window, item_id + (s32)MsgItemPlainName);
			RenderOutput_RedrawSavedRectFar((struct UiWindow *)price_window);
			Shop_DrawItemPrice(price_window, item_id, stock_item->price, 0);
		}
		if ((gKeyState & KEY_A) != 0)
		{
			selected = cursor;
			AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
			result = 0;
			goto stock_done;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			result = -1;
			goto stock_done;
		}
		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			prev = cursor;
			cursor--;
			cursor = (cursor + count) % count;
			if (prev != cursor)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			prev = cursor;
			cursor++;
			cursor = (cursor + count) % count;
			if (prev != cursor)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
		}
		if ((gKeysRepeat & KEY_UP) != 0 && cursor - 7 >= 0)
		{
			cursor -= 7;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_DOWN) != 0)
		{
			if (cursor + 7 < (count + 6) / 7 * 7)
			{
				cursor += 7;
				redraw = 1;
			}
			if (cursor > count - 1)
				cursor = count - 1;
		}
		WaitFrames(1);
	}

stock_done:
	UiWork_FinalizeFar(message_window, 2);
	UiWork_FinalizeFar(stock_window, 2);
	WaitFrames(1);
	if (result != 0)
		goto close;

	shop->selected_item = shop->stock_item_ids[selected];
	UiMessage_ShowAndWait((s32)MsgWhoGetsThis);
	item = Item_GetDirectFar(shop->selected_item);
	quantity = 1;
	member = 0;
	ask_again = 0;
	cursor = 0;
	party_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
	shop->cursor.anchor->active = 4;
	shop->mode = 12;
	PsynergyMenu_InitializeEntryObjectsFar(party_window, 2, 0, 8, 0);
	item_window = UiWindow_CreateFar(16, 11, 14, 9, 2);
	redraw = 1;

	for (;;)
	{
		if (ask_again != 0)
		{
			ask_again = 0;
			UiMessage_ShowAndWait((s32)MsgWhoGetsThis);
			redraw = 1;
		}
		if (redraw != 0)
		{
			redraw = 0;
			cursor = (cursor + shop->party_member_count) % shop->party_member_count;
			member = shop->party_member_ids[cursor];
			Shop_PlaceCursor((void *)party_window, cursor * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty(party_window, cursor, shop->selected_item);
			if (Item_GetEquipmentGroupFar(shop->selected_item) == 0)
				Shop_DrawUnitItem(item_window, member, shop->selected_item);
			else
				Shop_DrawEquipComparison(item_window, member, shop->selected_item);
		}
		if ((gKeyState & KEY_A) != 0)
		{
			slot = Inventory_AddItemFar(member, shop->selected_item);
			if (slot < 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				UiWork_PushValueSlotFar(member, 1);
				UiWork_PushValueSlotFar(shop->selected_item, 2);
				if (Inventory_CountFar(member) == 15)
					UiMessage_ShowAndWait((s32)MsgPackFull);
				else
					UiMessage_ShowAndWait((s32)MsgCannotCarryItem);
				continue;
			}
			Inventory_RemoveFar(member, slot);
			if ((u32)item->price > gGameState.coins)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				UiMessage_ShowAndRestoreState((s32)MsgBuyNotEnoughMoney);
				result = -1;
				goto party_done;
			}
			if (Item_IsCompatibleWithOwnerFar(member, shop->selected_item) == 0)
			{
				UiWork_PushValueSlotFar(member, 1);
				UiMessage_ShowAndWait((s32)MsgCannotEquipStillWant);
				if (UiMessage_ShowChoice(0) != 0)
				{
					ask_again = 1;
					continue;
				}
			}
			AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
			WaitFrames(1);
			quantity = Shop_SelBuyNum(member, shop->selected_item);
			ask_again = 1;
			if (quantity == -1)
				continue;
			Shop_BuyDone(member, shop->selected_item, quantity);
			Shop_BuySpecialItem((void *)party_window, item_window);
			result = 0;
			goto party_done;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			result = -1;
			goto party_done;
		}
		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			cursor--;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			cursor++;
			redraw = 1;
		}
		WaitFrames(1);
	}

party_done:
	Menu_ReleaseEntryObjectsFar();
	UiWork_FinalizeFar(item_window, 2);
	UiWork_FinalizeFar(party_window, 2);
	WaitFrames(1);
	if (result == 0 && shop->party_action == 2)
	{
		for (i = 0; i < quantity; i++)
			Item_AdjustCounterFar(shop->selected_item, -1);
		if (AbilityMenu_BuildAvailableList() == 0)
			goto close;
		if (selected > shop->stock_count - 1)
			selected = shop->stock_count - 1;
	}
	UiMessage_ShowAndWait((s32)MsgBuyAnythingElse);
	goto next_page;

close:
	UiWork_FinalizeFar(price_window, 2);
	UiWork_FinalizeFar(shop->money_window, 2);
	WaitFrames(1);
	return 0;
}

void Shop_DrawStock(s32 window, s32 selected)
{

	struct ShopRuntime	  *shop;
	s16					  *item_ids;
	s16					  *item_id;
	s32					   item_count;
	s32					   first;
	s32					   slot;
	s32					   x;
	s32					   clear;
	s32					   highlight;
	s16					   stock_item;
	struct ItemDefinition *definition;
	struct RenderOutput	  *icon;

	shop = gMenuWork;
	item_ids = shop->stock_item_ids;
	item_count = shop->stock_count;
	first = selected - selected % 7;
	if (window != 0)
	{
		RenderOutput_PrepareForRedrawFar(window);
		if (first != 0)
		{
			icon = RenderOutput_CreateFar(
				shop->previous_page_icon, 0x40000000, (struct RenderInput *)window, 216, -16);
			clear = 0;
			icon->kind = clear;
			icon->active = 17;
			icon->anim_step = clear;
		}
		if (first + 7 < item_count)
		{
			icon = RenderOutput_CreateFar(
				shop->next_page_icon, 0x40000000, (struct RenderInput *)window, 216, 24);
			clear = 0;
			icon->kind = clear;
			icon->active = 15;
			icon->anim_step = clear;
		}
		slot = 0;
		if (first < (u32)item_count)
		{
			item_id = (s16 *)((first * 2) + (u32)item_ids);
			x = 16;
			for (; (u32)slot <= 6 && first < (u32)item_count; x += 32, slot++, item_id++, first++)
			{
				stock_item = *item_id;
				definition = Item_GetDirectFar(stock_item);
				icon = RenderOutput_CreateLoadedFar(
					stock_item, 1, (struct RenderInput *)window, slot * 32, 0);
				icon->sentinel = 252;
				if (first == selected)
				{
					icon->active = 9;
					highlight = 10;
					icon->anim_step = highlight;
					icon->sentinel = 253;
				}
				icon = Shop_CreatePriceSprite(definition->price, window, x, 0);
				icon->sentinel = 251;
			}
		}
	}
}

void Shop_DrawMoney(void)
{
	struct ShopRuntime *shop;
	s32					window;

	shop = gMenuWork;
	window = shop->money_window;
	if (window != 0)
	{
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar((s32)MsgYourCoins, window, 0, 0);
		UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0x20, 8);
#else
		s32 msg = (s32)MsgYourCoins;

		UiText_DrawCharacterAtOffsetFar(msg, window, 0, 0);
		UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0, 8);
		UiText_DrawCharacterAtOffsetFar(msg - 2, window, 48, 8);
#endif
	}
}

// アイテム名の下の行の x（ドット）
#if EDITION_INTERNATIONAL
#define ITEM_NOTE_X 0
#else
#define ITEM_NOTE_X 8
#endif

void Shop_DrawItemPrice(s32 window, s32 item, s32 price, s32 price_mode)
{
	s32 message;

	if (window == 0)
		goto done;
	goto draw;
cannot_buy:
	UiText_DrawCharacterAtOffsetFar((s32)MsgCannotBuyThat, window, ITEM_NOTE_X, 8);
	goto done;
show_price:
	message = (s32)MsgPrice;
	UiText_DrawCharacterAtOffsetFar(message, window, ITEM_NOTE_X, 8);
	message -= 3;
#if defined(TBS_EDITION_FR)
	UiText_DrawNumberInWindowFar(price, 5, window, 0x18, 8);
	UiText_DrawCharacterAtOffsetFar(message, window, 0x40, 8);
#elif defined(TBS_EDITION_ES)
	UiText_DrawNumberInWindowFar(price, 5, window, 0x20, 8);
	UiText_DrawCharacterAtOffsetFar(message, window, 0x50, 8);
#else
	UiText_DrawNumberInWindowFar(price, 5, window, 0x20, 8);
	UiText_DrawCharacterAtOffsetFar(message, window, 0x48, 8);
#endif
	goto done;
draw:
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	UiText_DrawCharacterAtOffsetFar(item + (s32)MsgItemName, window, 0, 0);
	if (price != 0)
		goto show_price;
	switch (price_mode)
	{
	case 1:
		goto cannot_buy;
	case 2:
		UiText_DrawCharacterAtOffsetFar((s32)MsgNotBroken, window, ITEM_NOTE_X, 8);
		break;
	default:
		goto show_price;
	}
done:
	return;
}

//--------------------------------------------------------------------------
// win を描きなおし、メッセージ mes を左上から書く（win が 0 なら何もしない）
// 日本版は描きなおしたあと 1 フレーム待つ
//--------------------------------------------------------------------------
void Shop_DrawMsg(s32 win, s32 mes)
{
	if (win != 0)
	{
#if EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar((struct UiWindow *)win);
		UiText_DrawCharacterAtOffsetFar(mes, win, 0, 0);
#else
		RenderOutput_ClearListFar((struct UiWindow *)win);
		WaitFrames(1);
		UiText_DrawResourceFar(mes, win, 0, 0);
#endif
	}
}

void Shop_DrawParty(s32 window, s32 selected, s32 requirement)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					index;

	if (window != 0)
	{
		for (index = 0; index < shop->party_member_count; index++)
		{
			if (index == selected)
				AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 30);
			else
				AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 1);
			shop->party_member_scale[index] = 0x10000;
			if (!Item_IsCompatibleWithOwnerFar(shop->party_member_ids[index], requirement))
				shop->party_member_scale[index] = 0xcccc;
		}
	}
}

void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id)
{
	struct ShopRuntime	  *shop = gMenuWork;
	struct BtlUnit		  *unit = UnitGetFar(unit_id);
	struct ItemDefinition *item = Item_GetDirectFar(item_id);
	s32					   replaced = -1;
	s32					   slot;
	s32					   saved;
	s32					   i;
	s32					   without[4];
	s32					   with[4];

	if (window == 0)
		return;
	RenderOutput_PrepareForRedrawFar(window);
	if (!Item_CanOwnerEquip(unit_id, item_id))
	{
		UiText_DrawResourceFar((s32)MsgShopCannotEquip, window, 8, 24);
		return;
	}
	slot = Inventory_FindEquippedFar(unit_id, item->type);
	if (slot == -1)
	{
		for (i = 0; i <= 14; i++)
		{
			if (!(unit->inventory[i] & 0x200))
				break;
		}
		if (i == 15)
		{
			for (i = 0; i <= 14; i++)
			{
				if (Item_GetDirectFar(unit->inventory[i])->type == 6)
					break;
			}
			if (i == 15)
				i = 0;
		}
		slot = i;
	}
	else
	{
		replaced = unit->inventory[slot] & 0x1ff;
	}
	saved = unit->inventory[slot];
	unit->inventory[slot] = item_id | 0x200;
	UnitCalcFar(unit_id);
	with[0] = unit->atk;
	with[1] = unit->def;
	with[2] = unit->agi;
	with[3] = unit->luck;
	unit->inventory[slot] = saved;
	UnitCalcFar(unit_id);
	without[0] = unit->atk;
	without[1] = unit->def;
	without[2] = unit->agi;
	without[3] = unit->luck;
	for (i = 0; i <= 2; i++)
	{
		if (without[i] > with[i])
			RenderOutput_CreateFar(
				shop->stat_down_icon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)
				->kind = 0;
		else if (without[i] < with[i])
			RenderOutput_CreateFar(
				shop->stat_up_icon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)
				->kind = 0;
		UiText_DrawNumberInWindowFar(without[i], 3, window, 32, i * 16);
		if (without[i] != with[i])
			UiText_DrawNumberInWindowFar(with[i], 3, window, 72, i * 16);
		UiText_DrawCharacterAtOffsetFar((s32)MsgShopStatLabel + i, window, 0, i * 16);
		UiWindow_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
	}
	if (replaced != -1)
		UiText_DrawCharacterAtOffsetFar(replaced + (s32)MsgItemName, window, 0, 48);
}
void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id)
{

	u8 *unit;
	s32 x;
	s32 y;
	s32 item_index;
	s32 slot;
	s32 off;
	s32 first_offset;
	s32 item_offset;
	s32 next_offset;
	u8 *icon;

	unit = (u8 *)UnitGetFar(unit_id);
	x = 8;
	y = 8;
	if (window != 0)
	{
		RenderOutput_PrepareForRedrawFar(window);
		slot = Inventory_FindFar(unit_id, item_id);
		if (slot != -1)
		{
			off = slot * 2 + 216;
			UiWork_PushValueSlotFar((*(u16 *)(unit + off) >> 11) + 1, 5);
			UiText_DrawCharacterAtOffsetFar((s32)MsgYouHave, window, 0, 0);
		}
		else
		{
			UiText_DrawCharacterAtOffsetFar((s32)MsgShopNoneInStock, window, 0, 0);
		}
		item_index = 0;
		first_offset = 216;
		if (*(u16 *)(unit + first_offset) != 0)
		{
			for (;;)
			{
				item_offset = item_index * 2 + 216;
				icon = (u8 *)RenderOutput_CreateLoadedFar(
					*(u16 *)(unit + item_offset), 27, (struct RenderInput *)window, x, y);
				icon[15] = 252;
				x += 16;
				if (item_index == 4)
				{
					x = 8;
					y += 16;
				}
				if (item_index == 9)
				{
					x = 8;
					y += 16;
				}
				item_index++;
				if (item_index > 14)
					break;
				next_offset = item_index * 2 + 216;
				if (*(u16 *)(unit + next_offset) == 0)
					break;
			}
		}
	}
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define ACTION_Y 120
#else
#define ACTION_Y 128
#endif

s32 Shop_SelBuyNum(s32 unit_id, s32 item_id)
{
	struct ShopRuntime	  *shop;
	struct BtlUnit		  *unit;
	struct ItemDefinition *item;
	s32					   quantity;
	s32					   chance;
	s32					   slot;
	s32					   maximum;
	s32					   result;

	shop = gMenuWork;
	unit = UnitGetFar(unit_id);
	item = Item_GetDirectFar(item_id);
	result = 1;
	if (item->flags & 0x10)
	{
		UiMessage_ShowAndWait((s32)MsgHowManyToBuy);
		slot = Inventory_FindFar(unit_id, item_id);
		if (slot != -1)
		{
			quantity = (unit->inventory[slot] >> 11) + 1;
		}
		else
		{
			quantity = 0;
		}

		chance = 30;
		if (item->price != 0)
			chance = (u32)gGameState.coins / item->price;

		if (shop->party_action == 2)
		{
			maximum = Item_AdjustCounterFar(item_id, 0);
			if (chance > maximum)
				maximum = Item_AdjustCounterFar(item_id, 0);
			else
				maximum = chance;
			chance = maximum;
		}

		chance += quantity;
		if (chance > 30)
			chance = 30;

		shop->mode = 12;
		Shop_PlaceCursor(0, ACTION_Y, 0x30);
		result = Shop_SelectQuantity(quantity, chance, item->price);
	}
	return result;
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define QUANTITY_X 64
#define COINS_X 128
#elif defined(TBS_EDITION_ES)
#define QUANTITY_X 72
#define COINS_X 144
#else
#define QUANTITY_X 72
#define COINS_X 136
#endif

s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price)
{
	void				*buffer;
	s32					 window;
	s32					 changed;
	s32					 slot;
	s32					 count;
	s32					 result;
	struct RenderOutput *sprite;
#if !EDITION_INTERNATIONAL
	s32 msg;
#endif

	buffer = Runtime_AllocateBlock(14, 0x400);
	changed = 1;
	maximum -= minimum;
	result = -1;
	count = 0;
	window = UiWindow_CreateFar(7, 4, 23, 3, 2);
	if (window != 0)
	{
		slot = Resource_FindFreeEntry();
		if (slot != 96)
		{
			VramBlock_LoadCached(slot, 0x100, 0);
			RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 0, 0);
			sprite = RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 32, 0);
			sprite->oam.index += 4;
			for (;;)
			{
				if (gKeyState & KEY_A)
				{
					AudioCommand_PlayFar(112);
					result = count + 1;
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
					changed = 1;
					count--;
				}
				if (gKeysRepeat & KEY_RIGHT)
				{
					AudioCommand_PlayFar(111);
					changed = 1;
					count++;
				}
				if (changed)
				{
					changed = 0;
					count = (count + maximum) % maximum;
					Dma_Set(Shop_QuantityTiles, buffer, 0x84000040, REG_DMA3);
					Shop_FillSelector(30, 14, buffer);
					Shop_FillSelector(minimum + maximum, 0, buffer);
					Shop_FillSelector(minimum + count + 1, 10, buffer);
					Shop_FillSelector(minimum, 2, buffer);
					VramBlock_LoadCached(slot, 0x100, buffer);
					result = count + 1;
#if EDITION_INTERNATIONAL
					UiText_DrawNumberInWindowFar(result, 2, window, QUANTITY_X, 0);
					UiText_DrawNumberInWindowFar(price * result, 6, window, QUANTITY_X + 16, 0);
					UiText_DrawCharacterAtOffsetFar((s32)MsgCoins, window, COINS_X, 0);
#else
					UiText_DrawNumberInWindowFar(result, 2, window, 64, 0);
					msg = (s32)MsgPiece;
					UiText_DrawCharacterAtOffsetFar(msg, window, 80, 0);
					UiText_DrawNumberInWindowFar(price * result, 6, window, 96, 0);
					UiText_DrawCharacterAtOffsetFar(msg - 1, window, 144, 0);
#endif
				}
				WaitFrames(1);
			}
			WaitFrames(1);
			UiWork_FinalizeFar(window, 2);
		}
	}
	Runtime_ReleaseHeapBlock(14);
	return result;
}

void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity)
{
	s32					   replaced_slot;
	s32					   remaining;
	s32					   added_slot;
	struct ItemDefinition *item;

	remaining = quantity;
	item = Item_GetDirectFar(item_id);
	added_slot = 0;
	replaced_slot = Inventory_FindEquippedFar(unit_id, item->type);
	AudioCommand_PlayFar(SOUND_SHOP_PURCHASE);
	if (added_slot < remaining)
	{
		do
		{
			added_slot = Inventory_AddItemFar(unit_id, item_id);
			Party_AdjustCoinsFar(0 - item->price);
			remaining -= 1;
			Party_AdjustSixDigitCounterBFar(item->price);
			Shop_DrawMoney();
		} while (remaining != 0);
	}
	UiMessage_ShowAndRestoreState((s32)MsgHereYouGo);
	if (Shop_ConfirmEquip(unit_id, added_slot) != 0)
	{
		Shop_SellOld(unit_id, replaced_slot);
	}
}

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
	struct ShopRuntime	  *menu = gMenuWork;
	struct BtlUnit		  *unit = UnitGetFar(unit_id);
	s32					   item_id = unit->inventory[slot] & 0x1ff;
	struct ItemDefinition *info = Item_GetDirectFar(item_id);
	s32					   replaced;
	s32					   menu_value;

	if (unit->inventory[slot] & 0x200)
		return 0;

	if (Item_CanOwnerEquip(unit_id, item_id) == 0)
		return 0;

	replaced = Inventory_FindEquippedFar(unit_id, info->type);
	if (replaced != -1)
	{
		struct ItemDefinition *old_info = Item_GetDirectFar(unit->inventory[replaced]);

		if (old_info->flags & 2)
			return 0;
	}

	UiWork_PushValueSlotFar(unit_id, 1);
	UiMessage_ShowAndWait((s32)MsgEquipNowPrompt);
	if (UiMessage_ShowChoice(0) != 0)
		return 0;

	Inventory_EquipFar(unit_id, slot);
	menu_value = menu->item_window;
	if (menu_value != 0)
		Shop_DrawUnitGrid(menu_value, unit_id);

	if (info->flags & 1)
	{
		AudioCommand_PlayFar(103);
		UiWork_FinalizePendingCoreFar();
		UiText_OpenMessageWindowFar((s32)MsgBecameCursed, 8, 4, 2);
		while (UiWork_IsCompleteFar() == 0)
		{
			WaitFrames(1);
		}
	}

	UiMessage_ShowAndRestoreState((s32)MsgLookBolder);
	return 1;
}

s32 Shop_SellOld(s32 unit_id, s32 slot)
{
	struct BtlUnit *unit;
	s32				item_id;

	unit = UnitGetFar(unit_id);
	if (slot == -1)
	{
		return 0;
	}
	item_id = unit->inventory[slot] & 0x1ff;
	if (Item_GetDirectFar(item_id)->type == 6)
	{
		return 0;
	}
	if (Item_GetDirectFar(item_id)->flags & 8)
	{
		return 0;
	}
	Shop_SellItem(unit_id, slot, -1);
	return 1;
}

s32 Shop_SalePrice(s32 item_id)
{
	s32 price;

	price = Item_GetDirectFar(item_id)->price;

	if (Item_GetDirectFar(item_id)->flags & 8)
	{
		price = 0;
	}
	else if (item_id & 0x400)
	{
		price = price / 2;
	}
	else
	{
		price = price * 3 / 4;
	}
	return price;
}

#if EDITION_INTERNATIONAL
#define BASE_W 12
#else
#define BASE_W 11
#endif

s32 Shop_PickUnit(void)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					list_window;
	s32					selection = 0;
	s32					redraw = 1;
	s32					unit_id = 0;

	shop->money_window = UiWindow_CreateFar(0, 9, BASE_W, 4, 2);
	Shop_DrawMoney();
	shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
	list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
	shop->cursor.anchor->active = 4;
	shop->mode = 12;
	PsynergyMenu_InitializeEntryObjectsFar(list_window, 2, 0, 8, 0);

	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			selection = (selection + shop->party_member_count) % shop->party_member_count;
			unit_id = shop->party_member_ids[selection];
			Shop_PlaceCursor((void *)list_window, selection * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty(list_window, selection, 0);
			Shop_DrawUnitGrid(shop->item_window, unit_id);
		}

		if ((gKeyState & KEY_A) != 0)
		{
			WaitFrames(1);
			if (Inventory_CountFar(unit_id) == 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			}
			else
			{
				AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
				if (shop->party_action == 1)
					Shop_SelSell(unit_id);
				else
					Shop_SelRepair(unit_id);
				shop->cursor.anchor->active = 4;
				shop->mode = 12;
				redraw = 1;
			}
			continue;
		}

		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			Menu_ReleaseEntryObjectsFar();
			UiWork_FinalizeFar(list_window, 2);
			UiWork_FinalizeFar(shop->item_window, 2);
			UiWork_FinalizeFar(shop->money_window, 2);
			WaitFrames(1);
			return 0;
		}

		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			selection--;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			selection++;
			redraw = 1;
		}
		WaitFrames(1);
	}
}

s32 Shop_SelSell(s32 unit_id)
{
	s32					price_window;
	struct ShopRuntime *shop;
	s32					list_window;
	struct BtlUnit	   *unit;
	s32					selection;
	s32					item_count;
	s32					redraw;
	s32					result;
	s32					item_id;
	s32					quantity;
	void			   *window;
	s32					x;

	shop = gMenuWork;
	unit = UnitGetFar(unit_id);
	item_count = 1;
	list_window = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
	selection = 0;

	for (;;)
	{
		price_window = UiWindow_CreateFar(0, 5, 30, 3, 2);
		shop->cursor.anchor->active = 18;
		shop->mode = 12;
		redraw = 1;

		for (;;)
		{
			if (redraw != 0)
			{
				redraw = 0;
				item_count = Inventory_CountFar(unit_id);
				if (selection > item_count - 1)
					selection = item_count - 1;
				item_id = 0x1ff & unit->inventory[selection];
				window = (void *)shop->item_window;
				x = selection % 5 * 16;
				Shop_PlaceCursor(window, x, selection / 5 * 16 + 8);
				shop->mode = 3;
				Shop_DrawItemPrice(
					list_window, item_id, Shop_SalePrice(unit->inventory[selection]), 1);
				Shop_DrawMsg(price_window, item_id + (s32)MsgItemPlainName);
			}

			if ((gKeyState & KEY_A) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
				result = 0;
				goto done;
			}
			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				result = -1;
				goto done;
			}
			if ((gKeysRepeat & KEY_LEFT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection -= 1;
				selection = (selection + item_count) % item_count;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_RIGHT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection += 1;
				selection = (selection + item_count) % item_count;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_UP) != 0)
			{
				selection -= 5;
				if (selection < 0)
					selection += 15;
				while (selection >= item_count)
					selection -= 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_DOWN) != 0)
			{
				selection += 5;
				if (selection >= item_count)
					selection -= 15;
				while (selection < 0)
					selection += 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			WaitFrames(1);
		}
	done:
		UiWork_FinalizeFar(price_window, 2);
		WaitFrames(1);
		if (result != 0)
			break;

		quantity = Shop_SelSellNum(unit_id, selection);
		if (quantity != -1)
			Shop_SellItem(unit_id, selection, quantity);
		UiMessage_ShowAndWait((s32)MsgSellAnythingElse);
		if (Inventory_CountFar(unit_id) == 0)
			break;
	}

	UiWork_FinalizeFar(list_window, 2);
	return result;
}

void Shop_DrawUnitGrid(s32 window, s32 unit_id)
{
	struct BtlUnit		*unit;
	s32					 x;
	s32					 y;
	s32					 slot;
	struct RenderOutput *icon;

	unit = UnitGetFar(unit_id);
	x = 8;
	y = 0;
	if (window != 0)
	{
		RenderOutput_PrepareForRedrawFar(window);
		if (Inventory_CountFar(unit_id) == 0)
		{
#if defined(TBS_EDITION_IT)
			UiText_DrawResourceFar((s32)MsgNoItems, window, 20, 12);
#else
			UiText_DrawResourceFar((s32)MsgNoItems, window, 8, 20);
#endif
		}
		else
		{
			slot = 0;
			do
			{
				if (unit->inventory[slot] != 0)
				{
					icon = RenderOutput_CreateLoadedFar(
						unit->inventory[slot], 27, (struct RenderInput *)window, x, y);
					icon->sentinel = 252;
				}
				x += 16;
				if (slot == 4)
				{
					x = 8;
					y += 16;
				}
				if (slot == 9)
				{
					x = 8;
					y += 16;
				}
				slot++;
			} while (slot <= 14);
		}
	}
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define EFFECT_X 0x78
#else
#define EFFECT_X 0x80
#endif

s32 Shop_SelSellNum(s32 unit_id, s32 slot)
{
	s32					   result;
	s16					   saved_x;
	s16					   saved_y;
	s32					   effect;
	s32					   state;
	s32					   selection;
	struct ItemDefinition *item;
	struct ShopRuntime	  *shop;
	struct BtlUnit		  *unit;

	shop = gMenuWork;
	unit = UnitGetFar(unit_id);
	item = Item_GetDirectFar(unit->inventory[slot]);
	result = 1;
	effect = Shop_SalePrice(unit->inventory[slot]);
	state = Shop_GetSelectionState(unit_id, slot);
	selection = state;
	if ((item->flags & 0x10) && state > 1)
	{
		UiMessage_ShowAndWait((s32)MsgHowManyToSell);
		saved_x = shop->cursor.target_x;
		saved_y = shop->cursor.target_y;
		shop->cursor.anchor->active = 4;
		shop->mode = 0xc;
		Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
		result = Shop_SelectQuantity(0, selection, effect);
		WaitFrames(1);
		UiIcon_PrepareObjectFar(shop->cursor.anchor);
		Shop_PlaceCursor(NULL, saved_x, saved_y);
	}
	return result;
}

void Shop_SellItem(s32 unit_id, s32 slot, s32 count)
{
	struct ShopRuntime	  *shop = gMenuWork;
	struct BtlUnit		  *unit = UnitGetFar(unit_id);
	s32					   item_id = unit->inventory[slot] & 0x1ff;
	struct ItemDefinition *item = Item_GetDirectFar(item_id);
	u8					   rare = item->flags & 4;
	s32					   all = 0;
	s32					   total;
	s32					   message;
	s32					   i;

	if (count == -1)
	{
		all = 1;
		count = 1;
	}
	total = Shop_SalePrice(unit->inventory[slot]) * count;
	if (total == 0)
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState((s32)MsgWeDoNotBuy);
		return;
	}
	if ((unit->inventory[slot] & 0x200) && (item->flags & 2))
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState((s32)MsgCannotSellEquipped);
		return;
	}
	if (all)
		message = (s32)MsgOldItemSellOffer;
	else if (unit->inventory[slot] & 0x400)
		message = (s32)MsgDamagedSellOffer;
	else if (count > 1)
		message = (s32)MsgSellTotal;
	else if (rare)
		message = (s32)MsgRareSellOffer;
	else
		message = (s32)MsgSellOffer;
	UiWork_PushValueSlotFar(item_id, 2);
	UiWork_PushValueSlotFar(total, 5);
	UiMessage_ShowAndRestoreState(message);
	if (UiMessage_ShowChoice(0) != 0)
	{
		if (rare || all)
			message = (s32)MsgKeepIt;
		else
			message = (s32)MsgChangedMind;
		UiMessage_ShowAndRestoreState(message);
		return;
	}
	AudioCommand_PlayFar(102);
	for (i = 0; i < count; i++)
		Inventory_DiscardFar(unit_id, slot);
	Party_AdjustCoinsFar(total);
	Shop_DrawMoney();
	Shop_DrawUnitGrid(shop->item_window, unit_id);
	if (rare || all)
		message = (s32)MsgSold;
	else
		message = (s32)MsgDeal;
	UiMessage_ShowAndRestoreState(message);
}

s32 Shop_RepairPrice(s32 item_id)
{
	s32 result = Item_GetDirectFar(item_id)->price / 4;

	if ((item_id & 0x400) == 0)
	{
		result = 0;
	}
	return result;
}

#define SPECIAL_ITEM 228

s32 Shop_SelUnit(void)
{
	Shop_PickUnit();
	return 0;
}

s32 Shop_SelRepair(s32 unit_id)
{
	struct ShopRuntime *shop;
	s32					item_count;
	s32					price_window;
	s32					list_window;
	struct BtlUnit	   *unit;
	s32					selection;
	s32					redraw;
	s32					result;
	s32					item_id;
	s32					price;
	void			   *window;
	s32					x;
	s32					y;

	shop = gMenuWork;
	unit = UnitGetFar(unit_id);
	item_count = 1;
	list_window = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
	selection = 0;

	for (;;)
	{
		price_window = UiWindow_CreateFar(0, 5, 30, 3, 2);
		shop->cursor.anchor->active = 18;
		shop->mode = 12;
		redraw = 1;

		for (;;)
		{
			if (redraw != 0)
			{
				redraw = 0;
				item_count = Inventory_CountFar(unit_id);
				if (selection > item_count - 1)
					selection = item_count - 1;
				item_id = 0x1ff & unit->inventory[selection];
				window = (void *)shop->item_window;
				x = selection % 5 * 16;
				y = selection / 5 * 16 + 8;
				Shop_PlaceCursor(window, x, y);
				shop->mode = 3;
				price = Shop_RepairPrice(unit->inventory[selection]);
				Shop_DrawItemPrice(list_window, item_id, price, 2);
				Shop_DrawMsg(price_window, item_id + (s32)MsgItemPlainName);
			}
			if ((gKeyState & KEY_A) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
				result = 0;
				goto done;
			}
			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				result = -1;
				goto done;
			}
			if ((gKeysRepeat & KEY_LEFT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection -= 1;
				selection = (selection + item_count) % item_count;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_RIGHT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection += 1;
				selection = (selection + item_count) % item_count;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_UP) != 0)
			{
				selection -= 5;
				if (selection < 0)
					selection += 15;
				while (selection >= item_count)
					selection -= 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_DOWN) != 0)
			{
				selection += 5;
				if (selection >= item_count)
					selection -= 15;
				while (selection < 0)
					selection += 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			WaitFrames(1);
		}
	done:

		UiWork_FinalizeFar(price_window, 2);
		WaitFrames(1);
		if (result != 0)
			break;

		Shop_RepairItem(unit_id, selection);
		UiMessage_ShowAndWait((s32)MsgRepairAnythingElse);
		if (Inventory_CountFar(unit_id) == 0)
			break;
	}

	UiWork_FinalizeFar(list_window, 2);
	return result;
}

void Shop_RepairItem(s32 unit_id, s32 slot)
{
	struct ShopRuntime	  *shop = gMenuWork;
	struct BtlUnit		  *unit = UnitGetFar(unit_id);
	s32					   item_id = unit->inventory[slot] & 0x1ff;
	struct ItemDefinition *item = Item_GetDirectFar(item_id);
	s32					   equipped = Inventory_FindEquippedFar(unit_id, item->type);
	u32					   price = Shop_RepairPrice(unit->inventory[slot]);
	s32					   message;
	u8					   kind;
	u32					   saved;

	kind = item->use_type;
	if (kind != 2)
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState((s32)MsgCannotRepairKind);
		return;
	}
	if (!(unit->inventory[slot] & 0x400))
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState((s32)MsgNotDamaged);
		return;
	}
	if ((unit->inventory[slot] & 0x200) && (kind & item->flags))
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState((s32)MsgCursedWontComeOff);
		return;
	}
	if (price > gGameState.coins)
	{
		UiMessage_ShowAndRestoreState((s32)MsgNotEnoughMoney);
		return;
	}
	UiWork_PushValueSlotFar(item_id, 2);
	UiWork_PushValueSlotFar(price, 5);
	message = (s32)MsgShopRepairConfirm;
	UiMessage_ShowAndRestoreState(message);
	if (UiMessage_ShowChoice(0) != 0)
	{
		UiMessage_ShowAndRestoreState(message + 1);
		return;
	}
	saved = unit->inventory[slot];
	unit->inventory[slot] = 0;
	Shop_DrawUnitGrid(shop->item_window, unit_id);
	UiWork_PushValueSlotFar(item_id, 2);
	UiMessage_ShowAndRestoreState(message + 2);
	UiWork_FinalizePendingCoreFar();
	WaitFrames(10);
	AudioCommand_PlayFar(100);
	WaitFrames(110);
	AudioCommand_PlayFar(100);
	WaitFrames(110);
	AudioCommand_PlayFar(100);
	WaitFrames(110);
	AudioCommand_PlayFar(112);
	WaitFrames(20);
	unit->inventory[slot] = saved;
	Inventory_RepairFar(unit_id, slot);
	Party_AdjustCoinsFar(-price);
	Shop_DrawMoney();
	Shop_DrawUnitGrid(shop->item_window, unit_id);
	UiWork_PushValueSlotFar(item_id, 2);
	UiMessage_ShowAndRestoreState(message + 3);
	if (Shop_ConfirmEquip(unit_id, slot))
		Shop_SellOld(unit_id, equipped);
}

void Shop_BuySpecialItem(void *window, s32 item_window)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					price;
	u32					saved;
	s32					redraw;
	s32					unit_id;
	s32					selected_index;
	s32					message;
	s32					slot;

	redraw = 1;
	unit_id = 0;
	saved = shop->selected_item;
	price = Shop_SpecialItemPrices[gGameState.shop_gifts];
	selected_index = 0;

	if (price > gGameState.shop_credit)
		return;
	shop->selected_item = SPECIAL_ITEM;
	UiWork_PushValueSlotFar(SPECIAL_ITEM, 2);
	message = (s32)MsgTokenGift;
	UiMessage_ShowAndRestoreState(message);
	UiWork_PushValueSlotFar(shop->selected_item, 2);
	UiMessage_ShowAndRestoreState(message + 1);

	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			selected_index = (selected_index + shop->party_member_count) % shop->party_member_count;
			unit_id = shop->party_member_ids[selected_index];
			Shop_PlaceCursor(window, selected_index * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty((s32)window, selected_index, shop->selected_item);
			Shop_DrawUnitItem(item_window, unit_id, shop->selected_item);
		}
		if ((gKeyState & KEY_A) != 0)
		{
			slot = Inventory_AddItemFar(unit_id, shop->selected_item);
			if (slot < 0)
			{
				AudioCommand_PlayFar(0x71);
				UiWork_PushValueSlotFar(unit_id, 1);
				UiWork_PushValueSlotFar(shop->selected_item, 2);
				if (Inventory_CountFar(unit_id) == 15)
					UiMessage_ShowAndWait((s32)MsgPackFull);
				else
					UiMessage_ShowAndWait((s32)MsgCannotCarryItem);
				continue;
			}
			Inventory_RemoveFar(unit_id, slot);
			AudioCommand_PlayFar(0x65);
			UiMessage_ShowAndRestoreState((s32)MsgHereYouGo);
			Inventory_AddItemFar(unit_id, shop->selected_item);
			Party_AdjustSixDigitCounterBFar(-price);
			Party_AdjustCounterCappedAt28Far(1);
			goto done;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			UiMessage_ShowAndRestoreState((s32)MsgDontWantIt);
			AudioCommand_PlayFar(0x71);
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
		WaitFrames(1);
	}
done:
	shop->selected_item = saved;
}
