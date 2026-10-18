#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_TABLE_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_TABLE_H

#include "types.h"
#include "render_placement.h"
#include "vector3.h"

// The two kinds of target strike (gTargetStrike_Records).
#define STRIKE_KIND_SLASH 0  // sideways slashes
#define STRIKE_KIND_COLUMN 1 // columns of light

// A target strike style's extras.
#define STRIKE_SMOKE 1  // each hit raises smoke puffs from the target's feet
#define STRIKE_ABSORB 2 // motes fly out of the target and back into the user
#define STRIKE_GLINTS 4 // glints sparkle around the target
#define STRIKE_SPARKS 8 // each hit scatters sparks
#define STRIKE_HEAVY 16 // one heavy hit: a big shake and the target recoils

// Every table effect_table.c defines, in its order. A table read by columns
// is a two-dimensional array, its columns named by the enum above it.
extern u16 gBattleFx_MoteCells[];
extern u16 gBattleFx_FlareCells[];
extern u16 gBattleFx_PuffCells[];
extern u8 gBattleFx_PuffSizes[];
extern u8 gBattleFx_PuffArcCellWidths[];
extern u8 gBattleFx_PuffArcCellHeights[];
extern u8 gBattleFx_PuffArcCellBiasY[];
extern u16 gBattleFx_PuffArcCells[];
extern u16 gBattleFx_GlintCells[];
extern u8 gBattleFx_GlintCellWidths[];
extern u8 gBattleFx_GlintCellHeights[];

// Each Ice variant: its shards and how many frames it runs.
enum { ICE_SHARD_COUNT, ICE_FRAMES };
extern u8 gIce_Counts[][2];

// Each Douse variant: its bolt waves, sparks per bolt, spacing and frames.
enum {
  FALLING_BOLT_WAVE_COUNT,
  FALLING_BOLT_SPARKS_PER_BOLT,
  FALLING_BOLT_SPACING,
  FALLING_BOLT_DURATION
};
extern u8 gFallingBolts_Counts[][4];

extern s8 gStreakVolley_Records[];
extern s8 gStreakVolley_Tremble[];
extern u8 gTargetStrike_Records[];
extern u8 gTargetStrike_Widths[];
extern u8 gTargetStrike_Heights[];
extern u16 gTargetStrike_Offsets[];
extern u8 gTargetStrike_Reach[];
extern u8 gTargetStrike_ColumnStages[];
extern u8 gTargetStrike_GlintDrawFlags[];
extern u8 gBladeRain_CellWidths[];
extern u8 gBladeRain_CellHeights[];
extern u16 gBladeRain_Cells[];
extern u8 gTileAnimation_PieceWidths[];
extern u8 gTileAnimation_PieceHeights[];
extern u16 gTileAnimation_PieceCells[];
extern u8 gTileAnimation_PieceStartX[];
extern u8 gTileAnimation_PieceStartY[];
extern s8 gCornerSparks_X[];
extern s8 gCornerSparks_Y[];
extern u8 gCornerSparks_DrawFlags[];
extern u8 gSwirlSwipe_CellWidths[];
extern u8 gSwirlSwipe_CellHeights[];
extern u16 gSwirlSwipe_Cells[];
extern u8 gSwirlSwipe_CellX[];
extern s8 gSwirlSwipe_CellBiasY[];

// Each gathering-lights mode: the lights per target and the frames before the
// targets' turns.
enum { GATHERING_LIGHTS_COUNT, GATHERING_LIGHTS_FRAMES };
extern u8 gGatheringLights_Counts[][2];

extern u16 gEarthWall_Cells[];
extern u8 gEarthWall_CellWidths[];
extern u8 gEarthWall_CellHeights[];
extern u8 gDjinnBeams_MotePictures[];
extern u16 gDjinnBeams_HeadOffsets[];
extern u8 gDjinnBeams_HeadWidths[];
extern u8 gDjinnBeams_HeadHeights[];
extern s8 gLobbedShells_Records[];
extern u8 gLobbedShells_ShardWidths[];
extern u8 gLobbedShells_ShardHeights[];
extern u16 gLobbedShells_ShardOffsets[];
extern u8 gCondemn_PanelX[];
extern u8 gCondemn_PanelY[];
extern struct Vector3 gBind_Vertex[];
extern struct Vector3 gTriangleStrike_Vertex[];
extern struct Vector3 gAstralBlast_Points[];
extern u8 gShiningStar_Radii[];
extern u8 gWhirlwindComets_GridX[];
extern u8 gWhirlwindComets_GridY[];
extern u16 gWhirlwindComets_Frames[];
extern u8 gWhirlwindComets_CellWidths[];
extern u8 gWhirlwindComets_CellHeights[];
extern u8 gWhirlwindComets_CellBiasY[];
extern u16 gWhirlwindComets_Cells[];
extern u8 gPaletteParticles_DropSpeeds[];
extern u8 gPaletteParticles_SparkPixels[];
extern u8 gPaletteParticles_DropSizes[];
extern u8 gPaletteParticles_StreakWidths[];
extern u8 gPaletteParticles_StreakHeights[];
extern u8 gPaletteParticles_StreakDrops[];
extern u16 gPaletteParticles_StreakOffsets[];

// The foot of each of Nereid's columns of fire.
enum { NEREID_COLUMN_X, NEREID_COLUMN_Y };
extern u8 gNereid_Columns[][2];

extern s32 gNereid_Gravity[];

// Each Froth variant: its motes and frames.
enum { FROTH_MOTE_COUNT, FROTH_FRAMES };
extern u8 gFroth_Counts[][2];

// Each Whirlwind variant: its tornadoes, their sway and the screen shake.
enum { WHIRLWIND_TORNADOES, WHIRLWIND_SWAY, WHIRLWIND_SHAKE };
extern u8 gWhirlwind_Shapes[][3];

// Where each Prism shard starts in the crystal.
enum { PRISM_SHARD_START_X, PRISM_SHARD_START_Y };
extern u8 gPrism_ShardStarts[][2];

// Each Prism variant: its crystals and frames.
enum { PRISM_CRYSTAL_COUNT, PRISM_FRAMES };
extern u8 gPrism_Counts[][2];

extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[];
extern s32 gPrism_ShardOffsets[];

// Each Plasma variant: the sparks per pillar and the frames of shake.
enum { PLASMA_SPARKS, PLASMA_SHAKE_FRAMES };
extern u16 gPlasma_Sparks[][2];

extern u8 gPlasma_Columns[];
extern u8 gPlasma_Counts[];

// Each spark group variant: sparks per burst, bursts, and from
// SPARK_GROUPS_OFFSET each burst's offset across.
enum { SPARK_GROUPS_SPARKS, SPARK_GROUPS_BURSTS, SPARK_GROUPS_OFFSET };
extern u16 gSparkGroups_Shapes[][5];

extern u8 gSparkGroups_FlashCells[];
extern u8 gVolcano_Counts[];
extern s8 gVolcano_Depths[][4];
extern u8 gVolcano_StartFrames[];
extern u8 gTargetMotes_GlintFlips[];

// Cybele's waving poses: the animations of objects 3 and 4.
enum { CYBELE_POSE_OBJECT3, CYBELE_POSE_OBJECT4 };
extern u8 gCybele_ObjectPoses[][2];

extern u16 gCybele_ThornOffsets[];
extern u8 gCybele_ThornWidths[];
extern u8 gCybele_ThornHeights[];
extern u16 gCybele_LeafOffsets[];
extern u8 gCybele_LeafSizes[];
extern u16 gCybele_ChipOffsets[];
extern u8 gCybele_ChipWidths[];
extern u8 gCybele_ChipHeights[];
extern u16 gCybele_SparkOffsets[];
extern u8 gCybele_SparkSizes[];
extern u16 gCybele_EmberOffsets[];
extern u16 gCybele_EmberSizes[];
extern u8 gBoreas_Points[][2];
extern s8 gBoreas_ShakeOffsets[];
extern u16 gBoreas_RockCells[];
extern u8 gBoreas_RockWidths[];
extern u8 gBoreas_RockHeights[];
extern u8 gBoreas_Animations[];
extern u8 gBoreas_DebrisWidths[];
extern u8 gBoreas_DebrisHeights[];
extern u16 gBoreas_DebrisCells[];
extern u8 gBoreas_SprayWidths[];
extern u8 gBoreas_SprayHeights[];
extern u16 gBoreas_SprayCells[];
extern u16 gBoreas_BoulderCells[];
extern u8 gBoreas_BoulderWidths[];
extern u8 gBoreas_BoulderHeights[];
extern u8 gBoreas_FallWidths[];
extern u8 gBoreas_FallHeights[];
extern u16 gBoreas_FallCells[];
extern s32 gBreak_Gravities[];
extern u16 gStatReduction_ShardCells[];
extern u8 gStatReduction_ShardWidths[];
extern u8 gStatReduction_ShardHeights[];
extern u8 gQuake_Timings[][3];
extern u8 gFire_Counts[];
extern u8 gFire_FlareWidths[];
extern u8 gFire_FlareHeights[];
extern u8 gFire_FlareBiasY[];
extern u16 gFire_FlareCells[];

// Where each rising-motes ice column stands: x and the y of its foot.
enum { RISING_MOTES_COLUMN_X, RISING_MOTES_COLUMN_FOOT };
extern u8 gRisingMotes_ColumnSpots[][2];

extern u8 gRisingMotes_Timings[];
extern u8 gRisingMotes_MoteWidths[];
extern u8 gRisingMotes_MoteHeights[];
extern u16 gRisingMotes_MoteCells[];
extern u16 gRisingMotes_ColumnCells[];
extern u8 gRisingMotes_ColumnHeights[];
extern u8 gRisingMotes_ChipWidths[];
extern u8 gRisingMotes_ChipHeights[];
extern u16 gRisingMotes_ChipCells[];
extern u8 gRay_Sparks[][2];
extern u8 gEruptionEffect_Counts[][3];
extern u16 gEruptionEffect_PuffCells[];
extern u16 gEruptionEffect_PuffSizes[];
extern u16 gUnused_SheetCells6[];
extern u16 gUnused_SheetCells[];
extern u8 gUnused_SheetWidths[];
extern u8 gUnused_SheetHeights[];
extern struct SpriteScale gStagedParticles_UnitScale;
extern u8 gGrowth_VineWidths[];
extern u8 gGrowth_HeightLimits[];
extern u16 gGrowth_VineCells[];
extern u8 gGrowth_PillarWidths[];
extern u16 gGrowth_PillarCells[];
extern u8 gGrowth_ColumnCounts[];
extern u8 gGrowth_ColumnX[];
extern s8 gGrowth_PillarHeights[];
extern s8 gGrowth_PuffWidths[];
extern u8 gGrowth_PuffHeights[];
extern u16 gGrowth_PuffCells[];
extern s8 gVine_ColumnOffsets[];
extern u8 gThorn_Kinds[];
extern s8 gThorn_X[];
extern u8 gThorn_Counts[];
extern s8 gThorn_PuffWidths[];
extern u8 gThorn_PuffHeights[];
extern u16 gThorn_PuffCells[];
extern u8 gLightningBolts_Counts[];
extern u8 gLightningBolts_GlintFlips[];
extern u8 gLightningBolts_GlintModes[];
extern u8 gVolley_CometHeights[];
extern u8 gVolley_Modes[][5];
extern u8 gVolley_FlashWidths[];
extern u8 gVolley_FlashHeights[];
extern u8 gVolley_FlashSteps[];
extern u8 gVolley_FlashRises[];
extern u16 gVolley_FlashOffsets[];
extern u16 gVolley_PelletOffsets[];
extern u8 gMercury_LaunchFrames[];
extern u8 gHail_CellWidths[];
extern u8 gHail_CellHeights[];
extern u16 gHail_Cells[];
extern u8 gGround_ChipFlips[];
extern u8 gGround_ChipWidths[];
extern u8 gGround_ChipHeights[];
extern u16 gGround_ChipCells[];
extern u8 gGround_ChipX[];
extern u8 gGround_ChipY[];
extern u8 gThor_TargetPalettes[];
extern u8 gThor_SmokeFlips[];

// Where each Spire shard starts, before its rock's offset across.
enum { SPIRE_SHARD_X, SPIRE_SHARD_Y };
extern u8 gSpire_ShardOffsets[][2];

extern s8 gSpire_RockX[];
extern u8 gSpire_DropFrames[];
extern u8 gSpire_RockCounts[];
extern u8 gSpire_ShardWidths[];
extern u8 gSpire_ShardHeights[];
extern u16 gSpire_ShardCells[];
extern u8 gBurstScene_Records[];
extern u16 gGaia_RowCells[];
extern u8 gGaia_RowWidths[];
extern u16 gGaia_MoundCells[];
extern u8 gGaia_MoundWidths[];
extern u8 gGaia_MoundHeights[];
extern s8 gGaia_Bg2Shifts[][3];
extern u8 gGaia_LateRockColumns[];
extern u8 gBeamSequence_CellWidths[];
extern u8 gBeamSequence_CellHeights[];
extern u16 gBeamSequence_Cells[];
extern u8 gBeamSequence_CellReaches[];
extern u8 gCastingImpact_GlintDrawFlags[];
extern u8 gCastingImpact_ImageX[];
extern u8 gCastingImpact_ImageY[];
extern u16 gCastingImpact_IceChipCells[];
extern u8 gHelmSplitter_CellWidths[];
extern u8 gHelmSplitter_CellHeights[];
extern u16 gHelmSplitter_Cells[];
extern s8 gHelmSplitter_CellBiasX[];
extern s8 gHelmSplitter_CellBiasY[];
extern u8 gFormation_TwelveX[];
extern u8 gFormation_TwelveZ[];
extern u8 gFormation_RightEightX[];
extern u8 gFormation_RightEightZ[];
extern u8 gFormation_SmallEightX[];
extern u8 gFormation_SmallEightZ[];
extern u8 gImpactBurst_CellWidths[];
extern u8 gImpactBurst_CellHeights[];
extern u16 gImpactBurst_Cells[];
extern s16 gParticleStreams_OrbitPoints[][3];
extern s16 gParticleStreams_DropPoints[][2];
extern u16 gParticleStreams_FlashCells[];
extern u16 gParticleStreams_FlashSizes[];
extern u8 gRamses_ObjectColumns[];
extern u8 gRamses_ObjectRows[];
extern u16 gRamses_PuffCells[];
extern u16 gRamses_PuffSizes[];
extern u8 gRagnarok_StrikeColumns[];
extern u8 gRagnarok_FlashCells[];
extern u8 gTitanBlade_FlashCells[];
extern s32 gTitanBlade_DustGravity[];
extern u8 gJudgment_BurstHeights[];
extern u16 gJudgment_BurstCells[];
extern u16 gJudgment_ShardCells[];
extern u8 gJudgment_ShardWidths[];
extern u8 gJudgment_ShardHeights[];
extern u8 gKirin_Columns[];
extern u8 gKirin_Rows[];
extern u8 gNeptune_ObjectX[];
extern u8 gNeptune_ObjectY[];
extern s32 gNeptune_Gravity[];
extern u16 gUndeadSword_SparkCells[];
extern u16 gUndeadSword_SparkSizes[];

#endif
