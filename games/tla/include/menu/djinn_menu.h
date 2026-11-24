#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

/// @file
/// The Djinn menu: its lists, backdrop and state, the fields of a Djinni's list
/// entry, and the Djinn tutorial's lines.

#include "static_assert.h"
#include "types.h"
#include "fixed_math.h"

// A Djinni's list entry, as Djinn_ListUnitEntries writes it into
// DjinnMenuBackdrop.ids. bits: 15 set | 11..8 member | 7..5 element | 4..0
// index
#define DJINN_ENTRY_INDEX 31      ///< its number within its element
#define DJINN_ENTRY_ELEMENT 0xe0  ///< its element (enum Element)
#define DJINN_ENTRY_MEMBER 0xf00  ///< the party member holding it
#define DJINN_ENTRY_IS_SET 0x8000 ///< set; a standby one is TEXT_COLOR_RED
#define DJINN_ENTRY_ELEMENT_SHIFT 5
#define DJINN_ENTRY_MEMBER_SHIFT 8

#define DJINN_ELEMENTS 4
#define DJINN_LIST_MEMBERS 8 ///< list columns: one per party member
#define DJINN_LIST_ROWS 10   ///< entries one party member's list holds

/// The Djinn tutorial's lines, counted from its first (Menu_GetDjinnGiveMessage
/// and Tutorial_GetDjinnGiveMessage: one of its two copies, by
/// FLAG_ALREADY_KNOWS_DJINN).
enum DjinnTutorialLine {
  /// "To give your Djinni to someone else, move it like this."
  DJINN_TUTORIAL_LINE_GIVE = 0,
  /// "After choosing who gets the Djinni, you can \"set\" it."
  DJINN_TUTORIAL_LINE_SET_AFTER_GIVE = 1,
  /// "When your class changes, the Psynergy you can use may also change."
  DJINN_TUTORIAL_LINE_CLASS_PSYNERGY = 2,
  /// "Setting a Djinni changes your attributes and sometimes even your class."
  DJINN_TUTORIAL_LINE_SET_CHANGES = 3,
  /// "Each Djinni has different, unique powers."
  DJINN_TUTORIAL_LINE_POWERS = 4,
  /// "Once we have been set, you can unleash us in battle to use our powers."
  DJINN_TUTORIAL_LINE_UNLEASH = 5,
  /// "So, {protagonist_name}, let me demonstrate the power of my attack now."
  DJINN_TUTORIAL_LINE_DEMONSTRATE = 6,
  /// "See! What did I tell you? When you unleashed me, I used my special
  /// attack ability!"
  DJINN_TUTORIAL_LINE_UNLEASHED = 7,
  /// "After being unleashed, we Djinn will then stand by, ready to be summoned
  /// back again..."
  DJINN_TUTORIAL_LINE_STANDBY = 8,
  /// "If you have multiple standby Djinn, you can summon even greater powers
  /// into battle!"
  DJINN_TUTORIAL_LINE_SUMMON = 9,
  /// "Choose this command when you want to unleash me in battle, all right?"
  DJINN_TUTORIAL_LINE_UNLEASH_COMMAND = 10,
  /// "After a Djinni has been used to summon, it must spend time in recovery."
  DJINN_TUTORIAL_LINE_RECOVERY = 12,
  /// "And once we've recovered, we automatically set ourselves again, so we're
  /// ready to attack!"
  DJINN_TUTORIAL_LINE_SET_AGAIN = 13,
  /// "That's all you have to do to unleash a set Djinni!"
  DJINN_TUTORIAL_LINE_UNLEASH_HINT = 14
};

/// The Djinn menu's lists, the field tiles and palette it restores on closing,
/// and the tutorial's state, allocated when the menu opens and released when
/// it closes.
struct DjinnMenuBackdrop {
  u16 ids[DJINN_LIST_MEMBERS][DJINN_LIST_ROWS]; ///< each party member's Djinn
  s8 counts[DJINN_LIST_MEMBERS];
  u8 tiles[8192];    ///< BG character block 1's first 8 KB
  u8 palette[128];   ///< BG palettes 4 to 7
  s32 tutorialTimer; ///< frames into the tutorial's current step
  s32 tutorialStep;  ///< enum DjinnTutorialStep
};

/// A window as the Djinn menu's preview reads it: a Window whose y it
/// reads signed, in tiles.
struct DjinnPreviewWindow {
  u8 sprites[8]; ///< the window's sprite list; not read here
  u16 width;
  u16 height;
  u16 x;
  s16 y;
};

/// DjinnMenuState.pending's bits: what the chosen pair still waits for.
enum DjinnPending {
  DJINN_PENDING_SECOND = 1, ///< Unused: a second Djinni is to be chosen
  DJINN_PENDING_CONFIRM = 2 ///< a give or trade awaits its confirmation
};

/// The Djinn menu's state, in the HEAP_SLOT_MENU heap slot from the menu's
/// opening to its closing, laid out as ItemMenuState is: it names only what the
/// Djinn screen reads, and says whose the rest is.
struct DjinnMenuState {
  u8 unused[8];    ///< no view of the menu state names it
  s32 member;      ///< the party member whose Djinn are shown
  s32 moneyWindow; ///< ShopState's; not named here
  s32 statusWindow;
  struct WindowSprite *cursor;
  /// The second of ItemMenuState's sideIcons; not named here.
  struct WindowSprite *secondSideIcon;
  s8 selectedColumn[2];   ///< each side's party member, an index into members
  s8 sideCount[2];        ///< ItemMenuState's; not named here
  u8 unusedAfterSides[4]; ///< no view of the menu state names it
  s32 itemWindow;         ///< ItemMenuState's and ShopState's; not named here
  s32 window;
  s32 unopenedWindow;
  s32 infoWindow; ///< ItemMenuState's; not named here
  s32 listWindow;
  struct DjinnPreviewWindow *secondWindow;
  /// ItemMenuState's last windows and its icons; not named here.
  ///@{
  u8 itemWindows[16];
  struct WindowSprite *entryIcons[32];
  ///@}
  u8 unusedAfterIcons[36]; ///< no view of the menu state names it
  s32 helpWindow;
  u8 cursorGrid[4];       ///< ItemMenuState's cursor grid; not named here
  void *memberSprites[8]; ///< the party members' figures
  s16 memberX[8];         ///< screen pixels
  s16 memberY[8];         ///< screen pixels
  FIXED memberScale[8];   ///< ItemMenuState's; not named here
  s16 memberSpriteIds[8]; ///< ItemMenuState's; not named here
  u16 selection[2];       ///< each side's cursor (DJINN_CURSOR_ROW_STRIDE)
  u16 pairEntries[2];     ///< each side's chosen list entry
  struct WindowSprite *secondCursor;
  u8 unusedBeforeArrows[4]; ///< no view of the menu state names it
  /// The up and down arrows beside the party row (MenuCursor_Create).
  struct WindowSprite *pageArrows[2];
  struct DjinnMenuBackdrop *backdrop;
  u8 unusedAfterBackdrop[64];      ///< no view of the menu state names it
  u16 entries[32];                 ///< ItemMenuState's items; not named here
  u16 members[DJINN_LIST_MEMBERS]; ///< the party in order
  u8 pairCount;                    ///< not read by name here
  u8 memberCount;
  u8 shownMember[2]; ///< the party member giving and the one receiving
  /// ItemMenuState's selectedItemIcon; not named here.
  struct WindowSprite *selectedIcon;
  u16 pending;        ///< DjinnPending bits
  s16 completionFlag; ///< ItemMenuState's; not named here
  void *figures[4];   ///< ItemMenuState's; not named here
  u16 slotX[4];
  u16 slotY[4];
  s32 figurePhases[4];  ///< ItemMenuState's figureScales; not named here
  u16 figureFacings[4]; ///< ItemMenuState's; not named here
  /// The Djinn chosen on each side: index in their element, element, party
  /// member.
  ///@{
  u8 pairIndex[2];
  u8 pairElement[2];
  u8 pairMember[2];
  ///@}
  s16 messageOffset; ///< ItemMenuState's; not named here
};

STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tiles) == 168);
STATIC_ASSERT(offsetof(struct DjinnMenuBackdrop, tutorialTimer) == 8488);
STATIC_ASSERT(offsetof(struct DjinnMenuState, window) == 40);
STATIC_ASSERT(offsetof(struct DjinnMenuState, helpWindow) == 240);
STATIC_ASSERT(offsetof(struct DjinnMenuState, memberY) == 296);
STATIC_ASSERT(offsetof(struct DjinnMenuState, backdrop) == 384);
STATIC_ASSERT(offsetof(struct DjinnMenuState, members) == 516);
STATIC_ASSERT(offsetof(struct DjinnMenuState, shownMember) == 534);
STATIC_ASSERT(offsetof(struct DjinnMenuState, pairMember) == 604);

#endif
