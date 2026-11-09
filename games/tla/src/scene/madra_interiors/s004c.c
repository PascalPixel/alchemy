#include "madra_interiors.h"
#include "version.h"

const u8 *Func_0200004c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2)
    return gData_02002828;
  if (gMainRoutineWork.entrance == 20)
    return gData_02002750;
  if (GameFlag_Test(0x8ff))
    return gData_02002480;
  return gData_02002240;
}

void Func_020000a4(void) {
  s32 msg;

  Func_02001d50();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgMadraInteriors06;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(9, 0);
  Func_02001d58();
}

void Func_020000fc(void) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001f08(1, 22);
  } else {
#if !TLA_PATCH_1
    // Before patch 1 there is no choice prompt.
    if (GameFlag_Test(0x8ff))
      Event_SetMessageId((s32)MsgMadraInteriors155);
    else if (GameFlag_Test(0x8ab))
      Event_SetMessageId((s32)MsgMadraInteriors87);
    else
      Event_SetMessageId((s32)MsgMadraInteriors39);
    Event_ShowMessageAndWait(22, 0);
#else
    if (GameFlag_Test(0x8ff)) {
      s32 msg = (s32)MsgMadraInteriors155;

      Event_SetMessageId(msg);
      FieldMessage_ShowAtActor(22, 0);
      if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
        Event_Wait(10);
        Event_SetMessageId(msg + 1);
      } else {
        Event_Wait(20);
        Event_SetMessageId(msg + 2);
      }
      Event_ShowMessageAndWait(22, 0);
    } else {
      if (GameFlag_Test(0x8ab))
        Event_SetMessageId((s32)MsgMadraInteriors87);
      else
        Event_SetMessageId((s32)MsgMadraInteriors39);
      Event_ShowMessageAndWait(22, 0);
    }
#endif
  }
}
