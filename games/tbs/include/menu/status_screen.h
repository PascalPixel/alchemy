#ifndef ALCHEMY_STATUS_SCREEN_H
#define ALCHEMY_STATUS_SCREEN_H

/// @file
/// The Status screen's state, and the statuses it shows.

#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/menu/menu.h"
#include "types.h"
#include "heap_slots.h"
#include "static_assert.h"
#include "sprite.h"
#include "window.h"

struct Unit;
struct PsynergyEntry;

/// The work of the party member chooser and the Status menu, in the
/// HEAP_SLOT_MENU heap slot, which each takes as it opens and frees before it
/// returns.
struct StatusScreenWork {
  u8 unused[8]; ///< no view of the menu work names it (see menu/README.md)
  s32 selectedMember;         ///< the party member chosen
  struct Window *moneyWindow; ///< ShopWork's; not named here
  struct Window *memberWindow;
  struct WindowSprite *memberCursors[2];
  s8 memberIndex[2]; ///< each side's party member, in memberIds
  s8 memberCount[2]; ///< party members each side chooses from
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
  struct Sprite *memberSprites[8];
  s16 memberX[8];     ///< screen pixels
  s16 memberY[8];     ///< screen pixels
  s32 memberScale[8]; ///< 16.16 fixed point, Q16_ONE full size
  u16 selectedSlots[2];
  u16 selectedActions[2];
  struct WindowSprite *statusCursor;
  /// DjinnMenuWork's backdrop pointer at 0x184; no view names the rest.
  u8 aroundBackdrop[0x48];
  u16 entries[32];
  u16 memberIds[8]; ///< the party in order (Party_ListMembers)
  u8 entryCount;
  u8 partyCount;
  u8 selectedIds[2]; ///< each side's party member
  struct WindowSprite *selectedIcon;
  u16 page;           ///< 3 for the party member chooser, 0 for the Status menu
  s16 completionFlag; ///< ItemMenuWork's; not named here
  struct Sprite *rowSprites[4];
  s16 rowX[4]; ///< screen pixels
  s16 rowY[4]; ///< screen pixels
};

STATIC_ASSERT(offsetof(struct StatusScreenWork, memberWindow) == 0x10);
STATIC_ASSERT(offsetof(struct StatusScreenWork, memberCursors) == 0x14);
STATIC_ASSERT(offsetof(struct StatusScreenWork, statusWindow) == 0x24);
STATIC_ASSERT(offsetof(struct StatusScreenWork, entryIcons) == 0x48);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectorWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectedColumn) == 0x110);
STATIC_ASSERT(offsetof(struct StatusScreenWork, memberSprites) == 0x114);
STATIC_ASSERT(offsetof(struct StatusScreenWork, memberY) == 0x144);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectedSlots) == 0x174);
STATIC_ASSERT(offsetof(struct StatusScreenWork, statusCursor) == 0x17c);
STATIC_ASSERT(offsetof(struct StatusScreenWork, memberIds) == 0x208);
STATIC_ASSERT(offsetof(struct StatusScreenWork, selectedIds) == 0x21a);
STATIC_ASSERT(offsetof(struct StatusScreenWork, page) == 0x220);
STATIC_ASSERT(offsetof(struct StatusScreenWork, rowSprites) == 0x224);
STATIC_ASSERT(offsetof(struct StatusScreenWork, rowX) == 0x234);
STATIC_ASSERT(sizeof(struct StatusScreenWork) == 0x244);

/// The statuses the Status screens show, as StatusScreen_BuildAvailability
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
