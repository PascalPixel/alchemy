#ifndef TLA_CONTIGO_H
#define TLA_CONTIGO_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

struct ContigoObject;

void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_020040e4(s32 actor, s32 target);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *script);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *script);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Func_0200417c(s32 actor, s32 emote);
void Func_02004174(s32 actor, s32 height, s32 frames);
void Func_0200415c(s32 actor, s32 angle);
void Func_0200418c(s32 speed, s32 acceleration);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Func_0200413c(s32 actor, s32 a, s32 b);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
void Func_020042b4(s32 cue);
void Func_020040dc(s32 actor, s32 x, s32 z, s32 angle);
void Func_020040d4(s32 actor, s32 x, s32 z);
void Object_SetPartAttribute(struct ContigoObject *object, s32 value);
struct ContigoObject *Object_GetById(s32 id);
void Func_0200406c(void);
void Func_02004074(void);
void Func_02004204(s32 skip);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void WaitFrames(s32 frames);
void ObjectMotion_WaitForAnimationChange(s32 actor);
void Func_0200419c(void);
void Func_02003e54(struct ContigoObject *object, const void *effect);
void Func_02003f54(struct ContigoObject *object);
void Func_0200420c(void);
void Func_02004044(void);
void GameFlag_SetBit(s32 flag);
void Func_02004164(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h,
                   s32 i, s32 j, s32 k);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 mode);
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

static inline void Contigo_ObjFollow(s32 actor, s32 target) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Object_LinkObjectAndSetCallback(actor, target);
}

static inline void Contigo_Wait(s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Battle_WaitMode0(frames);
}

static inline void Contigo_ObjSpeed(s32 actor, s32 speed, s32 acceleration) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Contigo_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Contigo_ObjCommit(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static inline void Contigo_ObjTurn(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Contigo_ObjMoveTo(s32 actor, s32 target) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020040e4(actor, target);
}

static inline void Contigo_ObjAction(s32 actor, const void *script) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_EnableActionAndSetCallback(actor, script);
}

static inline void Contigo_ObjActionRefresh(s32 actor, const void *script) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Object_SetActionCallbackAndRefreshById(actor, script);
}

static inline void Contigo_ObjReset(s32 actor) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_EnableActionAndResetMotion(actor);
}

static inline void Contigo_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Motion_CamBounds(x, y, z, mode);
}

static inline void Contigo_ObjModeWait(s32 actor, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Motion_SetModeAndWaitAnimation(actor, mode);
}

static inline void Contigo_MessageSet(s32 message) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200412c(message);
}

static inline void Contigo_MessagePut(s32 speaker, s32 flags) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02004144(speaker, flags);
}

static inline void Contigo_ObjMode(s32 actor, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Object_SetModeById(actor, mode);
}

static inline void Contigo_ObjEmote(s32 actor, s32 emote) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
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
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200415c(actor, angle);
}

static inline void Contigo_CamSpeed(s32 speed, s32 acceleration) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200418c(speed, acceleration);
}

static inline void Contigo_ObjLaunch(s32 actor, s32 speed, s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_Launch(actor, speed, frames);
}

static inline void Contigo_ObjShake(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200413c(actor, a, b);
}

static inline s32 Contigo_MessageOpen(s32 speaker, s32 flags) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  return UiText_OpenMessageAtObject(speaker, flags);
}

static inline void Contigo_Se(s32 cue) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020042b4(cue);
}

static inline void Contigo_ObjPlaceTurn(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020040dc(actor, x, z, angle);
}

static inline void Contigo_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020040d4(actor, x, z);
}

static inline void Contigo_PairShow(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                    s32 g, s32 h, s32 i, s32 j, s32 k) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02004164(a, b, c, d, e, f, g, h, i, j, k);
}

static inline void Contigo_ObjVar(s32 actor, s32 value) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Motion_SetVarCbAndRefresh(actor, value);
}

static inline void Contigo_CopyCells(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                     s32 width, s32 height) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02004004(srcX, srcY, destX, destY, width, height);
}

static inline void Contigo_CopyAttrs(s32 srcX, s32 srcY, s32 width, s32 height,
                                     s32 destX, s32 destY) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200400c(srcX, srcY, width, height, destX, destY);
}

static inline void Contigo_CamSet(s32 a, s32 x, s32 y, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020041a4(a, x, y, z);
}

static inline void Contigo_Tiles(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200423c(srcX, srcY, width, height, destX, destY, mode);
}

#endif
