// differ 2: for the first Func_020057f8 the native loads object->y before it
// starts 0x580000; here the start of the constant comes first (the load waits
// on the flags pointer in r2). Otherwise as Func_0200322c, with the position
// setter forwarded and the speed limit  stored before the acceleration. Tried
// 2026-10-12: a temporary y with an empty asm barrier after its load orders
// that pair but moves mov r8, r0 above the load and lsls r6 below lsls r0
// (differ 4); the temporary alone changes nothing; a one-pass block around the
// load reorders six rows.
#include "../../../../../games/tla/src/scene/trial_road/trial_road.h"
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
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_02005970_(s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005970(a, b);
}

static __inline__ void Actor_ResetAndSetPositionInMode2_(s32 actor,
                                                                s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Actor_ResetAndSetPositionInMode2(actor, x, z);
}

static __inline__ void Func_02005960_(s32 actor, s32 value, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005960(actor, value, frames);
}

void Func_020057f8(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02005800(struct Actor *object);
void Func_02002d78(s32 actor, s32 x, s32 z, s32 angle);

static __inline__ void Func_020057f8_(struct Actor *object, s32 x,
                                      s32 y, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020057f8(object, x, y, z);
}

void Func_02002dec(void) {
  struct Actor *object;

  Func_02005880();
  Func_020059e8(0);
  Func_02005970_(0x20000, 0x4000);
  Camera_MoveWithinMapBounds_(0x1000000, -1, 0x1080000, 1);
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
  Task_Wait(20);
  Func_02005a68(138);
  Task_Wait(20);
  Func_02002d78(8, 344, 296, 0xc000);
  object = Actor_GetById(8);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->motionFlags = 0;
  Func_020057f8_(object, object->x, object->y + 0x580000, object->z);
  Func_020057c8(object, 2);
  Func_02005800(object);
  Func_020057c8(object, 1);
  object->motionFlags = 2;
  object->terrainHeight = 0x800000;
  object->velocityY = 0x40000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Func_020057f8_(object, object->x, object->y, object->z - 0x100000);
  Func_02005800(object);
  Task_Wait(12);
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  Func_020057f8_(object, Func_02002d20(41) << 16, object->y, 264 << 16);
  Func_02005800(object);
  Task_Wait(6);
  Func_02005a68(152);
  object->velocityY = 0x60000;
  object->speedLimit = 0x30000;
  object->acceleration = 0x20000;
  Func_020057f8_(object, Func_02002d20(37) << 16, object->y, 264 << 16);
  Func_02005800(object);
  Task_Wait(6);
  Func_020058f0(13, 8);
  Func_020058f0(14, 8);
  Actor_SetSpeedParameters_(8, 0x10000, 0x8000);
  Actor_SetSpeedParameters_(13, 0x10000, 0x8000);
  Actor_SetSpeedParameters_(14, 0x10000, 0x8000);
  Actor_ResetAndSetPositionInMode2_(8, Func_02002d20(36), 264);
  Actor_ResetAndSetPositionInMode2_(13, Func_02002d20(37), 248);
  Actor_ResetAndSetPositionInMode2_(14, Func_02002d20(37), 280);
  Actor_WaitForMoveById(8);
  Actor_ArmCallback(8, Func_02002d50(0x8000), 0);
  Actor_WaitForMoveById(13);
  Actor_ArmCallback(13, Func_02002d50(0x8000), 0);
  Actor_WaitForMoveById(14);
  Actor_ArmCallback(14, Func_02002d50(0x8000), 0);
  Func_02005938((s32)MsgTrialRoadSwiftChallenge);
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
