#ifndef ALCHEMY_INVENTORY_MENU_H
#define ALCHEMY_INVENTORY_MENU_H

/// @file
/// The item menu's state and commands.

#include "unit/menu/menu.h"
#include "global_cells.h"
#include "static_assert.h"
#include "menu_result.h"
#include "render_input.h"
#include "text/msg_ids.h"
#include "farcall.h"

struct AnimationObject;

struct UiWindow;

/// The item menu's work, in the HEAP_SLOT_MENU work slot: its windows, the
/// party figures, the items listed and the two choices a command makes. The
/// menu takes the slot as it opens and frees it as it closes.
struct InventoryMenuState {
  u8 unk000[8];      ///< not named by TBS code
  s32 selectedOwner; ///< the party member chosen
  u8 unk00c[4];      ///< not named by TBS code
  struct UiWindow *mainWindow;
  /// The two panes: the member whose items are shown, and the one an item is
  /// given to.
  ///@{
  struct RenderOutput *paneIcons[2];
  s8 paneIndex[2]; ///< the member chosen, in ownerIds; -1 for none at first
  s8 paneCount[2]; ///< members to choose from; 1 at first
  ///@}
  struct UiWindow *itemWindow;
  struct UiWindow *statusWindow;
  struct UiWindow *helpWindow;
  struct UiWindow *infoWindow;
  struct UiWindow *equipWindow;
  struct UiWindow *listWindow;
  struct UiWindow *unknownWindow38; ///< only closed by name in TBS
  struct UiWindow *modalWindow;
  struct UiWindow *unknownWindow40; ///< only closed by name in TBS
  struct RenderOutput *gridCursor;  ///< only prepared by name in TBS
  struct RenderOutput *entryIcons[32];
  struct RenderOutput *categoryIcons[8];
  u8 unk0e8[0x24]; ///< not named by TBS code
  struct UiWindow *messageWindow;
  /// The cursor grid: its place and its size, 1 by 1 at first.
  ///@{
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  ///@}
  struct AnimationObject *ownerObjects[8]; ///< the party members' figures
  s16 ownerX[8];                           ///< screen pixels
  s16 ownerY[8];                           ///< screen pixels
  s32 ownerScale[8];    ///< 16.16 fixed point, 0x10000 full size
  u16 selectedSlots[2]; ///< each pane's inventory slot chosen
  u16 selectedItems[2]; ///< each pane's inventory entry chosen
  struct RenderOutput *cursor;
  u8 unk180[0x48]; ///< not named by TBS code
  u16 items[32];   ///< the inventory entries listed
  u16 ownerIds[8]; ///< the party in order (Party_ListActiveOwners)
  u8 itemCount;
  u8 partyCount;
  u8 paneOwner[2]; ///< each pane's member
  struct RenderOutput *selectedItemIcon;
  u16 flags;          ///< 1 while the item's name and command grid are drawn
  s16 completionFlag; ///< set to 1 as item commands end; not read by name
  u8 unk224[0x36];    ///< not named by TBS code
  s16 messageOffset;  ///< the item use's result, after MsgItemUseResult
  s8 equipPreview;    ///< 1 while the stats preview a removal, not an equip
  u8 listMode;  ///< the command chosen last (ItemMenuCommand); 0xff for none
  u8 unk25e[2]; ///< not named by TBS code
  s8 selectedIndexByOwner[8]; ///< each member's entry chosen last
  u8 mode;          ///< not named through this struct; PsynergyMenuState.mode
  u8 unk269[0x129]; ///< not named by TBS code
  s16 resourceSlots[2]; ///< VRAM cache slots of two arrow pictures
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

/// The item command grid, by row * 3 + column: Use, Equip and Details on top,
/// Give, Remove and Drop below (ItemMenu_ChooseCommand).
enum ItemMenuCommand {
  ITEM_COMMAND_USE,
  ITEM_COMMAND_EQUIP,
  ITEM_COMMAND_INSPECT, ///< Details
  ITEM_COMMAND_GIVE,
  ITEM_COMMAND_REMOVE,
  ITEM_COMMAND_DROP
};

struct BattleUnit;

#endif
