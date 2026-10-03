#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

#include "GLOBAL_CELLS.H"
#include "ITEM.H"
#include "LAYOUT_GUARD.H"
#include "RENDER_INPUT.H"
#include "EFFECT_SLOT.H"

/* The animation work reached through a keeper's ObjectRuntime.animation.
   Its resource table starts with the id shown beside the shop's messages. */
struct ShopKeeperAnimation {
    u8 unknown_00[0x28];
    const u16 *resource;
};

LAYOUT_OFFSET_GUARD(ShopKeeperAnimation_Resource,
    struct ShopKeeperAnimation, resource, 0x28);

struct ShopCursor {
    struct RenderOutput *anchor;
    u16 x;
    u16 y;
    s16 target_x;
    s16 target_y;
    s8 active;
    s8 kind;
};

struct ShopRuntime {
    u8 unknown_000[0x0c];
    s32 money_window;                   /* 0x00c */
    u8 unknown_010[0x10];
    s32 item_window;                    /* 0x020 */
    u8 unknown_024[0xf0];
    void *party_member_icons[8];        /* 0x114 */
    s16 party_member_x[8];              /* 0x134 */
    s16 party_member_y[8];              /* 0x144 */
    s32 party_member_scale[8];          /* 0x154 */
    u8 unknown_174[0xf8];
    s16 stock_item_ids[129];            /* 0x26c */
    s16 party_member_ids[9];            /* 0x36e */
    struct ShopCursor cursor;           /* 0x380 */
    u16 cursor_icon;                    /* 0x390 */
    u16 previous_page_icon;             /* 0x392 */
    u16 next_page_icon;                 /* 0x394 */
    u16 gem_icon;                       /* 0x396 */
    u16 stat_up_icon;                   /* 0x398 */
    u16 stat_down_icon;                 /* 0x39a */
    u16 unknown_39c;
    u16 selected_item;                  /* 0x39e */
    u8 unknown_3a0[4];
    u16 keeper_resource;                /* 0x3a4 */
    s8 stock_count;                     /* 0x3a6 */
    s8 party_member_count;              /* 0x3a7 */
    u8 mode;                            /* 0x3a8 */
    u8 shop_type;                       /* 0x3a9 */
    s8 party_action;                    /* 0x3aa */
    s8 burst_member;                    /* 0x3ab, -1 when no member bursts */
    u8 warrior_shop;                    /* 0x3ac */
    u8 unknown_3ad[3];
    struct EffectSlot effects[24];       /* 0x3b0 */
};

LAYOUT_SIZE_GUARD(
    ShopCursor_Size,
    struct ShopCursor,
    0x10);
LAYOUT_OFFSET_GUARD(
    ShopCursor_TargetX,
    struct ShopCursor,
    target_x,
    0x08);
LAYOUT_OFFSET_GUARD(
    ShopCursor_Active,
    struct ShopCursor,
    active,
    0x0c);
LAYOUT_SIZE_GUARD(
    ShopRuntime_Size,
    struct ShopRuntime,
    0xa70);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_MoneyWindow,
    struct ShopRuntime,
    money_window,
    0x00c);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_ItemWindow,
    struct ShopRuntime,
    item_window,
    0x020);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_PartyMemberIcons,
    struct ShopRuntime,
    party_member_icons,
    0x114);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_PartyMemberScale,
    struct ShopRuntime,
    party_member_scale,
    0x154);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_StockItems,
    struct ShopRuntime,
    stock_item_ids,
    0x26c);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_PartyMembers,
    struct ShopRuntime,
    party_member_ids,
    0x36e);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_Cursor,
    struct ShopRuntime,
    cursor,
    0x380);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_StockCount,
    struct ShopRuntime,
    stock_count,
    0x3a6);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_PartyMemberCount,
    struct ShopRuntime,
    party_member_count,
    0x3a7);
LAYOUT_OFFSET_GUARD(
    ShopRuntime_PartyAction,
    struct ShopRuntime,
    party_action,
    0x3aa);
LAYOUT_OFFSET_GUARD(ShopRuntime_MemberX, struct ShopRuntime, party_member_x, 0x134);
LAYOUT_OFFSET_GUARD(ShopRuntime_MemberY, struct ShopRuntime, party_member_y, 0x144);
LAYOUT_OFFSET_GUARD(ShopRuntime_BurstMember, struct ShopRuntime, burst_member, 0x3ab);
LAYOUT_OFFSET_GUARD(ShopRuntime_WarriorShop, struct ShopRuntime, warrior_shop, 0x3ac);
LAYOUT_OFFSET_GUARD(ShopRuntime_Effects, struct ShopRuntime, effects, 0x3b0);

/* The shop's 16 by 16 cursor sprites: the pointing hand, the gem, the small
   and large up and down arrows. */
extern const u8 Shop_HandTiles[];
extern const u8 Shop_GemTiles[];
extern const u8 Shop_SmallDownArrowTiles[];
extern const u8 Shop_SmallUpArrowTiles[];
extern const u8 Shop_UpArrowTiles[];
extern const u8 Shop_DownArrowTiles[];

/* The blank price and quantity strips of eight tiles that the digits are
   drawn over. */
extern const u8 Shop_PriceTiles[];
extern const u8 Shop_QuantityTiles[];

void Shop_SetCursor(
    struct ShopCursor *cursor,
    s32 target_x,
    s32 target_y,
    s8 kind);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Shop_DrawStock(s32 window, s32 selected);
void Shop_DrawMoney(void);
void Shop_DrawItemPrice(
    s32 window,
    s32 item_name_message,
    s32 price,
    s32 price_mode);
void Shop_DrawMsg(s32 window, s32 message);
void Shop_DrawParty(s32 window, s32 selected, s32 requirement);
void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id);
s32 Shop_SelBuyNum(s32 unit_id, s32 item_id);
void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity);
s32 Shop_ConfirmEquip(s32 unit_id, s32 slot);
s32 Shop_SellOld(s32 unit_id, s32 slot);
s32 Shop_SelRepair(s32 unit_id);
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
void ShopCursor_Advance(struct ShopCursor *cursor);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y);
s32 Shop_SalePrice(s32 item_id);
s32 Shop_PickUnit(void);
void Shop_DrawUnitGrid(s32 window, s32 unit_id);
s32 Shop_SelSellNum(s32 unit_id, s32 slot);
s32 Shop_RepairPrice(s32 item_id);
s32 Shop_SelUnit(void);
void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_slot);

#endif
