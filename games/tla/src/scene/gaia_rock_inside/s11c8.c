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
  if (*((u8 *)Actor_GetById(10) + 96) > 2)
    return;
  Func_02004e5c();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x9ac)) {
    if (GameFlag_Test(0x9bc) || GameFlag_Test(0x9bd) || GameFlag_Test(0x9be) ||
        GameFlag_Test(0x9bf))
      Event_SetMessageId((s32)MsgGaiaRockInside10);
    else
      Event_SetMessageId((s32)MsgGaiaRockInside06);
    Event_ShowMessageAndWait(10, 0);
  } else {
    if (Actor_GetById(9)->script == gData_02005610)
      Actor_RefreshSelectorById(9);
    Func_02000f00();
    Event_SetMessageId((s32)MsgGaiaRockInside03);
    Actor_SetAngleToward(10, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
    Event_ShowMessageAndWait(10, 0);
    Motion_SetModeAndWaitAnimation(10, 3);
    Event_Wait(10);
    Func_02000fac();
  }
  Func_02004e64();
}
