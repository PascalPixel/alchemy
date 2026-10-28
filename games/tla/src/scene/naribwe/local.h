#ifndef NARIBWE_LOCAL_H
#define NARIBWE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ActorPlacement gActorPlacements[];
extern const struct Trigger gSceneEvents[];

struct WarpEntry {
  const void *script;
  u16 x;
  u16 z;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetSingleChildFlags(struct Actor *object,
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

static __inline__ void Actor_SetSpeedParameters_(s32 id, s32 speed,
                                                        s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void
Actor_SetSingleChildFlags_(struct Actor *object, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSingleChildFlags(object, flags);
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
