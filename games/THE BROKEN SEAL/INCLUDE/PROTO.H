#ifndef PROTO_H
#define PROTO_H

#include "TYPES.H"

struct AnimationEntry;
struct AnimationObject;
struct ArcParticle;
struct BattleActionRecord;
struct BattleEffectArgument;
struct BattleEventState;
struct BattlePlan;
struct BattleUnit;
struct BurstParticle;
struct DescendingOrbitObject;
struct DispatchObject;
struct DjinnListTable;
struct EffectLaunch;
struct EffectPosition;
struct EffectSlot;
struct EffectStep;
struct EnemyDefinition;
struct FacingObject;
struct FieldActor;
struct FieldPosition;
struct FxObject;
struct GlyphPalette;
struct ItemDefinition;
struct MapCell;
struct MenuResult;
struct MotionObject;
struct ObjectRuntime;
struct OrbitEffect;
struct Output_08097f80;
struct PillarEffect;
struct RenderInput;
struct RenderOutput;
struct S_02000474;
struct ScenePlacement;
struct SelectionNode;
struct SelectionScreen;
struct SoundPlayer;
struct SoundTrack;
struct SoundWork;
struct Source_0808f28c;
struct SparkObject;
struct Swirl;
struct TransitionActorList;
struct UiChannelSlot;
struct UiWindow;
struct V;
struct WorkspaceWork;
union FieldObject;

u32 __udivsi3(u32 numerator, u32 denominator);
void _call_via_r2(s32 arg0, s32 arg1, s32 target);
s32 Ability_GetAvailability(s32);
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
s32 Ability_LoadGlyphFar(s32, s32, s32 *, s32 *, s32);
void Ability_PlayUseAnimation(s32 action);
s32 AbilityMenu_BuildAvailableList(void);
void ActorPresentation_PlaceActorFourteenOnActorNine(void);
void ActorPresentation_PrepareActorFourteenWithCallback(void);
void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void);
void ActorPresentation_RepaintTenCellsAndActorEightCell(void);
void ActorPresentation_RunActorModeOneThenZero(s32 actor);
void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x);
void *AffineEffect_InitializeWork(void);
void AffineEffect_UpdateFrame(void);
void Animation_ApplyChildArgumentFar(void *, s32);
s32 Animation_InitializeObjects(struct AnimationObject *state);
void Animation_InitWorkFromMetadata(struct AnimationEntry *work);
s32 Animation_LookupValueByKey(s32 key);
void Animation_SetWorkEntryFar(struct AnimationEntry *effect, s32 entry_index);
struct AnimationEntry *AnimationObject_Allocate(s32 id);
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *group, s32 val);
void AnimationObjects_SetActiveTimerStepsFar(struct AnimationObject *object, s32 arg);
void ArutamiraDou_ReleaseWallBurst(void);
void ArutamiraDou_SpinActorWheel(void);
void ArutinYama_AdvanceRollingObject(struct FieldActor *object);
void ArutinYama_ApplyEntryState(void);
void ArutinYama_PlaceFlaggedActors(void);
void ArutinYama_RunRollingObject(s32 id, s32 heading);
void ArutinYama_SettleAndMountLeader(struct FieldActor *object);
void ArutinYama_StepPaletteAnim(void);
void ArutinYama_TurnRollingObjectA(struct FieldActor *object);
void ArutinYama_TurnRollingObjectB(struct FieldActor *object);
s32 Audio_Check(void);
void Audio_EmptyCallback(void);
void Audio_Initialize(void);
void Audio_InitializeRuntimeDefaultsFar(void);
s32 Audio_PlayCueReturnOne(s32 cue);
void Audio_PlaySound(u16 id);
void Audio_ResumeAllPlayers(void);
void Audio_ResumePlayer(struct SoundPlayer *player);
void Audio_StopAllPlayers(void);
s32 Audio_UmulHigh32(s32, s32);
s32 AudioCommand_GetSecondaryStateByteFar(void);
void AudioCommand_WaitForStateByteClear(void);
void AudioEngine_Initialize(struct SoundWork *work);
void AudioEngine_ResumeDirectSound(void);
void AudioEngine_RunMixerTick(void);
void AudioEngine_SetMode(u32 mode);
void AudioEngine_SetPcmRate(u32 mode);
void AudioEngine_SuspendDirectSound(void);
void BabiChika_MarkActorCells(void);
void BabiChika_RestorePlatformCells(void);
void BabiChika_SettleSteps(s32 wait);
void BabiChika_UpdateTrackedActor(void);
void BabiFune_CyclePalette(void);
void BabiFune_UpdateDriftingObject(u8 *obj);
void BabiFune_UpdateWaves(void);
void Battle_AwardSpoils(void);
s32 Battle_ClearSerialTransferPayload(void);
s32 Battle_CollectPartyCommandsFar(void *entries, u16 *excluded_units, s32 excluded_count);
u32 Battle_GetObjectTableValue(s32 id);
u32 Battle_GetSpriteFlagHighBits(u32 index);
u32 Battle_GetSpriteFlagNibble(u32 index);
void Battle_InitializeRenderObject(void);
void Battle_PlaceActorsByFormationKind(void);
void Battle_PlaceMapMarkers(void);
void Battle_ReservedNoOp2A08(void);
void Battle_ReservedNoOp9B2C(void);
void Battle_ReservedNoOpF674(void);
s32 Battle_ResolveTargetAction(struct BattlePlan *plan, s32 slot);
s32 Battle_RunEncounterFar(s32 entrance);
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand);
void BattleActor_RemoveFromLists(s32 unit_id);
s32 BattleActor_ResetRuntimeFields(s32 actor);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleActor_UpdateSpritePriorities(void);
void BattleBackground_Load(s32 mode, s32 resource, s32 level);
void BattleBackground_LoadFar(s32, s32, s32);
void BattleEffect_ClearAllObjects(void);
void BattleEffect_ClearOutOfBoundsObjects(void);
void BattleEffect_InitializeBuffers(void);
void BattleEffect_InitializeSharedScene(void);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);
void BattleEffect_RunPhasedRadialParticleSequence(s32 arg);
void BattleEffect_RunTileAndPaletteAnimationFar(struct TransitionActorList *list);
s32 BattleEffect_SelectNearbyTargetObject(s32 sourceId, s32 battleMode);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_SpawnBurstParticleField(void);
void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect);
void BattleEffect_WipeCanvas(s32 mode, s32 value);
s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned);
void BattleEv_MarkDisplay(void);
void BattleEv_RunWait(s32 action, s32 flag);
void BattleEvent_Playback(void);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
s32 BattleFormation_BuildEnemyList(s32 record_id);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin);
void BattleFx_AdvanceAngleWithRamp(void *obj);
void BattleFx_AdvanceHueCycle(void);
void BattleFx_AdvanceScrollOnInterval(void);
void BattleFx_ArmBg0HBlankDma(void);
void BattleFx_BuildWindowEdgeTable(void);
u32 BattleFx_CheckDescriptorKind3Result(s32 descriptor, s32 value);
void BattleFx_CleanupResourcesAndWindow(void);
void BattleFx_ClearActorTimers(void);
void BattleFx_ClearChildValueOnMismatch(void);
void BattleFx_ClearOwnedSlotFar(struct EffectSlot *effect);
void BattleFx_ClearRandomParticlesFar(void);
void BattleFx_ComputeHueChannels(s32 value, s32 *maximum, s32 *center, s32 *minimum);
void BattleFx_ConfigureLinkedObject(s32 id, s32 flags);
s32 BattleFx_CopyLinkedObjectPosition(void *obj);
void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void BattleFx_DrawScaledObject(struct EffectSlot *object);
void BattleFx_EmitRandomParticleFromEmitter(struct ObjectRuntime *emitter);
s32 BattleFx_ExecutePackedAbilityEffect(s32 packed);
s32 BattleFx_FilterObjectIdByFlags(s32 arg0);
s32 BattleFx_FindDescriptorWithOverride(s32 arg0);
void BattleFx_FinishHeavyImpact(s32 id);
void BattleFx_FinishSceneAndReleaseHeapBlock(void);
s32 BattleFx_HasMatchingEvent5Far(u8 effect);
s32 BattleFx_HasReachedTargetFar(struct EffectSlot *);
s32 BattleFx_HasTriggerFar(s32 item);
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high);
void BattleFx_InitializeMode12(s32 *);
void BattleFx_InitializeSlots(void);
s32 BattleFx_InitializeStarField(s32 mode);
void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames);
s32 BattleFx_LookupResult(void *arg0);
s32 BattleFx_MapKeyThroughTable(s32 key);
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_RenderAnimationMode0(s32 effect);
void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_ResetCounters(void);
void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor);
void BattleFx_RunBurstParticles(void);
void BattleFx_RunDualTable(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunEffect15(void);
void BattleFx_RunFiveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunFlashingCallbackSequence(void);
void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunGatheringBurstFar(struct EffectLaunch *launch);
void BattleFx_RunItemBreakSequence(void);
void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunMercuryDjinnCapture(s32 arg);
void BattleFx_RunNoEffect(void);
void BattleFx_RunNoEffectFrames(s32 frame_count);
void BattleFx_RunOrbitingParticles(void);
void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunParticleFieldVariant(struct BattleEffectArgument *object, s32 variant);
void BattleFx_RunParticlePool(struct BattleEffectArgument *effect, s32 mode);
s32 BattleFx_RunProjectileVolley(void *effect, s32 mode);
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant);
void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunSeriesB(struct BattleEffectArgument *effect, u32 mode);
s32 BattleFx_RunSevenMode(s32, s32);
void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode);
void BattleFx_RunVenusDjinnCapture(s32 arg);
void BattleFx_RunVisibilityTransition(void);
void BattleFx_ScheduleCallbacksAndReleaseBlocksFar(void);
void BattleFx_ScheduleCloakedMapCheck(void);
void BattleFx_SelectLocationRule(s32 check);
void BattleFx_SelectResultPointer(s32 arg0);
void BattleFx_SetApproachMotion(s32 first, s32 second, s32 divisor);
void BattleFx_SetObjectAlternatingWords(u8 *object);
void BattleFx_SetPrimaryBufferValue(u32 value);
void BattleFx_SetTransitionFlagAndDisplay(void);
void BattleFx_SetupCanvasTileMap(void);
void BattleFx_SetupResourcesAndWindow(void);
void BattleFx_ShrinkObjectAndDestroyFast(void *obj);
void BattleFx_ShrinkObjectAndDestroySlow(void *obj);
void BattleFx_SnapScaleToFull(struct MotionObject *obj);
void BattleFx_SpawnFallingParticles(void);
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void BattleFx_SpawnRandomParticleAtPosition(const struct Source_0808f28c *source);
void BattleFx_StartBufferInterpolation(s32 mode);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SteerLiftedTarget(s32 target_id);
void BattleFx_SteerPushedTarget(s32 target_id);
void BattleFx_StepPaletteToResource(s32 resource_id);
s32 BattleFx_StepRatioTransition(void);
s32 BattleFx_SumCounters(void);
void BattleFx_SwayParticle(struct SparkObject *object);
void BattleFx_UpdateAllEffectSlots(void);
void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);
void BattleFx_UpdateDescendingParticleNegativeArc(struct ArcParticle *particle);
void BattleFx_UpdateDescendingParticlePositiveArc(struct ArcParticle *particle);
void BattleFx_UpdateEffect16State(void);
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle);
void BattleFx_UpdateParticleLinearMotion(struct BurstParticle *particle);
void BattleFx_UpdateRadialBurst(struct EffectSlot *effect);
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect);
void BattleFx_UpdateRadialMotion(struct EffectSlot *effect);
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect);
void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect);
s32 BattleFx_UpdateStarField(void);
void BattleFx_UpdateStormFlash(void);
void BattleFx_UpdateWaveScene(void);
void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect);
void BattleMap_ApplyEntranceView(void);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 mode);
s32 BattleMotion_GetSlotPalette(s32 id);
s32 BattleMotion_GetSlotPaletteFar(s32);
void BattleMotion_InitializeActorRecords(s32 id);
void BattleMotion_ProjectPositionFar(s32, struct EffectPosition *);
s32 BattleMotion_ReleaseObjectSlotByValue(s32 value);
void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id);
void BattleMotion_SetMode5AndActivateSlot(s32 unit_id);
void BattleMotion_SetupEscapeObject(s32 object_id);
void BattlePalette_UpdateBlend(void);
void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled);
s32 BattleParty_ApplyStatusDamage(void);
void BattleParty_CollectUnitList(void);
u32 BattleParty_IsUnitListed(u32 arg0);
s32 BattleParty_ListActorIdsFar(s32 side, s32 group);
void BattleParty_ListAllUnitsAndSubmit(void);
s32 BattleParty_PrepareActiveOwnersFar(s32);
s32 BattlePlacement_ContainsId(s16 *entries, s32 id);
s32 BattlePlacement_CountValidEntries(u32 unit_id, u8 *counts);
s32 BattlePlacement_CountValidEntriesFar(s32, u8 *);
s32 BattlePlacement_UpdateTimedEntriesFar(void);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_ClearAllActorRecordModes(void);
void BattlePres_ConfigureEffectDisplay(void);
void BattlePres_DrawTransitionRows(void);
void BattlePres_ProcessPendingTileTransfer(void);
s32 BattlePres_RunAction(struct BattleActionRecord *action);
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect);
void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind);
s32 BattlePres_RunPsynergyCast(struct BattlePlan *plan, s32 mode);
void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect);
s32 BattlePres_RunUnitAction(s16 *action);
void BattlePres_RunWithZeroArguments(void);
void BattlePres_SetActorModes(u16 *actors, s32 mode);
void BattlePres_SetActorModesFar(u16 *,s32);
void BattlePres_SetupTransitionAtPairMidpoint(s32 first, s32 second, s32 mode);
void BattlePres_SetupTransitionAtPairMidpointFar(s32 actor, s32 target, s32 mode);
void BattlePres_SetupTransitionSceneFar(s32 x, s32 depth, s32 y, s32 mode);
s32 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action);
s32 BattlePres_SyncTurn(void);
void BattlePres_UpdateHBlankScroll(void);
s32 BattlePres_WaitSync(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
s32 BattlePresentation_BuildActions(struct BattleActionRecord *actions);
s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay);
void BattlePresentation_DrawStreaks(void);
void BattlePresentation_InitializeWorkAndResetState(void);
void BattlePresentation_PrepareSceneFar(s32 kind);
s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattlePlan *plan);
s32 BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode);
void BattlePresentation_UpdateCamera(void);
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count);
s32 BattleTarget_SelectRandomPosition(s32 require_living_unit);
s32 BattleUnit_LookupWeaponValueByClass(s32 id);
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
void BattleUnit_RefreshPlacement(void);
void BattleUnit_ResetGuardLevels(void);
void BattleUnit_ResetStateByMode(s32 id, s32 mode);
void Bg0_ClearTilemap(void);
void BiribinoMura_RestoreRaisedEntrance(void);
void BiribinoMura_RestoreVillageActors(void);
void BiribinoMura_UpdateCornerSpawn(void);
void BiribinoNiwa_ApplyEntryState(void);
void BitDecoder_DecodeImage(void);
void Blend_SetBrightenTarget0(s32 duration);
void Blend_SetBrightenTarget16(s32 duration);
void Blend_SetDarkenTarget0(s32 duration);
void Blend_SetDarkenTarget16(s32 duration);
void Blend_WaitForTransition(void);
void BlendTransition_Update(void);
s32 CalculateFacingAngle(s32, s32);
void Camera_ApplyPhasedDelta(void);
s32 Camera_ApplyTransformByFlag(void);
void Camera_ConfigureScene(s32 pos);
void Camera_ConfigureSceneFar(s32 offset);
void Camera_InitDefaultTransform(void);
s32 Cgb_KeyToFrequency(u8 kind, u8 key, u8 fine);
void Cgb_UpdateChannels(void);
void CgbChannel_Mute(u8 channel);
s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled);
s32 CheckMapPositionCellOccupiedFar(const s32 *position);
void CheckObjectMapTile(void);
void Clear_UpdateBlend(void);
u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale);
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
void ConfigureAndPlaceActorFourteen(void);
s32 Curve_LookupScaledValue(s32 input, s32 halve);
void Debug_RunPaletteEditor(void);
void Debug_SelectAbilityPair(void);
void DebugBattle_ViewMessages(void);
void DialogueLayout_ConfigureGroupOne(void);
void DialogueLayout_ConfigureGroupThree(void);
void DialogueLayout_ConfigureGroupTwo(void);
void DialogueLayout_ConfigureRowsByFlag301(void);
void DialogueLayout_ConfigureTwoRegions(void);
void DisplayBlend_DisableRunScript(void);
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);
void DisplayScroll_InitObjectTable(void);
void DisplayScroll_RenderEnteringLine(void);
void DisplayScroll_UpdateObjects(void);
s32 Djinn_AddToLeastLoadedOwnerFar(s32, void *);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 djinn);
s32 Djinn_CheckTurnBalance(s32 from, s32 to);
void Djinn_CountTurns(u8 *counts);
s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 unit_id, s32 element, s32 index);
void Djinn_ShowJoinedMessageFar(s32, s32, void *);
s32 Djinn_TransferFar(s32 owner, s32 element, s32 index, s32 target);
void DjinnMenu_DrawElementList(struct DjinnListTable *tbl);
s32 DjinnMenu_RunScreen(void);
s32 DjinnMenu_SelectDjinn(s32 mode);
s32 DjinnMenu_ShowHelp(void);
u16 Effect_AdjustColorChannels(u16 color, s32 adj);
void Effect_AdjustPaletteColors(s32 amount);
void Effect_SetBg3HofsSplit(void);
void Effect_UpdateBg3HofsByVcount(void);
void EffectRuntime_ClearCurrentFlags(s32 id);
void EffectRuntime_SetMode2(s32 id);
void EffectRuntime_SetMode4AndPlayCue(s32 id);
void EffectRuntime_SetMode5AndPlayCue(s32 id);
void EffectRuntime_SetMode7AndLaunch(s32 id);
void EffectSlot_InitializeFar(struct EffectSlot *effect, s32 mode, s32 x, s32 z);
void EffectSlot_SetObjectModeFar(struct EffectSlot *effect, s32 value);
void EffectSlot_SetPositionFar(struct EffectSlot *, s32, s32);
void EffectSlot_UpdateFar(s32);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Engine_ActorFollow(s32 actor, s32 leader);
void Engine_DebugClearWindow(struct RenderInput *window);
void Engine_DebugDrawItemDetails(struct RenderInput *window, s32 item);
u8 *Engine_DebugGetAbility(s32 ability);
struct ItemDefinition *Engine_DebugGetItem(s32 item);
void Engine_DebugRedrawWindow(struct RenderInput *window);
void Engine_EventClearInvalidPackedValues(void);
void Engine_ObjectAttachWorkTarget(s32 id, s32 target);
void Engine_PartyRemoveActiveOwner(s32 id);
s32 Engine_PartyRemoveItem(s32 item);
void Engine_SetTriggerFlag(s32 flag);
s32 Engine_TestTriggerFlag(s32 flag);
s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *unit);
s32 Equipment_HasValueFar(s32 group, s32 entry);
void Event_ClearInvalidPackedValues(void);
s32 Event_FindFacingTrigger(s32 source);
s32 Event_GetSpecialValueFar(void);
void Event_NoOpHook(void);
void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot);
s32 Event_ValidatePackedId(u32 packed_id);
void EventActor_RunHopAndLeapSequence(s32 arg);
void EventTable_ApplyRowAbilities(s32 row_no);
s32 EventTable_CopyRowHeader(s32 row_no, s16 *output);
s32 EventTable_GetRowLimit(void);
s32 EventTable_GetRowType(s32 index);
void FacingObject_TurnPairToFaceEachOther(struct FacingObject *raw_first, struct FacingObject *raw_second);
s32 FarCall_BlankLoadTable(s32 entrance);
s32 FarCall_BlankRunTable(s32 entrance);
void FarCall_WindowTable(void);
s32 Field_CheckConfiguredKeys(void);
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z);
s32 Field_RunScene(s32 entrance);
void FieldEffect_SpawnNearbyMarkers(void);
void FieldEffect_UpdateSparkles(void);
void FieldEffect_WatchLeaderDistance(void);
void FieldEvent_RunTypeHandler(void);
void FieldEvent_ShowStatusMessage(void);
void FieldMotes_Update(void);
struct ObjectRuntime *FieldObject_Create(s32 id, s32 x, s32 y, s32 z);
void FieldObject_PlaceSceneActors(void);
void FieldScene_CallWith560And44(void);
void FieldScene_ConfigureFixedPointValues(void);
void FieldScene_ConfigureFlaggedActors(void);
void FieldScene_ConfigureLeadActors(void);
void FieldScene_ConfigureThreeActors(void);
void FieldScene_DispatchBySelector(void);
void FieldScene_DispatchSceneByIndex(void);
void FieldScene_DrawTilesByActor8Row(void);
void FieldScene_PlaceAndPinSlots10And11(void);
void FieldScene_PlaceAndPinSlots8And9(void);
void FieldScene_PlaceAndPinSlots8To10(void);
void FieldScene_RequestAndWaitFrames(s32 selector, s32 frames);
void FieldScene_RunActorEightApproach(void);
void FieldScene_RunActorEventSequence(void);
void FieldScene_RunActorNinePresentationCycles(void);
void FieldScene_RunActorPresentationSequence(void);
void FieldScene_RunActors24And25Setup(void);
void FieldScene_RunActors24And25SetupWithValue929(void);
void FieldScene_RunActors24And25SetupWithValue92a(void);
void FieldScene_RunActorSequence(void);
void FieldScene_RunActorTransferSequence(void);
void FieldScene_RunActorTwentyDialogueSequence(void);
void FieldScene_RunActorUpdateSequence(void);
void FieldScene_RunBranchingActorSequence(void);
void FieldScene_RunBranchingFormationPresentation(void);
void FieldScene_RunCommandSequence(s32 a0);
void FieldScene_RunExtendedActorSequence(void);
void FieldScene_RunExtendedFormationPresentation(void);
void FieldScene_RunFlag845And847Branches(void);
void FieldScene_RunFlagBranchedLayoutSteps(void);
void FieldScene_RunFlagBranchedSetupCascade(void);
void FieldScene_RunFourActorPresentation(void);
void FieldScene_RunLargeStagingSequence(void);
void FieldScene_RunLateSequenceHead(void);
void FieldScene_RunLeaderDropSequence(void);
void FieldScene_RunMainCutsceneSequence(void);
void FieldScene_RunPartyRosterScene(void);
void FieldScene_RunPillarBurst(void);
void FieldScene_RunPillarSequence(void);
void FieldScene_RunRoofEnsembleSequence(void);
void FieldScene_RunScene373SequenceB(void);
void FieldScene_RunScene38bSequenceA(void);
void FieldScene_RunScene39aSequenceA(void);
void FieldScene_RunScene3a2SequenceA(void);
void FieldScene_RunScene3a3SequenceD(void);
void FieldScene_RunScene3a7SequenceD(void);
void FieldScene_RunScene3b2SequenceA(void);
void FieldScene_RunScene3b8SequenceB(void);
void FieldScene_RunScene3c6SequenceA(void);
void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);
void FieldScene_RunScriptedSceneSequence(void);
void FieldScene_RunSevenActorEnsemble(void);
void FieldScene_RunShipDeckEventScript(void);
void FieldScene_RunSixSteps380To3A8(void);
void FieldScene_RunStep6(void);
void FieldScene_RunTwoArmSequence(s32 a);
void FieldScene_RunTwoCallSequence(void);
struct FieldActor *FindActorNearPosition(s32 x, s32 y);
s32 FindNearestF2Actor(void);
s32 Fixed_Remainder(s32 value, s32 divisor);
s32 FixedSqrt(s32 value);
void FourObjectMotion_ClearSlotsAndSchedule(void);
void FourObjectMotion_ClearSlotsAndScheduleAlt(void);
s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative);
void FourObjectMotion_UpdateAllPositions(void);
void FourObjectMotion_UpdateBottomRow(void);
s32 Func_080091b0(s32 layer, s32 x, s32 z);
void FuneHeya_AdvanceWalkerAfterRise(u8 *o);
void FuneHeya_AnnounceDeparture(void);
void FuneHeya_AnnounceTurn(void);
void FuneHeya_ApplyFlaggedLayout(void);
void FuneHeya_AskAboutMonsters(void);
void FuneHeya_CallActorEighteenToRow(void);
void FuneHeya_CallActorSixteenToRow(void);
void FuneHeya_CastOff(void);
void FuneHeya_DismissReplacementRowers(void);
s32 FuneHeya_FindFirstSetFlag(u32 group, s32 alternate);
struct FieldActor *FuneHeya_PlaceAnchorCharm(void);
void FuneHeya_SetupFirstRowingPair(void);
void FuneHeya_SetupReplacementPair(void);
void FuneHeya_SetupReplacementRower(void);
void FuneHeya_TurnActorToOpenSide(s32 a0);
void FuneHobashira_SetupHiddenLeaderLookout(void);
void FuneHobashira_SetupVisibleLeaderLookout(void);
void FuneHobashira_UpdateSway(void);
void FuneKanpan_ApplyEntryState(void);
void FuneKanpan_ArriveAtTolbi(void);
void FuneKanpan_ChooseSlotModes(void);
void FuneKanpan_PlaceDeckActors(s32 a0, s32 a1);
void FuneKanpan_PlaceLeaderOnDeck(void);
void FuneKanpan_PlaceRandomDeckActors(void);
void FuneKanpan_RockDeck(void);
void FuneKanpan_RunDeckCrewScene(void);
void FuneKanpan_RunJumpScene(void);
void FuneKanpan_SetupDeckCrewman(void);
void FuneKanpan_SetupHiddenLeaderDeparture(void);
void FuneKanpan_SetupRaisedCrewman(void);
void FuneKanpan_SetupVisibleLeaderDeparture(void);
void GameFlag_ClearBit(s32 flag);
s32 GameFlag_GetByteFar(s32 id);
s32 GameFlag_IsConditionActive(s32 condition);
void GameFlag_RefreshLureCapFar(void);
void GameState_InitDefaults(void);
void GameState_InitDefaultsFar(void);
struct FieldActor *GetActorState(s32 actor_id);
struct AnimationObject *GetBattleEffectObject(s32 kind);
s32 GetWorldMapTerrainBehaviorFar(void *, s32 *);
void GomaHashira_DriveActor13Idle(void);
void Graphics_AdjustPaletteBank(s32 arg0);
void Graphics_AdvancePaletteCycle(void);
s32 Graphics_ClampRgb555Channel(s32 val);
s32 Graphics_ClampRgb555Component(s32 val);
void Graphics_ClearBg0Vofs(void);
void Graphics_ClearBg1ControlBit2(void);
void Graphics_ClearCharacterBlockAndPalette(s32 alternate);
void Graphics_ExpandVramTilesByColorTableFar(void *);
void Graphics_ResetBg2Pa(void);
void Graphics_RestoreTransferWork(void);
s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
void Graphics_ScaleRgb555ClampedFar(void *, void *, s32, s32);
void Graphics_SetBg2AffineScaleHalf(void);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
u32 Graphics_UploadVramBlock(void);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);
void HaidiaArashi_RunRiverSearch(void);
void HaidiaArashi_UpdatePulsingGlow(void);
void HaidiaBabi_SpawnEffectPair(union FieldObject *object);
void ImiruFuchin_ApplyEntrySetup(void);
void ImiruFuchin_ApplyRoomLayout(void);
void ImiruFuchin_PlaceDragonsEye(void);
void InitializeEscapeSceneActors(void);
void InitializeOrbitingRenderEffect(s32 id);
void Inn_Cleanup(void);
void Inn_PlaySleep(s32 room_price);
s32 Inn_RoomPrice(s32 mode);
void Inventory_AddAndEquip(s32 owner, s32 target);
s32 Inventory_CheckDiscardFar(s32, s32);
s32 Inventory_CountFar(s32 unit_id);
s32 Inventory_GetEquippedItemFar(struct BattleUnit *unit, s32 slot);
void Inventory_RepairFar(s32 unit_id, s32 slot);
s32 Inventory_RequestMode(s32, s32, s32, s32);
s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode);
s32 Item_ClassifyUseAbility(s32 owner, s32 item);
s32 Item_FindSlot(s32 unit_id, s32 item_id);
s32 Item_GetEquipmentGroupFar(s32 item);
s32 Item_GetEquippedElementFar(s32 unit_id);
void Item_PlayUseAnimation(u32 item);
void ItemIcon_LoadTilesFar(s32 item);
void ItemMenu_ApplyFlags(const u8 *flags);
void ItemMenu_ArrangeCategoryItemIcons(u16 *items);
s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state);
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items);
s32 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state);
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags);
void ItemMenu_PosCategory(void);
void ItemMenu_ResetCategory(void);
void IwramHalt(void);
void KareiMachi_SetupEntryActors(void);
void KareiMachi_StartWindowTransition(void);
void KorimaHashi_AlternateBridgePostHeights(void);
void KorimaMagari_CaptureShakeScroll(void);
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row);
void KorosseoKawa_EnsureMarkerSlot(void);
void KorosseoKawa_RunPostMatchScene(s32 a0);
void KorosseoKawa_WaitForCountdown(void);
void KorosseoKawa_WaitForStageReady(void);
void KuupuappuDou_EnableGroupAutomaticPriority(void);
void KuupuappuDou_RaiseActorPriorities(void);
void KuupuappuDou_RunRumble(void);
s32 KuupuappuHeya_GetIwanVenusDjinnCount(void);
s32 KuupuappuHeya_IsActorNearPoint(s32 x, s32 z, s32 id);
s32 KuupuappuHeya_IsRecordNearPoint(s32 x, s32 y, s32 id);
void KuupuappuHeya_StartActorStops(void);
void KuupuappuHeya_UpdateActorStops(void);
s32 Link_CreateCountdownLabelWindow(void);
void Link_DrawShiftedTilePair(s32 offset);
s32 LinkLobby_ExchangePartyRecords(void);
s32 LinkLobby_PartyContains(s32 id);
s32 LinkLobby_PeerSlotMatches(s32 slot);
s32 LinkLobby_ReceivePartyRecords(void);
s32 LinkLobby_SendPartyRecords(void);
void LinkLobby_WriteSlotValue(s32 slot);
s32 Localization_LookupEntryIdFar(u32 speaker);
void Makyuri_ClearPalette(void);
void MakyuriChojo_FlickerActorEight(void);
void MakyuriHeya_OpenStoneDoor(s32 side);
void MakyuriHeya_RideLift(void);
void MakyuriHeya_SpawnSparkPair(void);
s32 MakyuriHeya_StartPillarPush(void);
void Map_ApplyWorkOriginAndSpanFar(void);
s32 Map_CheckObjectStep(struct ObjectRuntime *object, struct FieldPosition *position);
s32 Map_DisableUpdateCallbackFar(void);
s32 Map_EnableUpdateCallbackFar(void);
s32 Map_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);
void Map_InitializePerspectiveSceneFar(void);
void Map_LoadAreaGraphics(void);
s32 Map_LoadLayeredSceneFar(s32 index);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);
void Map_RenderAnimatedTileFrame(u8 *object, u32 position);
s32 Map_RenderAnimatedTileFramesForObjectFar(void *);
void Map_RenderPaletteMappedColumn(u32 value);
void Map_RenderPaletteMappedRow(u32 value);
void Map_ResumeAnimation(void);
s32 Map_ResumeAnimationFar(void);
void Map_SetCameraCenter(s32 x, s32 y);
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py);
void Map_ShowBg1FromBuffer(void);
void Map_ShowWorldMap(s32 picture);
void Map_UpdateCurrentTileBlock(void);
void Map_UpdateCurrentTileBlockFar(void);
void Map_UpdateCurrentTileBlockUntilBlocked(void);
void Map_UpdateLayerScroll(void);
void Map_UpdateWorldMapMarkers(void);
void Map_WaitWorkValuesBelow256(void);
s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
void MapActor_UpdateContact(void);
void MapAnimation_ApplyAffineFrame(void);
void MapAnimation_PresentFrame(void);
void MapAnimation_Update(void);
void MapEvent_RunTileTriggerSequence(void);
void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
s32 Math_Atan2(s32 dy, s32 dx);
u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count);
void Menu_AppendResourceEntry(s32 no);
void Menu_BuildPatternTiles(void);
void Menu_CancelSoundReset(void);
void Menu_CancelSoundTick(void);
void Menu_ClearFirstObjectRowAndScheduleUpdate(void);
void Menu_ClearSecondObjectRowAndScheduleUpdate(void);
s32 Menu_ComputeEntryValues(void *tbl);
u32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void Menu_CopyListOrder(s32 mode, u8 *order);
s32 Menu_CreateEightEntryObjects(s32 resource);
struct RenderInput *Menu_CreateWorkspaceWindows(void);
s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void Menu_DrawFlagBitTable(s32 window, s32 start_flag);
void Menu_DrawModeIndicator(void);
void Menu_DrawModeLabel(void);
void Menu_EndResourceSelection(void);
void Menu_EnsureCancelSound(void);
s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1);
s32 Menu_HandleFlagGridInput(s32 window, s32 *page, s32 *cursor);
s32 Menu_HandleSelectionRowInput(s32 arg, s16 value, s16 *sub, s16 *mode_arg);
void Menu_InitializeSelectedWorkspace(void);
void Menu_LayoutResourceEntries(s32 x, s32 y, s32 width, s32 resource_base);
void Menu_LoadSelectedResource(void);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index);
s32 Menu_OpenActionFlow(void);
s32 Menu_OpenBackdropScreen(void);
s32 Menu_OpenConfirmPromptFar(void);
void Menu_PlaceEntryObjectsInGrid(s32 origin_x, s32 origin_y, s32 phase);
void Menu_ReleaseEntryObjects(void);
void Menu_ReleaseEntryObjectsFar(void);
void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index);
void Menu_ResetTwoResourceEntries(void);
s32 Menu_ResolveSelectedAction(s32 *out_owner, s32 *unused, s32 *out_action);
s32 Menu_RunConfirmSelection(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 Menu_RunConfirmSelectionAtFar(s32, s32, s32);
s32 Menu_RunConfirmSelectionFar(s32 value, s32 x, s32 y, s32 z);
s32 Menu_RunResourceSelectionLoop(s32 initial);
void Menu_RunSelectedWorkspaceEntry(void);
s16 Menu_RunSelection(void);
void Menu_RunSelectionFar(void);
void Menu_RunSelectionWithCursorObjectFar(void);
void Menu_RunTopSelectionFar(void);
s32 Menu_RunWorkspaceOptions(void);
void Menu_RunWorkspaceResultLoopFar(s32 mode);
s32 Menu_RunWorkspaceSelectionLoop(void);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward);
s32 Menu_SelectEntry11To14Far(s32 choice);
s32 Menu_SelectEntry19To1cFar(s32 prev);
s32 Menu_SelectEntry20To21(s32 arg0);
s32 Menu_SelectResource(s32 start, s32 goal);
s32 Menu_SelectTopEntry(s32 sel);
void Menu_SendNodeCountList(struct SelectionScreen *screen);
s32 Menu_SetFirstObjectRowCoordinates(s32 arg0);
void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y);
void Menu_SetupSelectionBothSides(void);
void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index);
void Menu_StepLeft(struct SelectionScreen *state);
void Menu_StepRight(struct SelectionScreen *state);
void Menu_UpdateEntryObjectTransforms(void);
void Menu_UpdateFirstObjectRowPositions(void);
void Menu_UpdateSecondObjectRowPositions(void);
u32 Menu_WaitForSelectionInput(u32 mode);
void MenuSelection_DrawFrame(void);
void MenuSelection_DrawSideMarker(struct SelectionScreen *state, s32 index);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter);
void MusicPlayer_BeginFadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_Initialize(struct SoundPlayer *player, struct SoundTrack *tracks, u8 count);
void MusicPlayer_StepFade(struct SoundPlayer *player);
void MusicPlayer_StepVolumeAndPitchTowardTargetsFar(void);
void MusicPlayer_Stop(struct SoundPlayer *player);
void MusicPlayer_Tick(struct SoundPlayer *player);
void MusicTrack_CalcOutput(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_HandleNote(u32 command, struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_OperateWorkByte(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_ReleaseKey(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetLfoSpeedFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_SetModulationFromCommand(struct SoundPlayer *player, struct SoundTrack *track);
void MusicTrack_Stop(struct SoundPlayer *player, struct SoundTrack *track);
void *NewEffectObject(s32 first, s32 second, s32 third, s32 fourth);
void Object_AttachWorkTargetToObject(s32 id, s32 flag);
s32 Object_CallSpawnRoutineAtOrigin(s32 value);
void Object_DestroyIfPresent(void *object);
void Object_EffectSpawnCallback(void);
struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id);
s32 Object_GetScreenPosition(s32 object_id, s32 *position);
s32 Object_GetScreenPositionFar(s32 object, s32 *out);
s32 Object_GetTriggerTileAheadOfCurrent(void);
s32 Object_GetTriggerTileAheadOfCurrentFar(void);
s32 Object_IsTargetUnset(struct ObjectRuntime *object);
void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1);
void Object_UpdateAllMotion(void);
void Object_UpdateAllThumb(void);
void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value);
void ObjectDispatch_ApplyValueToKind200Children(s32 value);
void ObjectDispatch_InitFromTable0Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable1Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable2Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable3Far(struct FieldActor *actor);
void ObjectDispatch_RegisterChildMetadataFar(struct DispatchObject *object, s32 value);
void ObjectDispatch_WaitForValue16Far(struct ObjectRuntime *);
void ObjectEffect_BeginContextEffect25(void);
void ObjectEffect_BeginContextEffect26(void);
void ObjectEffect_EndContextEffect(s32 arg0);
void ObjectEffect_PrepareContextEffect(s32 value);
s32 ObjectEffect_RunPendingFlagEvent(void);
void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object);
void ObjectGroup_TickMemberTimers(void);
void ObjectSystem_Configure(s32 mode);
void ObjectSystem_Initialize(s32 mode);
void ObjectSystem_InitializeFar(s32 mode);
void ObjectSystem_UpdateCamera(void);
void ObjectSystem_UpdateCameraFixed(void);
void ObjectTable_ClearBattleSlots(void);
s32 ObjectTable_FindLastActiveId(void);
void ObjectTable_Restore(void);
void OptionMenu_InitializeWork(void);
void OverlayObject_ActivateSlotWithMode3(s32 a);
void OverlayObject_AdvancePositionByDelta(union FieldObject *raw);
void OverlayObject_ApplyRandomSlotOnOddFrames(union FieldObject *object);
void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void OverlayObject_InitObject22(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void OverlayObject_InitWithRandomFields(s32 a);
void OverlayObject_ResetMotionFields(s32 actor);
s32 OverlayObject_RunObjectTwoWhenFlagged(void);
s32 OverlayObject_SetFacingTowardObject10(void *self);
void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d);
void OverlayObject_SpawnKind24AtActor(struct FieldActor *actor);
s32 Owner_ApplyLevelGains(s32 owner, s32 levels);
s32 Owner_DetermineClass(s32 character, const u8 *djinn);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
u32 Owner_GetLevelThresholdFar(s32 owner_id, s32 level);
struct EnemyDefinition *Owner_GetRecordFar(s32 class_id);
s32 Owner_GetResistanceValue(s32 owner, s32 index);
s32 Owner_GetResistanceValueFar(s32 unit, s32 element);
s32 Owner_GetValueIfLevelThresholdReachedFar(s32 unit_id, s16 *gains);
void Owner_InitRecords(void);
s32 Owner_LookupFourColumnTable(s32 row, s32 column);
void Owner_RefreshClassActionsFar(s32 owner);
void Owner_UpdateRatioPairFar(struct BattleUnit *, s32);
s32 OwnerAction_RunCompareLoop(s32 mode);
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta);
void Palette_CopyObjectBankToBackground14(void);
void Palette_DarkenSceneStep(void);
void Palette_LightenBankHighlight(s32 bank);
u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b);
void Palette_StepTowardResource(s32 resource_id);
void PaletteDma_LoadBlock(void);
s32 PaletteFar_Entry00(s32 mode);
void PaletteFar_Entry20(s32 mode);
void PaletteGlow_UpdateSine(void);
void Party_AdjustCounterCappedAt28Far(s32 value);
void Party_AdvanceOwnerCountToTargetFar(s32 owner, s32 level);
void Party_Apply(s32, u16 *);
void Party_ApplyStatePreset(void);
s32 Party_CheckMemberValueTotal(s32 id);
s32 Party_ComputeEligibleMemberAverage(s32 record_id);
void Party_Do(void *);
s32 Party_GetAverageLevelFar(void);
s32 Party_RemapCharacterIdByFlags(s32 arg0);
s32 Party_RemapCharacterIdByFlagsFar(u16 value);
s32 Party_RemapCharacterIdIfEnabledFar(s8 kind, s8 variant);
void Party_ResolveTablePair(void);
void Party_SetReturnPoint(s32 reason);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 PartyInventory_AddFar(s32 item);
s32 PartyInventory_CountItemFar(s32 item);
void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset);
s32 PsynergyMenu_DrawActionPage(s32 window, s32 unused, const struct MenuResult *state);
s32 PsynergyMenu_DrawDetailPage(s32 window, s32 *work, struct MenuResult *state);
s32 PsynergyMenu_DrawRangePage(s32 window, s32 unused, struct MenuResult *state);
s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner);
void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns);
s32 PsynergyMenu_RunList(s32 pane);
s32 PsynergyMenu_SelectAction(void);
s32 PsynergyMenu_SelectOwner(void);
s32 PsynergyMenu_SelectTarget(s32 mode);
void RamakanSabaku_RaiseQuarterTriggers(void);
void RamakanSabaku_RunSandstorm(void);
u32 Random16Far(void);
void ReelGame_InitTitle(void);
void ReelGame_Run(void);
void ReelGame_RunFrame(void);
void render_animated_tile_frameFar(u8 *record, u32 frame);
void Render_ResetTransformState(void);
void RenderOutput_PrepareForRedrawFar(s32 window);
void RenderOutput_Release(struct RenderOutput *entry);
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag);
void ResetSceneTransitionEffect(void);
struct FacingObject *ResolveFacingObject(s16);
void Resource_ClearOwnerListAndCounters(void);
s32 Resource_DecodeType2(const void *source, void *destination);
void Resource_DecompressHalfwords(const void *source, void *destination);
u8 *Resource_DecompressLz(const u8 *source, u8 *destination);
s32 Resource_FindFreeSlot(s32 key);
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
s32 Resource_GetTableEntryFar(void);
void Resource_InitializeTable(void);
s32 Resource_LoadByMode(s32 mode, s32 value);
u16 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
s32 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1);
s32 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index);
void Resource_LoadPairedBlocksIfAvailable(void);
void Resource_LoadWorkHeader(void);
void Resource_ResetOwnerEntries(void);
void Resource_ResetPendingTransfer(void);
void Resource_ScheduleOwnerReset(void);
void Resource_ScheduleOwnerResetDelayed(void);
void Resource3ba_NoOpCallback(void);
void ResourceMetadata_ClearRecordFar(void *record);
s32 ResourceMetadata_RegisterFar(struct AnimationObject *context, s32 effect_id);
s32 ResourceMetadata_SumCommandLengthsFar(s32 battle_value, s32 second, s32 third);
void ResourceMetadata_UnregisterFar(struct AnimationObject *context, s32 entry_handle);
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant);
s32 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 value, u32 variant);
s32 ResourceTable_CountFreeBlocks(void);
void RunActorsEightAndNineMapEvent(void);
s32 RunAssetSelectionScreenFar(void);
void RunBattleEffect03(void);
void RunBattleEffect05(void);
void RunBattleEffect07(void);
void RunBattleEffect08(void);
void RunBattleEffect11(void);
void RunBattleEffect13(void);
void RunBattleEffect16(void);
s32 RunBattlePresentation(struct BattlePlan *plan, s32 mode);
s32 RunFlashEraseVerifier(u8 *, u8 *);
void RunScene59Sequence(void);
void RunSceneSelectionChain(void);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b);
s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
struct BattleUnit *Runtime_GetObject(s32 object_id);
void Runtime_ReleaseHeapBlock10(void);
void Runtime_RemapBytesByTableFar(void *, s32);
void Runtime_ScheduleCallbackAndReleaseBlock20A(void);
void Runtime_ScheduleCallbackAndReleaseBlock20B(void);
void Runtime_SetWorkTripleIfNonNegativeFar(s32 x, s32 y, s32 z);
void Runtime_WriteDebugTextTiles(const u8 *src);
s32 Sanctum_RunPartyService(void);
void SaveState_CaptureObjectTableFar(void);
void Scene_RepairTheHouse(void);
void Scene_ResetFlagsOnEnter(s32 unused, s32 keep);
void Scene_ResolveInteractionResult(void);
void Scene_RunActorExchange(void);
void Scene_RunEastParticleWaveSequence(void);
void Scene_RunExtendedPresentationSequence(void);
void Scene_RunFourActorProgressPresentation(void);
void Scene_RunFourActorStagingSequence(void);
void Scene_RunPairedParticleWaveSequence(void);
void Scene_RunParticleWaveSequence(void);
void Scene_RunScene371SequenceA(s32 palette);
void Scene_UpdateFormationActor10Flags(void);
void Scene_UpdateFormationActor11Flags(void);
void Scene_UpdateFormationActor12Flags(void);
void Scene_UpdateFormationActor13Flags(void);
void Scene_UpdateFormationActor14Flags(void);
void Scene_UpdateFormationActor9Flags(void);
void Scene_UpdateOuterActor10Flags(void);
void Scene_UpdateOuterActor9Flags(void);
void Scene_UpdatePuzzleActors(void);
void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);
void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay);
void SceneActor_ApplyPositionsOfActors11And12(void);
void SceneActor_ApplySlotsMatchingKind212(void);
void SceneActor_BranchOnSlotZeroAtTile38(void);
void SceneActor_CheckActors8To11NearSlotZero(void);
void SceneActor_ClearActorModeAndSetState5(s32 no);
void SceneActor_FaceActors24And25TowardActorZero(void);
void SceneActor_InitSlots10To15AndStartTask(void);
s32 SceneActor_LiftLowActorOnSubjectTile(s32 subject_actor);
void SceneActor_MarkSlot13AndSetFlag200(void);
void SceneActor_MarkSlot21AndSetFlag205(void);
void SceneActor_PassActorNinePositionWithId107(void);
void SceneActor_PlaceActors20To27(void);
void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 delay);
void SceneActor_PlaceSlots1To3FromWork(void);
void SceneActor_PublishMarkerBySlotZeroHeight(void);
void SceneActor_PushObjectAheadIfLevel(void);
void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);
void SceneActor_RunActorCommandWithFlag91(s32 x);
void SceneActor_RunWhenActor9AtTile45x43(void);
s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct FieldActor *actor);
void SceneActor_SetFlagByteBySlotZeroPosition(void);
void SceneActor_SetMode55OnSevenRecords(void);
void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c);
void SceneActor_SetupActors11To14AndInstallTask(void);
void SceneActor_SetupSlotTwenty(void);
void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc);
void SceneActor_WaitHeightBelowLimit(struct FieldActor *obj, s32 limit);
s32 SceneData_GetDifferenceOfPairSums(void);
void SceneData_SelectBlockAndResetCounters(u32 mode, u32 param);
s32 SceneData_SelectTableEntryByFlagGroup(u32 sel);
s32 SceneDialogue_PickTopicVariantId(s32 topic);
void SceneDialogue_PromptAndCountSkip(s32 x);
void SceneDialogue_RunActorFourteenFlagDialogue(void);
void SceneEffect_AdjustPaletteColors(s32 a);
void SceneEffect_AdvanceGatedRiseCounter(struct FieldActor *actor);
void SceneEffect_AdvancePositionByAxisMode(struct PillarEffect *effect);
void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot);
void SceneEffect_InitSlotsEightToNineteen(void);
void SceneEffect_LoadTablesAndStopDma0(void);
void SceneEffect_RestoreBlendRegisters(void);
void SceneEffect_RunActorBurst(s32 no);
void SceneEffect_SelectSlotValueAndPosition(s32 id, s32 slot, s32 flags);
void SceneEffect_SetupBlendByFlag201(void);
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor);
void SceneEffect_UpdateLobeOrbitEffect26(void);
void ScenePlacement_ClipToView(s32 placements);
void SceneState_ApplyFiveRectsAtColumn78(void);
void SceneState_ApplyFlagGatedActorEightSetup(void);
void SceneState_ApplyRectAndMarkActor16(void);
void SceneState_ApplyRectAndSetupActor19(void);
void SceneState_ApplyRectsByFlag844(s32 flag);
void SceneState_ApplyRectsByFlatla384And962(void);
void SceneState_ApplyStepToSlots15To18(void);
void SceneState_ApplyThreeRects(void);
void SceneState_ApplyThreeRectsRows9And10(void);
void SceneState_ApplyTwoRects(void);
void SceneState_ApplyTwoRectsAtRow56(void);
void SceneState_CallWith432And32(void);
void SceneState_CheckFlags941And940(void);
void SceneState_CheckPositionWindow(void);
void SceneState_ClearSlotsBySubState(void);
void SceneState_ConfigureEntries8Through19(void);
void SceneState_ConfigureRegion26_30AndClearActor18Mode(void);
void SceneState_ConfigureRegion26_30AndMarkActor17(void);
void SceneState_CountDownEveryFortyTicks(void);
void SceneState_DispatchByActorZeroDepth(void);
s32 SceneState_FindFirstSetFlagOfGroup(u32 sel);
void SceneState_InitActorSlots8To19(void);
void SceneState_InitControlWhenFlag109Clear(s32 resource);
void SceneState_InitFourActorRecordsAndInstallTask(void);
void SceneState_LinkRecordZeroWhenFlag200Clear(void);
void SceneState_RaiseTriggerOnPsynergyTarget(void);
void SceneState_ResetCounterAndStartTask(void);
void SceneState_RunFlagGatedSetupCascade(void);
void SceneState_RunGuardedActorStep(s32 x);
void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b);
void SceneState_Send210AndApplyRectAt40x84(void);
void SceneState_SetFlagByActorPosition(void);
void SceneState_SetMode66AndPassOpeningSequence(void);
void SceneState_TriggerColumnNineteen(void);
void SceneState_TriggerColumnTen(void);
void SceneTable_Prepare(void *);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Scheduler_ScheduleCallbackA(void);
void Scheduler_ScheduleCallbackAAfterFrames(void);
struct FxObject *ScriptObject_FindOverlappingEntryFar(struct FxObject *object, struct Output_08097f80 *position);
void ScrollFar_Entry00(s32 mode);
void SerialRuntime_Initialize(void);
void ShianJiin_AskIfCurious(void);
s32 ShindenHeya_ChooseRestartOption(void);
void ShindenHeya_RaiseItemIcon(s32 item);
void ShindenHeya_ResetActorUpdate(void);
void ShindenHeya_UpdateRisingSpark(union FieldObject *object);
s32 Shop_CanServe(s32 entry_no, s32 kind);
void Shop_CopyGlyphs(s32 arg0, u8 *arg1, u32 arg2);
s32 Shop_CountUnits(void);
struct RenderOutput *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y);
void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id);
s32 Shop_DrawItemPage(s32 window, s32 unused, struct MenuResult *state);
void Shop_DrawSelMsg(s32 target, s32 selection);
void Shop_FillSelectorFar(s32 value, s32 column, void *tiles);
s32 Shop_GetSelectionState(s32 owner, s32 slot);
void Shop_HiliteUnit(s32 enabled, s32 selected);
void Shop_InitEffect(void);
void Shop_InitEffectFar(void);
void Shop_InitializeCursorWork(void);
s32 Shop_MsgByMode(s32 value);
s32 Shop_PickUnitItemFar(s32 *owner, s32 *slot);
void Shop_RepairItem(s32 unit_id, s32 slot);
void Shop_ResetEffects(void);
void Shop_RestoreSceneTiles(s32 layer);
void Shop_RestoreSceneTilesFar(s32 value);
void Shop_RunPartyMemberIconBurst(s32 member);
s32 Shop_SelBuy(void);
void Shop_SellItem(s32 unit_id, s32 slot, s32 count);
s32 Shop_SelSell(s32 unit_id);
s32 Shop_SelUse(s32 actor);
s32 Shop_ServicePrice(s32 entry_no, s32 kind);
void Shop_StepCursor(void);
void SoruIriguchi_ApplyEntryState(void);
void SoruSekizo_RunSealOpenedSequence(void);
void Sound_Mixer(void);
void Sound_TickDmaRestartTimer(void);
void SoundDriver_EnterMixer(void);
s32 Sprite_ComposeAnimationFrame(struct AnimationObject *obj, u16 dir);
void StagedActor_PlacePairAtOffsetAndRun(s32 actor_id, s32 dx, s32 dz);
void StagedActor_StepDownUntilClamp(s32 index);
void State_ApplyRectByLayoutSelector(void);
s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode);
void State_StampRecordCells(s16 *records, s32 value);
void State_UpdateScrollRegistersWithPreset(void);
s32 StoryActor_ApplyMapRotation(struct FieldActor *actor);
void StoryScene_StartTransition(void);
void Summon_ClearWorkFields(void);
s32 Summon_GetEntryByte4(s32 index);
s32 Summon_GetEntryFlag1Field(s32 index);
s32 Summon_GetEntryValue(s32 index);
u32 Summon_GetLayoutKind(s32 index);
s32 Summon_IsEntryFlagged(s32 index);
s32 Summon_IsEntrySecondaryFlaggedFar(s32 index);
void Summon_LayoutPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions);
s32 SummonSlot_RegisterActorSprites(s32 unit);
void System_VBlankHandler(void);
void TakaraAshiba_InitializeScene1(void);
void TakaraAshiba_InitializeScene2(void);
void TakaraAshiba_InitializeScene3(void);
void TakaraAshiba_OpenPlatformPassage(s32 a0);
void TakaraAshiba_RaiseTriggerOnStand(void);
void TakaraAshiba_RepaintActorCells(void);
void TakaraAshiba_RestoreActorEightTerrain(void);
void TakaraAshiba_RestoreActorTenLeftCell(void);
void TakaraAshiba_RestoreActorTenRightCell(void);
s32 TakaraHashira_LowerActorToLedge(s32 id, s32 far);
void TakaraHashira_PushPillar(void);
void TakaraHashira_SetCellAttributes(s32 layer, s32 x, s32 y, struct MapCell *src);
s32 TakaraHashira_SyncPriorityIfAhead(struct FieldActor *front, struct FieldActor *actor);
s32 TakaraHashira_SyncPriorityIfBehind(struct FieldActor *actor, struct FieldActor *front);
void TakaraShima_LeaveClearedRoom(void);
void TakaraShima_RestoreRoomActors(void);
void Text_FormatHexToWork(u32 value);
void *Text_FormatPlayTime(s32 value, u8 *out);
void Text_FormatSignedDecimalToWork(s32 arg0);
void Tilemap_ConvertBuffer(void);
void Title_PlaceIntroSprites(void);
void TorebiIzumi_RevealPrize(s32 item);
s32 TorebiIzumi_RunSpringRide(s32 side);
void TorebiIzumi_SetActorActive(s32 arg0, s32 arg1);
void ToretoEda_StartBg3Split(void);
void ToretoHeya_ApplyFlaggedMapPatches(void);
void ToretoHeya_RunLandingDustScene(void);
void ToretoHeya_UpdateSwirlObject(struct Swirl *obj);
void ToretoPalette_CaptureBank(void);
s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit);
s32 Trade_CanOfferDjinnFar(s32 unit_id, s32 element, s32 index);
void Transform_UpdateVertices(void);
s32 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested);
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag);
s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot);
s32 Ui_ClearVramBlock(void);
void Ui_DrawValuePairRows(struct BattleUnit *unit, s32 layer);
void Ui_FillBank15PaletteGrey(void);
void Ui_FillGridColumnFromMetadata(s32 slot, s32 value);
s32 Ui_FindNextNumberWithMetadata(s32 start, s32 step);
s32 Ui_GetTableWordZeroFar(s32 index);
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);
void Ui_LoadWindowGraphics(void);
void Ui_PrepareTransferFromTableEntry(u32 index);
void Ui_SetGridColumnNumber(s32 slot, s32 no);
void Ui_SetGridColumnParameter(s32 slot, s32 value);
void Ui_SetGridColumnPriority(s32 slot, s32 value);
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y);
s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot);
void UiIcon_PrepareObject(struct RenderOutput *object);
s32 UiMenu_CreateCursor(void *source);
void UiMenu_PositionCursor(s32 x_offset, s32 y_offset);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMessage_ShowAndRestoreState(s32 message_id);
void UiMessage_ShowAndWait(s32 arg0);
s32 UiMessage_ShowChoice(s32 arg0);
s32 UiMessage_ShowChoiceVariant(s32 arg0);
void UiMessage_ShowResolvedAndRestoreState(s32 arg0);
void UiMessage_ShowResolvedAndWait(s32 value);
s32 UiText_BuildRenderEntriesMode1(s32 arg0);
void UiText_CopyMessageStringFar(s32 message_id, void *buffer, s32 length);
void UiText_DecodeMessageFar(s32 message, u16 *buffer, s32 length);
void UiText_DrawAt(s32, s32, s32, s32);
void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStatComparison(const struct BattleUnit *alt, const struct BattleUnit *base, s32 work);
void UiText_DrawWorkValueWithLabel(s32 work);
s32 UiText_GetResourceDimensionsAltFar(s32 message, s32 *x, s32 *y, u32 *width, u32 *height);
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile);
void UiText_MeasureResourceEntriesFar(s32 message, s32 *width, s32 *height);
s32 UiText_OpenEntryMessageFar(s32 window, s32 message);
void UiText_PrepareMessageWorkFar(s32 message);
void UiText_RenderWideStringAtOffsetFar(void *buffer, struct UiWindow *destination, s32 offset, s32 terminator);
void UiTextResource_Release(s32 value);
void UiTimedNotice_CloseIfActiveFar(void);
void UiTimedNotice_CreateFar(s32 scene);
void UiTimedNotice_Tick(void);
void UiWindow_BuildLayoutBounds(s32 flags);
void UiWindow_Clear(s32 window);
void UiWindow_Commit(s32 window);
void UiWindow_CreateWithLayoutBounds(s32 flags);
void UiWindow_CreateWithLayoutBoundsFar(s32 value);
void UiWindow_DrawFrameFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_DrawPartyStatusContents(s32 flags);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_FillFromSceneFar(u16 *lines);
void UiWindow_InitializeWork(s32 unused);
void UiWindow_MarkVisibleTileAttributesFar(void);
void UiWindow_OpenMode1AndWaitFrame(void);
void UiWindow_SetPaletteBitRectFar(s32, s32, s32, s32, s32);
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position);
s32 UiWindow_UpdateOrCreate(s32 *handle, s32 first, s32 second, s32 third, s32 fourth, s32 flags);
struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *work, s32 value, s32 preserve);
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *channel);
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *channel);
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *channel);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel);
void UiWork_DrainPendingFar(s32);
void UiWork_DrawByAttributes(struct UiWindow *window);
void UiWork_FinalizeAndReleaseBlock16(void);
void UiWork_FinalizeAndReleaseBlock16Far(void);
void UiWork_FinalizeSharedSlotFar(void);
void UiWork_InitCountersAndScheduleRefresh(s32 initialize);
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
void UiWork_InitializeFar(s32 mode);
s32 UiWork_IsIdle(struct UiWindow *window);
void UiWork_ProcessAll(void);
void UiWork_ProcessDirectWork(void);
void UiWork_ProcessRenderChannels(void);
void UiWork_ResetChannelTransition(struct UiChannelSlot *channel);
void UiWork_ResetFreeChannelFar(void);
void UiWork_SetAltFlagAndClearTableFar(s32 value);
void UiWork_SetBusyFlagsFar(s32);
void UiWork_SetTwoEntriesTo999(void);
s32 UiWork_StepChannelScript(struct UiChannelSlot *ch);
void UiWork_UploadDirtyBlocks(void);
void UiWork_WaitWindowDuration(struct UiWindow *window);
s32 Unnamed_080ae2f4(void);
void Unnamed_080d1714(s32 *);
void Unnamed_080ea0d8(s32 *);
s32 UpdateActorProximity(u8 *actor);
s32 UpdateFacingFromResolvedObject(struct FacingObject *object);
void UpdateRisingParticleBurst(void *object);
void VinasuChojo_StepSpriteRotation(struct FieldActor *actor);
void VinasuChojo_UpdateOrbitingEffect(union FieldObject *object);
void VinasuHeya_LowerFloatingBlocks(s32 wait);
s32 VinasuHeya_UpdateRisingSpray(struct FieldActor *actor);
void WorldMap_ActivateSite138(s32 actor);
void WorldMap_CompleteTransfer(void);
void WorldMap_DisembarkCarriage(void);
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
void WorldMap_RunBlackOrbScene(void);
void WorldMap_RunOpeningRise(void);
void WorldMap_StopLeaderLeavingShip(void);
void WorldMap_TransferToExit102(void);
void WorldMap_TransferToExit103(void);
void WorldMap_TransferToExit104(void);
void WorldMap_TransferToExit106(void);
void WorldMap_TransferToFuneHeya(void);
void WorldMap_UpdateView(void);
void YamaRama_DisableActorCollision(void);

struct AbilityListEntry;
struct AnimationEntry;
struct AnimationObject;
struct ArcEffectObject;
struct ArcObject;
struct BattleAction;
struct BattleActionRecord;
struct BattleEffectArgument;
struct BattleObjectSlot;
struct BattlePlan;
struct BattleSummonState;
struct DispatchObject;
struct EffectDescriptor;
struct EffectObject_02000c5c;
struct EffectPosition;
struct EffectSlot;
struct EffectVector;
struct FieldActor;
struct ItemBreakFragmentSource;
struct LinkedCaptureObject;
struct MakyuriLights;
struct MapActor;
struct MenuResult;
struct MotionObject;
struct NumberRow;
struct ObjectRuntime;
struct OrbitEffect;
struct OwnerActionSlot;
struct PlacementDescriptor;
struct RenderInput;
struct RenderOutput;
struct SceneObject_02000b98;
struct ScenePlacement;
struct SceneRegionEntry;
struct ScriptObjectEntry;
struct ShopCursor;
struct SparkObject;
struct TileRun;
struct TileRun2;
struct UiBobPairOutput;
struct UiScaleSprite;
struct UiWindow;

#ifndef OWN__call_via_r0
void _call_via_r0(u32 routine);
#endif
#ifndef OWN_Ability_GetData
struct BattleAction * Ability_GetData(s32 action_id);
#endif
#ifndef OWN_Animation_ApplyChildPalette
void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette);
#endif
#ifndef OWN_Animation_SetWorkEntry
void Animation_SetWorkEntry(struct AnimationEntry *work, s32 no);
#endif
#ifndef OWN_Battle_DispatchInputEvent
s32 Battle_DispatchInputEvent(s32 event);
#endif
#ifndef OWN_Battle_FindTaggedSlotByValue
s32 Battle_FindTaggedSlotByValue(u32 value);
#endif
#ifndef OWN_Battle_GetObjectTableValueFar
s32 Battle_GetObjectTableValueFar(s32 actor_id);
#endif
#ifndef OWN_Battle_GetTaggedSlotValue
s16 Battle_GetTaggedSlotValue(s32 slot);
#endif
#ifndef OWN_Battle_ProcessRoundEnd
s32 Battle_ProcessRoundEnd(void);
#endif
#ifndef OWN_Battle_RunEncounter
s32 Battle_RunEncounter(s32 enemy_group);
#endif
#ifndef OWN_BattleAction_RunDescriptor
s32 BattleAction_RunDescriptor(s32 arg0);
#endif
#ifndef OWN_BattleActor_SpawnObjectsForList
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
#endif
#ifndef OWN_BattleCommand_ExecuteSelectedAction
s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction);
#endif
#ifndef OWN_BattleCommand_ExecuteSelectedItem
s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot);
#endif
#ifndef OWN_BattleCommand_SelectAutomatic
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *request, s32 retry);
#endif
#ifndef OWN_BattleEffect_ApplyToTargets
s32 BattleEffect_ApplyToTargets(s32 effect_id, s32 source_id, s32 target_id, s32 fixed_scale);
#endif
#ifndef OWN_BattleEffect_RunCirclingFallingScene
void BattleEffect_RunCirclingFallingScene(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleEffect_RunEmberColumns
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleEffect_RunStagedParticles
void BattleEffect_RunStagedParticles(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleEffect_RunTargetedItemBreak
void BattleEffect_RunTargetedItemBreak(void);
#endif
#ifndef OWN_BattleEffect_SelectNearbyObject
s32 BattleEffect_SelectNearbyObject(s32 sourceId);
#endif
#ifndef OWN_BattleFx_AdvanceSpinAngle
void BattleFx_AdvanceSpinAngle(struct MotionObject *object);
#endif
#ifndef OWN_BattleFx_ApplyColorToTargetBuffer
void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode);
#endif
#ifndef OWN_BattleFx_ApplyLookupResult
s32 BattleFx_ApplyLookupResult(s32 arg0, s32 arg1);
#endif
#ifndef OWN_BattleFx_CallEffect03AndStop
void BattleFx_CallEffect03AndStop(void);
#endif
#ifndef OWN_BattleFx_CallEffect04
void BattleFx_CallEffect04(void);
#endif
#ifndef OWN_BattleFx_CallEffect05
void BattleFx_CallEffect05(void);
#endif
#ifndef OWN_BattleFx_CircleAnchor
void BattleFx_CircleAnchor(struct OrbitEffect *effect);
#endif
#ifndef OWN_BattleFx_DispatchByIdRangeFar
void BattleFx_DispatchByIdRangeFar(s32 *arg0);
#endif
#ifndef OWN_BattleFx_DispatchModeFar
void BattleFx_DispatchModeFar(s32 *state);
#endif
#ifndef OWN_BattleFx_FindDescriptor
struct EffectDescriptor * BattleFx_FindDescriptor(s32 kind, s32 value);
#endif
#ifndef OWN_BattleFx_GetCycledTableWord
u16 BattleFx_GetCycledTableWord(void);
#endif
#ifndef OWN_BattleFx_GetPhaseResult
unsigned char BattleFx_GetPhaseResult(s32 phase_index);
#endif
#ifndef OWN_BattleFx_GetResourceGroup
s8 BattleFx_GetResourceGroup(s32 effect_index);
#endif
#ifndef OWN_BattleFx_GetResourceId
s32 BattleFx_GetResourceId(u32 id);
#endif
#ifndef OWN_BattleFx_GetResourceIdFar
s32 BattleFx_GetResourceIdFar(u16);
#endif
#ifndef OWN_BattleFx_HalveDistanceToTarget
void BattleFx_HalveDistanceToTarget(struct LinkedCaptureObject *object);
#endif
#ifndef OWN_BattleFx_InitializeMode10
void BattleFx_InitializeMode10(struct BattleEffectArgument *efx);
#endif
#ifndef OWN_BattleFx_InitializeMode6
void BattleFx_InitializeMode6(struct BattleEffectArgument *efx);
#endif
#ifndef OWN_BattleFx_LoadResourceGroup
void BattleFx_LoadResourceGroup(void);
#endif
#ifndef OWN_BattleFx_MarkChildAndRunFallbackTransition
void BattleFx_MarkChildAndRunFallbackTransition(void);
#endif
#ifndef OWN_BattleFx_PlayUnitElementEffect
void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant);
#endif
#ifndef OWN_BattleFx_RenderMode5
void BattleFx_RenderMode5(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleFx_ResumeObject
void BattleFx_ResumeObject(void);
#endif
#ifndef OWN_BattleFx_RunBurstParticleMainObject
void BattleFx_RunBurstParticleMainObject(void);
#endif
#ifndef OWN_BattleFx_RunCastingImpact
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind);
#endif
#ifndef OWN_BattleFx_RunDescriptorAction
s32 BattleFx_RunDescriptorAction(s32 id);
#endif
#ifndef OWN_BattleFx_RunEffect13Hook
void BattleFx_RunEffect13Hook(void);
#endif
#ifndef OWN_BattleFx_RunEventAction
s32 BattleFx_RunEventAction(void *arg0, s32 arg1, s32 arg2);
#endif
#ifndef OWN_BattleFx_RunIceFleckBurstFar
void BattleFx_RunIceFleckBurstFar(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleFx_RunKind6DescriptorAction
s32 BattleFx_RunKind6DescriptorAction(s32 arg0);
#endif
#ifndef OWN_BattleFx_RunObjectRow
void BattleFx_RunObjectRow(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleFx_RunSparkEmitter
void BattleFx_RunSparkEmitter(struct SparkObject *object);
#endif
#ifndef OWN_BattleFx_SetCallbackWhenTargetUnset
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *target);
#endif
#ifndef OWN_BattleFx_SetSpecialFromTable
void BattleFx_SetSpecialFromTable(s32 arg0, s32 arg1);
#endif
#ifndef OWN_BattleFx_ShrinkObjectScaleUntilHalf
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj);
#endif
#ifndef OWN_BattleFx_SpawnDescendingArcParticles
void BattleFx_SpawnDescendingArcParticles(struct ArcObject *source);
#endif
#ifndef OWN_BattleFx_SpawnItemBreakMode1
struct MotionObject * BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
#endif
#ifndef OWN_BattleFx_SpawnRadialParticleRing
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin);
#endif
#ifndef OWN_BattleFx_StartOrbitingParticles
void BattleFx_StartOrbitingParticles(void);
#endif
#ifndef OWN_BattleFx_UpdateItemBreakFragment
void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *source);
#endif
#ifndef OWN_BattleFx_UpdatePairedArcSpawner
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *object);
#endif
#ifndef OWN_BattleFx_UpdateScaledArcObjectA
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj);
#endif
#ifndef OWN_BattleFx_UpdateScaledArcObjectB
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj);
#endif
#ifndef OWN_BattleFx_UpdateShrinkingOrbitObject
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *object);
#endif
#ifndef OWN_BattleLayout_HighlightPartyPanels
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
#endif
#ifndef OWN_BattleMotion_ProjectScaledPosition
s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected);
#endif
#ifndef OWN_BattleParty_ReconcileDjinnRecovery
s32 BattleParty_ReconcileDjinnRecovery(void);
#endif
#ifndef OWN_BattleParty_ResetActiveRuntimeFields
s32 BattleParty_ResetActiveRuntimeFields(void);
#endif
#ifndef OWN_BattlePres_BuildOpponentEntries
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
#endif
#ifndef OWN_BattlePres_BuildUnitEntries
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
#endif
#ifndef OWN_BattlePres_RunItem
s32 BattlePres_RunItem(struct BattlePlan *plan, s32 unused, struct BattleActionRecord *selection);
#endif
#ifndef OWN_BattlePres_SetActorModeAndAction
s32 BattlePres_SetActorModeAndAction(s32 id);
#endif
#ifndef OWN_BattlePresentation_BuildSortedUnitEntries
s32 BattlePresentation_BuildSortedUnitEntries(struct BattleActionRecord *entries);
#endif
#ifndef OWN_BattlePresentation_BuildTilemap
void BattlePresentation_BuildTilemap(s32 *destination);
#endif
#ifndef OWN_BattlePresentation_ConfigurePaletteFade
void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade);
#endif
#ifndef OWN_BattlePresentation_ConfigurePaletteFadeFar
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, u16 value, s32 fade);
#endif
#ifndef OWN_BattlePresentation_SetPaletteLevelFar
void BattlePresentation_SetPaletteLevelFar(s32 unused, s32 level);
#endif
#ifndef OWN_BattlePresentation_SpawnActorObject
void BattlePresentation_SpawnActorObject(struct BattleObjectSlot *actor, s32 unit, s32 x, s32 y);
#endif
#ifndef OWN_BattlePresentation_WaitForPromptAt
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y);
#endif
#ifndef OWN_BattleRandomPercent
u32 BattleRandomPercent(void);
#endif
#ifndef OWN_BattleSummon_UpdateAvailability
struct BattleSummonState * BattleSummon_UpdateAvailability(void);
#endif
#ifndef OWN_BattleTarget_SelectForAction
s32 BattleTarget_SelectForAction(s32 actor_id, struct BattleAction *action);
#endif
#ifndef OWN_BattleUnit_ProcessTurnEnd
s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan);
#endif
#ifndef OWN_Camera_ApplyShake
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
#endif
#ifndef OWN_Camera_StoreSceneParameters
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2);
#endif
#ifndef OWN_Camera_WorldToScreen
void Camera_WorldToScreen(struct EffectVector *position);
#endif
#ifndef OWN_ColorBuffer_BackupAndBrighten
void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
#endif
#ifndef OWN_ColorBuffer_BackupAndDarken
void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
#endif
#ifndef OWN_ColorBuffer_BackupAndHalve
void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes);
#endif
#ifndef OWN_ColorBuffer_BackupAndScaleThreeQuarters
void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
#endif
#ifndef OWN_CountRemainingErasedFlashBytes
s32 CountRemainingErasedFlashBytes(u8 *ptr);
#endif
#ifndef OWN_DisplayBlend_StartScript
void DisplayBlend_StartScript(u16 *script);
#endif
#ifndef OWN_DisplayScroll_InitializeHBlankDma
void DisplayScroll_InitializeHBlankDma(s32 mode, s32 freq_x, s32 step_x, s32 amp_x, s32 freq_y, s32 step_y, s32 amp_y);
#endif
#ifndef OWN_Djinn_DeactivateFar
u32 Djinn_DeactivateFar(s32 owner, s32 index, s32 bit);
#endif
#ifndef OWN_Djinn_ListOwnerEntries
s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element);
#endif
#ifndef OWN_DjinnMenu_ShowCurrentList
s32 DjinnMenu_ShowCurrentList(void);
#endif
#ifndef OWN_EffectPosition_ApplyAnimationAndYOffset
void EffectPosition_ApplyAnimationAndYOffset(s32 id, struct EffectPosition *position);
#endif
#ifndef OWN_EffectRuntime_GetCurrentObject
struct ObjectRuntime * EffectRuntime_GetCurrentObject(s32 id);
#endif
#ifndef OWN_EffectRuntime_LookupByTableEntry
s32 EffectRuntime_LookupByTableEntry(u32 index);
#endif
#ifndef OWN_EffectRuntime_PrepareRisingObject
void EffectRuntime_PrepareRisingObject(struct ObjectRuntime *object);
#endif
#ifndef OWN_EffectSlot_Initialize
void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z);
#endif
#ifndef OWN_Event_RequestDestination
void Event_RequestDestination(u16 first, u16 second);
#endif
#ifndef OWN_FarCall_EffectTable
s32 FarCall_EffectTable(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);
#endif
#ifndef OWN_Field_RunTileAction
s32 Field_RunTileAction(s32 tile);
#endif
#ifndef OWN_Field_TryJumpForward
s32 Field_TryJumpForward(void);
#endif
#ifndef OWN_FieldScene_BuildSceneDescriptorAndInstallTask
void FieldScene_BuildSceneDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre, s32 extra, s32 third, s32 fourth);
#endif
#ifndef OWN_FieldScene_PrepareActors
void FieldScene_PrepareActors(struct SceneRegionEntry *entry);
#endif
#ifndef OWN_FourObjectMotion_InitializeBottomRow
void FourObjectMotion_InitializeBottomRow(void);
#endif
#ifndef OWN_Game_ResetForNewGameFar
void Game_ResetForNewGameFar(s32 mode);
#endif
#ifndef OWN_GameFlag_IsSet
s32 GameFlag_IsSet(s32 flag);
#endif
#ifndef OWN_GetMotionRecordFar
void * GetMotionRecordFar(struct MotionObject *object, s32 record_index);
#endif
#ifndef OWN_Graphics_BuildSequentialTileTable
void Graphics_BuildSequentialTileTable(s32 *destination);
#endif
#ifndef OWN_Graphics_ScaleRgb555
s32 Graphics_ScaleRgb555(u16 *source, u16 *destination, s32 scale, s32 count);
#endif
#ifndef OWN_Graphics_TransformSmallPalette
void Graphics_TransformSmallPalette(s32 index, s32 transform);
#endif
#ifndef OWN_Inn_CheckIn
s32 Inn_CheckIn(s32 mode, s32 object_id);
#endif
#ifndef OWN_Inventory_DiscardFar
s32 Inventory_DiscardFar(s32 owner, s32 slot);
#endif
#ifndef OWN_Inventory_EquipFar
s32 Inventory_EquipFar(s32 owner, s32 slot);
#endif
#ifndef OWN_Inventory_FindEquippedFar
s32 Inventory_FindEquippedFar(s32 owner, s32 type);
#endif
#ifndef OWN_InventoryMenu_ShowModalMessage
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y);
#endif
#ifndef OWN_Item_AdjustCounterFar
s32 Item_AdjustCounterFar(s32 item_id, s32 delta);
#endif
#ifndef OWN_Item_EncodeBankedId
u32 Item_EncodeBankedId(u32 value);
#endif
#ifndef OWN_Item_IsCompatibleWithOwnerFar
s32 Item_IsCompatibleWithOwnerFar(s32 owner_id, s32 item_id);
#endif
#ifndef OWN_Link_DrawShiftedTilePairFar
void Link_DrawShiftedTilePairFar(s32 offset);
#endif
#ifndef OWN_LinkLobby_PollPeerReady
s32 LinkLobby_PollPeerReady(void);
#endif
#ifndef OWN_Makyuri_SpawnLightObjects
void Makyuri_SpawnLightObjects(s32 region, struct MakyuriLights *st);
#endif
#ifndef OWN_Map_LoadAreaGraphicsFar
void Map_LoadAreaGraphicsFar(void);
#endif
#ifndef OWN_MapGroupTable_SelectEntry
void MapGroupTable_SelectEntry(s32 kind);
#endif
#ifndef OWN_Menu_OpenSelectionWindow
void Menu_OpenSelectionWindow(s32 mode, u32 count);
#endif
#ifndef OWN_Menu_SpawnFourObjectsAtOrigin
void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y);
#endif
#ifndef OWN_MenuSelection_BuildEntries
void MenuSelection_BuildEntries(void);
#endif
#ifndef OWN_Object_CollectResources
s32 Object_CollectResources(struct AbilityListEntry *output);
#endif
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
#ifndef OWN_Object_InitializeMode
s32 Object_InitializeMode(struct AnimationObject *state, s32 flags);
#endif
#ifndef OWN_Object_ReplaceResourceEntry
void * Object_ReplaceResourceEntry(void *src, void *alt);
#endif
#ifndef OWN_Object_SetMoveTargetFar
void Object_SetMoveTargetFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
#endif
#ifndef OWN_Object_SetPositionAndResetMotionFar
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
#endif
#ifndef OWN_ObjectDispatch_ApplyValueToChildrenFar
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object, s32 value);
#endif
#ifndef OWN_ObjectDispatch_InitFromTable4WithArgument
void ObjectDispatch_InitFromTable4WithArgument(struct DispatchObject *object, s32 argument);
#endif
#ifndef OWN_ObjectDispatch_RegisterChildMetadata
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object, s32 value);
#endif
#ifndef OWN_ObjectGroup_ApplyRandomChildValues
void ObjectGroup_ApplyRandomChildValues(void *owner);
#endif
#ifndef OWN_ObjectGroup_SetActionForOthers
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excluded_object, s32 group_mode, s32 action);
#endif
#ifndef OWN_ObjectGroup_SetChildValueUnlessFifteenFar
void ObjectGroup_SetChildValueUnlessFifteenFar(void *raw_parent, u32 value);
#endif
#ifndef OWN_ObjectPlacement_CreateGroup
void ObjectPlacement_CreateGroup(struct RenderInput *origin, s32 x, s32 y, struct PlacementDescriptor *descriptor);
#endif
#ifndef OWN_ObjectTable_ResetForObject
void ObjectTable_ResetForObject(struct ScenePlacement *table);
#endif
#ifndef OWN_ObjectVisual_CopyAttributes
void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id);
#endif
#ifndef OWN_OverlayObject_PrepareObject
void * OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth);
#endif
#ifndef OWN_OverlayObject_WaitUntilSettledAndReset
void OverlayObject_WaitUntilSettledAndReset(u8 *obj);
#endif
#ifndef OWN_Owner_RecalculateStatsFar
void Owner_RecalculateStatsFar(s32 owner);
#endif
#ifndef OWN_OwnerAction_DiffSlots
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b, u16 *out, s32 *first_count, s32 *second_count);
#endif
#ifndef OWN_PaletteGlow_UpdateFar
void PaletteGlow_UpdateFar(s32 phase, s32 brightness);
#endif
#ifndef OWN_Party_AdjustSixDigitCounterBFar
s32 Party_AdjustSixDigitCounterBFar(s32 amount);
#endif
#ifndef OWN_Party_GetAverageLevel
u32 Party_GetAverageLevel(void);
#endif
#ifndef OWN_Party_LookupCharacterValueByFlag32
s32 Party_LookupCharacterValueByFlag32(u32 index);
#endif
#ifndef OWN_Party_ReservedNoOp5B14
void Party_ReservedNoOp5B14(void);
#endif
#ifndef OWN_PartyTalkMenu_Choose
s32 PartyTalkMenu_Choose(void);
#endif
#ifndef OWN_PsynergyMenu_CallIconRoutineWithValue
void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1);
#endif
#ifndef OWN_PsynergyMenu_DrawListPage
s32 PsynergyMenu_DrawListPage(s32 window, s32 unused, const struct MenuResult *res);
#endif
#ifndef OWN_PsynergyMenu_DrawRange
void PsynergyMenu_DrawRange(s32 window, s32 x, s32 y, s32 range, s32 unused);
#endif
#ifndef OWN_RamakanSabaku_EmitSandEffect
s32 RamakanSabaku_EmitSandEffect(struct FieldActor *actor);
#endif
#ifndef OWN_RenderOutput_Create
struct RenderOutput * RenderOutput_Create(s32 slot, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
#endif
#ifndef OWN_RenderResource_CreateFrame
struct RenderOutput * RenderResource_CreateFrame(s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4);
#endif
#ifndef OWN_RenderResource_CreatePair
struct RenderOutput * RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y);
#endif
#ifndef OWN_Runtime_GetBuildStampTimeFar
u32 Runtime_GetBuildStampTimeFar(void);
#endif
#ifndef OWN_Runtime_GetLowTableAddress
s32 Runtime_GetLowTableAddress(void);
#endif
#ifndef OWN_Runtime_GetRemainingEwram
s32 Runtime_GetRemainingEwram(void);
#endif
#ifndef OWN_Runtime_GetRemainingIwram
s32 Runtime_GetRemainingIwram(void);
#endif
#ifndef OWN_RuntimeDispatch_NoOpHook
void RuntimeDispatch_NoOpHook(void);
#endif
#ifndef OWN_Scene_FadeColorFromWhite
s32 Scene_FadeColorFromWhite(void);
#endif
#ifndef OWN_Scene_PushBlockAlongRun
void Scene_PushBlockAlongRun(struct TileRun2 *runs);
#endif
#ifndef OWN_SceneActor_PlaceFiveActorsInRow
void SceneActor_PlaceFiveActorsInRow(u8 *p);
#endif
#ifndef OWN_SceneActor_UpdateTracking
s32 SceneActor_UpdateTracking(struct MapActor *actor, struct MapActor *target, s32 distance_limit, s32 force_tracking);
#endif
#ifndef OWN_SceneData_FindTileRunAt
const struct TileRun * SceneData_FindTileRunAt(const struct TileRun *run, s32 x, s32 y);
#endif
#ifndef OWN_SceneEffect_SpawnEffect284AtCell
struct EffectObject_02000c5c * SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2);
#endif
#ifndef OWN_SceneEffect_SpawnRandomizedParticleEveryFourFrames
s32 SceneEffect_SpawnRandomizedParticleEveryFourFrames(struct SceneObject_02000b98 *obj);
#endif
#ifndef OWN_SceneTransform_ApplyPosition
void SceneTransform_ApplyPosition(const s32 *position);
#endif
#ifndef OWN_SceneTransform_ApplyScale
void SceneTransform_ApplyScale(const s32 *scale);
#endif
#ifndef OWN_ScriptObject_CheckOverlapFar
s32 ScriptObject_CheckOverlapFar(struct ScriptObjectEntry *object, s32 *values);
#endif
#ifndef OWN_Shop_BuySpecialItem
void Shop_BuySpecialItem(void *window, s32 item_window);
#endif
#ifndef OWN_Shop_FillSelector
void Shop_FillSelector(s32 count, s32 selector, u8 *base);
#endif
#ifndef OWN_Shop_Run
s32 Shop_Run(s32 row, s32 keeper_id);
#endif
#ifndef OWN_Shop_SetCursor
void Shop_SetCursor(struct ShopCursor *cursor, s32 target_x, s32 target_y, s8 kind);
#endif
#ifndef OWN_Shop_SetCursorFar
void Shop_SetCursorFar(struct ShopCursor *cursor, s32 target_x, s32 target_y, s8 kind);
#endif
#ifndef OWN_ShopCursor_AdvanceFar
void ShopCursor_AdvanceFar(struct ShopCursor *cursor);
#endif
#ifndef OWN_ShopCursor_MoveTowardTargetFar
void ShopCursor_MoveTowardTargetFar(struct ShopCursor *cursor);
#endif
#ifndef OWN_ShopCursor_SetPositionImmediateFar
void ShopCursor_SetPositionImmediateFar(struct ShopCursor *cursor, s32 x, s32 y);
#endif
#ifndef OWN_SideObject_CreateFar
struct RenderOutput * SideObject_CreateFar(s32 object_kind, s32 position, s32 side, s32 arg3, s32 arg4, s32 arg5);
#endif
#ifndef OWN_StagedActor_CountdownUntilPositionUnset
s32 StagedActor_CountdownUntilPositionUnset(struct FieldActor *o);
#endif
#ifndef OWN_StartFlashTimer
void StartFlashTimer(u8 index);
#endif
#ifndef OWN_StatusMenu_DrawCharacterSummary
void StatusMenu_DrawCharacterSummary(s32 surface, u8 *st);
#endif
#ifndef OWN_StoryActor_ConfigureSpawnedObject
void StoryActor_ConfigureSpawnedObject(struct FieldActor *actor);
#endif
#ifndef OWN_TakaraHashira_ReadMapCell
void TakaraHashira_ReadMapCell(s32 layer, s32 x, s32 y, u32 *dst);
#endif
#ifndef OWN_Text_FormatHex
void Text_FormatHex(u32 value, u32 count, u8 *output);
#endif
#ifndef OWN_Tilemap_DecodeStagedBuffer
void Tilemap_DecodeStagedBuffer(s32 size);
#endif
#ifndef OWN_Title_ShowSplashScreen
s32 Title_ShowSplashScreen(s32 mode);
#endif
#ifndef OWN_Trade_AddOfferFar
u32 * Trade_AddOfferFar(u32 kind, u32 first, u32 second);
#endif
#ifndef OWN_Ui_AdjustValueWithoutLimit
void Ui_AdjustValueWithoutLimit(s32 no, s32 value);
#endif
#ifndef OWN_Ui_ApplyTableOffsetToPair
void Ui_ApplyTableOffsetToPair(struct UiBobPairOutput *obj);
#endif
#ifndef OWN_Ui_ApplyTableScaleToObject
void Ui_ApplyTableScaleToObject(struct UiScaleSprite *obj);
#endif
#ifndef OWN_UiGlyph_LoadEntryWithPalette
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse);
#endif
#ifndef OWN_UiIcon_BuildAbilityIconTiles
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse);
#endif
#ifndef OWN_UiIcon_CreateWithLoadedResource
s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id);
#endif
#ifndef OWN_UiIcon_Draw
struct RenderOutput * UiIcon_Draw(s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y);
#endif
#ifndef OWN_UiIcon_DrawVariantWithTileOffset
s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant);
#endif
#ifndef OWN_UiIcon_PrepareObjectFar
void UiIcon_PrepareObjectFar(struct RenderOutput *object);
#endif
#ifndef OWN_UiNumber_DrawAt
void UiNumber_DrawAt(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
#endif
#ifndef OWN_UiText_DrawFourNumbersInRow
void UiText_DrawFourNumbersInRow(struct RenderInput *work, struct NumberRow *row);
#endif
#ifndef OWN_UiText_DrawMessage
void UiText_DrawMessage(s32 no, s32 flags);
#endif
#ifndef OWN_UiText_DrawStringInWindowFar
void UiText_DrawStringInWindowFar(u8 *text, s32 arg1, u32 x, u32 y);
#endif
#ifndef OWN_UiText_GetResourceDimensions
s32 UiText_GetResourceDimensions(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
#endif
#ifndef OWN_UiText_GetResourceDimensionsFar
s32 UiText_GetResourceDimensionsFar(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
#endif
#ifndef OWN_UiText_RenderWideStringAtOffset
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x, s32 y);
#endif
#ifndef OWN_UiWindow_ClearInteriorTiles
void UiWindow_ClearInteriorTiles(const struct RenderInput *window, u32 left, u32 top, u32 right, u32 bottom);
#endif
#ifndef OWN_UiWindow_ClearInteriorTilesFar
void UiWindow_ClearInteriorTilesFar(const struct RenderInput *window, u32 left, u32 top, u32 right, u32 bottom);
#endif
#ifndef OWN_UiWindow_CreateWithSideObjectFar
s32 UiWindow_CreateWithSideObjectFar(s32 arg0, s32 arg1, s32 x, s32 y);
#endif
#ifndef OWN_UiWindow_FillTilemapRect
void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height);
#endif
#ifndef OWN_UiWindow_MapTextCanvasTiles
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width);
#endif
#ifndef OWN_UiWork_CloseAndRelease
void UiWork_CloseAndRelease(void);
#endif
#ifndef OWN_UiWork_FinalizeEntityMatchingLocalizedId
void UiWork_FinalizeEntityMatchingLocalizedId(void);
#endif

#endif
