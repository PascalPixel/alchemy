#ifndef TLA_DAILA_INTERIORS_DAILA_H
#define TLA_DAILA_INTERIORS_DAILA_H

#include "types.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 actor, s32 flags);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02000d4c(s32 actor, s32 x, s32 z);
void Func_02000d94(s32 actor, s32 jump, s32 frames);
void Func_02000cac(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);

static inline s32 Daila_FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void Daila_FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static inline void Daila_FlagClr(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
}

static inline void Daila_Wait(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_WaitFrames(frames);
}

static inline void Daila_MessageSet(s32 message) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Engine_EventSetMessage(message);
}

static inline void Daila_MessagePut(s32 actor, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Engine_EventShowMessage(actor, flags);
}

static inline void Daila_ObjLaunch(s32 actor, s32 speed, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_Launch(actor, speed, frames);
}

static inline void Daila_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Daila_ObjTurn(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Daila_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000d4c(actor, x, z);
}

static inline void Daila_Jump(s32 actor, s32 height, s32 frames) {
  s32 h = height;
  s32 f = frames;
  s32 a = actor;

  /* FAKEMATCH: plain arguments copy the actor two instructions early; finish
   * height and frames first. */
  __asm__("" : "+l"(h), "+l"(f), "+l"(a));
  Func_02000d94(a, h, f);
}

static inline void Daila_CopyCells(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                   s32 width, s32 height) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000cac(srcX, srcY, destX, destY, width, height);
}

#endif
