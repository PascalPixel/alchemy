#include "trial_road.h"
#include "text/msg_ids.h"

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

extern const u8 gData_02005c88[];
extern const u8 gData_02005be8[];
extern const u8 gData_02005d28[];
extern const u8 gData_02005dc8[];
extern const u8 gData_02005e40[];
extern const u8 gData_02005ed4[];

void FieldEvent_WaitFrames(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_SetMovementTypeById(s32 actor, const u8 *action);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 actor);
void Actor_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Event_StartScreenTransition(void);
void Func_02005880(void);
void Func_020059e8(s32 value);
void Func_02005970(s32 a, s32 b);
void Func_02005980(void);
void Func_02005938(s32 message);
void Func_02005940(s32 actor, s32 flags);
void Func_02005838(s32 a, s32 b, s32 c);
void Func_02005990(s32 value);

static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Func_020058e0_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020058e0(id, x, z);
}

static __inline__ void Func_02005970_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005970(a, b);
}

static __inline__ void Actor_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02005838_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005838(a, b, c);
}

static __inline__ void Actor_CommitPositionAndActivate_(s32 actor,
                                                               s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void
Actor_OffsetPositionAndResetMotion_(s32 actor, s32 dx, s32 dz) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(actor, dx, dz);
}

static __inline__ void Actor_SetPositionAndReset_(s32 actor, s32 x,
                                                         s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(actor, x, z);
}

void Func_02003900(void) {
  s32 i;

  Func_02005880();
  Func_020059e8(0);
  Func_020058e0_(gAdventure.currentOwner, 0, 0x3f80000);
  Func_020058e0_(8, 0x3f80000, 0x3f80000);
  for (i = 9; i < 80; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object->x != 0)
      object->x -= 0x900000;
  }
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Actor_SetMovementTypeById(13, gData_02005c88);
  Actor_SetMovementTypeById(14, gData_02005be8);
  Event_StartScreenTransition();
  FieldEvent_WaitFrames(20);
  Func_02005970_(0x20000, 0x4000);
  Camera_MoveWithinMapBounds_(184 << 16, -1, 600 << 16, 1);
  Func_02005980();
  Camera_MoveWithinMapBounds_(120 << 16, -1, 552 << 16, 1);
  Func_02005980();
  Camera_MoveWithinMapBounds_(120 << 16, -1, 408 << 16, 1);
  Func_02005980();
  Camera_MoveWithinMapBounds_(120 << 16, -1, 120 << 16, 1);
  Func_02005938((s32)MsgTrialRoadTrapInstructions);
  Func_02005940(8, 0);
  Func_02005980();
  Camera_MoveWithinMapBounds_(216 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Func_02005940(8, 0);
  Func_02005940(4, 0);
  Actor_SetMovementTypeById(13, gData_02005d28);
  Actor_RefreshSelectorById(13);
  Actor_SetModeById(13, 1);
  FieldEvent_WaitFrames(10);
  Actor_SetSpeedParameters_(13, 0x4000, 0x2000);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate_(13, 0, 8);
  FieldEvent_WaitFrames(1);
  /* FAKEMATCH: the bit is set through the 8-bit field, as in overlay 685: read
   * as a plain byte the store takes the order of a store that aliases what
   * follows. */
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  FieldEvent_WaitFrames(20);
  Actor_SetSpeedParameters_(13, 0x20000, 0x10000);
  Actor_SetModeById(13, 5);
  Actor_OffsetPositionAndResetMotion_(13, 0, -40);
  Actor_WaitForMoveById(13);
  Func_02005a68(103);
  Func_02005838_(0x20000, 0x40000, 0x10000);
  Actor_GetById(13)->velocityY = 0x40000;
  Func_02005838_(-1, -1, 0xe666);
  OBJECT_BYTE(Actor_GetById(13), 0x5a) &= ~1;
  Actor_CommitPositionAndActivate_(13, 0, 16);
  FieldEvent_WaitFrames(1);
  ((struct ActorSolid *)Actor_GetById(13))->solid |= 1;
  FieldEvent_WaitFrames(60);
  Actor_ArmCallback_(13, 0x4000, 0);
  FieldEvent_WaitFrames(20);
  Motion_SetModeAndWaitAnimation(13, 4);
  Func_02005940(8, 0);
  Actor_ArmCallback_(13, 0xa000, 0);
  Motion_SetModeAndWaitAnimation(13, 3);
  FieldEvent_WaitFrames(10);
  Actor_SetSpeedParameters_(13, 0x10000, 0x8000);
  Actor_CommitPositionAndActivate_(13, -16, -16);
  Actor_ArmCallback_(13, 0x8000, 0);
  Func_02005a68(124);
  Actor_SetModeById(9, 6);
  FieldEvent_WaitFrames(30);
  Func_02005940(8, 0);
  Actor_SetPositionAndReset_(13, 200, 168);
  Actor_ArmCallback_(13, 0xc000, 0);
  FieldEvent_WaitFrames(40);
  Func_02005a68(125);
  Actor_SetModeById(9, 5);
  FieldEvent_WaitFrames(20);
  Func_02005a68(138);
  Map_Stamp(127, 3, 1, 2, 48, 3);
  Map_CopyCells(0, 127, 1, 1, 14, 8);
  FieldEvent_WaitFrames(20);
  Actor_SetMovementTypeById(13, gData_02005dc8);
  Actor_RefreshSelectorById(13);
  Func_02005a68(123);
  Func_02005940(8, 0);
  Actor_SetMovementTypeById(14, gData_02005e40);
  Camera_MoveWithinMapBounds_(328 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Func_02005940(8, 0);
  Func_02005940(8, 0);
  Actor_ArmCallback_(14, 0xe000, 0);
  Motion_SetModeAndWaitAnimation(14, 3);
  FieldEvent_WaitFrames(10);
  Actor_SetSpeedParameters_(14, 0x10000, 0x8000);
  Actor_CommitPositionAndActivate_(14, 16, -16);
  Actor_ArmCallback_(14, 0, 0);
  Func_02005a68(124);
  Actor_SetModeById(11, 6);
  Actor_SetModeById(12, 6);
  FieldEvent_WaitFrames(30);
  Actor_SetPositionAndReset_(14, 350, 168);
  Actor_ArmCallback_(14, 0xc000, 0);
  FieldEvent_WaitFrames(40);
  Func_02005a68(125);
  Actor_SetModeById(11, 5);
  FieldEvent_WaitFrames(40);
  Actor_SetPositionAndReset_(14, 364, 168);
  Actor_ArmCallback_(14, 0xc000, 0);
  FieldEvent_WaitFrames(40);
  Func_02005a68(125);
  Actor_SetModeById(12, 5);
  FieldEvent_WaitFrames(20);
  Func_02005a68(138);
  Map_Stamp(127, 3, 1, 2, 53, 3);
  Map_CopyCells(0, 127, 1, 1, 19, 8);
  FieldEvent_WaitFrames(30);
  Actor_ArmCallback_(14, 0xa000, 0);
  FieldEvent_WaitFrames(10);
  Motion_SetModeAndWaitAnimation(14, 3);
  Func_02005940(8, 0);
  Camera_MoveWithinMapBounds_(408 << 16, -1, 120 << 16, 1);
  Func_02005980();
  Func_02005940(8, 0);
  Actor_SetMovementTypeById(14, gData_02005ed4);
  Actor_RefreshSelectorById(14);
  Func_02005940(8, 0);
  Func_02005990(10);
}
