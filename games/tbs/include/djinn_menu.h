#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

/// @file
/// The Djinn menu: its list tables, backdrop and work, and the fields of a
/// Djinni's list entry.

#include "types.h"
#include "global_cells.h"
#include "render_input.h"
#include "djinn_ids.h"

// A Djinni's list entry, as Djinn_ListOwnerEntries writes it into
// DjinnListTable.ids. bits: 15 set | 11..8 owner | 7..5 element | 4..0 index
#define DJINN_ENTRY_INDEX 31      ///< its number within its element
#define DJINN_ENTRY_ELEMENT 0xe0  ///< its element (enum Element)
#define DJINN_ENTRY_OWNER 0xf00   ///< the member holding it
#define DJINN_ENTRY_IS_SET 0x8000 ///< set; an unset one is drawn in colour 2

#define DJINN_ELEMENTS 4
#define DJINN_LIST_OWNERS 8 ///< list columns: one per party member
#define DJINN_LIST_ROWS 10  ///< entries one member's list holds

/// The Djinn menu's lists: each party member's Djinn entries and their count.
struct DjinnListTable {
  u16 ids[DJINN_LIST_OWNERS][DJINN_LIST_ROWS];
  s8 counts[DJINN_LIST_OWNERS];
};

/// The Djinn menu's lists, the field tiles and palette it restores on closing,
/// and the tutorial's state. The menu allocates it outside any slot, EWRAM
/// first, as it opens, and frees it (MemFree) as it closes.
struct DjinnMenuBackdrop {
  struct DjinnListTable list;
  u8 tiles[0x2000];  ///< BG character block 1's first 0x2000 bytes
  u16 palette[64];   ///< BG palettes 4 to 7
  s32 tutorialTimer; ///< frames into the tutorial's current step
  /// The Djinn tutorial's step (DJINN_TUTORIAL_ in menu_internal.h), chosen
  /// to start by FLAG_DJINN_LESSON_PART_A and FLAG_DJINN_LESSON_PART_B.
  long tutorialStep;
};

/// The Djinn menu's work, in the HEAP_SLOT_MENU work slot from the menu's
/// opening to its closing.
struct DjinnMenuWork {
  u8 unk000[8]; ///< not named by TBS code
  s32 owner;
  u8 unk00c[4]; ///< not named by TBS code
  struct UiWindow *statusWindow;
  struct RenderOutput *cursor;
  u8 unk018[4];         ///< not named by TBS code
  s8 selectedColumn[2]; ///< each pane's member, in owners
  u8 unk01e[6];         ///< not named by TBS code
  struct UiWindow *window;
  /// Never given a window, so NULL from UiWindow_InitializeWork on; the preview
  /// still redraws it, which reads the window's place from the BIOS. Outside
  /// the BIOS that returns the last BIOS instruction fetched, whose low half
  /// as a width is over 30, so UiWindow_DrawFrame draws nothing.
  struct UiWindow *unknownWindow028;
  u8 unk02c[4]; ///< not named by TBS code
  struct UiWindow *listWindow;
  struct UiWindow *secondWindow;
  u8 unk038[0xd4]; ///< not named by TBS code
  struct UiWindow *helpWindow;
  u8 unk110[0x24];    ///< not named by TBS code
  s16 ownerX[8];      ///< screen pixels
  s16 ownerY[8];      ///< screen pixels
  u8 unk154[0x20];    ///< not named by TBS code
  u16 selection[2];   ///< each pane's Djinni row chosen
  u16 pairEntries[2]; ///< 255 at first
  struct RenderOutput *secondCursor;
  u8 unk180[0x04]; ///< not named by TBS code
  struct DjinnMenuBackdrop *backdrop;
  u8 unk188[0x80];               ///< not named by TBS code
  u16 owners[DJINN_LIST_OWNERS]; ///< the party in order
  u8 pairCount;
  u8 ownerCount;
  u8 shownOwner[2]; ///< the member giving and the one receiving
  u8 unk21c[4];     ///< not named by TBS code
  /// bits: 1 a give or trade awaits its confirmation | 0 a second Djinni is
  /// to be chosen
  u16 pending;
  u8 unk222[0x12]; ///< not named by TBS code
  u16 slotX[4];
  u16 slotY[4];
  u8 unk244[0x10]; ///< not named by TBS code
  /// The Djinn chosen in each pane: index in their element, element, owner.
  ///@{
  u8 pairIndex[2];
  u8 pairElement[2];
  u8 pairOwner[2]; ///< not named by TBS code
  ///@}
};

STATIC_ASSERT(offsetof(struct DjinnMenuWork, statusWindow) == 0x10);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, selectedColumn) == 0x1c);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, ownerX) == 0x134);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, ownerY) == 0x144);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, selection) == 0x174);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, pending) == 0x220);
STATIC_ASSERT(offsetof(struct DjinnMenuWork, pairIndex) == 0x254);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialTimer) == 0x2128);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialStep) == 0x212c);

#endif
