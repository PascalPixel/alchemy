#ifndef NARIBWE_LOCAL_H
#define NARIBWE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_scene.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

struct WarpEntry {
  const void *script;
  u16 x;
  u16 z;
};

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
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000534(const void *script, s32 x, s32 z);
void Func_0200054c(void);
void Func_02000554(void);
void Func_02000574(s32 id, s32 x, s32 z);
void Func_02000584(s32 message);
void Func_02000594(s32 id, s32 a);
void Func_0200059c(s32 id);
void Func_020005a4(s32 a);
void Func_020005bc(s32 id, s32 x, s32 z);
void Func_020005c4(s32 a);
void Func_020005cc(void);
void Func_020005d4(s32 a, s32 b, s32 c, s32 d);
s32 Func_020005dc(void);
void Func_020005e4(s32 a, s32 b);
void Func_020005ec(s32 a);
void Func_020005f4(s32 a);
void Func_02000464(void);

extern const u8 gData_020006cc[];
extern const u8 gData_020008c4[];
extern const u8 gData_02000954[];
extern const u8 gData_02000b4c[];
extern const u8 gData_02000cfc[];
extern const struct WarpEntry gData_0200064c[];

extern u8 MsgNaribwe02;
extern u8 MsgNaribwe03;
extern u8 MsgNaribwe07;
extern u8 MsgNaribwe08;
extern u8 MsgNaribwe09;
extern u8 MsgNaribwe21;
extern u8 MsgNaribwe43;
extern u8 MsgNaribwe23;
extern u8 MsgNaribwe45;

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

static __inline__ void Event_FinishScreenTransition_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_FinishScreenTransition();
}

static __inline__ void Event_WaitTransitionFrames_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Event_WaitTransitionFrames();
}

static __inline__ void Func_02000574_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000574(id, x, z);
}

static __inline__ void Func_020005bc_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020005bc(id, x, z);
}

#endif
