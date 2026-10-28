#ifndef TLA_JUPITER_LIGHTHOUSE_ROOM_6_H
#define TLA_JUPITER_LIGHTHOUSE_ROOM_6_H

#include "types.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct JupiterLighthouseRoom6Object {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 height;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[3];
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

struct JupiterLighthouseRoom6EventWork {
  u8 unk000[0x154];
  s16 exit;
  u8 unk156[4];
  s16 field15a;
  u8 unk15c[2];
  s16 field15e;
  u8 unk160[6];
  s16 field166;
  u8 unk168[0x44];
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
struct JupiterLighthouseRoom6Object *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotion(struct JupiterLighthouseRoom6Object *object, s32 x,
                                      s32 y, s32 z);
void Actor_SetPartAttribute(struct JupiterLighthouseRoom6Object *object, s32 value);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_SetSpritePriority(s32 actor, s32 variant);

void Func_02001b04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001b44(void);
void Func_02001b4c(void);
void Func_02001bc4(s32 skip);
void Func_02001bec(s32 cue);
void Func_02001b6c(s32 actor, s32 x, s32 z);
struct JupiterLighthouseRoom6Object *Func_02001acc(s32 type, s32 x, s32 y, s32 z);
void Func_02001ab4(struct JupiterLighthouseRoom6Object *object, s32 mode);
void Actor_SetSingleChildFlags(struct JupiterLighthouseRoom6Object *object,
                                        u32 value);
void Animation_SetStateFlags(struct JupiterLighthouseRoom6Object *object, s32 flags);
void Actor_SetModeById(s32 actor, s32 mode);
void Task_Add(void (*callback)(void), s32 priority);
void Task_RemoveFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001ba4(s32 countdown);

void Func_02001ae4(struct JupiterLighthouseRoom6Object *object, s32 x, s32 y, s32 z);
void Func_02001aec(struct JupiterLighthouseRoom6Object *object);
void Event_StartScreenTransition(void);

static inline void JupiterLighthouseRoom6_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                           s32 f) {
  /* FAKEMATCH: the blend start reaches the main image through one inline layer. */
  Func_02001b04(a, b, c, d, e, f);
}

static inline s32 JupiterLighthouseRoom6_FlagCheck(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static inline void JupiterLighthouseRoom6_FlagSet(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static inline void JupiterLighthouseRoom6_ObjArm(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

static inline void JupiterLighthouseRoom6_ObjSetPos(struct JupiterLighthouseRoom6Object *object,
                                          s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct JupiterLighthouseRoom6Object *Func_02001b9c(void);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Func_02001b84(s32 actor, s32 emote);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Actor_WaitForMoveById(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void JupiterLighthouseRoom6_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void JupiterLighthouseRoom6_ObjSpeed(s32 actor, s32 speed,
                                         s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static inline void JupiterLighthouseRoom6_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndReset(actor, x, z);
}

static inline void JupiterLighthouseRoom6_Wait(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldEvent_WaitFrames(frames);
}

static inline void JupiterLighthouseRoom6_SePlay(s32 cue) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001bec(cue);
}

static inline void JupiterLighthouseRoom6_WaitFrames(s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  WaitFrames(frames);
}

#define JUPITER_LIGHTHOUSE_ROOM_6_TASK_ADD(callback, priority)                            \
  {                                                                            \
    /* FAKEMATCH: a temporary keeps the priority constant ahead of the         \
     * callback load. */                                                       \
    s32 priority_ = (priority);                                                \
    Task_Add((callback), priority_);                      \
  }

static inline void JupiterLighthouseRoom6_ObjEmoteWait(s32 actor, s32 emote) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001b84(actor, emote);
}

static inline void JupiterLighthouseRoom6_TaskAdd(void (*callback)(void), s32 priority) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Task_Add(callback, priority);
}

static inline void JupiterLighthouseRoom6_MessageOpen(s32 speaker, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldMessage_ShowAtActor(speaker, flags);
}

void Func_02001b0c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static inline void JupiterLighthouseRoom6_MapPlay(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02001b0c(a, b, c, d, e, f);
}

// ---- this overlay

void Func_020000b4(s32 mode);

#endif
