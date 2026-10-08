	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00007ac8, "ax"
	.incbin "baserom.gba", 0x00007ac8, 0x00000024
	.section .rom.00007b98, "ax"
	.incbin "baserom.gba", 0x00007b98, 0x00000004
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.000290a4, "ax"
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x000290a4, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x000294a0, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002d048, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002d2c8, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002fa60, 0x00000800
	.section .rom.000330b4, "ax"
	.incbin "baserom.gba", 0x000330b4, 0x0000001c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x000330d0, 0x00000030
	.global UiText_IndefiniteArticles
UiText_IndefiniteArticles:
	.incbin "baserom.gba", 0x00033100, 0x00000020
	.global UiText_ItemPossessives
UiText_ItemPossessives:
	.incbin "baserom.gba", 0x00033120, 0x00000008
	.section .rom.000a22c8, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x000a22c8, 0x00000638
	.section .rom.000a2b48, "ax"
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000a2b48, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x000a2f30, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000a3330, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x000a33c8, 0x00000218
	.section .rom.000a3626, "ax"
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x000a3626, 0x000001b2
	.section .rom.000e6172, "ax"
	.incbin "baserom.gba", 0x000e6172, 0x00000002
	.section .rom.000f169f, "ax"
	.global PuffArc_CellWidths
PuffArc_CellWidths:
	.incbin "baserom.gba", 0x000f169f, 0x00000006
	.global PuffArc_CellHeights
PuffArc_CellHeights:
	.incbin "baserom.gba", 0x000f16a5, 0x00000006
	.global PuffArc_CellBiasY
PuffArc_CellBiasY:
	.incbin "baserom.gba", 0x000f16ab, 0x00000007
	.global PuffArc_CellSourceOffsets
PuffArc_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f16b2, 0x0000000c
	.global BattleFx_GlintCellOffsets
BattleFx_GlintCellOffsets:
	.incbin "baserom.gba", 0x000f16be, 0x0000000c
	.global BattleFx_GlintCellWidths
BattleFx_GlintCellWidths:
	.incbin "baserom.gba", 0x000f16ca, 0x00000006
	.global BattleFx_GlintCellHeights
BattleFx_GlintCellHeights:
	.incbin "baserom.gba", 0x000f16d0, 0x00000006
	.global FallingShards_Counts
FallingShards_Counts:
	.incbin "baserom.gba", 0x000f16d6, 0x00000006
	.global FallingBolts_Counts
FallingBolts_Counts:
	.incbin "baserom.gba", 0x000f16dc, 0x0000000c
	.global FiveMode_Records
FiveMode_Records:
	.incbin "baserom.gba", 0x000f16e8, 0x00000014
	.global FiveMode_Tremble
FiveMode_Tremble:
	.incbin "baserom.gba", 0x000f16fc, 0x00000008
	.global TwelveMode_Records
TwelveMode_Records:
	.incbin "baserom.gba", 0x000f1704, 0x00000054
	.global TwelveMode_StrikeWidths
TwelveMode_StrikeWidths:
	.incbin "baserom.gba", 0x000f1758, 0x00000006
	.global TwelveMode_StrikeHeights
TwelveMode_StrikeHeights:
	.incbin "baserom.gba", 0x000f175e, 0x00000006
	.global TwelveMode_StrikeOffsets
TwelveMode_StrikeOffsets:
	.incbin "baserom.gba", 0x000f1764, 0x0000000c
	.global TwelveMode_StrikeReach
TwelveMode_StrikeReach:
	.incbin "baserom.gba", 0x000f1770, 0x00000006
	.global TwelveMode_ColumnStages
TwelveMode_ColumnStages:
	.incbin "baserom.gba", 0x000f1776, 0x00000005
	.global TwelveMode_GlintDrawFlags
TwelveMode_GlintDrawFlags:
	.incbin "baserom.gba", 0x000f177b, 0x00000004
	.global BladeRain_CellWidths
BladeRain_CellWidths:
	.incbin "baserom.gba", 0x000f177f, 0x00000004
	.global BladeRain_CellHeights
BladeRain_CellHeights:
	.incbin "baserom.gba", 0x000f1783, 0x00000005
	.global BladeRain_CellSourceOffsets
BladeRain_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f1788, 0x00000008
	.global BattlePieceWidth
BattlePieceWidth:
	.incbin "baserom.gba", 0x000f1790, 0x00000021
	.global BattlePieceHeight
BattlePieceHeight:
	.incbin "baserom.gba", 0x000f17b1, 0x00000021
	.global BattlePieceOffset
BattlePieceOffset:
	.incbin "baserom.gba", 0x000f17d2, 0x00000044
	.global BattlePieceStartX
BattlePieceStartX:
	.incbin "baserom.gba", 0x000f1816, 0x00000021
	.global BattlePieceStartY
BattlePieceStartY:
	.incbin "baserom.gba", 0x000f1837, 0x00000021
	.global CornerSparks_X
CornerSparks_X:
	.incbin "baserom.gba", 0x000f1858, 0x00000004
	.global CornerSparks_Y
CornerSparks_Y:
	.incbin "baserom.gba", 0x000f185c, 0x00000004
	.global CornerSparks_DrawFlags
CornerSparks_DrawFlags:
	.incbin "baserom.gba", 0x000f1860, 0x00000004
	.global TwoResource_CellWidths
TwoResource_CellWidths:
	.incbin "baserom.gba", 0x000f1864, 0x00000006
	.global TwoResource_CellHeights
TwoResource_CellHeights:
	.incbin "baserom.gba", 0x000f186a, 0x00000006
	.global TwoResource_CellSourceOffsets
TwoResource_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f1870, 0x0000000c
	.global TwoResource_CellX
TwoResource_CellX:
	.incbin "baserom.gba", 0x000f187c, 0x0000000c
	.global TwoResource_CellBiasY
TwoResource_CellBiasY:
	.incbin "baserom.gba", 0x000f1888, 0x00000008
	.global MemberBurst_Counts
MemberBurst_Counts:
	.incbin "baserom.gba", 0x000f1890, 0x00000006
	.global EarthWall_CellSourceOffsets
EarthWall_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f1896, 0x00000006
	.global EarthWall_CellWidths
EarthWall_CellWidths:
	.incbin "baserom.gba", 0x000f189c, 0x00000003
	.global EarthWall_CellHeights
EarthWall_CellHeights:
	.incbin "baserom.gba", 0x000f189f, 0x00000003
	.global MemberBeam_MotePictures
MemberBeam_MotePictures:
	.incbin "baserom.gba", 0x000f18a2, 0x00000008
	.global MemberBeam_HeadOffsets
MemberBeam_HeadOffsets:
	.incbin "baserom.gba", 0x000f18aa, 0x00000006
	.global MemberBeam_HeadWidths
MemberBeam_HeadWidths:
	.incbin "baserom.gba", 0x000f18b0, 0x00000003
	.global MemberBeam_HeadHeights
MemberBeam_HeadHeights:
	.incbin "baserom.gba", 0x000f18b3, 0x00000003
	.global SevenMode_Records
SevenMode_Records:
	.incbin "baserom.gba", 0x000f18b6, 0x0000000e
	.global SevenMode_ShardWidths
SevenMode_ShardWidths:
	.incbin "baserom.gba", 0x000f18c4, 0x00000012
	.global SevenMode_ShardHeights
SevenMode_ShardHeights:
	.incbin "baserom.gba", 0x000f18d6, 0x00000012
	.global SevenMode_ShardOffsets
SevenMode_ShardOffsets:
	.incbin "baserom.gba", 0x000f18e8, 0x00000024
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000f190c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000f191a, 0x0000000e
	.global SpinningTriangle_Vertex
SpinningTriangle_Vertex:
	.incbin "baserom.gba", 0x000f1928, 0x0000000c
	.global TriangleStrike_Vertex
TriangleStrike_Vertex:
	.incbin "baserom.gba", 0x000f1934, 0x0000000c
	.global RingBolts_Points
RingBolts_Points:
	.incbin "baserom.gba", 0x000f1940, 0x00000018
	.global SpinningStars_Radii
SpinningStars_Radii:
	.incbin "baserom.gba", 0x000f1958, 0x00000002
	.global Data_080ee15a
Data_080ee15a:
	.incbin "baserom.gba", 0x000f195a, 0x00000009
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000f1963, 0x00000009
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000f196c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000f1974, 0x00000003
	.global Data_080ee177
Data_080ee177:
	.incbin "baserom.gba", 0x000f1977, 0x00000003
	.global Data_080ee17a
Data_080ee17a:
	.incbin "baserom.gba", 0x000f197a, 0x00000004
	.global Data_080ee17e
Data_080ee17e:
	.incbin "baserom.gba", 0x000f197e, 0x00000006
	.global PaletteParticles_DropSpeeds
PaletteParticles_DropSpeeds:
	.incbin "baserom.gba", 0x000f1984, 0x00000004
	.global PaletteParticles_SparkPixels
PaletteParticles_SparkPixels:
	.incbin "baserom.gba", 0x000f1988, 0x00000002
	.global PaletteParticles_DropSizes
PaletteParticles_DropSizes:
	.incbin "baserom.gba", 0x000f198a, 0x00000004
	.global PaletteParticles_StreakWidths
PaletteParticles_StreakWidths:
	.incbin "baserom.gba", 0x000f198e, 0x00000006
	.global PaletteParticles_StreakHeights
PaletteParticles_StreakHeights:
	.incbin "baserom.gba", 0x000f1994, 0x00000006
	.global PaletteParticles_StreakDrops
PaletteParticles_StreakDrops:
	.incbin "baserom.gba", 0x000f199a, 0x00000006
	.global PaletteParticles_StreakOffsets
PaletteParticles_StreakOffsets:
	.incbin "baserom.gba", 0x000f19a0, 0x0000000c
	.global EmberColumns_Columns
EmberColumns_Columns:
	.incbin "baserom.gba", 0x000f19ac, 0x00000008
	.global EmberColumns_Gravity
EmberColumns_Gravity:
	.incbin "baserom.gba", 0x000f19b4, 0x00000010
	.global VortexMotes_Counts
VortexMotes_Counts:
	.incbin "baserom.gba", 0x000f19c4, 0x00000006
	.global Tornado_Shapes
Tornado_Shapes:
	.incbin "baserom.gba", 0x000f19ca, 0x00000009
	.global Crystal_ShardStarts
Crystal_ShardStarts:
	.incbin "baserom.gba", 0x000f19d3, 0x00000022
	.global Crystal_Counts
Crystal_Counts:
	.incbin "baserom.gba", 0x000f19f5, 0x00000006
	.global Crystal_ShardWidths
Crystal_ShardWidths:
	.incbin "baserom.gba", 0x000f19fb, 0x0000000c
	.global Crystal_ShardHeights
Crystal_ShardHeights:
	.incbin "baserom.gba", 0x000f1a07, 0x0000000d
	.global Crystal_ShardOffsets
Crystal_ShardOffsets:
	.incbin "baserom.gba", 0x000f1a14, 0x00000030
	.global LightningPillar_Sparks
LightningPillar_Sparks:
	.incbin "baserom.gba", 0x000f1a44, 0x0000000c
	.global LightningPillar_Columns
LightningPillar_Columns:
	.incbin "baserom.gba", 0x000f1a50, 0x0000000e
	.global LightningPillar_Counts
LightningPillar_Counts:
	.incbin "baserom.gba", 0x000f1a5e, 0x00000004
	.global SparkGroups_Shapes
SparkGroups_Shapes:
	.incbin "baserom.gba", 0x000f1a62, 0x00000032
	.global SparkGroups_FlashCells
SparkGroups_FlashCells:
	.incbin "baserom.gba", 0x000f1a94, 0x00000006
	.global FirePillars_Counts
FirePillars_Counts:
	.incbin "baserom.gba", 0x000f1a9a, 0x00000003
	.global FirePillars_Depths
FirePillars_Depths:
	.incbin "baserom.gba", 0x000f1a9d, 0x0000000c
	.global FirePillars_StartFrames
FirePillars_StartFrames:
	.incbin "baserom.gba", 0x000f1aa9, 0x00000005
	.global RenderMode_GlintFlips
RenderMode_GlintFlips:
	.incbin "baserom.gba", 0x000f1aae, 0x00000006
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000f1ab4, 0x0000065c
	.global ThornVines_ObjectPoses
ThornVines_ObjectPoses:
	.incbin "baserom.gba", 0x000f2110, 0x00000006
	.global ThornVines_ThornOffsets
ThornVines_ThornOffsets:
	.incbin "baserom.gba", 0x000f2116, 0x0000000a
	.global ThornVines_ThornWidths
ThornVines_ThornWidths:
	.incbin "baserom.gba", 0x000f2120, 0x00000005
	.global ThornVines_ThornHeights
ThornVines_ThornHeights:
	.incbin "baserom.gba", 0x000f2125, 0x00000005
	.global ThornVines_LeafOffsets
ThornVines_LeafOffsets:
	.incbin "baserom.gba", 0x000f212a, 0x00000006
	.global ThornVines_LeafSizes
ThornVines_LeafSizes:
	.incbin "baserom.gba", 0x000f2130, 0x00000004
	.global ThornVines_ChipOffsets
ThornVines_ChipOffsets:
	.incbin "baserom.gba", 0x000f2134, 0x0000000a
	.global ThornVines_ChipWidths
ThornVines_ChipWidths:
	.incbin "baserom.gba", 0x000f213e, 0x00000005
	.global ThornVines_ChipHeights
ThornVines_ChipHeights:
	.incbin "baserom.gba", 0x000f2143, 0x00000005
	.global ThornVines_SparkOffsets
ThornVines_SparkOffsets:
	.incbin "baserom.gba", 0x000f2148, 0x0000000a
	.global ThornVines_SparkSizes
ThornVines_SparkSizes:
	.incbin "baserom.gba", 0x000f2152, 0x00000006
	.global ThornVines_EmberOffsets
ThornVines_EmberOffsets:
	.incbin "baserom.gba", 0x000f2158, 0x0000000e
	.global ThornVines_EmberSizes
ThornVines_EmberSizes:
	.incbin "baserom.gba", 0x000f2166, 0x0000000e
	.global BattleFx10_Points
BattleFx10_Points:
	.incbin "baserom.gba", 0x000f2174, 0x00000020
	.global BattleFx10_ShakeOffsets
BattleFx10_ShakeOffsets:
	.incbin "baserom.gba", 0x000f2194, 0x00000004
	.global BattleFx10_RockCells
BattleFx10_RockCells:
	.incbin "baserom.gba", 0x000f2198, 0x00000006
	.global BattleFx10_RockWidths
BattleFx10_RockWidths:
	.incbin "baserom.gba", 0x000f219e, 0x00000003
	.global BattleFx10_RockHeights
BattleFx10_RockHeights:
	.incbin "baserom.gba", 0x000f21a1, 0x00000003
	.global BattleFx10_Animations
BattleFx10_Animations:
	.incbin "baserom.gba", 0x000f21a4, 0x00000004
	.global BattleFx10_DebrisWidths
BattleFx10_DebrisWidths:
	.incbin "baserom.gba", 0x000f21a8, 0x0000000b
	.global BattleFx10_DebrisHeights
BattleFx10_DebrisHeights:
	.incbin "baserom.gba", 0x000f21b3, 0x0000000b
	.global BattleFx10_DebrisCells
BattleFx10_DebrisCells:
	.incbin "baserom.gba", 0x000f21be, 0x00000016
	.global BattleFx10_SprayWidths
BattleFx10_SprayWidths:
	.incbin "baserom.gba", 0x000f21d4, 0x00000003
	.global BattleFx10_SprayHeights
BattleFx10_SprayHeights:
	.incbin "baserom.gba", 0x000f21d7, 0x00000003
	.global BattleFx10_SprayCells
BattleFx10_SprayCells:
	.incbin "baserom.gba", 0x000f21da, 0x00000006
	.global BattleFx10_BoulderCells
BattleFx10_BoulderCells:
	.incbin "baserom.gba", 0x000f21e0, 0x00000006
	.global BattleFx10_BoulderWidths
BattleFx10_BoulderWidths:
	.incbin "baserom.gba", 0x000f21e6, 0x00000003
	.global BattleFx10_BoulderHeights
BattleFx10_BoulderHeights:
	.incbin "baserom.gba", 0x000f21e9, 0x00000003
	.global BattleFx10_FallWidths
BattleFx10_FallWidths:
	.incbin "baserom.gba", 0x000f21ec, 0x00000003
	.global BattleFx10_FallHeights
BattleFx10_FallHeights:
	.incbin "baserom.gba", 0x000f21ef, 0x00000003
	.global BattleFx10_FallCells
BattleFx10_FallCells:
	.incbin "baserom.gba", 0x000f21f2, 0x00000006
	.global IceShardBursts_Gravities
IceShardBursts_Gravities:
	.incbin "baserom.gba", 0x000f21f8, 0x00000010
	.global PaletteRamp_ShardCells
PaletteRamp_ShardCells:
	.incbin "baserom.gba", 0x000f2208, 0x00000018
	.global PaletteRamp_ShardWidths
PaletteRamp_ShardWidths:
	.incbin "baserom.gba", 0x000f2220, 0x0000000c
	.global PaletteRamp_ShardHeights
PaletteRamp_ShardHeights:
	.incbin "baserom.gba", 0x000f222c, 0x0000000c
	.global RockWall_Timings
RockWall_Timings:
	.incbin "baserom.gba", 0x000f2238, 0x00000009
	.global HomingEmbers_Counts
HomingEmbers_Counts:
	.incbin "baserom.gba", 0x000f2241, 0x00000003
	.global HomingEmbers_FlareWidths
HomingEmbers_FlareWidths:
	.incbin "baserom.gba", 0x000f2244, 0x00000006
	.global HomingEmbers_FlareHeights
HomingEmbers_FlareHeights:
	.incbin "baserom.gba", 0x000f224a, 0x00000006
	.global HomingEmbers_FlareBiasY
HomingEmbers_FlareBiasY:
	.incbin "baserom.gba", 0x000f2250, 0x00000006
	.global HomingEmbers_FlareCells
HomingEmbers_FlareCells:
	.incbin "baserom.gba", 0x000f2256, 0x0000000c
	.global RisingMotes_ColumnSpots
RisingMotes_ColumnSpots:
	.incbin "baserom.gba", 0x000f2262, 0x00000026
	.global RisingMotes_Timings
RisingMotes_Timings:
	.incbin "baserom.gba", 0x000f2288, 0x00000009
	.global RisingMotes_MoteWidths
RisingMotes_MoteWidths:
	.incbin "baserom.gba", 0x000f2291, 0x00000008
	.global RisingMotes_MoteHeights
RisingMotes_MoteHeights:
	.incbin "baserom.gba", 0x000f2299, 0x00000009
	.global RisingMotes_MoteCells
RisingMotes_MoteCells:
	.incbin "baserom.gba", 0x000f22a2, 0x00000010
	.global RisingMotes_ColumnCells
RisingMotes_ColumnCells:
	.incbin "baserom.gba", 0x000f22b2, 0x00000006
	.global RisingMotes_ColumnHeights
RisingMotes_ColumnHeights:
	.incbin "baserom.gba", 0x000f22b8, 0x00000003
	.global RisingMotes_EmberWidths
RisingMotes_EmberWidths:
	.incbin "baserom.gba", 0x000f22bb, 0x00000008
	.global RisingMotes_EmberHeights
RisingMotes_EmberHeights:
	.incbin "baserom.gba", 0x000f22c3, 0x00000009
	.global RisingMotes_EmberCells
RisingMotes_EmberCells:
	.incbin "baserom.gba", 0x000f22cc, 0x00000010
	.global LightningBolts_Sparks
LightningBolts_Sparks:
	.incbin "baserom.gba", 0x000f22dc, 0x00000006
	.global ParticleField_Counts
ParticleField_Counts:
	.incbin "baserom.gba", 0x000f22e2, 0x0000000a
	.global ParticleField_PuffCells
ParticleField_PuffCells:
	.incbin "baserom.gba", 0x000f22ec, 0x0000000e
	.global ParticleField_PuffSizes
ParticleField_PuffSizes:
	.incbin "baserom.gba", 0x000f22fa, 0x00000046
	.global StagedParticles_UnitScale
StagedParticles_UnitScale:
	.incbin "baserom.gba", 0x000f2340, 0x00000008
	.global DualTable_VineWidths
DualTable_VineWidths:
	.incbin "baserom.gba", 0x000f2348, 0x00000003
	.global DualTable_HeightLimits
DualTable_HeightLimits:
	.incbin "baserom.gba", 0x000f234b, 0x00000003
	.global DualTable_VineCells
DualTable_VineCells:
	.incbin "baserom.gba", 0x000f234e, 0x00000006
	.global DualTable_PillarWidths
DualTable_PillarWidths:
	.incbin "baserom.gba", 0x000f2354, 0x00000004
	.global DualTable_PillarCells
DualTable_PillarCells:
	.incbin "baserom.gba", 0x000f2358, 0x00000006
	.global DualTable_ColumnCounts
DualTable_ColumnCounts:
	.incbin "baserom.gba", 0x000f235e, 0x00000003
	.global DualTable_ColumnX
DualTable_ColumnX:
	.incbin "baserom.gba", 0x000f2361, 0x00000010
	.global DualTable_PillarHeights
DualTable_PillarHeights:
	.incbin "baserom.gba", 0x000f2371, 0x00000008
	.global DualTable_PuffWidths
DualTable_PuffWidths:
	.incbin "baserom.gba", 0x000f2379, 0x00000007
	.global DualTable_PuffHeights
DualTable_PuffHeights:
	.incbin "baserom.gba", 0x000f2380, 0x00000008
	.global DualTable_PuffCells
DualTable_PuffCells:
	.incbin "baserom.gba", 0x000f2388, 0x0000000e
	.global RisingColumns_ColumnOffsets
RisingColumns_ColumnOffsets:
	.incbin "baserom.gba", 0x000f2396, 0x00000010
	.global BattleFxPillar_Kinds
BattleFxPillar_Kinds:
	.incbin "baserom.gba", 0x000f23a6, 0x00000008
	.global BattleFxPillar_X
BattleFxPillar_X:
	.incbin "baserom.gba", 0x000f23ae, 0x00000008
	.global BattleFxPillar_Counts
BattleFxPillar_Counts:
	.incbin "baserom.gba", 0x000f23b6, 0x00000003
	.global BattleFxPillar_PuffWidths
BattleFxPillar_PuffWidths:
	.incbin "baserom.gba", 0x000f23b9, 0x00000007
	.global BattleFxPillar_PuffHeights
BattleFxPillar_PuffHeights:
	.incbin "baserom.gba", 0x000f23c0, 0x00000008
	.global BattleFxPillar_PuffCells
BattleFxPillar_PuffCells:
	.incbin "baserom.gba", 0x000f23c8, 0x0000000e
	.global LightningBolts_Counts
LightningBolts_Counts:
	.incbin "baserom.gba", 0x000f23d6, 0x0000000c
	.global LightningBolts_GlintPalettes
LightningBolts_GlintPalettes:
	.incbin "baserom.gba", 0x000f23e2, 0x00000004
	.global LightningBolts_GlintModes
LightningBolts_GlintModes:
	.incbin "baserom.gba", 0x000f23e6, 0x00000003
	.global Volley_CometHeights
Volley_CometHeights:
	.incbin "baserom.gba", 0x000f23e9, 0x00000003
	.global Volley_Modes
Volley_Modes:
	.incbin "baserom.gba", 0x000f23ec, 0x0000003c
	.global Volley_FlashWidths
Volley_FlashWidths:
	.incbin "baserom.gba", 0x000f2428, 0x00000007
	.global Volley_FlashHeights
Volley_FlashHeights:
	.incbin "baserom.gba", 0x000f242f, 0x00000007
	.global Volley_FlashSteps
Volley_FlashSteps:
	.incbin "baserom.gba", 0x000f2436, 0x00000007
	.global Volley_FlashRises
Volley_FlashRises:
	.incbin "baserom.gba", 0x000f243d, 0x00000007
	.global Volley_FlashOffsets
Volley_FlashOffsets:
	.incbin "baserom.gba", 0x000f2444, 0x0000000e
	.global Volley_PelletOffsets
Volley_PelletOffsets:
	.incbin "baserom.gba", 0x000f2452, 0x00000008
	.global MercuryDjinnFlames_LaunchFrames
MercuryDjinnFlames_LaunchFrames:
	.incbin "baserom.gba", 0x000f245a, 0x00000005
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000f245f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000f2463, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f2468, 0x00000008
	.global RockToss_ChipFlips
RockToss_ChipFlips:
	.incbin "baserom.gba", 0x000f2470, 0x00000004
	.global RockToss_ChipWidths
RockToss_ChipWidths:
	.incbin "baserom.gba", 0x000f2474, 0x00000009
	.global RockToss_ChipHeights
RockToss_ChipHeights:
	.incbin "baserom.gba", 0x000f247d, 0x00000009
	.global RockToss_ChipCells
RockToss_ChipCells:
	.incbin "baserom.gba", 0x000f2486, 0x00000012
	.global RockToss_ChipX
RockToss_ChipX:
	.incbin "baserom.gba", 0x000f2498, 0x00000009
	.global RockToss_ChipY
RockToss_ChipY:
	.incbin "baserom.gba", 0x000f24a1, 0x00000009
	.global Mode12_Animations
Mode12_Animations:
	.incbin "baserom.gba", 0x000f24aa, 0x00000004
	.global Mode12_SmokeFlips
Mode12_SmokeFlips:
	.incbin "baserom.gba", 0x000f24ae, 0x00000004
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000f24b2, 0x00000040
	.global ShatterRocks_RockX
ShatterRocks_RockX:
	.incbin "baserom.gba", 0x000f24f2, 0x00000005
	.global ShatterRocks_DropFrames
ShatterRocks_DropFrames:
	.incbin "baserom.gba", 0x000f24f7, 0x00000005
	.global ShatterRocks_RockCounts
ShatterRocks_RockCounts:
	.incbin "baserom.gba", 0x000f24fc, 0x00000003
	.global ShatterRocks_ShardWidths
ShatterRocks_ShardWidths:
	.incbin "baserom.gba", 0x000f24ff, 0x0000000f
	.global ShatterRocks_ShardHeights
ShatterRocks_ShardHeights:
	.incbin "baserom.gba", 0x000f250e, 0x00000010
	.global ShatterRocks_ShardCells
ShatterRocks_ShardCells:
	.incbin "baserom.gba", 0x000f251e, 0x00000020
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000f253e, 0x00000040
	.global RisingWall_RowSheetOffsets
RisingWall_RowSheetOffsets:
	.incbin "baserom.gba", 0x000f257e, 0x00000012
	.global RisingWall_RowWidths
RisingWall_RowWidths:
	.incbin "baserom.gba", 0x000f2590, 0x0000000a
	.global RisingWall_MoundSheetOffsets
RisingWall_MoundSheetOffsets:
	.incbin "baserom.gba", 0x000f259a, 0x00000006
	.global RisingWall_MoundWidths
RisingWall_MoundWidths:
	.incbin "baserom.gba", 0x000f25a0, 0x00000003
	.global RisingWall_MoundHeights
RisingWall_MoundHeights:
	.incbin "baserom.gba", 0x000f25a3, 0x00000003
	.global RisingWall_Bg2Shifts
RisingWall_Bg2Shifts:
	.incbin "baserom.gba", 0x000f25a6, 0x00000006
	.global RisingWall_LateRockColumns
RisingWall_LateRockColumns:
	.incbin "baserom.gba", 0x000f25ac, 0x00000006
	.global BeamSequence_CellWidths
BeamSequence_CellWidths:
	.incbin "baserom.gba", 0x000f25b2, 0x00000006
	.global BeamSequence_CellHeights
BeamSequence_CellHeights:
	.incbin "baserom.gba", 0x000f25b8, 0x00000006
	.global BeamSequence_CellSheetOffsets
BeamSequence_CellSheetOffsets:
	.incbin "baserom.gba", 0x000f25be, 0x0000000c
	.global BeamSequence_CellReaches
BeamSequence_CellReaches:
	.incbin "baserom.gba", 0x000f25ca, 0x00000006
	.global CastingImpact_GlintDrawFlags
CastingImpact_GlintDrawFlags:
	.incbin "baserom.gba", 0x000f25d0, 0x00000004
	.global CastingImpact_ImageX
CastingImpact_ImageX:
	.incbin "baserom.gba", 0x000f25d4, 0x0000000e
	.global CastingImpact_ImageY
CastingImpact_ImageY:
	.incbin "baserom.gba", 0x000f25e2, 0x00000008
	.global CastingImpact_OrbitCells
CastingImpact_OrbitCells:
	.incbin "baserom.gba", 0x000f25ea, 0x0000000a
	.global FireSwirl_CellWidths
FireSwirl_CellWidths:
	.incbin "baserom.gba", 0x000f25f4, 0x00000007
	.global FireSwirl_CellHeights
FireSwirl_CellHeights:
	.incbin "baserom.gba", 0x000f25fb, 0x00000007
	.global FireSwirl_CellSourceOffsets
FireSwirl_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f2602, 0x0000000e
	.global FireSwirl_CellBiasX
FireSwirl_CellBiasX:
	.incbin "baserom.gba", 0x000f2610, 0x00000007
	.global FireSwirl_CellBiasY
FireSwirl_CellBiasY:
	.incbin "baserom.gba", 0x000f2617, 0x00000007
	.global Formation_TwelveX
Formation_TwelveX:
	.incbin "baserom.gba", 0x000f261e, 0x0000000c
	.global Formation_TwelveZ
Formation_TwelveZ:
	.incbin "baserom.gba", 0x000f262a, 0x0000000c
	.global Formation_RightEightX
Formation_RightEightX:
	.incbin "baserom.gba", 0x000f2636, 0x00000008
	.global Formation_RightEightZ
Formation_RightEightZ:
	.incbin "baserom.gba", 0x000f263e, 0x00000008
	.global Formation_SmallEightX
Formation_SmallEightX:
	.incbin "baserom.gba", 0x000f2646, 0x00000008
	.global Formation_SmallEightZ
Formation_SmallEightZ:
	.incbin "baserom.gba", 0x000f264e, 0x00000008
	.global ImpactBurst_CellWidths
ImpactBurst_CellWidths:
	.incbin "baserom.gba", 0x000f2656, 0x00000008
	.global ImpactBurst_CellHeights
ImpactBurst_CellHeights:
	.incbin "baserom.gba", 0x000f265e, 0x00000008
	.global ImpactBurst_CellSourceOffsets
ImpactBurst_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f2666, 0x00000010
	.global ParticleStreams_OrbitPoints
ParticleStreams_OrbitPoints:
	.incbin "baserom.gba", 0x000f2676, 0x0000002a
	.global ParticleStreams_DropPoints
ParticleStreams_DropPoints:
	.incbin "baserom.gba", 0x000f26a0, 0x0000001c
	.global ParticleStreams_FlashSheetOffsets
ParticleStreams_FlashSheetOffsets:
	.incbin "baserom.gba", 0x000f26bc, 0x0000000e
	.global ParticleStreams_FlashSizes
ParticleStreams_FlashSizes:
	.incbin "baserom.gba", 0x000f26ca, 0x0000000e
	.global CirclingScene_ObjectColumns
CirclingScene_ObjectColumns:
	.incbin "baserom.gba", 0x000f26d8, 0x00000009
	.global CirclingScene_ObjectRows
CirclingScene_ObjectRows:
	.incbin "baserom.gba", 0x000f26e1, 0x00000009
	.global CirclingScene_PuffSheetOffsets
CirclingScene_PuffSheetOffsets:
	.incbin "baserom.gba", 0x000f26ea, 0x0000000e
	.global CirclingScene_PuffSizes
CirclingScene_PuffSizes:
	.incbin "baserom.gba", 0x000f26f8, 0x0000000e
	.global FlameBlade_StrikeColumns
FlameBlade_StrikeColumns:
	.incbin "baserom.gba", 0x000f2706, 0x00000006
	.global FlameBlade_FlashCells
FlameBlade_FlashCells:
	.incbin "baserom.gba", 0x000f270c, 0x00000006
	.global FallingSword_FlashCells
FallingSword_FlashCells:
	.incbin "baserom.gba", 0x000f2712, 0x00000006
	.global FallingSword_DustGravity
FallingSword_DustGravity:
	.incbin "baserom.gba", 0x000f2718, 0x00000010
	.global ShockwaveScene_BurstHeights
ShockwaveScene_BurstHeights:
	.incbin "baserom.gba", 0x000f2728, 0x00000008
	.global ShockwaveScene_BurstCells
ShockwaveScene_BurstCells:
	.incbin "baserom.gba", 0x000f2730, 0x0000000e
	.global ShockwaveScene_ShardCells
ShockwaveScene_ShardCells:
	.incbin "baserom.gba", 0x000f273e, 0x0000000c
	.global ShockwaveScene_ShardWidths
ShockwaveScene_ShardWidths:
	.incbin "baserom.gba", 0x000f274a, 0x00000006
	.global ShockwaveScene_ShardHeights
ShockwaveScene_ShardHeights:
	.incbin "baserom.gba", 0x000f2750, 0x00000006
	.global ObjectRow_Columns
ObjectRow_Columns:
	.incbin "baserom.gba", 0x000f2756, 0x00000009
	.global ObjectRow_Rows
ObjectRow_Rows:
	.incbin "baserom.gba", 0x000f275f, 0x00000009
	.global BattleFx6_ObjectX
BattleFx6_ObjectX:
	.incbin "baserom.gba", 0x000f2768, 0x00000008
	.global BattleFx6_ObjectY
BattleFx6_ObjectY:
	.incbin "baserom.gba", 0x000f2770, 0x00000008
	.global BattleFx6_Gravity
BattleFx6_Gravity:
	.incbin "baserom.gba", 0x000f2778, 0x00000010
	.global RisingBurst_SparkCells
RisingBurst_SparkCells:
	.incbin "baserom.gba", 0x000f2788, 0x0000000e
	.global RisingBurst_SparkSizes
RisingBurst_SparkSizes:
	.incbin "baserom.gba", 0x000f2796, 0x0000000e
	.section .rom.000f2814, "ax"
	.incbin "baserom.gba", 0x000f2814, 0x00000020
	.global SentouKouka_BitOperands
SentouKouka_BitOperands:
	.incbin "baserom.gba", 0x000f2834, 0x00000020
	.section .rom.000f3a5c, "ax"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f3a5c, 0x000007d9
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f4235, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f4298, 0x00000544
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000f47dc, 0x000002f3
	.section .rom.000f68bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f68bc, 0x000000ad
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f6969, 0x00000006
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f696f, 0x0000003d
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000f69ac, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000f69ec, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000f6a2c, 0x00000040
	.section .rom.000f83dc, "ax"
	.incbin "baserom.gba", 0x000f83dc, 0x00000020
	.global Data_080f53fc
Data_080f53fc:
	.incbin "baserom.gba", 0x000f83fc, 0x00000004
	.global Data_080f5400
Data_080f5400:
	.incbin "baserom.gba", 0x000f8400, 0x00000008
	.global Data_080f5408
Data_080f5408:
	.incbin "baserom.gba", 0x000f8408, 0x00000012
	.global Data_080f541a
Data_080f541a:
	.incbin "baserom.gba", 0x000f841a, 0x0000000c
	.section .rom.000fb744, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000fb744, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000fb758, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000fb75e, 0x00000009
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000fb767, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000fb775, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000fb783, 0x00000009
	.section .rom.000fdfa0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000fdfa0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000fe030, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000fe0e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000fe114, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000fe12c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000fe1b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000fe1c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000fe204, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000fe214, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000fe248, 0x00000030
	.section .rom.000fed04, "ax"
	.incbin "baserom.gba", 0x000fed04, 0x00000090
	.section .rom.000fee24, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fee24, 0x00000060
	.section .rom.003227a0, "ax"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x003227a0, 0x00000010
	.section .rom.003261e7, "ax"
	.incbin "baserom.gba", 0x003261e7, 0x00000001
	.section .rom.0032c89b, "ax"
	.incbin "baserom.gba", 0x0032c89b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032c89c, 0x000086f8
	.section .rom.00336fe5, "ax"
	.incbin "baserom.gba", 0x00336fe5, 0x00000003
	.section .rom.003388f5, "ax"
	.incbin "baserom.gba", 0x003388f5, 0x00000003
	.section .rom.0033c3f9, "ax"
	.incbin "baserom.gba", 0x0033c3f9, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033c3fc, 0x0000071c
	.section .rom.00340f5a, "ax"
	.incbin "baserom.gba", 0x00340f5a, 0x00000002
	.section .rom.003516ca, "ax"
	.incbin "baserom.gba", 0x003516ca, 0x00000002
	.section .rom.00354cba, "ax"
	.incbin "baserom.gba", 0x00354cba, 0x00000002
	.section .rom.0036075e, "ax"
	.incbin "baserom.gba", 0x0036075e, 0x00000002
	.section .rom.00370e0e, "ax"
	.incbin "baserom.gba", 0x00370e0e, 0x00000002
	.section .rom.003788ae, "ax"
	.incbin "baserom.gba", 0x003788ae, 0x00000002
	.section .rom.0037c0ee, "ax"
	.incbin "baserom.gba", 0x0037c0ee, 0x00000002
	.section .rom.00385522, "ax"
	.incbin "baserom.gba", 0x00385522, 0x00000002
	.section .rom.0039094a, "ax"
	.incbin "baserom.gba", 0x0039094a, 0x00000002
	.section .rom.00398806, "ax"
	.incbin "baserom.gba", 0x00398806, 0x00000002
	.section .rom.0039d296, "ax"
	.incbin "baserom.gba", 0x0039d296, 0x00000002
	.section .rom.003a154a, "ax"
	.incbin "baserom.gba", 0x003a154a, 0x00000002
	.section .rom.003a4eca, "ax"
	.incbin "baserom.gba", 0x003a4eca, 0x00000002
	.section .rom.003b1706, "ax"
	.incbin "baserom.gba", 0x003b1706, 0x00000002
	.section .rom.003c004e, "ax"
	.incbin "baserom.gba", 0x003c004e, 0x00000002
	.section .rom.003c5951, "ax"
	.incbin "baserom.gba", 0x003c5951, 0x00000003
	.section .rom.003c72d3, "ax"
	.incbin "baserom.gba", 0x003c72d3, 0x00000001
	.section .rom.003ca0f1, "ax"
	.incbin "baserom.gba", 0x003ca0f1, 0x00000003
	.section .rom.003cd06e, "ax"
	.incbin "baserom.gba", 0x003cd06e, 0x00000002
	.section .rom.003cd8b5, "ax"
	.incbin "baserom.gba", 0x003cd8b5, 0x00000003
	.section .rom.003cf0cb, "ax"
	.incbin "baserom.gba", 0x003cf0cb, 0x00000001
	.section .rom.003cf6b6, "ax"
	.incbin "baserom.gba", 0x003cf6b6, 0x00000002
	.section .rom.003cfd7a, "ax"
	.incbin "baserom.gba", 0x003cfd7a, 0x00000002
	.section .rom.003d0061, "ax"
	.incbin "baserom.gba", 0x003d0061, 0x00000003
	.section .rom.003d13c7, "ax"
	.incbin "baserom.gba", 0x003d13c7, 0x00000001
	.section .rom.003d17b1, "ax"
	.incbin "baserom.gba", 0x003d17b1, 0x00000003
	.section .rom.003d1b83, "ax"
	.incbin "baserom.gba", 0x003d1b83, 0x00000001
	.section .rom.003d2423, "ax"
	.incbin "baserom.gba", 0x003d2423, 0x00000001
	.section .rom.003d28c6, "ax"
	.incbin "baserom.gba", 0x003d28c6, 0x00000002
	.section .rom.003d3a7f, "ax"
	.incbin "baserom.gba", 0x003d3a7f, 0x00000001
	.section .rom.003d4f42, "ax"
	.incbin "baserom.gba", 0x003d4f42, 0x00000002
	.section .rom.003d5f0b, "ax"
	.incbin "baserom.gba", 0x003d5f0b, 0x00000001
	.section .rom.003d6166, "ax"
	.incbin "baserom.gba", 0x003d6166, 0x00000002
	.section .rom.003d7bc1, "ax"
	.incbin "baserom.gba", 0x003d7bc1, 0x00000003
	.section .rom.003d810d, "ax"
	.incbin "baserom.gba", 0x003d810d, 0x00000003
	.section .rom.003d9e96, "ax"
	.incbin "baserom.gba", 0x003d9e96, 0x00000002
	.section .rom.003da10f, "ax"
	.incbin "baserom.gba", 0x003da10f, 0x00000001
	.section .rom.003da5d6, "ax"
	.incbin "baserom.gba", 0x003da5d6, 0x00000002
	.section .rom.003dc1ab, "ax"
	.incbin "baserom.gba", 0x003dc1ab, 0x00000001
	.section .rom.003ddda9, "ax"
	.incbin "baserom.gba", 0x003ddda9, 0x00000003
	.section .rom.003ddfca, "ax"
	.incbin "baserom.gba", 0x003ddfca, 0x00000002
	.section .rom.003de407, "ax"
	.incbin "baserom.gba", 0x003de407, 0x00000001
	.section .rom.003de519, "ax"
	.incbin "baserom.gba", 0x003de519, 0x00000003
	.section .rom.003dec97, "ax"
	.incbin "baserom.gba", 0x003dec97, 0x00000001
	.section .rom.003df135, "ax"
	.incbin "baserom.gba", 0x003df135, 0x00000003
	.section .rom.003dfe4a, "ax"
	.incbin "baserom.gba", 0x003dfe4a, 0x00000002
	.section .rom.003e0bca, "ax"
	.incbin "baserom.gba", 0x003e0bca, 0x00000002
	.section .rom.003e0f5b, "ax"
	.incbin "baserom.gba", 0x003e0f5b, 0x00000001
	.section .rom.003e2ca2, "ax"
	.incbin "baserom.gba", 0x003e2ca2, 0x00000002
	.section .rom.003e3a79, "ax"
	.incbin "baserom.gba", 0x003e3a79, 0x00000003
	.section .rom.003e42a6, "ax"
	.incbin "baserom.gba", 0x003e42a6, 0x00000002
	.section .rom.003e4931, "ax"
	.incbin "baserom.gba", 0x003e4931, 0x00000003
	.section .rom.003e4aef, "ax"
	.incbin "baserom.gba", 0x003e4aef, 0x00000001
	.section .rom.003e5847, "ax"
	.incbin "baserom.gba", 0x003e5847, 0x00000001
	.section .rom.003e5d93, "ax"
	.incbin "baserom.gba", 0x003e5d93, 0x00000001
	.section .rom.003e616d, "ax"
	.incbin "baserom.gba", 0x003e616d, 0x00000003
	.section .rom.003e651b, "ax"
	.incbin "baserom.gba", 0x003e651b, 0x00000001
	.section .rom.003e84bf, "ax"
	.incbin "baserom.gba", 0x003e84bf, 0x00000001
	.section .rom.003e925b, "ax"
	.incbin "baserom.gba", 0x003e925b, 0x00000001
	.section .rom.003e9477, "ax"
	.incbin "baserom.gba", 0x003e9477, 0x00000001
	.section .rom.003e9773, "ax"
	.incbin "baserom.gba", 0x003e9773, 0x00000001
	.section .rom.003eb921, "ax"
	.incbin "baserom.gba", 0x003eb921, 0x00000003
	.section .rom.003ec6ef, "ax"
	.incbin "baserom.gba", 0x003ec6ef, 0x00000001
	.section .rom.003ed851, "ax"
	.incbin "baserom.gba", 0x003ed851, 0x00000003
	.section .rom.003eddab, "ax"
	.incbin "baserom.gba", 0x003eddab, 0x00000001
	.section .rom.003ef695, "ax"
	.incbin "baserom.gba", 0x003ef695, 0x00000003
	.section .rom.003eff36, "ax"
	.incbin "baserom.gba", 0x003eff36, 0x00000002
	.section .rom.003f0313, "ax"
	.incbin "baserom.gba", 0x003f0313, 0x00000001
	.section .rom.003f059d, "ax"
	.incbin "baserom.gba", 0x003f059d, 0x00000003
	.section .rom.003f0943, "ax"
	.incbin "baserom.gba", 0x003f0943, 0x00000001
	.section .rom.003f0b9e, "ax"
	.incbin "baserom.gba", 0x003f0b9e, 0x00000002
	.section .rom.003f0f56, "ax"
	.incbin "baserom.gba", 0x003f0f56, 0x00000002
	.section .rom.003f243f, "ax"
	.incbin "baserom.gba", 0x003f243f, 0x00000001
	.section .rom.003f3a02, "ax"
	.incbin "baserom.gba", 0x003f3a02, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f3a04, 0x00000a6c
	.section .rom.003f5246, "ax"
	.incbin "baserom.gba", 0x003f5246, 0x00000002
	.section .rom.003f55c9, "ax"
	.incbin "baserom.gba", 0x003f55c9, 0x00000003
	.section .rom.003f6279, "ax"
	.incbin "baserom.gba", 0x003f6279, 0x00000003
	.section .rom.003f6dc1, "ax"
	.incbin "baserom.gba", 0x003f6dc1, 0x00000003
	.section .rom.003f6f5a, "ax"
	.incbin "baserom.gba", 0x003f6f5a, 0x00000002
	.section .rom.003f77e7, "ax"
	.incbin "baserom.gba", 0x003f77e7, 0x00000001
	.section .rom.003f82ae, "ax"
	.incbin "baserom.gba", 0x003f82ae, 0x00000002
	.section .rom.003f87c6, "ax"
	.incbin "baserom.gba", 0x003f87c6, 0x00000002
	.section .rom.003f8dea, "ax"
	.incbin "baserom.gba", 0x003f8dea, 0x00000002
	.section .rom.003f9651, "ax"
	.incbin "baserom.gba", 0x003f9651, 0x00000003
	.section .rom.003f9a75, "ax"
	.incbin "baserom.gba", 0x003f9a75, 0x00000003
	.section .rom.003f9d0f, "ax"
	.incbin "baserom.gba", 0x003f9d0f, 0x00000001
	.section .rom.003fb6ab, "ax"
	.incbin "baserom.gba", 0x003fb6ab, 0x00000001
	.section .rom.003fd422, "ax"
	.incbin "baserom.gba", 0x003fd422, 0x00000002
	.section .rom.003ff397, "ax"
	.incbin "baserom.gba", 0x003ff397, 0x00000001
	.section .rom.003ff867, "ax"
	.incbin "baserom.gba", 0x003ff867, 0x00000001
	.section .rom.003ffef9, "ax"
	.incbin "baserom.gba", 0x003ffef9, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003ffefc, 0x00000a34
	.section .rom.00401d01, "ax"
	.incbin "baserom.gba", 0x00401d01, 0x00000003
	.section .rom.004027d2, "ax"
	.incbin "baserom.gba", 0x004027d2, 0x00000002
	.section .rom.0040330f, "ax"
	.incbin "baserom.gba", 0x0040330f, 0x00000001
	.section .rom.0040394f, "ax"
	.incbin "baserom.gba", 0x0040394f, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00403950, 0x00001588
	.section .rom.00404f39, "ax"
	.incbin "baserom.gba", 0x00404f39, 0x00000003
	.section .rom.004052a7, "ax"
	.incbin "baserom.gba", 0x004052a7, 0x00000001
	.section .rom.00405845, "ax"
	.incbin "baserom.gba", 0x00405845, 0x00000003
	.section .rom.00405b79, "ax"
	.incbin "baserom.gba", 0x00405b79, 0x00000003
	.section .rom.00405eb5, "ax"
	.incbin "baserom.gba", 0x00405eb5, 0x00000003
	.section .rom.00406ece, "ax"
	.incbin "baserom.gba", 0x00406ece, 0x00000002
	.section .rom.0040a4da, "ax"
	.incbin "baserom.gba", 0x0040a4da, 0x00000002
	.section .rom.0040ac7d, "ax"
	.incbin "baserom.gba", 0x0040ac7d, 0x00000003
	.section .rom.0040c00f, "ax"
	.incbin "baserom.gba", 0x0040c00f, 0x00000001
	.section .rom.0040c43f, "ax"
	.incbin "baserom.gba", 0x0040c43f, 0x00000001
	.section .rom.0040e235, "ax"
	.incbin "baserom.gba", 0x0040e235, 0x00000003
	.section .rom.0040eb4a, "ax"
	.incbin "baserom.gba", 0x0040eb4a, 0x00000002
	.section .rom.0041067e, "ax"
	.incbin "baserom.gba", 0x0041067e, 0x00000002
	.section .rom.004116cd, "ax"
	.incbin "baserom.gba", 0x004116cd, 0x00000003
	.section .rom.00411d83, "ax"
	.incbin "baserom.gba", 0x00411d83, 0x00000001
	.section .rom.00412e29, "ax"
	.incbin "baserom.gba", 0x00412e29, 0x00000003
	.section .rom.0042623b, "ax"
	.incbin "baserom.gba", 0x0042623b, 0x00000001
	.section .rom.0042638d, "ax"
	.incbin "baserom.gba", 0x0042638d, 0x00000003
	.section .rom.0042681e, "ax"
	.incbin "baserom.gba", 0x0042681e, 0x00000002
	.section .rom.00426a15, "ax"
	.incbin "baserom.gba", 0x00426a15, 0x00000003
	.section .rom.004280d2, "ax"
	.incbin "baserom.gba", 0x004280d2, 0x00000002
	.section .rom.004295f1, "ax"
	.incbin "baserom.gba", 0x004295f1, 0x00000003
	.section .rom.0042a1bd, "ax"
	.incbin "baserom.gba", 0x0042a1bd, 0x00000003
	.section .rom.0042ad32, "ax"
	.incbin "baserom.gba", 0x0042ad32, 0x00000002
	.section .rom.0042cbdb, "ax"
	.incbin "baserom.gba", 0x0042cbdb, 0x00000001
	.section .rom.0042e369, "ax"
	.incbin "baserom.gba", 0x0042e369, 0x00000003
	.section .rom.0042f81a, "ax"
	.incbin "baserom.gba", 0x0042f81a, 0x00000002
	.section .rom.00431f67, "ax"
	.incbin "baserom.gba", 0x00431f67, 0x00000001
	.section .rom.00433801, "ax"
	.incbin "baserom.gba", 0x00433801, 0x00000003
	.section .rom.00434dca, "ax"
	.incbin "baserom.gba", 0x00434dca, 0x00000002
	.section .rom.0043685a, "ax"
	.incbin "baserom.gba", 0x0043685a, 0x00000002
	.section .rom.00441032, "ax"
	.incbin "baserom.gba", 0x00441032, 0x00000002
	.section .rom.004431e6, "ax"
	.incbin "baserom.gba", 0x004431e6, 0x00000002
	.section .rom.0044734d, "ax"
	.incbin "baserom.gba", 0x0044734d, 0x00000003
	.section .rom.0044a572, "ax"
	.incbin "baserom.gba", 0x0044a572, 0x00000002
	.section .rom.0044bc76, "ax"
	.incbin "baserom.gba", 0x0044bc76, 0x00000002
	.section .rom.0045285e, "ax"
	.incbin "baserom.gba", 0x0045285e, 0x00000002
	.section .rom.0045b1e2, "ax"
	.incbin "baserom.gba", 0x0045b1e2, 0x00000002
	.section .rom.00462cbe, "ax"
	.incbin "baserom.gba", 0x00462cbe, 0x00000002
	.section .rom.00465d52, "ax"
	.incbin "baserom.gba", 0x00465d52, 0x00000002
	.section .rom.00469161, "ax"
	.incbin "baserom.gba", 0x00469161, 0x00000003
	.section .rom.0046b9cd, "ax"
	.incbin "baserom.gba", 0x0046b9cd, 0x00000003
	.section .rom.0046d4be, "ax"
	.incbin "baserom.gba", 0x0046d4be, 0x00000002
	.section .rom.0046e2d6, "ax"
	.incbin "baserom.gba", 0x0046e2d6, 0x00000002
	.section .rom.0046efc1, "ax"
	.incbin "baserom.gba", 0x0046efc1, 0x00000003
	.section .rom.0047847e, "ax"
	.incbin "baserom.gba", 0x0047847e, 0x00000002
	.section .rom.00479189, "ax"
	.incbin "baserom.gba", 0x00479189, 0x00000003
	.section .rom.0047b29d, "ax"
	.incbin "baserom.gba", 0x0047b29d, 0x00000003
	.section .rom.0047bf0f, "ax"
	.incbin "baserom.gba", 0x0047bf0f, 0x00000001
	.section .rom.0047cb27, "ax"
	.incbin "baserom.gba", 0x0047cb27, 0x00000001
	.section .rom.0047d29b, "ax"
	.incbin "baserom.gba", 0x0047d29b, 0x00000001
	.section .rom.0047eb2b, "ax"
	.incbin "baserom.gba", 0x0047eb2b, 0x00000001
	.section .rom.0047f4f9, "ax"
	.incbin "baserom.gba", 0x0047f4f9, 0x00000003
	.section .rom.00480117, "ax"
	.incbin "baserom.gba", 0x00480117, 0x00000001
	.section .rom.004827cb, "ax"
	.incbin "baserom.gba", 0x004827cb, 0x00000001
	.section .rom.00484ede, "ax"
	.incbin "baserom.gba", 0x00484ede, 0x00000002
	.section .rom.00489e33, "ax"
	.incbin "baserom.gba", 0x00489e33, 0x00000001
	.section .rom.0048e15d, "ax"
	.incbin "baserom.gba", 0x0048e15d, 0x00000003
	.section .rom.0048ed4a, "ax"
	.incbin "baserom.gba", 0x0048ed4a, 0x00000002
	.section .rom.004938ff, "ax"
	.incbin "baserom.gba", 0x004938ff, 0x00000001
	.section .rom.00495c69, "ax"
	.incbin "baserom.gba", 0x00495c69, 0x00000003
	.section .rom.0049760b, "ax"
	.incbin "baserom.gba", 0x0049760b, 0x00000001
	.section .rom.0049becd, "ax"
	.incbin "baserom.gba", 0x0049becd, 0x00000003
	.section .rom.0049ff95, "ax"
	.incbin "baserom.gba", 0x0049ff95, 0x00000003
	.section .rom.004a365e, "ax"
	.incbin "baserom.gba", 0x004a365e, 0x00000002
	.section .rom.004ab62f, "ax"
	.incbin "baserom.gba", 0x004ab62f, 0x00000001
	.section .rom.004b293f, "ax"
	.incbin "baserom.gba", 0x004b293f, 0x00000001
	.section .rom.004b78bf, "ax"
	.incbin "baserom.gba", 0x004b78bf, 0x00000001
	.section .rom.004bc26f, "ax"
	.incbin "baserom.gba", 0x004bc26f, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004bc270, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004bc27c, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004bc3cc, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004bc50c, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bc64c, 0x00000140
	.section .rom.004c1b9b, "ax"
	.incbin "baserom.gba", 0x004c1b9b, 0x00000001
	.section .rom.004c1d29, "ax"
	.incbin "baserom.gba", 0x004c1d29, 0x00000003
	.section .rom.004c6d89, "ax"
	.incbin "baserom.gba", 0x004c6d89, 0x00000003
	.section .rom.004cb2f5, "ax"
	.incbin "baserom.gba", 0x004cb2f5, 0x00000003
	.section .rom.004d04b9, "ax"
	.incbin "baserom.gba", 0x004d04b9, 0x00000003
	.section .rom.004d064f, "ax"
	.incbin "baserom.gba", 0x004d064f, 0x00000001
	.section .rom.004d32a3, "ax"
	.incbin "baserom.gba", 0x004d32a3, 0x00000001
	.section .rom.004d9f57, "ax"
	.incbin "baserom.gba", 0x004d9f57, 0x00000001
	.section .rom.004dd092, "ax"
	.incbin "baserom.gba", 0x004dd092, 0x00000002
	.section .rom.004dd213, "ax"
	.incbin "baserom.gba", 0x004dd213, 0x00000001
	.section .rom.004df9cd, "ax"
	.incbin "baserom.gba", 0x004df9cd, 0x00000003
	.section .rom.004e2016, "ax"
	.incbin "baserom.gba", 0x004e2016, 0x00000002
	.section .rom.004e46fa, "ax"
	.incbin "baserom.gba", 0x004e46fa, 0x00000002
	.section .rom.004e576f, "ax"
	.incbin "baserom.gba", 0x004e576f, 0x00000001
	.section .rom.004e78c2, "ax"
	.incbin "baserom.gba", 0x004e78c2, 0x00000002
	.section .rom.004e7a55, "ax"
	.incbin "baserom.gba", 0x004e7a55, 0x00000003
	.section .rom.004ea297, "ax"
	.incbin "baserom.gba", 0x004ea297, 0x00000001
	.section .rom.004ec0ad, "ax"
	.incbin "baserom.gba", 0x004ec0ad, 0x00000003
	.section .rom.004ee78e, "ax"
	.incbin "baserom.gba", 0x004ee78e, 0x00000002
	.section .rom.004ef8d5, "ax"
	.incbin "baserom.gba", 0x004ef8d5, 0x00000003
	.section .rom.004f29e9, "ax"
	.incbin "baserom.gba", 0x004f29e9, 0x00000003
	.section .rom.004f6b51, "ax"
	.incbin "baserom.gba", 0x004f6b51, 0x00000003
	.section .rom.004f814e, "ax"
	.incbin "baserom.gba", 0x004f814e, 0x00000002
	.section .rom.004f9417, "ax"
	.incbin "baserom.gba", 0x004f9417, 0x00000001
	.section .rom.004fc82f, "ax"
	.incbin "baserom.gba", 0x004fc82f, 0x00000001
	.section .rom.004fc9bd, "ax"
	.incbin "baserom.gba", 0x004fc9bd, 0x00000003
	.section .rom.004fe76e, "ax"
	.incbin "baserom.gba", 0x004fe76e, 0x00000002
	.section .rom.005022e5, "ax"
	.incbin "baserom.gba", 0x005022e5, 0x00000003
	.section .rom.005037f3, "ax"
	.incbin "baserom.gba", 0x005037f3, 0x00000001
	.section .rom.005043c7, "ax"
	.incbin "baserom.gba", 0x005043c7, 0x00000001
	.section .rom.005044c5, "ax"
	.incbin "baserom.gba", 0x005044c5, 0x00000003
	.section .rom.005058aa, "ax"
	.incbin "baserom.gba", 0x005058aa, 0x00000002
	.section .rom.00506a2d, "ax"
	.incbin "baserom.gba", 0x00506a2d, 0x00000003
	.section .rom.005073b6, "ax"
	.incbin "baserom.gba", 0x005073b6, 0x00000002
	.section .rom.0050a033, "ax"
	.incbin "baserom.gba", 0x0050a033, 0x00000001
	.section .rom.0050a116, "ax"
	.incbin "baserom.gba", 0x0050a116, 0x00000002
	.section .rom.0050b301, "ax"
	.incbin "baserom.gba", 0x0050b301, 0x00000003
	.section .rom.0050cf89, "ax"
	.incbin "baserom.gba", 0x0050cf89, 0x00000003
	.section .rom.0050d497, "ax"
	.incbin "baserom.gba", 0x0050d497, 0x00000001
	.section .rom.0050d5d7, "ax"
	.incbin "baserom.gba", 0x0050d5d7, 0x00000001
	.section .rom.0050e7f2, "ax"
	.incbin "baserom.gba", 0x0050e7f2, 0x00000002
	.section .rom.0050e94d, "ax"
	.incbin "baserom.gba", 0x0050e94d, 0x00000003
	.section .rom.00512d73, "ax"
	.incbin "baserom.gba", 0x00512d73, 0x00000001
	.section .rom.00514aa2, "ax"
	.incbin "baserom.gba", 0x00514aa2, 0x00000002
	.section .rom.0051581b, "ax"
	.incbin "baserom.gba", 0x0051581b, 0x00000001
	.section .rom.0051594f, "ax"
	.incbin "baserom.gba", 0x0051594f, 0x00000001
	.section .rom.00517dd2, "ax"
	.incbin "baserom.gba", 0x00517dd2, 0x00000002
	.section .rom.00518b4f, "ax"
	.incbin "baserom.gba", 0x00518b4f, 0x00000001
	.section .rom.0051962e, "ax"
	.incbin "baserom.gba", 0x0051962e, 0x00000002
	.section .rom.0051a4a6, "ax"
	.incbin "baserom.gba", 0x0051a4a6, 0x00000002
	.section .rom.0051c57a, "ax"
	.incbin "baserom.gba", 0x0051c57a, 0x00000002
	.section .rom.0051cec1, "ax"
	.incbin "baserom.gba", 0x0051cec1, 0x00000003
	.section .rom.0051db0e, "ax"
	.incbin "baserom.gba", 0x0051db0e, 0x00000002
	.section .rom.0051dd35, "ax"
	.incbin "baserom.gba", 0x0051dd35, 0x00000003
	.section .rom.0051f677, "ax"
	.incbin "baserom.gba", 0x0051f677, 0x00000001
	.section .rom.0051f78b, "ax"
	.incbin "baserom.gba", 0x0051f78b, 0x00000001
	.section .rom.00521151, "ax"
	.incbin "baserom.gba", 0x00521151, 0x00000003
	.section .rom.0052129b, "ax"
	.incbin "baserom.gba", 0x0052129b, 0x00000001
	.section .rom.00523367, "ax"
	.incbin "baserom.gba", 0x00523367, 0x00000001
	.section .rom.0052348f, "ax"
	.incbin "baserom.gba", 0x0052348f, 0x00000001
	.section .rom.00524d17, "ax"
	.incbin "baserom.gba", 0x00524d17, 0x00000001
	.section .rom.00526fb9, "ax"
	.incbin "baserom.gba", 0x00526fb9, 0x00000003
	.section .rom.005270fb, "ax"
	.incbin "baserom.gba", 0x005270fb, 0x00000001
	.section .rom.00529103, "ax"
	.incbin "baserom.gba", 0x00529103, 0x00000001
	.section .rom.0052920e, "ax"
	.incbin "baserom.gba", 0x0052920e, 0x00000002
	.section .rom.0052b1f2, "ax"
	.incbin "baserom.gba", 0x0052b1f2, 0x00000002
	.section .rom.0052c917, "ax"
	.incbin "baserom.gba", 0x0052c917, 0x00000001
	.section .rom.0052d9cb, "ax"
	.incbin "baserom.gba", 0x0052d9cb, 0x00000001
	.section .rom.0052f6b2, "ax"
	.incbin "baserom.gba", 0x0052f6b2, 0x00000002
	.section .rom.0052f80d, "ax"
	.incbin "baserom.gba", 0x0052f80d, 0x00000003
	.section .rom.00531756, "ax"
	.incbin "baserom.gba", 0x00531756, 0x00000002
	.section .rom.005318db, "ax"
	.incbin "baserom.gba", 0x005318db, 0x00000001
	.section .rom.0053442d, "ax"
	.incbin "baserom.gba", 0x0053442d, 0x00000003
	.section .rom.00536af2, "ax"
	.incbin "baserom.gba", 0x00536af2, 0x00000002
	.section .rom.0053924a, "ax"
	.incbin "baserom.gba", 0x0053924a, 0x00000002
	.section .rom.0053aa9d, "ax"
	.incbin "baserom.gba", 0x0053aa9d, 0x00000003
	.section .rom.0053d19d, "ax"
	.incbin "baserom.gba", 0x0053d19d, 0x00000003
	.section .rom.0053ef69, "ax"
	.incbin "baserom.gba", 0x0053ef69, 0x00000003
	.section .rom.005414b3, "ax"
	.incbin "baserom.gba", 0x005414b3, 0x00000001
	.section .rom.0054277e, "ax"
	.incbin "baserom.gba", 0x0054277e, 0x00000002
	.section .rom.0054342f, "ax"
	.incbin "baserom.gba", 0x0054342f, 0x00000001
	.section .rom.00543e99, "ax"
	.incbin "baserom.gba", 0x00543e99, 0x00000003
	.section .rom.00543f8f, "ax"
	.incbin "baserom.gba", 0x00543f8f, 0x00000001
	.section .rom.005459ee, "ax"
	.incbin "baserom.gba", 0x005459ee, 0x00000002
	.section .rom.00546792, "ax"
	.incbin "baserom.gba", 0x00546792, 0x00000002
	.section .rom.005482cb, "ax"
	.incbin "baserom.gba", 0x005482cb, 0x00000001
	.section .rom.0054b1b9, "ax"
	.incbin "baserom.gba", 0x0054b1b9, 0x00000003
	.section .rom.0055299f, "ax"
	.incbin "baserom.gba", 0x0055299f, 0x00000001
	.section .rom.00552a6b, "ax"
	.incbin "baserom.gba", 0x00552a6b, 0x00000001
	.section .rom.0055822d, "ax"
	.incbin "baserom.gba", 0x0055822d, 0x00000003
	.section .rom.0055836f, "ax"
	.incbin "baserom.gba", 0x0055836f, 0x00000001
	.section .rom.0055979d, "ax"
	.incbin "baserom.gba", 0x0055979d, 0x00000003
	.section .rom.0055c4da, "ax"
	.incbin "baserom.gba", 0x0055c4da, 0x00000002
	.section .rom.0055e5d6, "ax"
	.incbin "baserom.gba", 0x0055e5d6, 0x00000002
	.section .rom.00565fe2, "ax"
	.incbin "baserom.gba", 0x00565fe2, 0x00000002
	.section .rom.0056617e, "ax"
	.incbin "baserom.gba", 0x0056617e, 0x00000002
	.section .rom.0056aa19, "ax"
	.incbin "baserom.gba", 0x0056aa19, 0x00000003
	.section .rom.0056cf1a, "ax"
	.incbin "baserom.gba", 0x0056cf1a, 0x00000002
	.section .rom.0057490b, "ax"
	.incbin "baserom.gba", 0x0057490b, 0x00000001
	.section .rom.00574aa6, "ax"
	.incbin "baserom.gba", 0x00574aa6, 0x00000002
	.section .rom.00577655, "ax"
	.incbin "baserom.gba", 0x00577655, 0x00000003
	.section .rom.00579729, "ax"
	.incbin "baserom.gba", 0x00579729, 0x00000003
	.section .rom.0057b782, "ax"
	.incbin "baserom.gba", 0x0057b782, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057b784, 0x00001cec
	.section .rom.0057ec19, "ax"
	.incbin "baserom.gba", 0x0057ec19, 0x00000003
	.section .rom.005818f5, "ax"
	.incbin "baserom.gba", 0x005818f5, 0x00000003
	.section .rom.005836c7, "ax"
	.incbin "baserom.gba", 0x005836c7, 0x00000001
	.section .rom.00583807, "ax"
	.incbin "baserom.gba", 0x00583807, 0x00000001
	.section .rom.00586861, "ax"
	.incbin "baserom.gba", 0x00586861, 0x00000003
	.section .rom.005897de, "ax"
	.incbin "baserom.gba", 0x005897de, 0x00000002
	.section .rom.0058c0a5, "ax"
	.incbin "baserom.gba", 0x0058c0a5, 0x00000003
	.section .rom.0059300d, "ax"
	.incbin "baserom.gba", 0x0059300d, 0x00000003
	.section .rom.005948b3, "ax"
	.incbin "baserom.gba", 0x005948b3, 0x00000001
	.section .rom.005951b2, "ax"
	.incbin "baserom.gba", 0x005951b2, 0x00000002
	.section .rom.00595f2b, "ax"
	.incbin "baserom.gba", 0x00595f2b, 0x00000001
	.section .rom.0059608b, "ax"
	.incbin "baserom.gba", 0x0059608b, 0x00000001
	.section .rom.00597ce3, "ax"
	.incbin "baserom.gba", 0x00597ce3, 0x00000001
	.section .rom.0059957b, "ax"
	.incbin "baserom.gba", 0x0059957b, 0x00000001
	.section .rom.005996ff, "ax"
	.incbin "baserom.gba", 0x005996ff, 0x00000001
	.section .rom.0059c23b, "ax"
	.incbin "baserom.gba", 0x0059c23b, 0x00000001
	.section .rom.0059e9b5, "ax"
	.incbin "baserom.gba", 0x0059e9b5, 0x00000003
	.section .rom.0059f9a2, "ax"
	.incbin "baserom.gba", 0x0059f9a2, 0x00000002
	.section .rom.005a1063, "ax"
	.incbin "baserom.gba", 0x005a1063, 0x00000001
	.section .rom.005a120d, "ax"
	.incbin "baserom.gba", 0x005a120d, 0x00000003
	.section .rom.005a3fa1, "ax"
	.incbin "baserom.gba", 0x005a3fa1, 0x00000003
	.section .rom.005a8036, "ax"
	.incbin "baserom.gba", 0x005a8036, 0x00000002
	.section .rom.005a8177, "ax"
	.incbin "baserom.gba", 0x005a8177, 0x00000001
	.section .rom.005aa03f, "ax"
	.incbin "baserom.gba", 0x005aa03f, 0x00000001
	.section .rom.005aa1e1, "ax"
	.incbin "baserom.gba", 0x005aa1e1, 0x00000003
	.section .rom.005ac283, "ax"
	.incbin "baserom.gba", 0x005ac283, 0x00000001
	.section .rom.005ad210, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005ad210, 0x000022d8
	.section .rom.005b1fd9, "ax"
	.incbin "baserom.gba", 0x005b1fd9, 0x00000003
	.section .rom.005b215e, "ax"
	.incbin "baserom.gba", 0x005b215e, 0x00000002
	.section .rom.005b98ab, "ax"
	.incbin "baserom.gba", 0x005b98ab, 0x00000001
	.section .rom.005b9db5, "ax"
	.incbin "baserom.gba", 0x005b9db5, 0x00000003
	.section .rom.005bb48d, "ax"
	.incbin "baserom.gba", 0x005bb48d, 0x00000003
	.section .rom.005bb631, "ax"
	.incbin "baserom.gba", 0x005bb631, 0x00000003
	.section .rom.005bcf36, "ax"
	.incbin "baserom.gba", 0x005bcf36, 0x00000002
	.section .rom.005be763, "ax"
	.incbin "baserom.gba", 0x005be763, 0x00000001
	.section .rom.005be8e1, "ax"
	.incbin "baserom.gba", 0x005be8e1, 0x00000003
	.section .rom.005c31d5, "ax"
	.incbin "baserom.gba", 0x005c31d5, 0x00000003
	.section .rom.005c421a, "ax"
	.incbin "baserom.gba", 0x005c421a, 0x00000002
	.section .rom.005c503e, "ax"
	.incbin "baserom.gba", 0x005c503e, 0x00000002
	.section .rom.005d8d22, "ax"
	.incbin "baserom.gba", 0x005d8d22, 0x00000002
	.section .rom.005ef13a, "ax"
	.incbin "baserom.gba", 0x005ef13a, 0x00000002
	.section .rom.0060ac10, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x0060ac10, 0x00002904
	.section .rom.00692825, "ax"
	.incbin "baserom.gba", 0x00692825, 0x00000003
	.section .rom.006d6fe5, "ax"
	.incbin "baserom.gba", 0x006d6fe5, 0x00000003
	.section .rom.007192b4, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007192b4, 0x00001164
	.section .rom.0074701f, "ax"
	.incbin "baserom.gba", 0x0074701f, 0x00000001
	.section .rom.0075729c, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x0075729c, 0x000000e4
	.section .rom.0075d51c, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075d51c, 0x00004600
