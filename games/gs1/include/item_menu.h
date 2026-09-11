#ifndef ALCHEMY_ITEM_MENU_H
#define ALCHEMY_ITEM_MENU_H

#include "layout_guard.h"
#include "menu_result.h"

struct ItemMenuIcon {
    u8 unknown_00[5];
    u8 state;                       /* 0x05 */
    u16 x;                          /* 0x06 */
    u16 y;                          /* 0x08 */
    u8 unknown_0a[4];
    u8 render_target;               /* 0x0e */
};

struct ItemMenuState {
    u8 unknown_000[0x20];
    s32 item_window;                /* 0x020 */
    u8 unknown_024[8];
    s32 info_window;                /* 0x02c */
    u8 unknown_030[0x18];
    struct ItemMenuIcon *entry_icons[32]; /* 0x048 */
    u8 unknown_0c8[0x44];
    s32 message_window;             /* 0x10c */
    u8 unknown_110[0x64];
    u16 selected_slot;              /* 0x174 */
    u8 unknown_176[2];
    u16 selected_item;              /* 0x178 */
    u8 unknown_17a[2];
    struct ItemMenuIcon *cursor; /* 0x17c */
    u8 unknown_180[0x48];
    u16 items[32];                  /* 0x1c8 */
    u16 owner_ids[8];               /* 0x208 */
    u8 item_count;                  /* 0x218 */
    u8 party_count;                 /* 0x219 */
    u8 item_owner;                  /* 0x21a */
    u8 target_owner;                /* 0x21b */
    struct ItemMenuIcon * volatile selected_item_icon; /* 0x21c */
    u8 unknown_220[2];
    s16 completion_flag;            /* 0x222 */
    u8 unknown_224[0x36];
    s16 message_offset;             /* 0x25a */
};

LAYOUT_OFFSET_GUARD(
    ItemMenuIcon_State,
    struct ItemMenuIcon,
    state,
    0x05);
LAYOUT_OFFSET_GUARD(
    ItemMenuIcon_RenderTarget,
    struct ItemMenuIcon,
    render_target,
    0x0e);
LAYOUT_OFFSET_GUARD(
    ItemMenuIcon_X,
    struct ItemMenuIcon,
    x,
    0x06);
LAYOUT_OFFSET_GUARD(
    ItemMenuIcon_Y,
    struct ItemMenuIcon,
    y,
    0x08);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_ItemWindow,
    struct ItemMenuState,
    item_window,
    0x020);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_InfoWindow,
    struct ItemMenuState,
    info_window,
    0x02c);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_EntryIcons,
    struct ItemMenuState,
    entry_icons,
    0x048);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_MessageWindow,
    struct ItemMenuState,
    message_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_SelectedSlot,
    struct ItemMenuState,
    selected_slot,
    0x174);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_SelectedItem,
    struct ItemMenuState,
    selected_item,
    0x178);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_Cursor,
    struct ItemMenuState,
    cursor,
    0x17c);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_Items,
    struct ItemMenuState,
    items,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_ItemCount,
    struct ItemMenuState,
    item_count,
    0x218);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_OwnerIds,
    struct ItemMenuState,
    owner_ids,
    0x208);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_ItemOwner,
    struct ItemMenuState,
    item_owner,
    0x21a);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_TargetOwner,
    struct ItemMenuState,
    target_owner,
    0x21b);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_SelectedItemIcon,
    struct ItemMenuState,
    selected_item_icon,
    0x21c);
LAYOUT_OFFSET_GUARD(
    ItemMenuState_MessageOffset,
    struct ItemMenuState,
    message_offset,
    0x25a);
LAYOUT_SIZE_GUARD(
    ItemMenuState_Size,
    struct ItemMenuState,
    0x25c);

extern char Value_00000ad7;

#define ItemMenu_EmptyMsg         Value_00000ad7

#endif
