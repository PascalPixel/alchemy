#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

#include "unit/menu/shop/shop.h"
#include "global_cells.h"
#include "item.h"
#include "layout_guard.h"
#include "render_input.h"
#include "effect_slot.h"
#include "animspr.h"

struct ShopCursor
{
	struct RenderOutput *anchor;
	u16					 x;
	u16					 y;
	s16					 target_x;
	s16					 target_y;
	s8					 active;
	s8					 kind;
};

struct ShopRuntime
{
	u8						unknown_000[0x0c];
	s32						money_window;
	u8						unknown_010[0x10];
	s32						item_window;
	u8						unknown_024[0xf0];
	struct AnimationObject *party_member_icons[8];
	s16						party_member_x[8];
	s16						party_member_y[8];
	s32						party_member_scale[8];
	u8						unknown_174[0xf8];
	s16						stock_item_ids[129];
	s16						party_member_ids[9];
	struct ShopCursor		cursor;
	u16						cursor_icon;
	u16						previous_page_icon;
	u16						next_page_icon;
	u16						gem_icon;
	u16						stat_up_icon;
	u16						stat_down_icon;
	u16						unknown_39c;
	u16						selected_item;
	u8						unknown_3a0[4];
	u16						keeper_resource;
	s8						stock_count;
	s8						party_member_count;
	u8						mode;
	u8						shop_type;
	s8						party_action;
	s8						burst_member;
	u8						warrior_shop;
	u8						unknown_3ad[3];
	struct EffectSlot		effects[24];
};

LAYOUT_SIZE_GUARD(ShopCursor_Size, struct ShopCursor, 0x10);
LAYOUT_OFFSET_GUARD(ShopCursor_TargetX, struct ShopCursor, target_x, 0x08);
LAYOUT_OFFSET_GUARD(ShopCursor_Active, struct ShopCursor, active, 0x0c);
LAYOUT_SIZE_GUARD(ShopRuntime_Size, struct ShopRuntime, 0xa70);
LAYOUT_OFFSET_GUARD(ShopRuntime_MoneyWindow, struct ShopRuntime, money_window, 0x00c);
LAYOUT_OFFSET_GUARD(ShopRuntime_ItemWindow, struct ShopRuntime, item_window, 0x020);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMemberIcons, struct ShopRuntime, party_member_icons, 0x114);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMemberScale, struct ShopRuntime, party_member_scale, 0x154);
LAYOUT_OFFSET_GUARD(ShopRuntime_StockItems, struct ShopRuntime, stock_item_ids, 0x26c);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMembers, struct ShopRuntime, party_member_ids, 0x36e);
LAYOUT_OFFSET_GUARD(ShopRuntime_Cursor, struct ShopRuntime, cursor, 0x380);
LAYOUT_OFFSET_GUARD(ShopRuntime_StockCount, struct ShopRuntime, stock_count, 0x3a6);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyMemberCount, struct ShopRuntime, party_member_count, 0x3a7);
LAYOUT_OFFSET_GUARD(ShopRuntime_PartyAction, struct ShopRuntime, party_action, 0x3aa);
LAYOUT_OFFSET_GUARD(ShopRuntime_MemberX, struct ShopRuntime, party_member_x, 0x134);
LAYOUT_OFFSET_GUARD(ShopRuntime_MemberY, struct ShopRuntime, party_member_y, 0x144);
LAYOUT_OFFSET_GUARD(ShopRuntime_BurstMember, struct ShopRuntime, burst_member, 0x3ab);
LAYOUT_OFFSET_GUARD(ShopRuntime_WarriorShop, struct ShopRuntime, warrior_shop, 0x3ac);
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
