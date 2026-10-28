#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "event_work.h"

extern u8 MsgFieldAddressCompanion;
extern const u8 gData_020005e4[];

void FieldEvent_Wait(s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_RefreshSelectorById(u32 id);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200055c(void);
void Func_02000564(void);
void Func_0200059c(s32 message);
void Func_020005ac(s32 countdown);
void Func_020005cc(s32 skip);
void Func_02000310(s32 kind, s32 frames);

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline sets the id between the speed's
   * last add and the call, as the native does (differ 2 called directly). */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

void Func_020003cc(void) {
  struct EventWork *work;
  u8 saved;

  saved = gSequenceParentWork.eventMode;
  gSequenceParentWork.eventMode = 2;
  Func_0200055c();
  Func_020005cc(0);
  Actor_SetSpeedParameters_(8, 0x451e, 0x228f);
  Actor_SetMovementTypeById(8, gData_020005e4);
  FieldEvent_Wait(80);
  Func_0200059c((s32)&MsgFieldAddressCompanion);
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
  Actor_RefreshSelectorById(8);
  FieldEvent_Wait(10);
  work = Ram_HeapSlots->eventWork;
  work->delay = 30;
  work->screenEffect = 0x204;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020005ac(1);
  gSequenceParentWork.eventMode = saved;
  Func_02000564();
}
