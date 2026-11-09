#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02000044(void);
s32 Func_0200006c(s32 slot);
void LinkLobby_WriteSlotValue(s32 slot);
void Func_02001a14(void);
void Func_02001a1c(void);

/// The lobby clerk: offers the link battle, or reports on the one waiting.
s32 Func_02000bb0(void) {
  s32 answer;
  s32 msg;
  s32 line;

  msg = (s32)MsgPleaseSpeakTo;
  Func_02001a14();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  if (Func_0200006c(0) == 0)
    wait(1);
  if (Func_0200006c(0) == 0) {
    LinkLobby_WriteSlotValue(5);
    Func_02000044();
    if (!Event_GetFlag(0x173)) {
      Event_SetMessageId(msg + 5);
      FieldMessage_ShowAtActor(8, 0);
      answer = Event_AskYesNoWithActorAnimation(4, 0);
      if (answer == 0) {
        GameFlag_SetByte(1000, 0);
        Event_SetFlag(0x173);
        Event_ClearFlag(0x172);
        Event_ClearFlag(0x16c);
        Event_SetFlag(0x202);
        line = msg + 7;
        gMainRoutineWork.linkTallies[3] = answer;
        goto message;
      }
      Event_ClearFlag(0x173);
      Event_SetFlag(0x16c);
      LinkLobby_WriteSlotValue(0);
      line = msg + 6;
      goto message;
    } else {
      line = msg + 3;
      goto message;
    }
  } else {
    if (Event_GetFlag(0x173)) {
      LinkLobby_WriteSlotValue(0);
      Event_SetMessageId((s32)MsgYourOpponentHas);
      FieldMessage_ShowAtActor(8, 0);
      Event_ClearFlag(0x202);
      Event_ClearFlag(0x173);
    }
    if (Event_GetFlag(0x202)) {
      line = msg + 3;
      // The first branch's answers jump into this block's message tail, and an
      // accepted challenge back into its opening: the native keeps one copy of
      // each, as ☀️'s lobby does.
    message:
      Event_SetMessageId(line);
    open_message:
      FieldMessage_ShowAtActor(8, 0);
    } else if (!Event_GetFlag(0x201) && !Event_GetFlag(0x300)) {
      Event_SetMessageId(msg);
      FieldMessage_ShowAtActor(8, 0);
      Event_SetFlag(0x300);
    } else {
      Event_SetFlag(0x300);
      if (Event_GetFlag(0x201))
        Event_SetMessageId(msg + 2);
      else
        Event_SetMessageId(msg + 1);
      FieldMessage_ShowAtActor(8, 0);
      if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
        if (Func_0200006c(0)) {
          Event_SetFlag(0x16c);
          Event_SetFlag(0x172);
          if (Event_GetFlag(0x201))
            Event_SetMessageId(msg + 3);
          else
            Event_SetMessageId(msg + 4);
          LinkLobby_WriteSlotValue(1);
          Event_SetFlag(0x202);
          goto open_message;
        } else {
          Event_SetFlag(0x205);
        }
      } else {
        Event_SetMessageId(msg);
        FieldMessage_ShowAtActor(8, 0);
      }
    }
  }
  Func_02001a1c();
}
