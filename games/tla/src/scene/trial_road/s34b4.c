#include "trial_road.h"
#include "text/msg_ids.h"

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

void FieldEvent_WaitFrames(s32 frames);
void Task_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void ObjectMotion_SetVariantCallback(s32 actor, s32 variant);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Object_LinkPair(s32 a, s32 b, s32 c);
s32 PartyInventory_GiveItem(s32 item, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005880(void);
void Func_020059e8(s32 value);
void Func_02002d78(s32 actor, s32 x, s32 z, s32 angle);
s32 Func_02002d20(s32 value);
s32 Func_02002d50(s32 value);
void Func_02005938(s32 message);
s32 Func_02005960(s32 actor, s32 value, s32 frames);
void Func_02005940(s32 actor, s32 flags);
void Func_02005950(s32 actor, s32 angle);
void Func_02005860(s32 cue);

static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

void Func_020034b4(void) {
  Func_02005880();
  Func_020059e8(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Func_02002d78(4, 248, 272, 0);
  Func_02002d78(7, 224, 248, 0);
  Func_02002d78(5, 232, 264, 0);
  Func_02002d78(6, 224, 280, 0);
  Func_02002d78(9, 248, 256, 0);
  Func_02002d78(8, 272, 264, 0x4000);
  Func_02002d78(13, 288, 248, 0x4000);
  Func_02002d78(14, 288, 280, 0x4000);
  Object_SetModeById(8, 6);
  Object_SetModeById(13, 6);
  Object_SetModeById(14, 6);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(30);
  Func_02005938((s32)MsgTrialRoadHeroesProven);
  ObjectMotion_SetVariantCallback(13, 2);
  Task_Wait(4);
  ObjectMotion_SetVariantCallback(14, 2);
  Task_Wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Func_02005940(8, 0);
  Object_SetModeById(8, 7);
  FieldEvent_WaitFrames(15);
  Object_SetModeById(13, 7);
  FieldEvent_WaitFrames(10);
  Object_SetModeById(14, 7);
  FieldEvent_WaitFrames(5);
  Object_SetModeById(8, 1);
  FieldEvent_WaitFrames(15);
  ObjectMotion_ArmCallback(8, Func_02002d50(0x8000), 0);
  Object_SetModeById(13, 1);
  FieldEvent_WaitFrames(10);
  ObjectMotion_ArmCallback(13, Func_02002d50(0x8000), 0);
  Object_SetModeById(14, 1);
  FieldEvent_WaitFrames(5);
  ObjectMotion_ArmCallback(14, Func_02002d50(0x8000), 0);
  FieldEvent_WaitFrames(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_02005940(8, 0);
  ObjectMotion_SetSpeedParameters(8, 0x10000, 0x8000);
  ObjectMotion_ResetAndSetPositionInMode2(8, Func_02002d20(32), 264);
  Object_LinkPair(9, 7, 0);
  Object_LinkPair(5, 6, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(8);
  ObjectMotion_SetAngleToward(9, 4, 0);
  ObjectMotion_SetAngleToward(7, 4, 0);
  ObjectMotion_SetAngleToward(6, 4, 0);
  ObjectMotion_SetAngleToward(5, 4, 0);
  FieldEvent_WaitFrames(20);
  Func_02005860(65);
  PartyInventory_GiveItem(215, 0);
  ObjectMotion_SetAngleToward(4, 8, 0);
  OBJECT_BYTE(Object_GetById(8), 0x5a) &= ~1;
  ObjectMotion_SetPositionAndReset(8, Func_02002d20(35), 264);
  FieldEvent_WaitFrames(1);
  /* FAKEMATCH: the bit is set through the 8-bit field, as in overlay 685: read
   * as a plain byte the store takes the order of a store that aliases what
   * follows. */
  ((struct ActorSolid *)Object_GetById(8))->solid |= 1;
  FieldEvent_WaitFrames(20);
  Func_02005960(8, 261, 10);
  Func_02005940(8, 0);
  Func_02005950(8, Func_02002d50(0));
  ObjectMotion_SetAngleToward(13, 8, 0);
  ObjectMotion_SetAngleToward(14, 8, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Func_02005940(8, 0);
  Object_SetModeById(13, 3);
  Task_Wait(3);
  Motion_SetModeAndWaitAnimation(14, 3);
  ObjectMotion_SetAngleToward(4, 9, 0);
  ObjectMotion_SetAngleToward(7, 9, 0);
  ObjectMotion_SetAngleToward(5, 9, 0);
  ObjectMotion_SetAngleToward(6, 9, 0);
  Func_02005940(9, 0);
  Object_SetModeById(4, 3);
  Task_Wait(2);
  Object_SetModeById(6, 3);
  Object_SetModeById(7, 3);
  Task_Wait(1);
  Object_SetModeById(5, 3);
  Task_Wait(30);
}
