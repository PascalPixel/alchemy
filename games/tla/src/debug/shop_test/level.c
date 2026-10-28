#include "types.h"
#include "adventure.h"
#include "owner_state.h"
#include "input.h"

extern const u8 MsgRaiseEveryoneS[];
extern const u8 gData_02000e80[];

void wait(s32 frames);
void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawMessage(const u8 *message, void *window, s32 x, s32 y);
void Text_DrawStringAtOffset(const void *text, void *window, s32 x, s32 y);
void Text_DrawStringInWindow(const void *text, void *window, s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 a, void *window, s32 x, s32 y);
void WindowSprite_RedrawSavedRect(void *window);
void Window_Close(void *window, s32 mode);
void Func_02000704(s32 levels);
void Func_02000e78(s32 cue);

/// The level-raising window: shows the current member's name and level,
/// raises every active member 5 levels on Start or Select and 1 on A, and
/// closes on B.
// ☀️'s debug EV11_TALK.
void Func_02000748(void) {
  u8 *unit = Unit_Get(gAdventure.currentOwner);
  void *window = Window_Create(0, 0, 30, 9, 2);
  const u8 *message = MsgRaiseEveryoneS;
  s32 redraw;

  Text_DrawMessage(message, window, 0, 0);
  Text_DrawMessage(message + 1, window, 0, 16);
  redraw = 1;
  Text_DrawMessage(message + 2, window, 0, 32);
  for (;;) {
    if (redraw) {
      WindowSprite_RedrawSavedRect(window);
      Text_DrawStringAtOffset(unit, window, 0, 48);
      Text_DrawStringInWindow(gData_02000e80, window, 48, 48);
      redraw = 0;
      Text_DrawNumberInWindow(unit[15], 0, window, 72, 48);
    }
    wait(1);
    /* FAKEMATCH: the volatile reads load the pressed keys again for each test,
     * as the native does (plain: differ 30). */
    if ((((volatile struct InputState *)&gInput)->pressed & 8) ||
        (((volatile struct InputState *)&gInput)->pressed & 4)) {
      Func_02000704(5);
      Func_02000e78(93);
      redraw = 1;
    }
    /* FAKEMATCH: the memory barrier keeps GCSE's PRE from reusing the Start and
     * Select tests' input address here, so the A test loads it again as the
     * native does (differ 17 without). */
    __asm__ volatile("" : : : "memory");
    if (((volatile struct InputState *)&gInput)->pressed & 1) {
      Func_02000704(1);
      Func_02000e78(91);
      redraw = 1;
    }
    if (((volatile struct InputState *)&gInput)->pressed & 2)
      break;
  }
  Func_02000e78(113);
  Window_Close(window, 2);
}
