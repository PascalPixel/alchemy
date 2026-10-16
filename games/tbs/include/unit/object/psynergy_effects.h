#ifndef ALCHEMY_UNIT_OBJECT_PSYNERGY_EFFECTS_H
#define ALCHEMY_UNIT_OBJECT_PSYNERGY_EFFECTS_H

#include "types.h"
#include "effect_slot.h"
#include "unit/object/object.h"

struct ScenePlacement;
union FieldObject;

// One frame of a sparkle: its height offset and tile (fxdata.c).
struct SparkleFrame {
  s16 dy;
  u16 tile;
};

extern const struct SparkleFrame gFieldFx_SparkleFrames[32];

void Event_UpdateDebugSkip(void);
void Battle_WaitMode0(s32 frames);
void Leader_StopMoving(void);
void Battle_Reset(void);
void BattleFx_FinishAction(void);
void Event_NoOpHookA(void);
void Event_NoOpHook(void);
void Event_RunObjectHookAndWait(struct ScenePlacement *table);
void Event_CallWithLastActiveObjectId(struct ScenePlacement *table);
void Event_SetSceneEvents(s32 events);
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
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 forceBottom);
s32 Menu_AskYesNo(s32 atBottom);
void Party_LearnAbility(s32 unit, s32 ability);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Event_SetDestination(s32 scene, s32 entrance);
void Event_SetSavedDestination(u16 scene, u16 entrance);
void Event_SetExitRequest(u16 request);
void BattleFx_SetWeightedResult(s32 zoneNo, s32 slotNo);
void BattleFx_SetPhaseRequest(s32 flag, s32 no);
void Event_SetNextDestination(s32 scene, s32 entrance);
void Event_SetDefeatDestination(s32 scene, s32 entrance);
void Event_RequestDestination(u16 scene, u16 entrance);
void BattleFx_SetQueuedSoundAndPlay(s32 soundId);
void BattleFx_PlayQueuedSound(void);

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
// Clear child and collision flags, then return to the idle script.
#define ACTOR_ACTION_RESET 7

void ObjectMotion_EnableActionAndSetCallback(u32 objectId, s32 action);
void ObjectMotion_EnableActionAndResetMotion(u32 objectId);
void Object_LinkObjectAndSetCallback(u32 objectId, u32 linkedObjectId);
void Object_RefreshSelectorById(u32 objectId);
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
void BattleFx_UpdateParticleLinearMotion(union FieldObject *fieldObject);
void BattleFx_SpawnBurstParticle(struct FieldActor *source, s32 param);
void Actor_JumpDownLedge(s32 actor, s32 pose, s32 param);
void ObjectMotion_SetAngleToward(s32 firstId, s32 secondId, s32 wait);
void Object_LinkPair(s32 firstId, s32 secondId, s32 wait);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *raw_first,
                                          struct FacingObject *raw_second);
void ObjectTable_DestroyById(s32 index);
void ObjectTable_DestroyNoOp(void);
void ObjectGroup_ConfigureChildValue(s32 objectId, s32 param);
void ObjectGroup_ApplyIndexedChildValue(union FieldObject *object);
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
void BattleEv_RunWait(s32 actor, s32 flags);
void BattlePres_RunActionThenWaitIfModeZero(s32 actor, s32 flags, s32 wait);
s32 BattleEventRuntime_ProcessAction(s32 actor, s32 flags);
void BattleEventRuntime_EmptyCallback(void);
void Event_ShowSpeakerMessage(s32 speaker);
void ObjectTable_CallRefreshHook(s32 unused0, s32 unused1, s32 x, s32 y);
void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y);
void ObjectTable_RunIfActive(s32 actor);
void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 firstX, s32 firstY,
                                      s32 firstPortraitX, s32 firstPortraitY,
                                      s32 second, s32 secondX, s32 secondY,
                                      s32 secondPortraitX, s32 secondPortraitY);
void Ui_SetRenderResultFromObject(s32 actor);

void Object_AttachWorkTargetToObject(s32 actor, s32 keepPosition);
void ObjectTable_AllocateAndSetObjectSpeed(s32 speedLimit, s32 acceleration);
void Motion_CamBounds(s32 requestedX, s32 requestedY, s32 requestedZ,
                      s32 glide);
void Object_PlaceCurrentWithinCameraBounds(s32 actor, s32 glide);
void BattleFx_CommitObjectPositionAndWait(void);
struct FieldActor *Battle_GetWorkObject1e0(void);
void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target,
                                 s32 keepCurrentPosition);
void Map_SetViewBounds(s32 minX, s32 minY, s32 maxX, s32 maxY);
#ifdef UBFIX
void BattleFx_StepRatioTransition(void);
#else
s32 BattleFx_StepRatioTransition(void);
#endif
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames);
void Event_WaitForZoomTransition(void);
#ifdef UBFIX
void BattleFx_CopyLinkedObjectPosition(union FieldObject *fieldObject);
#else
s32 BattleFx_CopyLinkedObjectPosition(union FieldObject *fieldObject);
#endif
void BattleFx_SpawnLinked(s32 actor, s32 emote, s32 wait);
void BattleFx_ConfigureLinkedObject(s32 actor, s32 flags);
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
s32 Field_StartEncounterTransition(s32 unused);
void ObjectEffect_PrepareContextEffect(s32 animation);
void ObjectEffect_BeginContextEffect26(void);
void ObjectEffect_BeginContextEffect25(void);
void ObjectEffect_EndContextEffect(s32 animation);
s32 ObjectEffect_RunPendingFlagEvent(void);

void DisplayScroll_ArmHBlankDma(void);
void DisplayScroll_BuildAndSwapHBlankPage(void);
void DisplayScroll_DisarmHBlankDma(void);
void FieldFx_UpdateGroundParticles(void);
void FieldFx_UpdateLightning(void);
void FieldFx_StartGroundParticles(void);
void FieldMotes_Update(void);
void FieldMotes_Start(void);
void FieldEffect_UpdateSparkles(void);
void FieldEffect_InitSparkles(void);
void FieldFx_StartLightning(void);
void FieldFx_StopLightning(void);
void FieldFx_LastLightning(void);
void FieldFx_ResumeLightning(void);
void BattleFx_StartBufferBlend(s32 from, s32 to);

struct DescendingOrbitObject;
struct EffectSlot;
struct MotionObject;

void ObjectMotion_MoveHalfwayTowardTarget(union FieldObject *fieldObject);
void Djinn_RunBattleStartHop(s32 id);
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn);
void FieldObject_PlaceSceneActors(void);
/* Djinn_ResolvePendingEvent is called from object.c with no prototype in scope:
 * Field_RunScene calls it, and object.c comes before this unit in ROM order, so
 * in one file the call would still come first. No split evidence. */
void BattleFx_UpdateAllSlots(void);
void BattleFx_InitializeSlots(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);
void BattleFx_AdvanceSpinAngle(union FieldObject *fieldObject);
void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor);
void Djinn_RunJupiterJoinEffect(s32 djinniId);
void BattleFx_SetObjectAlternatingWords(union FieldObject *fieldObject);
void BattleFx_AdvanceAngleWithRamp(union FieldObject *fieldObject);
void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect);
void Djinn_RunMarsJoinEffect(s32 arg);
void BattleFx_UpdateDescendingOrbitObject(union FieldObject *fieldObject);
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect);
void BattleFx_RunVenusDjinnCapture(s32 arg);
void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect);
void BattleFx_HalveDistanceToTarget(union FieldObject *fieldObject);
void BattleFx_RunMercuryDjinnCapture(s32 id);

struct EffectVector;

void Field_DispatchTypeHandler(s32 kind);
void BattleFx_Run(void);
void BattleFx_DispatchRequestKind(void);
void BattleFx_ClearChildValueOnMismatch(void);
void FieldEvent_RunTypeHandler(void);
s32 BattleFx_RunEventAction(void *event, s32 param, s32 target);
void ObjectGroup_ApplyRandomChildValues(union FieldObject *fieldObject);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle);
s32 ResourceTable_CountFreeBlocks(void);
void *Object_ReplaceResourceEntry(void *src, void *alt);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state,
                                    s32 value);
void BattleFx_UpdateScaledArcObjectA(union FieldObject *fieldObject);
void BattleFx_UpdateScaledArcObjectB(union FieldObject *fieldObject);
void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked);
void BattleFx_FlickerObjectAndTick(union FieldObject *fieldObject);
void BattleFx_CycleObjectValueByCounter(union FieldObject *fieldObject);
void BattleFx_UpdateAllEffectSlots(void);
void BattleFx_LoadActionEffectResources(s32 abilityId, s32 mode);
void BattleFx_SetupObjectPair(s32 userId, s32 targetId);
void EffectRuntime_StopCurrentObject(void);
void BattleEffect_CleanupSceneObjects(void);
void EventObject_Initialize(void);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Camera_WorldToScreen(struct EffectVector *position);

struct ArcEffectObject;
struct Effect05Object;
struct OrbitEffect;
struct SparkObject;

void RunSceneTransitionEffect(s32 x, s32 y);
void BattleFx_FinishSceneAndReleaseHeapBlock(void);
void BattleFx_UpdateWaveScene(void);
void BattleFx_ArmBg0HBlankDma(void);
void BattleFx_AdvanceHueCycle(void);
void BattleFx_ComputeHueChannels(s32 hue, s32 *r, s32 *g, s32 *b);
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high);
s32 Fixed_Remainder(s32 value, s32 divisor);
void BattleFx_SetCallbackWhenTargetUnset(union FieldObject *fieldObject);
void Ui_FillBank15PaletteGrey(void);
void Ui_SetBank15PaletteAndClearRenderMode(void);
s32 Input_GetDpadAngle(u32 unused);
void BattleFx_UpdateItemBreakFragment(union FieldObject *fieldObject);
void BattleFx_SteerPushedTarget(s32 targetId);
void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *slot);
void BattleFx_RunItemBreakSequence(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(struct MotionObject *obj);
void UpdateRisingParticleBurst(void *object);
void ObjectDispatch_ApplyValueToKind200Children(s32 value);
void FieldEffect_WatchLeaderDistance(void);
void RunBattleEffect08(void);
void ResetSceneTransitionEffect(void);
void BattleFx_RunBurstParticleMainObject(s32 unused);
void BattleFx_RunBurstParticles(void);
void BattleEffect_SpawnBurstParticleField(void);
void BattleEffect_RunTargetedItemBreak(s32 unused);
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z,
                                                  s32 angle);
void BattleFx_UpdateRadialBurst(struct EffectSlot *slot);
s32 SpawnHeavyImpactEffect(struct ScriptInterpreter *interpreter);
void BattleFx_CallEffect04(s32 unused);
void RunBattleEffect04(void);
void BattleFx_UpdateOrbitingParticleMain(union FieldObject *fieldObject);
void BattleFx_UpdateOrbitingParticleFade(union FieldObject *fieldObject);
void BattleFx_UpdateOrbitingParticleLeft(union FieldObject *fieldObject);
void BattleFx_UpdateOrbitingParticleRight(union FieldObject *fieldObject);
void BattleFx_StartOrbitingParticles(s32 unused);
void BattleFx_RunOrbitingParticles(void);
void BattleFx_SwayParticle(union FieldObject *fieldObject);
void BattleFx_UpdateShrinkingOrbitObject(union FieldObject *fieldObject);
void BattleFx_RunSparkEmitter(union FieldObject *fieldObject);
void BattleFx_CallEffect03AndStop(s32 unused);
void RunBattleEffect03(void);
void FieldFx_CheckCloakTile(void);
void FieldFx_EndCloak(void);
void FieldFx_ResumeCloakCheck(void);
void FieldFx_StartCloak(void);
void BattleFx_SpawnRandomAngleTriplet(union FieldObject *fieldObject);
void BattleFx_UpdateDriftingFallObject(union FieldObject *fieldObject);
void BattleFx_CallEffect05(s32 unused);
void RunBattleEffect05(void);
void BattleFx_SpawnFallingParticles(void);
void BattleFx_SteerLiftedTarget(s32 id);
struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z,
                                                  s32 angle);
void BattleFx_UpdateParticleMotionAndScale(union FieldObject *object);
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin);
void BattleFx_WanderAroundAnchor(union FieldObject *fieldObject);
void BattleFx_CircleAnchor(union FieldObject *fieldObject);
void BattleFx_ShrinkObjectScaleUntilHalf(union FieldObject *fieldObject);
void BattleFx_CallEffect14(s32 unused);
void RunBattleEffect14(void);
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect);
void BattleFx_MarkChildAndRunFallbackTransition(s32 unused);
void BattleEffect_RunFallbackObjectTransition(void);
void BattleEffect_SetRandomTableValueOnObject(union FieldObject *fieldObject);
void BattleEffect_PauseObject(s32 id);
void BattleFx_ResumeObject(s32 id);
s32 BattleFx_FilterObjectIdByFlags(s32 id);
void BattleFx_RunEffect13Hook(s32 unused);
void RunBattleEffect13(void);
void BattleFx_UpdateSpinAngle(union FieldObject *fieldObject);
void BattleFx_UpdateDescendingObject(union FieldObject *fieldObject);
void BattleFx_UpdateRadialLaunch(struct EffectSlot *slot);
void BattleFx_RunEffect15(void);
void BattleFx_UpdateDescendingParticlePositiveArc(
    union FieldObject *fieldObject);
void BattleFx_UpdateDescendingParticleNegativeArc(
    union FieldObject *fieldObject);
void BattleFx_SpawnDescendingArcParticles(struct FieldActor *arc);
void BattleFx_UpdateEffect16State(void);
void BattleFx_UpdatePairedArcSpawner(union FieldObject *fieldObject);
void FieldEvent_ShowStatusMessage(void);
void RunBattleEffect16(void);

void EffectSlot_Update(struct EffectSlot *effect);
void BattleFx_DrawScaledObject(struct EffectSlot *object);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
u32 BattleFx_HasReachedTarget(struct EffectSlot *effect);
void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z);
void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode);
void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback);
void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z);
void BattleFx_ClearOwnedSlot(struct EffectSlot *slot);

void BattleFx_SetupResourcesAndWindow(void);
void BattleFx_CleanupResourcesAndWindow(void);
void Map_UpdateWorldMapMarkers(void);
void Map_ShowWorldMap(s32 picture);
void BattleFx_UpdateObjectVisibilityBounds(void);
void BattleFx_RunVisibilityTransition(void);

#endif
