#ifndef TLA_LEMURIAN_SHIP_H
#define TLA_LEMURIAN_SHIP_H

#include "types.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct LemurianShipObject {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[2];
  u8 field22;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x18];
  s32 field44;
  s32 field48;
  u8 unk4c[4];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 unk5a[2];
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct LemurianShipEventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[2];
  u16 field16a;
  u8 unk16c[0x40];
  s32 screenEffect;
  u8 unk1b0[4];
  s32 field1b4;
  u8 unk1b8[0x0c];
  u16 field1c4;
};

extern s32 gFrameCount;

u32 Random16Far(void);
void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
struct LemurianShipObject *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct LemurianShipObject *object, s32 x,
                                      s32 y, s32 z);
void Object_SetPartAttribute(struct LemurianShipObject *object, s32 value);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);

void Func_02002c30(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002c38(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002c68(void);
void Func_02002c70(void);
void Func_02002d18(s32 message);
void Func_02002d20(s32 speaker, s32 flags);
void Func_02002d30(s32 actor, s32 angle);
void Func_02002da8(s32 skip);
void Func_02002dc0(const void *script);
void Func_02002de0(s32 cue);
void Func_02002cd0(s32 actor, s32 x, s32 z);
struct LemurianShipObject *Func_02002c00(s32 type, s32 x, s32 y, s32 z);
void Func_02002bf0(struct LemurianShipObject *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct LemurianShipObject *object,
                                        u32 value);
void Animation_SetStateFlags(struct LemurianShipObject *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Task_Add(void (*callback)(void), s32 priority);
void Task_RemoveFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002d70(s32 countdown);

void Func_02002db8(const void *script);
void Func_02002c18(struct LemurianShipObject *object, s32 x, s32 y, s32 z);
void Func_02002c20(struct LemurianShipObject *object);
void Event_StartScreenTransition(void);

static inline void LemurianShip_ObjTurn(s32 actor, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002d30(actor, angle);
}

static inline void LemurianShip_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                           s32 f) {
  /* FAKEMATCH: the blend start reaches the main image through one inline layer. */
  Func_02002c38(a, b, c, d, e, f);
}

static inline s32 LemurianShip_FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void LemurianShip_FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static inline void LemurianShip_ObjArm(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void LemurianShip_ObjSetPos(struct LemurianShipObject *object,
                                          s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Object_SetPositionAndResetMotion(object, x, y, z);
}

void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02002d58(s32 speed, s32 acceleration);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02002cd8(s32 actor, s32 target);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Func_02002d40(s32 actor, s32 emote, s32 frames);
void Func_02002d48(s32 actor, s32 emote);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void LemurianShip_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void LemurianShip_ObjSpeed(s32 actor, s32 speed,
                                         s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void LemurianShip_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void LemurianShip_MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002c30(a, b, c, d, e, f);
}

static inline void LemurianShip_Wait(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_WaitFrames(frames);
}

static inline void LemurianShip_SePlay(s32 cue) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002de0(cue);
}

static inline void LemurianShip_WaitFrames(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  WaitFrames(frames);
}

#define LEMURIAN_SHIP_TASK_ADD(callback, priority)                             \
  {                                                                            \
    /* FAKEMATCH: a temporary keeps the priority constant ahead of the         \
     * callback load. */                                                       \
    s32 priority_ = (priority);                                                \
    Task_Add((callback), priority_);                      \
  }

static inline void LemurianShip_ObjEmoteWait(s32 actor, s32 emote) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002d48(actor, emote);
}

static inline void LemurianShip_TaskAdd(void (*callback)(void), s32 priority) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Task_Add(callback, priority);
}

static inline void LemurianShip_MessagePut(s32 speaker, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002d20(speaker, flags);
}

static inline void LemurianShip_MessageOpen(s32 speaker, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  UiText_OpenMessageAtObject(speaker, flags);
}

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void Camera_FollowActor(s32 actor, s32 value);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void ObjectMotion_SetVariantCallback(s32 actor, s32 value);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_02002c10(void);
void Func_02002c48(s32 a, s32 b, s32 c);
void Func_02002d68(s32 a, s32 b, s32 c, s32 d);
void Func_02002d78(s32 a, s32 b);
void Func_02002d88(s32 a, s32 b);
void Func_02002dd0(const void *script);
void Func_02002dd8(void);

static inline void LemurianShip_ObjCommit(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static inline void LemurianShip_CamSpeed(s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002d58(speed, acceleration);
}

static inline void LemurianShip_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002cd0(actor, x, z);
}

static inline void LemurianShip_CamSet(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002d68(a, b, c, d);
}

// ---- this overlay

void Func_02000d10(s32 id);
void Func_02000e2c(void);
void Func_02001380(s32 id);
void Func_020013b8(s32 id);

extern const u8 gData_02002e30[];
extern const u8 gData_02002e6c[];
extern const u8 gData_02002e9c[];
extern const u8 gData_02003010[];
extern const u8 gData_02003014[];
extern const u8 gData_0200301e[];
extern const u8 gData_02003034[];
extern const u8 gData_0200304e[];

void Func_02001a04(void);
void Func_02001650(void);
void Func_02002bf8(struct LemurianShipObject *object, const void *script);
void Func_02002dc8(const void *script);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
extern const u8 gData_02003ef8[];

#endif
