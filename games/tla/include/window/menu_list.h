#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

/// @file
/// The battle menus' shared work, and the sprite shapes the menus queue.

#include "types.h"

struct Window;

/// A menu cursor's sprite: its render slot link and OAM attributes 0 to 2.
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
  u16 affineParam; ///< OAM's fourth halfword, the affine parameter slot
};

/// A point on the screen, in pixels.
struct MenuPoint {
  s32 x;
  s32 y;
};

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

/// The battle menus' work (HEAP_SLOT_BATTLE_MENU): what the command row and
/// the lists remember between them while the party's commands are chosen.
struct BattleMenuWork {
  /// The sprites BattleMenu_UpdateEachFrame draws each frame: the chosen
  /// command's icon, the chosen ability's icon and the party member's portrait.
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
  /// status), [1] for a party member's commands.
  s32 commandSelection[2];
  /// The window that shows the link battle's seconds left, or NULL.
  void *timerWindow;
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
  /// The Djinn tutorial's step: 0 off, 1 from the start of each turn while
  /// FLAG_DJINN_LESSON is set, then one more for each hint shown.
  s32 tutorialStep;
  s32 tutorialDelay; ///< frames until the tutorial's next key press
  /// The party member whose commands are being chosen; -1 between members.
  s32 commandActor;
  /// The Djinni each command has chosen, element << 8 | index, or
  /// BATTLE_MENU_NO_DJINNI.
  u32 chosenDjinn[8];
  s32 swappedUnit;  ///< the unit a reserve swap replaced, -1 for none
  s32 swappedPlace; ///< its party place
  /// The target BattleTarget_RunSelection chose last, where it starts again.
  s32 lastTarget;
};

/// BattleMenuWork.chosenDjinn of a command that has chosen no Djinni.
#define BATTLE_MENU_NO_DJINNI 0x80000000

/// The party status window and the rectangle it last drew.
struct PartyStatusWindow {
  struct Window *window;
  u16 x;
  u16 y;
  u16 width;
  u16 height;
  u16 flags;
};

/// The rows a battle list window shows.
enum { BATTLE_LIST_ROWS = 5 };

/// Gives a menu sprite its shape and clears the rest of its attributes, its
/// tile and place still to come. The battle lists, the status screen and the
/// command row start their sprites so.
#define MenuSprite_Initialize(sprite, shape)                                   \
  do {                                                                         \
    (sprite).oam.word.attr01 = (shape);                                        \
    (sprite).oam.word.attr23 = 0;                                              \
  } while (0)

/// The battle menus' shared work, in its heap slot.
#define BATTLE_MENU_WORK                                                       \
  ((struct BattleMenuWork *)Ram_HeapSlots->battleMenuWork)

/// Restores the previous selection: page, row and return row.
#define BATTLE_MENU_GET_CURSOR(page_, row_, preferred_)                        \
  do {                                                                         \
    (page_) = BATTLE_MENU_WORK->page;                                          \
    (row_) = BATTLE_MENU_WORK->row;                                            \
    (preferred_) = BATTLE_MENU_WORK->preferredRow;                             \
  } while (0)

/// Saves the selection: page, row and return row.
#define BATTLE_MENU_SET_CURSOR(page_, row_, preferred_)                        \
  do {                                                                         \
    BATTLE_MENU_WORK->page = (page_);                                          \
    BATTLE_MENU_WORK->row = (row_);                                            \
    BATTLE_MENU_WORK->preferredRow = (preferred_);                             \
  } while (0)

/// Marks the 4-row band of the window tilemap that holds `row` for upload
/// (WindowWork.dirty): the battle lists mark their top border's
/// row so, and the Djinn list its preview's.
#define WINDOW_MARK_TILEMAP_ROW(work, row)                                     \
  do {                                                                         \
    (work)->dirty |= 2 << ((row) >> 2);                                        \
  } while (0)

#endif
