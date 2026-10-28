#ifndef APOJII_ISLANDS_LOCAL_H
#define APOJII_ISLANDS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "field_sprite.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gScenePlacements[];
extern const struct Trigger gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
struct Actor *Import_ActorCreate(s32 kind, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Animation_ApplyChildArgument(void *animation, s32 value);
u8 *ResourceMetadata_Register(void *animation, s32 id);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_SetAngleToward(s32 id, s32 target, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_Jump(s32 id, s32 a, s32 b);
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
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Map_GetTerrainHeight(s32 x, s32 z);
void Func_02001bf4(void *callback);
void Func_02001bfc(void *callback);
void Func_02001c0c(s32 a, s32 b, void *callback);
void Func_02001c24(const void *source, void *destination);
void Func_02001c6c(struct Actor *object, s32 mode);
void Func_02001c74(struct Actor *object, const void *data);
void Func_02001c84(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02001c8c(struct Actor *object);
s32 Func_02001c9c(struct Actor *object, s32 *position);
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
void Func_02000770(struct Actor *object);
void Func_02000780(struct Actor *object);
void Func_02000974(void);
void Func_02000ef8(void);
void Func_02000f58(void);
void Func_02000f98(void);
void Func_02001054(void);
void Func_020010b4(void);
void Func_020010d8(struct Actor *object);
s32 Func_020013d8(struct Actor *object);
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

static __inline__ struct Actor *Actor_GetById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Actor_GetById(id);
}

static __inline__ void Actor_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetModeById(id, mode);
}

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x,
                                                             s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Import_ActorCreate(kind, x, y, z);
}

static __inline__ void Actor_SetPartAttribute_(struct Actor *object,
                                                s32 attribute) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPartAttribute(object, attribute);
}

static __inline__ void
Actor_SetSingleChildFlags_(struct Actor *object, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSingleChildFlags(object, flags);
}

static __inline__ void Animation_ApplyChildArgument_(void *animation,
                                                     s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Animation_ApplyChildArgument(animation, value);
}

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 x,
                                                                  s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void Actor_WaitForMoveById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_WaitForMoveById(id);
}

static __inline__ void Actor_CommitPositionAndActivate_(s32 id, s32 x,
                                                               s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Actor_SetSpritePriority_(u32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpritePriority(id, variant);
}

static __inline__ void Actor_SetAngleToward_(s32 id, s32 target,
                                                    s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetAngleToward(id, target, flags);
}

static __inline__ void Actor_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, flags);
}

static __inline__ void Actor_Jump_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_Jump(id, a, b);
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

static __inline__ s32 Map_GetTerrainHeight_(s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Map_GetTerrainHeight(x, z);
}

static __inline__ void Func_02001c6c_(struct Actor *object, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c6c(object, mode);
}

static __inline__ void Func_02001c84_(struct Actor *object, s32 x,
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

static __inline__ void Func_02001c74_(struct Actor *object,
                                      const void *data) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001c74(object, data);
}

#endif
