#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

#include "GLOBAL_CELLS.H"
#include "ITEM.H"

struct ShopCursorAnchor {
    u8 unknown_00[5];
    u8 kind;
    u16 x;
    u16 y;
};

struct ShopCursor {
    struct ShopCursorAnchor *anchor;
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
    void *party_member_icons[9];        /* 0x114 */
    u8 unknown_138[0x1c];
    s32 party_member_styles[9];         /* 0x154 */
    u8 unknown_178[0xf4];
    s16 stock_item_ids[129];            /* 0x26c */
    s16 party_member_ids[9];            /* 0x36e */
    struct ShopCursor cursor;           /* 0x380 */
    u16 cursor_icon;                    /* 0x390 */
    u16 previous_page_icon;             /* 0x392 */
    u16 next_page_icon;                 /* 0x394 */
    u16 unknown_396;
    u16 stat_down_icon;                 /* 0x398 */
    u16 stat_up_icon;                   /* 0x39a */
    u16 unknown_39c;
    u16 selected_item;                  /* 0x39e */
    u8 unknown_3a0[4];
    u16 keeper_resource;                /* 0x3a4 */
    s8 stock_count;                     /* 0x3a6 */
    s8 party_member_count;              /* 0x3a7 */
    u8 mode;                            /* 0x3a8 */
    u8 shop_type;                       /* 0x3a9 */
    s8 party_action;                    /* 0x3aa */
};

struct ShopPartyState {
    u8 unknown_00[0x10];
    s32 money;                          /* 0x10 */
};

LAYOUT_SIZE_GUARD(
    ShopCursorAnchor_Size,
    struct ShopCursorAnchor,
    0x0c);
LAYOUT_OFFSET_GUARD(
    ShopCursorAnchor_Kind,
    struct ShopCursorAnchor,
    kind,
    0x05);
LAYOUT_OFFSET_GUARD(
    ShopCursorAnchor_X,
    struct ShopCursorAnchor,
    x,
    0x06);
LAYOUT_OFFSET_GUARD(
    ShopCursorAnchor_Y,
    struct ShopCursorAnchor,
    y,
    0x08);
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
    0x3ac);
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
    ShopRuntime_PartyMemberStyles,
    struct ShopRuntime,
    party_member_styles,
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
LAYOUT_OFFSET_GUARD(
    ShopPartyState_Money,
    struct ShopPartyState,
    money,
    0x10);
LAYOUT_SIZE_GUARD(
    ShopPartyState_Size,
    struct ShopPartyState,
    0x14);

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
s32 Shop_SalePrice(s32 item_id);
s32 Shop_PickUnit(void);
void Shop_DrawUnitGrid(s32 window, s32 unit_id);
s32 Shop_SelSellNum(s32 unit_id, s32 slot);
s32 Shop_RepairPrice(s32 item_id);
s32 Shop_SelUnit(void);
void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_slot);

#endif
