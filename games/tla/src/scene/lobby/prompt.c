#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020019e4(s32 value, s32 digits);
void Func_02001a14(void);
void Func_02001a1c(void);
s32 Window_Create();
void WindowSprite_PrepareForRedraw();
void Text_DrawNumber();
s32 DebugParty_LoadPreset();
void MessageArgument_ClearAll(void);

s32 Func_02000e4c(s32 id) {
  s32 window;
  s32 choice;
  s32 shown;

  Func_02001a14();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(id, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgChooseYourParameters);
  Event_ShowMessageAndWait(id, 0);
  window = Window_Create(0, 0, 6, 4, 2);
  choice = 0;
  shown = -1;
  for (;;) {
    if (choice != shown) {
      WindowSprite_PrepareForRedraw(window);
      Text_DrawNumber(choice, 3, window, 0, 0);
      shown = choice;
    }
    if (gInput.repeat & 0x20) {
      choice--;
    }
    if (gInput.repeat & 0x10) {
      choice++;
    }
    if (choice < 0) {
      choice = 0;
    }
    if (((struct InputState *)&gInput)->pressed & 1) {
      break;
    }
    if (((volatile struct InputState *)&gInput)->pressed & 2) {
      choice = -1;
      break;
    }
    wait(1);
  }
  Window_Close(window, 1);
  {
    s32 ok;
    s32 message;

    if (choice >= 0) {
      ok = DebugParty_LoadPreset(choice);
    } else {
      message = (s32)MsgWhatIsThis;
      goto show;
    }
    // The refusal jumps into the unknown-preset branch, so the two share one
    // message call.
    if (ok != 0) {
      message = (s32)MsgThereSNoSuch;
    show:
      Event_SetMessageId(message);
      Event_ShowMessageAndWait(9, 0);
    } else {
      Event_SetMessageId((s32)MsgTheSettingsAre);
      Event_ShowMessageAndWait(9, 0);
    }
  }
  wait(10);
  Func_02001a1c();
}
