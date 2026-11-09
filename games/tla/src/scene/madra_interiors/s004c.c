#include "madra_interiors.h"
#include "version.h"

const u8 *Func_0200004c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov659Room2)
    return gData_02002828;
  if (gMainRoutineWork.entrance == 20)
    return gData_02002750;
  if (Event_GetFlag(0x8ff))
    return gData_02002480;
  return gData_02002240;
}

void Func_020000a4(void) {
  s32 msg;

  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  msg = (s32)MsgMadraInteriors06;
  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(9, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(9, 0);
  Func_02001d58();
}

void Func_020000fc(void) {
  u16 facing = Actor_Get(gMainRoutineWork.currentOwner)->angle;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Func_02001f08(1, 22);
  } else {
#if !TLA_PATCH_1
    // Before patch 1 there is no choice prompt.
    if (Event_GetFlag(0x8ff))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors155);
    else if (Event_GetFlag(0x8ab))
      FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
    else
      FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
    Event_ShowMessage(22, 0);
#else
    if (Event_GetFlag(0x8ff)) {
      s32 msg = (s32)MsgMadraInteriors155;

      FieldEvent_SetMessageId(msg);
      Event_OpenMessage(22, 0);
      if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
        FieldEvent_Wait(10);
        FieldEvent_SetMessageId(msg + 1);
      } else {
        FieldEvent_Wait(20);
        FieldEvent_SetMessageId(msg + 2);
      }
      Event_ShowMessage(22, 0);
    } else {
      if (Event_GetFlag(0x8ab))
        FieldEvent_SetMessageId((s32)MsgMadraInteriors87);
      else
        FieldEvent_SetMessageId((s32)MsgMadraInteriors39);
      Event_ShowMessage(22, 0);
    }
#endif
  }
}
