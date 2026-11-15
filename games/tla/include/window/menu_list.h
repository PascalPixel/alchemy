#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

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
  u16 unk;
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

/// The battle menu work (heap slot 57), The Broken Seal's BattleMenuWork.
struct BattleMenuWork {
  /// The sprites UpdateLinkSessionCountdown draws each frame: the chosen
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

#endif
