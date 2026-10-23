#ifndef ALCHEMY_SUMMARY_SCREEN_H
#define ALCHEMY_SUMMARY_SCREEN_H

/// @file
/// The summary screen's state, and the statuses it shows.

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "types.h"
#include "heap_slots.h"
#include "static_assert.h"
#include "animspr.h"
#include "window.h"

struct Unit;
struct PsynergyEntry;

/// The work of the member chooser and the Status menu, in the HEAP_SLOT_MENU
/// heap slot, which each takes as it opens and frees before it returns.
struct SummaryScreenWork {
  u8 unused[8];      ///< no view of the menu work names it (see menu/README.md)
  s32 selectedOwner; ///< the party member chosen
  struct Window *moneyWindow; ///< ShopWork's; not named here
  struct Window *ownerWindow;
  struct WindowSprite *ownerCursors[2];
  s8 ownerIndex[2]; ///< each pane's member, in ownerIds
  s8 ownerCount[2]; ///< members each pane chooses from
  struct Window *actionWindow;
  struct Window *statusWindow;
  struct Window *unopenedWindow; ///< never given a window; only cleared
  struct Window *helpWindow;
  /// ItemMenuWork's equipWindow to gridCursor; not named here.
  u8 itemWindows[0x18];
  struct WindowSprite *entryIcons[32];
  struct WindowSprite *categoryIcons[8];
  u8 unusedAfterIcons[0x24]; ///< no view of the menu work names it
  struct Window *selectorWindow;
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  struct Sprite *ownerObjects[8];
  s16 ownerX[8];     ///< screen pixels
  s16 ownerY[8];     ///< screen pixels
  s32 ownerScale[8]; ///< 16.16 fixed point, 0x10000 full size
  u16 selectedSlots[2];
  u16 selectedActions[2];
  struct WindowSprite *statusCursor;
  /// DjinnMenuWork's backdrop pointer at 0x184; no view names the rest.
  u8 aroundBackdrop[0x48];
  u16 entries[32];
  u16 ownerIds[8]; ///< the party in order (Party_ListMembers)
  u8 entryCount;
  u8 partyCount;
  u8 selectedIds[2]; ///< each pane's member
  struct WindowSprite *selectedIcon;
  u16 page;           ///< 3 for the member chooser, 0 for the Status menu
  s16 completionFlag; ///< ItemMenuWork's; not named here
  struct Sprite *rowObjects[4];
  s16 rowX[4]; ///< screen pixels
  s16 rowY[4]; ///< screen pixels
};

STATIC_ASSERT(offsetof(struct SummaryScreenWork, ownerWindow) == 0x10);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, ownerCursors) == 0x14);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, statusWindow) == 0x24);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, entryIcons) == 0x48);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, selectorWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, selectedColumn) == 0x110);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, ownerObjects) == 0x114);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, ownerY) == 0x144);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, selectedSlots) == 0x174);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, statusCursor) == 0x17c);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, ownerIds) == 0x208);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, selectedIds) == 0x21a);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, page) == 0x220);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, rowObjects) == 0x224);
STATIC_ASSERT(offsetof(struct SummaryScreenWork, rowX) == 0x234);
STATIC_ASSERT(sizeof(struct SummaryScreenWork) == 0x244);

/// The statuses the summary screens show, as SummaryScreen_BuildAvailability
/// fills them: one byte each, nonzero while the member has it.
enum UnitStatus {
  CHARACTER_DOWN,   ///< HP 0; listed only when the caller asks for it
  CHARACTER_POISON, ///< Unit.poison 1
  CHARACTER_VENOM,  ///< Unit.poison any other nonzero value
  CHARACTER_CURSE,  ///< Unit.curse nonzero
  CHARACTER_HAUNT,  ///< Unit.haunt nonzero
  CHARACTER_AILMENT_COUNT
};

#endif
