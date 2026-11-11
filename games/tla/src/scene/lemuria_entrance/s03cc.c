#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

extern u8 MsgFieldAddressCompanion;
extern const u8 gData_020005e4[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200055c(void);
void Func_02000564(void);
void Func_02000310(s32 kind, s32 frames);

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                 s32 acceleration) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, so the id is
   * set between the speed's last add and the call; as a direct call to
   * Actor_SetSpeedParameters, 2 of 113 instructions differ in tla-en. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

void Func_020003cc(void) {
  struct EventWork *work;
  u8 saved;

  saved = gMainRoutineWork.eventMode;
  gMainRoutineWork.eventMode = 2;
  Func_0200055c();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeedParameters_(8, 0x451e, 0x228f);
  Actor_SetScript(8, gData_020005e4);
  FieldEvent_Wait(80);
  FieldEvent_SetMessageId((s32)&MsgFieldAddressCompanion);
  Func_02000310(6, 90);
  Func_02000310(7, 90);
  Func_02000310(6, 180);
  Func_02000310(7, 90);
  Func_02000310(6, 250);
  Func_02000310(7, 180);
  Func_02000310(5, 250);
  Func_02000310(8, 90);
  Func_02000310(7, 180);
  Func_02000310(8, 180);
  Func_02000310(7, 180);
  Func_02000310(6, 90);
  Func_02000310(7, 250);
  Func_02000310(5, 250);
  Func_02000310(7, 250);
  Func_02000310(6, 90);
  Func_02000310(5, 90);
  Func_02000310(8, 90);
  Func_02000310(7, 250);
  Func_02000310(4, 90);
  Func_02000310(7, 250);
  FieldActor_RefreshSelectorById(8);
  FieldEvent_Wait(10);
  work = Ram_HeapSlots->eventWork;
  work->delay = 30;
  work->screenEffect = 0x204;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(1);
  gMainRoutineWork.eventMode = saved;
  Func_02000564();
}
