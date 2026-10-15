#include "unit/shop/shop.h"
#include "unit/runtime/runtime.h"
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
#include "battle_system.h"
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
#include "battle_calc.h"
#include "field_obj.h"

s32 AbilityMenu_BuildAvailableList(void)
{
	struct ShopRuntime *state;
	s16				   *output;
	s32					index;
	s32					count;
	s8					mode;

	state = gMenuWork;
	count = 0;
	index = 0;
	output = state->stockItemIds;
	do
	{
		mode = state->shopType;
		if (mode == Item_GetEquipmentGroupFar(index) && Item_AdjustCounterFar(index, 0) != 0)
		{
			*output = index;
			count++;
			output++;
		}
		index++;
	} while (index <= 0x1ff);
	state->stockItemIds[count] = 0;
	state->stockCount = count;
	return count;
}

void Shop_StepCursor(void)
{
	ShopCursor_Advance(&((struct ShopRuntime *)gMenuWork)->cursor);
}

void Shop_InitializeCursorWork(void)
{
	struct ShopRuntime *work;
	s32					slot;

	work = Runtime_AllocateHeapBlock(55, 0xa70);
	Battle_ResetEffectCounterFar();
	Dma_Fill32(0, work, 0x29c * 4);
	work->mode = 12;
	work->partyMemberCount = Party_ListActiveOwnersFar(work->partyMemberIds);
	slot = Resource_FindFreeEntry();
	work->cursorIcon = slot;
	VramBlock_LoadCached(slot, 128, gShop_HandTiles);
	slot = Resource_FindFreeEntry();
	work->previousPageIcon = slot;
	VramBlock_LoadCached(slot, 128, gShop_UpArrowTiles);
	slot = Resource_FindFreeEntry();
	work->nextPageIcon = slot;
	VramBlock_LoadCached(slot, 128, gShop_DownArrowTiles);
	slot = Resource_FindFreeEntry();
	work->gemIcon = slot;
	VramBlock_LoadCached(slot, 128, gShop_GemTiles);
	slot = Resource_FindFreeEntry();
	work->statDownIcon = slot;
	VramBlock_LoadCached(slot, 128, gShop_SmallDownArrowTiles);
	slot = Resource_FindFreeEntry();
	work->statUpIcon = slot;
	VramBlock_LoadCached(slot, 128, gShop_SmallUpArrowTiles);
	Scheduler_AddOrUpdateCallback(Shop_StepCursor, 0xc80);
}

void Inn_Cleanup(void)
{
	struct ShopRuntime *state;

	state = gMenuWork;
	Scheduler_RemoveCallback(Shop_StepCursor);
	UiWork_FinalizePendingCoreFar();
	Resource_ResetEntry(state->cursorIcon);
	Resource_ResetEntry(state->previousPageIcon);
	Resource_ResetEntry(state->nextPageIcon);
	Resource_ResetEntry(state->gemIcon);
	Resource_ResetEntry(state->statUpIcon);
	Resource_ResetEntry(state->statDownIcon);
	Runtime_ReleaseHeapBlock(0x37);
}

s32 Shop_Run(s32 row, s32 keeper_id)
{
	struct ShopRuntime	*shop;
	struct RenderOutput *anchor;
	s32					 window;
	s32					 choice = 0;

	if (row >= EventTable_GetRowLimit() || row < 0)
		row = 0;
	EventTable_ApplyRowAbilities(row);
	Shop_InitializeCursorWork();
	shop = gMenuWork;
	shop->shopType = EventTable_GetRowType(row);
	if (row == 16)
		shop->warriorShop = 1;
	if (row == 17)
		shop->warriorShop = 1;
	if (row == 18)
		shop->warriorShop = 1;
	shop->keeperResource =
		((struct AnimationObject *)((struct ObjectRuntime *)Object_GetByIdFar(keeper_id))
				->animation)
			->entries[0]
			->animId;
	window = UiWindow_CreateWithSideObjectFar(shop->keeperResource, 0, 0, 0);
	if (window == 0)
		window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
	anchor =
		RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, (struct RenderInput *)window, 0, 0);
	anchor->active = 1;
	anchor->kind = 0;
	ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = anchor;
	UiMessage_ShowAndWait(MsgWeaponShopWelcome);
	while (1)
	{
		choice = Menu_SelectEntry11To14Far(choice);
		shop->partyAction = choice;
		if (choice == 0)
		{
			shop->stockCount = EventTable_CopyRowHeader(row, shop->stockItemIds);
			UiMessage_ShowAndWait(MsgWhatWouldYouLike);
			Shop_SelectBuy();
		}
		else if (choice == 1)
		{
			UiMessage_ShowAndWait(MsgWhatToSell);
			Shop_PickUnit();
		}
		else if (choice == 2)
		{
			if (AbilityMenu_BuildAvailableList() != 0)
			{
				UiMessage_ShowAndWait(MsgConnoisseur);
				Shop_SelectBuy();
			}
			else
			{
				UiMessage_ShowAndWait(MsgOutOfStock);
				WaitFrames(1);
			}
		}
		else if (choice == 3)
		{
			UiMessage_ShowAndWait(MsgFixDamaged);
			Shop_SelectUnit();
		}
		else
		{
			break;
		}
		ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
		UiMessage_ShowAndWait(MsgAnythingElse);
	}
	UiMessage_ShowAndWait(MsgShopFarewell);
	UiWork_FinalizeFar(window, 2);
	Inn_Cleanup();
	return 0;
}

#if EDITION_INTERNATIONAL
#define FINAL_ARG 1
#else
#define FINAL_ARG 2
#endif

s32 Battle_ApplyPresetItemsAndFlags(s32 unused)
{
	gGameState.coins = 0x30d40;
	gGameState.shopGifts = 0x1c;
	Inventory_EquipFar(1, Inventory_AddItemFar(1, 0x48d));
	Inventory_EquipFar(0, Inventory_AddItemFar(0, 0x40b));
	Inventory_AddItemFar(2, 0xe7);
	UnitGetFar(3)->poison = 1;
	UnitGetFar(5)->poison = 1;
	UnitGetFar(2)->haunt = 1;
	Shop_Run(FINAL_ARG, 0x1e);
	return 0;
}

void AudioCommand_WaitForStateByteClear(void)
{
	while (AudioCommand_GetStateByteFar() != 0)
	{
		WaitFrames(1);
	}
}

struct SelectorRecord
{
	u8 filler0[4];
	u8 values[21];
};

void UiMessage_ShowAndWait(s32 arg0)
{
	struct ShopRuntime *state = gMenuWork;
	s32					value = BattleFx_GetResourceIdFar(state->keeperResource);
	s32					result = arg0;
	s8					mode;

	UiWork_FinalizePendingCoreFar();
	mode = state->shopType;
	if (mode == 2)
		result += MsgArmorShopWelcome - MsgWeaponShopWelcome;
	if (mode == 0)
		result += MsgItemShopWelcome - MsgWeaponShopWelcome;
	if ((s8)state->warriorShop != 0)
		result += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
	UiText_OpenMessageWindowFar(result, 5, 0, (value << 16) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);
	WaitFrames(1);
}

void UiMessage_ShowAndRestoreState(s32 messageId)
{
	s32					  variant;
	s32					  no;
	s8					  mode;
	s8					  flag;
	u8					  saved;
	struct ShopRuntime	 *state;
	struct RenderOutput **slot;

	state = gMenuWork;
	slot = &state->cursor.anchor;
	saved = (*slot)->active;
	no = messageId;
	variant = BattleFx_GetResourceIdFar(state->keeperResource);
	mode = state->shopType;
	if (mode == 2)
	{
		no += MsgArmorShopWelcome - MsgWeaponShopWelcome;
	}
	if (mode == 0)
	{
		no += MsgItemShopWelcome - MsgWeaponShopWelcome;
	}
	flag = state->warriorShop;
	if (flag != 0)
	{
		no += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
	}
	(*slot)->active = 13;
	UiWork_FinalizePendingCoreFar();
	UiText_OpenMessageWindowFar(no, 5, 0, (variant << 0x10) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
	{
		WaitFrames(1);
	}
	WaitFrames(1);
	state->cursor.anchor->active = saved;
}

s32 UiMessage_ShowChoice(s32 arg0)
{
	struct RenderOutput **slot = &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
	u8					  saved = (*slot)->active;
	UiIcon_PrepareObjectFar(*slot);
#if defined(TBS_EDITION_DE)
	arg0 = Menu_RunConfirmSelectionAtFar(6, 5, arg0);
#else
	arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
#endif
	(*slot)->active = saved;
	return arg0;
}

s32 UiMessage_ShowChoiceVariant(s32 arg0)
{
	struct RenderOutput **slot = &((struct ShopRuntime *)gMenuWork)->cursor.anchor;
	u8					  saved = (*slot)->active;
	UiIcon_PrepareObjectFar(*slot);
	arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
	(*slot)->active = saved;
	return arg0;
}

struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *state)
{
	struct NodeChainNode *node = state->node;
	s32					  index;

	for (index = 0; index != state->count; ++index)
	{
		node = node->next;
	}
	return node;
}

// 選択枠のタイル位置
static const u16 sSelectorTileOffsets[30] =
{
	0x00, 0x01, 0x02, 0x03,
	0x20, 0x21, 0x22, 0x23,
	0x40, 0x41, 0x42, 0x43,
	0x60, 0x61, 0x62, 0x63,
	0x80, 0x81, 0x82, 0x83,
	0xa0, 0xa1, 0xa2, 0xa3,
	0xc0, 0xc1, 0xc2, 0xc3,
	0xe0, 0xe1,
};

void Shop_FillSelector(s32 count, s32 selector, u8 *base)
{
	const u16 *offset;

	selector = (selector << 4) + 1;

	if (count > 0)
	{
		offset = sSelectorTileOffsets;
		do
		{
			struct SelectorRecord *record = (struct SelectorRecord *)(base + *offset++);
			record->values[0] = selector;
			record->values[4] = selector;
			record->values[8] = selector;
			record->values[12] = selector;
			record->values[16] = selector;
			record->values[20] = selector;
			count--;
		} while (count != 0);
	}
}

// 数字の転送先
static const u16 sGlyphOffsets[5] =
{
	0xc0, 0x90, 0x80, 0x50, 0x40,
};

void Shop_CopyGlyphs(s32 arg0, u8 *arg1, u32 arg2)
{
	u8 *src = gShop_GlyphBytes + (arg0 << 5);
	u8 *dst = arg1 + sGlyphOffsets[arg2] + 2;
	s32 count = 3;

	do
	{
		if (*src != 0)
		{
			dst[0] = *src++;
			if (*src != 0)
			{
				dst[1] = *src++;
				if (*src != 0)
				{
					dst[30] = *src++;
					if (*src != 0)
						dst[31] = *src++;
				}
			}
		}
		dst += 4;
		count--;
	} while (count >= 0);
}

struct RenderOutput *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y)
{
	u8					*buf;
	s32					 slot;
	struct RenderOutput *sprite;

	buf = Runtime_AllocateBlock(14, 0x400);
	sprite = 0;
	Dma_Set(gShop_PriceTiles, buf, 0x84000040, REG_DMA3);
	Shop_CopyGlyphs(value % 10, buf, 0);
	value = value / 10;
	if (value != 0)
	{
		Shop_CopyGlyphs(value % 10, buf, 1);
		value = value / 10;
		if (value != 0)
		{
			Shop_CopyGlyphs(value % 10, buf, 2);
			value = value / 10;
			if (value != 0)
			{
				s32 last;

				Shop_CopyGlyphs(value % 10, buf, 3);
				last = value / 10;
				if (last != 0)
					Shop_CopyGlyphs(last % 10, buf, 4);
			}
		}
	}
	slot = Resource_FindFreeEntry();
	if (slot != 96)
	{
		VramBlock_LoadCached(slot, 0x100, buf);
		sprite = RenderOutput_CreateFar(slot, 0x80008000, (struct RenderInput *)window, x, y);
	}
	Runtime_ReleaseHeapBlock(14);
	return sprite;
}

void Shop_RestoreSceneTiles(s32 layer)
{
	u32 *scene = (u32 *)&gEventWork;
	u8	*work = (u8 *)scene[5];
	u8	*map = (u8 *)scene[0];
	u8	*saved = work + 0xe00;

	Dma_Set(saved, map + 0x236, 0x84000150, REG_DMA3);
	Dma_Set(saved, work + 0x380, 0x840002a0, REG_DMA3);
	BattleFx_ApplyColorToTargetBufferFar(layer, 1);
	BattleFx_StartBufferInterpolationFar(16);
}

void Shop_InitEffect(void)
{
	BattleFx_ApplyColorToTargetBufferFar((s32)gEventWork + 0x236, 1);
	BattleFx_StartBufferInterpolationFar(0x10);
}

void ShopCursor_Advance(struct ShopCursor *cursor)
{
	struct RenderOutput *sprite;
	s32					 kind;
	s32					 step;
	s32					 x;
	s32					 y;

	if (cursor == NULL)
		return;
	kind = cursor->kind;
	if (kind == 0)
		return;
	sprite = cursor->anchor;
	step = ++cursor->active;
	x = cursor->x + (cursor->targetX - (s16)cursor->x) * step / kind;
	sprite->x = x;
	sprite->oam.x = x;
	y = cursor->y + (cursor->targetY - (s16)cursor->y) * step / kind;
	sprite->y = y;
	sprite->oam.y = y;
	if (step == kind)
	{
		cursor->kind = 0;
		cursor->active = 0;
	}
}

void ShopCursor_MoveTowardTarget(struct ShopCursor *cursor)
{
	struct RenderOutput *sprite;
	s32					 delta;
	s32					 step;

	sprite = cursor->anchor;
	if (sprite != NULL)
	{
		delta = sprite->x - cursor->targetX;
		step = delta / 4;
		if (step < 0)
			step = -step;
		if (delta > 0)
		{
			if (step != 0)
				sprite->x = sprite->x - step;
			else
				sprite->x = sprite->x - 1;
			sprite->oam.x = sprite->x;
		}
		else if (delta < 0)
		{
			if (step != 0)
				sprite->x = sprite->x + step;
			else
				sprite->x = sprite->x + 1;
			sprite->oam.x = sprite->x;
		}

		delta = sprite->y - cursor->targetY;
		step = delta / 4;
		if (step < 0)
			step = -step;
		if (delta > 0)
		{
			if (step != 0)
				sprite->y = sprite->y - step;
			else
				sprite->y = sprite->y - 1;
			sprite->oam.y = sprite->y;
		}
		else if (delta < 0)
		{
			if (step != 0)
				sprite->y = sprite->y + step;
			else
				sprite->y = sprite->y + 1;
			sprite->oam.y = sprite->y;
		}
	}
}

void Shop_SetCursor(struct ShopCursor *cursor, s32 targetX, s32 targetY, s32 kind)
{
	struct RenderOutput *anchor = cursor->anchor;

	cursor->x = anchor->x;
	cursor->y = anchor->y;
	cursor->targetX = targetX;
	cursor->targetY = targetY;
	cursor->kind = kind;
	cursor->active = 0;
}

void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y)
{

	{
		struct RenderOutput *sprite = cursor->anchor;

		cursor->targetX = x;
		cursor->kind = 1;
		cursor->active = 0;
		cursor->x = x;
		sprite->oam.x = sprite->x = x;
	}
	{
		struct RenderOutput *sprite;

		cursor->targetY = y;
		cursor->y = y;
		sprite = cursor->anchor;
		sprite->oam.y = sprite->y = y;
	}
}

void Shop_PlaceCursor(void *window, s32 x, s32 y)
{
	s32					cursor_x;
	s32					cursor_y;
	struct ShopRuntime *shop;

	cursor_x = x;
	cursor_y = y;
	shop = gMenuWork;
	if (window != NULL)
	{
		cursor_x = cursor_x + (((struct RenderInput *)window)->x * 8) + 8;
		cursor_y = cursor_y + (((struct RenderInput *)window)->y * 8) + 8;
	}
	Shop_SetCursor(&shop->cursor, cursor_x, cursor_y, (s8)shop->mode);
}

s32 Shop_SelectBuy(void)
{
	struct ShopRuntime	  *shop = gMenuWork;
	s32					   price_window = 0;
	s32					   stock_window;
	s32					   messageWindow;
	s32					   party_window;
	s32					   selected = 0;
	s32					   quantity;
	struct ItemDefinition *item;
	s32					   ask_again;
	s32					   itemWindow;
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

	shop->itemWindow = 0;
	shop->moneyWindow = UiWindow_CreateFar(30 - SHOP_MONEY_WIDTH, 7, SHOP_MONEY_WIDTH, 4, 2);
	Shop_DrawMoney();
	price_window = UiWindow_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, 2);

next_page:
	cursor = selected;
	stock_window = UiWindow_CreateFar(0, 12, 30, 4, 2);
	shop->cursor.anchor->active = 18;
	shop->mode = 12;
	messageWindow = UiWindow_CreateFar(0, 17, 30, 3, 2);
	redraw = 1;

	while (1)
	{
		count = shop->stockCount;
		if (redraw != 0)
		{
			ids = shop->stockItemIds;
			item_id = ids[cursor];
			stock_item = Item_GetDirectFar(item_id);
			redraw = 0;
			Shop_PlaceCursor((void *)stock_window, cursor % 7 * 32 - 8, 8);
			shop->mode = 4;
			Shop_DrawStock(stock_window, cursor);
			Shop_DrawMsg(messageWindow, item_id + MsgItemPlainName);
			RenderOutput_RedrawSavedRectFar((struct UiWindow *)price_window);
			Shop_DrawItemPrice(price_window, item_id, stock_item->price, 0);
		}
		if ((gKeyState & KEY_A) != 0)
		{
			selected = cursor;
			AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
			result = 0;
			break;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			result = -1;
			break;
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

	UiWork_FinalizeFar(messageWindow, 2);
	UiWork_FinalizeFar(stock_window, 2);
	WaitFrames(1);
	if (result != 0)
		goto close;

	shop->selectedItem = shop->stockItemIds[selected];
	UiMessage_ShowAndWait(MsgWhoGetsThis);
	item = Item_GetDirectFar(shop->selectedItem);
	quantity = 1;
	member = 0;
	ask_again = 0;
	cursor = 0;
	party_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
	shop->cursor.anchor->active = 4;
	shop->mode = 12;
	PsynergyMenu_InitializeEntryObjectsFar(party_window, 2, 0, 8, 0);
	itemWindow = UiWindow_CreateFar(16, 11, 14, 9, 2);
	redraw = 1;

	while (1)
	{
		if (ask_again != 0)
		{
			ask_again = 0;
			UiMessage_ShowAndWait(MsgWhoGetsThis);
			redraw = 1;
		}
		if (redraw != 0)
		{
			redraw = 0;
			cursor = (cursor + shop->partyMemberCount) % shop->partyMemberCount;
			member = shop->partyMemberIds[cursor];
			Shop_PlaceCursor((void *)party_window, cursor * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty(party_window, cursor, shop->selectedItem);
			if (Item_GetEquipmentGroupFar(shop->selectedItem) == 0)
				Shop_DrawUnitItem(itemWindow, member, shop->selectedItem);
			else
				Shop_DrawEquipComparison(itemWindow, member, shop->selectedItem);
		}
		if ((gKeyState & KEY_A) != 0)
		{
			slot = Inventory_AddItemFar(member, shop->selectedItem);
			if (slot < 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				UiWork_PushValueSlotFar(member, 1);
				UiWork_PushValueSlotFar(shop->selectedItem, 2);
				if (Inventory_CountFar(member) == 15)
					UiMessage_ShowAndWait(MsgPackFull);
				else
					UiMessage_ShowAndWait(MsgCannotCarryItem);
				continue;
			}
			Inventory_RemoveFar(member, slot);
			if ((u32)item->price > gGameState.coins)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				UiMessage_ShowAndRestoreState(MsgBuyNotEnoughMoney);
				result = -1;
				break;
			}
			if (Item_IsCompatibleWithOwnerFar(member, shop->selectedItem) == 0)
			{
				UiWork_PushValueSlotFar(member, 1);
				UiMessage_ShowAndWait(MsgCannotEquipStillWant);
				if (UiMessage_ShowChoice(0) != 0)
				{
					ask_again = 1;
					continue;
				}
			}
			AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
			WaitFrames(1);
			quantity = Shop_SelectBuyNum(member, shop->selectedItem);
			ask_again = 1;
			if (quantity == -1)
				continue;
			Shop_BuyDone(member, shop->selectedItem, quantity);
			Shop_BuySpecialItem((void *)party_window, itemWindow);
			result = 0;
			break;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			result = -1;
			break;
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

	Menu_ReleaseEntryObjectsFar();
	UiWork_FinalizeFar(itemWindow, 2);
	UiWork_FinalizeFar(party_window, 2);
	WaitFrames(1);
	if (result == 0 && shop->partyAction == 2)
	{
		for (i = 0; i < quantity; i++)
			Item_AdjustCounterFar(shop->selectedItem, -1);
		if (AbilityMenu_BuildAvailableList() == 0)
			goto close;
		if (selected > shop->stockCount - 1)
			selected = shop->stockCount - 1;
	}
	UiMessage_ShowAndWait(MsgBuyAnythingElse);
	goto next_page;

close:
	UiWork_FinalizeFar(price_window, 2);
	UiWork_FinalizeFar(shop->moneyWindow, 2);
	WaitFrames(1);
	return 0;
}

void Shop_DrawStock(s32 window, s32 selected)
{

	struct ShopRuntime	  *shop;
	s16					  *itemIds;
	s16					  *item_id;
	s32					   itemCount;
	s32					   first;
	s32					   slot;
	s32					   x;
	s32					   clear;
	s32					   highlight;
	s16					   stock_item;
	struct ItemDefinition *definition;
	struct RenderOutput	  *icon;

	shop = gMenuWork;
	itemIds = shop->stockItemIds;
	itemCount = shop->stockCount;
	first = selected - selected % 7;
	if (window != 0)
	{
		RenderOutput_PrepareForRedrawFar(window);
		if (first != 0)
		{
			icon = RenderOutput_CreateFar(
				shop->previousPageIcon, 0x40000000, (struct RenderInput *)window, 216, -16);
			clear = 0;
			icon->kind = clear;
			icon->active = 17;
			icon->animStep = clear;
		}
		if (first + 7 < itemCount)
		{
			icon = RenderOutput_CreateFar(
				shop->nextPageIcon, 0x40000000, (struct RenderInput *)window, 216, 24);
			clear = 0;
			icon->kind = clear;
			icon->active = 15;
			icon->animStep = clear;
		}
		slot = 0;
		if (first < (u32)itemCount)
		{
			item_id = (s16 *)((first * 2) + (u32)itemIds);
			x = 16;
			for (; (u32)slot <= 6 && first < (u32)itemCount; x += 32, slot++, item_id++, first++)
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
					icon->animStep = highlight;
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
	window = shop->moneyWindow;
	if (window != 0)
	{
#if EDITION_INTERNATIONAL
		UiText_DrawCharacterAtOffsetFar(MsgYourCoins, window, 0, 0);
		UiText_DrawNumberInWindowFar(gGameState.coins, 6, window, 0x20, 8);
#else
		s32 msg = MsgYourCoins;

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
		return;
	goto draw;
cannot_buy:
	UiText_DrawCharacterAtOffsetFar(MsgCannotBuyThat, window, ITEM_NOTE_X, 8);
	return;
show_price:
	message = MsgPrice;
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
	return;
draw:
	RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
	UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, 0, 0);
	if (price != 0)
		goto show_price;
	switch (price_mode)
	{
	case 1:
		goto cannot_buy;
	case 2:
		UiText_DrawCharacterAtOffsetFar(MsgNotBroken, window, ITEM_NOTE_X, 8);
		break;
	default:
		goto show_price;
	}
	return;
}

//--------------------------------------------------------------------------
// win を描きなおし、メッセージ message を左上から書く（win が 0 なら何もしない）
// 日本版は描きなおしたあと 1 フレーム待つ
//--------------------------------------------------------------------------
void Shop_DrawMsg(s32 win, s32 message)
{
	if (win != 0)
	{
#if EDITION_INTERNATIONAL
		RenderOutput_RedrawSavedRectFar((struct UiWindow *)win);
		UiText_DrawCharacterAtOffsetFar(message, win, 0, 0);
#else
		RenderOutput_ClearListFar((struct UiWindow *)win);
		WaitFrames(1);
		UiText_DrawResourceFar(message, win, 0, 0);
#endif
	}
}

void Shop_DrawParty(s32 window, s32 selected, s32 requirement)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					index;

	if (window != 0)
	{
		for (index = 0; index < shop->partyMemberCount; index++)
		{
			if (index == selected)
				AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[index], 30);
			else
				AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[index], 1);
			shop->partyMemberScale[index] = 0x10000;
			if (!Item_IsCompatibleWithOwnerFar(shop->partyMemberIds[index], requirement))
				shop->partyMemberScale[index] = 0xcccc;
		}
	}
}

void Shop_DrawEquipComparison(s32 window, s32 unitId, s32 item_id)
{
	struct ShopRuntime	  *shop = gMenuWork;
	struct BattleUnit		  *unit = UnitGetFar(unitId);
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
	if (!Item_CanOwnerEquip(unitId, item_id))
	{
		UiText_DrawResourceFar(MsgShopCannotEquip, window, 8, 24);
		return;
	}
	slot = Inventory_FindEquippedFar(unitId, item->type);
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
	UnitCalcFar(unitId);
	with[0] = unit->atk;
	with[1] = unit->def;
	with[2] = unit->agi;
	with[3] = unit->luck;
	unit->inventory[slot] = saved;
	UnitCalcFar(unitId);
	without[0] = unit->atk;
	without[1] = unit->def;
	without[2] = unit->agi;
	without[3] = unit->luck;
	for (i = 0; i <= 2; i++)
	{
		if (without[i] > with[i])
			RenderOutput_CreateFar(
				shop->statDownIcon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)
				->kind = 0;
		else if (without[i] < with[i])
			RenderOutput_CreateFar(
				shop->statUpIcon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)
				->kind = 0;
		UiText_DrawNumberInWindowFar(without[i], 3, window, 32, i * 16);
		if (without[i] != with[i])
			UiText_DrawNumberInWindowFar(with[i], 3, window, 72, i * 16);
		UiText_DrawCharacterAtOffsetFar(MsgShopStatLabel + i, window, 0, i * 16);
		UiWindow_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
	}
	if (replaced != -1)
		UiText_DrawCharacterAtOffsetFar(replaced + MsgItemName, window, 0, 48);
}
void Shop_DrawUnitItem(s32 window, s32 unitId, s32 item_id)
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

	unit = (u8 *)UnitGetFar(unitId);
	x = 8;
	y = 8;
	if (window != 0)
	{
		RenderOutput_PrepareForRedrawFar(window);
		slot = Inventory_FindFar(unitId, item_id);
		if (slot != -1)
		{
			off = slot * 2 + 216;
			UiWork_PushValueSlotFar((*(u16 *)(unit + off) >> 11) + 1, 5);
			UiText_DrawCharacterAtOffsetFar(MsgYouHave, window, 0, 0);
		}
		else
		{
			UiText_DrawCharacterAtOffsetFar(MsgShopNoneInStock, window, 0, 0);
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

s32 Shop_SelectBuyNum(s32 unitId, s32 item_id)
{
	struct ShopRuntime	  *shop;
	struct BattleUnit		  *unit;
	struct ItemDefinition *item;
	s32					   quantity;
	s32					   chance;
	s32					   slot;
	s32					   maximum;
	s32					   result;

	shop = gMenuWork;
	unit = UnitGetFar(unitId);
	item = Item_GetDirectFar(item_id);
	result = 1;
	if (item->flags & 0x10)
	{
		UiMessage_ShowAndWait(MsgHowManyToBuy);
		slot = Inventory_FindFar(unitId, item_id);
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

		if (shop->partyAction == 2)
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
					Dma_Set(gShop_QuantityTiles, buffer, 0x84000040, REG_DMA3);
					Shop_FillSelector(30, 14, buffer);
					Shop_FillSelector(minimum + maximum, 0, buffer);
					Shop_FillSelector(minimum + count + 1, 10, buffer);
					Shop_FillSelector(minimum, 2, buffer);
					VramBlock_LoadCached(slot, 0x100, buffer);
					result = count + 1;
#if EDITION_INTERNATIONAL
					UiText_DrawNumberInWindowFar(result, 2, window, QUANTITY_X, 0);
					UiText_DrawNumberInWindowFar(price * result, 6, window, QUANTITY_X + 16, 0);
					UiText_DrawCharacterAtOffsetFar(MsgCoins, window, COINS_X, 0);
#else
					UiText_DrawNumberInWindowFar(result, 2, window, 64, 0);
					msg = MsgPiece;
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

void Shop_BuyDone(s32 unitId, s32 item_id, s32 quantity)
{
	s32					   replaced_slot;
	s32					   remaining;
	s32					   added_slot;
	struct ItemDefinition *item;

	remaining = quantity;
	item = Item_GetDirectFar(item_id);
	added_slot = 0;
	replaced_slot = Inventory_FindEquippedFar(unitId, item->type);
	AudioCommand_PlayFar(SOUND_SHOP_PURCHASE);
	if (added_slot < remaining)
	{
		do
		{
			added_slot = Inventory_AddItemFar(unitId, item_id);
			Party_AdjustCoinsFar(0 - item->price);
			remaining -= 1;
			Party_AdjustSixDigitCounterBFar(item->price);
			Shop_DrawMoney();
		} while (remaining != 0);
	}
	UiMessage_ShowAndRestoreState(MsgHereYouGo);
	if (Shop_ConfirmEquip(unitId, added_slot) != 0)
	{
		Shop_SellOld(unitId, replaced_slot);
	}
}

s32 Shop_ConfirmEquip(s32 unitId, s32 slot)
{
	struct ShopRuntime	  *menu = gMenuWork;
	struct BattleUnit		  *unit = UnitGetFar(unitId);
	s32					   item_id = unit->inventory[slot] & 0x1ff;
	struct ItemDefinition *info = Item_GetDirectFar(item_id);
	s32					   replaced;
	s32					   menu_value;

	if (unit->inventory[slot] & 0x200)
		return 0;

	if (Item_CanOwnerEquip(unitId, item_id) == 0)
		return 0;

	replaced = Inventory_FindEquippedFar(unitId, info->type);
	if (replaced != -1)
	{
		struct ItemDefinition *old_info = Item_GetDirectFar(unit->inventory[replaced]);

		if (old_info->flags & 2)
			return 0;
	}

	UiWork_PushValueSlotFar(unitId, 1);
	UiMessage_ShowAndWait(MsgEquipNowPrompt);
	if (UiMessage_ShowChoice(0) != 0)
		return 0;

	Inventory_EquipFar(unitId, slot);
	menu_value = menu->itemWindow;
	if (menu_value != 0)
		Shop_DrawUnitGrid(menu_value, unitId);

	if (info->flags & 1)
	{
		AudioCommand_PlayFar(103);
		UiWork_FinalizePendingCoreFar();
		UiText_OpenMessageWindowFar(MsgBecameCursed, 8, 4, 2);
		while (UiWork_IsCompleteFar() == 0)
		{
			WaitFrames(1);
		}
	}

	UiMessage_ShowAndRestoreState(MsgLookBolder);
	return 1;
}

s32 Shop_SellOld(s32 unitId, s32 slot)
{
	struct BattleUnit *unit;
	s32				item_id;

	unit = UnitGetFar(unitId);
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
	Shop_SellItem(unitId, slot, -1);
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
	s32					listWindow;
	s32					selection = 0;
	s32					redraw = 1;
	s32					unitId = 0;

	shop->moneyWindow = UiWindow_CreateFar(0, 9, BASE_W, 4, 2);
	Shop_DrawMoney();
	shop->itemWindow = UiWindow_CreateFar(16, 12, 14, 8, 2);
	listWindow = UiWindow_CreateFar(0, 14, 13, 3, 2);
	shop->cursor.anchor->active = 4;
	shop->mode = 12;
	PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, 0);

	for (;;)
	{
		if (redraw != 0)
		{
			redraw = 0;
			selection = (selection + shop->partyMemberCount) % shop->partyMemberCount;
			unitId = shop->partyMemberIds[selection];
			Shop_PlaceCursor((void *)listWindow, selection * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty(listWindow, selection, 0);
			Shop_DrawUnitGrid(shop->itemWindow, unitId);
		}

		if ((gKeyState & KEY_A) != 0)
		{
			WaitFrames(1);
			if (Inventory_CountFar(unitId) == 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			}
			else
			{
				AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
				if (shop->partyAction == 1)
					Shop_SelectSell(unitId);
				else
					Shop_SelectRepair(unitId);
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
			UiWork_FinalizeFar(listWindow, 2);
			UiWork_FinalizeFar(shop->itemWindow, 2);
			UiWork_FinalizeFar(shop->moneyWindow, 2);
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

s32 Shop_SelectSell(s32 unitId)
{
	s32					price_window;
	struct ShopRuntime *shop;
	s32					listWindow;
	struct BattleUnit	   *unit;
	s32					selection;
	s32					itemCount;
	s32					redraw;
	s32					result;
	s32					item_id;
	s32					quantity;
	void			   *window;
	s32					x;

	shop = gMenuWork;
	unit = UnitGetFar(unitId);
	itemCount = 1;
	listWindow = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
	selection = 0;

	for (;;)
	{
		price_window = UiWindow_CreateFar(0, 5, 30, 3, 2);
		shop->cursor.anchor->active = 18;
		shop->mode = 12;
		redraw = 1;

		while (1)
		{
			if (redraw != 0)
			{
				redraw = 0;
				itemCount = Inventory_CountFar(unitId);
				if (selection > itemCount - 1)
					selection = itemCount - 1;
				item_id = 0x1ff & unit->inventory[selection];
				window = (void *)shop->itemWindow;
				x = selection % 5 * 16;
				Shop_PlaceCursor(window, x, selection / 5 * 16 + 8);
				shop->mode = 3;
				Shop_DrawItemPrice(
					listWindow, item_id, Shop_SalePrice(unit->inventory[selection]), 1);
				Shop_DrawMsg(price_window, item_id + MsgItemPlainName);
			}

			if ((gKeyState & KEY_A) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
				result = 0;
				break;
			}
			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				result = -1;
				break;
			}
			if ((gKeysRepeat & KEY_LEFT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection -= 1;
				selection = (selection + itemCount) % itemCount;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_RIGHT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection += 1;
				selection = (selection + itemCount) % itemCount;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_UP) != 0)
			{
				selection -= 5;
				if (selection < 0)
					selection += 15;
				while (selection >= itemCount)
					selection -= 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_DOWN) != 0)
			{
				selection += 5;
				if (selection >= itemCount)
					selection -= 15;
				while (selection < 0)
					selection += 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			WaitFrames(1);
		}

		UiWork_FinalizeFar(price_window, 2);
		WaitFrames(1);
		if (result != 0)
			break;

		quantity = Shop_SelectSellNum(unitId, selection);
		if (quantity != -1)
			Shop_SellItem(unitId, selection, quantity);
		UiMessage_ShowAndWait(MsgSellAnythingElse);
		if (Inventory_CountFar(unitId) == 0)
			break;
	}

	UiWork_FinalizeFar(listWindow, 2);
	return result;
}

void Shop_DrawUnitGrid(s32 window, s32 unitId)
{
	struct BattleUnit		*unit;
	s32					 x;
	s32					 y;
	s32					 slot;
	struct RenderOutput *icon;

	unit = UnitGetFar(unitId);
	x = 8;
	y = 0;
	if (window != 0)
	{
		RenderOutput_PrepareForRedrawFar(window);
		if (Inventory_CountFar(unitId) == 0)
		{
#if defined(TBS_EDITION_IT)
			UiText_DrawResourceFar(MsgNoItems, window, 20, 12);
#else
			UiText_DrawResourceFar(MsgNoItems, window, 8, 20);
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

s32 Shop_SelectSellNum(s32 unitId, s32 slot)
{
	s32					   result;
	s16					   savedX;
	s16					   savedY;
	s32					   effect;
	s32					   state;
	s32					   selection;
	struct ItemDefinition *item;
	struct ShopRuntime	  *shop;
	struct BattleUnit		  *unit;

	shop = gMenuWork;
	unit = UnitGetFar(unitId);
	item = Item_GetDirectFar(unit->inventory[slot]);
	result = 1;
	effect = Shop_SalePrice(unit->inventory[slot]);
	state = Shop_GetSelectionState(unitId, slot);
	selection = state;
	if ((item->flags & 0x10) && state > 1)
	{
		UiMessage_ShowAndWait(MsgHowManyToSell);
		savedX = shop->cursor.targetX;
		savedY = shop->cursor.targetY;
		shop->cursor.anchor->active = 4;
		shop->mode = 0xc;
		Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
		result = Shop_SelectQuantity(0, selection, effect);
		WaitFrames(1);
		UiIcon_PrepareObjectFar(shop->cursor.anchor);
		Shop_PlaceCursor(NULL, savedX, savedY);
	}
	return result;
}

void Shop_SellItem(s32 unitId, s32 slot, s32 count)
{
	struct ShopRuntime	  *shop = gMenuWork;
	struct BattleUnit		  *unit = UnitGetFar(unitId);
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
		UiMessage_ShowAndRestoreState(MsgWeDoNotBuy);
		return;
	}
	if ((unit->inventory[slot] & 0x200) && (item->flags & 2))
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState(MsgCannotSellEquipped);
		return;
	}
	if (all)
		message = MsgOldItemSellOffer;
	else if (unit->inventory[slot] & 0x400)
		message = MsgDamagedSellOffer;
	else if (count > 1)
		message = MsgSellTotal;
	else if (rare)
		message = MsgRareSellOffer;
	else
		message = MsgSellOffer;
	UiWork_PushValueSlotFar(item_id, 2);
	UiWork_PushValueSlotFar(total, 5);
	UiMessage_ShowAndRestoreState(message);
	if (UiMessage_ShowChoice(0) != 0)
	{
		if (rare || all)
			message = MsgKeepIt;
		else
			message = MsgChangedMind;
		UiMessage_ShowAndRestoreState(message);
		return;
	}
	AudioCommand_PlayFar(102);
	for (i = 0; i < count; i++)
		Inventory_DiscardFar(unitId, slot);
	Party_AdjustCoinsFar(total);
	Shop_DrawMoney();
	Shop_DrawUnitGrid(shop->itemWindow, unitId);
	if (rare || all)
		message = MsgSold;
	else
		message = MsgDeal;
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

s32 Shop_SelectUnit(void)
{
	Shop_PickUnit();
	return 0;
}

s32 Shop_SelectRepair(s32 unitId)
{
	struct ShopRuntime *shop;
	s32					itemCount;
	s32					price_window;
	s32					listWindow;
	struct BattleUnit	   *unit;
	s32					selection;
	s32					redraw;
	s32					result;
	s32					item_id;
	s32					price;
	void			   *window;
	s32					x;
	s32					y;

	shop = gMenuWork;
	unit = UnitGetFar(unitId);
	itemCount = 1;
	listWindow = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
	selection = 0;

	for (;;)
	{
		price_window = UiWindow_CreateFar(0, 5, 30, 3, 2);
		shop->cursor.anchor->active = 18;
		shop->mode = 12;
		redraw = 1;

		while (1)
		{
			if (redraw != 0)
			{
				redraw = 0;
				itemCount = Inventory_CountFar(unitId);
				if (selection > itemCount - 1)
					selection = itemCount - 1;
				item_id = 0x1ff & unit->inventory[selection];
				window = (void *)shop->itemWindow;
				x = selection % 5 * 16;
				y = selection / 5 * 16 + 8;
				Shop_PlaceCursor(window, x, y);
				shop->mode = 3;
				price = Shop_RepairPrice(unit->inventory[selection]);
				Shop_DrawItemPrice(listWindow, item_id, price, 2);
				Shop_DrawMsg(price_window, item_id + MsgItemPlainName);
			}
			if ((gKeyState & KEY_A) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
				result = 0;
				break;
			}
			if ((gKeyState & KEY_B) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				result = -1;
				break;
			}
			if ((gKeysRepeat & KEY_LEFT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection -= 1;
				selection = (selection + itemCount) % itemCount;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_RIGHT) != 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				selection += 1;
				selection = (selection + itemCount) % itemCount;
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_UP) != 0)
			{
				selection -= 5;
				if (selection < 0)
					selection += 15;
				while (selection >= itemCount)
					selection -= 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			if ((gKeysRepeat & KEY_DOWN) != 0)
			{
				selection += 5;
				if (selection >= itemCount)
					selection -= 15;
				while (selection < 0)
					selection += 5;
				AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
				redraw = 1;
			}
			WaitFrames(1);
		}

		UiWork_FinalizeFar(price_window, 2);
		WaitFrames(1);
		if (result != 0)
			break;

		Shop_RepairItem(unitId, selection);
		UiMessage_ShowAndWait(MsgRepairAnythingElse);
		if (Inventory_CountFar(unitId) == 0)
			break;
	}

	UiWork_FinalizeFar(listWindow, 2);
	return result;
}

void Shop_RepairItem(s32 unitId, s32 slot)
{
	struct ShopRuntime	  *shop = gMenuWork;
	struct BattleUnit		  *unit = UnitGetFar(unitId);
	s32					   item_id = unit->inventory[slot] & 0x1ff;
	struct ItemDefinition *item = Item_GetDirectFar(item_id);
	s32					   equipped = Inventory_FindEquippedFar(unitId, item->type);
	u32					   price = Shop_RepairPrice(unit->inventory[slot]);
	s32					   message;
	u8					   kind;
	u32					   saved;

	kind = item->useType;
	if (kind != 2)
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState(MsgCannotRepairKind);
		return;
	}
	if (!(unit->inventory[slot] & 0x400))
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState(MsgNotDamaged);
		return;
	}
	if ((unit->inventory[slot] & 0x200) && (kind & item->flags))
	{
		UiWork_PushValueSlotFar(item_id, 2);
		UiMessage_ShowAndRestoreState(MsgCursedWontComeOff);
		return;
	}
	if (price > gGameState.coins)
	{
		UiMessage_ShowAndRestoreState(MsgNotEnoughMoney);
		return;
	}
	UiWork_PushValueSlotFar(item_id, 2);
	UiWork_PushValueSlotFar(price, 5);
	message = MsgShopRepairConfirm;
	UiMessage_ShowAndRestoreState(message);
	if (UiMessage_ShowChoice(0) != 0)
	{
		UiMessage_ShowAndRestoreState(message + 1);
		return;
	}
	saved = unit->inventory[slot];
	unit->inventory[slot] = 0;
	Shop_DrawUnitGrid(shop->itemWindow, unitId);
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
	Inventory_RepairFar(unitId, slot);
	Party_AdjustCoinsFar(-price);
	Shop_DrawMoney();
	Shop_DrawUnitGrid(shop->itemWindow, unitId);
	UiWork_PushValueSlotFar(item_id, 2);
	UiMessage_ShowAndRestoreState(message + 3);
	if (Shop_ConfirmEquip(unitId, slot))
		Shop_SellOld(unitId, equipped);
}

// 贈り物の値段
static const s16 sSpecialItemPrices[30] =
{
	300, 300, 300, 300, 300,
	500, 500, 500, 500, 500,
	1000, 1000, 1000, 1000, 1000,
	2000, 2000, 2000, 2000, 2000,
	4000, 4000, 4000, 4000, 4000,
	8000, 8000, 8000, 8000, 8000,
};

// 贈り物の品物 (0 で終わり)。どの関数も読まない
const s16 gShop_GiftItemIds[21] =
{
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
	16, 195, 38, 84, 89, 90, 180, 181, 190, 0,
};

void Shop_BuySpecialItem(void *window, s32 itemWindow)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					price;
	u32					saved;
	s32					redraw;
	s32					unitId;
	s32					selectedIndex;
	s32					message;
	s32					slot;

	redraw = 1;
	unitId = 0;
	saved = shop->selectedItem;
	price = sSpecialItemPrices[gGameState.shopGifts];
	selectedIndex = 0;

	if (price > gGameState.shopCredit)
		return;
	shop->selectedItem = SPECIAL_ITEM;
	UiWork_PushValueSlotFar(SPECIAL_ITEM, 2);
	message = MsgTokenGift;
	UiMessage_ShowAndRestoreState(message);
	UiWork_PushValueSlotFar(shop->selectedItem, 2);
	UiMessage_ShowAndRestoreState(message + 1);

	while (1)
	{
		if (redraw != 0)
		{
			redraw = 0;
			selectedIndex = (selectedIndex + shop->partyMemberCount) % shop->partyMemberCount;
			unitId = shop->partyMemberIds[selectedIndex];
			Shop_PlaceCursor(window, selectedIndex * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty((s32)window, selectedIndex, shop->selectedItem);
			Shop_DrawUnitItem(itemWindow, unitId, shop->selectedItem);
		}
		if ((gKeyState & KEY_A) != 0)
		{
			slot = Inventory_AddItemFar(unitId, shop->selectedItem);
			if (slot < 0)
			{
				AudioCommand_PlayFar(0x71);
				UiWork_PushValueSlotFar(unitId, 1);
				UiWork_PushValueSlotFar(shop->selectedItem, 2);
				if (Inventory_CountFar(unitId) == 15)
					UiMessage_ShowAndWait(MsgPackFull);
				else
					UiMessage_ShowAndWait(MsgCannotCarryItem);
				continue;
			}
			Inventory_RemoveFar(unitId, slot);
			AudioCommand_PlayFar(0x65);
			UiMessage_ShowAndRestoreState(MsgHereYouGo);
			Inventory_AddItemFar(unitId, shop->selectedItem);
			Party_AdjustSixDigitCounterBFar(-price);
			Party_AdjustCounterCappedAt28Far(1);
			break;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			UiMessage_ShowAndRestoreState(MsgDontWantIt);
			AudioCommand_PlayFar(0x71);
			break;
		}
		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(0x6f);
			selectedIndex--;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(0x6f);
			selectedIndex++;
			redraw = 1;
		}
		WaitFrames(1);
	}
	shop->selectedItem = saved;
}

// 店ごとの品物: 品物 24 個 (0 で終わり), 特別な品物 8 個 (0 で終わり), 種類
static const s16 sStockTable[35][33] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		1, 16, 43, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		89, 90, 103, 118, 127, 156, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 187, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		1, 16, 31, 43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		56, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		90, 118, 127, 136, 156, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 187, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		1, 2, 17, 31, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		57, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		75, 90, 104, 119, 128, 136, 145, 157, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		2, 17, 31, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		58, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		75, 92, 119, 128, 136, 145, 157, 166, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		2, 17, 32, 44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		75, 92, 119, 128, 137, 146, 157, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		2, 18, 32, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		92, 105, 120, 137, 146, 157, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		106, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		3, 18, 32, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		59, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		77, 92, 105, 120, 129, 137, 147, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		3, 18, 33, 45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		60, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		77, 107, 120, 129, 138, 147, 158, 167, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		180, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		61, 0, 0, 0, 0, 0, 0, 0,
		1,
	},
	{
		78, 94, 107, 121, 129, 138, 148, 158, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		182, 189, 0, 0, 0, 0, 0, 0,
		0,
	},
	{
		4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		5, 20, 34, 62, 0, 0, 0, 0,
		1,
	},
	{
		79, 139, 149, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		109, 131, 0, 0, 0, 0, 0, 0,
		2,
	},
	{
		180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		182, 183, 189, 186, 0, 0, 0, 0,
		0,
	},
	{
		4, 19, 33, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		6, 21, 47, 63, 0, 0, 0, 0,
		1,
	},
	{
		80, 150, 169, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		110, 122, 140, 159, 0, 0, 0, 0,
		2,
	},
	{
		180, 181, 187, 188, 236, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		182, 183, 189, 186, 0, 0, 0, 0,
		0,
	},
};

s32 EventTable_GetRowLimit(void)
{
	return 35;
}

void EventTable_ApplyRowAbilities(s32 row_no)
{
	const s16 *row;
	const s16 *entry;
	s32		   value;
	s32		   count;
	s32		   flag;

	flag = row_no + 0x400;
	if (GameFlag_TestFar(flag) == 0)
	{
		GameFlag_SetBitFar(flag);
		count = 0;
		value = sStockTable[row_no][24];
		if (value != 0)
		{
			row = sStockTable[row_no];
			entry = row + 24;
			do
			{
				Item_AdjustCounterFar(value, 1);
				count++;
				if (count > 7)
					break;
				entry++;
				value = *entry;
			} while (value != 0);
		}
	}
}

s32 EventTable_CopyRowHeader(s32 row_no, s16 *output)
{
	const s16 *src;
	s16		  *dst;
	s32		   count;

	count = 0;
	if (sStockTable[row_no][0] != 0)
	{
		dst = output;
		src = sStockTable[row_no];
		do
		{
			/* FAKEMATCH: only the copy read is volatile, so its lifetime stays separate from the signed sentinel read. Without volatile: differ 41; indexed and while forms: 34 to 54; read or copied as u16 (2026-10-13): differ 41, the sentinel read is reused for the copy. */
			*dst = *(const volatile s16 *)src;
			count++;
			src++;
			dst++;
			if (count > 23)
				break;
		} while (*src != 0);
	}
	output[count] = 0;
	return count;
}

s32 EventTable_GetRowType(s32 index)
{
	return sStockTable[index][32];
}

s32 Shop_ServicePrice(s32 entry_id, s32 kind)
{
	u8	value = UnitGetFar(entry_id)->level;
	s32 result = 0;

	if (kind == 0)
	{
		result = value * 20;
	}
	else if (kind == 1)
	{
		result = 10;
	}
	else if (kind == 2)
	{
		result = 50;
	}
	else if (kind == 3)
	{
		result = value * 10;
	}
	return result;
}

s32 Shop_CanServe(s32 entry_id, s32 kind)
{
	struct BattleUnit *entry = UnitGetFar(entry_id);
	s32				result = 0;

	if ((kind == 0 && entry->hp <= 0) || (kind == 1 && entry->poison != 0)
		|| (kind == 2 && entry->haunt != 0) || (kind == 3 && entry->restraint != 0))
	{
		result = 1;
	}
	return result;
}

s32 Shop_CountUnits(void)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					active = 0;
	s32					variant = shop->partyAction;
	s32					index;

	for (index = 0; index < shop->partyMemberCount; index++)
	{
		if (Shop_CanServe(shop->partyMemberIds[index], variant))
			active++;
	}
	return active;
}

s32 Shop_MsgByMode(s32 value)
{
	s8 mode = ((struct ShopRuntime *)gMenuWork)->partyAction;

	if (mode == 1)
	{
		value += MsgCurePoisonService - MsgReviveService;
	}
	if (mode == 2)
	{
		value += MsgExorcismService - MsgReviveService;
	}
	if (mode == 3)
	{
		value += MsgRemoveCurseService - MsgReviveService;
	}
	return value;
}

void UiMessage_ShowResolvedAndWait(s32 value)
{
	s32 no;

	no = BattleFx_GetResourceIdFar(((struct ShopRuntime *)gMenuWork)->keeperResource);
	UiWork_FinalizePendingCoreFar();
	value = Shop_MsgByMode(value);
	UiText_OpenMessageWindowFar(value, 5, 0, (no << 0x10) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
	{
		WaitFrames(1U);
	}
	WaitFrames(1U);
}

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
	struct ShopRuntime	 *state;
	struct RenderOutput **slot;
	s32					  value;
	u8					  saved;

	state = gMenuWork;
	slot = &state->cursor.anchor;
	saved = (*slot)->active;
	value = BattleFx_GetResourceIdFar(state->keeperResource);
	arg0 = Shop_MsgByMode(arg0);
	(*slot)->active = 13;
	UiWork_FinalizePendingCoreFar();
	UiText_OpenMessageWindowFar(arg0, 5, 0, (value << 16) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);
	WaitFrames(1);
	state->cursor.anchor->active = saved;
}

s32 Shop_ConfirmAct(s32 unitId)
{
	s32					 partyAction = 0;
	s32					 listWindow = 0;
	struct ShopRuntime	*shop;
	struct RenderOutput *cursorAnchor;

	Shop_InitializeCursorWork();
	shop = gMenuWork;
	shop->partyAction = listWindow;

	{
		s32 shown =
			(u16)((struct AnimationObject *)((struct ObjectRuntime *)Object_GetByIdFar(unitId))
					  ->animation)
				->entries[0]
				->animId;
		shop->keeperResource = shown;
	}

	listWindow = UiWindow_CreateWithSideObjectFar(shop->keeperResource, 0, 0, 0);
	if (listWindow == 0)
	{
		listWindow = UiWindow_CreateFar(-5, 0, 5, 5, 2);
	}
	if (listWindow == 0)
	{
		listWindow = UiWindow_CreateFar(0, 0, 5, 5, 2);
		SideObject_CreateFar(2, 0, 0, listWindow, -4, -4);
	}

	cursorAnchor = RenderOutput_CreateFar(
		shop->cursorIcon, 0x40000000, (struct RenderInput *)listWindow, 0, 0);
	cursorAnchor->active = 1;
	cursorAnchor->kind = 0;
	ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursorAnchor;
	UiMessage_ShowResolvedAndWait(MsgSanctumWelcome);

#if EDITION_INTERNATIONAL
	shop->moneyWindow = UiWindow_CreateFar(16, 11, 12, 4, 2);
#else
	shop->moneyWindow = UiWindow_CreateFar(16, 11, 11, 4, 2);
#endif
	Shop_DrawMoney();

	for (;;)
	{
		partyAction = Menu_SelectEntry19To1cFar(partyAction);
		shop->partyAction = partyAction;
		if (partyAction == -1)
			break;

		{
			s32 base = MsgReviveService;
			s32 message = base;

			base = 0;
			UiMessage_ShowResolvedAndWait(message);
			if (Shop_CountUnits() == 0)
			{
				UiMessage_ShowResolvedAndWait(message + 1);
			}
			else
			{
				Sanctum_RunPartyService();
			}
		}
		shop->partyAction = 0;
		ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
		UiMessage_ShowResolvedAndWait(MsgSanctumMoreAid);
	}

	UiMessage_ShowResolvedAndWait(MsgSanctumFarewell);
	UiWork_FinalizeFar(shop->moneyWindow, 2);
	UiWork_FinalizeFar(listWindow, 2);
	Inn_Cleanup();
	return 0;
}

s32 Sanctum_RunPartyService(void)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					price_window;
	s32					redraw;
	s32					kind;
	s32					listWindow;
	s32					selection;
	s32					unitId;
	s32					retry;
	s32					price;
	s32					message;

	price_window = 0;
	redraw = 1;
	kind = shop->partyAction;
	UiMessage_ShowResolvedAndWait(MsgWhoToRevive);

	listWindow = UiWindow_CreateFar(1, 12, 13, 3, 2);
	shop->cursor.anchor->active = 4;
	shop->mode = redraw;
	PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, price_window);
#if defined(TBS_EDITION_DE)
	price_window = UiWindow_CreateFar(0, 16, 30, 3, 2);
#elif defined(TBS_EDITION_FR)
	price_window = UiWindow_CreateFar(1, 16, 25, 3, 2);
#else
	price_window = UiWindow_CreateFar(1, 16, 23, 3, 2);
#endif

	selection = 0;
	unitId = 0;
	retry = 0;
	while (selection < shop->partyMemberCount)
	{
		unitId = shop->partyMemberIds[selection];
		if (Shop_CanServe(unitId, kind) != 0)
			break;
		selection++;
	}

	redraw = 1;
	for (;;)
	{
		if (retry != 0)
		{
			retry = 0;
			UiMessage_ShowResolvedAndWait(MsgWhoToRevive);
			redraw = 1;
			selection = 0;
			while (selection < shop->partyMemberCount)
			{
				unitId = shop->partyMemberIds[selection];
				if (Shop_CanServe(unitId, kind) != 0)
					break;
				selection++;
			}
		}

		if (redraw != 0)
		{
			redraw = 0;
			selection = (selection + shop->partyMemberCount) % shop->partyMemberCount;
			unitId = shop->partyMemberIds[selection];
			Shop_PlaceCursor((void *)listWindow, selection * 24 - 12, 0);
			shop->mode = 3;
			Shop_HiliteUnit(listWindow, selection);
			Shop_DrawSelectMsg(price_window, unitId);
		}

		if ((gKeyState & KEY_A) != 0)
		{
			WaitFrames(1);
			price = Shop_ServicePrice(unitId, kind);
			if (Shop_CanServe(unitId, kind) == 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				continue;
			}
			UiWork_PushValueSlotFar(unitId, 1);
			UiWork_PushValueSlotFar(price, 5);
			message = MsgReviveDonation;
			UiMessage_ShowResolvedAndWait(message);
			if (UiMessage_ShowChoiceVariant(0) != 0)
			{
				UiMessage_ShowResolvedAndRestoreState(message + 2);
				retry = 1;
				continue;
			}
			if (price > (u32)gGameState.coins)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				UiMessage_ShowResolvedAndRestoreState(message + 1);
				retry = 1;
				continue;
			}
			UiWork_PushValueSlotFar(unitId, 1);
			UiMessage_ShowResolvedAndWait(message + 3);
			UiWork_FinalizePendingCoreFar();
			BattleUnit_ResetStateByMode(unitId, kind);
			Shop_RunPartyMemberIconBurst(selection);
			Party_AdjustCoinsFar(-price);
			Shop_DrawMoney();
			UiWork_PushValueSlotFar(unitId, 1);
			UiMessage_ShowResolvedAndWait(message + 4);
			if (Shop_CountUnits() != 0)
			{
				retry = 1;
				continue;
			}
			break;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			break;
		}
		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			redraw = 1;
			selection -= 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			redraw = 1;
			selection += 1;
		}
		WaitFrames(1);
		continue;
	}

	Menu_ReleaseEntryObjectsFar();
	UiWork_FinalizeFar(price_window, 2);
	UiWork_FinalizeFar(listWindow, 2);
	WaitFrames(1);
	return 0;
}

void BattleUnit_ResetStateByMode(s32 id, s32 mode)
{
	struct BattleUnit *unit;

	unit = UnitGetFar(id);
	if (mode == 0)
	{
		unit->hp = unit->maxHp;
		UnitRatioFar(id);
	}
	else if (mode == 1)
	{
		unit->poison = 0;
	}
	else if (mode == 2)
	{
		unit->haunt = 0;
	}
	else if (mode == 3)
	{
		s32 i;

		for (i = 0; i < 15; i++)
		{
			if (unit->inventory[i] & 0x200)
			{
				if (Item_GetDirectFar(unit->inventory[i])->flags & 1)
				{
					unit->inventory[i] ^= 0x200;
					UnitCalcFar(id);
				}
			}
		}
	}
}

void Shop_HiliteUnit(s32 enabled, s32 selected)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					index;
	s32					variant = shop->partyAction;

	if (enabled != 0)
	{
		for (index = 0; index < shop->partyMemberCount; index++)
		{
			if (index == selected)
				AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[index], 30);
			else
				AnimationObjects_SelectAnimationFar(shop->partyMemberIcons[index], 1);
			shop->partyMemberScale[index] = 0x10000;
			if (!Shop_CanServe(shop->partyMemberIds[index], variant))
				shop->partyMemberScale[index] = 0xb333;
		}
	}
}

void Shop_DrawSelectMsg(s32 target, s32 selection)
{
	s32 variant;
	s32 message;

	variant = ((struct ShopRuntime *)gMenuWork)->partyAction;
	message = Shop_ServicePrice(selection, variant);
	if (target != 0)
	{
		RenderOutput_PrepareForRedrawFar(target);
		if (Shop_CanServe(selection, variant) != 0)
		{
			variant = MsgReviveCost;
		}
		else
		{
			variant = MsgNoHealingNeeded;
		}
		variant = Shop_MsgByMode(variant);
		UiWork_PushValueSlotFar(message, 5);
		UiText_DrawResourceFar(variant, target, 0, 0);
	}
}

void BattleFx_UpdateRadialMotion(struct EffectSlot *effect)
{
	struct FixedPointPosition position;
	s8						 *state_pointer;
	s32						  state;
	s32						  result;

	state_pointer = &effect->state;
	state = *state_pointer;
	if (state == 0)
	{
		position.x = effect->originX;
		position.z = effect->originZ;
		Vector_AddPolarOffset(0x280000, Random16(), (s32 *)&position);
		EffectSlot_SetPositionFar(effect, position.x, position.z);
		position.x = effect->originX;
		position.z = effect->originZ;
		Vector_AddPolarOffset(0x40000, Random16(), (s32 *)&position);
		effect->targetX = position.x;
		effect->targetZ = position.z;
		effect->maxSpeed = 0x20000;
		effect->acceleration = 0x6666;
		effect->flag42 = state;
		*state_pointer = *state_pointer + 1;
	}
	else if (state == 1)
	{
		result = BattleFx_HasReachedTargetFar(effect);
		if (result == 0)
			*state_pointer = result;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTargetFar(effect) == 0)
			BattleFx_ClearOwnedSlotFar(effect);
	}
}

void Shop_ResetEffects(void)
{
	struct ShopRuntime *shop = gMenuWork;
	struct EffectSlot  *effect = shop->effects;
	s32					count = 23;
	s8					member;

	do
	{
		count--;
		EffectSlot_UpdateFar((s32)effect);
		effect++;
	} while (count >= 0);
	member = shop->burstMember;
	if (member != -1)
		ObjectGroup_SetChildValueUnlessFifteenFar(
			shop->partyMemberIcons[member], (Random16() * 7) >> 16);
}

// 仲間の動作ごとの効果音
static const s8 sPartyActionCues[4] =
{
	0x52, 0x54, 0x58, 0x57,
};

void Shop_RunPartyMemberIconBurst(s32 member)
{

	struct ShopRuntime		 *shop;
	s8						  saved_kind;
	struct FixedPointPosition position;
	struct EffectSlot		 *effect;
	s32						  callback_flags;
	s32						  i;

	shop = gMenuWork;
	saved_kind = shop->cursor.anchor->active;
	shop->burstMember = 0xff;
	shop->cursor.anchor->active = 13;
	AudioCommand_PlayFar(sPartyActionCues[shop->partyAction]);
	Shop_RestoreSceneTiles(0x00202108);
	AnimationObjects_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 0);
	WaitFrames(20);
	callback_flags = 0xc80;
	Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callback_flags);

	position.x = shop->partyMemberX[member] << 16;
	position.z = (shop->partyMemberY[member] << 16) + 0xfff40000;

	i = 0;
	effect = shop->effects;
	do
	{
		EffectSlot_InitializeFar(effect, 0x11c, position.x, position.z);
		EffectSlot_SetCallbackFar(effect, BattleFx_UpdateRadialMotion);
		EffectSlot_SetObjectModeFar(effect, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(effect->object, (Random16() * 7) >> 16);
		effect->scaleY = 0xb333;
		effect->scaleX = 0xb333;
		WaitFrames(3);
		if (i == 5)
		{
			shop->burstMember = member;
		}
		i++;
		effect++;
	} while (i <= 17);

	AudioCommand_WaitForStateByteClear();
	{
		u8	active_mode = 2;
		u8 *entry = (u8 *)&shop->effects[0].state;
		for (i = 23; i >= 0; i--)
		{
			if (*(s8 *)(entry + 5) != 0)
			{
				*entry = active_mode;
			}
			entry += sizeof(struct EffectSlot);
		}
	}

	WaitFrames(20);
	AudioCommand_PlayFar(126);
	shop->burstMember = -1;
	ObjectGroup_SetChildValueUnlessFifteenFar(shop->partyMemberIcons[member], 0);
	WaitFrames(20);

	{
		u8				  *flag_entry = (u8 *)&shop->effects[0].active;
		struct EffectSlot *entry2 = shop->effects;
		for (i = 23; i >= 0; i--)
		{
			s32 flag = *flag_entry;

			flag <<= 24;

			flag_entry += 0x48;
			if (flag != 0)
			{
				BattleFx_ClearOwnedSlotFar(entry2);
			}
			entry2++;
		}
	}

	Scheduler_RemoveCallback(Shop_ResetEffects);
	AnimationObjects_SetActiveTimerStepsFar(shop->partyMemberIcons[member], 16);
	Shop_InitEffect();
	WaitFrames(30);
	shop->cursor.anchor->active = saved_kind;
}

enum InnMessageId
{
	INN_MESSAGE_WELCOME = 0xd1c,
	INN_MESSAGE_STAY_COMPLETE,
	INN_MESSAGE_NOT_ENOUGH_COINS,
	INN_MESSAGE_GOODBYE,
	INN_MESSAGE_REST_COMPLETE,
};

#if EDITION_INTERNATIONAL
#define MESSAGE_WINDOW_ROWS 12
#else
#define MESSAGE_WINDOW_ROWS 11
#endif

// 宿代の倍率
static const s8 sInnPriceMultipliers[12] =
{
	2, 4, 7, 8, 8, 10, 12, 15, 20, 0, 18, 25,
};

s32 Inn_RoomPrice(s32 mode)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					active = 0;
	s32					factor = sInnPriceMultipliers[mode];
	s32					index;

	for (index = 0; index < shop->partyMemberCount; index++)
	{
		if (UnitGetFar(shop->partyMemberIds[index])->hp != 0)
			active++;
	}
	return factor * active;
}

s32 Inn_CheckIn(s32 mode, s32 objectId)
{
	struct InnRuntimeState *state;
	struct ObjectRuntime   *object;
	s32						win;
	s32						amount;
	s32						message_base;

	Shop_InitializeCursorWork();
	state = gMenuWork;
	state->active = 1;
	if (mode == 5)
		state->specialActive = 1;

	object = Object_GetByIdFar(objectId);
	state->resourceId = ((struct AnimationObject *)object->animation)->entries[0]->animId;
	win = UiWindow_CreateWithSideObjectFar(state->resourceId, 0, 0, 0);

	amount = Inn_RoomPrice(mode);
	UiWork_PushValueSlotFar(amount, 5);
	message_base = MsgInnWelcome;
	UiMessage_ShowAndWait(message_base);
	state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
	Shop_DrawMoney();

	if (UiMessage_ShowChoice(0) != 0)
	{
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
		UiWork_FinalizeFar(state->window, 2);
	}
	else if (amount > (u32)gGameState.coins)
	{
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
		UiWork_FinalizeFar(state->window, 2);
	}
	else
	{
		UiWork_FinalizeFar(state->window, 2);
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
		UiWork_FinalizeFar(win, 2);
		Inn_PlaySleep(amount);

		object = Object_GetByIdFar(objectId);
		state->resourceId = ((struct AnimationObject *)object->animation)->entries[0]->animId;
		win = UiWindow_CreateWithSideObjectFar(state->resourceId, 0, 0, 0);
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
	}

	UiWork_FinalizeFar(win, 2);
	Inn_Cleanup();
	return 0;
}

void Inn_PlaySleep(s32 room_price)
{
	s16					 objects[8];
	s32					 count;
	s32					 index;
	struct BattleUnit		*object;
	struct EventRuntime *state;

	count = Party_ListActiveOwnersFar(objects);
	Party_AdjustCoinsFar(-room_price);

	for (index = 0; index < count; index++)
	{
		object = UnitGetFar(objects[index]);
		if (object->hp != 0)
		{
			object->hp = object->maxHp;
			object->pp = object->maxPp;
			UnitRatioFar(objects[index]);
		}
	}

	state = (struct EventRuntime *)gEventWork;
	state->value1c0 = 0x209;
	state->value1c8 = 60;
	WaitFrames(20);
	Event_FinishScreenTransitionFar();
	Event_WaitTransitionFramesFar();
	AudioCommand_PlayFar(86);
	AudioCommand_WaitForStateByteClear();
	WaitFrames(10);
	Event_StartScreenTransitionFar();
	Event_WaitTransitionFramesFar();
	WaitFrames(30);
	((struct EventRuntime *)gEventWork)->value1c8 = 16;
}

// アイテム名の下の行の x（ドット）
#if EDITION_INTERNATIONAL
#define ITEM_NOTE_X 0
#else
#define ITEM_NOTE_X 8
#endif

s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selectedItem)
{
	struct ShopRuntime	*shop;
	struct RenderOutput *cursorAnchor;
	s32					 listWindow;
	s32					 selectedIndex = 0;
	s32					 redraw = 1;
	s32					 unitId = 0;
	s32					 item_slot;
	s32					 result = 0;

	Shop_InitializeCursorWork();
	shop = gMenuWork;
	shop->itemWindow = UiWindow_CreateFar(16, 12, 14, 8, 2);
	listWindow = UiWindow_CreateFar(0, 14, 13, 3, 2);
	cursorAnchor = RenderOutput_CreateFar(
		shop->cursorIcon, 0x40000000, (struct RenderInput *)listWindow, 0, result);
	cursorAnchor->active = 4;
	cursorAnchor->kind = result;
	ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursorAnchor;
	shop->mode = 12;
	PsynergyMenu_InitializeEntryObjectsFar(listWindow, 2, 0, 8, result);

	while (1)
	{
		if (redraw != 0)
		{
			redraw = 0;
			selectedIndex = (selectedIndex + shop->partyMemberCount) % shop->partyMemberCount;
			unitId = shop->partyMemberIds[selectedIndex];
			Shop_PlaceCursor((void *)listWindow, selectedIndex * 24 - 12, 0);
			shop->mode = 3;
			Shop_DrawParty(listWindow, selectedIndex, 0);
			Shop_DrawUnitGrid(shop->itemWindow, unitId);
		}

		WaitFrames(1);
		if ((gKeyState & KEY_A) != 0)
		{
			if (Inventory_CountFar(unitId) == 0)
			{
				AudioCommand_PlayFar(0x71);
				continue;
			}

			AudioCommand_PlayFar(0x70);
			item_slot = Shop_SelectUse(unitId);
			if (item_slot == -1)
			{
				shop->cursor.anchor->active = 4;
				shop->mode = 12;
				redraw = 1;
				continue;
			}
			*selected_unit = unitId;
			*selectedItem = item_slot;
			result = 0;
			break;
		}

		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(0x71);
			*selected_unit = -1;
			*selectedItem = -1;
			result = -1;
			break;
		}

		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(0x6f);
			selectedIndex--;
			redraw = 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(0x6f);
			selectedIndex++;
			redraw = 1;
		}
	}

	Menu_ReleaseEntryObjectsFar();
	UiWork_FinalizeFar(listWindow, 2);
	UiWork_FinalizeFar(shop->itemWindow, 2);
	WaitFrames(1);
	Inn_Cleanup();
	return result;
}
s32 Shop_SelectUse(s32 actor)
{
	struct ShopRuntime *shop = gMenuWork;

	s32				win2;
	s32				win1;
	struct BattleUnit *object;
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

	while (1)
	{
		if (redraw != 0)
		{
			redraw = 0;
			count = Inventory_CountFar(actor);
			if (selection > count - 1)
				selection = count - 1;

			flags = object->inventory[selection] & 0x1ff;
			window = (void *)shop->itemWindow;
			x = selection % 5 << 4;
			y = (selection / 5 << 4) + 8;
			Shop_PlaceCursor(window, x, y);
			shop->mode = 3;
			Shop_DrawUseItem(win1, actor, selection);
			Shop_DrawMsg(win2, flags + MsgItemPlainName);
		}

		WaitFrames(1);

		if ((gKeyState & KEY_A) != 0)
		{
			status = Inventory_CheckDiscardFar(actor, selection);
			if (status == 0)
			{
				AudioCommand_PlayFar(112);
				result = selection;
				break;
			}
			if (status == -4)
			{
				UiText_OpenMessageWindowFar(MsgCannotDropIt, 8, 1, 2);
			}
			else if (result == -3)
			{
				UiText_OpenMessageWindowFar(MsgCannotRemoveIt, 8, 1, 2);
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
			break;
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

	UiWork_FinalizeFar(win2, 2);
	UiWork_FinalizeFar(win1, 2);
	WaitFrames(1);
	return result;
}

void Shop_DrawUseItem(s32 window, s32 unitId, s32 item_id)
{
	struct BattleUnit *unit = UnitGetFar(unitId);
	s32				masked = unit->inventory[item_id] & 0x1ff;
	u32				entry = unit->inventory[item_id];
	s32				mult = (entry >> 11) + 1;

	if (window != 0)
	{
		s32 result;

		RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
		UiText_DrawCharacterAtOffsetFar(masked + MsgItemName, window, 0, 0);

		result = Inventory_CheckDiscardFar(unitId, item_id);
		if (result == -4)
		{
			UiText_DrawCharacterAtOffsetFar(MsgCannotDrop, window, ITEM_NOTE_X, 8);
		}
		else if (result == -3)
		{
			UiText_DrawCharacterAtOffsetFar(MsgCannotRemove, window, ITEM_NOTE_X, 8);
		}
		else
		{
			s32 qty;
			s32 total;

			qty = Shop_SalePrice(unit->inventory[item_id]);
			total = mult * qty;

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_IT)
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading - 5, window, 80, 8);
#elif defined(TBS_EDITION_DE)
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading, window, 0, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 32, 8);
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading - 5, window, 72, 8);
#elif defined(TBS_EDITION_ES)
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading - 5, window, 88, 8);
#elif defined(TBS_EDITION_FR)
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 32, 8);
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading - 5, window, 72, 8);
#else
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading, window, 8, 8);
			UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
			UiText_DrawCharacterAtOffsetFar(MsgPriceHeading - 1, window, 80, 8);
#endif
		}
	}
}
