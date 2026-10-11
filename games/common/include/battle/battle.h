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
struct BtlPlan;
struct BtlUnit;
struct DispatchObject;
struct EffectSlot;
struct ObjectRuntime;
struct SceneAreaNameRule;

extern u16		BattleFx_CyclePatternWords[];
extern s8		BattleFx_RandomChildValues[];
extern const s8 BattlePlacement_StepPairs[];
extern u8		Encounter_AreaEntryTable[];
extern u8		gPlayerObjectId[];
extern u16		Resource_SlotAssignments[];
s32				BtlTgtExec(struct BtlPlan *plan, s32 slot);
struct BtlObj;
#ifndef OWN_BattlePresentation_SpawnActorObject
void BtlObjMake(struct BtlObj *actor, s32 unit, s32 x, s32 y);
#endif
#ifndef OWN_BattleActor_SpawnObjectsForList
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh);
#endif
void BattleEffect_InitializeSharedScene(void);
void BattleEvent_Playback(void);
void BattleFx_DrawScaledObject(struct EffectSlot *object);
#if GAME_TLA
#ifndef OWN_BattleFx_GetResourceGroup
s8 BattleFx_GetResourceGroup(s32 effect_index);
#endif
#endif
void  BattleFx_PrepareBufferInterpolation(void);
void  BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode);
void  BattleFx_RunNoEffect(void);
void  BattleFx_RunOrbitingParticles(void);
void  BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect, s32 mode);
void  BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode);
void *BattleFx_StartItemBreak(void *object);
s32	  BattleMotion_GetSlotPalette(s32 id);
void  BattleMotion_ResetObjectAtScaledAnchor(s32 object_id);
void  BattleMotion_SetMode5AndActivateSlot(s32 unit_id);
void  BattleMotion_SetupEscapeObject(s32 object_id);
#ifndef OWN_BattlePres_BuildOpponentEntries
s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries);
#endif
#ifndef OWN_BattlePres_BuildUnitEntries
s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries);
#endif
s32	 BattlePres_WaitSync(void);
s32	 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
void BattleUnit_ResetGuardLevels(void);
void Camera_InitDefaultTransform(void);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
void RunBattleEffect05(void);
s32	 Summon_IsEntryFlagged(s32 index);
#if GAME_TLA
void BattlePres_SetActorModesFar(u16 *, s32);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object, s32 value);
void Owner_UpdateRatioPairFar(struct BtlUnit *, s32);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_FinalizeSharedSlotFar(void);
#endif
void UpdateRisingParticleBurst(void *object);

#if !GAME_TLA
struct AnimationEntry;
struct AnimationObject;
struct ArcEffectObject;
struct ArcObject;
struct ArcParticle;
struct BtlAct;
struct BattleActionRecord;
struct BattleEffectArgument;
struct BattleEffectEntry;
struct BattleEventState;
struct BattleGroupOrder;
struct BattleLocationRule;
struct BtlObj;
struct BtlPlan;
struct BattlePresentationTransition;
struct BattleSummonState;
struct BtlUnit;
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
struct Output_08097f80;
struct RockWallHeights;
struct SceneInteractionEntry;
struct ScriptObjectEntry;
struct Source_0808f28c;
struct SparkObject;
struct TransitionActorList;
struct WaveFxWork;

struct CameraWork
{
	u8	unknown_00[0x36];
	u16 scroll;
};
struct Cells03001ad0
{
	u16 unk00;
	u16 unk02;
	u16 unk04;
	u16 unk06;
};
struct EffectVector
{
	s32 x;
	s32 y;
	s32 z;
};
struct Ring
{
	s32 x;
	s32 y;
	s32 vel_x;
	s32 vel_y;
	s32 age;
};
struct ScatterPosition
{
	s32 x;
	s32 y;
	s32 z;
};
struct Vector3
{
	s32 x;
	s32 y;
	s32 z;
};
extern u8						Animation_ChildPaletteCycle[];
extern u8						BattleCommand_SelectionWeights[3][8];
extern const u8					BattleFx_ArcSparkTiles[];
extern const s32				BattleFx_BurstParticleObjectScript[];
extern struct BattleEffectEntry BattleFx_DefinitionTable[];
extern u8						BattleFx_GlintCellHeights[];
extern u16						BattleFx_GlintCellOffsets[];
extern u8						BattleFx_GlintCellWidths[];
extern const s32				BattleFx_LinkedObjectScript[];
extern const s32				BattleFx_MarkerParticleScript[];
extern const s32				BattleFx_ParticleEmitterScript[];
extern const s32				BattleFx_ParticleScript[];
#ifndef OWN_BattleFx_PuffCells
extern u16 BattleFx_PuffCells[];
#endif
#ifndef OWN_BattleFx_PuffSizes
extern u8 BattleFx_PuffSizes[];
#endif
extern u32							 BattleFx_PulseScales[];
extern const s32					 BattleFx_SourceHoldScript[];
extern s16							 BattleFx_TargetRangeByMode[];
extern const s32					 BattleFx_UntargetedObjectScript[];
extern const u8						 BattleFx10_Animations[];
extern const u16					 BattleFx10_BoulderCells[];
extern const u8						 BattleFx10_BoulderHeights[];
extern const u8						 BattleFx10_BoulderWidths[];
extern const u16					 BattleFx10_DebrisCells[];
extern const u8						 BattleFx10_DebrisHeights[];
extern const u8						 BattleFx10_DebrisWidths[];
extern const u16					 BattleFx10_FallCells[];
extern const u8						 BattleFx10_FallHeights[];
extern const u8						 BattleFx10_FallWidths[];
extern const u8						 BattleFx10_Points[][2];
extern const u16					 BattleFx10_RockCells[];
extern const u8						 BattleFx10_RockHeights[];
extern const u8						 BattleFx10_RockWidths[];
extern const s8						 BattleFx10_ShakeOffsets[];
extern const u16					 BattleFx10_SprayCells[];
extern const u8						 BattleFx10_SprayHeights[];
extern const u8						 BattleFx10_SprayWidths[];
extern u16							 BattleFx6_FlareCells[];
extern const s32					 BattleFx6_Gravity[];
extern const u8						 BattleFx6_ObjectX[];
extern const u8						 BattleFx6_ObjectY[];
extern u8							 BattleFxPillar_Counts[];
extern u8							 BattleFxPillar_Kinds[];
extern u16							 BattleFxPillar_PuffCells[];
extern u8							 BattleFxPillar_PuffHeights[];
extern s8							 BattleFxPillar_PuffWidths[];
extern s8							 BattleFxPillar_X[];
extern s32							 BattleMotion_VariantAcceleration[];
extern s32							 BattleMotion_VariantDistancePercent[];
extern s32							 BattleMotion_VariantSpeedLimit[];
extern s32							 BattleMotion_VariantVelocityY[];
extern s8							 BattleParty_CenterOrderOffsets[];
extern const struct BattleGroupOrder BattleParty_RoundEndGroupOrder;
extern const s32					 BattlePres_ActorObjectScript[];
extern u8							 BattlePres_AdvanceArrowTiles[];
extern const u8						 BattlePres_CurtainTiles[];
extern const u8						 BattlePres_TileVariants[];
extern u16							 BattleUnit_WeaponAnimsClass1[];
extern u16							 BattleUnit_WeaponAnimsClass2[];
extern u16							 BattleUnit_WeaponAnimsClass3[];
extern u16							 BattleUnit_WeaponAnimsClass5[];
extern u8							 BitDecoder_Size[];
extern const u8						 BladeRain_CellHeights[];
extern const u16					 BladeRain_CellSourceOffsets[];
extern const u8						 BladeRain_CellWidths[];
extern u8							 BurstScene_Records[];
extern u8							 Camera_FlagTransformWork[];
extern u8							 CastingImpact_GlintDrawFlags[];
extern u8							 CastingImpact_ImageX[];
extern u8							 CastingImpact_ImageY[];
extern u16							 CastingImpact_OrbitCells[];
extern u8							 CirclingScene_ObjectColumns[];
extern u8							 CirclingScene_ObjectRows[];
extern u16							 CirclingScene_PuffSheetOffsets[];
extern u16							 CirclingScene_PuffSizes[];
extern u8							 CornerSparks_DrawFlags[];
extern s8							 CornerSparks_X[];
extern s8							 CornerSparks_Y[];
extern u8							 CounterReveal_PanelX[];
extern u8							 CounterReveal_PanelY[];
extern u8							 Crystal_Counts[];
extern u8							 Crystal_ShardHeights[];
extern s32							 Crystal_ShardOffsets[];
extern u8							 Crystal_ShardStarts[];
extern u8							 Crystal_ShardWidths[];
extern const u8						 BattleFx_SparkTiles[];
extern const u8						 BattleFx_QuadrantModes[];
extern const s32					 BattleFx_TargetColors[];
extern s32							 BattleFx_StarWorkOffsets[];
extern u8							 BattleFx_StarSizes[];
extern s32							 BattleFx_RingFrameOffsets[];
extern s8							 BattleFx_HighlightBlendRamp[];
extern u8							 DualTable_ColumnCounts[];
extern u8							 DualTable_ColumnX[];
extern u8							 DualTable_HeightLimits[];
extern u16							 DualTable_PillarCells[];
extern s8							 DualTable_PillarHeights[];
extern u8							 DualTable_PillarWidths[];
extern u16							 DualTable_PuffCells[];
extern u8							 DualTable_PuffHeights[];
extern s8							 DualTable_PuffWidths[];
extern u16							 DualTable_VineCells[];
extern u8							 DualTable_VineWidths[];
extern u8							 EarthWall_CellHeights[];
extern u16							 EarthWall_CellSourceOffsets[];
extern u8							 EarthWall_CellWidths[];
extern u8							 EmberColumns_Columns[];
extern s32							 EmberColumns_Gravity[];
extern u8							 FallingBolts_Counts[];
extern u8							 FallingShards_Counts[];
extern const s32					 FallingSword_DustGravity[];
extern const u8						 FallingSword_FlashCells[];
extern s16							 FieldFx_GroundParticleFrames[];
extern const u8						 FieldFx_GroundParticleTiles[];
extern u8							 FirePillars_Counts[];
extern s8							 FirePillars_Depths[][4];
extern u8							 FirePillars_StartFrames[];
extern s8							 FireSwirl_CellBiasX[];
extern s8							 FireSwirl_CellBiasY[];
extern u8							 FireSwirl_CellHeights[];
extern u16							 FireSwirl_CellSourceOffsets[];
extern u8							 FireSwirl_CellWidths[];
extern s8							 FiveMode_Records[];
extern s8							 FiveMode_Tremble[];
extern const u8						 FlameBlade_FlashCells[];
extern const u8						 FlameBlade_StrikeColumns[];
extern const u8						 Formation_RightEightX[];
extern const u8						 Formation_RightEightZ[];
extern const u8						 Formation_SmallEightX[];
extern const u8						 Formation_SmallEightZ[];
extern const u8						 Formation_TwelveX[];
extern const u8						 Formation_TwelveZ[];
#ifndef OWN_gBattleBgFxWork
extern struct WaveFxWork *gBattleBgFxWork;
#endif
extern const u16 gBattleCueTable[];
#ifndef OWN_gBattleFxWork
extern u8 gBattleFxWork[];
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
extern u8				   HomingEmbers_Counts[];
extern u8				   HomingEmbers_FlareBiasY[];
extern u16				   HomingEmbers_FlareCells[];
extern u8				   HomingEmbers_FlareHeights[];
extern u8				   HomingEmbers_FlareWidths[];
extern s32				   IceShardBursts_Gravities[];
extern u8				   ImpactBurst_CellHeights[];
extern u16				   ImpactBurst_CellSourceOffsets[];
extern u8				   ImpactBurst_CellWidths[];
extern u8				   LightningBolts_Counts[];
extern u8				   LightningBolts_GlintModes[];
extern u8				   LightningBolts_GlintPalettes[];
extern const u8			   LightningBolts_Sparks[][2];
extern u8				   LightningPillar_Columns[];
extern u8				   LightningPillar_Counts[];
extern u16				   LightningPillar_Sparks[];
extern u8				   MemberBeam_HeadHeights[];
extern u16				   MemberBeam_HeadOffsets[];
extern u8				   MemberBeam_HeadWidths[];
extern u8				   MemberBeam_MotePictures[];
extern u8				   MemberBurst_Counts[];
extern u8				   MercuryDjinnFlames_LaunchFrames[];
extern const u8			   ObjectMotion_LinkedActionScript[];
extern const u8			   ObjectRow_Columns[];
extern const struct FxPair ObjectRow_RisePair;
extern const u8			   ObjectRow_Rows[];
extern const struct FxPair ObjectRow_SweepPair;
extern u8				   PaletteParticles_DropSizes[];
extern u8				   PaletteParticles_DropSpeeds[];
extern u8				   PaletteParticles_SparkPixels[];
extern u8				   PaletteParticles_StreakDrops[];
extern u8				   PaletteParticles_StreakHeights[];
extern u16				   PaletteParticles_StreakOffsets[];
extern u8				   PaletteParticles_StreakWidths[];
extern u16				   PaletteRamp_ShardCells[];
extern u8				   PaletteRamp_ShardHeights[];
extern u8				   PaletteRamp_ShardWidths[];
extern u8				   ParticleField_Counts[][3];
extern u16				   ParticleField_PuffCells[];
extern u16				   ParticleField_PuffSizes[];
extern u8				   ParticleReveal_CellHeights[];
extern u16				   ParticleReveal_CellSourceOffsets[];
extern u8				   ParticleReveal_CellWidths[];
#ifndef OWN_ParticleStreams_CellOffsets
extern u16 ParticleStreams_CellOffsets[];
#endif
extern const s16					Party_PairResolveRules[];
extern u8							PuffArc_CellBiasY[];
extern u8							PuffArc_CellHeights[];
extern u16							PuffArc_CellSourceOffsets[];
extern u8							PuffArc_CellWidths[];
extern u8							RenderMode_GlintFlips[];
extern struct Vector3				RingBolts_Points[];
extern const u16					RisingBurst_SparkCells[];
extern const u16					RisingBurst_SparkSizes[];
extern s8							RisingColumns_ColumnOffsets[];
extern u16							RisingMotes_ColumnCells[];
extern u8							RisingMotes_ColumnHeights[];
extern u8							RisingMotes_ColumnSpots[];
extern u16							RisingMotes_EmberCells[];
extern u8							RisingMotes_EmberHeights[];
extern u8							RisingMotes_EmberWidths[];
extern u16							RisingMotes_MoteCells[];
extern u8							RisingMotes_MoteHeights[];
extern u8							RisingMotes_MoteWidths[];
extern u8							RisingMotes_Timings[];
extern s8							RisingWall_Bg2Shifts[][3];
extern u8							RisingWall_LateRockColumns[];
extern u8							RisingWall_MoundHeights[];
extern u16							RisingWall_MoundSheetOffsets[];
extern u8							RisingWall_MoundWidths[];
extern u16							RisingWall_RowSheetOffsets[];
extern u8							RisingWall_RowWidths[];
extern u16							RockToss_ChipCells[];
extern u8							RockToss_ChipFlips[];
extern u8							RockToss_ChipHeights[];
extern u8							RockToss_ChipWidths[];
extern u8							RockToss_ChipX[];
extern u8							RockToss_ChipY[];
extern const struct RockWallHeights RockWall_Heights;
extern const u8						RockWall_Timings[][3];
extern const s32					BattleFx_RisingPulseScript[];
extern u16							BattleUnit_WeaponAnimsClass0[];
extern u16							BattleFormation_LevelMatchedEnemies[];
extern s32							Battle_BackdropScenes[];
extern const u32					SentouKouka_Gousei[];
extern u8							ShatterRocks_DropFrames[];
extern u8							ShatterRocks_RockCounts[];
extern s8							ShatterRocks_RockX[];
extern u16							ShatterRocks_ShardCells[];
extern u8							ShatterRocks_ShardHeights[];
extern u8							ShatterRocks_ShardOffsets[];
extern u8							ShatterRocks_ShardWidths[];
extern u8							SparkGroups_FlashCells[];
extern u16							SparkGroups_Shapes[];
extern u8							SpinningStars_Radii[];
extern struct Vector3				SpinningTriangle_Vertex[];
extern u8							ThornVines_ChipHeights[];
extern u16							ThornVines_ChipOffsets[];
extern u8							ThornVines_ChipWidths[];
extern u16							ThornVines_EmberOffsets[];
extern u16							ThornVines_EmberSizes[];
extern u16							ThornVines_LeafOffsets[];
extern u8							ThornVines_LeafSizes[];
extern u8							ThornVines_ObjectPoses[];
extern u16							ThornVines_SparkOffsets[];
extern u8							ThornVines_SparkSizes[];
extern u8							ThornVines_ThornHeights[];
extern u16							ThornVines_ThornOffsets[];
extern u8							ThornVines_ThornWidths[];
extern u8							Tornado_Shapes[];
extern struct Vector3				TriangleStrike_Vertex[];
extern const u8						TwelveMode_ColumnStages[];
extern const u8						TwelveMode_GlintDrawFlags[];
extern u8							TwelveMode_Records[];
extern const u8						TwelveMode_StrikeHeights[];
extern const u16					TwelveMode_StrikeOffsets[];
extern const u8						TwelveMode_StrikeReach[];
extern const u8						TwelveMode_StrikeWidths[];
extern s8							TwoResource_CellBiasY[];
extern u8							TwoResource_CellHeights[];
extern u16							TwoResource_CellSourceOffsets[];
extern u8							TwoResource_CellWidths[];
extern u8							TwoResource_CellX[];
extern u8							Ui_RenderResultValues[];
extern u8							VortexMotes_Counts[];
extern const u16					WorldMap_CursorDirectionAngles[];
extern const u8						WorldMap_MarkerBlendCycle[];
extern const s32					WorldMap_PlaceMarkers[];
u32									__udivsi3(u32 numerator, u32 denominator);
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
#ifndef OWN_BattleFx_ApplyLookupResult
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
#ifndef OWN_EffectRuntime_LookupByTableEntry
#endif
#ifndef OWN_EffectRuntime_PrepareRisingObject
#endif
#ifndef OWN_EffectSlot_Initialize
#endif
void					FarCall_WindowTable(void);
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
struct BtlObj;
struct DispatchObject;
struct ParticleChild;
struct PendingEvent;
struct PlacementTable;
struct ScaledSprite;
struct SceneCueRule;
struct WorldMapWork;

struct ParticleEffectObject
{
	u8					  padding[80];
	struct ParticleChild *child;
};
struct ParticlePosition
{
	s32 x;
	s32 y;
	s32 z;
};
extern const u8					 BattleFx_ArcTiles[];
extern u16						 BattleFx_EightWayHeadingWords[];
extern const u8					 BattleFx_ParticleScript[];
extern const u8					 BattleFx_UntargetedObjectScript[];
extern const u8					 ColorBuffer_FadeStep1Map[32];
extern const u8					 ColorBuffer_FadeStep2Map[32];
extern const u8					 ColorBuffer_FadeStep3Map[32];
extern u16						 Encounter_EnemyGroupTable[];
extern volatile u16				 gLinkStatus;
extern struct WorldMapWork		 gWorldMapWork;
extern const struct SceneCueRule Scene_CueRuleTable[];
s32								 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
s16								*BattleAction_FindDescriptor(s16 action);
s32								 BattleActor_RemoveFromLists(s32);
void							 BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode);
void							 BattleFx_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half);
void							 BattleFx_CallEffect14(void);
u16								 BattleFx_GetWeightedResult(s32 arg0, s32 arg1);
s32								 BattleFx_HasMatchingEvent5(s32 effectId);
void							 BattleFx_IntegrateVector2(s32 *vector, s32 damping, s32 gravity);
void							 BattleFx_IntegrateVector3(s32 *vector, s32 damping, s32 gravity);
s32								 BattleFx_LookupResult(void *);
s32								 BattleFx_RunProjectileVolleyB(void *effect, s32 mode);
s32								 BattleFx_RunSparkGroups(void *, s32);
void							 BattleFx_SnapScaleToFull(void *object);
void							 BattleFx_StartBufferInterpolation(s32 frames);
s32								 BattleFx_StartRandomParticleEmitter(s32, s32);
void							 BattleFx_UpdateArcEffect(void);
void							 BattleLayout_HighlightPartyPanelsFar(u16 *);
void							 BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void							 BattleMotion_DestroyAllSlotObjects(void);
void							 BattleMotion_ReleaseObjectSlotByValue(s32 object_id);
void							 BattleMotion_SetObjectPosition(struct BtlObj *slot);
void							 BattleMotion_SetRecordChildValues(void *, s32);
void							 BattlePalette_UpdateBlend(void);
s32								 BattleParty_PrepareReserveOwners(u16 *owners);
void							 BattlePres_SetActorModes(u16 *, s32);
void							 BattlePresentation_WaitForAdvance(void);
void				 BattleQueue_SortByPriority(struct BattleActionRecord *actions, s32 count);
void				 Camera_ConfigureScene(s32 scene);
s32					 Curve_LookupScaledValue(s32 selector, s32 mode);
s32					 Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);
s32					 Func_08020190(s32, s32, s32);
s32					 Func_080cdac0(s32 effect, s32 from, s32 to);
s32					 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void				 Func_080e3074(void);
void				 Func_0811b75c(s16 *list, s32 refresh, s32 spawn);
void				 Func_0818f620(void *effect, s32 mode);
void				 GameFlag_ClearBitFar(s32 flag);
void				 GameFlag_SetBitFar(s32 flag);
s32					 Map_RenderAnimatedTileFramesForObjectFar(void *object);
#ifndef OWN_Object_Destroy
void Object_Destroy(struct DispatchObject *work);
#endif
void ObjectMotion_SnapToTerrain(void *object);
s32	 Owner_GetDefaultElement(void *state);
#ifndef OWN_Owner_RecalculateStatsFar
void UnitCalcFar(s32);
#endif
s32	 Party_CountActiveOwnersFar(void);
void Render_ApplyProjectedPlacementFar(
	struct ScaledSprite *sprite, s32 *position, s32 *scale, s32 mode);
void				   SummonFx_RunCanvasEffect(void *action, s32 mode);
struct PlacementTable *Trade_GetOfferStateFar(s32 owner);
s32					   Trade_RemoveOfferFar(s32 unit_id, s32 element, s32 index);
void				   UiText_DrawQuantity(s32, s32);
void				   UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void				   PtyStDrawFar(s32 mode);
#endif

#endif
