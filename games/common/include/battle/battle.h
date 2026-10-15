#ifndef BATTLE_H
#define BATTLE_H

/* BATTLE */

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "battle_effect_runtime.h"
#endif
#if !GAME_TLA
#include "farcall.h"
#endif

struct BattleActionRecord;
struct BattleEffectArgument;
struct BattlePlan;
struct BattleUnit;
struct DispatchObject;
struct EffectSlot;
struct ObjectRuntime;
struct SceneAreaNameRule;

extern u16 gBattleFx_CyclePatternWords[];
extern s8 gBattleFx_RandomChildValues[];
extern const s8 gBattlePlacement_StepPairs[];
#if GAME_TLA
extern u8 gEncounter_AreaEntryTable[];
#endif
extern u8 gPlayerObjectId[];
extern u16 gResource_SlotAssignments[];
#if GAME_TLA
s32 BattleTargetExec(struct BattlePlan *plan, s32 slot);
#endif
struct BattleObj;
#ifndef OWN_BattlePresentation_SpawnActorObject
#if GAME_TLA
void BattleObjMake(struct BattleObj *actor, s32 unit, s32 x, s32 y);
#endif
#endif
#ifndef OWN_BattleActor_SpawnObjectsForList
#if GAME_TLA
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
#endif
#endif
#if GAME_TLA
void BattleEffect_InitializeSharedScene(void);
void BattleEvent_Playback(void);
void BattleFx_DrawScaledObject(struct EffectSlot *object);
#endif
#ifndef OWN_BattleFx_GetResourceGroup
#if GAME_TLA
s8 BattleFx_GetResourceGroup(s32 effect_index);
#endif
#endif
#if GAME_TLA
void BattleFx_PrepareBufferInterpolation(void);
void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunNoEffect(void);
void BattleFx_RunOrbitingParticles(void);
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode);
void *BattleFx_StartItemBreak(void *object);
s32 BattleMotion_GetSlotPalette(s32 id);
void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId);
void BattleMotion_SetMode5AndActivateSlot(s32 unitId);
void BattleMotion_SetupEscapeObject(s32 objectId);
#endif
#ifndef OWN_BattlePres_BuildOpponentEntries
#if GAME_TLA
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
#endif
#endif
#ifndef OWN_BattlePres_BuildUnitEntries
#if GAME_TLA
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
#endif
#endif
#if GAME_TLA
s32 BattlePres_WaitSync(void);
s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions,
                                           s32 count);
void BattleUnit_ResetGuardLevels(void);
void Camera_InitDefaultTransform(void);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
void RunBattleEffect05(void);
s32 Summon_IsEntryFlagged(s32 index);
#endif
#if GAME_TLA
void BattlePres_SetActorModesFar(u16 *, s32);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x,
                                         s32 y, s32 z);
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object,
                                            s32 value);
void Owner_UpdateRatioPairFar(struct BattleUnit *, s32);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_FinalizeSharedSlotFar(void);
#endif
#if GAME_TLA
void UpdateRisingParticleBurst(void *object);
#endif

#if !GAME_TLA
struct AnimationEntry;
struct AnimationObject;
struct ArcEffectObject;
struct ArcObject;
struct ArcParticle;
struct BattleAct;
struct BattleActionRecord;
struct BattleEffectArgument;
struct BattleEffectEntry;
struct BattleEventState;
struct BattleGroupOrder;
struct BattleLocationRule;
struct BattleObj;
struct BattlePlan;
struct BattlePresentationTransition;
struct BattleSummonState;
struct BattleUnit;
struct DescendingOrbitObject;
struct EffectDescriptor;
struct EffectSlot;
struct EnemyDefinition;
struct FieldActor;
struct FxObject;
struct FxPair;
struct ItemBreakFragmentSource;
struct LinkedCaptureObject;
struct MotionObject;
struct ObjectRuntime;
struct OrbitEffect;
struct PolarOffsetOutput;
struct RockWallHeights;
struct SceneInteractionEntry;
struct ScriptObjectEntry;
struct BattleSource;
struct SparkObject;
struct TransitionActorList;
struct WaveFxWork;

struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};
struct BgScrollWords {
  u16 unk00;
  u16 unk02;
  u16 unk04;
  u16 unk06;
};
struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};
struct Ring {
  s32 x;
  s32 y;
  s32 velX;
  s32 velY;
  s32 age;
};
struct ScatterPosition {
  s32 x;
  s32 y;
  s32 z;
};
struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};
extern u8 gAnimation_ChildPaletteCycle[];
extern u8 gBattleCommand_SelectionWeights[3][8];
extern const u8 gBattleFx_ArcSparkTiles[];
extern const s32 gBattleFx_BurstParticleObjectScript[];
extern struct BattleEffectEntry gBattleFx_DefinitionTable[];
extern u8 gBattleFx_GlintCellHeights[];
extern u16 gBattleFx_GlintCellOffsets[];
extern u8 gBattleFx_GlintCellWidths[];
extern const s32 gBattleFx_LinkedObjectScript[];
extern const s32 gBattleFx_MarkerParticleScript[];
extern const s32 gBattleFx_ParticleEmitterScript[];
extern const s32 gBattleFx_ParticleScript[];
#ifndef OWN_BattleFx_PuffCells
extern u16 gBattleFx_PuffCells[];
#endif
#ifndef OWN_BattleFx_PuffSizes
extern u8 gBattleFx_PuffSizes[];
#endif
extern u32 gBattleFx_PulseScales[];
extern const s32 gBattleFx_SourceHoldScript[];
extern s16 gBattleFx_TargetRangeByMode[];
extern const s32 gBattleFx_UntargetedObjectScript[];
extern u16 gBattleFx6_FlareCells[];
extern u8 gBattleFxPillar_Counts[];
extern u8 gBattleFxPillar_Kinds[];
extern u16 gBattleFxPillar_PuffCells[];
extern u8 gBattleFxPillar_PuffHeights[];
extern s8 gBattleFxPillar_PuffWidths[];
extern s8 gBattleFxPillar_X[];
extern s32 gBattleMotion_VariantAcceleration[];
extern s32 gBattleMotion_VariantDistancePercent[];
extern s32 gBattleMotion_VariantSpeedLimit[];
extern s32 gBattleMotion_VariantVelocityY[];
extern s8 gBattleParty_CenterOrderOffsets[];
extern const struct BattleGroupOrder gBattleParty_RoundEndGroupOrder;
extern const s32 gBattlePres_ActorObjectScript[];
extern u8 gBattlePres_AdvanceArrowTiles[];
extern const u8 gBattlePres_CurtainTiles[];
extern const u8 gBattlePres_TileVariants[];
extern u16 gBattleUnit_WeaponAnimsClass1[];
extern u16 gBattleUnit_WeaponAnimsClass2[];
extern u16 gBattleUnit_WeaponAnimsClass3[];
extern u16 gBattleUnit_WeaponAnimsClass5[];
extern u8 BitDecoder_Size[];
extern u8 gBurstScene_Records[];
extern u8 gCamera_FlagTransformWork[];
extern u8 gCastingImpact_GlintDrawFlags[];
extern u8 gCastingImpact_ImageX[];
extern u8 gCastingImpact_ImageY[];
extern u16 gCastingImpact_OrbitCells[];
extern u8 gCirclingScene_ObjectColumns[];
extern u8 gCirclingScene_ObjectRows[];
extern u16 gCirclingScene_PuffSheetOffsets[];
extern u16 gCirclingScene_PuffSizes[];
extern u8 gCounterReveal_PanelX[];
extern u8 gCounterReveal_PanelY[];
extern const u8 gBattleFx_SparkTiles[];
extern const u8 gBattleFx_QuadrantModes[];
extern const s32 gBattleFx_TargetColors[];
extern s32 gBattleFx_StarWorkOffsets[];
extern u8 gBattleFx_StarSizes[];
extern s32 gBattleFx_RingFrameOffsets[];
extern s8 gBattleFx_HighlightBlendRamp[];
extern u8 gDualTable_ColumnCounts[];
extern u8 gDualTable_ColumnX[];
extern u8 gDualTable_HeightLimits[];
extern u16 gDualTable_PillarCells[];
extern s8 gDualTable_PillarHeights[];
extern u8 gDualTable_PillarWidths[];
extern u16 gDualTable_PuffCells[];
extern u8 gDualTable_PuffHeights[];
extern s8 gDualTable_PuffWidths[];
extern u16 gDualTable_VineCells[];
extern u8 gDualTable_VineWidths[];
extern u8 gEarthWall_CellHeights[];
extern u16 gEarthWall_CellSourceOffsets[];
extern u8 gEarthWall_CellWidths[];
extern u8 gEmberColumns_Columns[];
extern s32 gEmberColumns_Gravity[];
extern u16 gEncounter_EnemyGroupTable[];
extern u8 gFallingBolts_Counts[];
extern s16 gFieldFx_GroundParticleFrames[];
extern const u8 gFieldFx_GroundParticleTiles[];
extern u8 gFirePillars_Counts[];
extern s8 gFirePillars_Depths[][4];
extern u8 gFirePillars_StartFrames[];
extern s8 gFireSwirl_CellBiasX[];
extern s8 gFireSwirl_CellBiasY[];
extern u8 gFireSwirl_CellHeights[];
extern u16 gFireSwirl_CellSourceOffsets[];
extern u8 gFireSwirl_CellWidths[];
extern s8 gFiveMode_Records[];
#ifndef OWN_gBattleBgFxWork
extern struct WaveFxWork *gBattleBgFxWork;
#endif
#ifndef OWN_gBattleFxWork
extern void *gBattleFxWork[];
#endif
#ifndef OWN_gBattleOwnerStates
extern u8 *gBattleOwnerStates;
#endif
#ifndef OWN_gRandomSeed
extern u32 gRandomSeed;
#endif
#ifndef OWN_gTransitionWork
extern struct BattlePresentationTransition *gTransitionWork;
#endif
extern u8 gHomingEmbers_Counts[];
extern u8 gHomingEmbers_FlareBiasY[];
extern u16 gHomingEmbers_FlareCells[];
extern u8 gHomingEmbers_FlareHeights[];
extern u8 gHomingEmbers_FlareWidths[];
extern u8 gLightningBolts_Counts[];
extern u8 gLightningBolts_GlintModes[];
extern u8 gLightningBolts_GlintPalettes[];
extern u8 gMemberBeam_HeadHeights[];
extern u16 gMemberBeam_HeadOffsets[];
extern u8 gMemberBeam_HeadWidths[];
extern u8 gMemberBeam_MotePictures[];
extern u8 gMemberBurst_Counts[];
extern const u8 gObjectMotion_LinkedActionScript[];
extern const struct FxPair gObjectRow_RisePair;
extern const struct FxPair gObjectRow_SweepPair;
extern u8 gPaletteParticles_DropSizes[];
extern u8 gPaletteParticles_DropSpeeds[];
extern u8 gPaletteParticles_SparkPixels[];
extern u8 gPaletteParticles_StreakDrops[];
extern u8 gPaletteParticles_StreakHeights[];
extern u16 gPaletteParticles_StreakOffsets[];
extern u8 gPaletteParticles_StreakWidths[];
extern u8 gParticleField_Counts[][3];
extern u16 gParticleField_PuffCells[];
extern u16 gParticleField_PuffSizes[];
#ifndef OWN_ParticleStreams_CellOffsets
extern u16 gParticleStreams_CellOffsets[];
#endif
extern const s16 gParty_PairResolveRules[];
extern u8 gPuffArc_CellBiasY[];
extern u8 gPuffArc_CellHeights[];
extern u16 gPuffArc_CellSourceOffsets[];
extern u8 gPuffArc_CellWidths[];
extern u8 gRenderMode_GlintFlips[];
extern struct Vector3 gRingBolts_Points[];
extern u16 gRisingMotes_ColumnCells[];
extern u8 gRisingMotes_ColumnHeights[];
extern u8 gRisingMotes_ColumnSpots[];
extern u16 gRisingMotes_EmberCells[];
extern u8 gRisingMotes_EmberHeights[];
extern u8 gRisingMotes_EmberWidths[];
extern u16 gRisingMotes_MoteCells[];
extern u8 gRisingMotes_MoteHeights[];
extern u8 gRisingMotes_MoteWidths[];
extern u8 gRisingMotes_Timings[];
extern s8 gRisingWall_Bg2Shifts[][3];
extern u8 gRisingWall_LateRockColumns[];
extern u8 gRisingWall_MoundHeights[];
extern u16 gRisingWall_MoundSheetOffsets[];
extern u8 gRisingWall_MoundWidths[];
extern u16 gRisingWall_RowSheetOffsets[];
extern u8 gRisingWall_RowWidths[];
extern const s32 gBattleFx_RisingPulseScript[];
extern u16 gBattleUnit_WeaponAnimsClass0[];
extern u16 gBattleFormation_LevelMatchedEnemies[];
extern s32 gBattle_BackdropScenes[];
extern const u32 BattleEffect_Composite[];
extern u8 gShatterRocks_DropFrames[];
extern u8 gShatterRocks_RockCounts[];
extern s8 gShatterRocks_RockX[];
extern u16 gShatterRocks_ShardCells[];
extern u8 gShatterRocks_ShardHeights[];
extern u8 gShatterRocks_ShardOffsets[];
extern u8 gShatterRocks_ShardWidths[];
extern u8 gSpinningStars_Radii[];
extern struct Vector3 gSpinningTriangle_Vertex[];
extern u8 gThornVines_ChipHeights[];
extern u16 gThornVines_ChipOffsets[];
extern u8 gThornVines_ChipWidths[];
extern u16 gThornVines_EmberOffsets[];
extern u16 gThornVines_EmberSizes[];
extern u16 gThornVines_LeafOffsets[];
extern u8 gThornVines_LeafSizes[];
extern u8 gThornVines_ObjectPoses[];
extern u16 gThornVines_SparkOffsets[];
extern u8 gThornVines_SparkSizes[];
extern u8 gThornVines_ThornHeights[];
extern u16 gThornVines_ThornOffsets[];
extern u8 gThornVines_ThornWidths[];
extern struct Vector3 gTriangleStrike_Vertex[];
extern u8 gTwelveMode_Records[];
extern s8 gTwoResource_CellBiasY[];
extern u8 gTwoResource_CellHeights[];
extern u16 gTwoResource_CellSourceOffsets[];
extern u8 gTwoResource_CellWidths[];
extern u8 gTwoResource_CellX[];
extern u8 gUi_RenderResultValues[];
extern const u16 gWorldMap_CursorDirectionAngles[];
extern const u8 gWorldMap_MarkerBlendCycle[];
extern const s32 gWorldMap_PlaceMarkers[];
u32 __udivsi3(u32 numerator, u32 denominator);
#ifndef OWN__call_via_r0
void _call_via_r0(u32 routine);
#endif
void Animation_ApplyChildArgumentFar(void *, s32);
#ifndef OWN_Animation_ApplyChildPalette
#endif
#ifndef OWN_Battle_DispatchInputEvent
#endif
#ifndef OWN_Battle_FindTaggedSlotByValue
#endif
#ifndef OWN_Battle_GetTaggedSlotValue
#endif
#ifndef OWN_Battle_ProcessRoundEnd
#endif
#ifndef OWN_Battle_RunEncounter
#endif
#ifndef OWN_BattleAction_RunDescriptor
#endif
#ifndef OWN_BattleCommand_ExecuteSelectedAction
#endif
#ifndef OWN_BattleCommand_ExecuteSelectedItem
#endif
#ifndef OWN_BattleCommand_SelectAutomatic
#endif
#ifndef OWN_BattleEffect_RunCirclingFallingScene
#endif
#ifndef OWN_BattleEffect_RunEmberColumns
#endif
#ifndef OWN_BattleEffect_RunStagedParticles
#endif
#ifndef OWN_BattleEffect_RunTargetedItemBreak
#endif
#ifndef OWN_BattleEffect_SelectNearbyObject
#endif
#ifndef OWN_BattleFx_AdvanceSpinAngle
#endif
#ifndef OWN_BattleFx_ApplyColorToTargetBuffer
#endif
#ifndef OWN_Encounter_RollWorldMap
#endif
#ifndef OWN_BattleFx_CallEffect03AndStop
#endif
#ifndef OWN_BattleFx_CallEffect04
#endif
#ifndef OWN_BattleFx_CallEffect05
#endif
#ifndef OWN_BattleFx_CircleAnchor
#endif
#ifndef OWN_BattleFx_FindDescriptor
#endif
#ifndef OWN_BattleFx_GetCycledTableWord
#endif
#ifndef OWN_BattleFx_GetPhaseResult
#endif
#ifndef OWN_BattleFx_GetResourceId
#endif
#ifndef OWN_BattleFx_HalveDistanceToTarget
#endif
#ifndef OWN_BattleFx_InitializeMode10
#endif
#ifndef OWN_BattleFx_InitializeMode6
#endif
#ifndef OWN_BattleFx_LoadResourceGroup
#endif
#ifndef OWN_BattleFx_MarkChildAndRunFallbackTransition
#endif
#ifndef OWN_BattleFx_RenderMode5
#endif
#ifndef OWN_BattleFx_RunBurstParticleMainObject
#endif
#ifndef OWN_BattleFx_RunCastingImpact
#endif
#ifndef OWN_BattleFx_RunDescriptorAction
#endif
#ifndef OWN_BattleFx_RunEffect13Hook
#endif
#ifndef OWN_BattleFx_RunEventAction
#endif
#ifndef OWN_BattleFx_RunKind6DescriptorAction
#endif
#ifndef OWN_BattleFx_RunObjectRow
#endif
#ifndef OWN_BattleFx_RunSparkEmitter
#endif
#ifndef OWN_BattleFx_SetCallbackWhenTargetUnset
#endif
#ifndef OWN_BattleFx_SetSpecialFromTable
#endif
#ifndef OWN_BattleFx_ShrinkObjectScaleUntilHalf
#endif
#ifndef OWN_BattleFx_SpawnDescendingArcParticles
#endif
#ifndef OWN_BattleFx_SpawnItemBreakMode1
#endif
#ifndef OWN_BattleFx_SpawnRadialParticleRing
#endif
#ifndef OWN_BattleFx_StartOrbitingParticles
#endif
#ifndef OWN_BattleFx_UpdateItemBreakFragment
#endif
#ifndef OWN_BattleFx_UpdatePairedArcSpawner
#endif
#ifndef OWN_BattleFx_UpdateScaledArcObjectA
#endif
#ifndef OWN_BattleFx_UpdateScaledArcObjectB
#endif
#ifndef OWN_BattleFx_UpdateShrinkingOrbitObject
#endif
#ifndef OWN_BattleMotion_ProjectScaledPosition
#endif
#ifndef OWN_BattleParty_ReconcileDjinnRecovery
#endif
#ifndef OWN_BattleParty_ResetActiveRuntimeFields
#endif
#ifndef OWN_BattlePres_RunItem
#endif
#ifndef OWN_BattlePres_SetActorModeAndAction
#endif
#ifndef OWN_BattlePresentation_BuildSortedUnitEntries
#endif
#ifndef OWN_BattlePresentation_BuildTilemap
#endif
#ifndef OWN_BattlePresentation_ConfigurePaletteFade
#endif
#ifndef OWN_BattlePresentation_WaitForPromptAt
#endif
#ifndef OWN_BattleRandomPercent
#endif
#ifndef OWN_BattleSummon_UpdateAvailability
#endif
#ifndef OWN_BattleTarget_SelectForAction
#endif
#ifndef OWN_BattleUnit_ProcessTurnEnd
#endif
void BitDecoder_DecodeImage(void);
#ifndef OWN_Camera_WorldToScreen
#endif
#ifndef OWN_Encounter_RollTerrain
#endif
#ifndef OWN_EffectRuntime_PrepareRisingObject
#endif
#ifndef OWN_EffectSlot_Initialize
#endif
void FarCall_WindowTable(void);
struct AnimationObject *GetBattleEffectObject(s32 kind);
#ifndef OWN_Graphics_ScaleRgb555
#endif
#ifndef OWN_Item_EncodeBankedId
#endif
void ItemIcon_LoadTilesFar(s32 item);
#ifndef OWN_Motion_SetTargetPositionFromMagnitudeAngle
#endif
#ifndef OWN_Object_InitializeMode
s32 Object_InitializeMode(struct AnimationObject *state, s32 flags);
#endif
#ifndef OWN_ObjectGroup_ApplyRandomChildValues
#endif
#ifndef OWN_ObjectGroup_SetActionForOthers
#endif
#ifndef OWN_Party_ReservedNoOp5B14
#endif
#endif

#if GAME_TLA
struct AnimationObject;
struct BattleActionRecord;
struct BattleEffectBuffers;
struct BattleObj;
struct DispatchObject;
struct ParticleChild;
struct PendingEvent;
struct PlacementTable;
struct ScaledSprite;
struct SceneCueRule;
struct WorldMapWork;

struct ParticleEffectObject {
  u8 padding[80];
  struct ParticleChild *child;
};
struct ParticlePosition {
  s32 x;
  s32 y;
  s32 z;
};
extern const u8 gBattleFx_ArcTiles[];
extern u16 gBattleFx_EightWayHeadingWords[];
extern const u8 gBattleFx_ParticleScript[];
extern const u8 gBattleFx_UntargetedObjectScript[];
extern const u8 gColorBuffer_FadeStep1Map[32];
extern const u8 gColorBuffer_FadeStep2Map[32];
extern const u8 gColorBuffer_FadeStep3Map[32];
extern u16 gEncounter_EnemyGroupTable[];
extern struct WorldMapWork gWorldMapWork;
extern const struct SceneCueRule gScene_CueRuleTable[];
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
s16 *BattleAction_FindDescriptor(s16 action);
s32 BattleActor_RemoveFromLists(s32);
void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode);
void BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half);
void BattleFx_CallEffect14(void);
u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1);
s32 BattleFx_HasMatchingEvent5(s32 effectId);
void BattleFx_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void BattleFx_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
s32 BattleFx_LookupResult(void *);
s32 BattleFx_RunProjectileVolleyB(void *effect, s32 mode);
s32 BattleFx_RunSparkGroups(void *, s32);
void BattleFx_SnapScaleToFull(void *object);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 BattleFx_StartRandomParticleEmitter(s32, s32);
void BattleFx_UpdateArcEffect(void);
void BattleLayout_HighlightPartyPanelsFar(u16 *);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void BattleMotion_DestroyAllSlotObjects(void);
void BattleMotion_ReleaseObjectSlotByValue(s32 objectId);
void BattleMotion_SetObjectPosition(struct BattleObj *slot);
void BattleMotion_SetRecordChildValues(void *, s32);
void BattlePalette_UpdateBlend(void);
s32 BattleParty_PrepareReserveOwners(u16 *owners);
void BattlePres_SetActorModes(u16 *, s32);
void BattlePresentation_WaitForAdvance(void);
void BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
void Camera_ConfigureScene(s32 scene);
s32 Curve_LookupScaledValue(s32 selector, s32 mode);
s32 Djinn_ActivateFar(s32 unitId, s32 element, s32 index);
s32 Func_08020190(s32, s32, s32);
s32 Func_080cdac0(s32 effect, s32 from, s32 to);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void Func_080e3074(void);
void Func_0811b75c(s16 *list, s32 refresh, s32 spawn);
void Func_0818f620(void *effect, s32 mode);
void GameFlag_ClearBitFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
s32 Map_RenderAnimatedTileFramesForObjectFar(void *object);
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
void ObjectMotion_SnapToTerrain(void *object);
s32 Owner_GetDefaultElement(void *state);
#ifndef OWN_Owner_RecalculateStatsFar
void UnitCalcFar(s32);
#endif
s32 Party_CountActiveOwnersFar(void);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void SummonFx_RunCanvasEffect(void *action, s32 mode);
struct PlacementTable *Trade_GetOfferStateFar(s32 owner);
s32 Trade_RemoveOfferFar(s32 unitId, s32 element, s32 index);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWaitCoreFar(s32 messageId);
void PartyStatusDrawFar(s32 mode);
#endif

#endif
