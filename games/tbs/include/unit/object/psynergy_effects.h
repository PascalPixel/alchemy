#ifndef ALCHEMY_UNIT_OBJECT_PSYNERGY_EFFECTS_H
#define ALCHEMY_UNIT_OBJECT_PSYNERGY_EFFECTS_H

#include "types.h"
#include "effect_slot.h"
#include "unit/object/object.h"

struct AnimationObject;
struct SceneEvent;
struct ScenePlacement;
union FieldObject;

/// A step of the current Psynergy effect (Field_DispatchTypeHandler).
enum PsynergyEffectStep {
  PSYNERGY_STEP_UNTARGETED = 0, ///< casts it with no target (FieldEffect_Run)
  /// Casts it on its target, secondObjectId (FieldEffect_DispatchRequestKind).
  PSYNERGY_STEP_ON_TARGET = 1,
  PSYNERGY_STEP_FINISH = 2, ///< finishes it
  PSYNERGY_STEP_EXTRA = 3   ///< runs its kind's extra handler
};

/// A field Psynergy's effect, as the field kind: the type column of
/// games/tbs/src/game/actions.tsv, named for the abilities that have it. The
/// effect scene keeps it as its animation.
enum FieldKind {
  FIELD_KIND_MOVE = 1,
  FIELD_KIND_MIND_READ = 2,
  FIELD_KIND_PLY = 3,
  FIELD_KIND_FORCE = 4,
  FIELD_KIND_DOUSE = 5,
  FIELD_KIND_FROST = 6,
  FIELD_KIND_LIFT = 7,
  FIELD_KIND_REVEAL = 8,
  FIELD_KIND_HALT = 9,
  FIELD_KIND_CLOAK = 10,
  FIELD_KIND_CARRY = 11,
  FIELD_KIND_GROWTH = 12,
  FIELD_KIND_CATCH = 13,
  /// Whirlwind and Gale.
  FIELD_KIND_WHIRLWIND = 14,
  FIELD_KIND_RETREAT = 15,
  FIELD_KIND_AVOID = 16
};

/// One frame of a sparkle: its height offset and tile (fxdata.c).
struct SparkleFrame {
  s16 dy;
  u16 tile;
};

extern const struct SparkleFrame gFieldEffect_SparkleFrames[32];

void Event_UpdateDebugSkip(void);
void FieldEvent_WaitFrames(s32 frames);
void Leader_StopMoving(void);
void FieldEvent_Begin(void);
void FieldEvent_End(void);
void Event_NoOpHookA(void);
void Event_NoOpHook(void);
void Event_RunObjectHookAndWait(struct ScenePlacement *table);
void Event_CallWithLastActiveObjectId(struct ScenePlacement *table);
void Event_SetSceneEvents(const struct SceneEvent *events);
void Event_PrepareObjectAndApplyValue(s32 unit, s32 announce);
void Event_PrepareTwoObjectsAndApply(s32 first, s32 second);
s32 Event_ValidatePackedId(u32 shortcut);
void Event_ClearInvalidPackedValues(void);
void Party_RemoveOwnerRestored(s32 unit);
s32 Party_CheckItemLimit(s32 item);
s32 PartyInventory_GiveItem(s32 item, s32 unused);
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 unit);
void Inventory_NoOpCallback(void);
void Inventory_ReservedNoOp(void);
void Actor_WaitForAnimationEnd(s32 actor, s32 animation);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 forceBottom);
s32 Menu_AskYesNo(s32 atBottom);
void Party_LearnAbility(s32 unit, s32 ability);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_SetDestination(s32 scene, s32 entrance);
void Event_SetSavedDestination(u16 scene, u16 entrance);
void Event_SetExitRequest(u16 request);
void Field_RequestBattleFromEncounterTable(s32 zoneNo, s32 slotNo);
/// Field_RequestDjinnBattle's flag: the Djinni does not hop first.
#define DJINN_NO_HOP 0x800
void Field_RequestDjinnBattle(s32 flag, s32 no);
void Event_SetNextDestination(s32 scene, s32 entrance);
void Event_SetDefeatDestination(s32 scene, s32 entrance);
void Event_RequestDestination(u16 scene, u16 entrance);
void FieldEffect_SetQueuedSoundAndPlay(s32 soundId);
void FieldEffect_PlayQueuedSound(void);

struct DispatchObject;
struct FacingObject;
struct FieldActor;
struct ObjectRuntime;

struct FieldActor *Object_GetById(s32 actor);
void ObjectMotion_SetSpeedParameters(u32 objectId, s32 speedLimit,
                                     s32 acceleration);
// Built-in actions selected by ObjectMotion_SetActionCallback.
#define ACTOR_ACTION_IDLE 1
#define ACTOR_ACTION_FOLLOW_LINKED_OBJECT 5
#define ACTOR_ACTION_FACE_SELECTED_ACTOR 6
/// Clears child and collision flags, then returns to the idle script.
#define ACTOR_ACTION_RESET 7

void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action);
void ObjectMotion_EnableActionAndResetMotion(u32 objectId);
void Object_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId);
void Object_RefreshSelectorById(u32 objectId);
/// Accepts a built-in action number or script address; waits up to 300 frames.
void Object_SetActionCallbackAndRefreshById(u32 objectId, s32 action);
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
void Motion_LaunchFromFocusedObject(u32 objectId, s32 xOffset, s32 zOffset,
                                    s32 action);
void ObjectMotion_CommitCurrentPositionAndActivate(u32 objectId);
void ObjectMotion_SetHorizontalPositionWithTerrain(u32 objectId, s32 x, s32 z);
void ObjectMotion_SetPositionWithTerrain(u32 objectId, s32 x, s32 y, s32 z);
void Object_SetModeById(u32 objectId, s32 animation);
void Object_SetActionById(u32 objectId, s32 steps);
void ObjectMotion_WaitForAnimationChange(u32 objectId);
void Motion_SetModeAndWaitAnimation(u32 objectId, s32 animation);
void ObjectMotion_NoOp(void);
void ObjectMotion_Launch(u32 objectId, s32 speed, s32 frames);
void ObjectMotion_SetVariantCallback(u32 objectId, s32 count);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 count);
void FieldEffect_UpdateParticleLinearMotion(union FieldObject *fieldObject);
void FieldEffect_SpawnBurstParticle(struct FieldActor *source, s32 param);
void Actor_JumpDownLedge(s32 actor, s32 pose, s32 param);
void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait);
void Object_LinkPair(s32 firstId, s32 secondId, s32 wait);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *firstObject,
                                          struct FacingObject *secondObject);
void ObjectTable_DestroyById(s32 index);
void ObjectTable_DestroyNoOp(void);
void ObjectGroup_ConfigureChildValue(s32 objectId, s32 param);
void ObjectGroup_ApplyIndexedChildValue(union FieldObject *fieldObject);
void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 param);
void Object_SetTargetAndCallback(u32 objectId, s32 targetId,
                                 const void *script);
s32 ObjectMotion_StepAngle(struct ObjectRuntime *object);
void Object_ResetTargetAndSetMode1(u32 objectId);
void ObjectMotion_ArmCallback(s32 objectId, s32 angle, s32 wait);
void ObjectMotion_SetActionVariant(s32 objectId, s32 priority);
void ObjectVisual_CopyAttributes(u32 targetId, u32 sourceId);
void ObjectVisual_ReservedNoOp(void);

struct UiWindow;

void Event_SetMessageId(s32 message);
s32 ObjectTable_ReadActiveValue(s32 actor);
s32 ObjectTable_FindActiveByValue(s32 sprite);
struct UiWindow *UiText_OpenMessageAtObject(s32 actor, s32 unused);
void FieldEvent_ShowMessageAndWait(s32 actor, s32 flags);
void FieldEvent_ShowMessageThenWait(s32 actor, s32 flags, s32 wait);
s32 FieldEvent_AskYesNoAndShowResponse(s32 actor, s32 flags);
void FieldEvent_EmptyCallback(void);
void Event_ShowSpeakerMessage(s32 speaker);
void Event_ForwardShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void ObjectTable_RunIfActive(s32 actor);
void FieldEvent_ShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
                                       s32 firstPortraitX, s32 firstPortraitY,
                                       s32 second, s32 secondX, s32 secondY,
                                       s32 secondPortraitX,
                                       s32 secondPortraitY);
void Ui_SetMessageVoiceFromActor(s32 actor);

void Camera_FollowActor(s32 actor, s32 keepPosition);
void Camera_SetSpeedParameters(s32 speedLimit, s32 acceleration);
void Camera_MoveWithinMapBounds(s32 requestedX, s32 requestedY, s32 requestedZ,
                                s32 glide);
void Object_PlaceCurrentWithinCameraBounds(s32 actor, s32 glide);
void Camera_WaitForArrival(void);
struct FieldActor *Camera_GetEventObject(void);
void Camera_FollowObject(struct ObjectRuntime *target, s32 keepCurrentPosition);
void Map_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY);
#ifdef UBFIX
void FieldEffect_StepRatioTransition(void);
#else
s32 FieldEffect_StepRatioTransition(void);
#endif
void FieldEffect_ScheduleRatioTransition(s32 zoom, s32 frames);
void Event_WaitForZoomTransition(void);
#ifdef UBFIX
void FieldEffect_CopyLinkedObjectPosition(union FieldObject *fieldObject);
#else
s32 FieldEffect_CopyLinkedObjectPosition(union FieldObject *fieldObject);
#endif
void FieldEffect_SpawnLinked(s32 actor, s32 emote, s32 wait);
/// FieldEffect_SpawnLinked's emote flag: the bubble takes sprite priority 1
/// instead of its actor's. The low four bits pick the bubble (enum Emote).
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
  EMOTE_LIGHT_BULB = 6, ///< a lit bulb, with SE_110
  EMOTE_SCOWL = 7,      ///< a scowling face
  EMOTE_SMILE = 8,      ///< a smiling face
  /// A sweat drop; no scene shows it, and the Japanese sheet has no tenth
  /// bubble.
  EMOTE_SWEAT_DROP = 9
};
/// FieldEffect_ConfigureLinkedObject's flags: a mode in the low two bits, and
/// ATTACHED_EFFECT_IN_FRONT.
enum AttachedEffectFlag {
  ATTACHED_EFFECT_REMOVE = 0, ///< removes the attached object
  ATTACHED_EFFECT_ATTACH = 1, ///< attaches one that stays until removed
  ATTACHED_EFFECT_ONCE = 2,   ///< shows a one-off one that ends by itself
  /// The object takes sprite priority 1, not the actor's.
  ATTACHED_EFFECT_IN_FRONT = 0x100
};
void FieldEffect_ConfigureLinkedObject(s32 actor, s32 flags);
s32 Object_ClearChildAndCollisionFlags(void *obj);
s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object);
s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object);
void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 action);
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self,
                                                     s32 sprite);
s32 Field_TryJumpForward(void);
s32 Field_ClimbDown(void);
s32 Field_ClimbUp(void);
s32 Object_GetScreenPosition(s32 objectId, s32 *position);
void ObjectEffect_ReservedNoOp941DC(void);
#ifdef UBFIX
void Field_StartEncounterTransition(s32 unused);
#else
s32 Field_StartEncounterTransition(s32 unused);
#endif
void ObjectEffect_PrepareContextEffect(s32 animation);
void ObjectEffect_BeginContextEffect26(void);
void ObjectEffect_BeginContextEffect25(void);
void ObjectEffect_EndContextEffect(s32 animation);
s32 ObjectEffect_RunPendingFlagEvent(void);

void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
void DisplayScroll_DisarmHBlankDma(void);
void FieldEffect_UpdateGroundParticles(void);
void FieldEffect_UpdateLightning(void);
void FieldEffect_StartGroundParticles(void);
void FieldMotes_Update(void);
void FieldMotes_Start(void);
void FieldEffect_UpdateSparkles(void);
void FieldEffect_InitSparkles(void);
void FieldEffect_StartLightning(void);
void FieldEffect_StopLightning(void);
void FieldEffect_LastLightning(void);
void FieldEffect_ResumeLightning(void);
void FieldEffect_StartBufferBlend(s32 from, s32 to);

struct DescendingOrbitObject;
struct EffectSlot;
struct MotionObject;

void ObjectMotion_MoveHalfwayTowardTarget(union FieldObject *fieldObject);
void Djinn_RunBattleStartHop(s32 djinniId);
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn);
void FieldObject_PlaceSceneActors(void);
// Declared without a prototype, as Field_RunScene (object.c) calls it with
// one argument and with two. object.c comes before this unit in ROM order, so
// in one file the calls would still come before the definition.
void Djinn_ResolvePendingEvent();
void FieldEffect_UpdateAllSlots(void);
void FieldEffect_InitializeSlots(void);
void FieldEffect_ClearActiveSlotsAndScheduleUpdates(void);
void FieldEffect_AdvanceSpinAngle(union FieldObject *fieldObject);
void FieldEffect_RunAngledApproachPhases(struct EffectSlot *particle);
void Djinn_RunJupiterJoinEffect(s32 djinniId);
void FieldEffect_SetObjectAlternatingWords(union FieldObject *fieldObject);
void FieldEffect_AdvanceAngleWithRamp(union FieldObject *fieldObject);
void FieldEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect);
void Djinn_RunMarsJoinEffect(s32 djinniId);
void FieldEffect_UpdateDescendingOrbitObject(union FieldObject *fieldObject);
void FieldEffect_UpdateRadialSpread(struct EffectSlot *effect);
void FieldEffect_RunVenusDjinnCapture(s32 djinniId);
void FieldEffect_UpdateRandomTargetParticle(struct EffectSlot *effect);
void FieldEffect_HalveDistanceToTarget(union FieldObject *fieldObject);
void FieldEffect_RunMercuryDjinnCapture(s32 djinniId);

struct EffectVector;

void Field_DispatchTypeHandler(s32 kind);
void FieldEffect_Run(void);
void FieldEffect_DispatchRequestKind(void);
void FieldEffect_ClearChildValueOnMismatch(void);
void FieldEvent_RunTypeHandler(void);
s32 Field_RunEventAction(const void *event, s32 param, s32 target);
void ObjectGroup_ApplyRandomChildValues(union FieldObject *fieldObject);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle);
s32 ResourceTable_CountFreeBlocks(void);
struct AnimationObject *
Sprite_ShareVramCache(struct AnimationObject *sourceSprite,
                      struct AnimationObject *sharedSprite);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state,
                                    s32 value);
void FieldEffect_UpdateScaledArcObjectA(union FieldObject *fieldObject);
void FieldEffect_UpdateScaledArcObjectB(union FieldObject *fieldObject);
void FieldEffect_SpawnScaledArcObjects(struct FieldActor *linked);
void FieldEffect_FlickerObjectAndTick(union FieldObject *fieldObject);
void FieldEffect_CycleObjectValueByCounter(union FieldObject *fieldObject);
void FieldEffect_UpdateAllEffectSlots(void);
void FieldEffect_LoadActionEffectResources(s32 abilityId, s32 mode);
void FieldEffect_SetupObjectPair(s32 userId, s32 targetId);
void FieldEffect_StopCasterHook(void);
void FieldEffect_End(void);
void EventObject_Initialize(void);
void FieldEffect_FadeToEffectColor(void);
void FieldEffect_FadeFromEffectColor(void);
void Camera_WorldToScreen(struct EffectVector *position);

struct ArcEffectObject;
struct Effect05Object;
struct OrbitEffect;
struct SparkObject;

void RunSceneTransitionEffect(s32 x, s32 y);
void FieldEffect_FinishSceneAndReleaseHeapBlock(void);
void FieldEffect_UpdateMindReadWave(void);
void FieldEffect_ArmBg0HBlankDma(void);
void FieldEffect_AdvanceHueCycle(void);
void FieldEffect_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b);
s32 FieldEffect_HueChannelRamp(s32 angle, s32 low, s32 high);
s32 FixedPoint_Remainder(s32 value, s32 divisor);
void FieldEffect_SetCallbackWhenTargetUnset(union FieldObject *fieldObject);
void Ui_FillBank15PaletteGrey(void);
void Ui_SetBank15PaletteAndClearRenderMode(void);
s32 Input_GetDpadAngle(u32 unused);
void FieldEffect_UpdateItemBreakFragment(union FieldObject *fieldObject);
void FieldEffect_SteerPushedTarget(s32 targetId);
void FieldEffect_UpdateOrbitAndReturn(struct EffectSlot *slot);
void FieldEffect_RunMove(void);
void *FieldEffect_StartItemBreak(void *object);
void FieldEffect_SnapScaleToFull(struct MotionObject *obj);
void UpdateRisingParticleBurst(void *object);
void ObjectDispatch_ApplyValueToKind200Children(s32 value);
void FieldEffect_WatchLeaderDistance(void);
void FieldEffect_RunReveal(void);
void ResetSceneTransitionEffect(void);
void FieldEffect_RunGrowthOnTarget(s32 unused);
void FieldEffect_RunGrowth(void);
void FieldEffect_SpawnBurstParticleField(void);
void FieldEffect_RunLiftOnTarget(s32 unused);
struct MotionObject *FieldEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z,
                                                     s32 angle);
void FieldEffect_UpdateRadialBurst(struct EffectSlot *slot);
s32 SpawnHeavyImpactEffect(struct ScriptInterpreter *interpreter);
void FieldEffect_RunForceOnTarget(s32 unused);
void FieldEffect_RunForce(void);
void FieldEffect_UpdateOrbitingParticleMain(union FieldObject *fieldObject);
void FieldEffect_UpdateOrbitingParticleFade(union FieldObject *fieldObject);
void FieldEffect_UpdateOrbitingParticleLeft(union FieldObject *fieldObject);
void FieldEffect_UpdateOrbitingParticleRight(union FieldObject *fieldObject);
void FieldEffect_RunFrostOnTarget(s32 unused);
void FieldEffect_RunFrost(void);
void FieldEffect_SwayParticle(union FieldObject *fieldObject);
void FieldEffect_UpdateShrinkingOrbitObject(union FieldObject *fieldObject);
void FieldEffect_RunSparkEmitter(union FieldObject *fieldObject);
void FieldEffect_RunPlyOnTarget(s32 unused);
void FieldEffect_RunPly(void);
void FieldEffect_CheckCloakTile(void);
void FieldEffect_EndCloak(void);
void FieldEffect_ResumeCloakCheck(void);
void FieldEffect_StartCloak(void);
void FieldEffect_SpawnRandomAngleTriplet(union FieldObject *fieldObject);
void FieldEffect_UpdateDriftingFallObject(union FieldObject *fieldObject);
void FieldEffect_RunDouseOnTarget(s32 unused);
void FieldEffect_RunDouse(void);
void FieldEffect_SpawnFallingParticles(void);
void FieldEffect_SteerLiftedTarget(s32 targetId);
struct MotionObject *FieldEffect_SpawnItemBreakMode1(s32 x, s32 y, s32 z,
                                                     s32 angle);
void FieldEffect_UpdateParticleMotionAndScale(union FieldObject *object);
void FieldEffect_SpawnRadialParticleRing(struct MotionObject *origin);
void FieldEffect_WanderAroundAnchor(union FieldObject *fieldObject);
void FieldEffect_CircleAnchor(union FieldObject *fieldObject);
void FieldEffect_ShrinkObjectScaleUntilHalf(union FieldObject *fieldObject);
void FieldEffect_RunWhirlwindOnTarget(s32 unused);
void FieldEffect_RunWhirlwind(void);
void FieldEffect_UpdateRadialCamera(struct EffectSlot *effect);
void FieldEffect_RunHaltOnTarget(s32 unused);
void FieldEffect_RunHalt(void);
void FieldEffect_UpdatePausedObject(union FieldObject *fieldObject);
void FieldEffect_PauseObject(s32 actorId);
void FieldEffect_ResumeObject(s32 actorId);
s32 FieldEffect_FilterObjectIdByFlags(s32 id);
void FieldEffect_RunCatchOnTarget(s32 unused);
void FieldEffect_RunCatch(void);
void FieldEffect_UpdateSpinAngle(union FieldObject *fieldObject);
void FieldEffect_UpdateDescendingObject(union FieldObject *fieldObject);
void FieldEffect_UpdateRadialLaunch(struct EffectSlot *slot);
void FieldEffect_RunRetreat(void);
void FieldEffect_UpdateDescendingParticlePositiveArc(
    union FieldObject *fieldObject);
void FieldEffect_UpdateDescendingParticleNegativeArc(
    union FieldObject *fieldObject);
void FieldEffect_SpawnDescendingArcParticles(struct FieldActor *arc);
void FieldEffect_BlinkAvoidCaster(void);
void FieldEffect_UpdatePairedArcSpawner(union FieldObject *fieldObject);
void FieldEvent_ShowStatusMessage(void);
void FieldEffect_RunAvoid(void);

void EffectSlot_Update(struct EffectSlot *effect);
void FieldEffect_DrawScaledObject(struct EffectSlot *object);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
u32 EffectSlot_HasPendingTarget(struct EffectSlot *effect);
void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z);
void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode);
void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback);
void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z);
void FieldEffect_ClearOwnedSlot(struct EffectSlot *slot);

void FieldEffect_SetupResourcesAndWindow(void);
void FieldEffect_CleanupResourcesAndWindow(void);
void Map_UpdateWorldMapMarkers(void);
void Map_ShowWorldMap(s32 picture);
void Field_UpdateObjectVisibilityBounds(void);
void FieldEffect_RunVisibilityTransition(void);

#endif
