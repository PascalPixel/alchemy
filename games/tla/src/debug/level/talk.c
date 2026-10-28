#include "types.h"
#include "input.h"
#include "adventure.h"
#include "text/msg_ids.h"

extern const u8 gData_02000744[];

/// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

u8 *Unit_Get(s32 owner);
void Unit_Calculate(s32 owner);
void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void Text_DrawMessage(s32 msg, void *window, s32 x, s32 y);
void WindowSprite_RedrawSavedRect(void *window);
void Text_DrawStringAtOffset(const u8 *text, void *window, s32 x, s32 y);
void Text_DrawStringInWindow(const u8 *text, void *window, s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 digits, void *window, s32 x,
                               s32 y);
void Window_Close(void *window, s32 mode);
void WaitFrames(s32 frames);
void Func_0200017c(s32 delta);
void Func_0200073c(s32 cue);

/// The pressed keys under a mask, for the last test of a group: the next
/// test loads gInput's address again.
static inline u32 Input_PressedFenced(u32 mask) {
  u32 keys = KEYS_PRESSED & mask;

  /* FAKEMATCH: the memory clobber makes each test load gInput's address again;
   * without it, 11 instructions of Func_020001a8 differ in tla-en. */
  __asm__ volatile("" : "+r"(keys) : : "memory");
  return keys;
}

/// The debug room's level raiser: Start or Select raises the party's levels
/// by 5 and A by 1; B closes the window and recalculates the four units.
void Func_020001a8(void) {
  u8 *record;
  void *win;
  s32 flag;
  s32 msg;

  record = Unit_Get(gAdventure.currentOwner);
  win = Window_Create(0, 0, 30, 9, 2);

  msg = (s32)MsgRaiseEveryoneS;
  Text_DrawMessage(msg, win, 0, 0);
  Text_DrawMessage(msg + 1, win, 0, 16);
  msg += 2;
  flag = 1;
  Text_DrawMessage(msg, win, 0, 32);

  for (;;) {
    if (flag != 0) {
      WindowSprite_RedrawSavedRect(win);
      Text_DrawStringAtOffset(record, win, 0, 48);
      Text_DrawStringInWindow(gData_02000744, win, 48, 48);
      flag = 0;
      Text_DrawNumberInWindow(record[15], 0, win, 72, 48);
    }

    /* FAKEMATCH: the pressed keys are read through a volatile view
     * (KEYS_PRESSED), as ☀️ declares gKeyState volatile: each test reloads them
     * (read plainly, the two first tests merge into one mask, differ 30). */
    if ((KEYS_PRESSED & 8) != 0 || Input_PressedFenced(4) != 0) {
      Func_0200017c(5);
      Func_0200073c(93);
      flag = 1;
    }

    if ((KEYS_PRESSED & 1) != 0) {
      Func_0200017c(1);
      Func_0200073c(91);
      flag = 1;
    }

    if ((KEYS_PRESSED & 2) != 0) {
      Func_0200073c(113);
      WindowSprite_RedrawSavedRect(win);
      WaitFrames(1);
      Window_Close(win, 1);

      Unit_Calculate(0);
      Unit_Calculate(1);
      Unit_Calculate(3);
      Unit_Calculate(2);
      return;
    }

    WaitFrames(1);
  }
}
