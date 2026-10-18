#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_TABLE_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_TABLE_H

#include "types.h"

// The two kinds of target strike (gTargetStrike_Records).
#define STRIKE_KIND_SLASH 0  // sideways slashes
#define STRIKE_KIND_COLUMN 1 // columns of light

// A target strike style's extras.
#define STRIKE_SMOKE 1  // each hit raises smoke puffs from the target's feet
#define STRIKE_ABSORB 2 // motes fly out of the target and back into the user
#define STRIKE_GLINTS 4 // glints sparkle around the target
#define STRIKE_SPARKS 8 // each hit scatters sparks
#define STRIKE_HEAVY 16 // one heavy hit: a big shake and the target recoils

extern u8 gIce_Counts[];
extern s8 gStreakVolley_Tremble[];
extern u8 gTargetStrike_Widths[];
extern u8 gTargetStrike_Heights[];
extern u16 gTargetStrike_Offsets[];
extern u8 gTargetStrike_Reach[];
extern u8 gTargetStrike_ColumnStages[];
extern u8 gTargetStrike_GlintDrawFlags[];
extern u8 gBladeRain_CellWidths[];
extern u8 gBladeRain_CellHeights[];
extern u16 gBladeRain_CellSourceOffsets[];
extern u8 gBattlePieceWidth[];
extern u8 gBattlePieceHeight[];
extern u16 gBattlePieceOffset[];
extern u8 gBattlePieceStartX[];
extern u8 gBattlePieceStartY[];
extern s8 gCornerSparks_X[];
extern s8 gCornerSparks_Y[];
extern u8 gCornerSparks_DrawFlags[];
extern u8 gFroth_Counts[];
extern u8 gWhirlwind_Shapes[];
extern u8 gPrism_ShardStarts[];
extern u8 gPrism_Counts[];
extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[];
extern s32 gPrism_ShardOffsets[];
extern u16 gPlasma_Sparks[];
extern u8 gPlasma_Columns[];
extern u8 gPlasma_Counts[];
extern u16 gSparkGroups_Shapes[];
extern u8 gSparkGroups_FlashCells[];
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
extern u8 gLightningBolts_Sparks[][2];
extern u16 gUnused_SheetCells6[];
extern u16 gUnused_SheetCells[];
extern u8 gUnused_SheetWidths[];
extern u8 gUnused_SheetHeights[];
extern s8 gVine_ColumnOffsets[];
extern u8 gMercury_LaunchFrames[];
extern u8 gHail_CellWidths[];
extern u8 gHail_CellHeights[];
extern u16 gHail_CellSourceOffsets[];
extern u8 gGround_ChipFlips[];
extern u8 gGround_ChipWidths[];
extern u8 gGround_ChipHeights[];
extern u16 gGround_ChipCells[];
extern u8 gGround_ChipX[];
extern u8 gGround_ChipY[];
extern u8 gBeamSequence_CellWidths[];
extern u8 gBeamSequence_CellHeights[];
extern u16 gBeamSequence_CellSheetOffsets[];
extern u8 gBeamSequence_CellReaches[];
extern u8 gFormation_TwelveX[];
extern u8 gFormation_TwelveZ[];
extern u8 gFormation_RightEightX[];
extern u8 gFormation_RightEightZ[];
extern u8 gFormation_SmallEightX[];
extern u8 gFormation_SmallEightZ[];
extern u8 gImpactBurst_CellWidths[];
extern u8 gImpactBurst_CellHeights[];
extern u16 gImpactBurst_CellSourceOffsets[];
extern u8 gRagnarok_StrikeColumns[];
extern u8 gRagnarok_FlashCells[];
extern u8 gTitanBlade_FlashCells[];
extern s32 gTitanBlade_DustGravity[];
extern u8 gShockwaveScene_BurstHeights[];
extern u16 gShockwaveScene_BurstCells[];
extern u8 gKirin_Columns[];
extern u8 gKirin_Rows[];
extern u8 gNeptune_ObjectX[];
extern u8 gNeptune_ObjectY[];
extern s32 gNeptune_Gravity[];
extern u16 gUndeadSword_SparkCells[];
extern u16 gUndeadSword_SparkSizes[];

#endif
