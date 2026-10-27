#ifndef SUHALLA_GATE_LOCAL_H
#define SUHALLA_GATE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);
void FieldEvent_WaitTicks(s32 ticks);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void Object_SetActionCallback(struct ObjectRuntime *object, s32 a);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void Func_02000800(void);
void Func_02000970(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000978(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020009d0(s32 id, s32 x, s32 z);
void Func_02000a38(s32 a);
void Func_02000a48(s32 a, s32 b);
void Func_02000a90(s32 id, s32 a);
void Func_02000a98(s32 id, s32 a, s32 b);
void Func_02000aa0(s32 a);
void Func_02000988(void);
void Func_02000990(void);
void Func_02000a00(s32 message);
void Func_02000a08(s32 id, s32 a, s32 b);
void Func_02000a18(s32 id, s32 a, s32 b);
void Func_02000a30(void);
void Func_02000a70(s32 a);
void Func_02000a78(s32 id, s32 x, s32 y, s32 z);
void Func_020005e8(void);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Camera_FollowActor(s32 id, s32 a);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
extern u8 MsgMyThatWas;
extern u8 MsgNotThatWay;

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void WaitTicks_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  WaitTicks(frames);
}

static __inline__ void FieldEvent_WaitTicks_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_WaitTicks(frames);
}

static __inline__ struct ObjectRuntime *Object_GetById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Object_GetById(id);
}

static __inline__ void Object_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Object_SetModeById(id, mode);
}

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_CenterOnColumn(id, column, offset);
}

static __inline__ void Func_02000978_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000978(a, b, c, d, e, f);
}

static __inline__ void Func_020009d0_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020009d0(id, x, z);
}

static __inline__ void ObjectMotion_SetAngleToward_(s32 id, s32 target,
                                                    s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetAngleToward(id, target, flags);
}

static __inline__ void ObjectMotion_Launch_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_Launch(id, a, b);
}

static __inline__ void ObjectMotion_SetPositionAndCommit_(s32 id, s32 x,
                                                          s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndCommit(id, x, z);
}

static __inline__ void ObjectMotion_ResetAndSetPositionInMode2_(s32 id, s32 x,
                                                                s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void ObjectMotion_ResetAndSetPosition_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ResetAndSetPosition(id, x, z);
}

static __inline__ void ObjectMotion_CommitCurrentPositionAndActivate_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_CommitCurrentPositionAndActivate(id);
}

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 id, s32 x,
                                                               s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Motion_SetModeAndWaitAnimation_(s32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Motion_SetVarCbAndRefresh_(s32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetVarCbAndRefresh(id, variant);
}

static __inline__ void Event_StartScreenTransition_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_StartScreenTransition();
}

static __inline__ void Event_WaitTransitionFrames_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_WaitTransitionFrames();
}

#endif
