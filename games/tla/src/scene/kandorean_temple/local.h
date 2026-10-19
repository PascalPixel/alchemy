#ifndef KANDOREAN_TEMPLE_LOCAL_H
#define KANDOREAN_TEMPLE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "field_sprite.h"
#include "input.h"

extern const struct SceneEntrance gSceneEntrances[];
extern const u32 gSceneExits[];
extern const struct ScenePlacement gScenePlacements[];
extern const struct SceneEvent gSceneEvents[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
u32 Random16Far(void);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Func_020051c0(void);
void Func_020051c8(void);
void Func_02005230(s32 id, s32 a, s32 b);
void Func_02005280(s32 message);
void Func_02005298(s32 id, s32 a);
void Func_020052b0(s32 id, s32 a);
s32 Func_020052c8(s32 a, s32 b, s32 c);
void Func_02005308(s32 a, s32 b);
void Func_02005370(s32 a);
void Func_02005378(s32 id, s32 x, s32 z, s32 angle);
void Func_02000668(struct ObjectRuntime *object, s32 a);

extern u8 MsgKandoreanTemple11;

struct ObjectRuntime *Func_02005140(s32 type, s32 x, s32 y, s32 z);
void Func_02005138(struct ObjectRuntime *object, const void *data);
void Func_02005130(struct ObjectRuntime *object, s32 a);
s32 Func_02005180(struct ObjectRuntime *object, s32 *position);
void Animation_ApplyChildArgument(void *sprite, s32 a);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_020006b0(struct ObjectRuntime *object);
void Func_02000778(struct ObjectRuntime *object, s32 dz);
void Func_020052c0(s32 id);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z);
void Func_02005348(s32 a, s32 b);
void Func_02005350(s32 a, s32 b);
void Func_02005358(void);
void Func_02005360(void);
void Func_02005368(void);
void Field_DispatchTypeHandler(s32 type);
void Func_0200098c(s32 *position, s32 value);
void Func_02000adc(void);
extern s32 gData_02006298;
extern s32 gData_0200629c;
extern s32 gData_02006294;
extern u32 gFrameCount;
extern const u8 gData_02005494[];
extern const u8 gData_020054e4[];
extern const u8 gData_020054f0[];

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005170(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 Func_020052a0(s32 id, s32 a);
void Func_020052f8(s32 a);
s32 Func_020053b0(void);
void Func_020053c0(const void *data);
void Func_020053c8(void);
void Func_020053d0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020053d8(s32 cue);
void Func_02005160(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02005168(struct ObjectRuntime *object);
void Func_020052e8(void);
struct ObjectRuntime *Func_020052f0(void);
void Func_020052d8(s32 speed, s32 acceleration);
void Func_020052d0(s32 id, s32 value);
void Func_02005300(s32 a, s32 b);
void Func_02005338(s32 a);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
s32 ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_0200290c(void);
void Func_02002a5c(void);
void Func_02002a84(void);
void Func_02002c1c(void);
void Func_02003a14(void);
void Func_02003a5c(void);
void Func_02003a8c(void);
void Func_02003b2c(void);
void Func_02003c8c(void);
extern u8 MsgKandoreanTemple12;
extern u8 MsgKandoreanTemple06;
extern u8 MsgKandoreanTemple05;
extern char SceneId_Ov653Room1;
extern char SceneId_Ov653Room2;
extern char SceneId_Ov653Room3;
extern char SceneId_Ov653Room4;
extern char SceneId_Ov653Room5;
extern char SceneId_Ov653Room6;
extern char SceneId_Ov653Room7;
extern char SceneId_Ov653Room8;
extern char SceneId_Ov653Room9;
void Func_02000cf8(s32 id, s32 dy, s32 scale);
void Func_02005388(s32 a);
void Func_02005390(void);
void Func_020053a8(s32 a, s32 b, s32 c, s32 d);
void Func_02005238(s32 id, s32 x, s32 z, s32 angle);
void Func_020051b0(s32 a);
void Func_02005240(s32 id, s32 target);
void Event_StartScreenTransition(void);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void VramCache_Load(s32 block, s32 count, const void *data);
void Runtime_ReleaseHeapBlock(s32 slot);
extern struct ObjectRuntime *gData_020062ac;
extern s32 gData_02005548[];
extern s32 gData_02005550[];

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

void Func_02001d40(void);
void Func_02001db4(void);
void Func_02001f3c(void);
void Func_02003524(void);
extern u8 MsgKandoreanTemple01;
extern u8 MsgKandoreanTemple02;
extern u8 MsgKandoreanTemple03;
extern u8 MsgKandoreanTemple09;
extern u8 MsgKandoreanTemple10;
extern u32 gData_02005514[];
extern s16 gData_020054dc[];
extern s16 gData_020054e0[];

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

static __inline__ u32 Random16Far_(void) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Random16Far();
}

static __inline__ struct ObjectRuntime *Object_GetById_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Object_GetById(id);
}

static __inline__ void Object_SetModeById_(u32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Object_SetModeById(id, mode);
}

static __inline__ void ObjectMotion_SetActionVariant_(u32 id, s32 variant) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetActionVariant(id, variant);
}

static __inline__ void ObjectMotion_SetPositionAndReset_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void ObjectMotion_ResetAndSetPosition_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ResetAndSetPosition(id, x, z);
}

static __inline__ void ObjectMotion_ArmCallback_(s32 id, s32 angle, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(id, angle, flags);
}

static __inline__ void ObjectMotion_CommitCurrentPositionAndActivate_(s32 id) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_CommitCurrentPositionAndActivate(id);
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

static __inline__ void Motion_SetModeAndWaitAnimation_(s32 id, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Motion_SetModeAndWaitAnimation(id, mode);
}

static __inline__ void Func_02005378_(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005378(id, x, z, angle);
}

static __inline__ struct ObjectRuntime *Func_02005140_(s32 type, s32 x, s32 y,
                                                       s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return Func_02005140(type, x, y, z);
}

static __inline__ void Animation_ApplyChildArgument_(void *sprite, s32 a) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Animation_ApplyChildArgument(sprite, a);
}

static __inline__ void Object_SetPartAttribute_(struct ObjectRuntime *object,
                                                s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Object_SetPartAttribute(object, value);
}

static __inline__ void
Object_SetPositionAndResetMotion_(struct ObjectRuntime *object, s32 x, s32 y,
                                  s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Object_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Func_02005178_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005178(a, b, c, d, e, f);
}

static __inline__ void ObjectMotion_ResetAndSetPositionInMode2_(s32 id, s32 x,
                                                                s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void Func_020053d0_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters give each stack
   * argument its own register; a direct call builds them all in r3. */
  Func_020053d0(a, b, c, d, e, f, g);
}

static __inline__ void Func_02000cf8_(s32 id, s32 dy, s32 scale) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000cf8(id, dy, scale);
}

static __inline__ void Func_020053a8_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020053a8(a, b, c, d);
}

static __inline__ void Func_02005238_(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005238(id, x, z, angle);
}

static __inline__ void Func_02005230_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005230(id, x, z);
}

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z,
                                                   s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void Func_020052d8_(s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020052d8(speed, acceleration);
}

static __inline__ void Func_020052d0_(s32 id, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020052d0(id, value);
}

/* FAKEMATCH: the native schedules this call as a value-returning one (the
 * variant is loaded before the actor: one reordered pair with the shared void
 * prototype). The routine is not C yet, so its return type is unproven. */
#define ObjectMotion_SetActionVariantValue(id, variant)                        \
  (((s32 (*)(u32, s32))ObjectMotion_SetActionVariant)((id), (variant)))

/* FAKEMATCH: a whole-byte bit-field keeps the struct's alias set, so the store
 * cannot alias the party state's words, as native; as u8 priorityFlags, the
 * next load is scheduled one step early in tla-en. */
struct ObjectFlags23 {
  u8 unk00[0x23];
  u8 flags : 8;
};

#endif
