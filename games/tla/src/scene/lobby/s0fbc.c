#include "types.h"
#include "input.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u8 MsgYouHaveA[];
extern u8 MsgComeBackWhen[];
extern u16 gData_02001b02[];
extern s16 gData_02003860;

void Func_02001a14(void);
void Func_02001a1c(void);
s32 Func_02001adc(s32 song);
s32 Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void WindowSprite_PrepareForRedraw(s32 window);
void Text_DrawNumber(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Window_Close(s32 window, s32 release);

void Func_02000fbc(s32 actor) {
  s32 window;
  s32 song;
  s32 shown;
  s32 step;
  s32 all;

  Func_02001a14();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgYouHaveA);
  Event_ShowMessageAndWait(actor, 0);
  song = 0;
  all = 0;
  if ((gInput.held & 0x248) == 0x248)
    all = 1;
  Sound_Play(78);
  for (;;) {
    window = Window_Create(12, 8, 6, 3, 2);
    shown = -1;
    for (;;) {
      if (song != shown) {
        WindowSprite_PrepareForRedraw(window);
        Text_DrawNumber(song, 3, window, 0, 0);
        shown = song;
      }
      step = 0;
      if (gInput.repeat & 32)
        step--;
      if (gInput.repeat & 16)
        step = 1;
      while (step != 0) {
        song += step;
        if (song < 0)
          song = 96;
        if ((u32)song > 96)
          song = 0;
        if (all != 0 || gMainRoutineWork.unk22d[2] != 0 || gData_02003860 != 0)
          break;
        if (Func_02001adc(gData_02001b02[song]) != 0)
          goto keys;
      }
    keys:
      if (gInput.pressed & 1)
        break;
      if (gInput.pressed & 2) {
        song = -1;
        break;
      }
      wait(1);
    }
    Window_Close(window, 1);
    if (song < 0)
      break;
    Sound_Play(gData_02001b02[song]);
  }
  Sound_Play(78);
  wait(120);
  Sound_Play(0x2c5);
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 153 instructions differ
   * in tla-en. */
  do {
    Event_SetMessageId((s32)MsgComeBackWhen);
  } while (0);
  Event_ShowMessageAndWait(actor, 0);
  wait(10);
  Func_02001a1c();
}
