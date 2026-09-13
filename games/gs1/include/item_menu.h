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

extern struct ItemMenuState *Data_03001f2c;
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
void Func_080a3354(void);
s32 Func_080a355c(s32 party_slot);
s32 Func_080a4110(s32 column, s32 row);
s32 Func_080a413c(s32 column, s32 row);
void Func_080a448c(s8 *command_states);
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

#define ItemMenu_IsSpecial   Func_080a3ce4
#define ItemMenu_OpenCategory    Func_080a38a8
#define ItemMenu_DrawMsg          Func_080a3cf8
#define Menu_HideEmptyEntryIcons           Func_080a3d24
#define ItemMenu_Count           Func_080a3d6c
#define ItemMenu_GetQty      Func_080a3d9c
#define ItemMenu_Collect         Func_080a3ddc
#define ItemMenu_DrawIcons        Func_080a3e28
#define ItemMenu_RefreshOwner    Func_080a3e88
#define ItemMenu_NoOp                 Func_080a3eec
#define ItemMenu_Use      Func_080a32b8
#define ItemMenu_HideAllIcons     Func_080a345c
#define ItemMenu_HidePageIcons   Func_080a3480
#define ItemMenu_Close         Func_080a34c0
#define ItemMenu_Init  Func_080a3354
#define ItemMenu_PrepOwner    Func_080a355c
#define ItemMenu_CmdCursorX    Func_080a4110
#define ItemMenu_CmdCursorY    Func_080a413c
#define ItemMenu_BuildCmd   Func_080a448c
#define ItemMenu_DrawCmd    Func_080a45cc
#define ItemMenu_TryBreak Func_080a4754
#define ItemMenu_OpenDetail      Func_080a47b4
#define ItemMenu_DrawStat        Func_080a4db4
#define ItemMenu_SetItemWin5 Func_080a4e20
#define ItemMenu_SetItemWin3 Func_080a4e44
#define ItemMenu_SetMsgWin5 Func_080a4e68
#define ItemMenu_SetMsgWin6 Func_080a4e90
#define ItemMenu_SetMsgWin7 Func_080a4eb8
#define ItemMenu_SetMsgWin3 Func_080a4ee0
#define ItemMenu_DrawItemHead Func_080a51d0
#define Item_Use                            Func_080a9e48
#define ItemMenu_PageResult      Func_080a5578
#define ItemMenu_DrawNamePage     Func_080a56c8
#define ItemMenu_NamePageOk Func_080a5780
#define ItemMenu_DrawCategory    Func_080a9a5c
#define ItemMenu_DrawIcon         Func_08015298
#define ItemMenu_EmptyMsg         Value_00000ad7

#endif
