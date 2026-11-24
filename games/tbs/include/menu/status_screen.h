#ifndef ALCHEMY_STATUS_SCREEN_H
#define ALCHEMY_STATUS_SCREEN_H

/// @file
/// The Status screen's state, and the statuses it shows.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/sprite.h"
#include "menu/menu.h"
#include "system/heap_slots.h"
#include "window/screens.h"
#include "window/window.h"

struct Unit;

/// The state of the party member chooser and the Status menu, in the
/// HEAP_SLOT_MENU heap slot, which each takes as it opens and frees before it
/// returns.
struct StatusScreenState {
  u8 unused[8]; ///< no view of the menu state names it (see menu/README.md)
  s32 selectedMember;         ///< the party member chosen
  struct Window *moneyWindow; ///< ShopState's; not named here
  struct Window *memberWindow;
  struct WindowSprite *memberCursors[2];
  s8 memberIndex[2]; ///< each side's party member, in memberIds
  s8 memberCount[2]; ///< party members each side chooses from
  struct Window *actionWindow;
  struct Window *statusWindow;
  struct Window *unopenedWindow; ///< never given a window; only cleared
  struct Window *helpWindow;
  /// ItemMenuState's equipWindow to gridCursor; not named here.
  u8 itemWindows[24];
  struct WindowSprite *entryIcons[32];
  struct WindowSprite *statusIcons[8];
  u8 unusedAfterIcons[36]; ///< no view of the menu state names it
  struct Window *selectorWindow;
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  struct Sprite *memberSprites[8];
  s16 memberX[8];       ///< screen pixels
  s16 memberY[8];       ///< screen pixels
  FIXED memberScale[8]; ///< 16.16 fixed point, FIXED_ONE full size
  u16 selectedSlots[2];
  u16 selectedActions[2];
  struct WindowSprite *statusCursor;
  /// DjinnMenuState's backdrop pointer at 388; no view names the rest.
  u8 aroundBackdrop[72];
  u16 entries[32];
  u16 memberIds[8]; ///< the party in order (Party_ListMembers)
  u8 entryCount;
  u8 partyCount;
  u8 selectedIds[2]; ///< each side's party member
  struct WindowSprite *selectedIcon;
  u16 page;           ///< 3 for the party member chooser, 0 for the Status menu
  s16 completionFlag; ///< ItemMenuState's; not named here
  struct Sprite *rowSprites[4];
  s16 rowX[4]; ///< screen pixels
  s16 rowY[4]; ///< screen pixels
};

STATIC_ASSERT(offsetof(struct StatusScreenState, memberWindow) == 16);
STATIC_ASSERT(offsetof(struct StatusScreenState, memberCursors) == 20);
STATIC_ASSERT(offsetof(struct StatusScreenState, statusWindow) == 36);
STATIC_ASSERT(offsetof(struct StatusScreenState, entryIcons) == 72);
STATIC_ASSERT(offsetof(struct StatusScreenState, selectorWindow) == 268);
STATIC_ASSERT(offsetof(struct StatusScreenState, selectedColumn) == 272);
STATIC_ASSERT(offsetof(struct StatusScreenState, memberSprites) == 276);
STATIC_ASSERT(offsetof(struct StatusScreenState, memberY) == 324);
STATIC_ASSERT(offsetof(struct StatusScreenState, selectedSlots) == 372);
STATIC_ASSERT(offsetof(struct StatusScreenState, statusCursor) == 380);
STATIC_ASSERT(offsetof(struct StatusScreenState, memberIds) == 520);
STATIC_ASSERT(offsetof(struct StatusScreenState, selectedIds) == 538);
STATIC_ASSERT(offsetof(struct StatusScreenState, page) == 544);
STATIC_ASSERT(offsetof(struct StatusScreenState, rowSprites) == 548);
STATIC_ASSERT(offsetof(struct StatusScreenState, rowX) == 564);
STATIC_ASSERT(sizeof(struct StatusScreenState) == 580);

/// The statuses the Status screens show, as StatusScreen_ListStatuses
/// fills them: one byte each, nonzero while the party member has it.
enum UnitStatus {
  CHARACTER_DOWN,   ///< HP 0; listed only when the caller asks for it
  CHARACTER_POISON, ///< Unit.poison 1
  CHARACTER_VENOM,  ///< Unit.poison any other nonzero value
  CHARACTER_CURSE,  ///< Unit.curse nonzero
  CHARACTER_HAUNT,  ///< Unit.haunt nonzero
  CHARACTER_AILMENT_COUNT
};

#endif
