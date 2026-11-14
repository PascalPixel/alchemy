#ifndef TLA_KIBOMBO_H
#define TLA_KIBOMBO_H

#include "types.h"
#include "system/system.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "field/event.h"
#include "system/task.h"
#include "field_sprite.h"
#include "system/frame.h"

void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_SetSpriteColor(struct Actor *object, s32 value);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);

void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Event_ShowMessageThenWait(s32 actor, s32 a, s32 b);
void Event_ShowMessageAndWait(s32 speaker, s32 flags);
void Actor_FaceDirectionById(s32 actor, s32 angle);
void Actor_DisableById(s32 actor);
void FlagBlock_ApplyFlags(const void *script);
void Shop_Run(s32 shop, s32 keeper);
void Sanctum_Run(s32 keeper);
void Inn_CheckIn(s32 shop, s32 keeper);
void Sound_Play(s32 cue);
void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_Release(struct Actor *object);
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void Actor_SetModeById(u32 actorId, s32 animation);
void FieldPsynergy_RunHandler(s32 type);
void FieldCamera_WaitForArrival(void);
void Import_EventSetExitRequest(s32 countdown);
void BattleEffect_LoadActionEffectResources(s32 a, s32 b);
void BattleEffect_StopMainActor(void);

void Field_TryJumpForward(void);
void FieldEffect_StartBlankOverlay(s32 a);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
void FlagBlock_PlaceAll(const void *script);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMove(struct Actor *object);

static inline void Kibombo_ObjTurn(s32 actor, s32 angle) {
  Actor_FaceDirectionById(actor, angle);
}

static inline void Kibombo_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): the blend start reaches the main image through this
   * inline. */
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static inline s32 Kibombo_FlagCheck(s32 flag) { return GameFlag_Get(flag); }

static inline void Kibombo_ObjArm(s32 actor, s32 angle, s32 frames) {
  Actor_ArmCallback(actor, angle, frames);
}

static inline void Kibombo_ObjSetPos(struct Actor *object, s32 x, s32 y,
                                     s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

void FieldCamera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *FieldCamera_GetActor(void);
void Camera_SetSpeedParameters(s32 speed, s32 acceleration);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_MoveToObject(s32 actor, s32 target);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldEffect_SetAttached(s32 actor, s32 emote);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Actor_WaitForMoveById(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void Kibombo_CamMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void Kibombo_CamSpeed(s32 speed, s32 acceleration) {
  Camera_SetSpeedParameters(speed, acceleration);
}

static inline void Kibombo_ObjSpeed(s32 actor, s32 speed, s32 acceleration) {
  Actor_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Kibombo_ObjWalk(s32 actor, s32 x, s32 z) {
  Actor_WalkToAndWaitById(actor, x, z);
}

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);

static inline void Kibombo_ObjPlaceFacing(s32 actor, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

void Actor_WalkToById(s32 actor, s32 x, s32 z);

static inline void Kibombo_ObjPlace(s32 actor, s32 x, s32 z) {
  Actor_WalkToById(actor, x, z);
}

static inline void Kibombo_MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  MapGrid_CopyCellPictureIndicesRect(a, b, c, d, e, f);
}

static inline void Kibombo_SePlay(s32 cue) { Sound_Play(cue); }

void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);

static inline void Kibombo_FadeTo(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

#define KIBOMBO_TASK_ADD(func, priority)                                       \
  {                                                                            \
    s32 priority_ = (priority);                                                \
    Task_Add((func), priority_);                                               \
  }

static inline void Kibombo_ObjEmote(s32 actor, s32 emote, s32 frames) {
  Actor_ShowEmoteById(actor, emote, frames);
}

static inline void Kibombo_ObjEmoteWait(s32 actor, s32 emote) {
  FieldEffect_SetAttached(actor, emote);
}

static inline void Kibombo_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

static inline void Kibombo_MessagePut(s32 speaker, s32 flags) {
  Event_ShowMessageAndWait(speaker, flags);
}

static inline void Kibombo_MessageOpen(s32 speaker, s32 flags) {
  FieldMessage_ShowAtActor(speaker, flags);
}

// ---- this overlay

void Kibombo_InitializeVillage(void);
s32 Kibombo_ClampColor(s32 value);
void Kibombo_FlashSprite(struct Actor *object);
void Kibombo_DigFirstMarking(void);
void Kibombo_DigSecondMarking(void);
void Kibombo_DigThirdMarking(void);
void Kibombo_DigFourthMarking(void);
void Kibombo_PiersHasForgottenParty(void);
void Kibombo_PiersTakesPartyForEnemies(void);
void Kibombo_AkafubuOffersCrystal(void);
void Kibombo_PiersJoinsParty(void);
void Kibombo_FlickerColor(s32 index, s32 entry, s32 delta);
void Kibombo_FlickerRedderColor(s32 index, s32 entry, s32 delta);
void Kibombo_FlickerVillageFires(void);
void Kibombo_FlickerCeremonyFires(void);
void Kibombo_FlickerStatueFires(void);
void Kibombo_RaiseEmbers(void);
void Kibombo_PuffSmoke(void);
void Kibombo_LightFlameRow(void);
void Actor_Hide(s32 id);
void Scene_RunUnlessFlag(void);
void Kibombo_AkafubuChants(void);
void Kibombo_SetStatueMouth(s32 open, s32 hide);
void Kibombo_SetStatueTongue(s32 open, s32 mode);
void Kibombo_StopFireFlicker(void);
void Kibombo_DropIntoHole(s32 x, s32 z);
void Kibombo_OpenCeremonyScene(void);
void Kibombo_CrowdCheers(void);

extern s32 gKibombo_EnterDoorScript[];
extern u32 gKibombo_KradenMeetsPiersScript[];
extern u32 gKibombo_JennaMeetsPiersScript[];
extern s32 gKibombo_ShebaMeetsPiersScript[];
extern u32 gKibombo_PartyGathersScript[];
extern s32 gKibombo_JawsSettleScript[];
extern s32 gKibombo_JawsOpenScript[];
extern s16 gKibombo_FrozenPuddle[];
extern s16 gKibombo_FireColors[];
extern struct Actor *gKibombo_BlackCrystal;

#endif
