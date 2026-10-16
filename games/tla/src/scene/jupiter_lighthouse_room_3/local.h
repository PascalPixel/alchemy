#ifndef JUPITER_LIGHTHOUSE_ROOM_3_LOCAL_H
#define JUPITER_LIGHTHOUSE_ROOM_3_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "dma.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

struct PushedBlock {
  s32 a;
  s32 b;
  s32 c;
  s32 d;
  s32 e;
  s32 f;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Scene_SetArrivalFlags(s32 a);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02002360(struct ObjectRuntime *object, s32 a);
void Func_02002368();
void Func_020023b0(s32 id, s32 x, s32 z);
void Func_020023d0(s32 id);
void Func_020023f8(s32 id);
void Func_02002400(void);
void Func_02002440(s32 id, s32 a);
void Func_02002458(s32 x, s32 z, s32 a, s32 b);
void Func_02002460(s32 x, s32 z, s32 a, s32 b);
void Func_02002468(s32 id);
void Func_02002470(s32 cue);
void Func_02000054(struct ObjectRuntime *object);
void Func_02000404(void);
void Func_02000c78(s32 id);
s32 Func_02000dcc(struct PushedBlock *block);
void Func_02001050(struct PushedBlock block);

extern s32 gData_02002cc0;
extern s32 gData_02002cc4;
extern const u16 gData_02002cec[];
extern const u16 gData_02002d0c[];
extern const u16 gData_02002d10[];

static __inline__ void Lift_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02002368(x, z, a, b);
}

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_SetBit(flag);
}

static __inline__ void GameFlag_ClearBit_(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_ClearBit(flag);
}

static __inline__ void WaitFrames_(s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  WaitFrames(frames);
}

static __inline__ void FieldEvent_WaitFrames_(s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  FieldEvent_WaitFrames(frames);
}

static __inline__ struct ObjectRuntime *Object_GetById_(s32 id) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return Object_GetById(id);
}

static __inline__ void Object_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Object_SetModeById(id, mode);
}

static __inline__ void ObjectMotion_SetActionVariant_(u32 id, s32 variant) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ s32 Map_GetTerrainHeight_(s32 layer, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return Map_GetTerrainHeight(layer, x, z);
}

static __inline__ void Func_020023b0_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020023b0(id, x, z);
}

static __inline__ void Func_02002400_(void) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02002400();
}

static __inline__ void Func_02002458_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02002458(x, z, a, b);
}

static __inline__ void Func_02002460_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02002460(x, z, a, b);
}

#endif
