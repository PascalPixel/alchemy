#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

/// @file
/// The Djinn menu: its list tables, backdrop and state, and the fields of a
/// Djinni's list entry.

#include "djinn_ids.h"
#include "types.h"
#include "graphics/render_input.h"
#include "system/heap_slots.h"

// A Djinni's list entry, as Djinn_ListUnitEntries writes it into
// DjinnListTable.ids. bits: 15 set | 11..8 member | 7..5 element | 4..0 index
#define DJINN_ENTRY_INDEX 31      ///< its number within its element
#define DJINN_ENTRY_ELEMENT 0xe0  ///< its element (enum Element)
#define DJINN_ENTRY_MEMBER 0xf00  ///< the party member holding it
#define DJINN_ENTRY_IS_SET 0x8000 ///< set; an unset one is drawn in colour 2

#define DJINN_ELEMENTS 4 ///< Venus, Mercury, Mars and Jupiter (enum Element)
#define DJINN_LIST_MEMBERS 8 ///< list columns: one per party member
#define DJINN_LIST_ROWS 10   ///< entries one party member's list holds

/// The Djinn menu's lists: each party member's Djinn entries and their count.
struct DjinnListTable {
  u16 ids[DJINN_LIST_MEMBERS][DJINN_LIST_ROWS];
  s8 counts[DJINN_LIST_MEMBERS];
};

/// The Djinn menu's lists, the field tiles and palette it restores on closing,
/// and the tutorial's state.
struct DjinnMenuBackdrop {
  struct DjinnListTable list;
  u8 tiles[8192];    ///< BG character block 1's first 8 KB
  u16 palette[64];   ///< BG palettes 4 to 7
  s32 tutorialTimer; ///< frames into the tutorial's current step
  /// The Djinn tutorial's step (DJINN_TUTORIAL_ in menu_internal.h), chosen
  /// to start by FLAG_DJINN_LESSON_PART_A and FLAG_DJINN_LESSON_PART_B.
  long tutorialStep;
};

/// DjinnMenuState.pending's bits: what the chosen pair still waits for.
enum DjinnPending {
  DJINN_PENDING_SECOND = 1, ///< a second Djinni is to be chosen
  DJINN_PENDING_CONFIRM = 2 ///< a give or trade awaits its confirmation
};

/// The Djinn menu's state, in the HEAP_SLOT_MENU heap slot from the menu's
/// opening to its closing.
struct DjinnMenuState {
  u8 unused[8]; ///< no view of the menu state names it (see menu/README.md)
  s32 member;
  struct Window *moneyWindow; ///< ShopState's; not named here
  struct Window *statusWindow;
  struct WindowSprite *cursor;
  /// The second of ItemMenuState's sideIcons; not named here.
  struct WindowSprite *secondSideIcon;
  s8 selectedColumn[2]; ///< each side's party member, an index into party
                        ///< members
  s8 sideCount[2];      ///< ItemMenuState's; not named here
  /// ItemMenuState's and ShopState's itemWindow; not named here.
  struct Window *itemWindow;
  struct Window *window;
  /// Never given a window, so NULL from Menu_InitializeState on; the preview
  /// still redraws it, which reads the window's place from the BIOS.
  struct Window *unopenedWindow;
  struct Window *infoWindow; ///< ItemMenuState's; not named here
  struct Window *listWindow;
  struct Window *secondWindow;
  /// ItemMenuState's last windows and its icons; not named here.
  ///@{
  u8 itemWindows[16];
  struct WindowSprite *entryIcons[32];
  struct WindowSprite *categoryIcons[8];
  ///@}
  u8 unusedAfterIcons[36]; ///< no view of the menu state names it
  struct Window *helpWindow;
  /// ItemMenuState's cursor grid, selectedColumn to rowCount; not named
  /// here.
  u8 cursorGrid[4];
  /// ItemMenuState's party figures; not named here.
  struct Sprite *memberSprites[8];
  s16 memberX[8];     ///< screen pixels
  s16 memberY[8];     ///< screen pixels
  s32 memberScale[8]; ///< ItemMenuState's; not named here
  u16 selection[2];   ///< each side's Djinni row chosen
  u16 pairEntries[2]; ///< 255 at first
  struct WindowSprite *secondCursor;
  u8 unusedBeforeBackdrop[4]; ///< no view of the menu state names it
  struct DjinnMenuBackdrop *backdrop;
  u8 unusedAfterBackdrop[64];      ///< no view of the menu state names it
  u16 entries[32];                 ///< ItemMenuState's items; not named here
  u16 members[DJINN_LIST_MEMBERS]; ///< the party in order
  u8 pairCount;
  u8 memberCount;
  u8 shownMember[2]; ///< the party member giving and the one receiving
  /// ItemMenuState's selectedItemIcon; not named here.
  struct WindowSprite *selectedIcon;
  u16 pending;        ///< DjinnPending bits
  s16 completionFlag; ///< ItemMenuState's; not named here
  /// ItemMenuFiguresView's animations, the figures the Djinn screen
  /// moves through that view.
  struct Sprite *figures[4];
  u16 slotX[4];
  u16 slotY[4];
  s32 figurePhases[4]; ///< ItemMenuFiguresView's phases; not named here
  /// The Djinn chosen in each side: index in their element, element, party
  /// member.
  ///@{
  u8 pairIndex[2];
  u8 pairElement[2];
  u8 pairMember[2];
  ///@}
};

STATIC_ASSERT(offsetof(struct DjinnMenuState, statusWindow) == 16);
STATIC_ASSERT(offsetof(struct DjinnMenuState, selectedColumn) == 28);
STATIC_ASSERT(offsetof(struct DjinnMenuState, memberX) == 308);
STATIC_ASSERT(offsetof(struct DjinnMenuState, memberY) == 324);
STATIC_ASSERT(offsetof(struct DjinnMenuState, selection) == 372);
STATIC_ASSERT(offsetof(struct DjinnMenuState, pending) == 544);
STATIC_ASSERT(offsetof(struct DjinnMenuState, pairIndex) == 596);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialTimer) == 8488);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialStep) == 8492);

#endif
