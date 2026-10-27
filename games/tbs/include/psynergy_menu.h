#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

/// @file
/// The Psynergy menu's state.

#include "unit/menu/menu.h"
#include "heap_slots.h"
#include "static_assert.h"
#include "paged_list.h"
#include "render_input.h"
#include "text/msg_ids.h"

struct Sprite;

struct Unit;

/// The Psynergy menu: its windows, icons and cursors, the party members' tabs
/// and the Psynergy listed for the chosen party member. It is the
/// HEAP_SLOT_MENU block as this menu reads it, from the menu's opening to its
/// closing.
struct PsynergyMenuWork {
  u8 unused[8]; ///< no view of the menu work names it (see menu/README.md)
  s32 selectedMember;         ///< the party member chosen
  struct Window *moneyWindow; ///< ShopWork's; not named here
  struct Window *auxiliaryWindow;
  struct WindowSprite *sideIcon[2]; ///< each side's cursor
  s8 tabIndex[2];  ///< each side's party member, in memberTable
  s8 tabCounts[2]; ///< party members each side chooses from
  struct Window *psynergyWindow;
  struct Window *statusWindow;
  struct Window *shortcutWindow; ///< the L and R shortcuts
  struct Window *infoWindow;
  struct Window *equipWindow; ///< ItemMenuWork's; not named here
  struct Window *listWindow;
  /// ItemMenuWork's modalWindow and the two windows it never opens;
  /// not named here.
  u8 itemWindows[0x0c];
  struct WindowSprite *entryGridCursor;
  struct WindowSprite *entryIcons[32];
  struct WindowSprite *categoryIcons[8]; ///< ItemMenuWork's; not named here
  u8 unusedAfterIcons[0x24];             ///< no view of the menu work names it
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
  s32 memberScale[8];           ///< 16.16 fixed point, 0x10000 full size
  u16 sideRow[2];               ///< each side's Psynergy row chosen
  u16 sideAction[2]; ///< each side's Psynergy chosen, as a party member action
                     ///< entry
  struct WindowSprite *cursor; ///< ItemMenuWork's; not named here
  /// DjinnMenuWork's backdrop pointer at 0x184; no view names the rest.
  u8 aroundBackdrop[0x48];
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
  u8 figureRow[0x18];
  s16 slotY[4]; ///< screen pixels
  /// ItemMenuFiguresView's phases and DjinnMenuWork's pair; not named here.
  u8 figurePhases[0x16];
  s16 useResult;   ///< the use's result message, after MsgItemUseResult
  s8 equipPreview; ///< ItemMenuWork's; not named here
  u8 listMode;     ///< ItemMenuWork's; not named here
  u8 unusedAfterListMode[2];   ///< no view of the menu work names it
  s8 selectedIndexByMember[8]; ///< each party member's entry chosen last
  u8 mode; ///< 0 to use Psynergy; 1 or 2 to set the L or R shortcut
};

STATIC_ASSERT(offsetof(struct PsynergyMenuWork, auxiliaryWindow) == 0x010);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, psynergyWindow) == 0x020);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, entryGridCursor) == 0x044);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, entryIcons) == 0x048);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, messageWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, selectedColumn) == 0x110);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, selectedRow) == 0x111);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, columnCount) == 0x112);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, rowCount) == 0x113);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, sideAction[0]) == 0x178);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, psynergies) == 0x1c8);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, psynergyCount) == 0x218);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, memberIds) == 0x21a);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, selectedIndexByMember) ==
              0x260);

STATIC_ASSERT(offsetof(struct PsynergyMenuWork, skipSlide) == 0x222);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, tabSprites) == 0x114);
STATIC_ASSERT(offsetof(struct PsynergyMenuWork, memberScale) == 0x154);

#endif
