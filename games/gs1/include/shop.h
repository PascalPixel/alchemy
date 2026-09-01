#ifndef ALCHEMY_SHOP_H
#define ALCHEMY_SHOP_H

#include "global_cells.h"
#include "item.h"

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
    u8 unknown_390[2];
    u16 previous_page_icon;             /* 0x392 */
    u16 next_page_icon;                 /* 0x394 */
    u8 unknown_396[0x10];
    s8 stock_count;                     /* 0x3a6 */
    s8 party_member_count;              /* 0x3a7 */
    u8 mode;                            /* 0x3a8 */
    u8 unknown_3a9;
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

#define SHOP_RUNTIME (*(struct ShopRuntime **)ADDR_03001F2C)
#define SHOP_PARTY_STATE (*(struct ShopPartyState *)0x02000240)

void Func_080b09fc(
    struct ShopCursor *cursor,
    s32 target_x,
    s32 target_y,
    s8 kind);
void Func_080b0a6c(void *window, s32 x, s32 y);
void Func_080b0fa4(s32 window, s32 selected);
void Func_080b10cc(void);
void Func_080b110c(
    s32 window,
    s32 item_name_message,
    s32 price,
    s32 price_mode);
void Func_080b11a4(s32 window, s32 message);
void Func_080b11c4(s32 window, s32 selected, s32 requirement);
void Func_080b1470(s32 window, s32 unit_id, s32 item_id);
s32 Func_080b153c(s32 unit_id, s32 item_id);
void Func_080b17e4(s32 unit_id, s32 item_id, s32 quantity);
s32 Func_080b196c(s32 unit_id, s32 slot);
s32 Func_080b19cc(s32 item_id);
s32 Func_080b1a14(void);
void Func_080b1dec(s32 window, s32 unit_id);
s32 Func_080b1e80(s32 unit_id, s32 slot);
s32 Func_080b20e8(s32 item_id);
s32 Func_080b2110(void);
void Func_080b386c(s32 window, s32 unit_id, s32 item_slot);

#define Shop_SetCursorPosition        Func_080b09fc
#define Shop_PlaceCursor              Func_080b0a6c
#define Shop_DrawStockPage            Func_080b0fa4
#define Shop_DrawMoney                Func_080b10cc
#define Shop_DrawItemPrice            Func_080b110c
#define Shop_DrawMessage              Func_080b11a4
#define Shop_UpdatePartyMemberList    Func_080b11c4
#define Shop_DrawPartyMemberItems     Func_080b1470
#define Shop_SelectPurchaseQuantity   Func_080b153c
#define Shop_CompletePurchase         Func_080b17e4
#define Shop_SellReplacedItem         Func_080b196c
#define Shop_ComputeSalePrice         Func_080b19cc
#define Shop_SelectPartyMember        Func_080b1a14
#define Shop_DrawPartyMemberItemGrid  Func_080b1dec
#define Shop_SelectSaleQuantity       Func_080b1e80
#define Shop_ComputeRepairPrice       Func_080b20e8
#define Shop_RunPartyMemberSelection  Func_080b2110
#define Shop_DrawUseItemDetails       Func_080b386c

#endif
