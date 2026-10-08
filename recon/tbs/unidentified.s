	.section .unidentified.08007a68,"a"
	.incbin "baserom.gba", 0x00007a68, 0x00000024
	.section .unidentified.08007b38,"a"
	.incbin "baserom.gba", 0x00007b38, 0x00000004
	.section .unidentified.08033e24,"a"
	.incbin "baserom.gba", 0x00033e24, 0x0000001c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x00033e40, 0x00000020
	.section .unidentified.0809d170,"a"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x0009d170, 0x00000638
	.section .unidentified.0809d9f0,"a"
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x0009d9f0, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x0009ddd8, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x0009e1d8, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x0009e270, 0x00000218
	.section .unidentified.0809e4ce,"a"
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x0009e4ce, 0x000001b2
	.section .unidentified.080eda78,"a"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000eda78, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000eda80, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.global Data_080eda88
Data_080eda88:
	.incbin "baserom.gba", 0x000eda88, 0x00000010
	.global Mode12_Points
Mode12_Points:
	.incbin "baserom.gba", 0x000eda98, 0x00000018
	.global RisingWall_UnitScale
RisingWall_UnitScale:
	.incbin "baserom.gba", 0x000edab0, 0x00000008
	.global Formation_NormalScale
Formation_NormalScale:
	.incbin "baserom.gba", 0x000edab8, 0x00000008
	.global Formation_SmallScale
Formation_SmallScale:
	.incbin "baserom.gba", 0x000edac0, 0x00000008
	.global CirclingScene_UnitScale
CirclingScene_UnitScale:
	.incbin "baserom.gba", 0x000edac8, 0x00000008
	.global ShockwaveScene_UnitScale
ShockwaveScene_UnitScale:
	.incbin "baserom.gba", 0x000edad0, 0x00000008
	.global ObjectRow_SweepPair
ObjectRow_SweepPair:
	.incbin "baserom.gba", 0x000edad8, 0x00000008
	.global ObjectRow_RisePair
ObjectRow_RisePair:
	.incbin "baserom.gba", 0x000edae0, 0x00000008
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000edae8, 0x00000008
	.section .unidentified.080ede48,"a"
	.global ParticleStreams_CellOffsets
ParticleStreams_CellOffsets:
	.incbin "baserom.gba", 0x000ede48, 0x00000014
	.global BattleFx6_FlareCells
BattleFx6_FlareCells:
	.incbin "baserom.gba", 0x000ede5c, 0x00000028
	.global BattleFx_PuffCells
BattleFx_PuffCells:
	.incbin "baserom.gba", 0x000ede84, 0x00000012
	.global BattleFx_PuffSizes
BattleFx_PuffSizes:
	.incbin "baserom.gba", 0x000ede96, 0x00000009
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
	.incbin "baserom.gba", 0x000edf76, 0x00000004
	.incbin "baserom.gba", 0x000edf7a, 0x00000001
	.global TwelveMode_GlintDrawFlags
TwelveMode_GlintDrawFlags:
	.incbin "baserom.gba", 0x000edf7b, 0x00000003
	.incbin "baserom.gba", 0x000edf7e, 0x00000001
	.global BladeRain_CellWidths
BladeRain_CellWidths:
	.incbin "baserom.gba", 0x000edf7f, 0x00000003
	.incbin "baserom.gba", 0x000edf82, 0x00000001
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
	.incbin "baserom.gba", 0x000ee0b0, 0x00000002
	.incbin "baserom.gba", 0x000ee0b2, 0x00000001
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
	.incbin "baserom.gba", 0x000ee15a, 0x00000008
	.incbin "baserom.gba", 0x000ee162, 0x00000001
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000ee163, 0x00000009
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000ee16c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000ee174, 0x00000002
	.incbin "baserom.gba", 0x000ee176, 0x00000001
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
	.incbin "baserom.gba", 0x000ee1d3, 0x00000021
	.incbin "baserom.gba", 0x000ee1f4, 0x00000001
	.global Crystal_Counts
Crystal_Counts:
	.incbin "baserom.gba", 0x000ee1f5, 0x00000005
	.incbin "baserom.gba", 0x000ee1fa, 0x00000001
	.global Crystal_ShardWidths
Crystal_ShardWidths:
	.incbin "baserom.gba", 0x000ee1fb, 0x0000000b
	.incbin "baserom.gba", 0x000ee206, 0x00000001
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
	.incbin "baserom.gba", 0x000ee29a, 0x00000002
	.incbin "baserom.gba", 0x000ee29c, 0x00000001
	.global FirePillars_Depths
FirePillars_Depths:
	.incbin "baserom.gba", 0x000ee29d, 0x0000000b
	.incbin "baserom.gba", 0x000ee2a8, 0x00000001
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
	.incbin "baserom.gba", 0x000ee920, 0x00000004
	.incbin "baserom.gba", 0x000ee924, 0x00000001
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
	.incbin "baserom.gba", 0x000ee93e, 0x00000004
	.incbin "baserom.gba", 0x000ee942, 0x00000001
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
	.incbin "baserom.gba", 0x000eea38, 0x00000008
	.incbin "baserom.gba", 0x000eea40, 0x00000001
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
	.incbin "baserom.gba", 0x000eea88, 0x00000008
	.incbin "baserom.gba", 0x000eea90, 0x00000001
	.global RisingMotes_MoteWidths
RisingMotes_MoteWidths:
	.incbin "baserom.gba", 0x000eea91, 0x00000007
	.incbin "baserom.gba", 0x000eea98, 0x00000001
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
	.incbin "baserom.gba", 0x000eeab8, 0x00000002
	.incbin "baserom.gba", 0x000eeaba, 0x00000001
	.global RisingMotes_EmberWidths
RisingMotes_EmberWidths:
	.incbin "baserom.gba", 0x000eeabb, 0x00000007
	.incbin "baserom.gba", 0x000eeac2, 0x00000001
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
	.global Data_080eeafa
Data_080eeafa:
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
	.incbin "baserom.gba", 0x000eebe6, 0x00000002
	.incbin "baserom.gba", 0x000eebe8, 0x00000001
	.global Volley_CometHeights
Volley_CometHeights:
	.incbin "baserom.gba", 0x000eebe9, 0x00000003
	.global Volley_Modes
Volley_Modes:
	.incbin "baserom.gba", 0x000eebec, 0x0000003c
	.global Volley_FlashWidths
Volley_FlashWidths:
	.incbin "baserom.gba", 0x000eec28, 0x00000006
	.incbin "baserom.gba", 0x000eec2e, 0x00000001
	.global Volley_FlashHeights
Volley_FlashHeights:
	.incbin "baserom.gba", 0x000eec2f, 0x00000007
	.global Volley_FlashSteps
Volley_FlashSteps:
	.incbin "baserom.gba", 0x000eec36, 0x00000006
	.incbin "baserom.gba", 0x000eec3c, 0x00000001
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
	.incbin "baserom.gba", 0x000eecb2, 0x0000002a
	.incbin "baserom.gba", 0x000eecdc, 0x00000016
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
	.incbin "baserom.gba", 0x000eed1e, 0x0000001e
	.incbin "baserom.gba", 0x000eed3c, 0x00000002
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
	.incbin "baserom.gba", 0x000eeda0, 0x00000002
	.incbin "baserom.gba", 0x000eeda2, 0x00000001
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
	.incbin "baserom.gba", 0x000eedf4, 0x00000006
	.incbin "baserom.gba", 0x000eedfa, 0x00000001
	.global FireSwirl_CellHeights
FireSwirl_CellHeights:
	.incbin "baserom.gba", 0x000eedfb, 0x00000007
	.global FireSwirl_CellSourceOffsets
FireSwirl_CellSourceOffsets:
	.incbin "baserom.gba", 0x000eee02, 0x0000000e
	.global FireSwirl_CellBiasX
FireSwirl_CellBiasX:
	.incbin "baserom.gba", 0x000eee10, 0x00000006
	.incbin "baserom.gba", 0x000eee16, 0x00000001
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
	.incbin "baserom.gba", 0x000eeed8, 0x00000008
	.incbin "baserom.gba", 0x000eeee0, 0x00000001
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
	.section .unidentified.080ef014,"a"
	.incbin "baserom.gba", 0x000ef014, 0x00000020
	.global SentouKouka_BitOperands
SentouKouka_BitOperands:
	.incbin "baserom.gba", 0x000ef034, 0x00000020
	.section .unidentified.080f0a5c,"a"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f0a5c, 0x00000760
	.incbin "baserom.gba", 0x000f11bc, 0x00000001
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f11bd, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f1220, 0x00000550
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000f1770, 0x000002f3
	.section .unidentified.080f38bc,"a"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f38bc, 0x000000ee
	.incbin "baserom.gba", 0x000f39aa, 0x00000001
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f39ab, 0x00000005
	.incbin "baserom.gba", 0x000f39b0, 0x00000001
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f39b1, 0x0000003d
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000f39ee, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000f3a2e, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000f3a6e, 0x00000040
	.section .unidentified.080f53dc,"a"
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
	.section .unidentified.080f86f8,"a"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000f86f8, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000f870c, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000f8712, 0x00000008
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000f871a, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000f8728, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000f8736, 0x00000008
	.section .unidentified.080fb7a0,"a"
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
	.section .unidentified.080fc504,"a"
	.incbin "baserom.gba", 0x000fc504, 0x00000090
	.section .unidentified.080fc624,"a"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fc624, 0x00000060
	.section .unidentified.083203c8,"a"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00320fa0, 0x00000010
	.section .unidentified.083249e7,"a"
	.incbin "baserom.gba", 0x003249e7, 0x00000001
	.section .unidentified.0832b0a6,"a"
	.incbin "baserom.gba", 0x0032b0a6, 0x00000002
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032b0a8, 0x000086f8
	.section .unidentified.083357f1,"a"
	.incbin "baserom.gba", 0x003357f1, 0x00000003
	.section .unidentified.08337101,"a"
	.incbin "baserom.gba", 0x00337101, 0x00000003
	.section .unidentified.0833ac05,"a"
	.incbin "baserom.gba", 0x0033ac05, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033ac08, 0x000002b4
	.section .unidentified.0833f2fe,"a"
	.incbin "baserom.gba", 0x0033f2fe, 0x00000002
	.section .unidentified.0834fa6e,"a"
	.incbin "baserom.gba", 0x0034fa6e, 0x00000002
	.section .unidentified.0835305e,"a"
	.incbin "baserom.gba", 0x0035305e, 0x00000002
	.section .unidentified.0835eb02,"a"
	.incbin "baserom.gba", 0x0035eb02, 0x00000002
	.section .unidentified.0836f1b2,"a"
	.incbin "baserom.gba", 0x0036f1b2, 0x00000002
	.section .unidentified.08376c52,"a"
	.incbin "baserom.gba", 0x00376c52, 0x00000002
	.section .unidentified.0837a492,"a"
	.incbin "baserom.gba", 0x0037a492, 0x00000002
	.section .unidentified.083838c6,"a"
	.incbin "baserom.gba", 0x003838c6, 0x00000002
	.section .unidentified.0838ecee,"a"
	.incbin "baserom.gba", 0x0038ecee, 0x00000002
	.section .unidentified.08396baa,"a"
	.incbin "baserom.gba", 0x00396baa, 0x00000002
	.section .unidentified.0839b63a,"a"
	.incbin "baserom.gba", 0x0039b63a, 0x00000002
	.section .unidentified.0839f8ee,"a"
	.incbin "baserom.gba", 0x0039f8ee, 0x00000002
	.section .unidentified.083a326e,"a"
	.incbin "baserom.gba", 0x003a326e, 0x00000002
	.section .unidentified.083afaaa,"a"
	.incbin "baserom.gba", 0x003afaaa, 0x00000002
	.section .unidentified.083be3f2,"a"
	.incbin "baserom.gba", 0x003be3f2, 0x00000002
	.section .unidentified.083c3cf5,"a"
	.incbin "baserom.gba", 0x003c3cf5, 0x00000003
	.section .unidentified.083c5677,"a"
	.incbin "baserom.gba", 0x003c5677, 0x00000001
	.section .unidentified.083c8495,"a"
	.incbin "baserom.gba", 0x003c8495, 0x00000003
	.section .unidentified.083cb4b9,"a"
	.incbin "baserom.gba", 0x003cb4b9, 0x00000003
	.section .unidentified.083cbd01,"a"
	.incbin "baserom.gba", 0x003cbd01, 0x00000003
	.section .unidentified.083cd517,"a"
	.incbin "baserom.gba", 0x003cd517, 0x00000001
	.section .unidentified.083cdb02,"a"
	.incbin "baserom.gba", 0x003cdb02, 0x00000002
	.section .unidentified.083ce1c6,"a"
	.incbin "baserom.gba", 0x003ce1c6, 0x00000002
	.section .unidentified.083ce4ad,"a"
	.incbin "baserom.gba", 0x003ce4ad, 0x00000003
	.section .unidentified.083cf813,"a"
	.incbin "baserom.gba", 0x003cf813, 0x00000001
	.section .unidentified.083cfbfd,"a"
	.incbin "baserom.gba", 0x003cfbfd, 0x00000003
	.section .unidentified.083cffcf,"a"
	.incbin "baserom.gba", 0x003cffcf, 0x00000001
	.section .unidentified.083d086f,"a"
	.incbin "baserom.gba", 0x003d086f, 0x00000001
	.section .unidentified.083d0d12,"a"
	.incbin "baserom.gba", 0x003d0d12, 0x00000002
	.section .unidentified.083d1ecb,"a"
	.incbin "baserom.gba", 0x003d1ecb, 0x00000001
	.section .unidentified.083d338e,"a"
	.incbin "baserom.gba", 0x003d338e, 0x00000002
	.section .unidentified.083d4357,"a"
	.incbin "baserom.gba", 0x003d4357, 0x00000001
	.section .unidentified.083d45b2,"a"
	.incbin "baserom.gba", 0x003d45b2, 0x00000002
	.section .unidentified.083d600d,"a"
	.incbin "baserom.gba", 0x003d600d, 0x00000003
	.section .unidentified.083d6559,"a"
	.incbin "baserom.gba", 0x003d6559, 0x00000003
	.section .unidentified.083d82e2,"a"
	.incbin "baserom.gba", 0x003d82e2, 0x00000002
	.section .unidentified.083d855b,"a"
	.incbin "baserom.gba", 0x003d855b, 0x00000001
	.section .unidentified.083d8a22,"a"
	.incbin "baserom.gba", 0x003d8a22, 0x00000002
	.section .unidentified.083da5f7,"a"
	.incbin "baserom.gba", 0x003da5f7, 0x00000001
	.section .unidentified.083dc1f5,"a"
	.incbin "baserom.gba", 0x003dc1f5, 0x00000003
	.section .unidentified.083dc416,"a"
	.incbin "baserom.gba", 0x003dc416, 0x00000002
	.section .unidentified.083dc853,"a"
	.incbin "baserom.gba", 0x003dc853, 0x00000001
	.section .unidentified.083dc965,"a"
	.incbin "baserom.gba", 0x003dc965, 0x00000003
	.section .unidentified.083dd0e3,"a"
	.incbin "baserom.gba", 0x003dd0e3, 0x00000001
	.section .unidentified.083dd581,"a"
	.incbin "baserom.gba", 0x003dd581, 0x00000003
	.section .unidentified.083de296,"a"
	.incbin "baserom.gba", 0x003de296, 0x00000002
	.section .unidentified.083df016,"a"
	.incbin "baserom.gba", 0x003df016, 0x00000002
	.section .unidentified.083df3a7,"a"
	.incbin "baserom.gba", 0x003df3a7, 0x00000001
	.section .unidentified.083e10ee,"a"
	.incbin "baserom.gba", 0x003e10ee, 0x00000002
	.section .unidentified.083e1ec5,"a"
	.incbin "baserom.gba", 0x003e1ec5, 0x00000003
	.section .unidentified.083e26f2,"a"
	.incbin "baserom.gba", 0x003e26f2, 0x00000002
	.section .unidentified.083e2d7d,"a"
	.incbin "baserom.gba", 0x003e2d7d, 0x00000003
	.section .unidentified.083e2f3b,"a"
	.incbin "baserom.gba", 0x003e2f3b, 0x00000001
	.section .unidentified.083e3c93,"a"
	.incbin "baserom.gba", 0x003e3c93, 0x00000001
	.section .unidentified.083e41df,"a"
	.incbin "baserom.gba", 0x003e41df, 0x00000001
	.section .unidentified.083e45b9,"a"
	.incbin "baserom.gba", 0x003e45b9, 0x00000003
	.section .unidentified.083e4967,"a"
	.incbin "baserom.gba", 0x003e4967, 0x00000001
	.section .unidentified.083e690b,"a"
	.incbin "baserom.gba", 0x003e690b, 0x00000001
	.section .unidentified.083e76a7,"a"
	.incbin "baserom.gba", 0x003e76a7, 0x00000001
	.section .unidentified.083e78c3,"a"
	.incbin "baserom.gba", 0x003e78c3, 0x00000001
	.section .unidentified.083e7bbf,"a"
	.incbin "baserom.gba", 0x003e7bbf, 0x00000001
	.section .unidentified.083e9d6d,"a"
	.incbin "baserom.gba", 0x003e9d6d, 0x00000003
	.section .unidentified.083eab3b,"a"
	.incbin "baserom.gba", 0x003eab3b, 0x00000001
	.section .unidentified.083ebc9d,"a"
	.incbin "baserom.gba", 0x003ebc9d, 0x00000003
	.section .unidentified.083ec1f7,"a"
	.incbin "baserom.gba", 0x003ec1f7, 0x00000001
	.section .unidentified.083edae1,"a"
	.incbin "baserom.gba", 0x003edae1, 0x00000003
	.section .unidentified.083ee382,"a"
	.incbin "baserom.gba", 0x003ee382, 0x00000002
	.section .unidentified.083ee75f,"a"
	.incbin "baserom.gba", 0x003ee75f, 0x00000001
	.section .unidentified.083ee9e9,"a"
	.incbin "baserom.gba", 0x003ee9e9, 0x00000003
	.section .unidentified.083eed8f,"a"
	.incbin "baserom.gba", 0x003eed8f, 0x00000001
	.section .unidentified.083eefea,"a"
	.incbin "baserom.gba", 0x003eefea, 0x00000002
	.section .unidentified.083ef3a2,"a"
	.incbin "baserom.gba", 0x003ef3a2, 0x00000002
	.section .unidentified.083f088b,"a"
	.incbin "baserom.gba", 0x003f088b, 0x00000001
	.section .unidentified.083f1e4e,"a"
	.incbin "baserom.gba", 0x003f1e4e, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f1e50, 0x00000a6c
	.section .unidentified.083f3692,"a"
	.incbin "baserom.gba", 0x003f3692, 0x00000002
	.section .unidentified.083f3a15,"a"
	.incbin "baserom.gba", 0x003f3a15, 0x00000003
	.section .unidentified.083f46c5,"a"
	.incbin "baserom.gba", 0x003f46c5, 0x00000003
	.section .unidentified.083f520d,"a"
	.incbin "baserom.gba", 0x003f520d, 0x00000003
	.section .unidentified.083f53a6,"a"
	.incbin "baserom.gba", 0x003f53a6, 0x00000002
	.section .unidentified.083f5c33,"a"
	.incbin "baserom.gba", 0x003f5c33, 0x00000001
	.section .unidentified.083f66fa,"a"
	.incbin "baserom.gba", 0x003f66fa, 0x00000002
	.section .unidentified.083f6c12,"a"
	.incbin "baserom.gba", 0x003f6c12, 0x00000002
	.section .unidentified.083f7236,"a"
	.incbin "baserom.gba", 0x003f7236, 0x00000002
	.section .unidentified.083f7a9d,"a"
	.incbin "baserom.gba", 0x003f7a9d, 0x00000003
	.section .unidentified.083f7ec1,"a"
	.incbin "baserom.gba", 0x003f7ec1, 0x00000003
	.section .unidentified.083f815b,"a"
	.incbin "baserom.gba", 0x003f815b, 0x00000001
	.section .unidentified.083f9af7,"a"
	.incbin "baserom.gba", 0x003f9af7, 0x00000001
	.section .unidentified.083fb86e,"a"
	.incbin "baserom.gba", 0x003fb86e, 0x00000002
	.section .unidentified.083fd7e3,"a"
	.incbin "baserom.gba", 0x003fd7e3, 0x00000001
	.section .unidentified.083fdcb3,"a"
	.incbin "baserom.gba", 0x003fdcb3, 0x00000001
	.section .unidentified.083fe345,"a"
	.incbin "baserom.gba", 0x003fe345, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe348, 0x00000a34
	.section .unidentified.0840014d,"a"
	.incbin "baserom.gba", 0x0040014d, 0x00000003
	.section .unidentified.08400c1e,"a"
	.incbin "baserom.gba", 0x00400c1e, 0x00000002
	.section .unidentified.0840175b,"a"
	.incbin "baserom.gba", 0x0040175b, 0x00000001
	.section .unidentified.08401d9b,"a"
	.incbin "baserom.gba", 0x00401d9b, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00401d9c, 0x00001588
	.section .unidentified.08403385,"a"
	.incbin "baserom.gba", 0x00403385, 0x00000003
	.section .unidentified.084036f3,"a"
	.incbin "baserom.gba", 0x004036f3, 0x00000001
	.section .unidentified.08403c91,"a"
	.incbin "baserom.gba", 0x00403c91, 0x00000003
	.section .unidentified.08403fc5,"a"
	.incbin "baserom.gba", 0x00403fc5, 0x00000003
	.section .unidentified.08404301,"a"
	.incbin "baserom.gba", 0x00404301, 0x00000003
	.section .unidentified.0840531a,"a"
	.incbin "baserom.gba", 0x0040531a, 0x00000002
	.section .unidentified.08408926,"a"
	.incbin "baserom.gba", 0x00408926, 0x00000002
	.section .unidentified.084090c9,"a"
	.incbin "baserom.gba", 0x004090c9, 0x00000003
	.section .unidentified.0840a45b,"a"
	.incbin "baserom.gba", 0x0040a45b, 0x00000001
	.section .unidentified.0840a88b,"a"
	.incbin "baserom.gba", 0x0040a88b, 0x00000001
	.section .unidentified.0840c681,"a"
	.incbin "baserom.gba", 0x0040c681, 0x00000003
	.section .unidentified.0840cf96,"a"
	.incbin "baserom.gba", 0x0040cf96, 0x00000002
	.section .unidentified.0840eaca,"a"
	.incbin "baserom.gba", 0x0040eaca, 0x00000002
	.section .unidentified.0840fb19,"a"
	.incbin "baserom.gba", 0x0040fb19, 0x00000003
	.section .unidentified.084101cf,"a"
	.incbin "baserom.gba", 0x004101cf, 0x00000001
	.section .unidentified.08411275,"a"
	.incbin "baserom.gba", 0x00411275, 0x00000003
	.section .unidentified.08424687,"a"
	.incbin "baserom.gba", 0x00424687, 0x00000001
	.section .unidentified.084247d9,"a"
	.incbin "baserom.gba", 0x004247d9, 0x00000003
	.section .unidentified.08424c6a,"a"
	.incbin "baserom.gba", 0x00424c6a, 0x00000002
	.section .unidentified.08424e61,"a"
	.incbin "baserom.gba", 0x00424e61, 0x00000003
	.section .unidentified.0842651e,"a"
	.incbin "baserom.gba", 0x0042651e, 0x00000002
	.section .unidentified.08427a3d,"a"
	.incbin "baserom.gba", 0x00427a3d, 0x00000003
	.section .unidentified.08428609,"a"
	.incbin "baserom.gba", 0x00428609, 0x00000003
	.section .unidentified.0842917e,"a"
	.incbin "baserom.gba", 0x0042917e, 0x00000002
	.section .unidentified.0842b027,"a"
	.incbin "baserom.gba", 0x0042b027, 0x00000001
	.section .unidentified.0842c7b5,"a"
	.incbin "baserom.gba", 0x0042c7b5, 0x00000003
	.section .unidentified.0842dc66,"a"
	.incbin "baserom.gba", 0x0042dc66, 0x00000002
	.section .unidentified.084303b3,"a"
	.incbin "baserom.gba", 0x004303b3, 0x00000001
	.section .unidentified.08431c4d,"a"
	.incbin "baserom.gba", 0x00431c4d, 0x00000003
	.section .unidentified.08433216,"a"
	.incbin "baserom.gba", 0x00433216, 0x00000002
	.section .unidentified.08434ca6,"a"
	.incbin "baserom.gba", 0x00434ca6, 0x00000002
	.section .unidentified.08435ef7,"a"
	.incbin "baserom.gba", 0x00435ef7, 0x00000001
	.section .unidentified.0843f866,"a"
	.incbin "baserom.gba", 0x0043f866, 0x00000002
	.section .unidentified.08441a1a,"a"
	.incbin "baserom.gba", 0x00441a1a, 0x00000002
	.section .unidentified.08445b81,"a"
	.incbin "baserom.gba", 0x00445b81, 0x00000003
	.section .unidentified.08448da6,"a"
	.incbin "baserom.gba", 0x00448da6, 0x00000002
	.section .unidentified.0844a4aa,"a"
	.incbin "baserom.gba", 0x0044a4aa, 0x00000002
	.section .unidentified.08451092,"a"
	.incbin "baserom.gba", 0x00451092, 0x00000002
	.section .unidentified.08459a16,"a"
	.incbin "baserom.gba", 0x00459a16, 0x00000002
	.section .unidentified.084614f2,"a"
	.incbin "baserom.gba", 0x004614f2, 0x00000002
	.section .unidentified.08464586,"a"
	.incbin "baserom.gba", 0x00464586, 0x00000002
	.section .unidentified.08467995,"a"
	.incbin "baserom.gba", 0x00467995, 0x00000003
	.section .unidentified.0846a201,"a"
	.incbin "baserom.gba", 0x0046a201, 0x00000003
	.section .unidentified.0846bcf2,"a"
	.incbin "baserom.gba", 0x0046bcf2, 0x00000002
	.section .unidentified.0846cb0a,"a"
	.incbin "baserom.gba", 0x0046cb0a, 0x00000002
	.section .unidentified.0846d7f5,"a"
	.incbin "baserom.gba", 0x0046d7f5, 0x00000003
	.section .unidentified.08476cb2,"a"
	.incbin "baserom.gba", 0x00476cb2, 0x00000002
	.section .unidentified.084779bd,"a"
	.incbin "baserom.gba", 0x004779bd, 0x00000003
	.section .unidentified.08479ad1,"a"
	.incbin "baserom.gba", 0x00479ad1, 0x00000003
	.section .unidentified.0847a743,"a"
	.incbin "baserom.gba", 0x0047a743, 0x00000001
	.section .unidentified.0847b35b,"a"
	.incbin "baserom.gba", 0x0047b35b, 0x00000001
	.section .unidentified.0847bacf,"a"
	.incbin "baserom.gba", 0x0047bacf, 0x00000001
	.section .unidentified.0847d35f,"a"
	.incbin "baserom.gba", 0x0047d35f, 0x00000001
	.section .unidentified.0847dd2d,"a"
	.incbin "baserom.gba", 0x0047dd2d, 0x00000003
	.section .unidentified.0847e94b,"a"
	.incbin "baserom.gba", 0x0047e94b, 0x00000001
	.section .unidentified.08480fff,"a"
	.incbin "baserom.gba", 0x00480fff, 0x00000001
	.section .unidentified.08483712,"a"
	.incbin "baserom.gba", 0x00483712, 0x00000002
	.section .unidentified.08488667,"a"
	.incbin "baserom.gba", 0x00488667, 0x00000001
	.section .unidentified.0848c991,"a"
	.incbin "baserom.gba", 0x0048c991, 0x00000003
	.section .unidentified.0848d57e,"a"
	.incbin "baserom.gba", 0x0048d57e, 0x00000002
	.section .unidentified.08492133,"a"
	.incbin "baserom.gba", 0x00492133, 0x00000001
	.section .unidentified.0849449d,"a"
	.incbin "baserom.gba", 0x0049449d, 0x00000003
	.section .unidentified.08495e3f,"a"
	.incbin "baserom.gba", 0x00495e3f, 0x00000001
	.section .unidentified.0849a701,"a"
	.incbin "baserom.gba", 0x0049a701, 0x00000003
	.section .unidentified.0849e7c9,"a"
	.incbin "baserom.gba", 0x0049e7c9, 0x00000003
	.section .unidentified.084a1e92,"a"
	.incbin "baserom.gba", 0x004a1e92, 0x00000002
	.section .unidentified.084a9e63,"a"
	.incbin "baserom.gba", 0x004a9e63, 0x00000001
	.section .unidentified.084b1173,"a"
	.incbin "baserom.gba", 0x004b1173, 0x00000001
	.section .unidentified.084b60f3,"a"
	.incbin "baserom.gba", 0x004b60f3, 0x00000001
	.section .unidentified.084baaa4,"a"
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004baaa4, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004baab0, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004bac00, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004bad40, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bae80, 0x00000140
	.section .unidentified.084c055d,"a"
	.incbin "baserom.gba", 0x004c055d, 0x00000003
	.section .unidentified.084c55bd,"a"
	.incbin "baserom.gba", 0x004c55bd, 0x00000003
	.section .unidentified.084c9b29,"a"
	.incbin "baserom.gba", 0x004c9b29, 0x00000003
	.section .unidentified.084cee87,"a"
	.incbin "baserom.gba", 0x004cee87, 0x00000001
	.section .unidentified.084d1adb,"a"
	.incbin "baserom.gba", 0x004d1adb, 0x00000001
	.section .unidentified.084d878f,"a"
	.incbin "baserom.gba", 0x004d878f, 0x00000001
	.section .unidentified.084dba4b,"a"
	.incbin "baserom.gba", 0x004dba4b, 0x00000001
	.section .unidentified.084de205,"a"
	.incbin "baserom.gba", 0x004de205, 0x00000003
	.section .unidentified.084e084e,"a"
	.incbin "baserom.gba", 0x004e084e, 0x00000002
	.section .unidentified.084e2f32,"a"
	.incbin "baserom.gba", 0x004e2f32, 0x00000002
	.section .unidentified.084e3fa7,"a"
	.incbin "baserom.gba", 0x004e3fa7, 0x00000001
	.section .unidentified.084e628d,"a"
	.incbin "baserom.gba", 0x004e628d, 0x00000003
	.section .unidentified.084e8acf,"a"
	.incbin "baserom.gba", 0x004e8acf, 0x00000001
	.section .unidentified.084ea8e5,"a"
	.incbin "baserom.gba", 0x004ea8e5, 0x00000003
	.section .unidentified.084ecfc6,"a"
	.incbin "baserom.gba", 0x004ecfc6, 0x00000002
	.section .unidentified.084ee10d,"a"
	.incbin "baserom.gba", 0x004ee10d, 0x00000003
	.section .unidentified.084f5389,"a"
	.incbin "baserom.gba", 0x004f5389, 0x00000003
	.section .unidentified.084f6986,"a"
	.incbin "baserom.gba", 0x004f6986, 0x00000002
	.section .unidentified.084f7c4f,"a"
	.incbin "baserom.gba", 0x004f7c4f, 0x00000001
	.section .unidentified.084fb1f5,"a"
	.incbin "baserom.gba", 0x004fb1f5, 0x00000003
	.section .unidentified.084fcfa6,"a"
	.incbin "baserom.gba", 0x004fcfa6, 0x00000002
	.section .unidentified.08500b1d,"a"
	.incbin "baserom.gba", 0x00500b1d, 0x00000003
	.section .unidentified.0850202b,"a"
	.incbin "baserom.gba", 0x0050202b, 0x00000001
	.section .unidentified.08502cfd,"a"
	.incbin "baserom.gba", 0x00502cfd, 0x00000003
	.section .unidentified.085040e2,"a"
	.incbin "baserom.gba", 0x005040e2, 0x00000002
	.section .unidentified.08505265,"a"
	.incbin "baserom.gba", 0x00505265, 0x00000003
	.section .unidentified.08505bee,"a"
	.incbin "baserom.gba", 0x00505bee, 0x00000002
	.section .unidentified.0850894e,"a"
	.incbin "baserom.gba", 0x0050894e, 0x00000002
	.section .unidentified.08509b39,"a"
	.incbin "baserom.gba", 0x00509b39, 0x00000003
	.section .unidentified.0850b7c1,"a"
	.incbin "baserom.gba", 0x0050b7c1, 0x00000003
	.section .unidentified.0850bccf,"a"
	.incbin "baserom.gba", 0x0050bccf, 0x00000001
	.section .unidentified.0850be0f,"a"
	.incbin "baserom.gba", 0x0050be0f, 0x00000001
	.section .unidentified.0850d185,"a"
	.incbin "baserom.gba", 0x0050d185, 0x00000003
	.section .unidentified.085115ab,"a"
	.incbin "baserom.gba", 0x005115ab, 0x00000001
	.section .unidentified.085132da,"a"
	.incbin "baserom.gba", 0x005132da, 0x00000002
	.section .unidentified.08514187,"a"
	.incbin "baserom.gba", 0x00514187, 0x00000001
	.section .unidentified.08516607,"a"
	.incbin "baserom.gba", 0x00516607, 0x00000001
	.section .unidentified.08517383,"a"
	.incbin "baserom.gba", 0x00517383, 0x00000001
	.section .unidentified.08517e62,"a"
	.incbin "baserom.gba", 0x00517e62, 0x00000002
	.section .unidentified.0851adae,"a"
	.incbin "baserom.gba", 0x0051adae, 0x00000002
	.section .unidentified.0851b6f5,"a"
	.incbin "baserom.gba", 0x0051b6f5, 0x00000003
	.section .unidentified.0851c342,"a"
	.incbin "baserom.gba", 0x0051c342, 0x00000002
	.section .unidentified.0851c569,"a"
	.incbin "baserom.gba", 0x0051c569, 0x00000003
	.section .unidentified.0851dfbf,"a"
	.incbin "baserom.gba", 0x0051dfbf, 0x00000001
	.section .unidentified.0851f985,"a"
	.incbin "baserom.gba", 0x0051f985, 0x00000003
	.section .unidentified.0851facf,"a"
	.incbin "baserom.gba", 0x0051facf, 0x00000001
	.section .unidentified.08521cc3,"a"
	.incbin "baserom.gba", 0x00521cc3, 0x00000001
	.section .unidentified.0852354b,"a"
	.incbin "baserom.gba", 0x0052354b, 0x00000001
	.section .unidentified.085257ed,"a"
	.incbin "baserom.gba", 0x005257ed, 0x00000003
	.section .unidentified.0852592f,"a"
	.incbin "baserom.gba", 0x0052592f, 0x00000001
	.section .unidentified.08527a42,"a"
	.incbin "baserom.gba", 0x00527a42, 0x00000002
	.section .unidentified.08529a26,"a"
	.incbin "baserom.gba", 0x00529a26, 0x00000002
	.section .unidentified.0852b14b,"a"
	.incbin "baserom.gba", 0x0052b14b, 0x00000001
	.section .unidentified.0852c1ff,"a"
	.incbin "baserom.gba", 0x0052c1ff, 0x00000001
	.section .unidentified.0852e041,"a"
	.incbin "baserom.gba", 0x0052e041, 0x00000003
	.section .unidentified.0853010f,"a"
	.incbin "baserom.gba", 0x0053010f, 0x00000001
	.section .unidentified.08532c61,"a"
	.incbin "baserom.gba", 0x00532c61, 0x00000003
	.section .unidentified.08535326,"a"
	.incbin "baserom.gba", 0x00535326, 0x00000002
	.section .unidentified.08537a7e,"a"
	.incbin "baserom.gba", 0x00537a7e, 0x00000002
	.section .unidentified.085392d1,"a"
	.incbin "baserom.gba", 0x005392d1, 0x00000003
	.section .unidentified.0853d79d,"a"
	.incbin "baserom.gba", 0x0053d79d, 0x00000003
	.section .unidentified.0853fce7,"a"
	.incbin "baserom.gba", 0x0053fce7, 0x00000001
	.section .unidentified.08540fb2,"a"
	.incbin "baserom.gba", 0x00540fb2, 0x00000002
	.section .unidentified.08541c63,"a"
	.incbin "baserom.gba", 0x00541c63, 0x00000001
	.section .unidentified.085427c3,"a"
	.incbin "baserom.gba", 0x005427c3, 0x00000001
	.section .unidentified.08544222,"a"
	.incbin "baserom.gba", 0x00544222, 0x00000002
	.section .unidentified.08544fc6,"a"
	.incbin "baserom.gba", 0x00544fc6, 0x00000002
	.section .unidentified.085499ed,"a"
	.incbin "baserom.gba", 0x005499ed, 0x00000003
	.section .unidentified.0855129f,"a"
	.incbin "baserom.gba", 0x0055129f, 0x00000001
	.section .unidentified.08556b4a,"a"
	.incbin "baserom.gba", 0x00556b4a, 0x00000002
	.section .unidentified.08556c8b,"a"
	.incbin "baserom.gba", 0x00556c8b, 0x00000001
	.section .unidentified.0855adf6,"a"
	.incbin "baserom.gba", 0x0055adf6, 0x00000002
	.section .unidentified.0855cef2,"a"
	.incbin "baserom.gba", 0x0055cef2, 0x00000002
	.section .unidentified.08564a96,"a"
	.incbin "baserom.gba", 0x00564a96, 0x00000002
	.section .unidentified.08569331,"a"
	.incbin "baserom.gba", 0x00569331, 0x00000003
	.section .unidentified.0856b832,"a"
	.incbin "baserom.gba", 0x0056b832, 0x00000002
	.section .unidentified.085733be,"a"
	.incbin "baserom.gba", 0x005733be, 0x00000002
	.section .unidentified.08575f6d,"a"
	.incbin "baserom.gba", 0x00575f6d, 0x00000003
	.section .unidentified.08578041,"a"
	.incbin "baserom.gba", 0x00578041, 0x00000003
	.section .unidentified.0857a09a,"a"
	.incbin "baserom.gba", 0x0057a09a, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057a09c, 0x00001cec
	.section .unidentified.0858020d,"a"
	.incbin "baserom.gba", 0x0058020d, 0x00000003
	.section .unidentified.08581fdf,"a"
	.incbin "baserom.gba", 0x00581fdf, 0x00000001
	.section .unidentified.0858211f,"a"
	.incbin "baserom.gba", 0x0058211f, 0x00000001
	.section .unidentified.085880f6,"a"
	.incbin "baserom.gba", 0x005880f6, 0x00000002
	.section .unidentified.0858a9bd,"a"
	.incbin "baserom.gba", 0x0058a9bd, 0x00000003
	.section .unidentified.085931cb,"a"
	.incbin "baserom.gba", 0x005931cb, 0x00000001
	.section .unidentified.08593aca,"a"
	.incbin "baserom.gba", 0x00593aca, 0x00000002
	.section .unidentified.085949a3,"a"
	.incbin "baserom.gba", 0x005949a3, 0x00000001
	.section .unidentified.085965fb,"a"
	.incbin "baserom.gba", 0x005965fb, 0x00000001
	.section .unidentified.0859801b,"a"
	.incbin "baserom.gba", 0x0059801b, 0x00000001
	.section .unidentified.0859ab57,"a"
	.incbin "baserom.gba", 0x0059ab57, 0x00000001
	.section .unidentified.0859d2d1,"a"
	.incbin "baserom.gba", 0x0059d2d1, 0x00000003
	.section .unidentified.0859e2be,"a"
	.incbin "baserom.gba", 0x0059e2be, 0x00000002
	.section .unidentified.0859fb35,"a"
	.incbin "baserom.gba", 0x0059fb35, 0x00000003
	.section .unidentified.085a28c9,"a"
	.incbin "baserom.gba", 0x005a28c9, 0x00000003
	.section .unidentified.085a695e,"a"
	.incbin "baserom.gba", 0x005a695e, 0x00000002
	.section .unidentified.085a6a9f,"a"
	.incbin "baserom.gba", 0x005a6a9f, 0x00000001
	.section .unidentified.085a8b09,"a"
	.incbin "baserom.gba", 0x005a8b09, 0x00000003
	.section .unidentified.085aabab,"a"
	.incbin "baserom.gba", 0x005aabab, 0x00000001
	.section .unidentified.085abb38,"a"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005abb38, 0x000022d8
	.section .unidentified.085b0a86,"a"
	.incbin "baserom.gba", 0x005b0a86, 0x00000002
	.section .unidentified.085b81d3,"a"
	.incbin "baserom.gba", 0x005b81d3, 0x00000001
	.section .unidentified.085b86dd,"a"
	.incbin "baserom.gba", 0x005b86dd, 0x00000003
	.section .unidentified.085b9f59,"a"
	.incbin "baserom.gba", 0x005b9f59, 0x00000003
	.section .unidentified.085bb85e,"a"
	.incbin "baserom.gba", 0x005bb85e, 0x00000002
	.section .unidentified.085bd209,"a"
	.incbin "baserom.gba", 0x005bd209, 0x00000003
	.section .unidentified.085c1afd,"a"
	.incbin "baserom.gba", 0x005c1afd, 0x00000003
	.section .unidentified.085c2b42,"a"
	.incbin "baserom.gba", 0x005c2b42, 0x00000002
	.section .unidentified.085c3966,"a"
	.incbin "baserom.gba", 0x005c3966, 0x00000002
	.section .unidentified.085d764a,"a"
	.incbin "baserom.gba", 0x005d764a, 0x00000002
	.section .unidentified.085eda62,"a"
	.incbin "baserom.gba", 0x005eda62, 0x00000002
	.section .unidentified.08609520,"a"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609520, 0x00002904
	.section .unidentified.08691135,"a"
	.incbin "baserom.gba", 0x00691135, 0x00000003
	.section .unidentified.086d58f5,"a"
	.incbin "baserom.gba", 0x006d58f5, 0x00000003
	.section .unidentified.08717b8c,"a"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717b8c, 0x00001164
	.section .unidentified.087458f7,"a"
	.incbin "baserom.gba", 0x007458f7, 0x00000001
	.section .unidentified.0875bdd8,"a"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075bdd8, 0x00004604
	.section .unidentified.087fd4b9,"a"
	.incbin "baserom.gba", 0x007fd4b9, 0x00000003
	.section .unidentified.084baaa3,"a"
	.incbin "baserom.gba", 0x004baaa3, 0x00000001
	.section .unidentified.084c03cf,"a"
	.incbin "baserom.gba", 0x004c03cf, 0x00000001
	.section .unidentified.084cecf1,"a"
	.incbin "baserom.gba", 0x004cecf1, 0x00000003
	.section .unidentified.084db8ca,"a"
	.incbin "baserom.gba", 0x004db8ca, 0x00000002
	.section .unidentified.084e60fa,"a"
	.incbin "baserom.gba", 0x004e60fa, 0x00000002
	.section .unidentified.084f1221,"a"
	.incbin "baserom.gba", 0x004f1221, 0x00000003
	.section .unidentified.084fb067,"a"
	.incbin "baserom.gba", 0x004fb067, 0x00000001
	.section .unidentified.08502bff,"a"
	.incbin "baserom.gba", 0x00502bff, 0x00000001
	.section .unidentified.0850886b,"a"
	.incbin "baserom.gba", 0x0050886b, 0x00000001
	.section .unidentified.0850d02a,"a"
	.incbin "baserom.gba", 0x0050d02a, 0x00000002
	.section .unidentified.08514053,"a"
	.incbin "baserom.gba", 0x00514053, 0x00000001
	.section .unidentified.08518cda,"a"
	.incbin "baserom.gba", 0x00518cda, 0x00000002
	.section .unidentified.0851d292,"a"
	.incbin "baserom.gba", 0x0051d292, 0x00000002
	.section .unidentified.0851deab,"a"
	.incbin "baserom.gba", 0x0051deab, 0x00000001
	.section .unidentified.08521b9b,"a"
	.incbin "baserom.gba", 0x00521b9b, 0x00000001
	.section .unidentified.08527937,"a"
	.incbin "baserom.gba", 0x00527937, 0x00000001
	.section .unidentified.0852dee6,"a"
	.incbin "baserom.gba", 0x0052dee6, 0x00000002
	.section .unidentified.0852ff8a,"a"
	.incbin "baserom.gba", 0x0052ff8a, 0x00000002
	.section .unidentified.0853b9d1,"a"
	.incbin "baserom.gba", 0x0053b9d1, 0x00000003
	.section .unidentified.085426cd,"a"
	.incbin "baserom.gba", 0x005426cd, 0x00000003
	.section .unidentified.08546aff,"a"
	.incbin "baserom.gba", 0x00546aff, 0x00000001
	.section .unidentified.085511d3,"a"
	.incbin "baserom.gba", 0x005511d3, 0x00000001
	.section .unidentified.085580b9,"a"
	.incbin "baserom.gba", 0x005580b9, 0x00000003
	.section .unidentified.08560fce,"a"
	.incbin "baserom.gba", 0x00560fce, 0x00000002
	.section .unidentified.08561f13,"a"
	.incbin "baserom.gba", 0x00561f13, 0x00000001
	.section .unidentified.085648fa,"a"
	.incbin "baserom.gba", 0x005648fa, 0x00000002
	.section .unidentified.0856e772,"a"
	.incbin "baserom.gba", 0x0056e772, 0x00000002
	.section .unidentified.0857153f,"a"
	.incbin "baserom.gba", 0x0057153f, 0x00000001
	.section .unidentified.08573223,"a"
	.incbin "baserom.gba", 0x00573223, 0x00000001
	.section .unidentified.0857d531,"a"
	.incbin "baserom.gba", 0x0057d531, 0x00000003
	.section .unidentified.08583423,"a"
	.incbin "baserom.gba", 0x00583423, 0x00000001
	.section .unidentified.08585179,"a"
	.incbin "baserom.gba", 0x00585179, 0x00000003
	.section .unidentified.08591925,"a"
	.incbin "baserom.gba", 0x00591925, 0x00000003
	.section .unidentified.08594843,"a"
	.incbin "baserom.gba", 0x00594843, 0x00000001
	.section .unidentified.08597e97,"a"
	.incbin "baserom.gba", 0x00597e97, 0x00000001
	.section .unidentified.0859f98b,"a"
	.incbin "baserom.gba", 0x0059f98b, 0x00000001
	.section .unidentified.085a8967,"a"
	.incbin "baserom.gba", 0x005a8967, 0x00000001
	.section .unidentified.085b0901,"a"
	.incbin "baserom.gba", 0x005b0901, 0x00000003
	.section .unidentified.085b9db5,"a"
	.incbin "baserom.gba", 0x005b9db5, 0x00000003
	.section .unidentified.085bd08b,"a"
	.incbin "baserom.gba", 0x005bd08b, 0x00000001
