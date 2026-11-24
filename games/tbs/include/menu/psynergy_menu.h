#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

/// @file
/// The Psynergy menu's state.

#include "fixed_math.h"
#include "static_assert.h"
#include "graphics/render_input.h"
#include "menu/menu.h"
#include "menu/paged_list.h"
#include "system/heap_slots.h"
#include "text/msg_ids.h"

struct Sprite;

/// The Psynergy menu: its windows, icons and cursors, the party members' tabs
/// and the Psynergy listed for the chosen party member.
struct PsynergyMenuState {
  u8 unused[8]; ///< no view of the menu state names it (see menu/README.md)
  s32 selectedMember;         ///< the party member chosen
  struct Window *moneyWindow; ///< ShopState's; not named here
  struct Window *auxiliaryWindow;
  struct WindowSprite *sideIcon[2]; ///< each side's cursor
  s8 tabIndex[2];  ///< each side's party member, in memberTable
  s8 tabCounts[2]; ///< party members each side chooses from
  struct Window *psynergyWindow;
  struct Window *statusWindow;
  struct Window *shortcutWindow;
  struct Window *infoWindow;
  struct Window *equipWindow; ///< ItemMenuState's; not named here
  struct Window *listWindow;
  /// ItemMenuState's modalWindow and the two windows it never opens;
  /// not named here.
  u8 itemWindows[12];
  struct WindowSprite *entryGridCursor;
  struct WindowSprite *entryIcons[32];
  struct WindowSprite *categoryIcons[8]; ///< ItemMenuState's; not named here
  u8 unusedAfterIcons[36];               ///< no view of the menu state names it
  struct Window *messageWindow;
  /// The cursor grid: its place and its size.
  ///@{
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  ///@}
  struct Sprite *tabSprites[8]; ///< the party members' figures
  s16 tabX[8];                  ///< screen pixels
  s16 rowPositions[8];          ///< screen pixels
  FIXED memberScale[8];         ///< 16.16 fixed point, FIXED_ONE full size
  u16 sideRow[2];               ///< each side's Psynergy row chosen
  u16 sideAction[2]; ///< each side's Psynergy chosen, as a party member action
                     ///< entry
  struct WindowSprite *cursor; ///< ItemMenuState's; not named here
  /// DjinnMenuState's backdrop pointer at 388; no view names the rest.
  u8 aroundBackdrop[72];
  u16 psynergies[32]; ///< the chosen party member's Psynergy listed
  u16 memberTable[8]; ///< the party in order (Party_ListMembers)
  u8 psynergyCount;
  u8 memberCount;
  /// The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy
  /// on the whole party.
  u8 memberIds[2];
  struct WindowSprite *cursorIcon;
  /// bits: 1 the list shows the shortcut help | 0 set after a use or a
  /// cancelled target
  u16 flags;
  u16 skipSlide; ///< 1 skips the cursor's next slide once
  /// ItemMenuFiguresView's figures and their x; not named here.
  u8 figureRow[24];
  s16 slotY[4]; ///< screen pixels
  /// ItemMenuFiguresView's phases and DjinnMenuState's pair; not named here.
  u8 figurePhases[22];
  s16 useResult;   ///< the use's result message, after MsgItemUseResult
  s8 equipPreview; ///< ItemMenuState's; not named here
  u8 listMode;     ///< ItemMenuState's; not named here
  u8 unusedAfterListMode[2];   ///< no view of the menu state names it
  s8 selectedIndexByMember[8]; ///< each party member's entry chosen last
  u8 mode; ///< 0 to use Psynergy; 1 or 2 to set the L or R shortcut
};

STATIC_ASSERT(offsetof(struct PsynergyMenuState, auxiliaryWindow) == 16);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, psynergyWindow) == 32);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, entryGridCursor) == 68);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, entryIcons) == 72);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, messageWindow) == 268);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, selectedColumn) == 272);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, selectedRow) == 273);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, columnCount) == 274);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, rowCount) == 275);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, sideAction[0]) == 376);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, psynergies) == 456);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, psynergyCount) == 536);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, memberIds) == 538);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, selectedIndexByMember) == 608);

STATIC_ASSERT(offsetof(struct PsynergyMenuState, skipSlide) == 546);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, tabSprites) == 276);
STATIC_ASSERT(offsetof(struct PsynergyMenuState, memberScale) == 340);

#endif
