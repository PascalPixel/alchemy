#include "trial_road.h"
#include "text/msg_ids.h"

void FieldEvent_WaitFrames(s32 frames);
void Task_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_OffsetPositionAndReset(s32 actor, s32 dx, s32 dz);
void Func_02005880(void);
void Func_020059e8(s32 value);
void Func_02005970(s32 a, s32 b);
void Func_020058f0(s32 actor, s32 value);
s32 Func_02002d20(s32 value);
s32 Func_02002d50(s32 value);
s32 Func_02002d38(s32 value);
void Func_02005980(void);
void Func_02005938(s32 message);
s32 Func_02005960(s32 actor, s32 value, s32 frames);
void Func_02005940(s32 actor, s32 flags);

static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_02005970_(s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005970(a, b);
}

static __inline__ void ObjectMotion_ResetAndSetPositionInMode2_(s32 actor,
                                                                s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_ResetAndSetPositionInMode2(actor, x, z);
}

static __inline__ void Func_02005960_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005960(actor, value, frames);
}

void Func_0200322c(void) {
  Func_02005880();
  Func_020059e8(0);
  Func_02005970_(0x20000, 0x4000);
  Camera_MoveWithinMapBounds_(0x1000000, -1, 0x1080000, 1);
  Func_020058f0(5, 4);
  Func_020058f0(6, 4);
  Func_020058f0(7, 4);
  ObjectMotion_SetSpeedParameters_(4, 0x10000, 0x8000);
  ObjectMotion_SetSpeedParameters_(5, 0x10000, 0x8000);
  ObjectMotion_SetSpeedParameters_(6, 0x10000, 0x8000);
  ObjectMotion_SetSpeedParameters_(7, 0x10000, 0x8000);
  ObjectMotion_ResetAndSetPositionInMode2_(4, Func_02002d20(28), 272);
  ObjectMotion_ResetAndSetPositionInMode2_(7, Func_02002d20(28), 256);
  ObjectMotion_ResetAndSetPositionInMode2_(5, Func_02002d20(27), 248);
  ObjectMotion_ResetAndSetPositionInMode2_(6, Func_02002d20(27), 280);
  ObjectMotion_CommitCurrentPositionAndActivate(4);
  ObjectMotion_ArmCallback(4, Func_02002d50(0), 0);
  ObjectMotion_CommitCurrentPositionAndActivate(7);
  ObjectMotion_ArmCallback(7, Func_02002d50(0), 0);
  ObjectMotion_CommitCurrentPositionAndActivate(5);
  ObjectMotion_ArmCallback(5, Func_02002d50(0), 0);
  ObjectMotion_CommitCurrentPositionAndActivate(6);
  ObjectMotion_ArmCallback(6, Func_02002d50(0), 0);
  Func_02005980();
  Func_02005938((s32)MsgTrialRoadSlowArrival);
  Func_02005960_(8, 256, 10);
  Func_02005940(8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_02005940(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Func_02005940(8, 0);
  Object_SetModeById(4, 3);
  Object_SetModeById(5, 3);
  Object_SetModeById(6, 3);
  Object_SetModeById(7, 3);
  FieldEvent_WaitFrames(15);
  ObjectMotion_SetSpeedParameters_(4, 0x20000, 0x10000);
  ObjectMotion_SetSpeedParameters_(5, 0x1e666, 0xf333);
  ObjectMotion_SetSpeedParameters_(6, 0x1b333, 0xd999);
  ObjectMotion_SetSpeedParameters_(7, 0x1cccc, 0xe666);
  ObjectMotion_SetSpeedParameters_(8, 0x20000, 0x10000);
  ObjectMotion_SetSpeedParameters_(13, 0x1e666, 0xf333);
  ObjectMotion_SetSpeedParameters_(14, 0x1b333, 0xd999);
  ObjectMotion_OffsetPositionAndReset(4, Func_02002d38(3), 0);
  ObjectMotion_OffsetPositionAndReset(8, Func_02002d38(-3), 0);
  Task_Wait(1);
  ObjectMotion_OffsetPositionAndReset(5, Func_02002d38(3), 0);
  ObjectMotion_OffsetPositionAndReset(13, Func_02002d38(-3), 0);
  Task_Wait(1);
  ObjectMotion_OffsetPositionAndReset(7, Func_02002d38(3), 0);
  ObjectMotion_OffsetPositionAndReset(14, Func_02002d38(-3), 0);
  Task_Wait(1);
  ObjectMotion_OffsetPositionAndReset(6, Func_02002d38(3), 0);
  Task_Wait(1);
  FieldEvent_WaitFrames(6);
}
