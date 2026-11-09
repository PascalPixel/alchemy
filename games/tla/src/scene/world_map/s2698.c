#include "types.h"
#include "actor.h"
#include "text/msg_ids.h"
#include "task.h"
#include "scene_tables.h"
#include "event_inline.h"

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002594(void);
void Func_02002640(s32 a, s32 b, s32 c, s32 d);
void Func_02003d44(s32 value);
void Func_02004d30(void);
void Func_02004da8(void);
void Func_02004db0(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Func_02004b78_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

static __inline__ void Func_02004d78_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_02002698(void) {
  Func_02003d44(1);
  FieldEvent_SetMessageId((s32)MsgTheyVeDone);
  Event_ShowMessage(8, 0);
  Func_02004b78_(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Event_ShowMessage(5, 0);
  Func_02004db0();
  Event_PlaySound(145);
  PaletteFade_ApplySource(0x7fff, 0);
  Func_02004d78_(0x10000, 0);
  PaletteFade_Interpolate(60);
  Task_AddFrame(Func_02002594, TASK_PASS_VBLANK);
  Func_02004b78_(0x50000, 0x50000, 0x10000);
  FieldEvent_Wait(30);
  Func_02004b78_(0x70000, 0x60000, 0x10000);
  FieldEvent_Wait(30);
  Func_02004da8();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Event_ShowMessage(9, 0);
  Func_02004db0();
  Func_02004b78_(0x70000, 0x70000, 0x10000);
  Camera_MoveWithinMapBounds_(PIXELS(1988), -1, PIXELS(7553), 1);
  FieldEffect_ScheduleRatioTransition(0x9999, 60);
  Func_02004d30();
  FieldEvent_Wait(30);
  Func_02004da8();
  Event_ShowMessage(5, 0);
  Event_ShowMessage(8, 0);
  Func_02004db0();
  Event_PlaySound(145);
  Func_02002640(0, 128, 0, 0);
  Actor_SetAnimation(5, 19);
  Actor_SetAnimation(8, 5);
  Actor_SetAnimation(9, 7);
  System_Wait(4);
  Event_PlaySound(145);
  Func_02002640(32, 128, 32, 0);
  System_Wait(4);
  Func_02002640(64, 128, 32, 0);
  System_Wait(4);
  Camera_MoveWithinMapBounds_(PIXELS(2052), -1, PIXELS(7553), 1);
  Func_02004d30();
  Func_02004da8();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Event_ShowMessage(9, 0);
  Event_ShowMessage(5, 0);
  Func_02004db0();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Func_02002640(96, 128, 32, 0);
  System_Wait(20);
  Func_02002640(0, 160, 32, 0);
  System_Wait(20);
  Func_02002640(32, 160, 32, 0);
  FieldCamera_MoveTo(PIXELS(2148), -1, PIXELS(7553), 1);
  Func_02004d30();
  Func_02004da8();
  Event_ShowMessage(8, 0);
  Event_ShowMessage(5, 0);
  Event_PlaySound(78);
  Event_PlaySound(390);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(64);
  FieldEvent_Wait(120);
}
