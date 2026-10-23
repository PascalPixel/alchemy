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

struct Sprite;

struct Window;

/// The item menu's work, in the HEAP_SLOT_MENU heap slot: its windows, the
/// party figures, the items listed and the two choices a command makes. The
/// menu takes the slot as it opens and frees it as it closes.
struct ItemMenuWork {
  u8 unused[8];      ///< no view of the menu work names it (see menu/README.md)
  s32 selectedOwner; ///< the party member chosen
  struct Window *moneyWindow; ///< ShopWork's; not named here
  struct Window *mainWindow;
  /// The two panes: the member whose items are shown, and the one an item is
  /// given to.
  ///@{
  struct WindowSprite *sideIcons[2];
  s8 sideIndex[2]; ///< the member chosen, in ownerIds; -1 for none at first
  s8 sideCount[2]; ///< members to choose from; 1 at first
  ///@}
  struct Window *itemWindow;
  struct Window *statusWindow;
  struct Window *buttonHelpWindow;
  struct Window *descriptionWindow;
  struct Window *equipWindow;
  struct Window *listWindow;
  struct Window *unopenedWindow; ///< never given a window; only closed
  struct Window *modalWindow;
  struct Window *secondUnopenedWindow; ///< never given a window; only closed
  struct WindowSprite *gridCursor;     ///< only prepared by name in TBS
  struct WindowSprite *entryIcons[32];
  struct WindowSprite *categoryIcons[8];
  u8 unusedAfterIcons[0x24]; ///< no view of the menu work names it
  struct Window *questionWindow;
  /// The cursor grid: its place and its size, 1 by 1 at first.
  ///@{
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  ///@}
  struct Sprite *ownerObjects[8]; ///< the party members' figures
  s16 ownerX[8];                  ///< screen pixels
  s16 ownerY[8];                  ///< screen pixels
  s32 ownerScale[8];              ///< 16.16 fixed point, 0x10000 full size
  u16 selectedSlots[2];           ///< each pane's inventory slot chosen
  u16 selectedItems[2];           ///< each pane's inventory entry chosen
  struct WindowSprite *cursor;
  /// DjinnMenuWork's backdrop pointer at 0x184; no view names the rest.
  u8 aroundBackdrop[0x48];
  u16 items[32];   ///< the inventory entries listed
  u16 ownerIds[8]; ///< the party in order (Party_ListMembers)
  u8 itemCount;
  u8 partyCount;
  u8 sideMember[2]; ///< each pane's member
  struct WindowSprite *selectedItemIcon;
  u16 flags;          ///< 1 while the item's name and command grid are drawn
  s16 completionFlag; ///< set to 1 as item commands end; not read by name
  /// DjinnFiguresWork's row of four figures and DjinnMenuWork's pair;
  /// not named here.
  u8 figureRow[0x36];
  s16 messageOffset; ///< the item use's result, after MsgItemUseResult
  s8 equipPreview;   ///< 1 while the stats preview a removal, not an equip
  u8 listMode; ///< the command chosen last (ItemMenuCommand); 0xff for none
  u8 unusedAfterListMode[2];  ///< no view of the menu work names it
  s8 selectedIndexByOwner[8]; ///< each member's entry chosen last
  u8 mode;             ///< not named through this struct; PsynergyMenuWork.mode
  u8 shopStock[0x129]; ///< ShopWork's stock, party and cursor; not named
                       ///< here
  s16 resourceSlots[2]; ///< VRAM cache slots of two arrow pictures
};

STATIC_ASSERT(offsetof(struct ItemMenuWork, sideIcons) == 0x14);
STATIC_ASSERT(offsetof(struct ItemMenuWork, sideIndex) == 0x1c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, sideCount) == 0x1e);
STATIC_ASSERT(offsetof(struct ItemMenuWork, itemWindow) == 0x20);
STATIC_ASSERT(offsetof(struct ItemMenuWork, descriptionWindow) == 0x2c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, gridCursor) == 0x44);
STATIC_ASSERT(offsetof(struct ItemMenuWork, entryIcons) == 0x48);
STATIC_ASSERT(offsetof(struct ItemMenuWork, categoryIcons) == 0xc8);
STATIC_ASSERT(offsetof(struct ItemMenuWork, questionWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedColumn) == 0x110);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedRow) == 0x111);
STATIC_ASSERT(offsetof(struct ItemMenuWork, columnCount) == 0x112);
STATIC_ASSERT(offsetof(struct ItemMenuWork, rowCount) == 0x113);
STATIC_ASSERT(offsetof(struct ItemMenuWork, ownerObjects) == 0x114);
STATIC_ASSERT(offsetof(struct ItemMenuWork, ownerX) == 0x134);
STATIC_ASSERT(offsetof(struct ItemMenuWork, ownerY) == 0x144);
STATIC_ASSERT(offsetof(struct ItemMenuWork, ownerScale) == 0x154);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedSlots) == 0x174);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedItems) == 0x178);
STATIC_ASSERT(offsetof(struct ItemMenuWork, cursor) == 0x17c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, items) == 0x1c8);
STATIC_ASSERT(offsetof(struct ItemMenuWork, ownerIds) == 0x208);
STATIC_ASSERT(offsetof(struct ItemMenuWork, itemCount) == 0x218);
STATIC_ASSERT(offsetof(struct ItemMenuWork, sideMember) == 0x21a);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedItemIcon) == 0x21c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, messageOffset) == 0x25a);
STATIC_ASSERT(offsetof(struct ItemMenuWork, equipPreview) == 0x25c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedIndexByOwner) == 0x260);
STATIC_ASSERT(offsetof(struct ItemMenuWork, mode) == 0x268);
STATIC_ASSERT(offsetof(struct ItemMenuWork, resourceSlots) == 0x392);
STATIC_ASSERT(sizeof(struct ItemMenuWork) == 0x398);

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

struct Unit;

#endif
