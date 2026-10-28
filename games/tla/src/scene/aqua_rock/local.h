#ifndef AQUA_ROCK_LOCAL_H
#define AQUA_ROCK_LOCAL_H

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
extern const struct ActorPlacement gActorPlacements[];
extern const struct Trigger gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
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
void Task_Add(void *func, s32 priority);
void Task_RemoveFar(void *func);
s32 Random16Far(void);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

extern u32 gFrameCount;

#include "scene_ids.h"

struct AquaRockWork {
  struct Actor *object;
  u16 vramEntry;
  u16 unk06;
  s16 direction;
};

s32 Math_Sine(s32 angle);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Func_020028b8(const void *source, void *destination);
void Func_02002910(struct Actor *object, s32 mode);
void Func_02002918(struct Actor *object, const void *data);
void Func_02002928(struct Actor *object);
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
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
s32 Func_02002030(struct Actor *object);

void Func_02000468(struct Actor *object);
void Func_020004e4(struct Actor *object);
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

static __inline__ void wait_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  wait(frames);
}

static __inline__ void FieldEvent_Wait_(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_Wait(frames);
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

static __inline__ void Actor_ResetAndSetPosition_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ResetAndSetPosition(id, x, z);
}

static __inline__ void *MemAlloc_(s32 size) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Heap_Alloc(size);
}

static __inline__ s32 VramBlock_LoadCached_(s32 entry, s32 size, void *data) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return VramCache_Load(entry, size, data);
}

static __inline__ void Task_Add_(void *func,
                                                      s32 priority) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Task_Add(func, priority);
}

static __inline__ void Func_02002918_(struct Actor *object,
                                      const void *data) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002918(object, data);
}

#endif
