#ifndef APOJII_ISLANDS_LOCAL_H
#define APOJII_ISLANDS_LOCAL_H

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
void UiText_OpenMessageAtObject(s32 id, s32 flags);
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
s32 Map_GetTerrainHeight(s32 x, s32 z);
void Func_02001bf4(void *callback);
void Func_02001bfc(void *callback);
void Func_02001c0c(s32 a, s32 b, void *callback);
void Func_02001c24(const void *source, void *destination);
void Func_02001c6c(struct ObjectRuntime *object, s32 mode);
void Func_02001c74(struct ObjectRuntime *object, const void *data);
void Func_02001c84(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02001c8c(struct ObjectRuntime *object);
s32 Func_02001c9c(struct ObjectRuntime *object, s32 *position);
void Func_02001cc4(void);
void Func_02001ccc(void);
void Func_02001d04(s32 id, s32 x, s32 z);
void Func_02001d2c(s32 id, s32 a, s32 b);
void Func_02001d44(s32 message);
void Func_02001d54(s32 id, s32 a);
void Func_02001d6c(s32 id, s32 emote, s32 frames);
void Func_02001d74(s32 id, s32 emote);
void Func_02001d7c(s32 a, s32 b);
void Func_02001d8c(void);
void Func_02001d94(s32 a);
void Func_02001d9c(void);
void Func_02001da4(s32 a, s32 b);
void Func_02001dac(s32 a);
void Func_02001dcc(s32 a);
void Func_02001dd4(s32 a, s32 b, s32 c);
void Func_02001de4(void);
void Func_02001dec(s32 a);
void Func_02001df4(void);
void Func_02001dfc(void);
void Func_02001e04(s32 a, s32 b, s32 c, s32 d);
void Func_02001e0c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02001e14(void);
void Func_02001e1c(s32 a, s32 b);
void Func_02001e24(s32 a, s32 b);
void Func_02001e2c(s32 cue);

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);
void Func_020003e8(void);
void Func_02000540(void);
void Func_020006bc(void);
void Func_02000770(struct ObjectRuntime *object);
void Func_02000780(struct ObjectRuntime *object);
void Func_02000974(void);
void Func_02000ef8(void);
void Func_02000f58(void);
void Func_02000f98(void);
void Func_02001054(void);
void Func_020010b4(void);
void Func_020010d8(struct ObjectRuntime *object);
s32 Func_020013d8(struct ObjectRuntime *object);
void Func_02001500(void);
void Func_020015f8(void);
void Func_020017bc(void);
void Func_020017f8(s32 a, s32 b, s32 c);
void Func_02001940(void);
void Func_02001ad8(void);
void Func_02001ba0(s32 id);

extern s32 gData_02002a98;
extern u16 gData_02002a9c;
extern u16 gData_02002a9e;
extern u16 gData_02002aa0[];
extern const u8 gData_02001ee8[];
extern const u8 gData_02001ef4[];
extern const u8 gField_Tiles679[];
extern u32 gFrameCount;

extern u8 MsgWhatWhatJust;
extern u8 MsgWhyDoesMy;
extern u8 MsgICanTEver;
extern u8 MsgWowYouLook;
extern u8 MsgYouGuysAren;
extern u8 MsgItDBe;

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

static __inline__ void UiText_OpenMessageAtObject_(s32 id, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  UiText_OpenMessageAtObject(id, flags);
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

static __inline__ s32 Map_GetTerrainHeight_(s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Map_GetTerrainHeight(x, z);
}

static __inline__ void Func_02001c6c_(struct ObjectRuntime *object, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c6c(object, mode);
}

static __inline__ void Func_02001c84_(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c84(object, x, y, z);
}

static __inline__ void Func_02001d04_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d04(id, x, z);
}

static __inline__ void Func_02001d44_(s32 message) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d44(message);
}

static __inline__ void Func_02001d54_(s32 id, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d54(id, a);
}

static __inline__ void Func_02001d7c_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001d7c(a, b);
}

static __inline__ void Func_02001e0c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001e0c(a, b, c, d, e, f, g);
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

static __inline__ void Func_02001e04_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001e04(a, b, c, d);
}

static __inline__ void Func_02001c74_(struct ObjectRuntime *object,
                                      const void *data) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c74(object, data);
}

#endif
