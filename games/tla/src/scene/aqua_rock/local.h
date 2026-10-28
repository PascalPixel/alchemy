#ifndef AQUA_ROCK_LOCAL_H
#define AQUA_ROCK_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
struct ObjectRuntime *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_CenterOnColumn(s32 id, s32 column, s32 offset);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Task_Add(void *callback, s32 priority);
void Task_RemoveFar(void *callback);
s32 Random16Far(void);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void *Runtime_AllocateBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

extern u32 gFrameCount;

#include "scene_ids.h"

struct AquaRockWork {
  struct ObjectRuntime *object;
  u16 vramEntry;
  u16 unk06;
  s16 direction;
};

s32 Math_Sine(s32 angle);
void *MemAlloc(s32 size);
void MemFree(void *block);
void Func_020028b8(const void *source, void *destination);
void Func_02002910(struct ObjectRuntime *object, s32 mode);
void Func_02002918(struct ObjectRuntime *object, const void *data);
void Func_02002928(struct ObjectRuntime *object);
void Func_02002960(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002958(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002a20(s32 a, s32 b);
s32 Func_02002a68(void);
void Func_02002998(void);
void Func_020029a0(void);
void Func_020029d8(s32 id, s32 x, s32 z);
void Func_02002a18(s32 a);
void Func_02002a48(s32 a);
void Func_02002a60(void);
void Func_02002a70(s32 cue);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
s32 Func_02002030(struct ObjectRuntime *object);

void Func_02000468(struct ObjectRuntime *object);
void Func_020004e4(struct ObjectRuntime *object);
void Func_02002250(void);
void Func_020023c0(void);
void Func_02002534(void);

extern struct AquaRockWork gData_020034f0;
extern const u8 gData_02002c44[];
extern const u8 gData_02002c50[];
extern const u8 gData_02002c74[];
extern const u8 gData_02002e30[];
extern const u8 gData_02002e60[];
extern const u8 gData_02002eec[];
extern const u8 gData_02002f04[];
extern const u8 gData_0200312c[];
extern const u8 gData_02003214[];
extern const u8 gData_02003220[];
extern const u8 gData_020032d4[];

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void GameFlag_ClearBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
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

static __inline__ struct ObjectRuntime *Engine_ObjectCreate_(s32 kind, s32 x,
                                                             s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Engine_ObjectCreate(kind, x, y, z);
}

static __inline__ void Object_SetPartAttribute_(struct ObjectRuntime *object,
                                                s32 attribute) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Object_SetPartAttribute(object, attribute);
}

static __inline__ void
ObjectDispatch_SetSingleChildFlags_(struct ObjectRuntime *object, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectDispatch_SetSingleChildFlags(object, flags);
}

static __inline__ void Animation_ApplyChildArgument_(void *animation,
                                                     s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Animation_ApplyChildArgument(animation, value);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void ObjectMotion_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_CenterOnColumn(id, column, offset);
}

static __inline__ void ObjectMotion_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                                  s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_OffsetPositionAndResetMotion(id, x, z);
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

static __inline__ void ObjectMotion_SetActionVariant_(u32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ void ObjectMotion_SetAngleToward_(s32 id, s32 target,
                                                    s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetAngleToward(id, target, flags);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void ObjectMotion_Launch_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_Launch(id, a, b);
}

static __inline__ void Motion_SetModeAndWaitAnimation_(s32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Motion_SetVarCbAndRefresh_(s32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetVarCbAndRefresh(id, variant);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 a, s32 z, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, a, z, b);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldMessage_ShowAtActor(id, flags);
}

static __inline__ s32 Event_AskYesNoWithActorAnimation_(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Event_AskYesNoWithActorAnimation(id, flags);
}

static __inline__ void Event_StartScreenTransition_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_StartScreenTransition();
}

static __inline__ void Event_FinishScreenTransition_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_FinishScreenTransition();
}

static __inline__ void Event_WaitTransitionFrames_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_WaitTransitionFrames();
}

static __inline__ s32 Random16Far_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Random16Far();
}

static __inline__ s32 Map_GetTerrainHeight_(s32 layer, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Map_GetTerrainHeight(layer, x, z);
}

static __inline__ void Func_02002960_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002960(a, b, c, d, e, f);
}

static __inline__ void Func_02002958_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002958(a, b, c, d, e, f);
}

static __inline__ void Func_02002a20_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002a20(a, b);
}

static __inline__ void ObjectMotion_ResetAndSetPosition_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ResetAndSetPosition(id, x, z);
}

static __inline__ void *MemAlloc_(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return MemAlloc(size);
}

static __inline__ s32 VramBlock_LoadCached_(s32 entry, s32 size, void *data) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return VramCache_Load(entry, size, data);
}

static __inline__ void Task_Add_(void *callback,
                                                      s32 priority) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Task_Add(callback, priority);
}

static __inline__ void Func_02002918_(struct ObjectRuntime *object,
                                      const void *data) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002918(object, data);
}

#endif
