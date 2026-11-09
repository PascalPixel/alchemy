#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

extern s32 gData_02005610[];

void Func_02000f00(void);
void Func_02000fac(void);
void Func_02004e5c(void);
void Func_02004e64(void);

/// Talking to object 10 while its byte at 0x60 is at most 2: once flag 0x9ac is
/// set it says one of two lines, by whether any of flags 0x9bc to 0x9bf is set;
/// before that it turns to the leader and speaks.
void Func_020011c8(void) {
  if (*((u8 *)Actor_Get(10) + 96) > 2)
    return;
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  if (Event_GetFlag(0x9ac)) {
    if (Event_GetFlag(0x9bc) || Event_GetFlag(0x9bd) || Event_GetFlag(0x9be) ||
        Event_GetFlag(0x9bf))
      FieldEvent_SetMessageId((s32)MsgGaiaRockInside10);
    else
      FieldEvent_SetMessageId((s32)MsgGaiaRockInside06);
    Event_ShowMessage(10, 0);
  } else {
    if (Actor_Get(9)->script == gData_02005610)
      FieldActor_RefreshSelectorById(9);
    Func_02000f00();
    FieldEvent_SetMessageId((s32)MsgGaiaRockInside03);
    Actor_FaceActor(10, gMainRoutineWork.currentOwner, 0);
    FieldEvent_Wait(10);
    Event_ShowMessage(10, 0);
    Actor_SetAnimationAndWait(10, 3);
    FieldEvent_Wait(10);
    Func_02000fac();
  }
  Func_02004e64();
}
