#ifndef TLA_SHAMAN_VILLAGE_H
#define TLA_SHAMAN_VILLAGE_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct ShamanVillageObject {
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
  u8 actionFlags;
  u8 unk5b;
  u8 field5c;
  u8 unk5d[5];
  u8 timer;
  u8 unk63;
  s16 field64;
  s16 field66;
  s32 field68;
  void *update;
};

struct ShamanVillageEventWork {
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
void Battle_WaitMode0(s32 frames);
struct ShamanVillageObject *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct ShamanVillageObject *object, s32 x,
                                      s32 y, s32 z);
void Object_SetPartAttribute(struct ShamanVillageObject *object, s32 value);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);

void Func_02004844(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200484c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020048c4(void);
void Func_020048cc(void);
void Func_0200498c(s32 message);
void Func_0200499c(s32 actor, s32 a, s32 b);
void Func_020049a4(s32 speaker, s32 flags);
s32 Func_020049ac(s32 speaker, s32 flags);
void Func_020049bc(s32 actor, s32 angle);
void Func_02004a24(s32 skip);
void Func_02004a74(s32 shop, s32 keeper);
void Func_02004a7c(s32 keeper);
void Func_02004a84(s32 shop, s32 keeper);
void Engine_AudioPlayCue(s32 cue);
void Func_02004934(s32 actor, s32 x, s32 z);
struct ShamanVillageObject *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Engine_ObjectSetMode(struct ShamanVillageObject *object, s32 mode);
void Func_0200481c(struct ShamanVillageObject *object);
void ObjectDispatch_SetSingleChildFlags(struct ShamanVillageObject *object,
                                        u32 value);
void Animation_SetStateFlags(struct ShamanVillageObject *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020049f4(void);
void Func_02004a04(s32 countdown);

void Func_02004a44(void);
void Func_02004a4c(s32 a, s32 b, s32 c, s32 d);
void Func_02004834(struct ShamanVillageObject *object, s32 x, s32 y, s32 z);
void Func_0200483c(struct ShamanVillageObject *object);
void Event_StartScreenTransition(void);

static inline void ShamanVillage_ObjTurn(s32 actor, s32 angle) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020049bc(actor, angle);
}

static inline void ShamanVillage_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                            s32 f) {
  /* FAKEMATCH: the blend start reaches the engine through one inline layer. */
  Func_0200484c(a, b, c, d, e, f);
}

static inline s32 ShamanVillage_FlagCheck(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  return GameFlag_Test(flag);
}

static inline void ShamanVillage_FlagSet(s32 flag) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  GameFlag_SetBit(flag);
}

static inline void ShamanVillage_ObjArm(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void ShamanVillage_ObjSetPos(struct ShamanVillageObject *object,
                                           s32 x, s32 y, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Object_SetPositionAndResetMotion(object, x, y, z);
}

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
struct ShamanVillageObject *Func_020049fc(void);
void Func_020049e4(s32 speed, s32 acceleration);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02004944(s32 actor, s32 target);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Func_020049cc(s32 actor, s32 emote, s32 frames);
void Func_020049d4(s32 actor, s32 emote);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void ShamanVillage_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Motion_CamBounds(x, y, z, mode);
}

static inline void ShamanVillage_ObjSpeed(s32 actor, s32 speed,
                                          s32 acceleration) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void ShamanVillage_ObjWalk(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void ShamanVillage_MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e,
                                         s32 f) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02004844(a, b, c, d, e, f);
}

static inline void ShamanVillage_Wait(s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Battle_WaitMode0(frames);
}

static inline void ShamanVillage_SePlay(s32 cue) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Engine_AudioPlayCue(cue);
}

static inline void ShamanVillage_WaitFrames(s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  WaitFrames(frames);
}

#define SHAMAN_VILLAGE_TASK_ADD(callback, priority)                            \
  {                                                                            \
    /* FAKEMATCH: a temporary keeps the priority constant ahead of the         \
     * callback load. */                                                       \
    s32 priority_ = (priority);                                                \
    Scheduler_AddOrUpdateCallback((callback), priority_);                      \
  }

static inline void ShamanVillage_ObjEmoteWait(s32 actor, s32 emote) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020049d4(actor, emote);
}

static inline void ShamanVillage_TaskAdd(void (*callback)(void), s32 priority) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void ShamanVillage_MessagePut(s32 speaker, s32 flags) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020049a4(speaker, flags);
}

static inline void ShamanVillage_MessageOpen(s32 speaker, s32 flags) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  UiText_OpenMessageAtObject(speaker, flags);
}

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void Object_AttachWorkTargetToObject(s32 actor, s32 value);
void Func_02004a34(s32 a, s32 b, s32 c);
void Func_02004a54(void);
void Func_02004a5c(void);
void Func_02004a64(void);
void Func_02004a6c(struct ShamanVillageObject *object, s32 value);
void Func_02004a2c(void);
void Func_020048b4(void);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void ObjectMotion_SetVariantCallback(s32 actor, s32 value);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

static inline void ShamanVillage_CamSpeed(s32 speed, s32 acceleration) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020049e4(speed, acceleration);
}

static inline void ShamanVillage_ObjWalk2(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  ObjectMotion_ResetAndSetPositionInMode2(actor, x, z);
}

static inline void ShamanVillage_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_02004934(actor, x, z);
}

#define SHAMAN_VILLAGE_EMOTE(actor, emote, frames)                             \
  {                                                                            \
    /* FAKEMATCH: the native loads the emote and the frames before the actor   \
     * at every */                                                             \
    /* emote of this overlay; pinned to their argument registers they are      \
     * loaded first, */                                                        \
    /* as there. Measured against it: an inline (the actor comes one row early \
     * wherever */                                                             \
    /* the emote takes two instructions), an inline with its parameters in     \
     * another order */                                                        \
    /* or modified, a local (shared between calls), a one-pass block, and      \
     * barriers that */                                                        \
    /* hold r0 (each empty asm adds four bytes to the pool estimate and moves  \
     * the literal */                                                          \
    /* pool of a long scene). */                                               \
    register s32 emote_ __asm__("r1") = (emote);                               \
    /* FAKEMATCH: the frames, pinned with the emote above. */                  \
    register s32 frames_ __asm__("r2") = (frames);                             \
    Func_020049cc((actor), emote_, frames_);                                   \
  }

void Func_0200493c(s32 actor, s32 x, s32 z, s32 angle);

static inline void ShamanVillage_ObjPut(s32 actor, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200493c(actor, x, z, angle);
}

static inline void ShamanVillage_MessageWait(s32 speaker, s32 a, s32 frames) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_0200499c(speaker, a, frames);
}

// ---- this overlay

void Func_0200057c(void);
void Func_020011f8(void);
void Func_02002f20(s32 a, s32 b);
void Func_02002fb0(s32 a);
void Func_02003b6c(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f);
s32 Func_02000f94(void);
void Func_02001380(void);
void Func_0200289c(void);
void Func_0200290c(void);
void Func_02003040(void);

extern const u8 gData_02004c08[];
extern const u8 gData_020050f8[];
extern u8 gData_02005284[];
extern u8 gData_020052b4[];
extern u8 gData_02005314[];

#endif
