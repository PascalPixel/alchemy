#ifndef TLA_CONTIGO_H
#define TLA_CONTIGO_H

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

struct ContigoObject;

void Actor_WatchAndSetScript(s32 actor, s32 target);
void FieldEvent_Wait(s32 frames);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_020040e4(s32 actor, s32 target);
void Actor_SetMovementTypeById(s32 actor, const void *script);
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *script);
void Actor_Stop(s32 actor);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Actor_SetModeById(s32 actor, s32 mode);
void Func_0200417c(s32 actor, s32 emote);
void Func_02004174(s32 actor, s32 height, s32 frames);
void Func_0200415c(s32 actor, s32 angle);
void Func_0200418c(s32 speed, s32 acceleration);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Func_0200413c(s32 actor, s32 a, s32 b);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
void Func_020042b4(s32 cue);
void Func_020040dc(s32 actor, s32 x, s32 z, s32 angle);
void Func_020040d4(s32 actor, s32 x, s32 z);
void Actor_SetPartAttribute(struct ContigoObject *object, s32 value);
struct ContigoObject *Actor_GetById(s32 id);
void Func_0200406c(void);
void Func_02004074(void);
void Func_02004204(s32 skip);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void wait(s32 frames);
void Actor_WaitForAnimationChange(s32 actor);
void Func_0200419c(void);
void Func_02003e54(struct ContigoObject *object, const void *effect);
void Func_02003f54(struct ContigoObject *object);
void Func_0200420c(void);
void Func_02004044(void);
void GameFlag_SetBit(s32 flag);
void Func_02004164(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h,
                   s32 i, s32 j, s32 k);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02001938(s32 mode);
void Func_02001c5c(s32 a, s32 b, s32 c, s32 d);
void Func_02004004(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_0200400c(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_020041a4(s32 a, s32 x, s32 y, s32 z);
void Func_020041b4(s32 countdown);
void Func_02004214(s32 a, s32 b, s32 c);
void Func_0200423c(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY, s32 mode);

static inline void Contigo_ObjSpeed(s32 actor, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Contigo_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(actor, x, z);
}

static inline void Contigo_ObjTurn(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

static inline void Contigo_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void Contigo_MessagePut(s32 speaker, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004144(speaker, flags);
}

static inline void Contigo_ObjEmote(s32 actor, s32 emote) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200417c(actor, emote);
}

static inline void Contigo_Jump(s32 actor, s32 height, s32 frames) {
  /* FAKEMATCH: the native loads the actor after the height and frames. */
  register s32 h __asm__("r1") = height;
  /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
  register s32 f __asm__("r2") = frames;
  /* FAKEMATCH: and the actor last, in r0. */
  register s32 a __asm__("r0") = actor;

  Func_02004174(a, h, f);
}

static inline void Contigo_ObjFace(s32 actor, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200415c(actor, angle);
}

static inline void Contigo_CamSpeed(s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200418c(speed, acceleration);
}

static inline s32 Contigo_MessageOpen(s32 speaker, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return FieldMessage_ShowAtActor(speaker, flags);
}

static inline void Contigo_PairShow(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                    s32 g, s32 h, s32 i, s32 j, s32 k) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004164(a, b, c, d, e, f, g, h, i, j, k);
}

static inline void Contigo_CopyCells(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                     s32 width, s32 height) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004004(srcX, srcY, destX, destY, width, height);
}

static inline void Contigo_CopyAttrs(s32 srcX, s32 srcY, s32 width, s32 height,
                                     s32 destX, s32 destY) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200400c(srcX, srcY, width, height, destX, destY);
}

static inline void Contigo_CamSet(s32 a, s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020041a4(a, x, y, z);
}

static inline void Contigo_Tiles(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200423c(srcX, srcY, width, height, destX, destY, mode);
}

#endif
