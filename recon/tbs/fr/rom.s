	.section .rom.00007a78, "ax"
	.incbin "baserom.gba", 0x00007a78, 0x00000024
	.section .rom.00007b48, "ax"
	.incbin "baserom.gba", 0x00007b48, 0x00000004
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.00028e9c, "ax"
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00028e9c, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x00029298, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002ce40, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002d0c0, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002f858, 0x00000800
	.section .rom.00032ecc, "ax"
	.incbin "baserom.gba", 0x00032ecc, 0x0000002c
	.global Data_08033e40
Data_08033e40:
	.incbin "baserom.gba", 0x00032ef8, 0x00000020
	.global UiText_ItemPronoun
UiText_ItemPronoun:
	.incbin "baserom.gba", 0x00032f18, 0x00000006
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
	.section .rom.000f1278, "ax"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000f1278, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000f1280, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.incbin "baserom.gba", 0x000f1288, 0x00000010
	.global Mode12_Points
Mode12_Points:
	.incbin "baserom.gba", 0x000f1298, 0x00000018
	.global RisingWall_UnitScale
RisingWall_UnitScale:
	.incbin "baserom.gba", 0x000f12b0, 0x00000008
	.global Formation_NormalScale
Formation_NormalScale:
	.incbin "baserom.gba", 0x000f12b8, 0x00000008
	.global Formation_SmallScale
Formation_SmallScale:
	.incbin "baserom.gba", 0x000f12c0, 0x00000008
	.global CirclingScene_UnitScale
CirclingScene_UnitScale:
	.incbin "baserom.gba", 0x000f12c8, 0x00000008
	.global ShockwaveScene_UnitScale
ShockwaveScene_UnitScale:
	.incbin "baserom.gba", 0x000f12d0, 0x00000008
	.global ObjectRow_SweepPair
ObjectRow_SweepPair:
	.incbin "baserom.gba", 0x000f12d8, 0x00000008
	.global ObjectRow_RisePair
ObjectRow_RisePair:
	.incbin "baserom.gba", 0x000f12e0, 0x00000008
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000f12e8, 0x00000008
	.section .rom.000f1648, "ax"
	.global ParticleStreams_CellOffsets
ParticleStreams_CellOffsets:
	.incbin "baserom.gba", 0x000f1648, 0x00000014
	.global BattleFx6_FlareCells
BattleFx6_FlareCells:
	.incbin "baserom.gba", 0x000f165c, 0x00000028
	.global BattleFx_PuffCells
BattleFx_PuffCells:
	.incbin "baserom.gba", 0x000f1684, 0x00000012
	.global BattleFx_PuffSizes
BattleFx_PuffSizes:
	.incbin "baserom.gba", 0x000f1696, 0x00000009
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
	.incbin "baserom.gba", 0x000f3a5c, 0x000007e5
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f4241, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f42a4, 0x00000544
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000f47e8, 0x000002f3
	.section .rom.000f68bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f68bc, 0x000000e6
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f69a2, 0x00000006
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f69a8, 0x0000003c
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000f69e4, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000f6a24, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000f6a64, 0x00000040
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
	.section .rom.000fb708, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000fb708, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000fb71c, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000fb722, 0x00000008
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000fb72a, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000fb738, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000fb746, 0x00000008
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
	.section .rom.00338909, "ax"
	.incbin "baserom.gba", 0x00338909, 0x00000003
	.section .rom.0033c40d, "ax"
	.incbin "baserom.gba", 0x0033c40d, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033c410, 0x000006a8
	.section .rom.00340efa, "ax"
	.incbin "baserom.gba", 0x00340efa, 0x00000002
	.section .rom.0035166a, "ax"
	.incbin "baserom.gba", 0x0035166a, 0x00000002
	.section .rom.00354c5a, "ax"
	.incbin "baserom.gba", 0x00354c5a, 0x00000002
	.section .rom.003606fe, "ax"
	.incbin "baserom.gba", 0x003606fe, 0x00000002
	.section .rom.00370dae, "ax"
	.incbin "baserom.gba", 0x00370dae, 0x00000002
	.section .rom.0037884e, "ax"
	.incbin "baserom.gba", 0x0037884e, 0x00000002
	.section .rom.0037c08e, "ax"
	.incbin "baserom.gba", 0x0037c08e, 0x00000002
	.section .rom.003854c2, "ax"
	.incbin "baserom.gba", 0x003854c2, 0x00000002
	.section .rom.003908ea, "ax"
	.incbin "baserom.gba", 0x003908ea, 0x00000002
	.section .rom.003987a6, "ax"
	.incbin "baserom.gba", 0x003987a6, 0x00000002
	.section .rom.0039d236, "ax"
	.incbin "baserom.gba", 0x0039d236, 0x00000002
	.section .rom.003a14ea, "ax"
	.incbin "baserom.gba", 0x003a14ea, 0x00000002
	.section .rom.003a4e6a, "ax"
	.incbin "baserom.gba", 0x003a4e6a, 0x00000002
	.section .rom.003b16a6, "ax"
	.incbin "baserom.gba", 0x003b16a6, 0x00000002
	.section .rom.003bffee, "ax"
	.incbin "baserom.gba", 0x003bffee, 0x00000002
	.section .rom.003c58f1, "ax"
	.incbin "baserom.gba", 0x003c58f1, 0x00000003
	.section .rom.003c7273, "ax"
	.incbin "baserom.gba", 0x003c7273, 0x00000001
	.section .rom.003ca091, "ax"
	.incbin "baserom.gba", 0x003ca091, 0x00000003
	.section .rom.003cd23b, "ax"
	.incbin "baserom.gba", 0x003cd23b, 0x00000001
	.section .rom.003cda81, "ax"
	.incbin "baserom.gba", 0x003cda81, 0x00000003
	.section .rom.003cf297, "ax"
	.incbin "baserom.gba", 0x003cf297, 0x00000001
	.section .rom.003cf882, "ax"
	.incbin "baserom.gba", 0x003cf882, 0x00000002
	.section .rom.003cff46, "ax"
	.incbin "baserom.gba", 0x003cff46, 0x00000002
	.section .rom.003d022d, "ax"
	.incbin "baserom.gba", 0x003d022d, 0x00000003
	.section .rom.003d1593, "ax"
	.incbin "baserom.gba", 0x003d1593, 0x00000001
	.section .rom.003d197d, "ax"
	.incbin "baserom.gba", 0x003d197d, 0x00000003
	.section .rom.003d1d4f, "ax"
	.incbin "baserom.gba", 0x003d1d4f, 0x00000001
	.section .rom.003d25ef, "ax"
	.incbin "baserom.gba", 0x003d25ef, 0x00000001
	.section .rom.003d2a92, "ax"
	.incbin "baserom.gba", 0x003d2a92, 0x00000002
	.section .rom.003d3c4b, "ax"
	.incbin "baserom.gba", 0x003d3c4b, 0x00000001
	.section .rom.003d510e, "ax"
	.incbin "baserom.gba", 0x003d510e, 0x00000002
	.section .rom.003d60d7, "ax"
	.incbin "baserom.gba", 0x003d60d7, 0x00000001
	.section .rom.003d6332, "ax"
	.incbin "baserom.gba", 0x003d6332, 0x00000002
	.section .rom.003d7d8d, "ax"
	.incbin "baserom.gba", 0x003d7d8d, 0x00000003
	.section .rom.003d82d9, "ax"
	.incbin "baserom.gba", 0x003d82d9, 0x00000003
	.section .rom.003da062, "ax"
	.incbin "baserom.gba", 0x003da062, 0x00000002
	.section .rom.003da2db, "ax"
	.incbin "baserom.gba", 0x003da2db, 0x00000001
	.section .rom.003da7a2, "ax"
	.incbin "baserom.gba", 0x003da7a2, 0x00000002
	.section .rom.003dc377, "ax"
	.incbin "baserom.gba", 0x003dc377, 0x00000001
	.section .rom.003ddf75, "ax"
	.incbin "baserom.gba", 0x003ddf75, 0x00000003
	.section .rom.003de196, "ax"
	.incbin "baserom.gba", 0x003de196, 0x00000002
	.section .rom.003de5d3, "ax"
	.incbin "baserom.gba", 0x003de5d3, 0x00000001
	.section .rom.003de6e5, "ax"
	.incbin "baserom.gba", 0x003de6e5, 0x00000003
	.section .rom.003dee63, "ax"
	.incbin "baserom.gba", 0x003dee63, 0x00000001
	.section .rom.003df301, "ax"
	.incbin "baserom.gba", 0x003df301, 0x00000003
	.section .rom.003e0016, "ax"
	.incbin "baserom.gba", 0x003e0016, 0x00000002
	.section .rom.003e0d96, "ax"
	.incbin "baserom.gba", 0x003e0d96, 0x00000002
	.section .rom.003e1127, "ax"
	.incbin "baserom.gba", 0x003e1127, 0x00000001
	.section .rom.003e2e6e, "ax"
	.incbin "baserom.gba", 0x003e2e6e, 0x00000002
	.section .rom.003e3c45, "ax"
	.incbin "baserom.gba", 0x003e3c45, 0x00000003
	.section .rom.003e4472, "ax"
	.incbin "baserom.gba", 0x003e4472, 0x00000002
	.section .rom.003e4afd, "ax"
	.incbin "baserom.gba", 0x003e4afd, 0x00000003
	.section .rom.003e4cbb, "ax"
	.incbin "baserom.gba", 0x003e4cbb, 0x00000001
	.section .rom.003e5a13, "ax"
	.incbin "baserom.gba", 0x003e5a13, 0x00000001
	.section .rom.003e5f5f, "ax"
	.incbin "baserom.gba", 0x003e5f5f, 0x00000001
	.section .rom.003e6339, "ax"
	.incbin "baserom.gba", 0x003e6339, 0x00000003
	.section .rom.003e66e7, "ax"
	.incbin "baserom.gba", 0x003e66e7, 0x00000001
	.section .rom.003e868b, "ax"
	.incbin "baserom.gba", 0x003e868b, 0x00000001
	.section .rom.003e9427, "ax"
	.incbin "baserom.gba", 0x003e9427, 0x00000001
	.section .rom.003e9643, "ax"
	.incbin "baserom.gba", 0x003e9643, 0x00000001
	.section .rom.003e993f, "ax"
	.incbin "baserom.gba", 0x003e993f, 0x00000001
	.section .rom.003ebaed, "ax"
	.incbin "baserom.gba", 0x003ebaed, 0x00000003
	.section .rom.003ec8bb, "ax"
	.incbin "baserom.gba", 0x003ec8bb, 0x00000001
	.section .rom.003eda1d, "ax"
	.incbin "baserom.gba", 0x003eda1d, 0x00000003
	.section .rom.003edf77, "ax"
	.incbin "baserom.gba", 0x003edf77, 0x00000001
	.section .rom.003ef861, "ax"
	.incbin "baserom.gba", 0x003ef861, 0x00000003
	.section .rom.003f0102, "ax"
	.incbin "baserom.gba", 0x003f0102, 0x00000002
	.section .rom.003f04df, "ax"
	.incbin "baserom.gba", 0x003f04df, 0x00000001
	.section .rom.003f0769, "ax"
	.incbin "baserom.gba", 0x003f0769, 0x00000003
	.section .rom.003f0b0f, "ax"
	.incbin "baserom.gba", 0x003f0b0f, 0x00000001
	.section .rom.003f0d6a, "ax"
	.incbin "baserom.gba", 0x003f0d6a, 0x00000002
	.section .rom.003f1122, "ax"
	.incbin "baserom.gba", 0x003f1122, 0x00000002
	.section .rom.003f260b, "ax"
	.incbin "baserom.gba", 0x003f260b, 0x00000001
	.section .rom.003f3bce, "ax"
	.incbin "baserom.gba", 0x003f3bce, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f3bd0, 0x00000a6c
	.section .rom.003f5412, "ax"
	.incbin "baserom.gba", 0x003f5412, 0x00000002
	.section .rom.003f5795, "ax"
	.incbin "baserom.gba", 0x003f5795, 0x00000003
	.section .rom.003f6445, "ax"
	.incbin "baserom.gba", 0x003f6445, 0x00000003
	.section .rom.003f6f8d, "ax"
	.incbin "baserom.gba", 0x003f6f8d, 0x00000003
	.section .rom.003f7126, "ax"
	.incbin "baserom.gba", 0x003f7126, 0x00000002
	.section .rom.003f79b3, "ax"
	.incbin "baserom.gba", 0x003f79b3, 0x00000001
	.section .rom.003f847a, "ax"
	.incbin "baserom.gba", 0x003f847a, 0x00000002
	.section .rom.003f8992, "ax"
	.incbin "baserom.gba", 0x003f8992, 0x00000002
	.section .rom.003f8fb6, "ax"
	.incbin "baserom.gba", 0x003f8fb6, 0x00000002
	.section .rom.003f981d, "ax"
	.incbin "baserom.gba", 0x003f981d, 0x00000003
	.section .rom.003f9c41, "ax"
	.incbin "baserom.gba", 0x003f9c41, 0x00000003
	.section .rom.003f9edb, "ax"
	.incbin "baserom.gba", 0x003f9edb, 0x00000001
	.section .rom.003fb877, "ax"
	.incbin "baserom.gba", 0x003fb877, 0x00000001
	.section .rom.003fd5ee, "ax"
	.incbin "baserom.gba", 0x003fd5ee, 0x00000002
	.section .rom.003ff563, "ax"
	.incbin "baserom.gba", 0x003ff563, 0x00000001
	.section .rom.003ffa33, "ax"
	.incbin "baserom.gba", 0x003ffa33, 0x00000001
	.section .rom.004000c5, "ax"
	.incbin "baserom.gba", 0x004000c5, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x004000c8, 0x00000a34
	.section .rom.00401ecd, "ax"
	.incbin "baserom.gba", 0x00401ecd, 0x00000003
	.section .rom.0040299e, "ax"
	.incbin "baserom.gba", 0x0040299e, 0x00000002
	.section .rom.004034db, "ax"
	.incbin "baserom.gba", 0x004034db, 0x00000001
	.section .rom.00403b1b, "ax"
	.incbin "baserom.gba", 0x00403b1b, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x00403b1c, 0x00001588
	.section .rom.00405105, "ax"
	.incbin "baserom.gba", 0x00405105, 0x00000003
	.section .rom.00405473, "ax"
	.incbin "baserom.gba", 0x00405473, 0x00000001
	.section .rom.00405a11, "ax"
	.incbin "baserom.gba", 0x00405a11, 0x00000003
	.section .rom.00405d45, "ax"
	.incbin "baserom.gba", 0x00405d45, 0x00000003
	.section .rom.00406081, "ax"
	.incbin "baserom.gba", 0x00406081, 0x00000003
	.section .rom.0040709a, "ax"
	.incbin "baserom.gba", 0x0040709a, 0x00000002
	.section .rom.0040a6a6, "ax"
	.incbin "baserom.gba", 0x0040a6a6, 0x00000002
	.section .rom.0040ae49, "ax"
	.incbin "baserom.gba", 0x0040ae49, 0x00000003
	.section .rom.0040c1db, "ax"
	.incbin "baserom.gba", 0x0040c1db, 0x00000001
	.section .rom.0040c60b, "ax"
	.incbin "baserom.gba", 0x0040c60b, 0x00000001
	.section .rom.0040e401, "ax"
	.incbin "baserom.gba", 0x0040e401, 0x00000003
	.section .rom.0040ed16, "ax"
	.incbin "baserom.gba", 0x0040ed16, 0x00000002
	.section .rom.0041084a, "ax"
	.incbin "baserom.gba", 0x0041084a, 0x00000002
	.section .rom.00411899, "ax"
	.incbin "baserom.gba", 0x00411899, 0x00000003
	.section .rom.00411f4f, "ax"
	.incbin "baserom.gba", 0x00411f4f, 0x00000001
	.section .rom.00412ff5, "ax"
	.incbin "baserom.gba", 0x00412ff5, 0x00000003
	.section .rom.00426407, "ax"
	.incbin "baserom.gba", 0x00426407, 0x00000001
	.section .rom.00426559, "ax"
	.incbin "baserom.gba", 0x00426559, 0x00000003
	.section .rom.004269ea, "ax"
	.incbin "baserom.gba", 0x004269ea, 0x00000002
	.section .rom.00426be1, "ax"
	.incbin "baserom.gba", 0x00426be1, 0x00000003
	.section .rom.0042829e, "ax"
	.incbin "baserom.gba", 0x0042829e, 0x00000002
	.section .rom.004297bd, "ax"
	.incbin "baserom.gba", 0x004297bd, 0x00000003
	.section .rom.0042a389, "ax"
	.incbin "baserom.gba", 0x0042a389, 0x00000003
	.section .rom.0042aefe, "ax"
	.incbin "baserom.gba", 0x0042aefe, 0x00000002
	.section .rom.0042cda7, "ax"
	.incbin "baserom.gba", 0x0042cda7, 0x00000001
	.section .rom.0042e535, "ax"
	.incbin "baserom.gba", 0x0042e535, 0x00000003
	.section .rom.0042f9e6, "ax"
	.incbin "baserom.gba", 0x0042f9e6, 0x00000002
	.section .rom.00432133, "ax"
	.incbin "baserom.gba", 0x00432133, 0x00000001
	.section .rom.004339cd, "ax"
	.incbin "baserom.gba", 0x004339cd, 0x00000003
	.section .rom.00434f96, "ax"
	.incbin "baserom.gba", 0x00434f96, 0x00000002
	.section .rom.00436a26, "ax"
	.incbin "baserom.gba", 0x00436a26, 0x00000002
	.section .rom.004377db, "ax"
	.incbin "baserom.gba", 0x004377db, 0x00000001
	.section .rom.0044114a, "ax"
	.incbin "baserom.gba", 0x0044114a, 0x00000002
	.section .rom.004432fe, "ax"
	.incbin "baserom.gba", 0x004432fe, 0x00000002
	.section .rom.00447465, "ax"
	.incbin "baserom.gba", 0x00447465, 0x00000003
	.section .rom.0044a68a, "ax"
	.incbin "baserom.gba", 0x0044a68a, 0x00000002
	.section .rom.0044bd8e, "ax"
	.incbin "baserom.gba", 0x0044bd8e, 0x00000002
	.section .rom.00452976, "ax"
	.incbin "baserom.gba", 0x00452976, 0x00000002
	.section .rom.0045b2fa, "ax"
	.incbin "baserom.gba", 0x0045b2fa, 0x00000002
	.section .rom.00462dd6, "ax"
	.incbin "baserom.gba", 0x00462dd6, 0x00000002
	.section .rom.00465e6a, "ax"
	.incbin "baserom.gba", 0x00465e6a, 0x00000002
	.section .rom.00469279, "ax"
	.incbin "baserom.gba", 0x00469279, 0x00000003
	.section .rom.0046bae5, "ax"
	.incbin "baserom.gba", 0x0046bae5, 0x00000003
	.section .rom.0046d5d6, "ax"
	.incbin "baserom.gba", 0x0046d5d6, 0x00000002
	.section .rom.0046e3ee, "ax"
	.incbin "baserom.gba", 0x0046e3ee, 0x00000002
	.section .rom.0046f0d9, "ax"
	.incbin "baserom.gba", 0x0046f0d9, 0x00000003
	.section .rom.00478596, "ax"
	.incbin "baserom.gba", 0x00478596, 0x00000002
	.section .rom.004792a1, "ax"
	.incbin "baserom.gba", 0x004792a1, 0x00000003
	.section .rom.0047b3b5, "ax"
	.incbin "baserom.gba", 0x0047b3b5, 0x00000003
	.section .rom.0047c027, "ax"
	.incbin "baserom.gba", 0x0047c027, 0x00000001
	.section .rom.0047cc3f, "ax"
	.incbin "baserom.gba", 0x0047cc3f, 0x00000001
	.section .rom.0047d3b3, "ax"
	.incbin "baserom.gba", 0x0047d3b3, 0x00000001
	.section .rom.0047ec43, "ax"
	.incbin "baserom.gba", 0x0047ec43, 0x00000001
	.section .rom.0047f611, "ax"
	.incbin "baserom.gba", 0x0047f611, 0x00000003
	.section .rom.0048022f, "ax"
	.incbin "baserom.gba", 0x0048022f, 0x00000001
	.section .rom.004828e3, "ax"
	.incbin "baserom.gba", 0x004828e3, 0x00000001
	.section .rom.00484ff6, "ax"
	.incbin "baserom.gba", 0x00484ff6, 0x00000002
	.section .rom.00489f4b, "ax"
	.incbin "baserom.gba", 0x00489f4b, 0x00000001
	.section .rom.0048e275, "ax"
	.incbin "baserom.gba", 0x0048e275, 0x00000003
	.section .rom.0048ee62, "ax"
	.incbin "baserom.gba", 0x0048ee62, 0x00000002
	.section .rom.00493a17, "ax"
	.incbin "baserom.gba", 0x00493a17, 0x00000001
	.section .rom.00495d81, "ax"
	.incbin "baserom.gba", 0x00495d81, 0x00000003
	.section .rom.00497723, "ax"
	.incbin "baserom.gba", 0x00497723, 0x00000001
	.section .rom.0049bfe5, "ax"
	.incbin "baserom.gba", 0x0049bfe5, 0x00000003
	.section .rom.004a00ad, "ax"
	.incbin "baserom.gba", 0x004a00ad, 0x00000003
	.section .rom.004a3776, "ax"
	.incbin "baserom.gba", 0x004a3776, 0x00000002
	.section .rom.004ab747, "ax"
	.incbin "baserom.gba", 0x004ab747, 0x00000001
	.section .rom.004b2a57, "ax"
	.incbin "baserom.gba", 0x004b2a57, 0x00000001
	.section .rom.004b79d7, "ax"
	.incbin "baserom.gba", 0x004b79d7, 0x00000001
	.section .rom.004bc387, "ax"
	.incbin "baserom.gba", 0x004bc387, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004bc388, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004bc394, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004bc4e4, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004bc624, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bc764, 0x00000140
	.section .rom.004c1cb3, "ax"
	.incbin "baserom.gba", 0x004c1cb3, 0x00000001
	.section .rom.004c1e41, "ax"
	.incbin "baserom.gba", 0x004c1e41, 0x00000003
	.section .rom.004c6ea1, "ax"
	.incbin "baserom.gba", 0x004c6ea1, 0x00000003
	.section .rom.004cb40d, "ax"
	.incbin "baserom.gba", 0x004cb40d, 0x00000003
	.section .rom.004d05d1, "ax"
	.incbin "baserom.gba", 0x004d05d1, 0x00000003
	.section .rom.004d0767, "ax"
	.incbin "baserom.gba", 0x004d0767, 0x00000001
	.section .rom.004d33bb, "ax"
	.incbin "baserom.gba", 0x004d33bb, 0x00000001
	.section .rom.004da06f, "ax"
	.incbin "baserom.gba", 0x004da06f, 0x00000001
	.section .rom.004dd1aa, "ax"
	.incbin "baserom.gba", 0x004dd1aa, 0x00000002
	.section .rom.004dd32b, "ax"
	.incbin "baserom.gba", 0x004dd32b, 0x00000001
	.section .rom.004dfae5, "ax"
	.incbin "baserom.gba", 0x004dfae5, 0x00000003
	.section .rom.004e212e, "ax"
	.incbin "baserom.gba", 0x004e212e, 0x00000002
	.section .rom.004e4812, "ax"
	.incbin "baserom.gba", 0x004e4812, 0x00000002
	.section .rom.004e5887, "ax"
	.incbin "baserom.gba", 0x004e5887, 0x00000001
	.section .rom.004e79da, "ax"
	.incbin "baserom.gba", 0x004e79da, 0x00000002
	.section .rom.004e7b6d, "ax"
	.incbin "baserom.gba", 0x004e7b6d, 0x00000003
	.section .rom.004ea3af, "ax"
	.incbin "baserom.gba", 0x004ea3af, 0x00000001
	.section .rom.004ec1c5, "ax"
	.incbin "baserom.gba", 0x004ec1c5, 0x00000003
	.section .rom.004ee8a6, "ax"
	.incbin "baserom.gba", 0x004ee8a6, 0x00000002
	.section .rom.004ef9ed, "ax"
	.incbin "baserom.gba", 0x004ef9ed, 0x00000003
	.section .rom.004f2b01, "ax"
	.incbin "baserom.gba", 0x004f2b01, 0x00000003
	.section .rom.004f6c69, "ax"
	.incbin "baserom.gba", 0x004f6c69, 0x00000003
	.section .rom.004f8266, "ax"
	.incbin "baserom.gba", 0x004f8266, 0x00000002
	.section .rom.004f952f, "ax"
	.incbin "baserom.gba", 0x004f952f, 0x00000001
	.section .rom.004fc947, "ax"
	.incbin "baserom.gba", 0x004fc947, 0x00000001
	.section .rom.004fcad5, "ax"
	.incbin "baserom.gba", 0x004fcad5, 0x00000003
	.section .rom.004fe886, "ax"
	.incbin "baserom.gba", 0x004fe886, 0x00000002
	.section .rom.005023fd, "ax"
	.incbin "baserom.gba", 0x005023fd, 0x00000003
	.section .rom.0050390b, "ax"
	.incbin "baserom.gba", 0x0050390b, 0x00000001
	.section .rom.005044df, "ax"
	.incbin "baserom.gba", 0x005044df, 0x00000001
	.section .rom.005045dd, "ax"
	.incbin "baserom.gba", 0x005045dd, 0x00000003
	.section .rom.005059c2, "ax"
	.incbin "baserom.gba", 0x005059c2, 0x00000002
	.section .rom.00506b45, "ax"
	.incbin "baserom.gba", 0x00506b45, 0x00000003
	.section .rom.005074ce, "ax"
	.incbin "baserom.gba", 0x005074ce, 0x00000002
	.section .rom.0050a14b, "ax"
	.incbin "baserom.gba", 0x0050a14b, 0x00000001
	.section .rom.0050a22e, "ax"
	.incbin "baserom.gba", 0x0050a22e, 0x00000002
	.section .rom.0050b419, "ax"
	.incbin "baserom.gba", 0x0050b419, 0x00000003
	.section .rom.0050d0a1, "ax"
	.incbin "baserom.gba", 0x0050d0a1, 0x00000003
	.section .rom.0050d5af, "ax"
	.incbin "baserom.gba", 0x0050d5af, 0x00000001
	.section .rom.0050d6ef, "ax"
	.incbin "baserom.gba", 0x0050d6ef, 0x00000001
	.section .rom.0050e90a, "ax"
	.incbin "baserom.gba", 0x0050e90a, 0x00000002
	.section .rom.0050ea65, "ax"
	.incbin "baserom.gba", 0x0050ea65, 0x00000003
	.section .rom.00512e8b, "ax"
	.incbin "baserom.gba", 0x00512e8b, 0x00000001
	.section .rom.00514bba, "ax"
	.incbin "baserom.gba", 0x00514bba, 0x00000002
	.section .rom.00515933, "ax"
	.incbin "baserom.gba", 0x00515933, 0x00000001
	.section .rom.00515a67, "ax"
	.incbin "baserom.gba", 0x00515a67, 0x00000001
	.section .rom.00517eea, "ax"
	.incbin "baserom.gba", 0x00517eea, 0x00000002
	.section .rom.00518c67, "ax"
	.incbin "baserom.gba", 0x00518c67, 0x00000001
	.section .rom.00519746, "ax"
	.incbin "baserom.gba", 0x00519746, 0x00000002
	.section .rom.0051a5be, "ax"
	.incbin "baserom.gba", 0x0051a5be, 0x00000002
	.section .rom.0051c692, "ax"
	.incbin "baserom.gba", 0x0051c692, 0x00000002
	.section .rom.0051cfd9, "ax"
	.incbin "baserom.gba", 0x0051cfd9, 0x00000003
	.section .rom.0051dc26, "ax"
	.incbin "baserom.gba", 0x0051dc26, 0x00000002
	.section .rom.0051de4d, "ax"
	.incbin "baserom.gba", 0x0051de4d, 0x00000003
	.section .rom.0051f78f, "ax"
	.incbin "baserom.gba", 0x0051f78f, 0x00000001
	.section .rom.0051f8a3, "ax"
	.incbin "baserom.gba", 0x0051f8a3, 0x00000001
	.section .rom.00521269, "ax"
	.incbin "baserom.gba", 0x00521269, 0x00000003
	.section .rom.005213b3, "ax"
	.incbin "baserom.gba", 0x005213b3, 0x00000001
	.section .rom.0052347f, "ax"
	.incbin "baserom.gba", 0x0052347f, 0x00000001
	.section .rom.005235a7, "ax"
	.incbin "baserom.gba", 0x005235a7, 0x00000001
	.section .rom.00524e2f, "ax"
	.incbin "baserom.gba", 0x00524e2f, 0x00000001
	.section .rom.005270d1, "ax"
	.incbin "baserom.gba", 0x005270d1, 0x00000003
	.section .rom.00527213, "ax"
	.incbin "baserom.gba", 0x00527213, 0x00000001
	.section .rom.0052921b, "ax"
	.incbin "baserom.gba", 0x0052921b, 0x00000001
	.section .rom.00529326, "ax"
	.incbin "baserom.gba", 0x00529326, 0x00000002
	.section .rom.0052b30a, "ax"
	.incbin "baserom.gba", 0x0052b30a, 0x00000002
	.section .rom.0052ca2f, "ax"
	.incbin "baserom.gba", 0x0052ca2f, 0x00000001
	.section .rom.0052dae3, "ax"
	.incbin "baserom.gba", 0x0052dae3, 0x00000001
	.section .rom.0052f7ca, "ax"
	.incbin "baserom.gba", 0x0052f7ca, 0x00000002
	.section .rom.0052f925, "ax"
	.incbin "baserom.gba", 0x0052f925, 0x00000003
	.section .rom.0053186e, "ax"
	.incbin "baserom.gba", 0x0053186e, 0x00000002
	.section .rom.005319f3, "ax"
	.incbin "baserom.gba", 0x005319f3, 0x00000001
	.section .rom.00534545, "ax"
	.incbin "baserom.gba", 0x00534545, 0x00000003
	.section .rom.00536c0a, "ax"
	.incbin "baserom.gba", 0x00536c0a, 0x00000002
	.section .rom.00539362, "ax"
	.incbin "baserom.gba", 0x00539362, 0x00000002
	.section .rom.0053abb5, "ax"
	.incbin "baserom.gba", 0x0053abb5, 0x00000003
	.section .rom.0053d2b5, "ax"
	.incbin "baserom.gba", 0x0053d2b5, 0x00000003
	.section .rom.0053f081, "ax"
	.incbin "baserom.gba", 0x0053f081, 0x00000003
	.section .rom.005415cb, "ax"
	.incbin "baserom.gba", 0x005415cb, 0x00000001
	.section .rom.00542896, "ax"
	.incbin "baserom.gba", 0x00542896, 0x00000002
	.section .rom.00543547, "ax"
	.incbin "baserom.gba", 0x00543547, 0x00000001
	.section .rom.00543fb1, "ax"
	.incbin "baserom.gba", 0x00543fb1, 0x00000003
	.section .rom.005440a7, "ax"
	.incbin "baserom.gba", 0x005440a7, 0x00000001
	.section .rom.00545b06, "ax"
	.incbin "baserom.gba", 0x00545b06, 0x00000002
	.section .rom.005468aa, "ax"
	.incbin "baserom.gba", 0x005468aa, 0x00000002
	.section .rom.005483e3, "ax"
	.incbin "baserom.gba", 0x005483e3, 0x00000001
	.section .rom.0054b2d1, "ax"
	.incbin "baserom.gba", 0x0054b2d1, 0x00000003
	.section .rom.00552ab7, "ax"
	.incbin "baserom.gba", 0x00552ab7, 0x00000001
	.section .rom.00552b83, "ax"
	.incbin "baserom.gba", 0x00552b83, 0x00000001
	.section .rom.00558345, "ax"
	.incbin "baserom.gba", 0x00558345, 0x00000003
	.section .rom.00558487, "ax"
	.incbin "baserom.gba", 0x00558487, 0x00000001
	.section .rom.005598b5, "ax"
	.incbin "baserom.gba", 0x005598b5, 0x00000003
	.section .rom.0055c5f2, "ax"
	.incbin "baserom.gba", 0x0055c5f2, 0x00000002
	.section .rom.0055e6ee, "ax"
	.incbin "baserom.gba", 0x0055e6ee, 0x00000002
	.section .rom.005660fa, "ax"
	.incbin "baserom.gba", 0x005660fa, 0x00000002
	.section .rom.00566296, "ax"
	.incbin "baserom.gba", 0x00566296, 0x00000002
	.section .rom.0056ab31, "ax"
	.incbin "baserom.gba", 0x0056ab31, 0x00000003
	.section .rom.0056d032, "ax"
	.incbin "baserom.gba", 0x0056d032, 0x00000002
	.section .rom.00574a23, "ax"
	.incbin "baserom.gba", 0x00574a23, 0x00000001
	.section .rom.00574bbe, "ax"
	.incbin "baserom.gba", 0x00574bbe, 0x00000002
	.section .rom.0057776d, "ax"
	.incbin "baserom.gba", 0x0057776d, 0x00000003
	.section .rom.00579841, "ax"
	.incbin "baserom.gba", 0x00579841, 0x00000003
	.section .rom.0057b89a, "ax"
	.incbin "baserom.gba", 0x0057b89a, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057b89c, 0x00001cec
	.section .rom.0057ed31, "ax"
	.incbin "baserom.gba", 0x0057ed31, 0x00000003
	.section .rom.00581a0d, "ax"
	.incbin "baserom.gba", 0x00581a0d, 0x00000003
	.section .rom.005837df, "ax"
	.incbin "baserom.gba", 0x005837df, 0x00000001
	.section .rom.0058391f, "ax"
	.incbin "baserom.gba", 0x0058391f, 0x00000001
	.section .rom.00586979, "ax"
	.incbin "baserom.gba", 0x00586979, 0x00000003
	.section .rom.005898f6, "ax"
	.incbin "baserom.gba", 0x005898f6, 0x00000002
	.section .rom.0058c1bd, "ax"
	.incbin "baserom.gba", 0x0058c1bd, 0x00000003
	.section .rom.00593125, "ax"
	.incbin "baserom.gba", 0x00593125, 0x00000003
	.section .rom.005949cb, "ax"
	.incbin "baserom.gba", 0x005949cb, 0x00000001
	.section .rom.005952ca, "ax"
	.incbin "baserom.gba", 0x005952ca, 0x00000002
	.section .rom.00596043, "ax"
	.incbin "baserom.gba", 0x00596043, 0x00000001
	.section .rom.005961a3, "ax"
	.incbin "baserom.gba", 0x005961a3, 0x00000001
	.section .rom.00597dfb, "ax"
	.incbin "baserom.gba", 0x00597dfb, 0x00000001
	.section .rom.00599693, "ax"
	.incbin "baserom.gba", 0x00599693, 0x00000001
	.section .rom.00599817, "ax"
	.incbin "baserom.gba", 0x00599817, 0x00000001
	.section .rom.0059c353, "ax"
	.incbin "baserom.gba", 0x0059c353, 0x00000001
	.section .rom.0059eacd, "ax"
	.incbin "baserom.gba", 0x0059eacd, 0x00000003
	.section .rom.0059faba, "ax"
	.incbin "baserom.gba", 0x0059faba, 0x00000002
	.section .rom.005a117b, "ax"
	.incbin "baserom.gba", 0x005a117b, 0x00000001
	.section .rom.005a1325, "ax"
	.incbin "baserom.gba", 0x005a1325, 0x00000003
	.section .rom.005a40b9, "ax"
	.incbin "baserom.gba", 0x005a40b9, 0x00000003
	.section .rom.005a814e, "ax"
	.incbin "baserom.gba", 0x005a814e, 0x00000002
	.section .rom.005a828f, "ax"
	.incbin "baserom.gba", 0x005a828f, 0x00000001
	.section .rom.005aa157, "ax"
	.incbin "baserom.gba", 0x005aa157, 0x00000001
	.section .rom.005aa2f9, "ax"
	.incbin "baserom.gba", 0x005aa2f9, 0x00000003
	.section .rom.005ac39b, "ax"
	.incbin "baserom.gba", 0x005ac39b, 0x00000001
	.section .rom.005ad328, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005ad328, 0x000022d8
	.section .rom.005b20f1, "ax"
	.incbin "baserom.gba", 0x005b20f1, 0x00000003
	.section .rom.005b2276, "ax"
	.incbin "baserom.gba", 0x005b2276, 0x00000002
	.section .rom.005b99c3, "ax"
	.incbin "baserom.gba", 0x005b99c3, 0x00000001
	.section .rom.005b9ecd, "ax"
	.incbin "baserom.gba", 0x005b9ecd, 0x00000003
	.section .rom.005bb5a5, "ax"
	.incbin "baserom.gba", 0x005bb5a5, 0x00000003
	.section .rom.005bb749, "ax"
	.incbin "baserom.gba", 0x005bb749, 0x00000003
	.section .rom.005bd04e, "ax"
	.incbin "baserom.gba", 0x005bd04e, 0x00000002
	.section .rom.005be87b, "ax"
	.incbin "baserom.gba", 0x005be87b, 0x00000001
	.section .rom.005be9f9, "ax"
	.incbin "baserom.gba", 0x005be9f9, 0x00000003
	.section .rom.005c32ed, "ax"
	.incbin "baserom.gba", 0x005c32ed, 0x00000003
	.section .rom.005c4332, "ax"
	.incbin "baserom.gba", 0x005c4332, 0x00000002
	.section .rom.005c5156, "ax"
	.incbin "baserom.gba", 0x005c5156, 0x00000002
	.section .rom.005d8e3a, "ax"
	.incbin "baserom.gba", 0x005d8e3a, 0x00000002
	.section .rom.005ef252, "ax"
	.incbin "baserom.gba", 0x005ef252, 0x00000002
	.section .rom.0060ad28, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x0060ad28, 0x00002904
	.section .rom.0069293d, "ax"
	.incbin "baserom.gba", 0x0069293d, 0x00000003
	.section .rom.006d70fd, "ax"
	.incbin "baserom.gba", 0x006d70fd, 0x00000003
	.section .rom.007193cc, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x007193cc, 0x00001164
	.section .rom.00747137, "ax"
	.incbin "baserom.gba", 0x00747137, 0x00000001
	.section .rom.007573b4, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x007573b4, 0x000000e4
	.section .rom.0075d634, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075d634, 0x00004600
