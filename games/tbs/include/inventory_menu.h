#ifndef ALCHEMY_INVENTORY_MENU_H
#define ALCHEMY_INVENTORY_MENU_H

#include "unit/menu/menu.h"
#include "global_cells.h"
#include "layout_guard.h"
#include "menu_result.h"
#include "render_input.h"
#include "text/msg_ids.h"
#include "farcall.h"

struct AnimationObject;

struct UiWindow;

struct InventoryMenuState
{
    u8                      unk000[8];
    s32                     selectedOwner;
    u8                      unk00c[4];
    struct UiWindow        *mainWindow;
    struct RenderOutput    *paneIcons[2];
    s8                      paneIndex[2];
    s8                      paneCount[2];
    struct UiWindow        *itemWindow;
    struct UiWindow        *statusWindow;
    struct UiWindow        *helpWindow;
    struct UiWindow        *infoWindow;
    struct UiWindow        *equipWindow;
    struct UiWindow        *listWindow;
    struct UiWindow        *unknownWindow38;
    struct UiWindow        *modalWindow;
    struct UiWindow        *unknownWindow40;
    struct RenderOutput    *gridCursor;
    struct RenderOutput    *entryIcons[32];
    struct RenderOutput    *categoryIcons[8];
    u8                      unk0e8[0x24];
    struct UiWindow        *messageWindow;
    u8                      selectedColumn;
    u8                      selectedRow;
    u8                      columnCount;
    u8                      rowCount;
    struct AnimationObject *ownerObjects[8];
    s16                     ownerX[8];
    s16                     ownerY[8];
    s32                     ownerScale[8];
    u16                     selectedSlots[2];
    u16                     selectedItems[2];
    struct RenderOutput    *cursor;
    u8                      unk180[0x48];
    u16                     items[32];
    u16                     ownerIds[8];
    u8                      itemCount;
    u8                      partyCount;
    u8                      paneOwner[2];
    struct RenderOutput    *selectedItemIcon;
    u16                     flags;
    s16                     completionFlag;
    u8                      unk224[0x36];
    s16                     messageOffset;
    s8                      equipPreview;
    u8                      listMode;
    u8                      unk25e[2];
    s8                      selectedIndexByOwner[8];
    u8                      mode;
    u8                      unk269[0x129];
    s16                     resourceSlots[2];
};

LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneIcons, struct InventoryMenuState, paneIcons, 0x14);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneIndex, struct InventoryMenuState, paneIndex, 0x1c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneCount, struct InventoryMenuState, paneCount, 0x1e);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ItemWindow, struct InventoryMenuState, itemWindow, 0x20);
LAYOUT_OFFSET_GUARD(InventoryMenuState_InfoWindow, struct InventoryMenuState, infoWindow, 0x2c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_GridCursor, struct InventoryMenuState, gridCursor, 0x44);
LAYOUT_OFFSET_GUARD(InventoryMenuState_EntryIcons, struct InventoryMenuState, entryIcons, 0x48);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_CategoryIcons, struct InventoryMenuState, categoryIcons, 0xc8);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_MessageWindow, struct InventoryMenuState, messageWindow, 0x10c);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedColumn, struct InventoryMenuState, selectedColumn, 0x110);
LAYOUT_OFFSET_GUARD(InventoryMenuState_SelectedRow, struct InventoryMenuState, selectedRow, 0x111);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ColumnCount, struct InventoryMenuState, columnCount, 0x112);
LAYOUT_OFFSET_GUARD(InventoryMenuState_RowCount, struct InventoryMenuState, rowCount, 0x113);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_OwnerObjects, struct InventoryMenuState, ownerObjects, 0x114);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerX, struct InventoryMenuState, ownerX, 0x134);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerY, struct InventoryMenuState, ownerY, 0x144);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerScale, struct InventoryMenuState, ownerScale, 0x154);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedSlots, struct InventoryMenuState, selectedSlots, 0x174);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedItems, struct InventoryMenuState, selectedItems, 0x178);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Cursor, struct InventoryMenuState, cursor, 0x17c);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Items, struct InventoryMenuState, items, 0x1c8);
LAYOUT_OFFSET_GUARD(InventoryMenuState_OwnerIds, struct InventoryMenuState, ownerIds, 0x208);
LAYOUT_OFFSET_GUARD(InventoryMenuState_ItemCount, struct InventoryMenuState, itemCount, 0x218);
LAYOUT_OFFSET_GUARD(InventoryMenuState_PaneOwner, struct InventoryMenuState, paneOwner, 0x21a);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedItemIcon, struct InventoryMenuState, selectedItemIcon, 0x21c);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_MessageOffset, struct InventoryMenuState, messageOffset, 0x25a);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_EquipPreview, struct InventoryMenuState, equipPreview, 0x25c);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_SelectedIndex, struct InventoryMenuState, selectedIndexByOwner, 0x260);
LAYOUT_OFFSET_GUARD(InventoryMenuState_Mode, struct InventoryMenuState, mode, 0x268);
LAYOUT_OFFSET_GUARD(
    InventoryMenuState_ResourceSlots, struct InventoryMenuState, resourceSlots, 0x392);
LAYOUT_SIZE_GUARD(InventoryMenuState_Size, struct InventoryMenuState, 0x398);

enum ItemMenuCommand
{
    ITEM_COMMAND_USE,
    ITEM_COMMAND_EQUIP,
    ITEM_COMMAND_INSPECT,
    ITEM_COMMAND_GIVE,
    ITEM_COMMAND_REMOVE,
    ITEM_COMMAND_DROP
};

struct BattleUnit;

#endif
