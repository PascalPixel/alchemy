#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/buysell.h"
#include "unit/menu/shop/wares.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/objvram.h"
#include "unit/runtime/sched.h"
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
	output = state->stock_item_ids;
	do
	{
		mode = state->shop_type;
		if (mode == Item_GetEquipmentGroupFar(index) && Item_AdjustCounterFar(index, 0) != 0)
		{
			*output = index;
			count++;
			output++;
		}
		index++;
	} while (index <= 0x1ff);
	state->stock_item_ids[count] = 0;
	state->stock_count = count;
	return count;
}

void Shop_StepCursor(void)
{
	ShopCursor_Advance(&((struct ShopRuntime *)gMenuWork)->cursor);
}

void Shop_InitializeCursorWork(void)
{
	struct ShopRuntime *work;
	volatile s32		zero;
	s32					slot;

	work = Runtime_AllocateHeapBlock(55, 0xa70);
	Battle_ResetEffectCounterFar();
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x8500029c, REG_DMA3);
	work->mode = 12;
	work->party_member_count = Party_ListActiveOwnersFar(work->party_member_ids);
	slot = Resource_FindFreeEntry();
	work->cursor_icon = slot;
	VramBlock_LoadCached(slot, 128, Shop_HandTiles);
	slot = Resource_FindFreeEntry();
	work->previous_page_icon = slot;
	VramBlock_LoadCached(slot, 128, Shop_UpArrowTiles);
	slot = Resource_FindFreeEntry();
	work->next_page_icon = slot;
	VramBlock_LoadCached(slot, 128, Shop_DownArrowTiles);
	slot = Resource_FindFreeEntry();
	work->gem_icon = slot;
	VramBlock_LoadCached(slot, 128, Shop_GemTiles);
	slot = Resource_FindFreeEntry();
	work->stat_down_icon = slot;
	VramBlock_LoadCached(slot, 128, Shop_SmallDownArrowTiles);
	slot = Resource_FindFreeEntry();
	work->stat_up_icon = slot;
	VramBlock_LoadCached(slot, 128, Shop_SmallUpArrowTiles);
	Scheduler_AddOrUpdateCallback(Shop_StepCursor, 0xc80);
}

void Inn_Cleanup(void)
{
	struct ShopRuntime *state;

	state = gMenuWork;
	Scheduler_RemoveCallback(Shop_StepCursor);
	UiWork_FinalizePendingCoreFar();
	Resource_ResetEntry(state->cursor_icon);
	Resource_ResetEntry(state->previous_page_icon);
	Resource_ResetEntry(state->next_page_icon);
	Resource_ResetEntry(state->gem_icon);
	Resource_ResetEntry(state->stat_up_icon);
	Resource_ResetEntry(state->stat_down_icon);
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
	shop->shop_type = EventTable_GetRowType(row);
	if (row == 16)
		shop->warrior_shop = 1;
	if (row == 17)
		shop->warrior_shop = 1;
	if (row == 18)
		shop->warrior_shop = 1;
	shop->keeper_resource =
		((struct AnimationObject *)((struct ObjectRuntime *)Object_GetByIdFar(keeper_id))
				->animation)
			->entries[0]
			->anim_id;
	window = UiWindow_CreateWithSideObjectFar(shop->keeper_resource, 0, 0, 0);
	if (window == 0)
		window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
	anchor =
		RenderOutput_CreateFar(shop->cursor_icon, 0x40000000, (struct RenderInput *)window, 0, 0);
	anchor->active = 1;
	anchor->kind = 0;
	ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = anchor;
	UiMessage_ShowAndWait((s32)MsgWeaponShopWelcome);
loop:
	{
		choice = Menu_SelectEntry11To14Far(choice);
		shop->party_action = choice;
		if (choice == 0)
		{
			shop->stock_count = EventTable_CopyRowHeader(row, shop->stock_item_ids);
			UiMessage_ShowAndWait((s32)MsgWhatWouldYouLike);
			Shop_SelBuy();
		}
		else if (choice == 1)
		{
			UiMessage_ShowAndWait((s32)MsgWhatToSell);
			Shop_PickUnit();
		}
		else if (choice == 2)
		{
			if (AbilityMenu_BuildAvailableList() != 0)
			{
				UiMessage_ShowAndWait((s32)MsgConnoisseur);
				Shop_SelBuy();
			}
			else
			{
				UiMessage_ShowAndWait((s32)MsgOutOfStock);
				WaitFrames(1);
			}
		}
		else if (choice == 3)
		{
			UiMessage_ShowAndWait((s32)MsgFixDamaged);
			Shop_SelUnit();
		}
		else
		{
			goto done;
		}
		ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
		UiMessage_ShowAndWait((s32)MsgAnythingElse);
		goto loop;
	}
done:
	UiMessage_ShowAndWait((s32)MsgShopFarewell);
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
	gGameState.shop_gifts = 0x1c;
	Inventory_EquipFar(1, Inventory_AddItemFar(1, 0x48d));
	Inventory_EquipFar(0, Inventory_AddItemFar(0, 0x40b));
	Inventory_AddItemFar(2, 0xe7);
	UnitGetFar(3)->doku = 1;
	UnitGetFar(5)->doku = 1;
	UnitGetFar(2)->akuryo = 1;
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

struct Record_080b06c0
{
	u8 filler0[4];
	u8 values[21];
};

void UiMessage_ShowAndWait(s32 arg0)
{
	struct ShopRuntime *state = gMenuWork;
	s32					value = BattleFx_GetResourceIdFar(state->keeper_resource);
	s32					result = arg0;
	s8					mode;

	UiWork_FinalizePendingCoreFar();
	mode = state->shop_type;
	if (mode == 2)
		result += MsgArmorShopWelcome - MsgWeaponShopWelcome;
	if (mode == 0)
		result += MsgItemShopWelcome - MsgWeaponShopWelcome;
	if ((s8)state->warrior_shop != 0)
		result += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
	UiText_OpenMessageWindowFar(result, 5, 0, (value << 16) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);
	WaitFrames(1);
}

void UiMessage_ShowAndRestoreState(s32 message_id)
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
	no = message_id;
	variant = BattleFx_GetResourceIdFar(state->keeper_resource);
	mode = state->shop_type;
	if (mode == 2)
	{
		no += MsgArmorShopWelcome - MsgWeaponShopWelcome;
	}
	if (mode == 0)
	{
		no += MsgItemShopWelcome - MsgWeaponShopWelcome;
	}
	flag = state->warrior_shop;
	if (flag != 0)
	{
		no += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
	}
	(*slot)->active = 0xDU;
	UiWork_FinalizePendingCoreFar();
	UiText_OpenMessageWindowFar(no, 5, 0, (variant << 0x10) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
	{
		WaitFrames(1U);
	}
	WaitFrames(1U);
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
static const u16 Shop_SelectorTileOffsets[30] =
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
	u32		   shifted = selector << 4;
	const u16 *offset;

	selector = shifted + 1;

	if (count > 0)
	{
		offset = Shop_SelectorTileOffsets;
		do
		{
			struct Record_080b06c0 *record = (struct Record_080b06c0 *)(base + *offset++);
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
static const u16 Shop_GlyphOffsets[5] =
{
	0xc0, 0x90, 0x80, 0x50, 0x40,
};

void Shop_CopyGlyphs(s32 arg0, u8 *arg1, u32 arg2)
{
	u8 *src = Shop_GlyphBytes + (arg0 << 5);
	u8 *dst = arg1 + Shop_GlyphOffsets[arg2] + 2;
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
	Dma_Set(Shop_PriceTiles, buf, 0x84000040, REG_DMA3);
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
	x = cursor->x + (cursor->target_x - (s16)cursor->x) * step / kind;
	sprite->x = x;
	sprite->oam.x = x;
	y = cursor->y + (cursor->target_y - (s16)cursor->y) * step / kind;
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
		delta = sprite->x - cursor->target_x;
		step = delta / 4;
		if (step < 0)
			step = -step;
		if (delta > 0)
		{
			if (step != 0)
				sprite->x = sprite->x - step;
			else
				sprite->x = sprite->x - 1;
		}
		else
		{
			if (delta >= 0)
				goto move_y;
			if (step != 0)
				sprite->x = sprite->x + step;
			else
				sprite->x = sprite->x + 1;
		}
		sprite->oam.x = sprite->x;

	move_y:
		delta = sprite->y - cursor->target_y;
		step = delta / 4;
		if (step < 0)
			step = -step;
		if (delta > 0)
		{
			if (step != 0)
				sprite->y = sprite->y - step;
			else
				sprite->y = sprite->y - 1;
		}
		else
		{
			if (delta >= 0)
				return;
			if (step != 0)
				sprite->y = sprite->y + step;
			else
				sprite->y = sprite->y + 1;
		}
		sprite->oam.y = sprite->y;
	}
}

void Shop_SetCursor(struct ShopCursor *cursor, s32 target_x, s32 target_y, s32 kind)
{
	struct RenderOutput *anchor = cursor->anchor;

	cursor->x = anchor->x;
	cursor->y = anchor->y;
	cursor->target_x = target_x;
	cursor->target_y = target_y;
	cursor->kind = kind;
	cursor->active = 0;
}

void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y)
{

	{
		struct RenderOutput *sprite = cursor->anchor;

		cursor->target_x = x;
		cursor->kind = 1;
		cursor->active = 0;
		cursor->x = x;
		sprite->oam.x = sprite->x = x;
	}
	{
		struct RenderOutput *sprite;

		cursor->target_y = y;
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
