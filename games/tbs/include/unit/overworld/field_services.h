#ifndef ALCHEMY_UNIT_OVERWORLD_FIELD_SERVICES_H
#define ALCHEMY_UNIT_OVERWORLD_FIELD_SERVICES_H

#include "types.h"
#include "field_effect_sprite.h"
#include "unit/overworld/overworld.h"

struct Sprite;
struct Trigger;
struct ActorTemplate;
struct Actor;

/// A step of the current Psynergy effect (FieldPsynergy_RunHandler).
enum FieldPsynergyHandler {
  FIELD_PSYNERGY_HANDLER_UNTARGETED =
      0, ///< casts it with no target (FieldPsynergy_Run)
  /// Casts it on its target, secondObjectId (FieldPsynergy_Dispatch).
  FIELD_PSYNERGY_HANDLER_ON_TARGET = 1,
  FIELD_PSYNERGY_HANDLER_FINISH = 2, ///< finishes it
  FIELD_PSYNERGY_HANDLER_EXTRA = 3   ///< runs its kind's extra handler
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

void Event_UpdateDebugSkip(void);
void Event_WaitFrames(s32 frames);
void Leader_StopMoving(void);
void Event_Start(void);
void Event_End(void);
void Event_NoOpUpdateA(void);
void Event_NoOpUpdate(void);
void Event_RunActorUpdateAndWait(struct ActorTemplate *table);
void Event_CallWithLastActiveActorId(struct ActorTemplate *table);
void Event_SetTriggers(const struct Trigger *triggers);
void Event_PrepareActorAndApplyValue(s32 unit, s32 announce);
void Event_PrepareTwoActorsAndApply(s32 first, s32 second);
s32 Event_ValidatePackedId(u32 shortcut);
void Event_ClearInvalidPackedValues(void);
void Party_RemoveMemberRestored(s32 unit);
s32 Party_CheckItemLimit(s32 item);
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 unit);
void Inventory_NoOpCallback(void);
void Inventory_ReservedNoOp(void);
void Actor_WaitForAnimationEnd(s32 actorId, s32 animation);
s32 Event_AskYesNoWithActorAnimation(s32 actorId, s32 forceBottom);
s32 FieldYesNo_Ask(s32 atBottom);
void Party_LearnAbility(s32 unit, s32 ability);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_SetDestination(s32 sceneId, s32 entrance);
void Event_SetRestartPoint(u16 sceneId, u16 entrance);
void Event_SetExitRequest(u16 request);
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo);
/// Field_RequestDjinnBattle's flag: the Djinni does not hop first.
#define DJINN_NO_HOP 0x800
void Field_RequestDjinnBattle(s32 flag, s32 no);
void Event_SetAfterBattleDestination(s32 sceneId, s32 entrance);
void Event_SetDefeatDestination(s32 sceneId, s32 entrance);
void Event_LeaveFor(u16 sceneId, u16 entrance);
void Scene_QueueMusic(s32 soundId);
void Scene_PlayQueuedMusic(void);

struct Actor;
struct Actor;

struct Actor *Actor_GetById(s32 actorId);
void Actor_SetSpeedParameters(u32 actorId, s32 speedLimit, s32 acceleration);

void Actor_SetMovementTypeById(u32 actorId, s32 action);
void Actor_Stop(u32 actorId);
void Actor_LinkAndSetScript(u32 actorId, u32 linkedActorId);
void Actor_RefreshSelectorById(u32 actorId);
/// Accepts a built-in action number or script address; waits up to 300 frames.
void Actor_SetMovementTypeAndWaitById(u32 actorId, s32 action);
void Actor_ResetAndSetPosition(u32 actorId, s32 x, s32 z);
void Actor_SetPositionAndCommit(u32 actorId, s32 x, s32 z);
void Actor_ResetAndSetPositionInMode2(u32 actorId, s32 x, s32 z);
void Actor_SetPositionAndReset(u32 actorId, s32 x, s32 z);
void Actor_CenterOnColumn(u32 actorId, s32 priority, s32 zOffset);
void Actor_OffsetPositionAndResetMotion(u32 actorId, s32 xOffset, s32 zOffset);
void Actor_OffsetPositionAndReset(u32 actorId, s32 xOffset, s32 zOffset);
void Actor_CommitPositionAndActivate(u32 actorId, s32 xOffset, s32 zOffset);
void Actor_LaunchFromLeader(u32 actorId, s32 xOffset, s32 zOffset, s32 action);
void Actor_WaitForMoveById(u32 actorId);
void Actor_SetHorizontalPositionWithTerrain(u32 actorId, s32 x, s32 z);
void Actor_SetPositionWithTerrain(u32 actorId, s32 x, s32 y, s32 z);
void Actor_SetModeById(u32 actorId, s32 animation);
void Actor_SetAnimationSpeedById(u32 actorId, s32 steps);
void Actor_WaitForAnimationChange(u32 actorId);
void Actor_SetAnimationAndWait(u32 actorId, s32 animation);
void Actor_NoOp(void);
void Actor_Jump(u32 actorId, s32 speed, s32 frames);
void Actor_StartRepeatedMotion(u32 actorId, s32 count);
void Actor_RunRepeatedMotion(u32 actorId, s32 count);
void FieldEffect_UpdateParticleLinearMotion(struct Actor *actor);
void FieldEffect_SpawnBurstParticle(struct Actor *source, s32 param);
void Actor_JumpDownLedge(s32 actorId, s32 pose, s32 param);
void Actor_SetAngleToward(s32 firstId, s32 secondId, s32 wait);
void Actor_LinkPair(s32 firstId, s32 secondId, s32 wait);
void Actor_TurnPairToFaceEachOther(struct Actor *first, struct Actor *second);
void Actor_ReleaseById(s32 index);
void Actor_ReleaseNoOp(void);
void Actor_ConfigureSpriteColor(s32 actorId, s32 param);
void Actor_UpdateBlink(struct Actor *actor);
void Actor_SetSpriteColor(struct Actor *actor, s32 param);
void Actor_SetTargetAndCallback(u32 actorId, s32 targetId, const void *script);
s32 Actor_StepAngle(struct Actor *actor);
void Actor_ResetTargetAndSetMode1(u32 actorId);
void Actor_ArmCallback(s32 actorId, s32 angle, s32 wait);
void Actor_SetSpritePriority(s32 actorId, s32 priority);
void Actor_CopyGraphics(u32 targetId, u32 sourceId);
void Actor_ReservedNoOp(void);

struct Window;

void Event_SetMessageId(s32 message);
s32 Actor_ReadActiveValue(s32 actorId);
s32 Actor_FindActiveByValue(s32 sprite);
struct Window *FieldMessage_ShowAtActor(s32 actorId, s32 unused);
void Event_ShowMessageAndWait(s32 actorId, s32 flags);
void Event_ShowMessageThenWait(s32 actorId, s32 flags, s32 wait);
s32 Event_AskYesNoAndShowResponse(s32 actorId, s32 flags);
void Event_EmptyCallback(void);
void Event_ShowSpeakerMessage(s32 speaker);
void Event_ForwardShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void Actor_RunIfActive(s32 actorId);
void Event_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                  s32 firstPortraitX, s32 firstPortraitY,
                                  s32 second, s32 secondX, s32 secondY,
                                  s32 secondPortraitX, s32 secondPortraitY);
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
void Event_WaitForZoomTransition(void);
#ifdef UBFIX
void FieldEffect_CopyLinkedActorPosition(struct Actor *actor);
#else
s32 FieldEffect_CopyLinkedActorPosition(struct Actor *actor);
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
s32 Actor_MoveTowardTarget(struct Actor *actor);
s32 Actor_TurnTowardLinkedActor(struct Actor *actor);
void Actor_SetMovementType(struct Actor *actor, s32 action);
struct Actor *Actor_FindNearestFacingTarget(struct Actor *self, s32 sprite);
s32 Field_TryJumpForward(void);
s32 Field_ClimbDown(void);
s32 Field_ClimbUp(void);
s32 Actor_GetScreenPosition(s32 actorId, s32 *position);
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

struct FieldParticle;

void Actor_MoveHalfwayTowardTarget(struct Actor *actor);
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
void FieldEffect_AdvanceSpinAngle(struct Actor *actor);
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle);
void Djinn_RunJupiterJoinEffect(s32 djinniId);
void FieldEffect_SetActorAlternatingWords(struct Actor *actor);
void FieldEffect_AdvanceAngleWithRamp(struct Actor *actor);
void FieldEffect_UpdatePhasedRadialParticle(struct FieldParticle *effect);
void Djinn_RunMarsJoinEffect(s32 djinniId);
void FieldEffect_UpdateDescendingOrbitActor(struct Actor *actor);
void FieldEffect_UpdateRadialSpread(struct FieldParticle *effect);
void FieldEffect_RunVenusDjinnCapture(s32 djinniId);
void FieldEffect_UpdateRandomTargetParticle(struct FieldParticle *effect);
void FieldEffect_HalveDistanceToTarget(struct Actor *actor);
void FieldEffect_RunMercuryDjinnCapture(s32 djinniId);

struct Vector3;

void FieldPsynergy_RunHandler(s32 kind);
void FieldPsynergy_Run(void);
void FieldPsynergy_Dispatch(void);
void FieldPsynergy_FinishMindRead(void);
void Event_RunFieldPsynergy(void);
s32 Field_RunPsynergyEvent(const void *event, s32 param, s32 target);
void Actor_CycleSpriteColors(struct Actor *actor);
void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *actor,
                                               s32 magnitude, s32 angle);
s32 VramCache_CountFreeBlocks(void);
struct Sprite *Sprite_ShareVramCacheEntry(struct Sprite *sourceSprite,
                                          struct Sprite *sharedSprite);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetOthersPaused(struct Actor *except, s32 state, s32 value);
void FieldEffect_UpdateScaledArcActorA(struct Actor *actor);
void FieldEffect_UpdateScaledArcActorB(struct Actor *actor);
void FieldEffect_SpawnScaledArcActors(struct Actor *linked);
void FieldEffect_FlickerActorAndTick(struct Actor *actor);
void FieldEffect_CycleActorValueByCounter(struct Actor *actor);
void FieldEffect_UpdateAllEffectSlots(void);
void FieldPsynergy_LoadResources(s32 abilityId, s32 mode);
void FieldPsynergy_SetupActorPair(s32 userId, s32 targetId);
void FieldPsynergy_StopCasterUpdate(void);
void FieldPsynergy_End(void);
void FieldPsynergy_StartCast(void);
void FieldPsynergy_FadeToCastColor(void);
void FieldPsynergy_FadeFromCastColor(void);
void Camera_WorldToScreen(struct Vector3 *position);

struct ActorDouseView;
struct OrbitEffect;

void FieldPsynergy_StartMindReadWave(s32 x, s32 y);
void FieldPsynergy_FinishWaveAndReleaseHeapBlock(void);
void FieldPsynergy_UpdateMindReadWave(void);
void FieldEffect_ArmBg0HBlankDma(void);
void FieldEffect_AdvanceHueCycle(void);
void FieldEffect_CalculateHueChannels(s32 hue, s32 *r, s32 *g, s32 *b);
s32 FieldEffect_HueChannelRamp(s32 angle, s32 low, s32 high);
s32 FixedPoint_Remainder(s32 value, s32 divisor);
void FieldEffect_SetCallbackWhenTargetUnset(struct Actor *actor);
void FieldPsynergy_GreyMindReadPalette(void);
void FieldPsynergy_RestoreMindReadPalette(void);
s32 Input_GetDpadAngle(u32 unused);
void FieldEffect_UpdateItemBreakFragment(struct Actor *actor);
void FieldPsynergy_SteerPushedTarget(s32 targetId);
void FieldEffect_UpdateOrbitAndReturn(struct FieldParticle *effect);
void FieldPsynergy_Move(void);
void *FieldEffect_StartItemBreak(void *object);
void FieldEffect_SnapScaleToFull(struct Actor *actor);
void UpdateRisingParticleBurst(void *object);
void Actor_SetRevealSpriteColor(s32 value);
void FieldPsynergy_WatchLeaderDistance(void);
void FieldPsynergy_Reveal(void);
void FieldPsynergy_EndReveal(void);
void FieldPsynergy_GrowthOnTarget(s32 unused);
void FieldPsynergy_Growth(void);
void FieldEffect_SpawnBurstParticleField(void);
void FieldPsynergy_LiftOnTarget(s32 unused);
struct Actor *FieldEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void FieldEffect_UpdateRadialBurst(struct FieldParticle *effect);
s32 SpawnHeavyImpactEffect(struct Actor *interpreter);
void FieldPsynergy_ForceOnTarget(s32 unused);
void FieldPsynergy_Force(void);
void FieldEffect_UpdateOrbitingParticleMain(struct Actor *actor);
void FieldEffect_UpdateOrbitingParticleFade(struct Actor *actor);
void FieldEffect_UpdateOrbitingParticleLeft(struct Actor *actor);
void FieldEffect_UpdateOrbitingParticleRight(struct Actor *actor);
void FieldPsynergy_FrostOnTarget(s32 unused);
void FieldPsynergy_Frost(void);
void FieldEffect_SwayParticle(struct Actor *actor);
void FieldEffect_UpdateShrinkingOrbitActor(struct Actor *actor);
void FieldEffect_RunSparkEmitter(struct Actor *actor);
void FieldPsynergy_PlyOnTarget(s32 unused);
void FieldPsynergy_Ply(void);
void FieldPsynergy_CheckCloakTile(void);
void FieldPsynergy_EndCloak(void);
void FieldPsynergy_ResumeCloakCheck(void);
void FieldPsynergy_StartCloak(void);
void FieldEffect_SpawnRandomAngleTriplet(struct Actor *actor);
void FieldEffect_UpdateDriftingFallActor(struct Actor *actor);
void FieldPsynergy_DouseOnTarget(s32 unused);
void FieldPsynergy_Douse(void);
void FieldEffect_SpawnFallingParticles(void);
void FieldPsynergy_SteerLiftedTarget(s32 targetId);
struct Actor *FieldEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void FieldEffect_UpdateParticleMotionAndScale(struct Actor *actor);
void FieldEffect_SpawnRadialParticleRing(struct Actor *origin);
void FieldEffect_WanderAroundAnchor(struct Actor *actor);
void FieldEffect_CircleAnchor(struct Actor *actor);
void FieldEffect_ShrinkActorScaleUntilHalf(struct Actor *actor);
void FieldPsynergy_WhirlwindOnTarget(s32 unused);
void FieldPsynergy_Whirlwind(void);
void FieldEffect_UpdateRadialCamera(struct FieldParticle *effect);
void FieldPsynergy_HaltOnTarget(s32 unused);
void FieldPsynergy_Halt(void);
void FieldPsynergy_UpdateHaltedActor(struct Actor *actor);
void FieldPsynergy_HaltActor(s32 actorId);
void FieldPsynergy_ResumeHaltedActor(s32 actorId);
s32 FieldEffect_FilterActorIdByFlags(s32 id);
void FieldPsynergy_CatchOnTarget(s32 unused);
void FieldPsynergy_Catch(void);
void FieldEffect_UpdateSpinAngle(struct Actor *actor);
void FieldEffect_UpdateDescendingActor(struct Actor *actor);
void FieldEffect_UpdateRadialLaunch(struct FieldParticle *effect);
void FieldPsynergy_Retreat(void);
void FieldEffect_UpdateDescendingParticlePositiveArc(struct Actor *actor);
void FieldEffect_UpdateDescendingParticleNegativeArc(struct Actor *actor);
void FieldEffect_SpawnDescendingArcParticles(struct Actor *arc);
void FieldEffect_BlinkAvoidCaster(void);
void FieldEffect_UpdatePairedArcSpawner(struct Actor *actor);
void Event_ShowStatusMessage(void);
void FieldPsynergy_Avoid(void);

void FieldParticle_Update(struct FieldParticle *effect);
void FieldEffect_DrawScaledActor(struct FieldParticle *object);
void FieldParticle_UpdateMotion(struct FieldParticle *effect);
u32 FieldParticle_HasPendingTarget(struct FieldParticle *effect);
void FieldParticle_SetPosition(struct FieldParticle *effect, s32 x, s32 z);
void FieldParticle_SetActorMode(struct FieldParticle *effect, s32 mode);
void FieldParticle_SetCallback(struct FieldParticle *effect,
                               EffectCallback callback);
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z);
void FieldEffect_ClearOwnedSlot(struct FieldParticle *effect);

void FieldEffect_SetupResourcesAndWindow(void);
void FieldEffect_CleanupResourcesAndWindow(void);
void WorldPicture_UpdateMarkers(void);
void WorldPicture_Show(s32 picture);
void Field_UpdateActorVisibilityBounds(void);
void FieldEffect_RunVisibilityTransition(void);

#endif
