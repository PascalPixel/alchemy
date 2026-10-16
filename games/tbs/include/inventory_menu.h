#ifndef ALCHEMY_INVENTORY_MENU_H
#define ALCHEMY_INVENTORY_MENU_H

#include "unit/menu/menu.h"
#include "global_cells.h"
#include "static_assert.h"
#include "menu_result.h"
#include "render_input.h"
#include "text/msg_ids.h"
#include "farcall.h"

struct AnimationObject;

struct UiWindow;

struct InventoryMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  struct UiWindow *mainWindow;
  struct RenderOutput *paneIcons[2];
  s8 paneIndex[2];
  s8 paneCount[2];
  struct UiWindow *itemWindow;
  struct UiWindow *statusWindow;
  struct UiWindow *helpWindow;
  struct UiWindow *infoWindow;
  struct UiWindow *equipWindow;
  struct UiWindow *listWindow;
  struct UiWindow *unknownWindow38;
  struct UiWindow *modalWindow;
  struct UiWindow *unknownWindow40;
  struct RenderOutput *gridCursor;
  struct RenderOutput *entryIcons[32];
  struct RenderOutput *categoryIcons[8];
  u8 unk0e8[0x24];
  struct UiWindow *messageWindow;
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  struct AnimationObject *ownerObjects[8];
  s16 ownerX[8];
  s16 ownerY[8];
  s32 ownerScale[8];
  u16 selectedSlots[2];
  u16 selectedItems[2];
  struct RenderOutput *cursor;
  u8 unk180[0x48];
  u16 items[32];
  u16 ownerIds[8];
  u8 itemCount;
  u8 partyCount;
  u8 paneOwner[2];
  struct RenderOutput *selectedItemIcon;
  u16 flags;
  s16 completionFlag;
  u8 unk224[0x36];
  s16 messageOffset;
  s8 equipPreview;
  u8 listMode;
  u8 unk25e[2];
  s8 selectedIndexByOwner[8];
  u8 mode;
  u8 unk269[0x129];
  s16 resourceSlots[2];
};

STATIC_ASSERT(offsetof(struct InventoryMenuState, paneIcons) == 0x14);
STATIC_ASSERT(offsetof(struct InventoryMenuState, paneIndex) == 0x1c);
STATIC_ASSERT(offsetof(struct InventoryMenuState, paneCount) == 0x1e);
STATIC_ASSERT(offsetof(struct InventoryMenuState, itemWindow) == 0x20);
STATIC_ASSERT(offsetof(struct InventoryMenuState, infoWindow) == 0x2c);
STATIC_ASSERT(offsetof(struct InventoryMenuState, gridCursor) == 0x44);
STATIC_ASSERT(offsetof(struct InventoryMenuState, entryIcons) == 0x48);
STATIC_ASSERT(offsetof(struct InventoryMenuState, categoryIcons) == 0xc8);
STATIC_ASSERT(offsetof(struct InventoryMenuState, messageWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct InventoryMenuState, selectedColumn) == 0x110);
STATIC_ASSERT(offsetof(struct InventoryMenuState, selectedRow) == 0x111);
STATIC_ASSERT(offsetof(struct InventoryMenuState, columnCount) == 0x112);
STATIC_ASSERT(offsetof(struct InventoryMenuState, rowCount) == 0x113);
STATIC_ASSERT(offsetof(struct InventoryMenuState, ownerObjects) == 0x114);
STATIC_ASSERT(offsetof(struct InventoryMenuState, ownerX) == 0x134);
STATIC_ASSERT(offsetof(struct InventoryMenuState, ownerY) == 0x144);
STATIC_ASSERT(offsetof(struct InventoryMenuState, ownerScale) == 0x154);
STATIC_ASSERT(offsetof(struct InventoryMenuState, selectedSlots) == 0x174);
STATIC_ASSERT(offsetof(struct InventoryMenuState, selectedItems) == 0x178);
STATIC_ASSERT(offsetof(struct InventoryMenuState, cursor) == 0x17c);
STATIC_ASSERT(offsetof(struct InventoryMenuState, items) == 0x1c8);
STATIC_ASSERT(offsetof(struct InventoryMenuState, ownerIds) == 0x208);
STATIC_ASSERT(offsetof(struct InventoryMenuState, itemCount) == 0x218);
STATIC_ASSERT(offsetof(struct InventoryMenuState, paneOwner) == 0x21a);
STATIC_ASSERT(offsetof(struct InventoryMenuState, selectedItemIcon) == 0x21c);
STATIC_ASSERT(offsetof(struct InventoryMenuState, messageOffset) == 0x25a);
STATIC_ASSERT(offsetof(struct InventoryMenuState, equipPreview) == 0x25c);
STATIC_ASSERT(offsetof(struct InventoryMenuState, selectedIndexByOwner) ==
              0x260);
STATIC_ASSERT(offsetof(struct InventoryMenuState, mode) == 0x268);
STATIC_ASSERT(offsetof(struct InventoryMenuState, resourceSlots) == 0x392);
STATIC_ASSERT(sizeof(struct InventoryMenuState) == 0x398);

enum ItemMenuCommand {
  ITEM_COMMAND_USE,
  ITEM_COMMAND_EQUIP,
  ITEM_COMMAND_INSPECT,
  ITEM_COMMAND_GIVE,
  ITEM_COMMAND_REMOVE,
  ITEM_COMMAND_DROP
};

struct BattleUnit;

#endif
