#ifndef ALCHEMY_INVENTORY_MENU_H
#define ALCHEMY_INVENTORY_MENU_H

#include "layout_guard.h"
#include "menu_result.h"

struct InventoryMenuIcon {
    u8 unknown_00[5];
    u8 state;                       /* 0x05 */
    u16 x;                          /* 0x06 */
    u16 y;                          /* 0x08 */
    u8 unknown_0a[4];
    u8 render_target;               /* 0x0e */
};

struct InventoryMenuState {
    u8 unknown_000[0x20];
    s32 item_window;                /* 0x020 */
    u8 unknown_024[8];
    s32 info_window;                /* 0x02c */
    u8 unknown_030[0x18];
    struct InventoryMenuIcon *entry_icons[32]; /* 0x048 */
    u8 unknown_0c8[0x44];
    s32 message_window;             /* 0x10c */
    u8 unknown_110[0x64];
    u16 selected_slot;              /* 0x174 */
    u8 unknown_176[2];
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
    struct InventoryMenuIcon * volatile selected_item_icon; /* 0x21c */
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

extern struct InventoryMenuState *Data_03001f2c;
extern char Value_00000ad7;

s32 Func_080a3ce4(s32 item);
void Func_080a38a8(s32 owner);
void Func_080a3cf8(s32 unused, s32 message);
void Func_080a3d24(const u16 *items);
s32 Func_080a3d6c(s32 owner);
s32 Func_080a3d9c(s32 owner, s32 item);
u32 Func_080a3ddc(void *owner, u16 *items, s32 mode);
void Func_080a3e28(u16 *items, s32 style);
void Func_080a3e88(s32 owner, s32 mode);
void Func_080a3eec(void);
s32 Func_080a32b8(void);
void Func_080a345c(void);
void Func_080a3480(void);
void Func_080a34c0(void);
s32 Func_080a355c(s32 party_slot);
s32 Func_080a4110(s32 column, s32 row);
s32 Func_080a413c(s32 column, s32 row);
void Func_080a45cc(void *command_states, s32 window);
void Func_080a4754(void);
s32 Func_080a47b4(s32 item_index);
void Func_080a4db4(s32 delta, s32 unused, s32 window, s32 x, s32 y);
void Func_080a4e20(void);
void Func_080a4e44(void);
void Func_080a4e68(void);
void Func_080a4e90(void);
void Func_080a4eb8(void);
void Func_080a4ee0(void);
void Func_080a51d0(void);
s32 Func_080a9e48(s32 slot, s32 owner, s32 target);
s32 Func_080a5578(struct MenuResult *result, s32 index);
s32 Func_080a56c8(s32 window, s32 unused, const struct MenuResult *state);
s32 Func_080a5780(void);
void Func_080a9a5c(s32 window, s32 owner, s32 mode);
void Func_08015298(s32 style, u16 item, u8 target, s32 flags);

#define InventoryMenu_IsSpecialItemRange   Func_080a3ce4
#define InventoryMenu_OpenCategoryItems    Func_080a38a8
#define InventoryMenu_DrawMessage          Func_080a3cf8
#define Menu_HideEmptyEntryIcons           Func_080a3d24
#define InventoryMenu_CountItems           Func_080a3d6c
#define InventoryMenu_GetItemQuantity      Func_080a3d9c
#define InventoryMenu_CollectItems         Func_080a3ddc
#define InventoryMenu_DrawItemIcons        Func_080a3e28
#define InventoryMenu_RefreshOwnerItems    Func_080a3e88
#define InventoryMenu_NoOp                 Func_080a3eec
#define InventoryMenu_UseSelectedItem      Func_080a32b8
#define InventoryMenu_HideAllItemIcons     Func_080a345c
#define InventoryMenu_HidePageStartIcons   Func_080a3480
#define InventoryMenu_CloseWindows         Func_080a34c0
#define InventoryMenu_PrepareOwnerItems    Func_080a355c
#define InventoryMenu_GetCommandCursorX    Func_080a4110
#define InventoryMenu_GetCommandCursorY    Func_080a413c
#define InventoryMenu_DrawCommandLabels    Func_080a45cc
#define InventoryMenu_MaybeBreakSelectedItem Func_080a4754
#define InventoryMenu_OpenItemDetails      Func_080a47b4
#define InventoryMenu_DrawStatDelta        Func_080a4db4
#define InventoryMenu_SetItemWindowFiveRowBounds Func_080a4e20
#define InventoryMenu_SetItemWindowThreeRowBounds Func_080a4e44
#define InventoryMenu_SetMessageWindowRow5Bounds Func_080a4e68
#define InventoryMenu_SetMessageWindowRow6Bounds Func_080a4e90
#define InventoryMenu_SetMessageWindowRow7Bounds Func_080a4eb8
#define InventoryMenu_SetMessageWindowRow3Bounds Func_080a4ee0
#define InventoryMenu_DrawSelectedItemHeader Func_080a51d0
#define Item_Use                            Func_080a9e48
#define InventoryMenu_BuildPageResult      Func_080a5578
#define InventoryMenu_DrawItemNamePage     Func_080a56c8
#define InventoryMenu_ItemNamePageReturnTrue Func_080a5780
#define InventoryMenu_DrawCategoryItems    Func_080a9a5c
#define InventoryMenu_DrawItemIcon         Func_08015298
#define InventoryMenu_EmptyMessage         Value_00000ad7

#endif
