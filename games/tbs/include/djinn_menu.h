#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

/// @file
/// The Djinn menu: its list tables, backdrop and work, and the fields of a
/// Djinni's list entry.

#include "types.h"
#include "heap_slots.h"
#include "render_input.h"
#include "djinn_ids.h"

// A Djinni's list entry, as Djinn_ListUnitEntries writes it into
// DjinnListTable.ids. bits: 15 set | 11..8 member | 7..5 element | 4..0 index
#define DJINN_ENTRY_INDEX 31      ///< its number within its element
#define DJINN_ENTRY_ELEMENT 0xe0  ///< its element (enum Element)
#define DJINN_ENTRY_MEMBER 0xf00  ///< the member holding it
#define DJINN_ENTRY_IS_SET 0x8000 ///< set; an unset one is drawn in colour 2

#define DJINN_ELEMENTS 4
#define DJINN_LIST_MEMBERS 8 ///< list columns: one per party member
#define DJINN_LIST_ROWS 10   ///< entries one member's list holds

/// The Djinn menu's lists: each party member's Djinn entries and their count.
struct DjinnListTable {
  u16 ids[DJINN_LIST_MEMBERS][DJINN_LIST_ROWS];
  s8 counts[DJINN_LIST_MEMBERS];
};

/// The Djinn menu's lists, the field tiles and palette it restores on closing,
/// and the tutorial's state. The menu allocates it outside any slot, EWRAM
/// first, as it opens, and frees it (Free) as it closes.
struct DjinnMenuBackdrop {
  struct DjinnListTable list;
  u8 tiles[0x2000];  ///< BG character block 1's first 0x2000 bytes
  u16 palette[64];   ///< BG palettes 4 to 7
  s32 tutorialTimer; ///< frames into the tutorial's current step
  /// The Djinn tutorial's step (DJINN_TUTORIAL_ in menu_internal.h), chosen
  /// to start by FLAG_DJINN_LESSON_PART_A and FLAG_DJINN_LESSON_PART_B.
  long tutorialStep;
};

/// The Djinn menu's work, in the HEAP_SLOT_MENU heap slot from the menu's
/// opening to its closing.
struct DjinnMenuWork {
  u8 unused[8]; ///< no view of the menu work names it (see menu/README.md)
  s32 member;
  struct Window *moneyWindow; ///< ShopWork's; not named here
  struct Window *statusWindow;
  struct WindowSprite *cursor;
  /// The second of ItemMenuWork's sideIcons; not named here.
  struct WindowSprite *secondSideIcon;
  s8 selectedColumn[2]; ///< each side's member, an index into members
  s8 sideCount[2];      ///< ItemMenuWork's; not named here
  /// ItemMenuWork's and ShopWork's itemWindow; not named here.
  struct Window *itemWindow;
  struct Window *window;
  /// Never given a window, so NULL from Window_InitializeWork on; the preview
  /// still redraws it, which reads the window's place from the BIOS. Outside
  /// the BIOS that returns the last BIOS instruction fetched, whose low half
  /// as a width is over 30, so Window_DrawFrame draws nothing.
  struct Window *unopenedWindow;
  struct Window *infoWindow; ///< ItemMenuWork's; not named here
  struct Window *listWindow;
  struct Window *secondWindow;
  /// ItemMenuWork's last windows and its icons; not named here.
  ///@{
  u8 itemWindows[0x10];
  struct WindowSprite *entryIcons[32];
  struct WindowSprite *categoryIcons[8];
  ///@}
  u8 unusedAfterIcons[0x24]; ///< no view of the menu work names it
  struct Window *helpWindow;
  /// ItemMenuWork's cursor grid, selectedColumn to rowCount; not named
  /// here.
  u8 cursorGrid[4];
  /// ItemMenuWork's party figures; not named here.
  struct Sprite *memberSprites[8];
  s16 memberX[8];     ///< screen pixels
  s16 memberY[8];     ///< screen pixels
  s32 memberScale[8]; ///< ItemMenuWork's; not named here
  u16 selection[2];   ///< each side's Djinni row chosen
  u16 pairEntries[2]; ///< 255 at first
  struct WindowSprite *secondCursor;
  u8 unusedBeforeBackdrop[4]; ///< no view of the menu work names it
  struct DjinnMenuBackdrop *backdrop;
  u8 unusedAfterBackdrop[0x40];    ///< no view of the menu work names it
  u16 entries[32];                 ///< ItemMenuWork's items; not named here
  u16 members[DJINN_LIST_MEMBERS]; ///< the party in order
  u8 pairCount;
  u8 memberCount;
  u8 shownMember[2]; ///< the member giving and the one receiving
  /// ItemMenuWork's selectedItemIcon; not named here.
  struct WindowSprite *selectedIcon;
  /// bits: 1 a give or trade awaits its confirmation | 0 a second Djinni is
  /// to be chosen
  u16 pending;
  s16 completionFlag; ///< ItemMenuWork's; not named here
  /// ItemMenuFiguresView's animations, the figures the Djinn screen
  /// moves through that view.
  struct Sprite *figures[4];
  u16 slotX[4];
  u16 slotY[4];
  s32 figurePhases[4]; ///< ItemMenuFiguresView's phases; not named here
  /// The Djinn chosen in each side: index in their element, element, member.
  ///@{
  u8 pairIndex[2];
  u8 pairElement[2];
  u8 pairMember[2]; ///< not named by TBS code
  ///@}
};

STATIC_ASSERT(offsetof(struct DjinnMenuWork, statusWindow) == 0x10);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, selectedColumn) == 0x1c);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, memberX) == 0x134);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, memberY) == 0x144);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, selection) == 0x174);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, pending) == 0x220);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, pairIndex) == 0x254);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialTimer) == 0x2128);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialStep) == 0x212c);

#endif
