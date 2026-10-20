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

/// The battle command menus' work, heap slot 57 (0x104 bytes), reached as
/// gLinkCountdownWork. Battle_CollectPartyCommands allocates it each round; it
/// holds the icons beside the command window, the camera slide, the cursor
/// each list remembers, the link battle's timer, the Djinn tutorial's
/// key presses and what each command has spent.
struct BattleMenuWork {
  /// The sprites UpdateLinkSessionCountdown draws each frame: the chosen
  /// command's icon, the chosen ability's icon and the member's portrait.
  struct MenuSprite icons[3];
  u8 iconShown[3]; ///< whether each of icons is drawn
  u8 unk27;
  /// Where the battle scene slides to (Camera_ConfigureScene's vertical
  /// offset): -2 or 16 by the target menu's mode, 0 otherwise.
  s32 targetSceneOffset;
  s32 sceneOffset;  ///< where the slide has reached; a third of the way a frame
  s32 row;          ///< the list row chosen last
  s32 page;         ///< the list page chosen last
  s32 preferredRow; ///< the row to return to
  /// The command row's choice: [0] for the party's row (fight, flee,
  /// status), [1] for a member's commands.
  s32 commandSelection[2];
  /// The window that shows the link battle's seconds left, a struct UiWindow
  /// pointer kept as a word (as a pointer, two of the round's first stores
  /// swap), or 0.
  s32 timerWindow;
  /// Set while the status screen is open, so the timer opens no window.
  s32 timerHidden;
  /// Frames left to choose commands in a link battle; -1 outside link battles.
  /// At 0 a menu ends as if cancelled, or takes the first command.
  s32 timer;
  s32 linkTimer; ///< 1 while the link battle's timer runs
  /// The party's standby Djinn by element before each command: what the
  /// summons chosen by the earlier commands leave.
  u8 standbyDjinn[8][4];
  u8 listSlots[34];      ///< the Psynergy list's action slots, ended by 0
  u16 listAbilities[33]; ///< the Psynergy list's ability ids, ended by 0
  /// The Djinn tutorial's step: 0 off, 1 from the start of each round while
  /// FLAG_DJINN_TUTORIAL is set, then one more for each hint shown. While it
  /// is not 0 the menus press keys for the player.
  s32 tutorialStep;
  s32 tutorialDelay; ///< frames until the tutorial's next key press
  /// The member whose commands are being chosen; -1 between members.
  s32 commandActor;
  /// The Djinni each command has chosen, element << 8 | index, or 0x80000000.
  u32 chosenDjinn[8];
};

STATIC_ASSERT(sizeof(struct BattleMenuWork) == 0x104);
STATIC_ASSERT(offsetof(struct BattleMenuWork, targetSceneOffset) == 0x28);
STATIC_ASSERT(offsetof(struct BattleMenuWork, row) == 0x30);
STATIC_ASSERT(offsetof(struct BattleMenuWork, commandSelection) == 0x3c);
STATIC_ASSERT(offsetof(struct BattleMenuWork, timerWindow) == 0x44);
STATIC_ASSERT(offsetof(struct BattleMenuWork, timer) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleMenuWork, standbyDjinn) == 0x54);
STATIC_ASSERT(offsetof(struct BattleMenuWork, tutorialStep) == 0xd8);
STATIC_ASSERT(offsetof(struct BattleMenuWork, commandActor) == 0xe0);
STATIC_ASSERT(offsetof(struct BattleMenuWork, chosenDjinn) == 0xe4);

/// Restores the previous selection: page, row and return row.
// A statement macro (S2: a layer, 4 sites): the three cursor reads stay
// together at every use; as a plain block, tbs-en differs.
#define BATTLE_MENU_GET_CURSOR(page_, row_, preferred_)                        \
  do {                                                                         \
    (page_) = ((struct BattleMenuWork *)gWindowWork[42])->page;                \
    (row_) = ((struct BattleMenuWork *)gWindowWork[42])->row;                  \
    (preferred_) = ((struct BattleMenuWork *)gWindowWork[42])->preferredRow;   \
  } while (0)

/// Saves the selection: page, row and return row.
/* FAKEMATCH(one-pass): the do { } while (0) wrapper steers, so this is a device
 * at each of its 7 sites, not a layer (S2: not every use matches through it).
 * Battle_SelectAbility writes the same three stores plainly through its already
 * loaded menu pointer, and through this macro it differs 2 in all six editions.
 * At the 7 sites the work-cell pointer it loads stays
 * apart from the reload after it, and the three stores finish before the keys
 * are read; written as three plain stores, with the dead-store pads gone,
 * SummonMenu_SelectSummon differs 84 (stores through its nav pointer; through
 * gLinkCountdownWork, the macro's own stores unwrapped, it differs 4),
 * ItemList_SelectEntry 46 and BattleMenu_RunActionSelection 42 in tbs-en, and
 * elsewhere the row store goes first (differ 4) or the base is loaded before
 * mode = 0 (differ 2 to 4). */
#define BATTLE_MENU_SET_CURSOR(page_, row_, preferred_)                        \
  do {                                                                         \
    gLinkCountdownWork->page = (page_);                                        \
    gLinkCountdownWork->row = (row_);                                          \
    gLinkCountdownWork->preferredRow = (preferred_);                           \
  } while (0)

extern struct BattleMenuWork *gLinkCountdownWork;
extern u8 gResource_FixedBlockBTiles[];
extern const u8 gMenu_AnimatedCursorTiles[];

#endif
