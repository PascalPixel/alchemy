#include "types.h"
#include "object_runtime.h"
#include "text/msg_ids.h"

extern const u8 gData_02004e38[];
extern const u8 gData_02004e58[];
extern const u8 gData_02004e88[];

struct ObjectRuntime *Object_GetById(s32 id);
void FieldEvent_WaitFrames(s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndReset(s32 actor, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const u8 *action);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c90(s32 actor, s32 x, s32 z, s32 angle);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 actor, s32 flags);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);
void Func_02004e00(void);

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 actor, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 actor, s32 angle,
                                                 s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static __inline__ void Func_02004df8_(s32 value, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004df8(value, mode);
}

static __inline__ void Func_02004c90_(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004c90(actor, x, z, angle);
}

void Func_02001c5c(void) {
  s32 angle;

  Func_02004c20();
  Func_02004db8(0);
  Object_GetById(16)->motionFlags = 2;
  Object_GetById(17)->motionFlags = 2;
  Object_GetById(18)->motionFlags = 2;
  /* FAKEMATCH: held in a local, the facing angle is built once and shared by
   * the three placements as in the native; as a literal each forwarded call
   * builds it again. */
  angle = 0xe000;
  Func_02004c90_(16, 0x22d10000, 0x16dd0000, angle);
  Func_02004c90_(17, 0x22e10000, 0x16ed0000, angle);
  Func_02004c90_(18, 0x22e10000, 0x16fd0000, angle);
  Event_StartScreenTransition();
  ObjectMotion_SetSpeedParameters_(16, 0x3333, 0x1999);
  ObjectMotion_SetSpeedParameters_(17, 0x3333, 0x1999);
  ObjectMotion_SetSpeedParameters_(18, 0x3333, 0x1999);
  ObjectMotion_OffsetPositionAndReset(16, 10, 0);
  ObjectMotion_OffsetPositionAndReset(17, 10, 0);
  ObjectMotion_OffsetPositionAndReset(18, 10, 0);
  Event_WaitTransitionFrames();
  Func_02004df8_(0x13333, 6);
  Func_02004e00();
  Func_02004da8();
  ObjectMotion_CommitCurrentPositionAndActivate(16);
  FieldEvent_WaitFrames(40);
  Object_SetModeById(16, 3);
  Func_02004cd8((s32)MsgYouDonT);
  ObjectMotion_ArmCallback_(17, 0xd000, 0);
  ObjectMotion_ArmCallback_(18, 0xd000, 0);
  Func_02004cf0(16, 0);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_WaitFrames(30);
  Func_02004cf0(16, 0);
  Object_SetModeById(16, 3);
  Object_SetModeById(4, 3);
  FieldEvent_WaitFrames(60);
  Object_LinkObjectAndSetCallback(4, 18);
  ObjectMotion_SetSpeedParameters_(16, 0x6666, 0x3333);
  ObjectMotion_SetSpeedParameters_(17, 0x6666, 0x3333);
  ObjectMotion_SetSpeedParameters_(18, 0x6666, 0x3333);
  ObjectMotion_EnableActionAndSetCallback(16, gData_02004e38);
  FieldEvent_WaitFrames(40);
  ObjectMotion_EnableActionAndSetCallback(17, gData_02004e58);
  ObjectMotion_EnableActionAndSetCallback(18, gData_02004e88);
  Object_RefreshSelectorById(16);
  Object_RefreshSelectorById(17);
  Object_RefreshSelectorById(18);
  Func_02004db0();
  FieldEvent_WaitFrames(30);
  Func_02004df8_(0x10000, 6);
  Func_02004c28();
}
