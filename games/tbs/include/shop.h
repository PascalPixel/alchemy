#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

#include "unit/shop/shop.h"
#include "global_cells.h"
#include "item.h"
#include "layout_guard.h"
#include "render_input.h"
#include "effect_slot.h"
#include "animspr.h"
#include "cursor.h"

struct ShopRuntime
{
	u8						unk000[0x0c];
	s32						moneyWindow;
	u8						unk010[0x10];
	s32						itemWindow;
	u8						unk024[0xf0];
	struct AnimationObject *partyMemberIcons[8];
	s16						partyMemberX[8];
	s16						partyMemberY[8];
	s32						partyMemberScale[8];
	u8						unk174[0xf8];
	s16						stockItemIds[129];
	s16						partyMemberIds[9];
	struct ShopCursor		cursor;
	u16						cursorIcon;
	u16						previousPageIcon;
	u16						nextPageIcon;
	u16						gemIcon;
	u16						statUpIcon;
	u16						statDownIcon;
	u16						unk39c;
	u16						selectedItem;
	u8						unk3a0[4];
	u16						keeperResource;
	s8						stockCount;
	s8						partyMemberCount;
	u8						mode;
	u8						shopType;
	s8						partyAction;
	s8						burstMember;
	u8						warriorShop;
	u8						unk3ad[3];
	struct EffectSlot		effects[24];
};

LAYOUT_SIZE_GUARD(ShopRuntime_Size, struct ShopRuntime, 0xa70);
LAYOUT_OFFSET_GUARD(ShopRuntime_MoneyWindow, struct ShopRuntime, moneyWindow, 0x00c);
LAYOUT_OFFSET_GUARD(ShopRuntime_ItemWindow, struct ShopRuntime, itemWindow, 0x020);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMemberIcons, struct ShopRuntime, partyMemberIcons, 0x114);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMemberScale, struct ShopRuntime, partyMemberScale, 0x154);
LAYOUT_OFFSET_GUARD(ShopRuntime_StockItems, struct ShopRuntime, stockItemIds, 0x26c);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMembers, struct ShopRuntime, partyMemberIds, 0x36e);
LAYOUT_OFFSET_GUARD(ShopRuntime_Cursor, struct ShopRuntime, cursor, 0x380);
LAYOUT_OFFSET_GUARD(ShopRuntime_StockCount, struct ShopRuntime, stockCount, 0x3a6);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMemberCount, struct ShopRuntime, partyMemberCount, 0x3a7);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyAction, struct ShopRuntime, partyAction, 0x3aa);
LAYOUT_OFFSET_GUARD(ShopRuntime_MemberX, struct ShopRuntime, partyMemberX, 0x134);
LAYOUT_OFFSET_GUARD(ShopRuntime_MemberY, struct ShopRuntime, partyMemberY, 0x144);
LAYOUT_OFFSET_GUARD(ShopRuntime_BurstMember, struct ShopRuntime, burstMember, 0x3ab);
LAYOUT_OFFSET_GUARD(ShopRuntime_WarriorShop, struct ShopRuntime, warriorShop, 0x3ac);
LAYOUT_OFFSET_GUARD(ShopRuntime_Effects, struct ShopRuntime, effects, 0x3b0);

extern const u8 Shop_HandTiles[];
extern const u8 Shop_GemTiles[];
extern const u8 Shop_SmallDownArrowTiles[];
extern const u8 Shop_SmallUpArrowTiles[];
extern const u8 Shop_UpArrowTiles[];
extern const u8 Shop_DownArrowTiles[];

extern const u8 Shop_PriceTiles[];
extern const u8 Shop_QuantityTiles[];

#endif
