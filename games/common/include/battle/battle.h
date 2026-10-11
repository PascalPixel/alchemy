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
#ifndef OWN_BattleFx_GetResourceGroup
s8 BattleFx_GetResourceGroup(s32 effect_index);
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
void BattlePres_SetActorModesFar(u16 *, s32);
s32	 BattlePres_WaitSync(void);
s32	 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count);
void BattleUnit_ResetGuardLevels(void);
void Camera_InitDefaultTransform(void);
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);
#ifndef OWN_Object_SetPositionAndResetMotionFar
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
#endif
#ifndef OWN_ObjectDispatch_ApplyValueToChildrenFar
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object, s32 value);
#endif
void Owner_UpdateRatioPairFar(struct BtlUnit *, s32);
void RunBattleEffect05(void);
s32	 Summon_IsEntryFlagged(s32 index);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_FinalizeSharedSlotFar(void);
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
void Animation_ApplyChildPalette(struct MotionObject *obj, s32 palette);
#endif
void Animation_SetWorkEntryFar(struct AnimationEntry *effect, s32 entry_index);
void Battle_AwardSpoils(void);
s32	 Battle_ClearSerialTransferPayload(void);
s32	 Battle_CollectPartyCommandsFar(void *entries, u16 *excluded_units, s32 excluded_count);
#ifndef OWN_Battle_DispatchInputEvent
s32 Battle_DispatchInputEvent(s32 event);
#endif
#ifndef OWN_Battle_FindTaggedSlotByValue
s32 Battle_FindTaggedSlotByValue(u32 value);
#endif
u32 Battle_GetObjectTableValue(s32 id);
#ifndef OWN_Battle_GetObjectTableValueFar
s32 Battle_GetObjectTableValueFar(s32 actor_id);
#endif
u32 Battle_GetSpriteFlagHighBits(u32 index);
u32 Battle_GetSpriteFlagNibble(u32 index);
#ifndef OWN_Battle_GetTaggedSlotValue
s16 Battle_GetTaggedSlotValue(s32 slot);
#endif
void Battle_InitializeRenderObject(void);
void Battle_PlaceMapMarkers(void);
#ifndef OWN_Battle_ProcessRoundEnd
s32 Battle_ProcessRoundEnd(void);
#endif
void Battle_ReservedNoOp2A08(void);
void Battle_ReservedNoOp9B2C(void);
void Battle_ReservedNoOpF674(void);
#ifndef OWN_Battle_RunEncounter
s32 Battle_RunEncounter(s32 enemy_group);
#endif
void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand);
#ifndef OWN_BattleAction_RunDescriptor
s32 BattleAction_RunDescriptor(s32 arg0);
#endif
void BattleActor_RemoveFromLists(s32 unit_id);
s32	 BattleActor_ResetRuntimeFields(s32 actor);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleActor_UpdateSpritePriorities(void);
void BattleBackground_Load(s32 mode, s32 resource, s32 level);
void BattleBackground_LoadFar(s32, s32, s32);
#ifndef OWN_BattleCommand_ExecuteSelectedAction
s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction);
#endif
#ifndef OWN_BattleCommand_ExecuteSelectedItem
s32 BattleCommand_ExecuteSelectedItem(s32 arg, s32 slot);
#endif
#ifndef OWN_BattleCommand_SelectAutomatic
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *request, s32 retry);
#endif
void BattleEffect_ClearAllObjects(void);
void BattleEffect_ClearOutOfBoundsObjects(void);
void BattleEffect_InitializeBuffers(void);
#ifndef OWN_BattleEffect_RunCirclingFallingScene
void BattleEffect_RunCirclingFallingScene(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleEffect_RunEmberColumns
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect);
#endif
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);
void BattleEffect_RunPhasedRadialParticleSequence(s32 arg);
void BattleEffect_RunShockwaveScene(struct BattleEffectArgument *effect);
#ifndef OWN_BattleEffect_RunStagedParticles
void BattleEffect_RunStagedParticles(struct BattleEffectArgument *effect);
#endif
#ifndef OWN_BattleEffect_RunTargetedItemBreak
void BattleEffect_RunTargetedItemBreak(void);
#endif
#ifndef OWN_BattleEffect_SelectNearbyObject
s32 BattleEffect_SelectNearbyObject(s32 sourceId);
#endif
s32	 BattleEffect_SelectNearbyTargetObject(s32 sourceId, s32 battleMode);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_SpawnBurstParticleField(void);
void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect);
void BattleEffect_WipeCanvas(s32 mode, s32 value);
s32	 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned);
void BattleEv_MarkDisplay(void);
void BattleEv_RunWait(s32 action, s32 flag);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
s32	 BattleFormation_BuildEnemyList(s32 record_id);
s32	 BattleFormation_SelectLevelMatchedCandidate(s32 *out_margin);
void BattleFx_AdvanceAngleWithRamp(void *obj);
void BattleFx_AdvanceHueCycle(void);
void BattleFx_AdvanceScrollOnInterval(void);
#ifndef OWN_BattleFx_AdvanceSpinAngle
void BattleFx_AdvanceSpinAngle(struct MotionObject *object);
#endif
#ifndef OWN_BattleFx_ApplyColorToTargetBuffer
void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode);
#endif
#ifndef OWN_BattleFx_ApplyLookupResult
s32 BattleFx_ApplyLookupResult(s32 arg0, s32 arg1);
#endif
void BattleFx_ArmBg0HBlankDma(void);
void BattleFx_BuildWindowEdgeTable(void);
#ifndef OWN_BattleFx_CallEffect03AndStop
void BattleFx_CallEffect03AndStop(void);
#endif
#ifndef OWN_BattleFx_CallEffect04
void BattleFx_CallEffect04(void);
#endif
#ifndef OWN_BattleFx_CallEffect05
void BattleFx_CallEffect05(void);
#endif
u32 BattleFx_CheckDescriptorKind3Result(s32 descriptor, s32 value);
#ifndef OWN_BattleFx_CircleAnchor
void BattleFx_CircleAnchor(struct OrbitEffect *effect);
#endif
void BattleFx_CleanupResourcesAndWindow(void);
void BattleFx_ClearChildValueOnMismatch(void);
void BattleFx_ComputeHueChannels(s32 value, s32 *maximum, s32 *center, s32 *minimum);
s32	 BattleFx_CopyLinkedObjectPosition(void *obj);
void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void BattleFx_EmitRandomParticleFromEmitter(struct ObjectRuntime *emitter);
s32	 BattleFx_ExecutePackedAbilityEffect(s32 packed);
s32	 BattleFx_FilterObjectIdByFlags(s32 arg0);
#ifndef OWN_BattleFx_FindDescriptor
struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value);
#endif
s32	 BattleFx_FindDescriptorWithOverride(s32 arg0);
void BattleFx_FinishHeavyImpact(s32 id);
void BattleFx_FinishSceneAndReleaseHeapBlock(void);
#ifndef OWN_BattleFx_GetCycledTableWord
u16 BattleFx_GetCycledTableWord(void);
#endif
#ifndef OWN_BattleFx_GetPhaseResult
unsigned char BattleFx_GetPhaseResult(s32 phase_index);
#endif
#ifndef OWN_BattleFx_GetResourceId
s32 BattleFx_GetResourceId(u32 id);
#endif
#ifndef OWN_BattleFx_HalveDistanceToTarget
void BattleFx_HalveDistanceToTarget(struct LinkedCaptureObject *object);
#endif
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high);
#ifndef OWN_BattleFx_InitializeMode10
void BattleFx_InitializeMode10(struct BattleEffectArgument *efx);
#endif
void BattleFx_InitializeMode12(struct BattleEffectArgument *efx);
#ifndef OWN_BattleFx_InitializeMode6
void BattleFx_InitializeMode6(struct BattleEffectArgument *efx);
#endif
void BattleFx_InitializeSlots(void);
#ifndef OWN_BattleFx_LoadResourceGroup
void BattleFx_LoadResourceGroup(void);
#endif
s32 BattleFx_LookupResult(void *arg0);
s32 BattleFx_MapKeyThroughTable(s32 key);
#ifndef OWN_BattleFx_MarkChildAndRunFallbackTransition
void BattleFx_MarkChildAndRunFallbackTransition(void);
#endif
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleFx_RenderAnimationMode0(s32 effect);
#ifndef OWN_BattleFx_RenderMode5
void BattleFx_RenderMode5(struct BattleEffectArgument *effect);
#endif
void BattleFx_ResumeObject(s32 id);
void BattleFx_RunAngledApproachPhases(struct EffectSlot *actor);
#ifndef OWN_BattleFx_RunBurstParticleMainObject
void BattleFx_RunBurstParticleMainObject(void);
#endif
void BattleFx_RunBurstParticles(void);
#ifndef OWN_BattleFx_RunCastingImpact
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind);
#endif
#ifndef OWN_BattleFx_RunDescriptorAction
s32 BattleFx_RunDescriptorAction(s32 id);
#endif
void BattleFx_RunDualTable(struct BattleEffectArgument *effect, s32 mode);
#ifndef OWN_BattleFx_RunEffect13Hook
void BattleFx_RunEffect13Hook(void);
#endif
void BattleFx_RunEffect15(void);
#ifndef OWN_BattleFx_RunEventAction
s32 BattleFx_RunEventAction(void *arg0, s32 arg1, s32 arg2);
#endif
void BattleFx_RunFiveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunFlashingCallbackSequence(void);
void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunItemBreakSequence(void);
#ifndef OWN_BattleFx_RunKind6DescriptorAction
s32 BattleFx_RunKind6DescriptorAction(s32 arg0);
#endif
void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunMercuryDjinnCapture(s32 arg);
void BattleFx_RunNoEffectFrames(s32 frame_count);
#ifndef OWN_BattleFx_RunObjectRow
void BattleFx_RunObjectRow(struct BattleEffectArgument *effect);
#endif
void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunParticleFieldVariant(struct BattleEffectArgument *object, s32 variant);
void BattleFx_RunParticlePool(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant);
void BattleFx_RunSeriesB(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunSevenMode(struct BattleEffectArgument *effect, s32 mode);
#ifndef OWN_BattleFx_RunSparkEmitter
void BattleFx_RunSparkEmitter(struct SparkObject *object);
#endif
void BattleFx_RunTwelveMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode);
void BattleFx_RunVenusDjinnCapture(s32 arg);
void BattleFx_RunWhirlwindComets(struct BattleEffectArgument *effect);
void BattleFx_ScheduleCloakedMapCheck(void);
void BattleFx_SelectLocationRule(s32 check);
void BattleFx_SelectResultPointer(s32 arg0);
void BattleFx_SetApproachMotion(s32 first, s32 second, s32 divisor);
#ifndef OWN_BattleFx_SetCallbackWhenTargetUnset
void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *target);
#endif
void BattleFx_SetObjectAlternatingWords(u8 *object);
void BattleFx_SetPrimaryBufferValue(u32 value);
#ifndef OWN_BattleFx_SetSpecialFromTable
void BattleFx_SetSpecialFromTable(s32 arg0, s32 arg1);
#endif
void BattleFx_SetTransitionFlagAndDisplay(void);
void BattleFx_SetupCanvasTileMap(void);
void BattleFx_SetupResourcesAndWindow(void);
void BattleFx_ShrinkObjectAndDestroyFast(void *obj);
void BattleFx_ShrinkObjectAndDestroySlow(void *obj);
#ifndef OWN_BattleFx_ShrinkObjectScaleUntilHalf
void BattleFx_ShrinkObjectScaleUntilHalf(struct MotionObject *obj);
#endif
void BattleFx_SnapScaleToFull(struct MotionObject *obj);
#ifndef OWN_BattleFx_SpawnDescendingArcParticles
void BattleFx_SpawnDescendingArcParticles(struct ArcObject *source);
#endif
void BattleFx_SpawnFallingParticles(void);
#ifndef OWN_BattleFx_SpawnItemBreakMode1
struct MotionObject *BattleFx_SpawnItemBreakMode1(s32 x, s32 y, s32 z, s32 angle);
#endif
struct MotionObject *BattleFx_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
#ifndef OWN_BattleFx_SpawnRadialParticleRing
void BattleFx_SpawnRadialParticleRing(struct MotionObject *origin);
#endif
void BattleFx_SpawnRandomParticleAtPosition(const struct Source_0808f28c *source);
void BattleFx_StartBufferInterpolation(s32 mode);
#ifndef OWN_BattleFx_StartOrbitingParticles
void BattleFx_StartOrbitingParticles(void);
#endif
void BattleFx_SteerLiftedTarget(s32 target_id);
void BattleFx_SteerPushedTarget(s32 target_id);
void BattleFx_StepPaletteToResource(s32 resource_id);
s32	 BattleFx_StepRatioTransition(void);
void BattleFx_SwayParticle(struct SparkObject *object);
void BattleFx_UpdateAllEffectSlots(void);
void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);
void BattleFx_UpdateDescendingParticleNegativeArc(struct ArcParticle *particle);
void BattleFx_UpdateDescendingParticlePositiveArc(struct ArcParticle *particle);
void BattleFx_UpdateEffect16State(void);
#ifndef OWN_BattleFx_UpdateItemBreakFragment
void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *source);
#endif
void BattleFx_UpdateOrbitingParticleFade(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleLeft(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleMain(struct FieldActor *particle);
void BattleFx_UpdateOrbitingParticleRight(struct FieldActor *particle);
#ifndef OWN_BattleFx_UpdatePairedArcSpawner
void BattleFx_UpdatePairedArcSpawner(struct ArcEffectObject *object);
#endif
void BattleFx_UpdateRadialBurst(struct EffectSlot *effect);
void BattleFx_UpdateRadialCamera(struct EffectSlot *effect);
void BattleFx_UpdateRadialSpread(struct EffectSlot *effect);
void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect);
#ifndef OWN_BattleFx_UpdateScaledArcObjectA
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj);
#endif
#ifndef OWN_BattleFx_UpdateScaledArcObjectB
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj);
#endif
#ifndef OWN_BattleFx_UpdateShrinkingOrbitObject
void BattleFx_UpdateShrinkingOrbitObject(struct FieldActor *object);
#endif
s32	 BattleFx_UpdateStarField(void);
void BattleFx_UpdateStormFlash(void);
void BattleFx_UpdateWaveScene(void);
void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 mode);
void BattleMotion_InitializeActorRecords(s32 id);
#ifndef OWN_BattleMotion_ProjectScaledPosition
s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected);
#endif
s32	 BattleMotion_ReleaseObjectSlotByValue(s32 value);
void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled);
s32	 BattleParty_ApplyStatusDamage(void);
void BattleParty_CollectUnitList(void);
u32	 BattleParty_IsUnitListed(u32 arg0);
void BattleParty_ListAllUnitsAndSubmit(void);
#ifndef OWN_BattleParty_ReconcileDjinnRecovery
s32 BattleParty_ReconcileDjinnRecovery(void);
#endif
#ifndef OWN_BattleParty_ResetActiveRuntimeFields
s32 BattleParty_ResetActiveRuntimeFields(void);
#endif
s32	 BattlePlacement_ContainsId(s16 *entries, s32 id);
s32	 BattlePlacement_CountValidEntries(u32 unit_id, u8 *counts);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_ClearAllActorRecordModes(void);
void BattlePres_ConfigureEffectDisplay(void);
void BattlePres_DrawTransitionRows(void);
void BattlePres_ProcessPendingTileTransfer(void);
s32	 BattlePres_RunAction(struct BattleActionRecord *action);
void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind);
#ifndef OWN_BattlePres_RunItem
s32 BattlePres_RunItem(struct BtlPlan *plan, s32 unused, struct BattleActionRecord *selection);
#endif
s32	 BattlePres_RunPsynergyCast(struct BtlPlan *plan, s32 mode);
s32	 BattlePres_RunUnitAction(s16 *action);
void BattlePres_RunWithZeroArguments(void);
#ifndef OWN_BattlePres_SetActorModeAndAction
s32 BattlePres_SetActorModeAndAction(s32 id);
#endif
void BattlePres_SetActorModes(u16 *actors, s32 mode);
void BattlePres_SetupTransitionAtPairMidpoint(s32 first, s32 second, s32 mode);
void BattlePres_SetupTransitionSceneFar(s32 x, s32 depth, s32 y, s32 mode);
s32	 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action);
s32	 BattlePres_SyncTurn(void);
void BattlePres_UpdateHBlankScroll(void);
s32	 BattlePresentation_BuildActions(struct BattleActionRecord *actions);
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
s32	 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay);
void BattlePresentation_DrawStreaks(void);
void BattlePresentation_InitializeWorkAndResetState(void);
s32	 BattlePresentation_RunEncounterOrUnitTrigger(struct BtlPlan *plan);
s32	 BattlePresentation_RunUnitTransition(struct BtlPlan *plan, s32 mode);
#ifndef OWN_BattlePresentation_SetPaletteLevelFar
void BattlePresentation_SetPaletteLevelFar(s32 unused, s32 level);
#endif
void BattlePresentation_UpdateCamera(void);
#ifndef OWN_BattlePresentation_WaitForPromptAt
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y);
#endif
void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count);
#ifndef OWN_BattleRandomPercent
u32 BattleRandomPercent(void);
#endif
#ifndef OWN_BattleSummon_UpdateAvailability
struct BattleSummonState *BattleSummon_UpdateAvailability(void);
#endif
#ifndef OWN_BattleTarget_SelectForAction
s32 BattleTarget_SelectForAction(s32 actor_id, struct BtlAct *action);
#endif
s32 BattleTarget_SelectRandomPosition(s32 require_living_unit);
s32 BattleUnit_LookupWeaponValueByClass(s32 id);
#ifndef OWN_BattleUnit_ProcessTurnEnd
s32 BattleUnit_ProcessTurnEnd(struct BtlPlan *plan);
#endif
void BattleUnit_RefreshPlacement(void);
void BitDecoder_DecodeImage(void);
s32	 Camera_ApplyTransformByFlag(void);
void Camera_ConfigureScene(s32 pos);
#ifndef OWN_Camera_WorldToScreen
void Camera_WorldToScreen(struct EffectVector *position);
#endif
void CheckObjectMapTile(void);
s32	 Djinn_AddToLeastLoadedOwnerFar(s32, void *);
s32	 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
void Djinn_ShowJoinedMessageFar(s32, s32, void *);
#ifndef OWN_EffectRuntime_LookupByTableEntry
s32 EffectRuntime_LookupByTableEntry(u32 index);
#endif
#ifndef OWN_EffectRuntime_PrepareRisingObject
void EffectRuntime_PrepareRisingObject(struct ObjectRuntime *object);
#endif
#ifndef OWN_EffectSlot_Initialize
void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z);
#endif
s32						Equipment_GetUnleashRateBonusFar(struct BtlUnit *unit);
s32						Event_FindFacingTrigger(s32 source);
s32						Event_GetSpecialValueFar(void);
void					EventActor_RunHopAndLeapSequence(s32 arg);
void					FarCall_WindowTable(void);
s32						Field_RunScene(s32 entrance);
void					FieldEffect_SpawnNearbyMarkers(void);
void					FieldEffect_WatchLeaderDistance(void);
void					FieldEvent_RunTypeHandler(void);
void					FieldEvent_ShowStatusMessage(void);
void					FieldObject_PlaceSceneActors(void);
s32						Fixed_Remainder(s32 value, s32 divisor);
s32						GameFlag_GetByteFar(s32 id);
s32						GameFlag_IsConditionActive(s32 condition);
void					GameFlag_RefreshLureCapFar(void);
struct AnimationObject *GetBattleEffectObject(s32 kind);
s32						GetWorldMapTerrainBehaviorFar(void *, s32 *);
void					Graphics_AdvancePaletteCycle(void);
void					Graphics_ClearBg0Vofs(void);
void					Graphics_ResetBg2Pa(void);
#ifndef OWN_Graphics_ScaleRgb555
s32 Graphics_ScaleRgb555(u16 *source, u16 *destination, s32 scale, s32 count);
#endif
void Graphics_SetBg2AffineScaleHalf(void);
u32	 Graphics_UploadVramBlock(void);
s32	 Inventory_GetEquippedItemFar(struct BtlUnit *unit, s32 slot);
#ifndef OWN_Item_EncodeBankedId
u32 Item_EncodeBankedId(u32 value);
#endif
s32	 Item_GetEquippedElementFar(s32 unit_id);
void ItemIcon_LoadTilesFar(s32 item);
void Map_ApplyWorkOriginAndSpanFar(void);
s32	 Map_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);
void Map_InitializePerspectiveSceneFar(void);
#ifndef OWN_Map_LoadAreaGraphicsFar
void Map_LoadAreaGraphicsFar(void);
#endif
s32	 Map_LoadLayeredSceneFar(s32 index);
s32	 Map_RenderAnimatedTileFramesForObjectFar(void *);
s32	 Map_ResumeAnimationFar(void);
void Map_UpdateCurrentTileBlockFar(void);
void Map_UpdateWorldMapMarkers(void);
void MapEvent_RunTileTriggerSequence(void);
void Menu_RunSelectionFar(void);
void Menu_RunSelectionWithCursorObjectFar(void);
void Menu_RunTopSelectionFar(void);
void Menu_RunWorkspaceResultLoopFar(s32 mode);
#ifndef OWN_Motion_SetTargetPositionFromMagnitudeAngle
void Motion_SetTargetPositionFromMagnitudeAngle(
	struct ObjectRuntime *object, s32 magnitude, s32 angle);
#endif
void Object_DestroyIfPresent(void *object);
void Object_EffectSpawnCallback(void);
#ifndef OWN_Object_InitializeMode
s32 Object_InitializeMode(struct AnimationObject *state, s32 flags);
#endif
#ifndef OWN_Object_SetMoveTargetFar
void Object_SetMoveTargetFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
#endif
void ObjectDispatch_ApplyValueToKind200Children(s32 value);
void ObjectDispatch_InitFromTable0Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable1Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable2Far(struct FieldActor *actor);
void ObjectDispatch_InitFromTable3Far(struct FieldActor *actor);
#ifndef OWN_ObjectGroup_ApplyRandomChildValues
void ObjectGroup_ApplyRandomChildValues(void *owner);
#endif
#ifndef OWN_ObjectGroup_SetActionForOthers
void ObjectGroup_SetActionForOthers(
	struct ObjectRuntime *excluded_object, s32 group_mode, s32 action);
#endif
#ifndef OWN_ObjectGroup_SetChildValueUnlessFifteenFar
void ObjectGroup_SetChildValueUnlessFifteenFar(void *raw_parent, u32 value);
#endif
void					ObjectSystem_InitializeFar(s32 mode);
s32						Owner_ApplyLevelGains(s32 owner, s32 levels);
struct EnemyDefinition *Owner_GetRecordFar(s32 class_id);
s32						Owner_GetValueIfLevelThresholdReachedFar(s32 unit_id, s16 *gains);
void					Palette_DarkenSceneStep(void);
void					Palette_StepTowardResource(s32 resource_id);
#ifndef OWN_PaletteGlow_UpdateFar
void PaletteGlow_UpdateFar(s32 phase, s32 brightness);
#endif
s32 Party_ComputeEligibleMemberAverage(s32 record_id);
s32 Party_GetAverageLevelFar(void);
s32 Party_RemapCharacterIdByFlags(s32 arg0);
#ifndef OWN_Party_ReservedNoOp5B14
void Party_ReservedNoOp5B14(void);
#endif
void Party_ResolveTablePair(void);
s32	 PartyInventory_AddFar(s32 item);
void render_animated_tile_frameFar(u8 *record, u32 frame);
void ResetSceneTransitionEffect(void);
s32	 Resource_FindFreeSlot(s32 key);
u16	 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
s32	 ResourceMetadata_RegisterFar(struct AnimationObject *context, s32 effect_id);
s32	 ResourceMetadata_SumCommandLengthsFar(s32 battle_value, s32 second, s32 third);
void ResourceMetadata_UnregisterFar(struct AnimationObject *context, s32 entry_handle);
s32	 ResourceSlot_LoadFar(u32 slot, u32 *buffer, s32 value, u32 variant);
void RunBattleEffect03(void);
void RunBattleEffect07(void);
void RunBattleEffect08(void);
void RunBattleEffect11(void);
void RunBattleEffect13(void);
void RunBattleEffect16(void);
s32	 RunBattlePresentation(struct BtlPlan *plan, s32 mode);
void Runtime_ReleaseHeapBlock10(void);
void Runtime_SetWorkTripleIfNonNegativeFar(s32 x, s32 y, s32 z);
void Scene_ResolveInteractionResult(void);
#ifndef OWN_ScriptObject_CheckOverlapFar
s32 ScriptObject_CheckOverlapFar(struct ScriptObjectEntry *object, s32 *values);
#endif
struct FxObject *ScriptObject_FindOverlappingEntryFar(
	struct FxObject *object, struct Output_08097f80 *position);
void Summon_ClearWorkFields(void);
s32	 Summon_GetEntryByte4(s32 index);
s32	 Summon_GetEntryFlag1Field(s32 index);
s32	 Summon_GetEntryValue(s32 index);
u32	 Summon_GetLayoutKind(s32 index);
void Summon_LayoutPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions);
s32	 SummonSlot_RegisterActorSprites(s32 unit);
#ifndef OWN_Trade_AddOfferFar
u32 *Trade_AddOfferFar(u32 kind, u32 first, u32 second);
#endif
s32	 Trade_CanOfferDjinnFar(s32 unit_id, s32 element, s32 index);
void Ui_FillBank15PaletteGrey(void);
s32	 Ui_GetTableWordZeroFar(s32 index);
void UiText_MeasureResourceEntriesFar(s32 message, s32 *width, s32 *height);
void UiText_PrepareMessageWorkFar(s32 message);
void UiTimedNotice_CloseIfActiveFar(void);
void UiTimedNotice_CreateFar(s32 scene);
void UiWindow_CreateWithLayoutBoundsFar(s32 value);
void UiWork_InitializeFar(s32 mode);
void UiWork_ResetFreeChannelFar(void);
void UiWork_SetBusyFlagsFar(s32);
void WorldMap_LoadGraphicsFar(s32 x, s32 z);
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
