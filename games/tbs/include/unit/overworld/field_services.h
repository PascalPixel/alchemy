#ifndef ALCHEMY_UNIT_OVERWORLD_FIELD_SERVICES_H
#define ALCHEMY_UNIT_OVERWORLD_FIELD_SERVICES_H

#include "types.h"
#include "field_effect_sprite.h"
#include "unit/overworld/overworld.h"

struct Sprite;
struct MapEvent;
struct ObjectEventTemplate;
union ObjectEventOrEffect;

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
void FieldScript_RunObjectHookAndWait(struct ObjectEventTemplate *table);
void FieldScript_CallWithLastActiveObjectId(struct ObjectEventTemplate *table);
void FieldScript_SetMapEvents(const struct MapEvent *events);
void FieldScript_PrepareObjectAndApplyValue(s32 unit, s32 announce);
void FieldScript_PrepareTwoObjectsAndApply(s32 first, s32 second);
s32 FieldScript_ValidatePackedId(u32 shortcut);
void FieldScript_ClearInvalidPackedValues(void);
void Party_RemoveMemberRestored(s32 unit);
s32 Party_CheckItemLimit(s32 item);
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 Inventory_TryAddAndReturnMember(s32 item, s32 unused, s32 unit);
void Inventory_NoOpCallback(void);
void Inventory_ReservedNoOp(void);
void ObjectEvent_WaitForAnimationEnd(s32 localId, s32 animation);
s32 FieldScript_AskYesNoWithObjectEventAnimation(s32 localId, s32 forceBottom);
s32 FieldYesNo_Ask(s32 atBottom);
void Party_LearnAbility(s32 unit, s32 ability);
void FieldScript_StartScreenTransition(void);
void FieldScript_FinishScreenTransition(void);
void FieldScript_WaitTransitionFrames(void);
void FieldScript_SetDestination(s32 mapId, s32 warpDest);
void FieldScript_SetSavedDestination(u16 mapId, u16 warpDest);
void FieldScript_SetWarpRequest(u16 request);
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo);
/// Field_RequestDjinnBattle's flag: the Djinni does not hop first.
#define DJINN_NO_HOP 0x800
void Field_RequestDjinnBattle(s32 flag, s32 no);
void FieldScript_SetNextDestination(s32 mapId, s32 warpDest);
void FieldScript_SetDefeatDestination(s32 mapId, s32 warpDest);
void FieldScript_RequestDestination(u16 mapId, u16 warpDest);
void Map_QueueMusic(s32 soundId);
void Map_PlayQueuedMusic(void);

struct Object;
struct ObjectEvent;

struct ObjectEvent *ObjectEvent_GetById(s32 localId);
void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration);
// Built-in actions selected by ObjectEvent_SetMovementType.
#define OBJ_EVENT_ACTION_IDLE 1
#define OBJ_EVENT_ACTION_FOLLOW_LINKED_OBJECT 5
#define OBJ_EVENT_ACTION_FACE_SELECTED_OBJ_EVENT 6
/// Clears child and collision flags, then returns to the idle script.
#define OBJ_EVENT_ACTION_RESET 7

void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action);
void ObjectMotion_EnableActionAndResetMotion(u32 objectId);
void ObjectEvent_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId);
void ObjectEvent_RefreshSelectorById(u32 objectId);
/// Accepts a built-in action number or script address; waits up to 300 frames.
void ObjectEvent_SetActionCallbackAndRefreshById(u32 objectId, s32 action);
void ObjectMotion_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(u32 objectId, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionAndReset(u32 objectId, s32 x, s32 z);
void ObjectMotion_CenterOnColumn(u32 objectId, s32 priority, s32 zOffset);
void ObjectMotion_OffsetPositionAndResetMotion(u32 objectId, s32 xOffset,
                                               s32 zOffset);
void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 xOffset,
                                         s32 zOffset);
void ObjectMotion_CommitPositionAndActivate(u32 objectId, s32 xOffset,
                                            s32 zOffset);
void ObjectMotion_LaunchFromFocusedObject(u32 objectId, s32 xOffset,
                                          s32 zOffset, s32 action);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z);
void ObjectEvent_SetModeById(u32 objectId, s32 animation);
void ObjectEvent_SetActionById(u32 objectId, s32 steps);
void ObjectMotion_WaitForAnimationChange(u32 objectId);
void ObjectMotion_SetAnimationAndWait(u32 objectId, s32 animation);
void ObjectMotion_NoOp(void);
void ObjectMotion_Launch(u32 objectId, s32 speed, s32 frames);
void ObjectMotion_SetVariantCallback(u32 objectId, s32 count);
void ObjectMotion_SetVarCbAndRefresh(u32 objectId, s32 count);
void FieldEffect_UpdateParticleLinearMotion(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_SpawnBurstParticle(struct ObjectEvent *source, s32 param);
void ObjectEvent_JumpDownLedge(s32 localId, s32 pose, s32 param);
void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait);
void ObjectEvent_LinkPair(s32 firstId, s32 secondId, s32 wait);
void ObjectEvent_TurnPairToFaceEachOther(struct Object *first,
                                         struct Object *second);
void ObjectEvent_DestroyById(s32 index);
void ObjectEvent_DestroyNoOp(void);
void ObjectEvent_ConfigureSpriteColor(s32 objectId, s32 param);
void ObjectEvent_UpdateBlink(union ObjectEventOrEffect *fieldObject);
void ObjectEvent_SetSpriteColor(struct ObjectEvent *objectEvent, s32 param);
void ObjectEvent_SetTargetAndCallback(u32 objectId, s32 targetId,
                                      const void *script);
s32 ObjectMotion_StepAngle(struct Object *object);
void ObjectEvent_ResetTargetAndSetMode1(u32 objectId);
void ObjectMotion_ArmCallback(s32 objectId, s32 angle, s32 wait);
void ObjectMotion_SetActionVariant(s32 objectId, s32 priority);
void ObjectEvent_CopyGraphics(u32 targetId, u32 sourceId);
void ObjectEvent_ReservedNoOp(void);

struct Window;

void FieldScript_SetMessageId(s32 message);
s32 ObjectEvent_ReadActiveValue(s32 localId);
s32 ObjectEvent_FindActiveByValue(s32 sprite);
struct Window *FieldMessage_ShowAtObject(s32 localId, s32 unused);
void FieldScript_ShowMessageAndWait(s32 localId, s32 flags);
void FieldScript_ShowMessageThenWait(s32 localId, s32 flags, s32 wait);
s32 FieldScript_AskYesNoAndShowResponse(s32 localId, s32 flags);
void FieldScript_EmptyCallback(void);
void FieldScript_ShowSpeakerMessage(s32 speaker);
void FieldScript_ForwardShowMessageAtPosition(s32 unused0, s32 unused1, s32 x,
                                              s32 y);
void FieldScript_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void ObjectEvent_RunIfActive(s32 localId);
void FieldScript_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                        s32 firstPortraitX, s32 firstPortraitY,
                                        s32 second, s32 secondX, s32 secondY,
                                        s32 secondPortraitX,
                                        s32 secondPortraitY);
void Message_SetVoiceFromObjectEvent(s32 localId);

void FieldCamera_FollowObjectEvent(s32 localId, s32 keepPosition);
void FieldCamera_SetSpeedParameters(s32 speedLimit, s32 acceleration);
void FieldCamera_MoveWithinMapBounds(s32 requestedX, s32 requestedY,
                                     s32 requestedZ, s32 glide);
void ObjectEvent_PlaceCurrentWithinCameraBounds(s32 localId, s32 glide);
void FieldCamera_WaitForArrival(void);
struct ObjectEvent *FieldCamera_GetScriptObject(void);
void FieldCamera_FollowObject(struct Object *target, s32 keepCurrentPosition);
void MapGrid_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY);
#ifdef UBFIX
void FieldEffect_StepRatioTransition(void);
#else
s32 FieldEffect_StepRatioTransition(void);
#endif
void FieldEffect_ScheduleRatioTransition(s32 zoom, s32 frames);
void FieldScript_WaitForZoomTransition(void);
#ifdef UBFIX
void FieldEffect_CopyLinkedObjectPosition(
    union ObjectEventOrEffect *fieldObject);
#else
s32 FieldEffect_CopyLinkedObjectPosition(
    union ObjectEventOrEffect *fieldObject);
#endif
void FieldEffect_SpawnLinked(s32 localId, s32 emote, s32 wait);
/// FieldEffect_SpawnLinked's emote flag: the bubble takes sprite priority 1
/// instead of its object event's. The low four bits pick the bubble (enum
/// Emote).
#define EMOTE_IN_FRONT 0x100

/// The bubbles FieldEffect_SpawnLinked can show, by the animation they pick in
/// the bubble's sprite sheet (SPR_021).
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
/// FieldEffect_ConfigureLinkedObject's flags: a mode in the low two bits, and
/// ATTACHED_EFFECT_IN_FRONT.
enum AttachedEffectFlag {
  ATTACHED_EFFECT_REMOVE = 0, ///< removes the attached object
  ATTACHED_EFFECT_ATTACH = 1, ///< attaches one that stays until removed
  ATTACHED_EFFECT_ONCE = 2,   ///< shows a one-off one that ends by itself
  /// The object takes sprite priority 1, not the object event's.
  ATTACHED_EFFECT_IN_FRONT = 0x100
};
void FieldEffect_ConfigureLinkedObject(s32 localId, s32 flags);
s32 Object_ClearChildAndCollisionFlags(void *obj);
s32 ObjectMotion_MoveTowardTarget(struct Object *object);
s32 ObjectMotion_TurnTowardLinkedTarget(struct Object *object);
void ObjectEvent_SetMovementType(struct Object *object, s32 action);
struct Object *Object_FindNearestFacingTarget(struct Object *self, s32 sprite);
s32 Field_TryJumpForward(void);
s32 Field_ClimbDown(void);
s32 Field_ClimbUp(void);
s32 ObjectEvent_GetScreenPosition(s32 objectId, s32 *position);
void Warp_ReservedNoOp941DC(void);
#ifdef UBFIX
void Field_StartEncounterTransition(s32 unused);
#else
s32 Field_StartEncounterTransition(s32 unused);
#endif
void Warp_PrepareExitAnimation(s32 animation);
void Warp_StartStairsUpExit(void);
void Warp_StartStairsDownExit(void);
void Warp_EndArrivalAnimation(s32 animation);
s32 Warp_PlayArrival(void);

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

void ObjectMotion_MoveHalfwayTowardTarget(
    union ObjectEventOrEffect *fieldObject);
void Djinn_RunBattleStartHop(s32 djinniId);
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn);
void ObjectEvent_PlaceMapObjectEvents(void);
// Declared without a prototype, as Map_Run (overworld.c) calls it with
// one argument and with two. overworld.c comes before this unit in ROM order,
// so in one file the calls would still come before the definition.
void Djinn_ResolvePendingEvent();
void FieldEffect_UpdateAllSlots(void);
void FieldEffect_InitializeSlots(void);
void FieldEffect_ClearActiveSlotsAndScheduleUpdates(void);
void FieldEffect_AdvanceSpinAngle(union ObjectEventOrEffect *fieldObject);
void FieldEffect_RunAngledApproachPhases(struct FieldEffectSprite *particle);
void Djinn_RunJupiterJoinEffect(s32 djinniId);
void FieldEffect_SetObjectAlternatingWords(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_AdvanceAngleWithRamp(union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdatePhasedRadialParticle(struct FieldEffectSprite *effect);
void Djinn_RunMarsJoinEffect(s32 djinniId);
void FieldEffect_UpdateDescendingOrbitObject(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateRadialSpread(struct FieldEffectSprite *effect);
void FieldEffect_RunVenusDjinnCapture(s32 djinniId);
void FieldEffect_UpdateRandomTargetParticle(struct FieldEffectSprite *effect);
void FieldEffect_HalveDistanceToTarget(union ObjectEventOrEffect *fieldObject);
void FieldEffect_RunMercuryDjinnCapture(s32 djinniId);

struct Vector3;

void Field_DispatchTypeHandler(s32 kind);
void FieldEffect_Run(void);
void FieldEffect_DispatchRequestKind(void);
void FieldEffect_FinishMindRead(void);
void FieldScript_RunTypeHandler(void);
s32 Field_RunEventAction(const void *event, s32 param, s32 target);
void Object_CycleSpriteColors(union ObjectEventOrEffect *fieldObject);
void ObjectMotion_SetTargetPositionFromMagnitudeAngle(struct Object *object,
                                                      s32 magnitude, s32 angle);
s32 VramCache_CountFreeBlocks(void);
struct Sprite *Sprite_ShareVramCacheEntry(struct Sprite *sourceSprite,
                                          struct Sprite *sharedSprite);
struct Object *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectEvent_SetOthersPaused(struct Object *except, s32 state, s32 value);
void FieldEffect_UpdateScaledArcObjectA(union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateScaledArcObjectB(union ObjectEventOrEffect *fieldObject);
void FieldEffect_SpawnScaledArcObjects(struct ObjectEvent *linked);
void FieldEffect_FlickerObjectAndTick(union ObjectEventOrEffect *fieldObject);
void FieldEffect_CycleObjectValueByCounter(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateAllEffectSlots(void);
void FieldEffect_LoadActionEffectResources(s32 abilityId, s32 mode);
void FieldEffect_SetupObjectPair(s32 userId, s32 targetId);
void FieldEffect_StopCasterHook(void);
void FieldEffect_End(void);
void FieldEffect_StartCast(void);
void FieldEffect_FadeToEffectColor(void);
void FieldEffect_FadeFromEffectColor(void);
void Camera_WorldToScreen(struct Vector3 *position);

struct ObjectEventDouseView;
struct OrbitEffect;

void FieldEffect_StartMindReadWave(s32 x, s32 y);
void FieldEffect_FinishWaveAndReleaseHeapBlock(void);
void FieldEffect_UpdateMindReadWave(void);
void FieldEffect_ArmBg0HBlankDma(void);
void FieldEffect_AdvanceHueCycle(void);
void FieldEffect_CalculateHueChannels(s32 hue, s32 *r, s32 *g, s32 *b);
s32 FieldEffect_HueChannelRamp(s32 angle, s32 low, s32 high);
s32 FixedPoint_Remainder(s32 value, s32 divisor);
void FieldEffect_SetCallbackWhenTargetUnset(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_GreyMindReadPalette(void);
void FieldEffect_RestoreMindReadPalette(void);
s32 Input_GetDpadAngle(u32 unused);
void FieldEffect_UpdateItemBreakFragment(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_SteerPushedTarget(s32 targetId);
void FieldEffect_UpdateOrbitAndReturn(struct FieldEffectSprite *effect);
void FieldEffect_RunMove(void);
void *FieldEffect_StartItemBreak(void *object);
void FieldEffect_SnapScaleToFull(struct Object *object);
void UpdateRisingParticleBurst(void *object);
void Object_SetRevealSpriteColor(s32 value);
void FieldEffect_WatchLeaderDistance(void);
void FieldEffect_RunReveal(void);
void FieldEffect_EndReveal(void);
void FieldEffect_RunGrowthOnTarget(s32 unused);
void FieldEffect_RunGrowth(void);
void FieldEffect_SpawnBurstParticleField(void);
void FieldEffect_RunLiftOnTarget(s32 unused);
struct Object *FieldEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void FieldEffect_UpdateRadialBurst(struct FieldEffectSprite *effect);
s32 SpawnHeavyImpactEffect(struct Object *interpreter);
void FieldEffect_RunForceOnTarget(s32 unused);
void FieldEffect_RunForce(void);
void FieldEffect_UpdateOrbitingParticleMain(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateOrbitingParticleFade(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateOrbitingParticleLeft(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateOrbitingParticleRight(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_RunFrostOnTarget(s32 unused);
void FieldEffect_RunFrost(void);
void FieldEffect_SwayParticle(union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateShrinkingOrbitObject(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_RunSparkEmitter(union ObjectEventOrEffect *fieldObject);
void FieldEffect_RunPlyOnTarget(s32 unused);
void FieldEffect_RunPly(void);
void FieldEffect_CheckCloakTile(void);
void FieldEffect_EndCloak(void);
void FieldEffect_ResumeCloakCheck(void);
void FieldEffect_StartCloak(void);
void FieldEffect_SpawnRandomAngleTriplet(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateDriftingFallObject(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_RunDouseOnTarget(s32 unused);
void FieldEffect_RunDouse(void);
void FieldEffect_SpawnFallingParticles(void);
void FieldEffect_SteerLiftedTarget(s32 targetId);
struct Object *FieldEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
void FieldEffect_UpdateParticleMotionAndScale(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_SpawnRadialParticleRing(struct Object *origin);
void FieldEffect_WanderAroundAnchor(union ObjectEventOrEffect *fieldObject);
void FieldEffect_CircleAnchor(union ObjectEventOrEffect *fieldObject);
void FieldEffect_ShrinkObjectScaleUntilHalf(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_RunWhirlwindOnTarget(s32 unused);
void FieldEffect_RunWhirlwind(void);
void FieldEffect_UpdateRadialCamera(struct FieldEffectSprite *effect);
void FieldEffect_RunHaltOnTarget(s32 unused);
void FieldEffect_RunHalt(void);
void FieldEffect_UpdatePausedObject(union ObjectEventOrEffect *fieldObject);
void FieldEffect_PauseObject(s32 localId);
void FieldEffect_ResumeObject(s32 localId);
s32 FieldEffect_FilterObjectIdByFlags(s32 id);
void FieldEffect_RunCatchOnTarget(s32 unused);
void FieldEffect_RunCatch(void);
void FieldEffect_UpdateSpinAngle(union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateDescendingObject(union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateRadialLaunch(struct FieldEffectSprite *effect);
void FieldEffect_RunRetreat(void);
void FieldEffect_UpdateDescendingParticlePositiveArc(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_UpdateDescendingParticleNegativeArc(
    union ObjectEventOrEffect *fieldObject);
void FieldEffect_SpawnDescendingArcParticles(struct ObjectEvent *arc);
void FieldEffect_BlinkAvoidCaster(void);
void FieldEffect_UpdatePairedArcSpawner(union ObjectEventOrEffect *fieldObject);
void FieldScript_ShowStatusMessage(void);
void FieldEffect_RunAvoid(void);

void FieldEffectSprite_Update(struct FieldEffectSprite *effect);
void FieldEffect_DrawScaledObject(struct FieldEffectSprite *object);
void FieldEffectSprite_UpdateMotion(struct FieldEffectSprite *effect);
u32 FieldEffectSprite_HasPendingTarget(struct FieldEffectSprite *effect);
void FieldEffectSprite_SetPosition(struct FieldEffectSprite *effect, s32 x,
                                   s32 z);
void FieldEffectSprite_SetObjectMode(struct FieldEffectSprite *effect,
                                     s32 mode);
void FieldEffectSprite_SetCallback(struct FieldEffectSprite *effect,
                                   EffectCallback callback);
void FieldEffectSprite_Initialize(struct FieldEffectSprite *effect, s32 kind,
                                  s32 x, s32 z);
void FieldEffect_ClearOwnedSlot(struct FieldEffectSprite *effect);

void FieldEffect_SetupResourcesAndWindow(void);
void FieldEffect_CleanupResourcesAndWindow(void);
void RegionMap_UpdateMarkers(void);
void RegionMap_Show(s32 picture);
void Field_UpdateObjectVisibilityBounds(void);
void FieldEffect_RunVisibilityTransition(void);

#endif
