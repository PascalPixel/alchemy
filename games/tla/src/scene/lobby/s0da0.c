#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void MessageArgument_ClearAll(void);
void Func_020019e4(s32 value, s32 digits);
void Func_02001a14(void);

// Tells the leader's win count, total or in a row by the way the lead faces.
s32 Func_02000da0(s32 actor) {
  u32 facing = Actor_GetById(4)->angle;
  u16 *count;
  s32 base;

  Func_02001a14();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    base = (s32)MsgItLooksLikeThisIs;
    count = &gMainRoutineWork.linkTallies[4];
    if (*count != 0) {
      MessageArgument_ClearAll();
      Func_020019e4(*count, 5);
      Event_SetMessageId(base + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgIWillTell);
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    base = (s32)MsgItLooksLikeThisIsYour;
    count = &gMainRoutineWork.linkTallies[7];
    if (*count != 0) {
      MessageArgument_ClearAll();
      Func_020019e4(*count, 5);
      Event_SetMessageId(base + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgIWillTellYou);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}
