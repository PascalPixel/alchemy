#ifndef ALCHEMY_ITEM_MENU_H
#define ALCHEMY_ITEM_MENU_H

/// @file
/// The item menu's state and commands.

#include "fixed_math.h"
#include "static_assert.h"
#include "gamepak/far_call.h"
#include "menu/menu.h"
#include "menu/paged_list.h"
#include "system/heap_slots.h"
#include "text/msg_ids.h"
#include "window/window_sprite.h"

struct Sprite;

struct Window;

/// The icons the menus keep for their entries: items, Psynergy and Djinn.
#define MENU_ENTRY_ICONS 32

/// Where an entry or category icon waits, just past the screen's bottom right
/// corner, until a screen places it.
#define MENU_ICON_HIDDEN_X (DISPLAY_WIDTH + 8)
#define MENU_ICON_HIDDEN_Y (DISPLAY_HEIGHT + 8)
/// The y a party or Djinni figure is parked at, below the screen, while a
/// screen has no place for it.
#define MENU_FIGURE_HIDDEN_Y 200

/// The item menu's state, in the HEAP_SLOT_MENU heap slot: its windows, the
/// party figures, the items listed and the two choices a command makes.
struct ItemMenuState {
  u8 unused[8]; ///< no view of the menu state names it (see menu/README.md)
  s32 selectedMember;         ///< the party member chosen
  struct Window *moneyWindow; ///< ShopState's; not named here
  struct Window *mainWindow;
  /// The two sides: the party member whose items are shown, and the one an item
  /// is given to.
  ///@{
  struct WindowSprite *sideIcons[2];
  s8 sideIndex[2]; ///< the party member chosen, in memberIds; -1 for none at
                   ///< first
  s8 sideCount[2]; ///< party members to choose from; 1 at first
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
  struct WindowSprite *gridCursor;     ///< only prepared by name
  struct WindowSprite *entryIcons[MENU_ENTRY_ICONS];
  struct WindowSprite *statusIcons[8];
  u8 unusedAfterIcons[36]; ///< no view of the menu state names it
  struct Window *questionWindow;
  /// The cursor grid: its place and its size, 1 by 1 at first.
  ///@{
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  ///@}
  struct Sprite *memberSprites[8]; ///< the party members' figures
  s16 memberX[8];                  ///< screen pixels
  s16 memberY[8];                  ///< screen pixels
  FIXED memberScale[8];            ///< 16.16 fixed point, FIXED_ONE full size
  u16 selectedSlots[2];            ///< each side's inventory slot chosen
  u16 selectedItems[2];            ///< each side's inventory entry chosen
  struct WindowSprite *cursor;
  /// DjinnMenuState's backdrop pointer at 388; no view names the rest.
  u8 aroundBackdrop[72];
  u16 items[32];    ///< the inventory entries listed
  u16 memberIds[8]; ///< the party in order (Party_ListMembers)
  u8 itemCount;
  u8 partyCount;
  u8 sideMember[2]; ///< each side's party member
  struct WindowSprite *selectedItemIcon;
  u16 flags;          ///< 1 while the item's name and command grid are drawn
  s16 completionFlag; ///< set to 1 as item commands end; not read by name
  /// The row of four figures the Djinn and status screens show
  /// (menu_djinn_figures.c): each one's sprite and place in screen pixels, and
  /// its scale in 16.16, which closes a third of its gap to FIXED_ONE each
  /// frame, or holds -scale while negative.
  ///@{
  struct Sprite *figures[4];
  s16 figureX[4];
  s16 figureY[4];
  s32 figureScales[4];
  ///@}
  u8 unusedAfterFigures[6]; ///< DjinnMenuState's pair; not named here
  s16 messageOffset;        ///< the item use's result, after MsgItemUseResult
  s8 equipPreview; ///< 1 while the stats preview a removal, not an equip
  u8 listMode;     ///< the command chosen last (ItemMenuCommand); -1 for none
  u8 unusedAfterListMode[2];   ///< no view of the menu state names it
  s8 selectedIndexByMember[8]; ///< each party member's entry chosen last
  u8 mode;           ///< not named through this struct; PsynergyMenuState.mode
  u8 shopStock[297]; ///< ShopState's stock, party and cursor; not named
                     ///< here
  u16 resourceSlots[2]; ///< VRAM cache slots of two arrow pictures
};

STATIC_ASSERT(offsetof(struct ItemMenuState, sideIcons) == 20);
STATIC_ASSERT(offsetof(struct ItemMenuState, sideIndex) == 28);
STATIC_ASSERT(offsetof(struct ItemMenuState, sideCount) == 30);
STATIC_ASSERT(offsetof(struct ItemMenuState, itemWindow) == 32);
STATIC_ASSERT(offsetof(struct ItemMenuState, descriptionWindow) == 44);
STATIC_ASSERT(offsetof(struct ItemMenuState, gridCursor) == 68);
STATIC_ASSERT(offsetof(struct ItemMenuState, entryIcons) == 72);
STATIC_ASSERT(offsetof(struct ItemMenuState, statusIcons) == 200);
STATIC_ASSERT(offsetof(struct ItemMenuState, questionWindow) == 268);
STATIC_ASSERT(offsetof(struct ItemMenuState, selectedColumn) == 272);
STATIC_ASSERT(offsetof(struct ItemMenuState, selectedRow) == 273);
STATIC_ASSERT(offsetof(struct ItemMenuState, columnCount) == 274);
STATIC_ASSERT(offsetof(struct ItemMenuState, rowCount) == 275);
STATIC_ASSERT(offsetof(struct ItemMenuState, memberSprites) == 276);
STATIC_ASSERT(offsetof(struct ItemMenuState, memberX) == 308);
STATIC_ASSERT(offsetof(struct ItemMenuState, memberY) == 324);
STATIC_ASSERT(offsetof(struct ItemMenuState, memberScale) == 340);
STATIC_ASSERT(offsetof(struct ItemMenuState, selectedSlots) == 372);
STATIC_ASSERT(offsetof(struct ItemMenuState, selectedItems) == 376);
STATIC_ASSERT(offsetof(struct ItemMenuState, cursor) == 380);
STATIC_ASSERT(offsetof(struct ItemMenuState, items) == 456);
STATIC_ASSERT(offsetof(struct ItemMenuState, memberIds) == 520);
STATIC_ASSERT(offsetof(struct ItemMenuState, itemCount) == 536);
STATIC_ASSERT(offsetof(struct ItemMenuState, sideMember) == 538);
STATIC_ASSERT(offsetof(struct ItemMenuState, selectedItemIcon) == 540);
STATIC_ASSERT(offsetof(struct ItemMenuState, figures) == 548);
STATIC_ASSERT(offsetof(struct ItemMenuState, messageOffset) == 602);
STATIC_ASSERT(offsetof(struct ItemMenuState, equipPreview) == 604);
STATIC_ASSERT(offsetof(struct ItemMenuState, selectedIndexByMember) == 608);
STATIC_ASSERT(offsetof(struct ItemMenuState, mode) == 616);
STATIC_ASSERT(offsetof(struct ItemMenuState, resourceSlots) == 914);
STATIC_ASSERT(sizeof(struct ItemMenuState) == 920);

/// The item command grid, by row * 3 + column: Use, Equip and Details on top,
/// Give, Remove and Drop below (ItemMenu_ChooseCommand).
enum ItemMenuCommand {
  ITEM_COMMAND_USE,
  ITEM_COMMAND_EQUIP,
  ITEM_COMMAND_DETAILS, ///< Details
  ITEM_COMMAND_GIVE,
  ITEM_COMMAND_REMOVE,
  ITEM_COMMAND_DROP
};

#endif
