#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

/// @file
/// Menu lists: sprites, cursors, the party status window and paged selections.

#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "types.h"
#include "resource.h"
#include "task.h"
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

/// attr0 and attr1 of a square 16x16 sprite, as one word: size 1 (attr1 bits
/// 14 and 15) and every other field 0.
#define MENU_SPRITE_SHAPE_16X16 0x40000000

/// Sets sprite shape (attr0/attr1) and clears the remaining fields.
/* FAKEMATCH(one-pass): the two stores in a once-only block; as the plain stores
 * Battle_CollectPartyCommands and BattleStatusScreen_Run match with, 4 of 49
 * instructions differ in tbs-en (Text_DrawCharacter). */
#define MenuSprite_Initialize(sprite, shape)                                   \
  do {                                                                         \
    (sprite).oam.word.attr01 = (shape);                                        \
    (sprite).oam.word.attr23 = 0;                                              \
  } while (0)

/// The party status window's layout: its window, its place and size, and what
/// it shows (PARTY_STATUS_ flags).
struct PartyStatusWindow {
  struct Window *window;
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

/// The battle command menus' work, HEAP_SLOT_BATTLE_MENU (0x104 bytes),
/// reached as gBattleMenuWork. Battle_CollectPartyCommands allocates it each
/// round; it holds the icons beside the command window, the camera slide, the
/// cursor each list remembers, the link battle's timer, the Djinn tutorial's
/// key presses and what each command has spent.
struct BattleMenuWork {
  /// The sprites UpdateLinkSessionCountdown draws each frame: the chosen
  /// command's icon, the chosen ability's icon and the member's portrait.
  struct MenuSprite icons[3];
  u8 iconShown[3]; ///< whether each of icons is drawn
  u8 padding;      ///< up to the word targetViewOffset starts on
  /// Where the battle view slides to (BattleCamera_SetCommandView's vertical
  /// offset): -2 or 16 by the target menu's mode, 0 otherwise.
  s32 targetViewOffset;
  s32 viewOffset;   ///< where the slide has reached; a third of the way a frame
  s32 row;          ///< the list row chosen last
  s32 page;         ///< the list page chosen last
  s32 preferredRow; ///< the row to return to
  /// The command row's choice: [0] for the party's row (fight, flee,
  /// status), [1] for a member's commands.
  s32 commandSelection[2];
  /// The window that shows the link battle's seconds left, a struct Window
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
  /// FLAG_DJINN_LESSON is set, then one more for each hint shown. While it
  /// is not 0 the menus press keys for the player.
  s32 tutorialStep;
  s32 tutorialDelay; ///< frames until the tutorial's next key press
  /// The member whose commands are being chosen; -1 between members.
  s32 commandActor;
  /// The Djinni each command has chosen, element << 8 | index, or 0x80000000.
  u32 chosenDjinn[8];
};

STATIC_ASSERT(sizeof(struct BattleMenuWork) == 0x104);
STATIC_ASSERT(offsetof(struct BattleMenuWork, targetViewOffset) == 0x28);
STATIC_ASSERT(offsetof(struct BattleMenuWork, row) == 0x30);
STATIC_ASSERT(offsetof(struct BattleMenuWork, commandSelection) == 0x3c);
STATIC_ASSERT(offsetof(struct BattleMenuWork, timerWindow) == 0x44);
STATIC_ASSERT(offsetof(struct BattleMenuWork, timer) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleMenuWork, standbyDjinn) == 0x54);
STATIC_ASSERT(offsetof(struct BattleMenuWork, tutorialStep) == 0xd8);
STATIC_ASSERT(offsetof(struct BattleMenuWork, commandActor) == 0xe0);
STATIC_ASSERT(offsetof(struct BattleMenuWork, chosenDjinn) == 0xe4);

/// Restores the previous selection: page, row and return row.
// A statement macro (4 sites): the three cursor reads stay
// together at every use; as a plain block, tbs-en differs.
#define BATTLE_MENU_GET_CURSOR(page_, row_, preferred_)                        \
  do {                                                                         \
    (page_) = ((struct BattleMenuWork *)gWindowWork[42])->page;                \
    (row_) = ((struct BattleMenuWork *)gWindowWork[42])->row;                  \
    (preferred_) = ((struct BattleMenuWork *)gWindowWork[42])->preferredRow;   \
  } while (0)

/// Saves the selection: page, row and return row.
/* FAKEMATCH(one-pass): the three stores in a once-only block; as the plain
 * stores BattleDjinnMenu_Run matches with, 46 of 770 instructions differ in
 * tbs-en (BattleItemMenu_Run). */
#define BATTLE_MENU_SET_CURSOR(page_, row_, preferred_)                        \
  do {                                                                         \
    gBattleMenuWork->page = (page_);                                           \
    gBattleMenuWork->row = (row_);                                             \
    gBattleMenuWork->preferredRow = (preferred_);                              \
  } while (0)

extern struct BattleMenuWork *gBattleMenuWork;
extern u8 gPointingHandTiles[];
extern const u8 gMenu_AnimatedCursorTiles[];

#endif
