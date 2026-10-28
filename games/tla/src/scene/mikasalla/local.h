#ifndef MIKASALLA_LOCAL_H
#define MIKASALLA_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_scene.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_02000618(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000620(s32 a);
void Func_02000628(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000630(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000648(void);
void Func_02000650(void);
void Func_02000698(s32 message);
void Func_020006a8(s32 id, s32 a);
void Func_020006d0(s32 a);
void Func_020006d8(s32 a, s32 b, s32 c);
s32 Func_020006e8(void);

void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02000438(void);

extern u8 MsgFieldAreYouLookingForSomeone;

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void WaitFrames_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  WaitFrames(frames);
}

static __inline__ void FieldEvent_WaitFrames_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_WaitFrames(frames);
}

static __inline__ struct ObjectRuntime *Object_GetById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Object_GetById(id);
}

static __inline__ void Object_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Object_SetModeById(id, mode);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void
ObjectDispatch_SetSingleChildFlags_(struct ObjectRuntime *object, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectDispatch_SetSingleChildFlags(object, flags);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldMessage_ShowAtActor(id, flags);
}

static __inline__ s32 Event_AskYesNoWithActorAnimation_(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Event_AskYesNoWithActorAnimation(id, flags);
}

static __inline__ void Func_02000628_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000628(a, b, c, d, e, f);
}

static __inline__ void Func_02000630_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000630(a, b, c, d, e, f);
}

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 id, s32 x,
                                                               s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_CommitPositionAndActivate(id, x, z);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

#endif
