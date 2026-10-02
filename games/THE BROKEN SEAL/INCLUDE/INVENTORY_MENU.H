#ifndef ALCHEMY_INVENTORY_MENU_H
#define ALCHEMY_INVENTORY_MENU_H

#include "LAYOUT_GUARD.H"
#include "MENU_RESULT.H"

struct InventoryMenuIcon {
    struct InventoryMenuIcon *next;
    u8 unknown_04;
    u8 state;
    s16 x;
    s16 y;
    u8 unknown_0a[2];
    u16 unknown_0c;
    u8 render_target;
    u8 sentinel;
};

struct UiWindow;

struct InventoryMenuState {
    u8 unknown_000[8];
    s32 selected_owner;
    u8 unknown_00c[4];
    struct UiWindow *main_window;
    struct InventoryMenuIcon *pane_icons[2];
    s8 pane_index[2];
    s8 pane_count[2];
    struct UiWindow *item_window;
    struct UiWindow *status_window;
    struct UiWindow *help_window;
    struct UiWindow *info_window;
    struct UiWindow *equip_window;
    struct UiWindow *list_window;
    struct UiWindow *unknown_window_38;
    struct UiWindow *modal_window;
    struct UiWindow *unknown_window_40;
    struct InventoryMenuIcon *grid_cursor;
    struct InventoryMenuIcon *entry_icons[32];
    struct InventoryMenuIcon *category_icons[8];
    u8 unknown_0e8[0x24];
    struct UiWindow *message_window;
    u8 unknown_110[4];
    void *owner_objects[8];
    s16 owner_x[8];
    s16 owner_y[8];
    s32 owner_scale[8];
    u16 selected_slots[2];
    u16 selected_items[2];
    struct InventoryMenuIcon *cursor;
    u8 unknown_180[0x48];
    u16 items[32];
    u16 owner_ids[8];
    u8 item_count;
    u8 party_count;
    u8 pane_owner[2];
    struct InventoryMenuIcon *selected_item_icon;
    u16 flags;
    s16 completion_flag;
    u8 unknown_224[0x36];
    s16 message_offset;
    u8 equip_preview;
    u8 list_mode;
    u8 unknown_25e[2];
    s8 selected_index_by_owner[8];
    u8 mode;
};

LAYOUT_OFFSET_GUARD(InventoryMenuIcon_State, struct InventoryMenuIcon, state, 5);
LAYOUT_OFFSET_GUARD(InventoryMenuIcon_X, struct InventoryMenuIcon, x, 6);
LAYOUT_OFFSET_GUARD(InventoryMenuIcon_Y, struct InventoryMenuIcon, y, 8);
LAYOUT_OFFSET_GUARD(InventoryMenuIcon_RenderTarget, struct InventoryMenuIcon, render_target, 0x0e);
LAYOUT_OFFSET_GUARD(InventoryMenuIcon_Sentinel, struct InventoryMenuIcon, sentinel, 0x0f);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneIcons, struct InventoryMenuState, pane_icons, 0x14);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneIndex, struct InventoryMenuState, pane_index, 0x1c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneCount, struct InventoryMenuState, pane_count, 0x1e);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ItemWindow, struct InventoryMenuState, item_window, 0x20);
LAYOUT_OFFSET_GUARD(InventoryMenuState_InfoWindow, struct InventoryMenuState, info_window, 0x2c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_GridCursor, struct InventoryMenuState, grid_cursor, 0x44);
LAYOUT_OFFSET_GUARD(InventoryMenuState_EntryIcons, struct InventoryMenuState, entry_icons, 0x48);
LAYOUT_OFFSET_GUARD(InventoryMenuState_CategoryIcons, struct InventoryMenuState, category_icons, 0xc8);
LAYOUT_OFFSET_GUARD(InventoryMenuState_MessageWindow, struct InventoryMenuState, message_window, 0x10c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Unknown110, struct InventoryMenuState, unknown_110, 0x110);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerObjects, struct InventoryMenuState, owner_objects, 0x114);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerX, struct InventoryMenuState, owner_x, 0x134);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerY, struct InventoryMenuState, owner_y, 0x144);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerScale, struct InventoryMenuState, owner_scale, 0x154);
LAYOUT_OFFSET_GUARD(InventoryMenuState_SelectedSlots, struct InventoryMenuState, selected_slots, 0x174);
LAYOUT_OFFSET_GUARD(InventoryMenuState_SelectedItems, struct InventoryMenuState, selected_items, 0x178);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Cursor, struct InventoryMenuState, cursor, 0x17c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Items, struct InventoryMenuState, items, 0x1c8);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerIds, struct InventoryMenuState, owner_ids, 0x208);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ItemCount, struct InventoryMenuState, item_count, 0x218);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneOwner, struct InventoryMenuState, pane_owner, 0x21a);
LAYOUT_OFFSET_GUARD(InventoryMenuState_SelectedItemIcon, struct InventoryMenuState, selected_item_icon, 0x21c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_MessageOffset, struct InventoryMenuState, message_offset, 0x25a);
LAYOUT_OFFSET_GUARD(InventoryMenuState_EquipPreview, struct InventoryMenuState, equip_preview, 0x25c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_SelectedIndex, struct InventoryMenuState, selected_index_by_owner, 0x260);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Mode, struct InventoryMenuState, mode, 0x268);
LAYOUT_SIZE_GUARD(InventoryMenuState_Size, struct InventoryMenuState, 0x26c);

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
void ItemMenu_RefreshEntry(s32 layout);
void Menu_SpawnIconEntries(struct InventoryMenuState *menu, s32 window);
s32 ItemMenu_RunOwnerSelection(u16 *owners, u16 *items);
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
