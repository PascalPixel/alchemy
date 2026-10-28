#include "trial_road.h"
#include "text/msg_ids.h"

void FieldEvent_Wait(s32 frames);
void Task_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_OffsetPositionAndReset(s32 actor, s32 dx, s32 dz);
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
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_02005970_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005970(a, b);
}

static __inline__ void Actor_ResetAndSetPositionInMode2_(s32 actor,
                                                                s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ResetAndSetPositionInMode2(actor, x, z);
}

static __inline__ void Func_02005960_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005960(actor, value, frames);
}

void Func_0200322c(void) {
  Func_02005880();
  Func_020059e8(0);
  Func_02005970_(0x20000, 0x4000);
  Camera_MoveWithinMapBounds_(PIXELS(256), -1, PIXELS(264), 1);
  Func_020058f0(5, 4);
  Func_020058f0(6, 4);
  Func_020058f0(7, 4);
  Actor_SetSpeedParameters_(4, 0x10000, 0x8000);
  Actor_SetSpeedParameters_(5, 0x10000, 0x8000);
  Actor_SetSpeedParameters_(6, 0x10000, 0x8000);
  Actor_SetSpeedParameters_(7, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2_(4, Func_02002d20(28), 272);
  Actor_ResetAndSetPositionInMode2_(7, Func_02002d20(28), 256);
  Actor_ResetAndSetPositionInMode2_(5, Func_02002d20(27), 248);
  Actor_ResetAndSetPositionInMode2_(6, Func_02002d20(27), 280);
  Actor_WaitForMoveById(4);
  Actor_ArmCallback(4, Func_02002d50(0), 0);
  Actor_WaitForMoveById(7);
  Actor_ArmCallback(7, Func_02002d50(0), 0);
  Actor_WaitForMoveById(5);
  Actor_ArmCallback(5, Func_02002d50(0), 0);
  Actor_WaitForMoveById(6);
  Actor_ArmCallback(6, Func_02002d50(0), 0);
  Func_02005980();
  Func_02005938((s32)MsgTrialRoadSlowArrival);
  Func_02005960_(8, 256, 10);
  Func_02005940(8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_02005940(8, 0);
  Motion_SetVarCbAndRefresh(8, 2);
  Func_02005940(8, 0);
  Actor_SetModeById(4, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(7, 3);
  FieldEvent_Wait(15);
  Actor_SetSpeedParameters_(4, 0x20000, 0x10000);
  Actor_SetSpeedParameters_(5, 0x1e666, 0xf333);
  Actor_SetSpeedParameters_(6, 0x1b333, 0xd999);
  Actor_SetSpeedParameters_(7, 0x1cccc, 0xe666);
  Actor_SetSpeedParameters_(8, 0x20000, 0x10000);
  Actor_SetSpeedParameters_(13, 0x1e666, 0xf333);
  Actor_SetSpeedParameters_(14, 0x1b333, 0xd999);
  Actor_OffsetPositionAndReset(4, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(8, Func_02002d38(-3), 0);
  Task_Wait(1);
  Actor_OffsetPositionAndReset(5, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(13, Func_02002d38(-3), 0);
  Task_Wait(1);
  Actor_OffsetPositionAndReset(7, Func_02002d38(3), 0);
  Actor_OffsetPositionAndReset(14, Func_02002d38(-3), 0);
  Task_Wait(1);
  Actor_OffsetPositionAndReset(6, Func_02002d38(3), 0);
  Task_Wait(1);
  FieldEvent_Wait(6);
}
