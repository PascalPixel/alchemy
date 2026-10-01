#ifndef ALCHEMY_INVENTORY_MENU_H
#define ALCHEMY_INVENTORY_MENU_H

#include "LAYOUT_GUARD.H"
#include "MENU_RESULT.H"

struct InventoryMenuIcon {
    struct InventoryMenuIcon *next; /* 0x00 */
    u8 unknown_04;
    u8 state;                       /* 0x05 */
    u16 x;                          /* 0x06 */
    u16 y;                          /* 0x08 */
    u8 unknown_0a[4];
    u8 render_target;               /* 0x0e */
};

struct InventoryMenuState {
    u8 unknown_000[0x20];
    s32 item_window;                /* 0x020 */
    s32 status_window;              /* 0x024 */
    u8 unknown_028[4];
    s32 info_window;                /* 0x02c */
    u8 unknown_030[0x18];
    struct InventoryMenuIcon *entry_icons[32]; /* 0x048 */
    u8 unknown_0c8[0x44];
    s32 message_window;             /* 0x10c */
    u8 unknown_110[0x64];
    u16 selected_slot;              /* 0x174 */
    u16 equip_item;                 /* 0x176 */
    u16 selected_item;              /* 0x178 */
    u8 unknown_17a[2];
    struct InventoryMenuIcon *cursor; /* 0x17c */
    u8 unknown_180[0x48];
    u16 items[32];                  /* 0x1c8 */
    u16 owner_ids[8];               /* 0x208 */
    u8 item_count;                  /* 0x218 */
    u8 party_count;                 /* 0x219 */
    u8 item_owner;                  /* 0x21a */
    u8 target_owner;                /* 0x21b */
    struct InventoryMenuIcon *selected_item_icon; /* 0x21c */
    u8 unknown_220[2];
    s16 completion_flag;            /* 0x222 */
    u8 unknown_224[0x36];
    s16 message_offset;             /* 0x25a */
};

LAYOUT_OFFSET_GUARD(
    InventoryMenuIcon_State,
    struct InventoryMenuIcon,
    state,
    0x05);
LAYOUT_OFFSET_GUARD(
    InventoryMenuIcon_RenderTarget,
    struct InventoryMenuIcon,
    render_target,
    0x0e);
LAYOUT_OFFSET_GUARD(
    InventoryMenuIcon_X,
    struct InventoryMenuIcon,
    x,
    0x06);
LAYOUT_OFFSET_GUARD(
    InventoryMenuIcon_Y,
    struct InventoryMenuIcon,
    y,
    0x08);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_ItemWindow,
    struct InventoryMenuState,
    item_window,
    0x020);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_InfoWindow,
    struct InventoryMenuState,
    info_window,
    0x02c);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_EntryIcons,
    struct InventoryMenuState,
    entry_icons,
    0x048);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_MessageWindow,
    struct InventoryMenuState,
    message_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedSlot,
    struct InventoryMenuState,
    selected_slot,
    0x174);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedItem,
    struct InventoryMenuState,
    selected_item,
    0x178);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_Cursor,
    struct InventoryMenuState,
    cursor,
    0x17c);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_Items,
    struct InventoryMenuState,
    items,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_ItemCount,
    struct InventoryMenuState,
    item_count,
    0x218);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_OwnerIds,
    struct InventoryMenuState,
    owner_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_ItemOwner,
    struct InventoryMenuState,
    item_owner,
    0x21a);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_TargetOwner,
    struct InventoryMenuState,
    target_owner,
    0x21b);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedItemIcon,
    struct InventoryMenuState,
    selected_item_icon,
    0x21c);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_MessageOffset,
    struct InventoryMenuState,
    message_offset,
    0x25a);
LAYOUT_SIZE_GUARD(
    InventoryMenuState_Size,
    struct InventoryMenuState,
    0x25c);

extern struct InventoryMenuState *gMenuWork;
extern char MsgItemMenuEmpty;

s32 ItemMenu_IsSpecial(s32 item);
void ItemMenu_OpenCategory(s32 owner);
void ItemMenu_DrawMsg(s32 unused, s32 message);
void Menu_HideEmptyEntryIcons(const u16 *items);
s32 ItemMenu_Count(s32 owner);
s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item);
struct BattleUnit;
s32 ItemMenu_Collect(struct BattleUnit *owner, u16 *items, s32 mode);
void ItemMenu_DrawIcons(u16 *items, s32 style);
void ItemMenu_RefreshOwner(s32 owner, s32 mode);
void InventoryMenu_NoOp(void);
s32 ItemMenu_Use(void);
void ItemMenu_HideAllIcons(void);
void ItemMenu_HidePageIcons(void);
void ItemMenu_Close(void);
void ItemMenu_Init(void);
s32 ItemMenu_PrepOwner(s32 party_slot);
s32 ItemMenu_CmdCursorX(s32 column, s32 row);
s32 ItemMenu_CmdCursorY(s32 column, s32 row);
void ItemMenu_BuildCmd(s8 *command_states);
void ItemMenu_DrawCmd(void *command_states, s32 window);
void ItemMenu_TryBreak(void);
s32 ItemMenu_OpenDetail(s32 item_index);
void ItemMenu_DrawStat(s32 delta, s32 unused, s32 window, s32 x, s32 y);
void ItemMenu_SetItemWin5(void);
void ItemMenu_SetItemWin3(void);
void ItemMenu_SetMsgWin5(void);
void ItemMenu_SetMsgWin6(void);
void ItemMenu_SetMsgWin7(void);
void ItemMenu_SetMsgWin3(void);
void ItemMenu_DrawItemHead(void);
s32 Item_Use(s32 slot, s32 owner, s32 target);
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index);
s32 ItemMenu_DrawNamePage(s32 window, s32 unused, const struct MenuResult *state);
s32 InventoryMenu_ItemNamePageReturnTrue(void);
void ItemMenu_DrawCategory(s32 window, s32 owner, s32 mode);
void Resource_LoadByModeIntoSlotFar(s32 style, u16 item, u8 target, s32 flags);

#endif
