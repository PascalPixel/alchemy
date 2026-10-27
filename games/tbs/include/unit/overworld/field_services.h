#ifndef ALCHEMY_UNIT_OVERWORLD_FIELD_SERVICES_H
#define ALCHEMY_UNIT_OVERWORLD_FIELD_SERVICES_H

#include "types.h"
#include "field_effect_sprite.h"
#include "unit/overworld/overworld.h"

struct Sprite;
struct Trigger;
struct ActorTemplate;
union ActorOrEffect;

/// A step of the current Psynergy effect (Field_DispatchTypeHandler).
enum PsynergyEffectStep {
  PSYNERGY_STEP_UNTARGETED = 0, ///< casts it with no target (FieldEffect_Run)
  /// Casts it on its target, secondObjectId (FieldEffect_DispatchRequestKind).
  PSYNERGY_STEP_ON_TARGET = 1,
  PSYNERGY_STEP_FINISH = 2, ///< finishes it
  PSYNERGY_STEP_EXTRA = 3   ///< runs its kind's extra handler
};

/// A field Psynergy's effect, as the field kind: the type column of
/// games/tbs/src/game/abilities.tsv, named for the abilities that have it. The
/// field effect keeps it as its animation.
enum FieldPsynergy {
  FIELD_PSYNERGY_MOVE = 1,
  FIELD_PSYNERGY_MIND_READ = 2,
  FIELD_PSYNERGY_PLY = 3,
  FIELD_PSYNERGY_FORCE = 4,
  FIELD_PSYNERGY_DOUSE = 5,
  FIELD_PSYNERGY_FROST = 6,
  FIELD_PSYNERGY_LIFT = 7,
  FIELD_PSYNERGY_REVEAL = 8,
  FIELD_PSYNERGY_HALT = 9,
  FIELD_PSYNERGY_CLOAK = 10,
  FIELD_PSYNERGY_CARRY = 11,
  FIELD_PSYNERGY_GROWTH = 12,
  FIELD_PSYNERGY_CATCH = 13,
  /// Whirlwind and Gale.
  FIELD_PSYNERGY_WHIRLWIND = 14,
  FIELD_PSYNERGY_RETREAT = 15,
  FIELD_PSYNERGY_AVOID = 16
};

/// One frame of a sparkle: its height offset and tile (fxdata.c).
struct SparkleFrame {
  s16 dy;
  u16 tile;
};

extern const struct SparkleFrame gFieldEffect_SparkleFrames[32];

void FieldScript_UpdateDebugSkip(void);
void FieldScript_WaitFrames(s32 frames);
void Leader_StopMoving(void);
void FieldScript_Start(void);
void FieldScript_End(void);
void FieldScript_NoOpHookA(void);
void FieldScript_NoOpHook(void);
void Event_RunActorUpdateAndWait(struct ActorTemplate *table);
void Event_CallWithLastActiveActorId(struct ActorTemplate *table);
void FieldScript_SetTriggers(const struct Trigger *triggers);
void Event_PrepareActorAndApplyValue(s32 unit, s32 announce);
void Event_PrepareTwoActorsAndApply(s32 first, s32 second);
s32 FieldScript_ValidatePackedId(u32 shortcut);
void FieldScript_ClearInvalidPackedValues(void);
void Party_RemoveMemberRestored(s32 unit);
s32 Party_CheckItemLimit(s32 item);
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 unit);
void Inventory_NoOpCallback(void);
void Inventory_ReservedNoOp(void);
void Actor_WaitForAnimationEnd(s32 actorId, s32 animation);
s32 FieldScript_AskYesNoWithActorAnimation(s32 actorId, s32 forceBottom);
s32 FieldYesNo_Ask(s32 atBottom);
void Party_LearnAbility(s32 unit, s32 ability);
void FieldScript_StartScreenTransition(void);
void FieldScript_FinishScreenTransition(void);
void FieldScript_WaitTransitionFrames(void);
void FieldScript_SetDestination(s32 sceneId, s32 entrance);
void FieldScript_SetSavedDestination(u16 sceneId, u16 entrance);
void FieldScript_SetExitRequest(u16 request);
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo);
/// Field_RequestDjinnBattle's flag: the Djinni does not hop first.
#define DJINN_NO_HOP 0x800
void Field_RequestDjinnBattle(s32 flag, s32 no);
void FieldScript_SetNextDestination(s32 sceneId, s32 entrance);
void FieldScript_SetDefeatDestination(s32 sceneId, s32 entrance);
void FieldScript_RequestDestination(u16 sceneId, u16 entrance);
void Scene_QueueMusic(s32 soundId);
void Scene_PlayQueuedMusic(void);

struct Actor;
struct Actor;

struct Actor *Actor_GetById(s32 actorId);
void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit, s32 acceleration);

void Actor_SetMovementTypeById(u32 objectId, s32 action);
void Actor_Stop(u32 objectId);
void Actor_LinkAndSetScript(u32 objectId, u32 linkedObjectId);
void Actor_RefreshSelectorById(u32 objectId);
/// Accepts a built-in action number or script address; waits up to 300 frames.
void Actor_SetMovementTypeAndWaitById(u32 objectId, s32 action);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z);
void Actor_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z);
void Actor_SetPositionAndReset(u32 objectId, s32 x, s32 z);
void Actor_CenterOnColumn(u32 objectId, s32 priority, s32 zOffset);
void Actor_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset, s32 zOffset);
void Actor_OffsetPositionAndReset(u32 objectId, s32 xOffset, s32 zOffset);
void Actor_CommitPositionAndActivate(u32 objectId, s32 xOffset, s32 zOffset);
void Actor_LaunchFromLeader(u32 objectId, s32 xOffset, s32 zOffset, s32 action);
void Actor_WaitForMoveById(u32 objectId);
void Actor_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z);
void Actor_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z);
void Actor_SetModeById(u32 objectId, s32 animation);
void Actor_SetAnimationSpeedById(u32 objectId, s32 steps);
void Actor_WaitForAnimationChange(u32 objectId);
void Actor_SetAnimationAndWait(u32 objectId, s32 animation);
void Actor_NoOp(void);
void Actor_Jump(u32 objectId, s32 speed, s32 frames);
void Actor_StartRepeatedMotion(u32 objectId, s32 count);
void Actor_RunRepeatedMotion(u32 objectId, s32 count);
void FieldEffect_UpdateParticleLinearMotion(union ActorOrEffect *fieldObject);
void FieldEffect_SpawnBurstParticle(struct Actor *source, s32 param);
void Actor_JumpDownLedge(s32 actorId, s32 pose, s32 param);
void Actor_SetAngleToward(s32 firstId, s32 secondId, s32 wait);
void Actor_LinkPair(s32 firstId, s32 secondId, s32 wait);
void Actor_TurnPairToFaceEachOther(struct Actor *first, struct Actor *second);
void Actor_ReleaseById(s32 index);
void Actor_ReleaseNoOp(void);
void Actor_ConfigureSpriteColor(s32 objectId, s32 param);
void Actor_UpdateBlink(union ActorOrEffect *fieldObject);
void Actor_SetSpriteColor(struct Actor *actor, s32 param);
void Actor_SetTargetAndCallback(u32 objectId, s32 targetId, const void *script);
s32 Actor_StepAngle(struct Actor *object);
void Actor_ResetTargetAndSetMode1(u32 objectId);
void Actor_ArmCallback(s32 objectId, s32 angle, s32 wait);
void Actor_SetSpritePriority(s32 objectId, s32 priority);
void Actor_CopyGraphics(u32 targetId, u32 sourceId);
void Actor_ReservedNoOp(void);

struct Window;

void FieldScript_SetMessageId(s32 message);
s32 Actor_ReadActiveValue(s32 actorId);
s32 Actor_FindActiveByValue(s32 sprite);
struct Window *FieldMessage_ShowAtActor(s32 actorId, s32 unused);
void FieldScript_ShowMessageAndWait(s32 actorId, s32 flags);
void FieldScript_ShowMessageThenWait(s32 actorId, s32 flags, s32 wait);
s32 FieldScript_AskYesNoAndShowResponse(s32 actorId, s32 flags);
void FieldScript_EmptyCallback(void);
void FieldScript_ShowSpeakerMessage(s32 speaker);
void FieldScript_ForwardShowMessageAtPosition(s32 unused0, s32 unused1, s32 x,
                                              s32 y);
void FieldScript_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void Actor_RunIfActive(s32 actorId);
void FieldScript_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                        s32 firstPortraitX, s32 firstPortraitY,
                                        s32 second, s32 secondX, s32 secondY,
                                        s32 secondPortraitX,
                                        s32 secondPortraitY);
void Message_SetVoiceFromActor(s32 actorId);

void FieldCamera_FollowActor(s32 actorId, s32 keepPosition);
void FieldCamera_SetSpeedParameters(s32 speedLimit, s32 acceleration);
void FieldCamera_MoveWithinMapBounds(s32 requestedX, s32 requestedY,
                                     s32 requestedZ, s32 glide);
void Actor_PlaceCurrentWithinCameraBounds(s32 actorId, s32 glide);
void FieldCamera_WaitForArrival(void);
struct Actor *FieldCamera_GetActor(void);
void FieldCamera_Follow(struct Actor *target, s32 keepCurrentPosition);
void MapGrid_SetCameraBounds(s32 minX, s32 minY, s32 maxX, s32 maxY);
#ifdef UBFIX
void FieldEffect_StepRatioTransition(void);
#else
s32 FieldEffect_StepRatioTransition(void);
#endif
void FieldEffect_ScheduleRatioTransition(s32 zoom, s32 frames);
void FieldScript_WaitForZoomTransition(void);
#ifdef UBFIX
void FieldEffect_CopyLinkedActorPosition(union ActorOrEffect *fieldObject);
#else
s32 FieldEffect_CopyLinkedActorPosition(union ActorOrEffect *fieldObject);
#endif
void FieldEffect_SpawnAttached(s32 actorId, s32 emote, s32 wait);
/// FieldEffect_SpawnAttached's emote flag: the bubble takes sprite priority 1
/// instead of its actor's. The low four bits pick the bubble (enum
/// Emote).
#define EMOTE_IN_FRONT 0x100

/// The bubbles FieldEffect_SpawnAttached can show, by the animation they pick
/// in the bubble's sprite sheet (SPR_021).
enum Emote {
  EMOTE_EXCLAMATION = 0, ///< "!"
  EMOTE_QUESTION = 1,    ///< "?"
  /// A wincing face in English; a sweat drop in Japanese and a frown in
  /// Italian.
  EMOTE_WINCE = 2,
  EMOTE_ANGER_VEIN = 3, ///< the cross-shaped vein of anger
  EMOTE_HEART = 4,      ///< a red heart
  EMOTE_ELLIPSIS = 5,   ///< "..."
  EMOTE_LIGHT_BULB = 6, ///< a lit bulb, with SE_CHIME
  EMOTE_SCOWL = 7,      ///< a scowling face
  EMOTE_SMILE = 8,      ///< a smiling face
  /// A sweat drop; no map shows it, and the Japanese sheet has no tenth
  /// bubble.
  EMOTE_SWEAT_DROP = 9
};
/// FieldEffect_SetAttached's flags: a mode in the low two bits, and
/// ATTACHED_EFFECT_IN_FRONT.
enum AttachedEffectFlag {
  ATTACHED_EFFECT_REMOVE = 0, ///< removes the attached object
  ATTACHED_EFFECT_ATTACH = 1, ///< attaches one that stays until removed
  ATTACHED_EFFECT_ONCE = 2,   ///< shows a one-off one that ends by itself
  /// The object takes sprite priority 1, not the actor's.
  ATTACHED_EFFECT_IN_FRONT = 0x100
};
void FieldEffect_SetAttached(s32 actorId, s32 flags);
s32 Actor_ClearChildAndCollisionFlags(void *obj);
s32 Actor_MoveTowardTarget(struct Actor *object);
s32 Actor_TurnTowardLinkedActor(struct Actor *object);
void Actor_SetMovementType(struct Actor *object, s32 action);
struct Actor *Actor_FindNearestFacingTarget(struct Actor *self, s32 sprite);
s32 Field_TryJumpForward(void);
s32 Field_ClimbDown(void);
s32 Field_ClimbUp(void);
s32 Actor_GetScreenPosition(s32 objectId, s32 *position);
void Exit_NoOp(void);
#ifdef UBFIX
void Field_StartEncounterTransition(s32 unused);
#else
s32 Field_StartEncounterTransition(s32 unused);
#endif
void Exit_PrepareAnimation(s32 animation);
void Exit_StartStairsUp(void);
void Exit_StartStairsDown(void);
void Entrance_EndArrivalAnimation(s32 animation);
s32 Entrance_PlayArrival(void);

void ScanlineEffect_ArmDma(void);
void ScanlineEffect_BuildPage(void);
void ScanlineEffect_Stop(void);
void Weather_UpdateRain(void);
void Weather_UpdateLightning(void);
void Weather_StartRain(void);
void FieldMotes_Update(void);
void FieldMotes_Start(void);
void FieldEffect_UpdateSparkles(void);
void FieldEffect_InitializeSparkles(void);
void Weather_StartLightning(void);
void Weather_StopLightning(void);
void Weather_LastLightning(void);
void Weather_ResumeLightning(void);
void Weather_StartLightningFlash(s32 from, s32 to);

struct FieldEffectSprite;

void Actor_MoveHalfwayTowardTarget(union ActorOrEffect *fieldObject);
void Djinn_RunBattleStartHop(s32 djinniId);
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn);
void Actor_PlaceMapActors(void);
// Declared without a prototype, as SceneManager_Run (overworld.c) calls it with
// one argument and with two. overworld.c comes before this unit in ROM order,
// so in one file the calls would still come before the definition.
void Djinn_ResolvePending();
void FieldEffect_UpdateAllSlots(void);
void FieldEffect_InitializeSlots(void);
void FieldEffect_ClearActiveSlotsAndScheduleUpdates(void);
void FieldEffect_AdvanceSpinAngle(union ActorOrEffect *fieldObject);
void FieldEffect_RunAngledApproachPhases(struct FieldEffectSprite *particle);
void Djinn_RunJupiterJoinEffect(s32 djinniId);
void FieldEffect_SetActorAlternatingWords(union ActorOrEffect *fieldObject);
void FieldEffect_AdvanceAngleWithRamp(union ActorOrEffect *fieldObject);
void FieldEffect_UpdatePhasedRadialParticle(struct FieldEffectSprite *effect);
void Djinn_RunMarsJoinEffect(s32 djinniId);
void FieldEffect_UpdateDescendingOrbitActor(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateRadialSpread(struct FieldEffectSprite *effect);
void FieldEffect_RunVenusDjinnCapture(s32 djinniId);
void FieldEffect_UpdateRandomTargetParticle(struct FieldEffectSprite *effect);
void FieldEffect_HalveDistanceToTarget(union ActorOrEffect *fieldObject);
void FieldEffect_RunMercuryDjinnCapture(s32 djinniId);

struct Vector3;

void Field_DispatchTypeHandler(s32 kind);
void FieldEffect_Run(void);
void FieldEffect_DispatchRequestKind(void);
void FieldEffect_FinishMindRead(void);
void FieldScript_RunTypeHandler(void);
s32 Field_RunEventAction(const void *event, s32 param, s32 target);
void Actor_CycleSpriteColors(union ActorOrEffect *fieldObject);
void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                               s32 magnitude, s32 angle);
s32 VramCache_CountFreeBlocks(void);
struct Sprite *Sprite_ShareVramCacheEntry(struct Sprite *sourceSprite,
                                          struct Sprite *sharedSprite);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetOthersPaused(struct Actor *except, s32 state, s32 value);
void FieldEffect_UpdateScaledArcActorA(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateScaledArcActorB(union ActorOrEffect *fieldObject);
void FieldEffect_SpawnScaledArcActors(struct Actor *linked);
void FieldEffect_FlickerActorAndTick(union ActorOrEffect *fieldObject);
void FieldEffect_CycleActorValueByCounter(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateAllEffectSlots(void);
void FieldEffect_LoadActionEffectResources(s32 abilityId, s32 mode);
void FieldEffect_SetupActorPair(s32 userId, s32 targetId);
void FieldEffect_StopCasterHook(void);
void FieldEffect_End(void);
void FieldEffect_StartCast(void);
void FieldEffect_FadeToEffectColor(void);
void FieldEffect_FadeFromEffectColor(void);
void Camera_WorldToScreen(struct Vector3 *position);

struct ActorDouseView;
struct OrbitEffect;

void FieldEffect_StartMindReadWave(s32 x, s32 y);
void FieldEffect_FinishWaveAndReleaseHeapBlock(void);
void FieldEffect_UpdateMindReadWave(void);
void FieldEffect_ArmBg0HBlankDma(void);
void FieldEffect_AdvanceHueCycle(void);
void FieldEffect_CalculateHueChannels(s32 hue, s32 *r, s32 *g, s32 *b);
s32 FieldEffect_HueChannelRamp(s32 angle, s32 low, s32 high);
s32 FixedPoint_Remainder(s32 value, s32 divisor);
void FieldEffect_SetCallbackWhenTargetUnset(union ActorOrEffect *fieldObject);
void FieldEffect_GreyMindReadPalette(void);
void FieldEffect_RestoreMindReadPalette(void);
s32 Input_GetDpadAngle(u32 unused);
void FieldEffect_UpdateItemBreakFragment(union ActorOrEffect *fieldObject);
void FieldEffect_SteerPushedTarget(s32 targetId);
void FieldEffect_UpdateOrbitAndReturn(struct FieldEffectSprite *effect);
void FieldEffect_RunMove(void);
void *FieldEffect_StartItemBreak(void *object);
void FieldEffect_SnapScaleToFull(struct Actor *object);
void UpdateRisingParticleBurst(void *object);
void Actor_SetRevealSpriteColor(s32 value);
void FieldEffect_WatchLeaderDistance(void);
void FieldEffect_RunReveal(void);
void FieldEffect_EndReveal(void);
void FieldEffect_RunGrowthOnTarget(s32 unused);
void FieldEffect_RunGrowth(void);
void FieldEffect_SpawnBurstParticleField(void);
void FieldEffect_RunLiftOnTarget(s32 unused);
struct Actor *FieldEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void FieldEffect_UpdateRadialBurst(struct FieldEffectSprite *effect);
s32 SpawnHeavyImpactEffect(struct Actor *interpreter);
void FieldEffect_RunForceOnTarget(s32 unused);
void FieldEffect_RunForce(void);
void FieldEffect_UpdateOrbitingParticleMain(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateOrbitingParticleFade(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateOrbitingParticleLeft(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateOrbitingParticleRight(union ActorOrEffect *fieldObject);
void FieldEffect_RunFrostOnTarget(s32 unused);
void FieldEffect_RunFrost(void);
void FieldEffect_SwayParticle(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateShrinkingOrbitActor(union ActorOrEffect *fieldObject);
void FieldEffect_RunSparkEmitter(union ActorOrEffect *fieldObject);
void FieldEffect_RunPlyOnTarget(s32 unused);
void FieldEffect_RunPly(void);
void FieldEffect_CheckCloakTile(void);
void FieldEffect_EndCloak(void);
void FieldEffect_ResumeCloakCheck(void);
void FieldEffect_StartCloak(void);
void FieldEffect_SpawnRandomAngleTriplet(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateDriftingFallActor(union ActorOrEffect *fieldObject);
void FieldEffect_RunDouseOnTarget(s32 unused);
void FieldEffect_RunDouse(void);
void FieldEffect_SpawnFallingParticles(void);
void FieldEffect_SteerLiftedTarget(s32 targetId);
struct Actor *FieldEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void FieldEffect_UpdateParticleMotionAndScale(union ActorOrEffect *fieldObject);
void FieldEffect_SpawnRadialParticleRing(struct Actor *origin);
void FieldEffect_WanderAroundAnchor(union ActorOrEffect *fieldObject);
void FieldEffect_CircleAnchor(union ActorOrEffect *fieldObject);
void FieldEffect_ShrinkActorScaleUntilHalf(union ActorOrEffect *fieldObject);
void FieldEffect_RunWhirlwindOnTarget(s32 unused);
void FieldEffect_RunWhirlwind(void);
void FieldEffect_UpdateRadialCamera(struct FieldEffectSprite *effect);
void FieldEffect_RunHaltOnTarget(s32 unused);
void FieldEffect_RunHalt(void);
void FieldEffect_UpdatePausedActor(union ActorOrEffect *fieldObject);
void FieldEffect_PauseActor(s32 actorId);
void FieldEffect_ResumeActor(s32 actorId);
s32 FieldEffect_FilterActorIdByFlags(s32 id);
void FieldEffect_RunCatchOnTarget(s32 unused);
void FieldEffect_RunCatch(void);
void FieldEffect_UpdateSpinAngle(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateDescendingActor(union ActorOrEffect *fieldObject);
void FieldEffect_UpdateRadialLaunch(struct FieldEffectSprite *effect);
void FieldEffect_RunRetreat(void);
void FieldEffect_UpdateDescendingParticlePositiveArc(
    union ActorOrEffect *fieldObject);
void FieldEffect_UpdateDescendingParticleNegativeArc(
    union ActorOrEffect *fieldObject);
void FieldEffect_SpawnDescendingArcParticles(struct Actor *arc);
void FieldEffect_BlinkAvoidCaster(void);
void FieldEffect_UpdatePairedArcSpawner(union ActorOrEffect *fieldObject);
void FieldScript_ShowStatusMessage(void);
void FieldEffect_RunAvoid(void);

void FieldEffectSprite_Update(struct FieldEffectSprite *effect);
void FieldEffect_DrawScaledActor(struct FieldEffectSprite *object);
void FieldEffectSprite_UpdateMotion(struct FieldEffectSprite *effect);
u32 FieldEffectSprite_HasPendingTarget(struct FieldEffectSprite *effect);
void FieldEffectSprite_SetPosition(struct FieldEffectSprite *effect, s32 x,
                                   s32 z);
void FieldEffectSprite_SetActorMode(struct FieldEffectSprite *effect, s32 mode);
void FieldEffectSprite_SetCallback(struct FieldEffectSprite *effect,
                                   EffectCallback callback);
void FieldEffectSprite_Initialize(struct FieldEffectSprite *effect, s32 kind,
                                  s32 x, s32 z);
void FieldEffect_ClearOwnedSlot(struct FieldEffectSprite *effect);

void FieldEffect_SetupResourcesAndWindow(void);
void FieldEffect_CleanupResourcesAndWindow(void);
void WorldPicture_UpdateMarkers(void);
void WorldPicture_Show(s32 picture);
void Field_UpdateActorVisibilityBounds(void);
void FieldEffect_RunVisibilityTransition(void);

#endif
