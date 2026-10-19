#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

/// @file
/// Menu lists: sprites, cursors, the party status window and paged selections.

#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "resource.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "text_render_runtime.h"
#include "window.h"
#include "static_assert.h"
#include "keystate.h"
#include "farcall.h"
#include "frame.h"

enum { BATTLE_LIST_ROWS = 5 };

/// A menu sprite: a list link and its OAM attributes, as two words or as
/// fields.
struct MenuSprite {
  struct MenuSprite *next;
  union {
    struct {
      unsigned long attr01;
      unsigned long attr23;
    } word;
    struct {
      u16 y : 8;
      u16 affine : 2;
      u16 mode : 2;
      u16 mosaic : 1;
      u16 colors : 1;
      u16 shape : 2;
      u16 x : 9;
      u16 affineIndex : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      u16 unused;
    } f;
  } oam;
};

/// A cursor or prompt sprite: a list link and its OAM attributes as fields.
struct CursorSprite {
  struct CursorSprite *next;
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 mosaic : 1;
  u16 colors : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unused;
};

#define MENU_SPRITE_SIZE_16 0x40000000

/// Sets sprite shape (attr0/attr1) and clears the remaining fields.
/* FAKEMATCH: the one-pass block keeps both attribute words ahead of the stores
 * that follow (the x/y fields in the list setup loops of ItemList_SelectEntry,
 * BattleMenu_RunActionSelection and SummonMenu_SelectSummon; the tile in
 * UiText_DrawCharacter). Not source: the same two stores match only as plain
 * statements in Battle_CollectPartyCommands (resource_menu.c, five times) and
 * Ui_RunOwnerStatusScreen (preview.c, three times). */
#define MenuSprite_Init(sprite, shape)                                         \
  do {                                                                         \
    (sprite).oam.word.attr01 = (shape);                                        \
    (sprite).oam.word.attr23 = 0;                                              \
  } while (0)

/// The party status window's layout: its window, its place and size, and what
/// it shows (PARTY_STATUS_ flags).
struct PartyStatusWindow {
  struct UiWindow *window;
  u16 x;      ///< tiles
  u16 y;      ///< tiles
  u16 width;  ///< tiles
  u16 height; ///< tiles
  u16 flags;  ///< PARTY_STATUS_ flags
};

STATIC_ASSERT(sizeof(struct PartyStatusWindow) == 16);
STATIC_ASSERT(offsetof(struct PartyStatusWindow, flags) == 12);

/// A point on the screen where a menu shows its cursor.
struct MenuPoint {
  s32 x; ///< screen pixels
  s32 y; ///< screen pixels
};

/// The menu work that remembers the cursor between menus (page, row and
/// commands chosen) and drives the Djinn tutorial's key presses.
struct MenuCell {
  u8 unk00[0x28]; ///< LinkRoundState's sprites and their flags
  /// LinkRoundState.targetOffset: -2 or 16 by the target menu's mode, 0 after.
  s32 slideOffset;
  u8 unk2c[4];      ///< LinkRoundState.currentOffset; not named through here
  s32 row;          ///< the list row chosen last
  s32 page;         ///< the list page chosen last
  s32 preferredRow; ///< the row to return to
  /// The command row's choice: 0 in its first mode, 1 in the other.
  s32 commandSelection[2];
  void *work; ///< LinkRoundState.displayHandle; not named through here
  s32 busy;   ///< LinkRoundState.secondaryHandle; cleared by the status screen
  /// LinkRoundState.timer, the link battle's countdown: at 0 a menu ends as
  /// if cancelled, or takes the first command; -1 outside link battles.
  s32 active;
  u8 unk50[0x88]; ///< LinkRoundState's enabled flag and lists
  /// Set in the Djinn tutorial (LinkRoundState.hintCount): the target menu
  /// then presses a button for the player every autoDelay frames.
  s32 autoEnabled;
  s32 autoDelay; ///< frames
  s32 autoMode;  ///< LinkRoundState.actorId; not read through here
};

STATIC_ASSERT(offsetof(struct MenuCell, slideOffset) == 0x28);
STATIC_ASSERT(offsetof(struct MenuCell, row) == 0x30);
STATIC_ASSERT(offsetof(struct MenuCell, commandSelection) == 0x3c);
STATIC_ASSERT(offsetof(struct MenuCell, work) == 0x44);
STATIC_ASSERT(offsetof(struct MenuCell, busy) == 0x48);
STATIC_ASSERT(offsetof(struct MenuCell, active) == 0x4c);
STATIC_ASSERT(offsetof(struct MenuCell, autoEnabled) == 0xd8);
STATIC_ASSERT(offsetof(struct MenuCell, autoDelay) == 0xdc);
STATIC_ASSERT(offsetof(struct MenuCell, autoMode) == 0xe0);

/// Restores the previous selection: page, row and return row.
// A statement macro (S2: a layer, 4 sites): the three cursor reads stay
// together at every use; as a plain block, tbs-en differs.
#define MenuCell_GetCursor(page_, row_, preferred_)                            \
  do {                                                                         \
    (page_) = ((struct MenuCell *)gWindowWork[42])->page;                      \
    (row_) = ((struct MenuCell *)gWindowWork[42])->row;                        \
    (preferred_) = ((struct MenuCell *)gWindowWork[42])->preferredRow;         \
  } while (0)

/// Saves the selection: page, row and return row.
/* FAKEMATCH: the do { } while (0) wrapper steers, so this is a device at each
 * of its 7 sites, not a layer (S2: not every use matches through it).
 * Battle_SelectAbility writes the same three stores plainly through its already
 * loaded menu pointer, and through this macro it differs 2 in all six editions
 * (measured 2026-10-13). At the 7 sites the work-cell pointer it loads stays
 * apart from the reload after it, and the three stores finish before the keys
 * are read; written as three plain stores, with the dead-store pads gone,
 * SummonMenu_SelectSummon differs 84 (stores through its nav pointer; through
 * gLinkCountdownWork, the macro's own stores unwrapped, it differs 4),
 * ItemList_SelectEntry 46 and BattleMenu_RunActionSelection 42 in tbs-en, and
 * elsewhere the row store goes first (differ 4) or the base is loaded before
 * mode = 0 (differ 2 to 4). */
#define MenuCell_SetCursor(page_, row_, preferred_)                            \
  do {                                                                         \
    gLinkCountdownWork->page = (page_);                                        \
    gLinkCountdownWork->row = (row_);                                          \
    gLinkCountdownWork->preferredRow = (preferred_);                           \
  } while (0)

extern struct MenuCell *gLinkCountdownWork;
extern u8 gResource_FixedBlockBTiles[];
extern const u8 gMenu_AnimatedCursorTiles[];

#endif
