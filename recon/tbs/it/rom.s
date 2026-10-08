	.section .rom.00007aa8, "ax"
	.incbin "baserom.gba", 0x00007aa8, 0x00000024
	.section .rom.00007b78, "ax"
	.incbin "baserom.gba", 0x00007b78, 0x00000004
	.section .rom.0000caca, "ax"
	.incbin "baserom.gba", 0x0000caca, 0x00000002
	.section .rom.00033f6c, "ax"
	.incbin "baserom.gba", 0x00033f6c, 0x0000003c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x00033fa8, 0x0000003c
	.global UiText_ItemPossessives
UiText_ItemPossessives:
	.incbin "baserom.gba", 0x00033fe4, 0x00000010
	.section .rom.00072c78, "ax"
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "baserom.gba", 0x00072c78, 0x0000010d
	.section .rom.0009d2c8, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x0009d2c8, 0x00000638
	.section .rom.0009db48, "ax"
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x0009db48, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x0009df30, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x0009e330, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x0009e3c8, 0x00000218
	.section .rom.0009e626, "ax"
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x0009e626, 0x000001b2
	.section .rom.000e2972, "ax"
	.incbin "baserom.gba", 0x000e2972, 0x00000002
	.section .rom.000ede9f, "ax"
	.global PuffArc_CellWidths
PuffArc_CellWidths:
	.incbin "baserom.gba", 0x000ede9f, 0x00000006
	.global PuffArc_CellHeights
PuffArc_CellHeights:
	.incbin "baserom.gba", 0x000edea5, 0x00000006
	.global PuffArc_CellBiasY
PuffArc_CellBiasY:
	.incbin "baserom.gba", 0x000edeab, 0x00000007
	.global PuffArc_CellSourceOffsets
PuffArc_CellSourceOffsets:
	.incbin "baserom.gba", 0x000edeb2, 0x0000000c
	.global BattleFx_GlintCellOffsets
BattleFx_GlintCellOffsets:
	.incbin "baserom.gba", 0x000edebe, 0x0000000c
	.global BattleFx_GlintCellWidths
BattleFx_GlintCellWidths:
	.incbin "baserom.gba", 0x000edeca, 0x00000006
	.global BattleFx_GlintCellHeights
BattleFx_GlintCellHeights:
	.incbin "baserom.gba", 0x000eded0, 0x00000006
	.global FallingShards_Counts
FallingShards_Counts:
	.incbin "baserom.gba", 0x000eded6, 0x00000006
	.global FallingBolts_Counts
FallingBolts_Counts:
	.incbin "baserom.gba", 0x000ededc, 0x0000000c
	.global FiveMode_Records
FiveMode_Records:
	.incbin "baserom.gba", 0x000edee8, 0x00000014
	.global FiveMode_Tremble
FiveMode_Tremble:
	.incbin "baserom.gba", 0x000edefc, 0x00000008
	.global TwelveMode_Records
TwelveMode_Records:
	.incbin "baserom.gba", 0x000edf04, 0x00000054
	.global TwelveMode_StrikeWidths
TwelveMode_StrikeWidths:
	.incbin "baserom.gba", 0x000edf58, 0x00000006
	.global TwelveMode_StrikeHeights
TwelveMode_StrikeHeights:
	.incbin "baserom.gba", 0x000edf5e, 0x00000006
	.global TwelveMode_StrikeOffsets
TwelveMode_StrikeOffsets:
	.incbin "baserom.gba", 0x000edf64, 0x0000000c
	.global TwelveMode_StrikeReach
TwelveMode_StrikeReach:
	.incbin "baserom.gba", 0x000edf70, 0x00000006
	.global TwelveMode_ColumnStages
TwelveMode_ColumnStages:
	.incbin "baserom.gba", 0x000edf76, 0x00000005
	.global TwelveMode_GlintDrawFlags
TwelveMode_GlintDrawFlags:
	.incbin "baserom.gba", 0x000edf7b, 0x00000004
	.global BladeRain_CellWidths
BladeRain_CellWidths:
	.incbin "baserom.gba", 0x000edf7f, 0x00000004
	.global BladeRain_CellHeights
BladeRain_CellHeights:
	.incbin "baserom.gba", 0x000edf83, 0x00000005
	.global BladeRain_CellSourceOffsets
BladeRain_CellSourceOffsets:
	.incbin "baserom.gba", 0x000edf88, 0x00000008
	.global BattlePieceWidth
BattlePieceWidth:
	.incbin "baserom.gba", 0x000edf90, 0x00000021
	.global BattlePieceHeight
BattlePieceHeight:
	.incbin "baserom.gba", 0x000edfb1, 0x00000021
	.global BattlePieceOffset
BattlePieceOffset:
	.incbin "baserom.gba", 0x000edfd2, 0x00000044
	.global BattlePieceStartX
BattlePieceStartX:
	.incbin "baserom.gba", 0x000ee016, 0x00000021
	.global BattlePieceStartY
BattlePieceStartY:
	.incbin "baserom.gba", 0x000ee037, 0x00000021
	.global CornerSparks_X
CornerSparks_X:
	.incbin "baserom.gba", 0x000ee058, 0x00000004
	.global CornerSparks_Y
CornerSparks_Y:
	.incbin "baserom.gba", 0x000ee05c, 0x00000004
	.global CornerSparks_DrawFlags
CornerSparks_DrawFlags:
	.incbin "baserom.gba", 0x000ee060, 0x00000004
	.global TwoResource_CellWidths
TwoResource_CellWidths:
	.incbin "baserom.gba", 0x000ee064, 0x00000006
	.global TwoResource_CellHeights
TwoResource_CellHeights:
	.incbin "baserom.gba", 0x000ee06a, 0x00000006
	.global TwoResource_CellSourceOffsets
TwoResource_CellSourceOffsets:
	.incbin "baserom.gba", 0x000ee070, 0x0000000c
	.global TwoResource_CellX
TwoResource_CellX:
	.incbin "baserom.gba", 0x000ee07c, 0x0000000c
	.global TwoResource_CellBiasY
TwoResource_CellBiasY:
	.incbin "baserom.gba", 0x000ee088, 0x00000008
	.global MemberBurst_Counts
MemberBurst_Counts:
	.incbin "baserom.gba", 0x000ee090, 0x00000006
	.global EarthWall_CellSourceOffsets
EarthWall_CellSourceOffsets:
	.incbin "baserom.gba", 0x000ee096, 0x00000006
	.global EarthWall_CellWidths
EarthWall_CellWidths:
	.incbin "baserom.gba", 0x000ee09c, 0x00000003
	.global EarthWall_CellHeights
EarthWall_CellHeights:
	.incbin "baserom.gba", 0x000ee09f, 0x00000003
	.global MemberBeam_MotePictures
MemberBeam_MotePictures:
	.incbin "baserom.gba", 0x000ee0a2, 0x00000008
	.global MemberBeam_HeadOffsets
MemberBeam_HeadOffsets:
	.incbin "baserom.gba", 0x000ee0aa, 0x00000006
	.global MemberBeam_HeadWidths
MemberBeam_HeadWidths:
	.incbin "baserom.gba", 0x000ee0b0, 0x00000003
	.global MemberBeam_HeadHeights
MemberBeam_HeadHeights:
	.incbin "baserom.gba", 0x000ee0b3, 0x00000003
	.global SevenMode_Records
SevenMode_Records:
	.incbin "baserom.gba", 0x000ee0b6, 0x0000000e
	.global SevenMode_ShardWidths
SevenMode_ShardWidths:
	.incbin "baserom.gba", 0x000ee0c4, 0x00000012
	.global SevenMode_ShardHeights
SevenMode_ShardHeights:
	.incbin "baserom.gba", 0x000ee0d6, 0x00000012
	.global SevenMode_ShardOffsets
SevenMode_ShardOffsets:
	.incbin "baserom.gba", 0x000ee0e8, 0x00000024
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000ee10c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000ee11a, 0x0000000e
	.global SpinningTriangle_Vertex
SpinningTriangle_Vertex:
	.incbin "baserom.gba", 0x000ee128, 0x0000000c
	.global TriangleStrike_Vertex
TriangleStrike_Vertex:
	.incbin "baserom.gba", 0x000ee134, 0x0000000c
	.global RingBolts_Points
RingBolts_Points:
	.incbin "baserom.gba", 0x000ee140, 0x00000018
	.global SpinningStars_Radii
SpinningStars_Radii:
	.incbin "baserom.gba", 0x000ee158, 0x00000002
	.global Data_080ee15a
Data_080ee15a:
	.incbin "baserom.gba", 0x000ee15a, 0x00000009
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000ee163, 0x00000009
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000ee16c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000ee174, 0x00000003
	.global Data_080ee177
Data_080ee177:
	.incbin "baserom.gba", 0x000ee177, 0x00000003
	.global Data_080ee17a
Data_080ee17a:
	.incbin "baserom.gba", 0x000ee17a, 0x00000004
	.global Data_080ee17e
Data_080ee17e:
	.incbin "baserom.gba", 0x000ee17e, 0x00000006
	.global PaletteParticles_DropSpeeds
PaletteParticles_DropSpeeds:
	.incbin "baserom.gba", 0x000ee184, 0x00000004
	.global PaletteParticles_SparkPixels
PaletteParticles_SparkPixels:
	.incbin "baserom.gba", 0x000ee188, 0x00000002
	.global PaletteParticles_DropSizes
PaletteParticles_DropSizes:
	.incbin "baserom.gba", 0x000ee18a, 0x00000004
	.global PaletteParticles_StreakWidths
PaletteParticles_StreakWidths:
	.incbin "baserom.gba", 0x000ee18e, 0x00000006
	.global PaletteParticles_StreakHeights
PaletteParticles_StreakHeights:
	.incbin "baserom.gba", 0x000ee194, 0x00000006
	.global PaletteParticles_StreakDrops
PaletteParticles_StreakDrops:
	.incbin "baserom.gba", 0x000ee19a, 0x00000006
	.global PaletteParticles_StreakOffsets
PaletteParticles_StreakOffsets:
	.incbin "baserom.gba", 0x000ee1a0, 0x0000000c
	.global EmberColumns_Columns
EmberColumns_Columns:
	.incbin "baserom.gba", 0x000ee1ac, 0x00000008
	.global EmberColumns_Gravity
EmberColumns_Gravity:
	.incbin "baserom.gba", 0x000ee1b4, 0x00000010
	.global VortexMotes_Counts
VortexMotes_Counts:
	.incbin "baserom.gba", 0x000ee1c4, 0x00000006
	.global Tornado_Shapes
Tornado_Shapes:
	.incbin "baserom.gba", 0x000ee1ca, 0x00000009
	.global Crystal_ShardStarts
Crystal_ShardStarts:
	.incbin "baserom.gba", 0x000ee1d3, 0x00000022
	.global Crystal_Counts
Crystal_Counts:
	.incbin "baserom.gba", 0x000ee1f5, 0x00000006
	.global Crystal_ShardWidths
Crystal_ShardWidths:
	.incbin "baserom.gba", 0x000ee1fb, 0x0000000c
	.global Crystal_ShardHeights
Crystal_ShardHeights:
	.incbin "baserom.gba", 0x000ee207, 0x0000000d
	.global Crystal_ShardOffsets
Crystal_ShardOffsets:
	.incbin "baserom.gba", 0x000ee214, 0x00000030
	.global LightningPillar_Sparks
LightningPillar_Sparks:
	.incbin "baserom.gba", 0x000ee244, 0x0000000c
	.global LightningPillar_Columns
LightningPillar_Columns:
	.incbin "baserom.gba", 0x000ee250, 0x0000000e
	.global LightningPillar_Counts
LightningPillar_Counts:
	.incbin "baserom.gba", 0x000ee25e, 0x00000004
	.global SparkGroups_Shapes
SparkGroups_Shapes:
	.incbin "baserom.gba", 0x000ee262, 0x00000032
	.global SparkGroups_FlashCells
SparkGroups_FlashCells:
	.incbin "baserom.gba", 0x000ee294, 0x00000006
	.global FirePillars_Counts
FirePillars_Counts:
	.incbin "baserom.gba", 0x000ee29a, 0x00000003
	.global FirePillars_Depths
FirePillars_Depths:
	.incbin "baserom.gba", 0x000ee29d, 0x0000000c
	.global FirePillars_StartFrames
FirePillars_StartFrames:
	.incbin "baserom.gba", 0x000ee2a9, 0x00000005
	.global RenderMode_GlintFlips
RenderMode_GlintFlips:
	.incbin "baserom.gba", 0x000ee2ae, 0x00000006
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000ee2b4, 0x0000065c
	.global ThornVines_ObjectPoses
ThornVines_ObjectPoses:
	.incbin "baserom.gba", 0x000ee910, 0x00000006
	.global ThornVines_ThornOffsets
ThornVines_ThornOffsets:
	.incbin "baserom.gba", 0x000ee916, 0x0000000a
	.global ThornVines_ThornWidths
ThornVines_ThornWidths:
	.incbin "baserom.gba", 0x000ee920, 0x00000005
	.global ThornVines_ThornHeights
ThornVines_ThornHeights:
	.incbin "baserom.gba", 0x000ee925, 0x00000005
	.global ThornVines_LeafOffsets
ThornVines_LeafOffsets:
	.incbin "baserom.gba", 0x000ee92a, 0x00000006
	.global ThornVines_LeafSizes
ThornVines_LeafSizes:
	.incbin "baserom.gba", 0x000ee930, 0x00000004
	.global ThornVines_ChipOffsets
ThornVines_ChipOffsets:
	.incbin "baserom.gba", 0x000ee934, 0x0000000a
	.global ThornVines_ChipWidths
ThornVines_ChipWidths:
	.incbin "baserom.gba", 0x000ee93e, 0x00000005
	.global ThornVines_ChipHeights
ThornVines_ChipHeights:
	.incbin "baserom.gba", 0x000ee943, 0x00000005
	.global ThornVines_SparkOffsets
ThornVines_SparkOffsets:
	.incbin "baserom.gba", 0x000ee948, 0x0000000a
	.global ThornVines_SparkSizes
ThornVines_SparkSizes:
	.incbin "baserom.gba", 0x000ee952, 0x00000006
	.global ThornVines_EmberOffsets
ThornVines_EmberOffsets:
	.incbin "baserom.gba", 0x000ee958, 0x0000000e
	.global ThornVines_EmberSizes
ThornVines_EmberSizes:
	.incbin "baserom.gba", 0x000ee966, 0x0000000e
	.global BattleFx10_Points
BattleFx10_Points:
	.incbin "baserom.gba", 0x000ee974, 0x00000020
	.global BattleFx10_ShakeOffsets
BattleFx10_ShakeOffsets:
	.incbin "baserom.gba", 0x000ee994, 0x00000004
	.global BattleFx10_RockCells
BattleFx10_RockCells:
	.incbin "baserom.gba", 0x000ee998, 0x00000006
	.global BattleFx10_RockWidths
BattleFx10_RockWidths:
	.incbin "baserom.gba", 0x000ee99e, 0x00000003
	.global BattleFx10_RockHeights
BattleFx10_RockHeights:
	.incbin "baserom.gba", 0x000ee9a1, 0x00000003
	.global BattleFx10_Animations
BattleFx10_Animations:
	.incbin "baserom.gba", 0x000ee9a4, 0x00000004
	.global BattleFx10_DebrisWidths
BattleFx10_DebrisWidths:
	.incbin "baserom.gba", 0x000ee9a8, 0x0000000b
	.global BattleFx10_DebrisHeights
BattleFx10_DebrisHeights:
	.incbin "baserom.gba", 0x000ee9b3, 0x0000000b
	.global BattleFx10_DebrisCells
BattleFx10_DebrisCells:
	.incbin "baserom.gba", 0x000ee9be, 0x00000016
	.global BattleFx10_SprayWidths
BattleFx10_SprayWidths:
	.incbin "baserom.gba", 0x000ee9d4, 0x00000003
	.global BattleFx10_SprayHeights
BattleFx10_SprayHeights:
	.incbin "baserom.gba", 0x000ee9d7, 0x00000003
	.global BattleFx10_SprayCells
BattleFx10_SprayCells:
	.incbin "baserom.gba", 0x000ee9da, 0x00000006
	.global BattleFx10_BoulderCells
BattleFx10_BoulderCells:
	.incbin "baserom.gba", 0x000ee9e0, 0x00000006
	.global BattleFx10_BoulderWidths
BattleFx10_BoulderWidths:
	.incbin "baserom.gba", 0x000ee9e6, 0x00000003
	.global BattleFx10_BoulderHeights
BattleFx10_BoulderHeights:
	.incbin "baserom.gba", 0x000ee9e9, 0x00000003
	.global BattleFx10_FallWidths
BattleFx10_FallWidths:
	.incbin "baserom.gba", 0x000ee9ec, 0x00000003
	.global BattleFx10_FallHeights
BattleFx10_FallHeights:
	.incbin "baserom.gba", 0x000ee9ef, 0x00000003
	.global BattleFx10_FallCells
BattleFx10_FallCells:
	.incbin "baserom.gba", 0x000ee9f2, 0x00000006
	.global IceShardBursts_Gravities
IceShardBursts_Gravities:
	.incbin "baserom.gba", 0x000ee9f8, 0x00000010
	.global PaletteRamp_ShardCells
PaletteRamp_ShardCells:
	.incbin "baserom.gba", 0x000eea08, 0x00000018
	.global PaletteRamp_ShardWidths
PaletteRamp_ShardWidths:
	.incbin "baserom.gba", 0x000eea20, 0x0000000c
	.global PaletteRamp_ShardHeights
PaletteRamp_ShardHeights:
	.incbin "baserom.gba", 0x000eea2c, 0x0000000c
	.global RockWall_Timings
RockWall_Timings:
	.incbin "baserom.gba", 0x000eea38, 0x00000009
	.global HomingEmbers_Counts
HomingEmbers_Counts:
	.incbin "baserom.gba", 0x000eea41, 0x00000003
	.global HomingEmbers_FlareWidths
HomingEmbers_FlareWidths:
	.incbin "baserom.gba", 0x000eea44, 0x00000006
	.global HomingEmbers_FlareHeights
HomingEmbers_FlareHeights:
	.incbin "baserom.gba", 0x000eea4a, 0x00000006
	.global HomingEmbers_FlareBiasY
HomingEmbers_FlareBiasY:
	.incbin "baserom.gba", 0x000eea50, 0x00000006
	.global HomingEmbers_FlareCells
HomingEmbers_FlareCells:
	.incbin "baserom.gba", 0x000eea56, 0x0000000c
	.global RisingMotes_ColumnSpots
RisingMotes_ColumnSpots:
	.incbin "baserom.gba", 0x000eea62, 0x00000026
	.global RisingMotes_Timings
RisingMotes_Timings:
	.incbin "baserom.gba", 0x000eea88, 0x00000009
	.global RisingMotes_MoteWidths
RisingMotes_MoteWidths:
	.incbin "baserom.gba", 0x000eea91, 0x00000008
	.global RisingMotes_MoteHeights
RisingMotes_MoteHeights:
	.incbin "baserom.gba", 0x000eea99, 0x00000009
	.global RisingMotes_MoteCells
RisingMotes_MoteCells:
	.incbin "baserom.gba", 0x000eeaa2, 0x00000010
	.global RisingMotes_ColumnCells
RisingMotes_ColumnCells:
	.incbin "baserom.gba", 0x000eeab2, 0x00000006
	.global RisingMotes_ColumnHeights
RisingMotes_ColumnHeights:
	.incbin "baserom.gba", 0x000eeab8, 0x00000003
	.global RisingMotes_EmberWidths
RisingMotes_EmberWidths:
	.incbin "baserom.gba", 0x000eeabb, 0x00000008
	.global RisingMotes_EmberHeights
RisingMotes_EmberHeights:
	.incbin "baserom.gba", 0x000eeac3, 0x00000009
	.global RisingMotes_EmberCells
RisingMotes_EmberCells:
	.incbin "baserom.gba", 0x000eeacc, 0x00000010
	.global LightningBolts_Sparks
LightningBolts_Sparks:
	.incbin "baserom.gba", 0x000eeadc, 0x00000006
	.global ParticleField_Counts
ParticleField_Counts:
	.incbin "baserom.gba", 0x000eeae2, 0x0000000a
	.global ParticleField_PuffCells
ParticleField_PuffCells:
	.incbin "baserom.gba", 0x000eeaec, 0x0000000e
	.global ParticleField_PuffSizes
ParticleField_PuffSizes:
	.incbin "baserom.gba", 0x000eeafa, 0x00000046
	.global StagedParticles_UnitScale
StagedParticles_UnitScale:
	.incbin "baserom.gba", 0x000eeb40, 0x00000008
	.global DualTable_VineWidths
DualTable_VineWidths:
	.incbin "baserom.gba", 0x000eeb48, 0x00000003
	.global DualTable_HeightLimits
DualTable_HeightLimits:
	.incbin "baserom.gba", 0x000eeb4b, 0x00000003
	.global DualTable_VineCells
DualTable_VineCells:
	.incbin "baserom.gba", 0x000eeb4e, 0x00000006
	.global DualTable_PillarWidths
DualTable_PillarWidths:
	.incbin "baserom.gba", 0x000eeb54, 0x00000004
	.global DualTable_PillarCells
DualTable_PillarCells:
	.incbin "baserom.gba", 0x000eeb58, 0x00000006
	.global DualTable_ColumnCounts
DualTable_ColumnCounts:
	.incbin "baserom.gba", 0x000eeb5e, 0x00000003
	.global DualTable_ColumnX
DualTable_ColumnX:
	.incbin "baserom.gba", 0x000eeb61, 0x00000010
	.global DualTable_PillarHeights
DualTable_PillarHeights:
	.incbin "baserom.gba", 0x000eeb71, 0x00000008
	.global DualTable_PuffWidths
DualTable_PuffWidths:
	.incbin "baserom.gba", 0x000eeb79, 0x00000007
	.global DualTable_PuffHeights
DualTable_PuffHeights:
	.incbin "baserom.gba", 0x000eeb80, 0x00000008
	.global DualTable_PuffCells
DualTable_PuffCells:
	.incbin "baserom.gba", 0x000eeb88, 0x0000000e
	.global RisingColumns_ColumnOffsets
RisingColumns_ColumnOffsets:
	.incbin "baserom.gba", 0x000eeb96, 0x00000010
	.global BattleFxPillar_Kinds
BattleFxPillar_Kinds:
	.incbin "baserom.gba", 0x000eeba6, 0x00000008
	.global BattleFxPillar_X
BattleFxPillar_X:
	.incbin "baserom.gba", 0x000eebae, 0x00000008
	.global BattleFxPillar_Counts
BattleFxPillar_Counts:
	.incbin "baserom.gba", 0x000eebb6, 0x00000003
	.global BattleFxPillar_PuffWidths
BattleFxPillar_PuffWidths:
	.incbin "baserom.gba", 0x000eebb9, 0x00000007
	.global BattleFxPillar_PuffHeights
BattleFxPillar_PuffHeights:
	.incbin "baserom.gba", 0x000eebc0, 0x00000008
	.global BattleFxPillar_PuffCells
BattleFxPillar_PuffCells:
	.incbin "baserom.gba", 0x000eebc8, 0x0000000e
	.global LightningBolts_Counts
LightningBolts_Counts:
	.incbin "baserom.gba", 0x000eebd6, 0x0000000c
	.global LightningBolts_GlintPalettes
LightningBolts_GlintPalettes:
	.incbin "baserom.gba", 0x000eebe2, 0x00000004
	.global LightningBolts_GlintModes
LightningBolts_GlintModes:
	.incbin "baserom.gba", 0x000eebe6, 0x00000003
	.global Volley_CometHeights
Volley_CometHeights:
	.incbin "baserom.gba", 0x000eebe9, 0x00000003
	.global Volley_Modes
Volley_Modes:
	.incbin "baserom.gba", 0x000eebec, 0x0000003c
	.global Volley_FlashWidths
Volley_FlashWidths:
	.incbin "baserom.gba", 0x000eec28, 0x00000007
	.global Volley_FlashHeights
Volley_FlashHeights:
	.incbin "baserom.gba", 0x000eec2f, 0x00000007
	.global Volley_FlashSteps
Volley_FlashSteps:
	.incbin "baserom.gba", 0x000eec36, 0x00000007
	.global Volley_FlashRises
Volley_FlashRises:
	.incbin "baserom.gba", 0x000eec3d, 0x00000007
	.global Volley_FlashOffsets
Volley_FlashOffsets:
	.incbin "baserom.gba", 0x000eec44, 0x0000000e
	.global Volley_PelletOffsets
Volley_PelletOffsets:
	.incbin "baserom.gba", 0x000eec52, 0x00000008
	.global MercuryDjinnFlames_LaunchFrames
MercuryDjinnFlames_LaunchFrames:
	.incbin "baserom.gba", 0x000eec5a, 0x00000005
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000eec5f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000eec63, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eec68, 0x00000008
	.global RockToss_ChipFlips
RockToss_ChipFlips:
	.incbin "baserom.gba", 0x000eec70, 0x00000004
	.global RockToss_ChipWidths
RockToss_ChipWidths:
	.incbin "baserom.gba", 0x000eec74, 0x00000009
	.global RockToss_ChipHeights
RockToss_ChipHeights:
	.incbin "baserom.gba", 0x000eec7d, 0x00000009
	.global RockToss_ChipCells
RockToss_ChipCells:
	.incbin "baserom.gba", 0x000eec86, 0x00000012
	.global RockToss_ChipX
RockToss_ChipX:
	.incbin "baserom.gba", 0x000eec98, 0x00000009
	.global RockToss_ChipY
RockToss_ChipY:
	.incbin "baserom.gba", 0x000eeca1, 0x00000009
	.global Mode12_Animations
Mode12_Animations:
	.incbin "baserom.gba", 0x000eecaa, 0x00000004
	.global Mode12_SmokeFlips
Mode12_SmokeFlips:
	.incbin "baserom.gba", 0x000eecae, 0x00000004
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000eecb2, 0x00000040
	.global ShatterRocks_RockX
ShatterRocks_RockX:
	.incbin "baserom.gba", 0x000eecf2, 0x00000005
	.global ShatterRocks_DropFrames
ShatterRocks_DropFrames:
	.incbin "baserom.gba", 0x000eecf7, 0x00000005
	.global ShatterRocks_RockCounts
ShatterRocks_RockCounts:
	.incbin "baserom.gba", 0x000eecfc, 0x00000003
	.global ShatterRocks_ShardWidths
ShatterRocks_ShardWidths:
	.incbin "baserom.gba", 0x000eecff, 0x0000000f
	.global ShatterRocks_ShardHeights
ShatterRocks_ShardHeights:
	.incbin "baserom.gba", 0x000eed0e, 0x00000010
	.global ShatterRocks_ShardCells
ShatterRocks_ShardCells:
	.incbin "baserom.gba", 0x000eed1e, 0x00000020
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000eed3e, 0x00000040
	.global RisingWall_RowSheetOffsets
RisingWall_RowSheetOffsets:
	.incbin "baserom.gba", 0x000eed7e, 0x00000012
	.global RisingWall_RowWidths
RisingWall_RowWidths:
	.incbin "baserom.gba", 0x000eed90, 0x0000000a
	.global RisingWall_MoundSheetOffsets
RisingWall_MoundSheetOffsets:
	.incbin "baserom.gba", 0x000eed9a, 0x00000006
	.global RisingWall_MoundWidths
RisingWall_MoundWidths:
	.incbin "baserom.gba", 0x000eeda0, 0x00000003
	.global RisingWall_MoundHeights
RisingWall_MoundHeights:
	.incbin "baserom.gba", 0x000eeda3, 0x00000003
	.global RisingWall_Bg2Shifts
RisingWall_Bg2Shifts:
	.incbin "baserom.gba", 0x000eeda6, 0x00000006
	.global RisingWall_LateRockColumns
RisingWall_LateRockColumns:
	.incbin "baserom.gba", 0x000eedac, 0x00000006
	.global BeamSequence_CellWidths
BeamSequence_CellWidths:
	.incbin "baserom.gba", 0x000eedb2, 0x00000006
	.global BeamSequence_CellHeights
BeamSequence_CellHeights:
	.incbin "baserom.gba", 0x000eedb8, 0x00000006
	.global BeamSequence_CellSheetOffsets
BeamSequence_CellSheetOffsets:
	.incbin "baserom.gba", 0x000eedbe, 0x0000000c
	.global BeamSequence_CellReaches
BeamSequence_CellReaches:
	.incbin "baserom.gba", 0x000eedca, 0x00000006
	.global CastingImpact_GlintDrawFlags
CastingImpact_GlintDrawFlags:
	.incbin "baserom.gba", 0x000eedd0, 0x00000004
	.global CastingImpact_ImageX
CastingImpact_ImageX:
	.incbin "baserom.gba", 0x000eedd4, 0x0000000e
	.global CastingImpact_ImageY
CastingImpact_ImageY:
	.incbin "baserom.gba", 0x000eede2, 0x00000008
	.global CastingImpact_OrbitCells
CastingImpact_OrbitCells:
	.incbin "baserom.gba", 0x000eedea, 0x0000000a
	.global FireSwirl_CellWidths
FireSwirl_CellWidths:
	.incbin "baserom.gba", 0x000eedf4, 0x00000007
	.global FireSwirl_CellHeights
FireSwirl_CellHeights:
	.incbin "baserom.gba", 0x000eedfb, 0x00000007
	.global FireSwirl_CellSourceOffsets
FireSwirl_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eee02, 0x0000000e
	.global FireSwirl_CellBiasX
FireSwirl_CellBiasX:
	.incbin "baserom.gba", 0x000eee10, 0x00000007
	.global FireSwirl_CellBiasY
FireSwirl_CellBiasY:
	.incbin "baserom.gba", 0x000eee17, 0x00000007
	.global Formation_TwelveX
Formation_TwelveX:
	.incbin "baserom.gba", 0x000eee1e, 0x0000000c
	.global Formation_TwelveZ
Formation_TwelveZ:
	.incbin "baserom.gba", 0x000eee2a, 0x0000000c
	.global Formation_RightEightX
Formation_RightEightX:
	.incbin "baserom.gba", 0x000eee36, 0x00000008
	.global Formation_RightEightZ
Formation_RightEightZ:
	.incbin "baserom.gba", 0x000eee3e, 0x00000008
	.global Formation_SmallEightX
Formation_SmallEightX:
	.incbin "baserom.gba", 0x000eee46, 0x00000008
	.global Formation_SmallEightZ
Formation_SmallEightZ:
	.incbin "baserom.gba", 0x000eee4e, 0x00000008
	.global ImpactBurst_CellWidths
ImpactBurst_CellWidths:
	.incbin "baserom.gba", 0x000eee56, 0x00000008
	.global ImpactBurst_CellHeights
ImpactBurst_CellHeights:
	.incbin "baserom.gba", 0x000eee5e, 0x00000008
	.global ImpactBurst_CellSourceOffsets
ImpactBurst_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eee66, 0x00000010
	.global ParticleStreams_OrbitPoints
ParticleStreams_OrbitPoints:
	.incbin "baserom.gba", 0x000eee76, 0x0000002a
	.global ParticleStreams_DropPoints
ParticleStreams_DropPoints:
	.incbin "baserom.gba", 0x000eeea0, 0x0000001c
	.global ParticleStreams_FlashSheetOffsets
ParticleStreams_FlashSheetOffsets:
	.incbin "baserom.gba", 0x000eeebc, 0x0000000e
	.global ParticleStreams_FlashSizes
ParticleStreams_FlashSizes:
	.incbin "baserom.gba", 0x000eeeca, 0x0000000e
	.global CirclingScene_ObjectColumns
CirclingScene_ObjectColumns:
	.incbin "baserom.gba", 0x000eeed8, 0x00000009
	.global CirclingScene_ObjectRows
CirclingScene_ObjectRows:
	.incbin "baserom.gba", 0x000eeee1, 0x00000009
	.global CirclingScene_PuffSheetOffsets
CirclingScene_PuffSheetOffsets:
	.incbin "baserom.gba", 0x000eeeea, 0x0000000e
	.global CirclingScene_PuffSizes
CirclingScene_PuffSizes:
	.incbin "baserom.gba", 0x000eeef8, 0x0000000e
	.global FlameBlade_StrikeColumns
FlameBlade_StrikeColumns:
	.incbin "baserom.gba", 0x000eef06, 0x00000006
	.global FlameBlade_FlashCells
FlameBlade_FlashCells:
	.incbin "baserom.gba", 0x000eef0c, 0x00000006
	.global FallingSword_FlashCells
FallingSword_FlashCells:
	.incbin "baserom.gba", 0x000eef12, 0x00000006
	.global FallingSword_DustGravity
FallingSword_DustGravity:
	.incbin "baserom.gba", 0x000eef18, 0x00000010
	.global ShockwaveScene_BurstHeights
ShockwaveScene_BurstHeights:
	.incbin "baserom.gba", 0x000eef28, 0x00000008
	.global ShockwaveScene_BurstCells
ShockwaveScene_BurstCells:
	.incbin "baserom.gba", 0x000eef30, 0x0000000e
	.global ShockwaveScene_ShardCells
ShockwaveScene_ShardCells:
	.incbin "baserom.gba", 0x000eef3e, 0x0000000c
	.global ShockwaveScene_ShardWidths
ShockwaveScene_ShardWidths:
	.incbin "baserom.gba", 0x000eef4a, 0x00000006
	.global ShockwaveScene_ShardHeights
ShockwaveScene_ShardHeights:
	.incbin "baserom.gba", 0x000eef50, 0x00000006
	.global ObjectRow_Columns
ObjectRow_Columns:
	.incbin "baserom.gba", 0x000eef56, 0x00000009
	.global ObjectRow_Rows
ObjectRow_Rows:
	.incbin "baserom.gba", 0x000eef5f, 0x00000009
	.global BattleFx6_ObjectX
BattleFx6_ObjectX:
	.incbin "baserom.gba", 0x000eef68, 0x00000008
	.global BattleFx6_ObjectY
BattleFx6_ObjectY:
	.incbin "baserom.gba", 0x000eef70, 0x00000008
	.global BattleFx6_Gravity
BattleFx6_Gravity:
	.incbin "baserom.gba", 0x000eef78, 0x00000010
	.global RisingBurst_SparkCells
RisingBurst_SparkCells:
	.incbin "baserom.gba", 0x000eef88, 0x0000000e
	.global RisingBurst_SparkSizes
RisingBurst_SparkSizes:
	.incbin "baserom.gba", 0x000eef96, 0x0000000e
	.section .rom.000ef014, "ax"
	.incbin "baserom.gba", 0x000ef014, 0x00000020
	.global SentouKouka_BitOperands
SentouKouka_BitOperands:
	.incbin "baserom.gba", 0x000ef034, 0x00000020
	.section .rom.000f0a5c, "ax"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f0a5c, 0x000007e9
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f1245, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f12a8, 0x00000544
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000f17ec, 0x000002f3
	.section .rom.000f38bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f38bc, 0x000000c7
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f3983, 0x00000006
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f3989, 0x0000003d
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000f39c6, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000f3a06, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000f3a46, 0x00000040
	.section .rom.000f53dc, "ax"
	.incbin "baserom.gba", 0x000f53dc, 0x00000020
	.global Data_080f53fc
Data_080f53fc:
	.incbin "baserom.gba", 0x000f53fc, 0x00000004
	.global Data_080f5400
Data_080f5400:
	.incbin "baserom.gba", 0x000f5400, 0x00000008
	.global Data_080f5408
Data_080f5408:
	.incbin "baserom.gba", 0x000f5408, 0x00000012
	.global Data_080f541a
Data_080f541a:
	.incbin "baserom.gba", 0x000f541a, 0x0000000c
	.section .rom.000f8708, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000f8708, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000f871c, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000f8722, 0x00000008
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000f872a, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000f8738, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000f8746, 0x00000008
	.section .rom.000fb7a0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000fb7a0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000fb830, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000fb8e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000fb914, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000fb92c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000fb9b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000fb9c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000fba04, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000fba14, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000fba48, 0x00000030
	.section .rom.000fc504, "ax"
	.incbin "baserom.gba", 0x000fc504, 0x00000090
	.section .rom.000fc624, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fc624, 0x00000060
	.section .rom.00320fa0, "ax"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00320fa0, 0x00000010
	.section .rom.003249e7, "ax"
	.incbin "baserom.gba", 0x003249e7, 0x00000001
	.section .rom.0032b09b, "ax"
	.incbin "baserom.gba", 0x0032b09b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b09c, 0x000086f8
	.section .rom.003357e5, "ax"
	.incbin "baserom.gba", 0x003357e5, 0x00000003
	.section .rom.003370f5, "ax"
	.incbin "baserom.gba", 0x003370f5, 0x00000003
	.section .rom.0033abf9, "ax"
	.incbin "baserom.gba", 0x0033abf9, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033abfc, 0x00000688
	.section .rom.0033f6c6, "ax"
	.incbin "baserom.gba", 0x0033f6c6, 0x00000002
	.section .rom.0034fe36, "ax"
	.incbin "baserom.gba", 0x0034fe36, 0x00000002
	.section .rom.00353426, "ax"
	.incbin "baserom.gba", 0x00353426, 0x00000002
	.section .rom.0035eeca, "ax"
	.incbin "baserom.gba", 0x0035eeca, 0x00000002
	.section .rom.0036f57a, "ax"
	.incbin "baserom.gba", 0x0036f57a, 0x00000002
	.section .rom.0037701a, "ax"
	.incbin "baserom.gba", 0x0037701a, 0x00000002
	.section .rom.0037a85a, "ax"
	.incbin "baserom.gba", 0x0037a85a, 0x00000002
	.section .rom.00383c8e, "ax"
	.incbin "baserom.gba", 0x00383c8e, 0x00000002
	.section .rom.0038f0b6, "ax"
	.incbin "baserom.gba", 0x0038f0b6, 0x00000002
	.section .rom.00396f72, "ax"
	.incbin "baserom.gba", 0x00396f72, 0x00000002
	.section .rom.0039ba02, "ax"
	.incbin "baserom.gba", 0x0039ba02, 0x00000002
	.section .rom.0039fcb6, "ax"
	.incbin "baserom.gba", 0x0039fcb6, 0x00000002
	.section .rom.003a3636, "ax"
	.incbin "baserom.gba", 0x003a3636, 0x00000002
	.section .rom.003afe72, "ax"
	.incbin "baserom.gba", 0x003afe72, 0x00000002
	.section .rom.003be7ba, "ax"
	.incbin "baserom.gba", 0x003be7ba, 0x00000002
	.section .rom.003c40bd, "ax"
	.incbin "baserom.gba", 0x003c40bd, 0x00000003
	.section .rom.003c5a3f, "ax"
	.incbin "baserom.gba", 0x003c5a3f, 0x00000001
	.section .rom.003c885d, "ax"
	.incbin "baserom.gba", 0x003c885d, 0x00000003
	.section .rom.003cbfa9, "ax"
	.incbin "baserom.gba", 0x003cbfa9, 0x00000003
	.section .rom.003cd7bf, "ax"
	.incbin "baserom.gba", 0x003cd7bf, 0x00000001
	.section .rom.003cddaa, "ax"
	.incbin "baserom.gba", 0x003cddaa, 0x00000002
	.section .rom.003ce46e, "ax"
	.incbin "baserom.gba", 0x003ce46e, 0x00000002
	.section .rom.003ce755, "ax"
	.incbin "baserom.gba", 0x003ce755, 0x00000003
	.section .rom.003cfabb, "ax"
	.incbin "baserom.gba", 0x003cfabb, 0x00000001
	.section .rom.003cfea5, "ax"
	.incbin "baserom.gba", 0x003cfea5, 0x00000003
	.section .rom.003d0277, "ax"
	.incbin "baserom.gba", 0x003d0277, 0x00000001
	.section .rom.003d0b17, "ax"
	.incbin "baserom.gba", 0x003d0b17, 0x00000001
	.section .rom.003d0fba, "ax"
	.incbin "baserom.gba", 0x003d0fba, 0x00000002
	.section .rom.003d2173, "ax"
	.incbin "baserom.gba", 0x003d2173, 0x00000001
	.section .rom.003d3636, "ax"
	.incbin "baserom.gba", 0x003d3636, 0x00000002
	.section .rom.003d45ff, "ax"
	.incbin "baserom.gba", 0x003d45ff, 0x00000001
	.section .rom.003d485a, "ax"
	.incbin "baserom.gba", 0x003d485a, 0x00000002
	.section .rom.003d62b5, "ax"
	.incbin "baserom.gba", 0x003d62b5, 0x00000003
	.section .rom.003d6801, "ax"
	.incbin "baserom.gba", 0x003d6801, 0x00000003
	.section .rom.003d858a, "ax"
	.incbin "baserom.gba", 0x003d858a, 0x00000002
	.section .rom.003d8803, "ax"
	.incbin "baserom.gba", 0x003d8803, 0x00000001
	.section .rom.003d8cca, "ax"
	.incbin "baserom.gba", 0x003d8cca, 0x00000002
	.section .rom.003da89f, "ax"
	.incbin "baserom.gba", 0x003da89f, 0x00000001
	.section .rom.003dc49d, "ax"
	.incbin "baserom.gba", 0x003dc49d, 0x00000003
	.section .rom.003dc6be, "ax"
	.incbin "baserom.gba", 0x003dc6be, 0x00000002
	.section .rom.003dcafb, "ax"
	.incbin "baserom.gba", 0x003dcafb, 0x00000001
	.section .rom.003dcc0d, "ax"
	.incbin "baserom.gba", 0x003dcc0d, 0x00000003
	.section .rom.003dd38b, "ax"
	.incbin "baserom.gba", 0x003dd38b, 0x00000001
	.section .rom.003dd829, "ax"
	.incbin "baserom.gba", 0x003dd829, 0x00000003
	.section .rom.003de53e, "ax"
	.incbin "baserom.gba", 0x003de53e, 0x00000002
	.section .rom.003df2be, "ax"
	.incbin "baserom.gba", 0x003df2be, 0x00000002
	.section .rom.003df64f, "ax"
	.incbin "baserom.gba", 0x003df64f, 0x00000001
	.section .rom.003e1396, "ax"
	.incbin "baserom.gba", 0x003e1396, 0x00000002
	.section .rom.003e216d, "ax"
	.incbin "baserom.gba", 0x003e216d, 0x00000003
	.section .rom.003e299a, "ax"
	.incbin "baserom.gba", 0x003e299a, 0x00000002
	.section .rom.003e3025, "ax"
	.incbin "baserom.gba", 0x003e3025, 0x00000003
	.section .rom.003e31e3, "ax"
	.incbin "baserom.gba", 0x003e31e3, 0x00000001
	.section .rom.003e3f3b, "ax"
	.incbin "baserom.gba", 0x003e3f3b, 0x00000001
	.section .rom.003e4487, "ax"
	.incbin "baserom.gba", 0x003e4487, 0x00000001
	.section .rom.003e4861, "ax"
	.incbin "baserom.gba", 0x003e4861, 0x00000003
	.section .rom.003e4c0f, "ax"
	.incbin "baserom.gba", 0x003e4c0f, 0x00000001
	.section .rom.003e6bb3, "ax"
	.incbin "baserom.gba", 0x003e6bb3, 0x00000001
	.section .rom.003e794f, "ax"
	.incbin "baserom.gba", 0x003e794f, 0x00000001
	.section .rom.003e7b6b, "ax"
	.incbin "baserom.gba", 0x003e7b6b, 0x00000001
	.section .rom.003e7e67, "ax"
	.incbin "baserom.gba", 0x003e7e67, 0x00000001
	.section .rom.003ea015, "ax"
	.incbin "baserom.gba", 0x003ea015, 0x00000003
	.section .rom.003eade3, "ax"
	.incbin "baserom.gba", 0x003eade3, 0x00000001
	.section .rom.003ebf45, "ax"
	.incbin "baserom.gba", 0x003ebf45, 0x00000003
	.section .rom.003ec49f, "ax"
	.incbin "baserom.gba", 0x003ec49f, 0x00000001
	.section .rom.003edd89, "ax"
	.incbin "baserom.gba", 0x003edd89, 0x00000003
	.section .rom.003ee62a, "ax"
	.incbin "baserom.gba", 0x003ee62a, 0x00000002
	.section .rom.003eea07, "ax"
	.incbin "baserom.gba", 0x003eea07, 0x00000001
	.section .rom.003eec91, "ax"
	.incbin "baserom.gba", 0x003eec91, 0x00000003
	.section .rom.003ef037, "ax"
	.incbin "baserom.gba", 0x003ef037, 0x00000001
	.section .rom.003ef292, "ax"
	.incbin "baserom.gba", 0x003ef292, 0x00000002
	.section .rom.003ef64a, "ax"
	.incbin "baserom.gba", 0x003ef64a, 0x00000002
	.section .rom.003f0b33, "ax"
	.incbin "baserom.gba", 0x003f0b33, 0x00000001
	.section .rom.003f20f6, "ax"
	.incbin "baserom.gba", 0x003f20f6, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f20f8, 0x00000a6c
	.section .rom.003f393a, "ax"
	.incbin "baserom.gba", 0x003f393a, 0x00000002
	.section .rom.003f3cbd, "ax"
	.incbin "baserom.gba", 0x003f3cbd, 0x00000003
	.section .rom.003f496d, "ax"
	.incbin "baserom.gba", 0x003f496d, 0x00000003
	.section .rom.003f54b5, "ax"
	.incbin "baserom.gba", 0x003f54b5, 0x00000003
	.section .rom.003f564e, "ax"
	.incbin "baserom.gba", 0x003f564e, 0x00000002
	.section .rom.003f5edb, "ax"
	.incbin "baserom.gba", 0x003f5edb, 0x00000001
	.section .rom.003f69a2, "ax"
	.incbin "baserom.gba", 0x003f69a2, 0x00000002
	.section .rom.003f6eba, "ax"
	.incbin "baserom.gba", 0x003f6eba, 0x00000002
	.section .rom.003f74de, "ax"
	.incbin "baserom.gba", 0x003f74de, 0x00000002
	.section .rom.003f7d45, "ax"
	.incbin "baserom.gba", 0x003f7d45, 0x00000003
	.section .rom.003f8169, "ax"
	.incbin "baserom.gba", 0x003f8169, 0x00000003
	.section .rom.003f8403, "ax"
	.incbin "baserom.gba", 0x003f8403, 0x00000001
	.section .rom.003f9d9f, "ax"
	.incbin "baserom.gba", 0x003f9d9f, 0x00000001
	.section .rom.003fbb16, "ax"
	.incbin "baserom.gba", 0x003fbb16, 0x00000002
	.section .rom.003fda8b, "ax"
	.incbin "baserom.gba", 0x003fda8b, 0x00000001
	.section .rom.003fdf5b, "ax"
	.incbin "baserom.gba", 0x003fdf5b, 0x00000001
	.section .rom.003fe5ed, "ax"
	.incbin "baserom.gba", 0x003fe5ed, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe5f0, 0x00000a34
	.section .rom.004003f5, "ax"
	.incbin "baserom.gba", 0x004003f5, 0x00000003
	.section .rom.00400ec6, "ax"
	.incbin "baserom.gba", 0x00400ec6, 0x00000002
	.section .rom.00401a03, "ax"
	.incbin "baserom.gba", 0x00401a03, 0x00000001
	.section .rom.00402043, "ax"
	.incbin "baserom.gba", 0x00402043, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00402044, 0x00001588
	.section .rom.0040362d, "ax"
	.incbin "baserom.gba", 0x0040362d, 0x00000003
	.section .rom.0040399b, "ax"
	.incbin "baserom.gba", 0x0040399b, 0x00000001
	.section .rom.00403f39, "ax"
	.incbin "baserom.gba", 0x00403f39, 0x00000003
	.section .rom.0040426d, "ax"
	.incbin "baserom.gba", 0x0040426d, 0x00000003
	.section .rom.004045a9, "ax"
	.incbin "baserom.gba", 0x004045a9, 0x00000003
	.section .rom.004055c2, "ax"
	.incbin "baserom.gba", 0x004055c2, 0x00000002
	.section .rom.00408bce, "ax"
	.incbin "baserom.gba", 0x00408bce, 0x00000002
	.section .rom.00409371, "ax"
	.incbin "baserom.gba", 0x00409371, 0x00000003
	.section .rom.0040a703, "ax"
	.incbin "baserom.gba", 0x0040a703, 0x00000001
	.section .rom.0040ab33, "ax"
	.incbin "baserom.gba", 0x0040ab33, 0x00000001
	.section .rom.0040c929, "ax"
	.incbin "baserom.gba", 0x0040c929, 0x00000003
	.section .rom.0040d23e, "ax"
	.incbin "baserom.gba", 0x0040d23e, 0x00000002
	.section .rom.0040ed72, "ax"
	.incbin "baserom.gba", 0x0040ed72, 0x00000002
	.section .rom.0040fdc1, "ax"
	.incbin "baserom.gba", 0x0040fdc1, 0x00000003
	.section .rom.00410477, "ax"
	.incbin "baserom.gba", 0x00410477, 0x00000001
	.section .rom.0041151d, "ax"
	.incbin "baserom.gba", 0x0041151d, 0x00000003
	.section .rom.0042492f, "ax"
	.incbin "baserom.gba", 0x0042492f, 0x00000001
	.section .rom.00424a81, "ax"
	.incbin "baserom.gba", 0x00424a81, 0x00000003
	.section .rom.00424f12, "ax"
	.incbin "baserom.gba", 0x00424f12, 0x00000002
	.section .rom.00425109, "ax"
	.incbin "baserom.gba", 0x00425109, 0x00000003
	.section .rom.004267c6, "ax"
	.incbin "baserom.gba", 0x004267c6, 0x00000002
	.section .rom.00427ce5, "ax"
	.incbin "baserom.gba", 0x00427ce5, 0x00000003
	.section .rom.004288b1, "ax"
	.incbin "baserom.gba", 0x004288b1, 0x00000003
	.section .rom.00429426, "ax"
	.incbin "baserom.gba", 0x00429426, 0x00000002
	.section .rom.0042b2cf, "ax"
	.incbin "baserom.gba", 0x0042b2cf, 0x00000001
	.section .rom.0042ca5d, "ax"
	.incbin "baserom.gba", 0x0042ca5d, 0x00000003
	.section .rom.0042df0e, "ax"
	.incbin "baserom.gba", 0x0042df0e, 0x00000002
	.section .rom.0043065b, "ax"
	.incbin "baserom.gba", 0x0043065b, 0x00000001
	.section .rom.00431ef5, "ax"
	.incbin "baserom.gba", 0x00431ef5, 0x00000003
	.section .rom.004334be, "ax"
	.incbin "baserom.gba", 0x004334be, 0x00000002
	.section .rom.00434f4e, "ax"
	.incbin "baserom.gba", 0x00434f4e, 0x00000002
	.section .rom.0043f536, "ax"
	.incbin "baserom.gba", 0x0043f536, 0x00000002
	.section .rom.004416ea, "ax"
	.incbin "baserom.gba", 0x004416ea, 0x00000002
	.section .rom.00445851, "ax"
	.incbin "baserom.gba", 0x00445851, 0x00000003
	.section .rom.00448a76, "ax"
	.incbin "baserom.gba", 0x00448a76, 0x00000002
	.section .rom.0044a17a, "ax"
	.incbin "baserom.gba", 0x0044a17a, 0x00000002
	.section .rom.00450d62, "ax"
	.incbin "baserom.gba", 0x00450d62, 0x00000002
	.section .rom.004596e6, "ax"
	.incbin "baserom.gba", 0x004596e6, 0x00000002
	.section .rom.004611c2, "ax"
	.incbin "baserom.gba", 0x004611c2, 0x00000002
	.section .rom.00464256, "ax"
	.incbin "baserom.gba", 0x00464256, 0x00000002
	.section .rom.00467665, "ax"
	.incbin "baserom.gba", 0x00467665, 0x00000003
	.section .rom.00469ed1, "ax"
	.incbin "baserom.gba", 0x00469ed1, 0x00000003
	.section .rom.0046b9c2, "ax"
	.incbin "baserom.gba", 0x0046b9c2, 0x00000002
	.section .rom.0046c7da, "ax"
	.incbin "baserom.gba", 0x0046c7da, 0x00000002
	.section .rom.0046d4c5, "ax"
	.incbin "baserom.gba", 0x0046d4c5, 0x00000003
	.section .rom.00476982, "ax"
	.incbin "baserom.gba", 0x00476982, 0x00000002
	.section .rom.0047768d, "ax"
	.incbin "baserom.gba", 0x0047768d, 0x00000003
	.section .rom.004797a1, "ax"
	.incbin "baserom.gba", 0x004797a1, 0x00000003
	.section .rom.0047a413, "ax"
	.incbin "baserom.gba", 0x0047a413, 0x00000001
	.section .rom.0047b02b, "ax"
	.incbin "baserom.gba", 0x0047b02b, 0x00000001
	.section .rom.0047b79f, "ax"
	.incbin "baserom.gba", 0x0047b79f, 0x00000001
	.section .rom.0047d02f, "ax"
	.incbin "baserom.gba", 0x0047d02f, 0x00000001
	.section .rom.0047d9fd, "ax"
	.incbin "baserom.gba", 0x0047d9fd, 0x00000003
	.section .rom.0047e61b, "ax"
	.incbin "baserom.gba", 0x0047e61b, 0x00000001
	.section .rom.00480ccf, "ax"
	.incbin "baserom.gba", 0x00480ccf, 0x00000001
	.section .rom.004833e2, "ax"
	.incbin "baserom.gba", 0x004833e2, 0x00000002
	.section .rom.00488337, "ax"
	.incbin "baserom.gba", 0x00488337, 0x00000001
	.section .rom.0048c661, "ax"
	.incbin "baserom.gba", 0x0048c661, 0x00000003
	.section .rom.0048d24e, "ax"
	.incbin "baserom.gba", 0x0048d24e, 0x00000002
	.section .rom.00491e03, "ax"
	.incbin "baserom.gba", 0x00491e03, 0x00000001
	.section .rom.0049416d, "ax"
	.incbin "baserom.gba", 0x0049416d, 0x00000003
	.section .rom.00495b0f, "ax"
	.incbin "baserom.gba", 0x00495b0f, 0x00000001
	.section .rom.0049a3d1, "ax"
	.incbin "baserom.gba", 0x0049a3d1, 0x00000003
	.section .rom.0049e499, "ax"
	.incbin "baserom.gba", 0x0049e499, 0x00000003
	.section .rom.004a1b62, "ax"
	.incbin "baserom.gba", 0x004a1b62, 0x00000002
	.section .rom.004a9b33, "ax"
	.incbin "baserom.gba", 0x004a9b33, 0x00000001
	.section .rom.004b0e43, "ax"
	.incbin "baserom.gba", 0x004b0e43, 0x00000001
	.section .rom.004b5dc3, "ax"
	.incbin "baserom.gba", 0x004b5dc3, 0x00000001
	.section .rom.004ba773, "ax"
	.incbin "baserom.gba", 0x004ba773, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004ba774, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004ba780, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004ba8d0, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004baa10, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bab50, 0x00000140
	.section .rom.004c009f, "ax"
	.incbin "baserom.gba", 0x004c009f, 0x00000001
	.section .rom.004c022d, "ax"
	.incbin "baserom.gba", 0x004c022d, 0x00000003
	.section .rom.004c528d, "ax"
	.incbin "baserom.gba", 0x004c528d, 0x00000003
	.section .rom.004c97f9, "ax"
	.incbin "baserom.gba", 0x004c97f9, 0x00000003
	.section .rom.004ce9bd, "ax"
	.incbin "baserom.gba", 0x004ce9bd, 0x00000003
	.section .rom.004ceb53, "ax"
	.incbin "baserom.gba", 0x004ceb53, 0x00000001
	.section .rom.004d17a7, "ax"
	.incbin "baserom.gba", 0x004d17a7, 0x00000001
	.section .rom.004d845b, "ax"
	.incbin "baserom.gba", 0x004d845b, 0x00000001
	.section .rom.004db596, "ax"
	.incbin "baserom.gba", 0x004db596, 0x00000002
	.section .rom.004db717, "ax"
	.incbin "baserom.gba", 0x004db717, 0x00000001
	.section .rom.004dded1, "ax"
	.incbin "baserom.gba", 0x004dded1, 0x00000003
	.section .rom.004e051a, "ax"
	.incbin "baserom.gba", 0x004e051a, 0x00000002
	.section .rom.004e2bfe, "ax"
	.incbin "baserom.gba", 0x004e2bfe, 0x00000002
	.section .rom.004e3c73, "ax"
	.incbin "baserom.gba", 0x004e3c73, 0x00000001
	.section .rom.004e5dc6, "ax"
	.incbin "baserom.gba", 0x004e5dc6, 0x00000002
	.section .rom.004e5f59, "ax"
	.incbin "baserom.gba", 0x004e5f59, 0x00000003
	.section .rom.004e879b, "ax"
	.incbin "baserom.gba", 0x004e879b, 0x00000001
	.section .rom.004ea5b1, "ax"
	.incbin "baserom.gba", 0x004ea5b1, 0x00000003
	.section .rom.004ecc92, "ax"
	.incbin "baserom.gba", 0x004ecc92, 0x00000002
	.section .rom.004eddd9, "ax"
	.incbin "baserom.gba", 0x004eddd9, 0x00000003
	.section .rom.004f0eed, "ax"
	.incbin "baserom.gba", 0x004f0eed, 0x00000003
	.section .rom.004f5055, "ax"
	.incbin "baserom.gba", 0x004f5055, 0x00000003
	.section .rom.004f6652, "ax"
	.incbin "baserom.gba", 0x004f6652, 0x00000002
	.section .rom.004f791b, "ax"
	.incbin "baserom.gba", 0x004f791b, 0x00000001
	.section .rom.004fad33, "ax"
	.incbin "baserom.gba", 0x004fad33, 0x00000001
	.section .rom.004faec1, "ax"
	.incbin "baserom.gba", 0x004faec1, 0x00000003
	.section .rom.004fcc72, "ax"
	.incbin "baserom.gba", 0x004fcc72, 0x00000002
	.section .rom.005007e9, "ax"
	.incbin "baserom.gba", 0x005007e9, 0x00000003
	.section .rom.00501cf7, "ax"
	.incbin "baserom.gba", 0x00501cf7, 0x00000001
	.section .rom.005028cb, "ax"
	.incbin "baserom.gba", 0x005028cb, 0x00000001
	.section .rom.005029c9, "ax"
	.incbin "baserom.gba", 0x005029c9, 0x00000003
	.section .rom.00503dae, "ax"
	.incbin "baserom.gba", 0x00503dae, 0x00000002
	.section .rom.00504f31, "ax"
	.incbin "baserom.gba", 0x00504f31, 0x00000003
	.section .rom.005058ba, "ax"
	.incbin "baserom.gba", 0x005058ba, 0x00000002
	.section .rom.00508537, "ax"
	.incbin "baserom.gba", 0x00508537, 0x00000001
	.section .rom.0050861a, "ax"
	.incbin "baserom.gba", 0x0050861a, 0x00000002
	.section .rom.00509805, "ax"
	.incbin "baserom.gba", 0x00509805, 0x00000003
	.section .rom.0050b48d, "ax"
	.incbin "baserom.gba", 0x0050b48d, 0x00000003
	.section .rom.0050b99b, "ax"
	.incbin "baserom.gba", 0x0050b99b, 0x00000001
	.section .rom.0050badb, "ax"
	.incbin "baserom.gba", 0x0050badb, 0x00000001
	.section .rom.0050ccf6, "ax"
	.incbin "baserom.gba", 0x0050ccf6, 0x00000002
	.section .rom.0050ce51, "ax"
	.incbin "baserom.gba", 0x0050ce51, 0x00000003
	.section .rom.00511277, "ax"
	.incbin "baserom.gba", 0x00511277, 0x00000001
	.section .rom.00512fa6, "ax"
	.incbin "baserom.gba", 0x00512fa6, 0x00000002
	.section .rom.00513d1f, "ax"
	.incbin "baserom.gba", 0x00513d1f, 0x00000001
	.section .rom.00513e53, "ax"
	.incbin "baserom.gba", 0x00513e53, 0x00000001
	.section .rom.005162d6, "ax"
	.incbin "baserom.gba", 0x005162d6, 0x00000002
	.section .rom.00517053, "ax"
	.incbin "baserom.gba", 0x00517053, 0x00000001
	.section .rom.00517b32, "ax"
	.incbin "baserom.gba", 0x00517b32, 0x00000002
	.section .rom.005189aa, "ax"
	.incbin "baserom.gba", 0x005189aa, 0x00000002
	.section .rom.0051aa7e, "ax"
	.incbin "baserom.gba", 0x0051aa7e, 0x00000002
	.section .rom.0051b3c5, "ax"
	.incbin "baserom.gba", 0x0051b3c5, 0x00000003
	.section .rom.0051c012, "ax"
	.incbin "baserom.gba", 0x0051c012, 0x00000002
	.section .rom.0051c239, "ax"
	.incbin "baserom.gba", 0x0051c239, 0x00000003
	.section .rom.0051db7b, "ax"
	.incbin "baserom.gba", 0x0051db7b, 0x00000001
	.section .rom.0051dc8f, "ax"
	.incbin "baserom.gba", 0x0051dc8f, 0x00000001
	.section .rom.0051f655, "ax"
	.incbin "baserom.gba", 0x0051f655, 0x00000003
	.section .rom.0051f79f, "ax"
	.incbin "baserom.gba", 0x0051f79f, 0x00000001
	.section .rom.0052186b, "ax"
	.incbin "baserom.gba", 0x0052186b, 0x00000001
	.section .rom.00521993, "ax"
	.incbin "baserom.gba", 0x00521993, 0x00000001
	.section .rom.0052321b, "ax"
	.incbin "baserom.gba", 0x0052321b, 0x00000001
	.section .rom.005254bd, "ax"
	.incbin "baserom.gba", 0x005254bd, 0x00000003
	.section .rom.005255ff, "ax"
	.incbin "baserom.gba", 0x005255ff, 0x00000001
	.section .rom.00527607, "ax"
	.incbin "baserom.gba", 0x00527607, 0x00000001
	.section .rom.00527712, "ax"
	.incbin "baserom.gba", 0x00527712, 0x00000002
	.section .rom.005296f6, "ax"
	.incbin "baserom.gba", 0x005296f6, 0x00000002
	.section .rom.0052ae1b, "ax"
	.incbin "baserom.gba", 0x0052ae1b, 0x00000001
	.section .rom.0052becf, "ax"
	.incbin "baserom.gba", 0x0052becf, 0x00000001
	.section .rom.0052dbb6, "ax"
	.incbin "baserom.gba", 0x0052dbb6, 0x00000002
	.section .rom.0052dd11, "ax"
	.incbin "baserom.gba", 0x0052dd11, 0x00000003
	.section .rom.0052fc5a, "ax"
	.incbin "baserom.gba", 0x0052fc5a, 0x00000002
	.section .rom.0052fddf, "ax"
	.incbin "baserom.gba", 0x0052fddf, 0x00000001
	.section .rom.00532931, "ax"
	.incbin "baserom.gba", 0x00532931, 0x00000003
	.section .rom.00534ff6, "ax"
	.incbin "baserom.gba", 0x00534ff6, 0x00000002
	.section .rom.0053774e, "ax"
	.incbin "baserom.gba", 0x0053774e, 0x00000002
	.section .rom.00538fa1, "ax"
	.incbin "baserom.gba", 0x00538fa1, 0x00000003
	.section .rom.0053b6a1, "ax"
	.incbin "baserom.gba", 0x0053b6a1, 0x00000003
	.section .rom.0053d46d, "ax"
	.incbin "baserom.gba", 0x0053d46d, 0x00000003
	.section .rom.0053f9b7, "ax"
	.incbin "baserom.gba", 0x0053f9b7, 0x00000001
	.section .rom.00540c82, "ax"
	.incbin "baserom.gba", 0x00540c82, 0x00000002
	.section .rom.00541933, "ax"
	.incbin "baserom.gba", 0x00541933, 0x00000001
	.section .rom.0054239d, "ax"
	.incbin "baserom.gba", 0x0054239d, 0x00000003
	.section .rom.00542493, "ax"
	.incbin "baserom.gba", 0x00542493, 0x00000001
	.section .rom.00543ef2, "ax"
	.incbin "baserom.gba", 0x00543ef2, 0x00000002
	.section .rom.00544c96, "ax"
	.incbin "baserom.gba", 0x00544c96, 0x00000002
	.section .rom.005467cf, "ax"
	.incbin "baserom.gba", 0x005467cf, 0x00000001
	.section .rom.005496bd, "ax"
	.incbin "baserom.gba", 0x005496bd, 0x00000003
	.section .rom.00550ea3, "ax"
	.incbin "baserom.gba", 0x00550ea3, 0x00000001
	.section .rom.00550f6f, "ax"
	.incbin "baserom.gba", 0x00550f6f, 0x00000001
	.section .rom.00556731, "ax"
	.incbin "baserom.gba", 0x00556731, 0x00000003
	.section .rom.00556873, "ax"
	.incbin "baserom.gba", 0x00556873, 0x00000001
	.section .rom.00557ca1, "ax"
	.incbin "baserom.gba", 0x00557ca1, 0x00000003
	.section .rom.0055a9de, "ax"
	.incbin "baserom.gba", 0x0055a9de, 0x00000002
	.section .rom.0055cada, "ax"
	.incbin "baserom.gba", 0x0055cada, 0x00000002
	.section .rom.005644e6, "ax"
	.incbin "baserom.gba", 0x005644e6, 0x00000002
	.section .rom.00564682, "ax"
	.incbin "baserom.gba", 0x00564682, 0x00000002
	.section .rom.00568f1d, "ax"
	.incbin "baserom.gba", 0x00568f1d, 0x00000003
	.section .rom.0056b41e, "ax"
	.incbin "baserom.gba", 0x0056b41e, 0x00000002
	.section .rom.00572e0f, "ax"
	.incbin "baserom.gba", 0x00572e0f, 0x00000001
	.section .rom.00572faa, "ax"
	.incbin "baserom.gba", 0x00572faa, 0x00000002
	.section .rom.00575b59, "ax"
	.incbin "baserom.gba", 0x00575b59, 0x00000003
	.section .rom.00577c2d, "ax"
	.incbin "baserom.gba", 0x00577c2d, 0x00000003
	.section .rom.00579c86, "ax"
	.incbin "baserom.gba", 0x00579c86, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x00579c88, 0x00001cec
	.section .rom.0057d11d, "ax"
	.incbin "baserom.gba", 0x0057d11d, 0x00000003
	.section .rom.0057fdf9, "ax"
	.incbin "baserom.gba", 0x0057fdf9, 0x00000003
	.section .rom.00581bcb, "ax"
	.incbin "baserom.gba", 0x00581bcb, 0x00000001
	.section .rom.00581d0b, "ax"
	.incbin "baserom.gba", 0x00581d0b, 0x00000001
	.section .rom.00584d65, "ax"
	.incbin "baserom.gba", 0x00584d65, 0x00000003
	.section .rom.00587ce2, "ax"
	.incbin "baserom.gba", 0x00587ce2, 0x00000002
	.section .rom.0058a5a9, "ax"
	.incbin "baserom.gba", 0x0058a5a9, 0x00000003
	.section .rom.00591511, "ax"
	.incbin "baserom.gba", 0x00591511, 0x00000003
	.section .rom.00592db7, "ax"
	.incbin "baserom.gba", 0x00592db7, 0x00000001
	.section .rom.005936b6, "ax"
	.incbin "baserom.gba", 0x005936b6, 0x00000002
	.section .rom.0059442f, "ax"
	.incbin "baserom.gba", 0x0059442f, 0x00000001
	.section .rom.0059458f, "ax"
	.incbin "baserom.gba", 0x0059458f, 0x00000001
	.section .rom.005961e7, "ax"
	.incbin "baserom.gba", 0x005961e7, 0x00000001
	.section .rom.00597a7f, "ax"
	.incbin "baserom.gba", 0x00597a7f, 0x00000001
	.section .rom.00597c03, "ax"
	.incbin "baserom.gba", 0x00597c03, 0x00000001
	.section .rom.0059a73f, "ax"
	.incbin "baserom.gba", 0x0059a73f, 0x00000001
	.section .rom.0059ceb9, "ax"
	.incbin "baserom.gba", 0x0059ceb9, 0x00000003
	.section .rom.0059dea6, "ax"
	.incbin "baserom.gba", 0x0059dea6, 0x00000002
	.section .rom.0059f567, "ax"
	.incbin "baserom.gba", 0x0059f567, 0x00000001
	.section .rom.0059f711, "ax"
	.incbin "baserom.gba", 0x0059f711, 0x00000003
	.section .rom.005a24a5, "ax"
	.incbin "baserom.gba", 0x005a24a5, 0x00000003
	.section .rom.005a653a, "ax"
	.incbin "baserom.gba", 0x005a653a, 0x00000002
	.section .rom.005a667b, "ax"
	.incbin "baserom.gba", 0x005a667b, 0x00000001
	.section .rom.005a8543, "ax"
	.incbin "baserom.gba", 0x005a8543, 0x00000001
	.section .rom.005a86e5, "ax"
	.incbin "baserom.gba", 0x005a86e5, 0x00000003
	.section .rom.005aa787, "ax"
	.incbin "baserom.gba", 0x005aa787, 0x00000001
	.section .rom.005ab714, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005ab714, 0x000022d8
	.section .rom.005b04dd, "ax"
	.incbin "baserom.gba", 0x005b04dd, 0x00000003
	.section .rom.005b0662, "ax"
	.incbin "baserom.gba", 0x005b0662, 0x00000002
	.section .rom.005b7daf, "ax"
	.incbin "baserom.gba", 0x005b7daf, 0x00000001
	.section .rom.005b82b9, "ax"
	.incbin "baserom.gba", 0x005b82b9, 0x00000003
	.section .rom.005b9991, "ax"
	.incbin "baserom.gba", 0x005b9991, 0x00000003
	.section .rom.005b9b35, "ax"
	.incbin "baserom.gba", 0x005b9b35, 0x00000003
	.section .rom.005bb43a, "ax"
	.incbin "baserom.gba", 0x005bb43a, 0x00000002
	.section .rom.005bcc67, "ax"
	.incbin "baserom.gba", 0x005bcc67, 0x00000001
	.section .rom.005bcde5, "ax"
	.incbin "baserom.gba", 0x005bcde5, 0x00000003
	.section .rom.005c16d9, "ax"
	.incbin "baserom.gba", 0x005c16d9, 0x00000003
	.section .rom.005c271e, "ax"
	.incbin "baserom.gba", 0x005c271e, 0x00000002
	.section .rom.005c3542, "ax"
	.incbin "baserom.gba", 0x005c3542, 0x00000002
	.section .rom.005d7226, "ax"
	.incbin "baserom.gba", 0x005d7226, 0x00000002
	.section .rom.005ed63e, "ax"
	.incbin "baserom.gba", 0x005ed63e, 0x00000002
	.section .rom.00609114, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609114, 0x00002904
	.section .rom.00690d29, "ax"
	.incbin "baserom.gba", 0x00690d29, 0x00000003
	.section .rom.006d54e9, "ax"
	.incbin "baserom.gba", 0x006d54e9, 0x00000003
	.section .rom.007177b8, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007177b8, 0x00001164
	.section .rom.00745523, "ax"
	.incbin "baserom.gba", 0x00745523, 0x00000001
	.section .rom.007557a0, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x007557a0, 0x000000e4
	.section .rom.0075ba20, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075ba20, 0x00004600
