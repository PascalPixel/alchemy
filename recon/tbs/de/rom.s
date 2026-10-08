	.section .rom.00002e00, "ax"
	.incbin "baserom.gba", 0x00002e00, 0x00000020
	.section .rom.00007a98, "ax"
	.incbin "baserom.gba", 0x00007a98, 0x00000024
	.section .rom.00007b68, "ax"
	.incbin "baserom.gba", 0x00007b68, 0x00000004
	.section .rom.0000c2ca, "ax"
	.incbin "baserom.gba", 0x0000c2ca, 0x00000002
	.section .rom.00028ba8, "ax"
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00028ba8, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x00028fa4, 0x00003ba8
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002cb4c, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002cdcc, 0x0000279c
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x0002f568, 0x00000800
	.section .rom.0007a770, "ax"
	.global UiText_ItemGenders
UiText_ItemGenders:
	.incbin "baserom.gba", 0x0007a770, 0x0000010d
	.section .rom.0007a886, "ax"
	.incbin "baserom.gba", 0x0007a886, 0x00000002
	.section .rom.000a08d8, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x000a08d8, 0x00000638
	.section .rom.000a1158, "ax"
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000a1158, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x000a1540, 0x00000400
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000a1940, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x000a19d8, 0x00000218
	.section .rom.000e4772, "ax"
	.incbin "baserom.gba", 0x000e4772, 0x00000002
	.section .rom.000efc9f, "ax"
	.global PuffArc_CellWidths
PuffArc_CellWidths:
	.incbin "baserom.gba", 0x000efc9f, 0x00000006
	.global PuffArc_CellHeights
PuffArc_CellHeights:
	.incbin "baserom.gba", 0x000efca5, 0x00000006
	.global PuffArc_CellBiasY
PuffArc_CellBiasY:
	.incbin "baserom.gba", 0x000efcab, 0x00000007
	.global PuffArc_CellSourceOffsets
PuffArc_CellSourceOffsets:
	.incbin "baserom.gba", 0x000efcb2, 0x0000000c
	.global BattleFx_GlintCellOffsets
BattleFx_GlintCellOffsets:
	.incbin "baserom.gba", 0x000efcbe, 0x0000000c
	.global BattleFx_GlintCellWidths
BattleFx_GlintCellWidths:
	.incbin "baserom.gba", 0x000efcca, 0x00000006
	.global BattleFx_GlintCellHeights
BattleFx_GlintCellHeights:
	.incbin "baserom.gba", 0x000efcd0, 0x00000006
	.global FallingShards_Counts
FallingShards_Counts:
	.incbin "baserom.gba", 0x000efcd6, 0x00000006
	.global FallingBolts_Counts
FallingBolts_Counts:
	.incbin "baserom.gba", 0x000efcdc, 0x0000000c
	.global FiveMode_Records
FiveMode_Records:
	.incbin "baserom.gba", 0x000efce8, 0x00000014
	.global FiveMode_Tremble
FiveMode_Tremble:
	.incbin "baserom.gba", 0x000efcfc, 0x00000008
	.global TwelveMode_Records
TwelveMode_Records:
	.incbin "baserom.gba", 0x000efd04, 0x00000054
	.global TwelveMode_StrikeWidths
TwelveMode_StrikeWidths:
	.incbin "baserom.gba", 0x000efd58, 0x00000006
	.global TwelveMode_StrikeHeights
TwelveMode_StrikeHeights:
	.incbin "baserom.gba", 0x000efd5e, 0x00000006
	.global TwelveMode_StrikeOffsets
TwelveMode_StrikeOffsets:
	.incbin "baserom.gba", 0x000efd64, 0x0000000c
	.global TwelveMode_StrikeReach
TwelveMode_StrikeReach:
	.incbin "baserom.gba", 0x000efd70, 0x00000006
	.global TwelveMode_ColumnStages
TwelveMode_ColumnStages:
	.incbin "baserom.gba", 0x000efd76, 0x00000005
	.global TwelveMode_GlintDrawFlags
TwelveMode_GlintDrawFlags:
	.incbin "baserom.gba", 0x000efd7b, 0x00000004
	.global BladeRain_CellWidths
BladeRain_CellWidths:
	.incbin "baserom.gba", 0x000efd7f, 0x00000004
	.global BladeRain_CellHeights
BladeRain_CellHeights:
	.incbin "baserom.gba", 0x000efd83, 0x00000005
	.global BladeRain_CellSourceOffsets
BladeRain_CellSourceOffsets:
	.incbin "baserom.gba", 0x000efd88, 0x00000008
	.global BattlePieceWidth
BattlePieceWidth:
	.incbin "baserom.gba", 0x000efd90, 0x00000021
	.global BattlePieceHeight
BattlePieceHeight:
	.incbin "baserom.gba", 0x000efdb1, 0x00000021
	.global BattlePieceOffset
BattlePieceOffset:
	.incbin "baserom.gba", 0x000efdd2, 0x00000044
	.global BattlePieceStartX
BattlePieceStartX:
	.incbin "baserom.gba", 0x000efe16, 0x00000021
	.global BattlePieceStartY
BattlePieceStartY:
	.incbin "baserom.gba", 0x000efe37, 0x00000021
	.global CornerSparks_X
CornerSparks_X:
	.incbin "baserom.gba", 0x000efe58, 0x00000004
	.global CornerSparks_Y
CornerSparks_Y:
	.incbin "baserom.gba", 0x000efe5c, 0x00000004
	.global CornerSparks_DrawFlags
CornerSparks_DrawFlags:
	.incbin "baserom.gba", 0x000efe60, 0x00000004
	.global TwoResource_CellWidths
TwoResource_CellWidths:
	.incbin "baserom.gba", 0x000efe64, 0x00000006
	.global TwoResource_CellHeights
TwoResource_CellHeights:
	.incbin "baserom.gba", 0x000efe6a, 0x00000006
	.global TwoResource_CellSourceOffsets
TwoResource_CellSourceOffsets:
	.incbin "baserom.gba", 0x000efe70, 0x0000000c
	.global TwoResource_CellX
TwoResource_CellX:
	.incbin "baserom.gba", 0x000efe7c, 0x0000000c
	.global TwoResource_CellBiasY
TwoResource_CellBiasY:
	.incbin "baserom.gba", 0x000efe88, 0x00000008
	.global MemberBurst_Counts
MemberBurst_Counts:
	.incbin "baserom.gba", 0x000efe90, 0x00000006
	.global EarthWall_CellSourceOffsets
EarthWall_CellSourceOffsets:
	.incbin "baserom.gba", 0x000efe96, 0x00000006
	.global EarthWall_CellWidths
EarthWall_CellWidths:
	.incbin "baserom.gba", 0x000efe9c, 0x00000003
	.global EarthWall_CellHeights
EarthWall_CellHeights:
	.incbin "baserom.gba", 0x000efe9f, 0x00000003
	.global MemberBeam_MotePictures
MemberBeam_MotePictures:
	.incbin "baserom.gba", 0x000efea2, 0x00000008
	.global MemberBeam_HeadOffsets
MemberBeam_HeadOffsets:
	.incbin "baserom.gba", 0x000efeaa, 0x00000006
	.global MemberBeam_HeadWidths
MemberBeam_HeadWidths:
	.incbin "baserom.gba", 0x000efeb0, 0x00000003
	.global MemberBeam_HeadHeights
MemberBeam_HeadHeights:
	.incbin "baserom.gba", 0x000efeb3, 0x00000003
	.global SevenMode_Records
SevenMode_Records:
	.incbin "baserom.gba", 0x000efeb6, 0x0000000e
	.global SevenMode_ShardWidths
SevenMode_ShardWidths:
	.incbin "baserom.gba", 0x000efec4, 0x00000012
	.global SevenMode_ShardHeights
SevenMode_ShardHeights:
	.incbin "baserom.gba", 0x000efed6, 0x00000012
	.global SevenMode_ShardOffsets
SevenMode_ShardOffsets:
	.incbin "baserom.gba", 0x000efee8, 0x00000024
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000eff0c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000eff1a, 0x0000000e
	.global SpinningTriangle_Vertex
SpinningTriangle_Vertex:
	.incbin "baserom.gba", 0x000eff28, 0x0000000c
	.global TriangleStrike_Vertex
TriangleStrike_Vertex:
	.incbin "baserom.gba", 0x000eff34, 0x0000000c
	.global RingBolts_Points
RingBolts_Points:
	.incbin "baserom.gba", 0x000eff40, 0x00000018
	.global SpinningStars_Radii
SpinningStars_Radii:
	.incbin "baserom.gba", 0x000eff58, 0x00000002
	.global Data_080ee15a
Data_080ee15a:
	.incbin "baserom.gba", 0x000eff5a, 0x00000009
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000eff63, 0x00000009
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000eff6c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000eff74, 0x00000003
	.global Data_080ee177
Data_080ee177:
	.incbin "baserom.gba", 0x000eff77, 0x00000003
	.global Data_080ee17a
Data_080ee17a:
	.incbin "baserom.gba", 0x000eff7a, 0x00000004
	.global Data_080ee17e
Data_080ee17e:
	.incbin "baserom.gba", 0x000eff7e, 0x00000006
	.global PaletteParticles_DropSpeeds
PaletteParticles_DropSpeeds:
	.incbin "baserom.gba", 0x000eff84, 0x00000004
	.global PaletteParticles_SparkPixels
PaletteParticles_SparkPixels:
	.incbin "baserom.gba", 0x000eff88, 0x00000002
	.global PaletteParticles_DropSizes
PaletteParticles_DropSizes:
	.incbin "baserom.gba", 0x000eff8a, 0x00000004
	.global PaletteParticles_StreakWidths
PaletteParticles_StreakWidths:
	.incbin "baserom.gba", 0x000eff8e, 0x00000006
	.global PaletteParticles_StreakHeights
PaletteParticles_StreakHeights:
	.incbin "baserom.gba", 0x000eff94, 0x00000006
	.global PaletteParticles_StreakDrops
PaletteParticles_StreakDrops:
	.incbin "baserom.gba", 0x000eff9a, 0x00000006
	.global PaletteParticles_StreakOffsets
PaletteParticles_StreakOffsets:
	.incbin "baserom.gba", 0x000effa0, 0x0000000c
	.global EmberColumns_Columns
EmberColumns_Columns:
	.incbin "baserom.gba", 0x000effac, 0x00000008
	.global EmberColumns_Gravity
EmberColumns_Gravity:
	.incbin "baserom.gba", 0x000effb4, 0x00000010
	.global VortexMotes_Counts
VortexMotes_Counts:
	.incbin "baserom.gba", 0x000effc4, 0x00000006
	.global Tornado_Shapes
Tornado_Shapes:
	.incbin "baserom.gba", 0x000effca, 0x00000009
	.global Crystal_ShardStarts
Crystal_ShardStarts:
	.incbin "baserom.gba", 0x000effd3, 0x00000022
	.global Crystal_Counts
Crystal_Counts:
	.incbin "baserom.gba", 0x000efff5, 0x00000006
	.global Crystal_ShardWidths
Crystal_ShardWidths:
	.incbin "baserom.gba", 0x000efffb, 0x0000000c
	.global Crystal_ShardHeights
Crystal_ShardHeights:
	.incbin "baserom.gba", 0x000f0007, 0x0000000d
	.global Crystal_ShardOffsets
Crystal_ShardOffsets:
	.incbin "baserom.gba", 0x000f0014, 0x00000030
	.global LightningPillar_Sparks
LightningPillar_Sparks:
	.incbin "baserom.gba", 0x000f0044, 0x0000000c
	.global LightningPillar_Columns
LightningPillar_Columns:
	.incbin "baserom.gba", 0x000f0050, 0x0000000e
	.global LightningPillar_Counts
LightningPillar_Counts:
	.incbin "baserom.gba", 0x000f005e, 0x00000004
	.global SparkGroups_Shapes
SparkGroups_Shapes:
	.incbin "baserom.gba", 0x000f0062, 0x00000032
	.global SparkGroups_FlashCells
SparkGroups_FlashCells:
	.incbin "baserom.gba", 0x000f0094, 0x00000006
	.global FirePillars_Counts
FirePillars_Counts:
	.incbin "baserom.gba", 0x000f009a, 0x00000003
	.global FirePillars_Depths
FirePillars_Depths:
	.incbin "baserom.gba", 0x000f009d, 0x0000000c
	.global FirePillars_StartFrames
FirePillars_StartFrames:
	.incbin "baserom.gba", 0x000f00a9, 0x00000005
	.global RenderMode_GlintFlips
RenderMode_GlintFlips:
	.incbin "baserom.gba", 0x000f00ae, 0x00000006
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000f00b4, 0x0000065c
	.global ThornVines_ObjectPoses
ThornVines_ObjectPoses:
	.incbin "baserom.gba", 0x000f0710, 0x00000006
	.global ThornVines_ThornOffsets
ThornVines_ThornOffsets:
	.incbin "baserom.gba", 0x000f0716, 0x0000000a
	.global ThornVines_ThornWidths
ThornVines_ThornWidths:
	.incbin "baserom.gba", 0x000f0720, 0x00000005
	.global ThornVines_ThornHeights
ThornVines_ThornHeights:
	.incbin "baserom.gba", 0x000f0725, 0x00000005
	.global ThornVines_LeafOffsets
ThornVines_LeafOffsets:
	.incbin "baserom.gba", 0x000f072a, 0x00000006
	.global ThornVines_LeafSizes
ThornVines_LeafSizes:
	.incbin "baserom.gba", 0x000f0730, 0x00000004
	.global ThornVines_ChipOffsets
ThornVines_ChipOffsets:
	.incbin "baserom.gba", 0x000f0734, 0x0000000a
	.global ThornVines_ChipWidths
ThornVines_ChipWidths:
	.incbin "baserom.gba", 0x000f073e, 0x00000005
	.global ThornVines_ChipHeights
ThornVines_ChipHeights:
	.incbin "baserom.gba", 0x000f0743, 0x00000005
	.global ThornVines_SparkOffsets
ThornVines_SparkOffsets:
	.incbin "baserom.gba", 0x000f0748, 0x0000000a
	.global ThornVines_SparkSizes
ThornVines_SparkSizes:
	.incbin "baserom.gba", 0x000f0752, 0x00000006
	.global ThornVines_EmberOffsets
ThornVines_EmberOffsets:
	.incbin "baserom.gba", 0x000f0758, 0x0000000e
	.global ThornVines_EmberSizes
ThornVines_EmberSizes:
	.incbin "baserom.gba", 0x000f0766, 0x0000000e
	.global BattleFx10_Points
BattleFx10_Points:
	.incbin "baserom.gba", 0x000f0774, 0x00000020
	.global BattleFx10_ShakeOffsets
BattleFx10_ShakeOffsets:
	.incbin "baserom.gba", 0x000f0794, 0x00000004
	.global BattleFx10_RockCells
BattleFx10_RockCells:
	.incbin "baserom.gba", 0x000f0798, 0x00000006
	.global BattleFx10_RockWidths
BattleFx10_RockWidths:
	.incbin "baserom.gba", 0x000f079e, 0x00000003
	.global BattleFx10_RockHeights
BattleFx10_RockHeights:
	.incbin "baserom.gba", 0x000f07a1, 0x00000003
	.global BattleFx10_Animations
BattleFx10_Animations:
	.incbin "baserom.gba", 0x000f07a4, 0x00000004
	.global BattleFx10_DebrisWidths
BattleFx10_DebrisWidths:
	.incbin "baserom.gba", 0x000f07a8, 0x0000000b
	.global BattleFx10_DebrisHeights
BattleFx10_DebrisHeights:
	.incbin "baserom.gba", 0x000f07b3, 0x0000000b
	.global BattleFx10_DebrisCells
BattleFx10_DebrisCells:
	.incbin "baserom.gba", 0x000f07be, 0x00000016
	.global BattleFx10_SprayWidths
BattleFx10_SprayWidths:
	.incbin "baserom.gba", 0x000f07d4, 0x00000003
	.global BattleFx10_SprayHeights
BattleFx10_SprayHeights:
	.incbin "baserom.gba", 0x000f07d7, 0x00000003
	.global BattleFx10_SprayCells
BattleFx10_SprayCells:
	.incbin "baserom.gba", 0x000f07da, 0x00000006
	.global BattleFx10_BoulderCells
BattleFx10_BoulderCells:
	.incbin "baserom.gba", 0x000f07e0, 0x00000006
	.global BattleFx10_BoulderWidths
BattleFx10_BoulderWidths:
	.incbin "baserom.gba", 0x000f07e6, 0x00000003
	.global BattleFx10_BoulderHeights
BattleFx10_BoulderHeights:
	.incbin "baserom.gba", 0x000f07e9, 0x00000003
	.global BattleFx10_FallWidths
BattleFx10_FallWidths:
	.incbin "baserom.gba", 0x000f07ec, 0x00000003
	.global BattleFx10_FallHeights
BattleFx10_FallHeights:
	.incbin "baserom.gba", 0x000f07ef, 0x00000003
	.global BattleFx10_FallCells
BattleFx10_FallCells:
	.incbin "baserom.gba", 0x000f07f2, 0x00000006
	.global IceShardBursts_Gravities
IceShardBursts_Gravities:
	.incbin "baserom.gba", 0x000f07f8, 0x00000010
	.global PaletteRamp_ShardCells
PaletteRamp_ShardCells:
	.incbin "baserom.gba", 0x000f0808, 0x00000018
	.global PaletteRamp_ShardWidths
PaletteRamp_ShardWidths:
	.incbin "baserom.gba", 0x000f0820, 0x0000000c
	.global PaletteRamp_ShardHeights
PaletteRamp_ShardHeights:
	.incbin "baserom.gba", 0x000f082c, 0x0000000c
	.global RockWall_Timings
RockWall_Timings:
	.incbin "baserom.gba", 0x000f0838, 0x00000009
	.global HomingEmbers_Counts
HomingEmbers_Counts:
	.incbin "baserom.gba", 0x000f0841, 0x00000003
	.global HomingEmbers_FlareWidths
HomingEmbers_FlareWidths:
	.incbin "baserom.gba", 0x000f0844, 0x00000006
	.global HomingEmbers_FlareHeights
HomingEmbers_FlareHeights:
	.incbin "baserom.gba", 0x000f084a, 0x00000006
	.global HomingEmbers_FlareBiasY
HomingEmbers_FlareBiasY:
	.incbin "baserom.gba", 0x000f0850, 0x00000006
	.global HomingEmbers_FlareCells
HomingEmbers_FlareCells:
	.incbin "baserom.gba", 0x000f0856, 0x0000000c
	.global RisingMotes_ColumnSpots
RisingMotes_ColumnSpots:
	.incbin "baserom.gba", 0x000f0862, 0x00000026
	.global RisingMotes_Timings
RisingMotes_Timings:
	.incbin "baserom.gba", 0x000f0888, 0x00000009
	.global RisingMotes_MoteWidths
RisingMotes_MoteWidths:
	.incbin "baserom.gba", 0x000f0891, 0x00000008
	.global RisingMotes_MoteHeights
RisingMotes_MoteHeights:
	.incbin "baserom.gba", 0x000f0899, 0x00000009
	.global RisingMotes_MoteCells
RisingMotes_MoteCells:
	.incbin "baserom.gba", 0x000f08a2, 0x00000010
	.global RisingMotes_ColumnCells
RisingMotes_ColumnCells:
	.incbin "baserom.gba", 0x000f08b2, 0x00000006
	.global RisingMotes_ColumnHeights
RisingMotes_ColumnHeights:
	.incbin "baserom.gba", 0x000f08b8, 0x00000003
	.global RisingMotes_EmberWidths
RisingMotes_EmberWidths:
	.incbin "baserom.gba", 0x000f08bb, 0x00000008
	.global RisingMotes_EmberHeights
RisingMotes_EmberHeights:
	.incbin "baserom.gba", 0x000f08c3, 0x00000009
	.global RisingMotes_EmberCells
RisingMotes_EmberCells:
	.incbin "baserom.gba", 0x000f08cc, 0x00000010
	.global LightningBolts_Sparks
LightningBolts_Sparks:
	.incbin "baserom.gba", 0x000f08dc, 0x00000006
	.global ParticleField_Counts
ParticleField_Counts:
	.incbin "baserom.gba", 0x000f08e2, 0x0000000a
	.global ParticleField_PuffCells
ParticleField_PuffCells:
	.incbin "baserom.gba", 0x000f08ec, 0x0000000e
	.global ParticleField_PuffSizes
ParticleField_PuffSizes:
	.incbin "baserom.gba", 0x000f08fa, 0x00000046
	.global StagedParticles_UnitScale
StagedParticles_UnitScale:
	.incbin "baserom.gba", 0x000f0940, 0x00000008
	.global DualTable_VineWidths
DualTable_VineWidths:
	.incbin "baserom.gba", 0x000f0948, 0x00000003
	.global DualTable_HeightLimits
DualTable_HeightLimits:
	.incbin "baserom.gba", 0x000f094b, 0x00000003
	.global DualTable_VineCells
DualTable_VineCells:
	.incbin "baserom.gba", 0x000f094e, 0x00000006
	.global DualTable_PillarWidths
DualTable_PillarWidths:
	.incbin "baserom.gba", 0x000f0954, 0x00000004
	.global DualTable_PillarCells
DualTable_PillarCells:
	.incbin "baserom.gba", 0x000f0958, 0x00000006
	.global DualTable_ColumnCounts
DualTable_ColumnCounts:
	.incbin "baserom.gba", 0x000f095e, 0x00000003
	.global DualTable_ColumnX
DualTable_ColumnX:
	.incbin "baserom.gba", 0x000f0961, 0x00000010
	.global DualTable_PillarHeights
DualTable_PillarHeights:
	.incbin "baserom.gba", 0x000f0971, 0x00000008
	.global DualTable_PuffWidths
DualTable_PuffWidths:
	.incbin "baserom.gba", 0x000f0979, 0x00000007
	.global DualTable_PuffHeights
DualTable_PuffHeights:
	.incbin "baserom.gba", 0x000f0980, 0x00000008
	.global DualTable_PuffCells
DualTable_PuffCells:
	.incbin "baserom.gba", 0x000f0988, 0x0000000e
	.global RisingColumns_ColumnOffsets
RisingColumns_ColumnOffsets:
	.incbin "baserom.gba", 0x000f0996, 0x00000010
	.global BattleFxPillar_Kinds
BattleFxPillar_Kinds:
	.incbin "baserom.gba", 0x000f09a6, 0x00000008
	.global BattleFxPillar_X
BattleFxPillar_X:
	.incbin "baserom.gba", 0x000f09ae, 0x00000008
	.global BattleFxPillar_Counts
BattleFxPillar_Counts:
	.incbin "baserom.gba", 0x000f09b6, 0x00000003
	.global BattleFxPillar_PuffWidths
BattleFxPillar_PuffWidths:
	.incbin "baserom.gba", 0x000f09b9, 0x00000007
	.global BattleFxPillar_PuffHeights
BattleFxPillar_PuffHeights:
	.incbin "baserom.gba", 0x000f09c0, 0x00000008
	.global BattleFxPillar_PuffCells
BattleFxPillar_PuffCells:
	.incbin "baserom.gba", 0x000f09c8, 0x0000000e
	.global LightningBolts_Counts
LightningBolts_Counts:
	.incbin "baserom.gba", 0x000f09d6, 0x0000000c
	.global LightningBolts_GlintPalettes
LightningBolts_GlintPalettes:
	.incbin "baserom.gba", 0x000f09e2, 0x00000004
	.global LightningBolts_GlintModes
LightningBolts_GlintModes:
	.incbin "baserom.gba", 0x000f09e6, 0x00000003
	.global Volley_CometHeights
Volley_CometHeights:
	.incbin "baserom.gba", 0x000f09e9, 0x00000003
	.global Volley_Modes
Volley_Modes:
	.incbin "baserom.gba", 0x000f09ec, 0x0000003c
	.global Volley_FlashWidths
Volley_FlashWidths:
	.incbin "baserom.gba", 0x000f0a28, 0x00000007
	.global Volley_FlashHeights
Volley_FlashHeights:
	.incbin "baserom.gba", 0x000f0a2f, 0x00000007
	.global Volley_FlashSteps
Volley_FlashSteps:
	.incbin "baserom.gba", 0x000f0a36, 0x00000007
	.global Volley_FlashRises
Volley_FlashRises:
	.incbin "baserom.gba", 0x000f0a3d, 0x00000007
	.global Volley_FlashOffsets
Volley_FlashOffsets:
	.incbin "baserom.gba", 0x000f0a44, 0x0000000e
	.global Volley_PelletOffsets
Volley_PelletOffsets:
	.incbin "baserom.gba", 0x000f0a52, 0x00000008
	.global MercuryDjinnFlames_LaunchFrames
MercuryDjinnFlames_LaunchFrames:
	.incbin "baserom.gba", 0x000f0a5a, 0x00000005
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000f0a5f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000f0a63, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f0a68, 0x00000008
	.global RockToss_ChipFlips
RockToss_ChipFlips:
	.incbin "baserom.gba", 0x000f0a70, 0x00000004
	.global RockToss_ChipWidths
RockToss_ChipWidths:
	.incbin "baserom.gba", 0x000f0a74, 0x00000009
	.global RockToss_ChipHeights
RockToss_ChipHeights:
	.incbin "baserom.gba", 0x000f0a7d, 0x00000009
	.global RockToss_ChipCells
RockToss_ChipCells:
	.incbin "baserom.gba", 0x000f0a86, 0x00000012
	.global RockToss_ChipX
RockToss_ChipX:
	.incbin "baserom.gba", 0x000f0a98, 0x00000009
	.global RockToss_ChipY
RockToss_ChipY:
	.incbin "baserom.gba", 0x000f0aa1, 0x00000009
	.global Mode12_Animations
Mode12_Animations:
	.incbin "baserom.gba", 0x000f0aaa, 0x00000004
	.global Mode12_SmokeFlips
Mode12_SmokeFlips:
	.incbin "baserom.gba", 0x000f0aae, 0x00000004
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000f0ab2, 0x00000040
	.global ShatterRocks_RockX
ShatterRocks_RockX:
	.incbin "baserom.gba", 0x000f0af2, 0x00000005
	.global ShatterRocks_DropFrames
ShatterRocks_DropFrames:
	.incbin "baserom.gba", 0x000f0af7, 0x00000005
	.global ShatterRocks_RockCounts
ShatterRocks_RockCounts:
	.incbin "baserom.gba", 0x000f0afc, 0x00000003
	.global ShatterRocks_ShardWidths
ShatterRocks_ShardWidths:
	.incbin "baserom.gba", 0x000f0aff, 0x0000000f
	.global ShatterRocks_ShardHeights
ShatterRocks_ShardHeights:
	.incbin "baserom.gba", 0x000f0b0e, 0x00000010
	.global ShatterRocks_ShardCells
ShatterRocks_ShardCells:
	.incbin "baserom.gba", 0x000f0b1e, 0x00000020
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000f0b3e, 0x00000040
	.global RisingWall_RowSheetOffsets
RisingWall_RowSheetOffsets:
	.incbin "baserom.gba", 0x000f0b7e, 0x00000012
	.global RisingWall_RowWidths
RisingWall_RowWidths:
	.incbin "baserom.gba", 0x000f0b90, 0x0000000a
	.global RisingWall_MoundSheetOffsets
RisingWall_MoundSheetOffsets:
	.incbin "baserom.gba", 0x000f0b9a, 0x00000006
	.global RisingWall_MoundWidths
RisingWall_MoundWidths:
	.incbin "baserom.gba", 0x000f0ba0, 0x00000003
	.global RisingWall_MoundHeights
RisingWall_MoundHeights:
	.incbin "baserom.gba", 0x000f0ba3, 0x00000003
	.global RisingWall_Bg2Shifts
RisingWall_Bg2Shifts:
	.incbin "baserom.gba", 0x000f0ba6, 0x00000006
	.global RisingWall_LateRockColumns
RisingWall_LateRockColumns:
	.incbin "baserom.gba", 0x000f0bac, 0x00000006
	.global BeamSequence_CellWidths
BeamSequence_CellWidths:
	.incbin "baserom.gba", 0x000f0bb2, 0x00000006
	.global BeamSequence_CellHeights
BeamSequence_CellHeights:
	.incbin "baserom.gba", 0x000f0bb8, 0x00000006
	.global BeamSequence_CellSheetOffsets
BeamSequence_CellSheetOffsets:
	.incbin "baserom.gba", 0x000f0bbe, 0x0000000c
	.global BeamSequence_CellReaches
BeamSequence_CellReaches:
	.incbin "baserom.gba", 0x000f0bca, 0x00000006
	.global CastingImpact_GlintDrawFlags
CastingImpact_GlintDrawFlags:
	.incbin "baserom.gba", 0x000f0bd0, 0x00000004
	.global CastingImpact_ImageX
CastingImpact_ImageX:
	.incbin "baserom.gba", 0x000f0bd4, 0x0000000e
	.global CastingImpact_ImageY
CastingImpact_ImageY:
	.incbin "baserom.gba", 0x000f0be2, 0x00000008
	.global CastingImpact_OrbitCells
CastingImpact_OrbitCells:
	.incbin "baserom.gba", 0x000f0bea, 0x0000000a
	.global FireSwirl_CellWidths
FireSwirl_CellWidths:
	.incbin "baserom.gba", 0x000f0bf4, 0x00000007
	.global FireSwirl_CellHeights
FireSwirl_CellHeights:
	.incbin "baserom.gba", 0x000f0bfb, 0x00000007
	.global FireSwirl_CellSourceOffsets
FireSwirl_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f0c02, 0x0000000e
	.global FireSwirl_CellBiasX
FireSwirl_CellBiasX:
	.incbin "baserom.gba", 0x000f0c10, 0x00000007
	.global FireSwirl_CellBiasY
FireSwirl_CellBiasY:
	.incbin "baserom.gba", 0x000f0c17, 0x00000007
	.global Formation_TwelveX
Formation_TwelveX:
	.incbin "baserom.gba", 0x000f0c1e, 0x0000000c
	.global Formation_TwelveZ
Formation_TwelveZ:
	.incbin "baserom.gba", 0x000f0c2a, 0x0000000c
	.global Formation_RightEightX
Formation_RightEightX:
	.incbin "baserom.gba", 0x000f0c36, 0x00000008
	.global Formation_RightEightZ
Formation_RightEightZ:
	.incbin "baserom.gba", 0x000f0c3e, 0x00000008
	.global Formation_SmallEightX
Formation_SmallEightX:
	.incbin "baserom.gba", 0x000f0c46, 0x00000008
	.global Formation_SmallEightZ
Formation_SmallEightZ:
	.incbin "baserom.gba", 0x000f0c4e, 0x00000008
	.global ImpactBurst_CellWidths
ImpactBurst_CellWidths:
	.incbin "baserom.gba", 0x000f0c56, 0x00000008
	.global ImpactBurst_CellHeights
ImpactBurst_CellHeights:
	.incbin "baserom.gba", 0x000f0c5e, 0x00000008
	.global ImpactBurst_CellSourceOffsets
ImpactBurst_CellSourceOffsets:
	.incbin "baserom.gba", 0x000f0c66, 0x00000010
	.global ParticleStreams_OrbitPoints
ParticleStreams_OrbitPoints:
	.incbin "baserom.gba", 0x000f0c76, 0x0000002a
	.global ParticleStreams_DropPoints
ParticleStreams_DropPoints:
	.incbin "baserom.gba", 0x000f0ca0, 0x0000001c
	.global ParticleStreams_FlashSheetOffsets
ParticleStreams_FlashSheetOffsets:
	.incbin "baserom.gba", 0x000f0cbc, 0x0000000e
	.global ParticleStreams_FlashSizes
ParticleStreams_FlashSizes:
	.incbin "baserom.gba", 0x000f0cca, 0x0000000e
	.global CirclingScene_ObjectColumns
CirclingScene_ObjectColumns:
	.incbin "baserom.gba", 0x000f0cd8, 0x00000009
	.global CirclingScene_ObjectRows
CirclingScene_ObjectRows:
	.incbin "baserom.gba", 0x000f0ce1, 0x00000009
	.global CirclingScene_PuffSheetOffsets
CirclingScene_PuffSheetOffsets:
	.incbin "baserom.gba", 0x000f0cea, 0x0000000e
	.global CirclingScene_PuffSizes
CirclingScene_PuffSizes:
	.incbin "baserom.gba", 0x000f0cf8, 0x0000000e
	.global FlameBlade_StrikeColumns
FlameBlade_StrikeColumns:
	.incbin "baserom.gba", 0x000f0d06, 0x00000006
	.global FlameBlade_FlashCells
FlameBlade_FlashCells:
	.incbin "baserom.gba", 0x000f0d0c, 0x00000006
	.global FallingSword_FlashCells
FallingSword_FlashCells:
	.incbin "baserom.gba", 0x000f0d12, 0x00000006
	.global FallingSword_DustGravity
FallingSword_DustGravity:
	.incbin "baserom.gba", 0x000f0d18, 0x00000010
	.global ShockwaveScene_BurstHeights
ShockwaveScene_BurstHeights:
	.incbin "baserom.gba", 0x000f0d28, 0x00000008
	.global ShockwaveScene_BurstCells
ShockwaveScene_BurstCells:
	.incbin "baserom.gba", 0x000f0d30, 0x0000000e
	.global ShockwaveScene_ShardCells
ShockwaveScene_ShardCells:
	.incbin "baserom.gba", 0x000f0d3e, 0x0000000c
	.global ShockwaveScene_ShardWidths
ShockwaveScene_ShardWidths:
	.incbin "baserom.gba", 0x000f0d4a, 0x00000006
	.global ShockwaveScene_ShardHeights
ShockwaveScene_ShardHeights:
	.incbin "baserom.gba", 0x000f0d50, 0x00000006
	.global ObjectRow_Columns
ObjectRow_Columns:
	.incbin "baserom.gba", 0x000f0d56, 0x00000009
	.global ObjectRow_Rows
ObjectRow_Rows:
	.incbin "baserom.gba", 0x000f0d5f, 0x00000009
	.global BattleFx6_ObjectX
BattleFx6_ObjectX:
	.incbin "baserom.gba", 0x000f0d68, 0x00000008
	.global BattleFx6_ObjectY
BattleFx6_ObjectY:
	.incbin "baserom.gba", 0x000f0d70, 0x00000008
	.global BattleFx6_Gravity
BattleFx6_Gravity:
	.incbin "baserom.gba", 0x000f0d78, 0x00000010
	.global RisingBurst_SparkCells
RisingBurst_SparkCells:
	.incbin "baserom.gba", 0x000f0d88, 0x0000000e
	.global RisingBurst_SparkSizes
RisingBurst_SparkSizes:
	.incbin "baserom.gba", 0x000f0d96, 0x0000000e
	.section .rom.000f0e14, "ax"
	.incbin "baserom.gba", 0x000f0e14, 0x00000020
	.global SentouKouka_BitOperands
SentouKouka_BitOperands:
	.incbin "baserom.gba", 0x000f0e34, 0x00000020
	.section .rom.000f205c, "ax"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000f205c, 0x000007dd
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000f2839, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000f289c, 0x00000544
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000f2de0, 0x000002f3
	.section .rom.000f4ed0, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000f4ed0, 0x000000f4
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000f4fc4, 0x00000006
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000f4fca, 0x0000003c
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000f5006, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000f5046, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000f5086, 0x00000040
	.section .rom.000f69dc, "ax"
	.incbin "baserom.gba", 0x000f69dc, 0x00000020
	.global Data_080f53fc
Data_080f53fc:
	.incbin "baserom.gba", 0x000f69fc, 0x00000004
	.global Data_080f5400
Data_080f5400:
	.incbin "baserom.gba", 0x000f6a00, 0x00000008
	.global Data_080f5408
Data_080f5408:
	.incbin "baserom.gba", 0x000f6a08, 0x00000012
	.global Data_080f541a
Data_080f541a:
	.incbin "baserom.gba", 0x000f6a1a, 0x0000000c
	.section .rom.000f9cf8, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000f9cf8, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000f9d0c, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000f9d12, 0x00000008
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000f9d1a, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000f9d28, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000f9d36, 0x00000008
	.section .rom.000fc5a0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000fc5a0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000fc630, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000fc6e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000fc714, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000fc72c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000fc7b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000fc7c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000fc804, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000fc814, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000fc848, 0x00000030
	.section .rom.000fd304, "ax"
	.incbin "baserom.gba", 0x000fd304, 0x00000090
	.section .rom.000fd424, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000fd424, 0x00000060
	.section .rom.00320da0, "ax"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00320da0, 0x00000010
	.section .rom.003247e7, "ax"
	.incbin "baserom.gba", 0x003247e7, 0x00000001
	.section .rom.0032ae9b, "ax"
	.incbin "baserom.gba", 0x0032ae9b, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x0032ae9c, 0x000086f8
	.section .rom.003355e5, "ax"
	.incbin "baserom.gba", 0x003355e5, 0x00000003
	.section .rom.00336ef5, "ax"
	.incbin "baserom.gba", 0x00336ef5, 0x00000003
	.section .rom.0033a9f9, "ax"
	.incbin "baserom.gba", 0x0033a9f9, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x0033a9fc, 0x00000878
	.section .rom.0033f6b6, "ax"
	.incbin "baserom.gba", 0x0033f6b6, 0x00000002
	.section .rom.0034fe26, "ax"
	.incbin "baserom.gba", 0x0034fe26, 0x00000002
	.section .rom.00353416, "ax"
	.incbin "baserom.gba", 0x00353416, 0x00000002
	.section .rom.0035eeba, "ax"
	.incbin "baserom.gba", 0x0035eeba, 0x00000002
	.section .rom.0036f56a, "ax"
	.incbin "baserom.gba", 0x0036f56a, 0x00000002
	.section .rom.0037700a, "ax"
	.incbin "baserom.gba", 0x0037700a, 0x00000002
	.section .rom.0037a84a, "ax"
	.incbin "baserom.gba", 0x0037a84a, 0x00000002
	.section .rom.00383c7e, "ax"
	.incbin "baserom.gba", 0x00383c7e, 0x00000002
	.section .rom.0038f0a6, "ax"
	.incbin "baserom.gba", 0x0038f0a6, 0x00000002
	.section .rom.00396f62, "ax"
	.incbin "baserom.gba", 0x00396f62, 0x00000002
	.section .rom.0039b9f2, "ax"
	.incbin "baserom.gba", 0x0039b9f2, 0x00000002
	.section .rom.0039fca6, "ax"
	.incbin "baserom.gba", 0x0039fca6, 0x00000002
	.section .rom.003a3626, "ax"
	.incbin "baserom.gba", 0x003a3626, 0x00000002
	.section .rom.003afe62, "ax"
	.incbin "baserom.gba", 0x003afe62, 0x00000002
	.section .rom.003be7aa, "ax"
	.incbin "baserom.gba", 0x003be7aa, 0x00000002
	.section .rom.003c40ad, "ax"
	.incbin "baserom.gba", 0x003c40ad, 0x00000003
	.section .rom.003c5a2f, "ax"
	.incbin "baserom.gba", 0x003c5a2f, 0x00000001
	.section .rom.003c884d, "ax"
	.incbin "baserom.gba", 0x003c884d, 0x00000003
	.section .rom.003cc349, "ax"
	.incbin "baserom.gba", 0x003cc349, 0x00000003
	.section .rom.003cdb5f, "ax"
	.incbin "baserom.gba", 0x003cdb5f, 0x00000001
	.section .rom.003ce14a, "ax"
	.incbin "baserom.gba", 0x003ce14a, 0x00000002
	.section .rom.003ce80e, "ax"
	.incbin "baserom.gba", 0x003ce80e, 0x00000002
	.section .rom.003ceaf5, "ax"
	.incbin "baserom.gba", 0x003ceaf5, 0x00000003
	.section .rom.003cfe5b, "ax"
	.incbin "baserom.gba", 0x003cfe5b, 0x00000001
	.section .rom.003d0245, "ax"
	.incbin "baserom.gba", 0x003d0245, 0x00000003
	.section .rom.003d0617, "ax"
	.incbin "baserom.gba", 0x003d0617, 0x00000001
	.section .rom.003d0eb7, "ax"
	.incbin "baserom.gba", 0x003d0eb7, 0x00000001
	.section .rom.003d135a, "ax"
	.incbin "baserom.gba", 0x003d135a, 0x00000002
	.section .rom.003d2513, "ax"
	.incbin "baserom.gba", 0x003d2513, 0x00000001
	.section .rom.003d39d6, "ax"
	.incbin "baserom.gba", 0x003d39d6, 0x00000002
	.section .rom.003d499f, "ax"
	.incbin "baserom.gba", 0x003d499f, 0x00000001
	.section .rom.003d4bfa, "ax"
	.incbin "baserom.gba", 0x003d4bfa, 0x00000002
	.section .rom.003d6655, "ax"
	.incbin "baserom.gba", 0x003d6655, 0x00000003
	.section .rom.003d6ba1, "ax"
	.incbin "baserom.gba", 0x003d6ba1, 0x00000003
	.section .rom.003d892a, "ax"
	.incbin "baserom.gba", 0x003d892a, 0x00000002
	.section .rom.003d8ba3, "ax"
	.incbin "baserom.gba", 0x003d8ba3, 0x00000001
	.section .rom.003d906a, "ax"
	.incbin "baserom.gba", 0x003d906a, 0x00000002
	.section .rom.003dac3f, "ax"
	.incbin "baserom.gba", 0x003dac3f, 0x00000001
	.section .rom.003dc83d, "ax"
	.incbin "baserom.gba", 0x003dc83d, 0x00000003
	.section .rom.003dca5e, "ax"
	.incbin "baserom.gba", 0x003dca5e, 0x00000002
	.section .rom.003dce9b, "ax"
	.incbin "baserom.gba", 0x003dce9b, 0x00000001
	.section .rom.003dcfad, "ax"
	.incbin "baserom.gba", 0x003dcfad, 0x00000003
	.section .rom.003dd72b, "ax"
	.incbin "baserom.gba", 0x003dd72b, 0x00000001
	.section .rom.003ddbc9, "ax"
	.incbin "baserom.gba", 0x003ddbc9, 0x00000003
	.section .rom.003de8de, "ax"
	.incbin "baserom.gba", 0x003de8de, 0x00000002
	.section .rom.003df65e, "ax"
	.incbin "baserom.gba", 0x003df65e, 0x00000002
	.section .rom.003df9ef, "ax"
	.incbin "baserom.gba", 0x003df9ef, 0x00000001
	.section .rom.003e1736, "ax"
	.incbin "baserom.gba", 0x003e1736, 0x00000002
	.section .rom.003e250d, "ax"
	.incbin "baserom.gba", 0x003e250d, 0x00000003
	.section .rom.003e2d3a, "ax"
	.incbin "baserom.gba", 0x003e2d3a, 0x00000002
	.section .rom.003e33c5, "ax"
	.incbin "baserom.gba", 0x003e33c5, 0x00000003
	.section .rom.003e3583, "ax"
	.incbin "baserom.gba", 0x003e3583, 0x00000001
	.section .rom.003e42db, "ax"
	.incbin "baserom.gba", 0x003e42db, 0x00000001
	.section .rom.003e4827, "ax"
	.incbin "baserom.gba", 0x003e4827, 0x00000001
	.section .rom.003e4c01, "ax"
	.incbin "baserom.gba", 0x003e4c01, 0x00000003
	.section .rom.003e4faf, "ax"
	.incbin "baserom.gba", 0x003e4faf, 0x00000001
	.section .rom.003e6f53, "ax"
	.incbin "baserom.gba", 0x003e6f53, 0x00000001
	.section .rom.003e7cef, "ax"
	.incbin "baserom.gba", 0x003e7cef, 0x00000001
	.section .rom.003e7f0b, "ax"
	.incbin "baserom.gba", 0x003e7f0b, 0x00000001
	.section .rom.003e8207, "ax"
	.incbin "baserom.gba", 0x003e8207, 0x00000001
	.section .rom.003ea3b5, "ax"
	.incbin "baserom.gba", 0x003ea3b5, 0x00000003
	.section .rom.003eb183, "ax"
	.incbin "baserom.gba", 0x003eb183, 0x00000001
	.section .rom.003ec2e5, "ax"
	.incbin "baserom.gba", 0x003ec2e5, 0x00000003
	.section .rom.003ec83f, "ax"
	.incbin "baserom.gba", 0x003ec83f, 0x00000001
	.section .rom.003ee129, "ax"
	.incbin "baserom.gba", 0x003ee129, 0x00000003
	.section .rom.003ee9ca, "ax"
	.incbin "baserom.gba", 0x003ee9ca, 0x00000002
	.section .rom.003eeda7, "ax"
	.incbin "baserom.gba", 0x003eeda7, 0x00000001
	.section .rom.003ef031, "ax"
	.incbin "baserom.gba", 0x003ef031, 0x00000003
	.section .rom.003ef3d7, "ax"
	.incbin "baserom.gba", 0x003ef3d7, 0x00000001
	.section .rom.003ef632, "ax"
	.incbin "baserom.gba", 0x003ef632, 0x00000002
	.section .rom.003ef9ea, "ax"
	.incbin "baserom.gba", 0x003ef9ea, 0x00000002
	.section .rom.003f0ed3, "ax"
	.incbin "baserom.gba", 0x003f0ed3, 0x00000001
	.section .rom.003f2496, "ax"
	.incbin "baserom.gba", 0x003f2496, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003f2498, 0x00000a6c
	.section .rom.003f3cda, "ax"
	.incbin "baserom.gba", 0x003f3cda, 0x00000002
	.section .rom.003f405d, "ax"
	.incbin "baserom.gba", 0x003f405d, 0x00000003
	.section .rom.003f4d0d, "ax"
	.incbin "baserom.gba", 0x003f4d0d, 0x00000003
	.section .rom.003f5855, "ax"
	.incbin "baserom.gba", 0x003f5855, 0x00000003
	.section .rom.003f59ee, "ax"
	.incbin "baserom.gba", 0x003f59ee, 0x00000002
	.section .rom.003f627b, "ax"
	.incbin "baserom.gba", 0x003f627b, 0x00000001
	.section .rom.003f6d42, "ax"
	.incbin "baserom.gba", 0x003f6d42, 0x00000002
	.section .rom.003f725a, "ax"
	.incbin "baserom.gba", 0x003f725a, 0x00000002
	.section .rom.003f787e, "ax"
	.incbin "baserom.gba", 0x003f787e, 0x00000002
	.section .rom.003f80e5, "ax"
	.incbin "baserom.gba", 0x003f80e5, 0x00000003
	.section .rom.003f8509, "ax"
	.incbin "baserom.gba", 0x003f8509, 0x00000003
	.section .rom.003f87a3, "ax"
	.incbin "baserom.gba", 0x003f87a3, 0x00000001
	.section .rom.003fa13f, "ax"
	.incbin "baserom.gba", 0x003fa13f, 0x00000001
	.section .rom.003fbeb6, "ax"
	.incbin "baserom.gba", 0x003fbeb6, 0x00000002
	.section .rom.003fde2b, "ax"
	.incbin "baserom.gba", 0x003fde2b, 0x00000001
	.section .rom.003fe2fb, "ax"
	.incbin "baserom.gba", 0x003fe2fb, 0x00000001
	.section .rom.003fe98d, "ax"
	.incbin "baserom.gba", 0x003fe98d, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003fe990, 0x00000a34
	.section .rom.00400795, "ax"
	.incbin "baserom.gba", 0x00400795, 0x00000003
	.section .rom.00401266, "ax"
	.incbin "baserom.gba", 0x00401266, 0x00000002
	.section .rom.00401da3, "ax"
	.incbin "baserom.gba", 0x00401da3, 0x00000001
	.section .rom.004023e3, "ax"
	.incbin "baserom.gba", 0x004023e3, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x004023e4, 0x00001588
	.section .rom.004039cd, "ax"
	.incbin "baserom.gba", 0x004039cd, 0x00000003
	.section .rom.00403d3b, "ax"
	.incbin "baserom.gba", 0x00403d3b, 0x00000001
	.section .rom.004042d9, "ax"
	.incbin "baserom.gba", 0x004042d9, 0x00000003
	.section .rom.0040460d, "ax"
	.incbin "baserom.gba", 0x0040460d, 0x00000003
	.section .rom.00404949, "ax"
	.incbin "baserom.gba", 0x00404949, 0x00000003
	.section .rom.00405962, "ax"
	.incbin "baserom.gba", 0x00405962, 0x00000002
	.section .rom.00408f6e, "ax"
	.incbin "baserom.gba", 0x00408f6e, 0x00000002
	.section .rom.00409711, "ax"
	.incbin "baserom.gba", 0x00409711, 0x00000003
	.section .rom.0040aaa3, "ax"
	.incbin "baserom.gba", 0x0040aaa3, 0x00000001
	.section .rom.0040aed3, "ax"
	.incbin "baserom.gba", 0x0040aed3, 0x00000001
	.section .rom.0040ccc9, "ax"
	.incbin "baserom.gba", 0x0040ccc9, 0x00000003
	.section .rom.0040d5de, "ax"
	.incbin "baserom.gba", 0x0040d5de, 0x00000002
	.section .rom.0040f112, "ax"
	.incbin "baserom.gba", 0x0040f112, 0x00000002
	.section .rom.00410161, "ax"
	.incbin "baserom.gba", 0x00410161, 0x00000003
	.section .rom.00410817, "ax"
	.incbin "baserom.gba", 0x00410817, 0x00000001
	.section .rom.004118bd, "ax"
	.incbin "baserom.gba", 0x004118bd, 0x00000003
	.section .rom.00424ccf, "ax"
	.incbin "baserom.gba", 0x00424ccf, 0x00000001
	.section .rom.00424e21, "ax"
	.incbin "baserom.gba", 0x00424e21, 0x00000003
	.section .rom.004252b2, "ax"
	.incbin "baserom.gba", 0x004252b2, 0x00000002
	.section .rom.004254a9, "ax"
	.incbin "baserom.gba", 0x004254a9, 0x00000003
	.section .rom.00426b66, "ax"
	.incbin "baserom.gba", 0x00426b66, 0x00000002
	.section .rom.00428085, "ax"
	.incbin "baserom.gba", 0x00428085, 0x00000003
	.section .rom.00428c51, "ax"
	.incbin "baserom.gba", 0x00428c51, 0x00000003
	.section .rom.004297c6, "ax"
	.incbin "baserom.gba", 0x004297c6, 0x00000002
	.section .rom.0042b66f, "ax"
	.incbin "baserom.gba", 0x0042b66f, 0x00000001
	.section .rom.0042cdfd, "ax"
	.incbin "baserom.gba", 0x0042cdfd, 0x00000003
	.section .rom.0042e2ae, "ax"
	.incbin "baserom.gba", 0x0042e2ae, 0x00000002
	.section .rom.004309fb, "ax"
	.incbin "baserom.gba", 0x004309fb, 0x00000001
	.section .rom.00432295, "ax"
	.incbin "baserom.gba", 0x00432295, 0x00000003
	.section .rom.0043385e, "ax"
	.incbin "baserom.gba", 0x0043385e, 0x00000002
	.section .rom.004352ee, "ax"
	.incbin "baserom.gba", 0x004352ee, 0x00000002
	.section .rom.004360ea, "ax"
	.incbin "baserom.gba", 0x004360ea, 0x00000002
	.section .rom.0043fa5a, "ax"
	.incbin "baserom.gba", 0x0043fa5a, 0x00000002
	.section .rom.00441c0e, "ax"
	.incbin "baserom.gba", 0x00441c0e, 0x00000002
	.section .rom.00445d75, "ax"
	.incbin "baserom.gba", 0x00445d75, 0x00000003
	.section .rom.00448f9a, "ax"
	.incbin "baserom.gba", 0x00448f9a, 0x00000002
	.section .rom.0044a69e, "ax"
	.incbin "baserom.gba", 0x0044a69e, 0x00000002
	.section .rom.00451286, "ax"
	.incbin "baserom.gba", 0x00451286, 0x00000002
	.section .rom.00459c0a, "ax"
	.incbin "baserom.gba", 0x00459c0a, 0x00000002
	.section .rom.004616e6, "ax"
	.incbin "baserom.gba", 0x004616e6, 0x00000002
	.section .rom.0046477a, "ax"
	.incbin "baserom.gba", 0x0046477a, 0x00000002
	.section .rom.00467b89, "ax"
	.incbin "baserom.gba", 0x00467b89, 0x00000003
	.section .rom.0046a3f5, "ax"
	.incbin "baserom.gba", 0x0046a3f5, 0x00000003
	.section .rom.0046bee6, "ax"
	.incbin "baserom.gba", 0x0046bee6, 0x00000002
	.section .rom.0046ccfe, "ax"
	.incbin "baserom.gba", 0x0046ccfe, 0x00000002
	.section .rom.0046d9e9, "ax"
	.incbin "baserom.gba", 0x0046d9e9, 0x00000003
	.section .rom.00476ea6, "ax"
	.incbin "baserom.gba", 0x00476ea6, 0x00000002
	.section .rom.00477bb1, "ax"
	.incbin "baserom.gba", 0x00477bb1, 0x00000003
	.section .rom.00479cc5, "ax"
	.incbin "baserom.gba", 0x00479cc5, 0x00000003
	.section .rom.0047a937, "ax"
	.incbin "baserom.gba", 0x0047a937, 0x00000001
	.section .rom.0047b54f, "ax"
	.incbin "baserom.gba", 0x0047b54f, 0x00000001
	.section .rom.0047bcc3, "ax"
	.incbin "baserom.gba", 0x0047bcc3, 0x00000001
	.section .rom.0047d553, "ax"
	.incbin "baserom.gba", 0x0047d553, 0x00000001
	.section .rom.0047df21, "ax"
	.incbin "baserom.gba", 0x0047df21, 0x00000003
	.section .rom.0047eb3f, "ax"
	.incbin "baserom.gba", 0x0047eb3f, 0x00000001
	.section .rom.004811f3, "ax"
	.incbin "baserom.gba", 0x004811f3, 0x00000001
	.section .rom.00483906, "ax"
	.incbin "baserom.gba", 0x00483906, 0x00000002
	.section .rom.0048885b, "ax"
	.incbin "baserom.gba", 0x0048885b, 0x00000001
	.section .rom.0048cb85, "ax"
	.incbin "baserom.gba", 0x0048cb85, 0x00000003
	.section .rom.0048d772, "ax"
	.incbin "baserom.gba", 0x0048d772, 0x00000002
	.section .rom.00492327, "ax"
	.incbin "baserom.gba", 0x00492327, 0x00000001
	.section .rom.00494691, "ax"
	.incbin "baserom.gba", 0x00494691, 0x00000003
	.section .rom.00496033, "ax"
	.incbin "baserom.gba", 0x00496033, 0x00000001
	.section .rom.0049a8f5, "ax"
	.incbin "baserom.gba", 0x0049a8f5, 0x00000003
	.section .rom.0049e9bd, "ax"
	.incbin "baserom.gba", 0x0049e9bd, 0x00000003
	.section .rom.004a2086, "ax"
	.incbin "baserom.gba", 0x004a2086, 0x00000002
	.section .rom.004aa057, "ax"
	.incbin "baserom.gba", 0x004aa057, 0x00000001
	.section .rom.004b1367, "ax"
	.incbin "baserom.gba", 0x004b1367, 0x00000001
	.section .rom.004b62e7, "ax"
	.incbin "baserom.gba", 0x004b62e7, 0x00000001
	.section .rom.004bac97, "ax"
	.incbin "baserom.gba", 0x004bac97, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004bac98, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004baca4, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004badf4, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004baf34, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004bb074, 0x00000140
	.section .rom.004c05c3, "ax"
	.incbin "baserom.gba", 0x004c05c3, 0x00000001
	.section .rom.004c0751, "ax"
	.incbin "baserom.gba", 0x004c0751, 0x00000003
	.section .rom.004c57b1, "ax"
	.incbin "baserom.gba", 0x004c57b1, 0x00000003
	.section .rom.004c9d1d, "ax"
	.incbin "baserom.gba", 0x004c9d1d, 0x00000003
	.section .rom.004ceee5, "ax"
	.incbin "baserom.gba", 0x004ceee5, 0x00000003
	.section .rom.004cf07b, "ax"
	.incbin "baserom.gba", 0x004cf07b, 0x00000001
	.section .rom.004d1ccf, "ax"
	.incbin "baserom.gba", 0x004d1ccf, 0x00000001
	.section .rom.004d8983, "ax"
	.incbin "baserom.gba", 0x004d8983, 0x00000001
	.section .rom.004dbabe, "ax"
	.incbin "baserom.gba", 0x004dbabe, 0x00000002
	.section .rom.004dbc3f, "ax"
	.incbin "baserom.gba", 0x004dbc3f, 0x00000001
	.section .rom.004de3f9, "ax"
	.incbin "baserom.gba", 0x004de3f9, 0x00000003
	.section .rom.004e0a42, "ax"
	.incbin "baserom.gba", 0x004e0a42, 0x00000002
	.section .rom.004e3126, "ax"
	.incbin "baserom.gba", 0x004e3126, 0x00000002
	.section .rom.004e419b, "ax"
	.incbin "baserom.gba", 0x004e419b, 0x00000001
	.section .rom.004e62ee, "ax"
	.incbin "baserom.gba", 0x004e62ee, 0x00000002
	.section .rom.004e6481, "ax"
	.incbin "baserom.gba", 0x004e6481, 0x00000003
	.section .rom.004e8cc3, "ax"
	.incbin "baserom.gba", 0x004e8cc3, 0x00000001
	.section .rom.004eaad9, "ax"
	.incbin "baserom.gba", 0x004eaad9, 0x00000003
	.section .rom.004ed1ba, "ax"
	.incbin "baserom.gba", 0x004ed1ba, 0x00000002
	.section .rom.004ee301, "ax"
	.incbin "baserom.gba", 0x004ee301, 0x00000003
	.section .rom.004f1415, "ax"
	.incbin "baserom.gba", 0x004f1415, 0x00000003
	.section .rom.004f557d, "ax"
	.incbin "baserom.gba", 0x004f557d, 0x00000003
	.section .rom.004f6b7a, "ax"
	.incbin "baserom.gba", 0x004f6b7a, 0x00000002
	.section .rom.004f7e43, "ax"
	.incbin "baserom.gba", 0x004f7e43, 0x00000001
	.section .rom.004fb25b, "ax"
	.incbin "baserom.gba", 0x004fb25b, 0x00000001
	.section .rom.004fb3e9, "ax"
	.incbin "baserom.gba", 0x004fb3e9, 0x00000003
	.section .rom.004fd19a, "ax"
	.incbin "baserom.gba", 0x004fd19a, 0x00000002
	.section .rom.00500d11, "ax"
	.incbin "baserom.gba", 0x00500d11, 0x00000003
	.section .rom.0050221f, "ax"
	.incbin "baserom.gba", 0x0050221f, 0x00000001
	.section .rom.00502df3, "ax"
	.incbin "baserom.gba", 0x00502df3, 0x00000001
	.section .rom.00502ef1, "ax"
	.incbin "baserom.gba", 0x00502ef1, 0x00000003
	.section .rom.005042d6, "ax"
	.incbin "baserom.gba", 0x005042d6, 0x00000002
	.section .rom.00505459, "ax"
	.incbin "baserom.gba", 0x00505459, 0x00000003
	.section .rom.00505de2, "ax"
	.incbin "baserom.gba", 0x00505de2, 0x00000002
	.section .rom.00508a5f, "ax"
	.incbin "baserom.gba", 0x00508a5f, 0x00000001
	.section .rom.00508b42, "ax"
	.incbin "baserom.gba", 0x00508b42, 0x00000002
	.section .rom.00509d2d, "ax"
	.incbin "baserom.gba", 0x00509d2d, 0x00000003
	.section .rom.0050b9b5, "ax"
	.incbin "baserom.gba", 0x0050b9b5, 0x00000003
	.section .rom.0050bec3, "ax"
	.incbin "baserom.gba", 0x0050bec3, 0x00000001
	.section .rom.0050c003, "ax"
	.incbin "baserom.gba", 0x0050c003, 0x00000001
	.section .rom.0050d21e, "ax"
	.incbin "baserom.gba", 0x0050d21e, 0x00000002
	.section .rom.0050d379, "ax"
	.incbin "baserom.gba", 0x0050d379, 0x00000003
	.section .rom.0051179f, "ax"
	.incbin "baserom.gba", 0x0051179f, 0x00000001
	.section .rom.005134ce, "ax"
	.incbin "baserom.gba", 0x005134ce, 0x00000002
	.section .rom.00514247, "ax"
	.incbin "baserom.gba", 0x00514247, 0x00000001
	.section .rom.0051437b, "ax"
	.incbin "baserom.gba", 0x0051437b, 0x00000001
	.section .rom.005167fb, "ax"
	.incbin "baserom.gba", 0x005167fb, 0x00000001
	.section .rom.00517577, "ax"
	.incbin "baserom.gba", 0x00517577, 0x00000001
	.section .rom.00518056, "ax"
	.incbin "baserom.gba", 0x00518056, 0x00000002
	.section .rom.00518ece, "ax"
	.incbin "baserom.gba", 0x00518ece, 0x00000002
	.section .rom.0051afa2, "ax"
	.incbin "baserom.gba", 0x0051afa2, 0x00000002
	.section .rom.0051b8e9, "ax"
	.incbin "baserom.gba", 0x0051b8e9, 0x00000003
	.section .rom.0051c536, "ax"
	.incbin "baserom.gba", 0x0051c536, 0x00000002
	.section .rom.0051c75d, "ax"
	.incbin "baserom.gba", 0x0051c75d, 0x00000003
	.section .rom.0051e09f, "ax"
	.incbin "baserom.gba", 0x0051e09f, 0x00000001
	.section .rom.0051e1b3, "ax"
	.incbin "baserom.gba", 0x0051e1b3, 0x00000001
	.section .rom.0051fb79, "ax"
	.incbin "baserom.gba", 0x0051fb79, 0x00000003
	.section .rom.0051fcc3, "ax"
	.incbin "baserom.gba", 0x0051fcc3, 0x00000001
	.section .rom.00521d8f, "ax"
	.incbin "baserom.gba", 0x00521d8f, 0x00000001
	.section .rom.00521eb7, "ax"
	.incbin "baserom.gba", 0x00521eb7, 0x00000001
	.section .rom.0052373f, "ax"
	.incbin "baserom.gba", 0x0052373f, 0x00000001
	.section .rom.005259e1, "ax"
	.incbin "baserom.gba", 0x005259e1, 0x00000003
	.section .rom.00525b23, "ax"
	.incbin "baserom.gba", 0x00525b23, 0x00000001
	.section .rom.00527b2b, "ax"
	.incbin "baserom.gba", 0x00527b2b, 0x00000001
	.section .rom.00527c36, "ax"
	.incbin "baserom.gba", 0x00527c36, 0x00000002
	.section .rom.00529c1a, "ax"
	.incbin "baserom.gba", 0x00529c1a, 0x00000002
	.section .rom.0052b33f, "ax"
	.incbin "baserom.gba", 0x0052b33f, 0x00000001
	.section .rom.0052c3f3, "ax"
	.incbin "baserom.gba", 0x0052c3f3, 0x00000001
	.section .rom.0052e0da, "ax"
	.incbin "baserom.gba", 0x0052e0da, 0x00000002
	.section .rom.0052e235, "ax"
	.incbin "baserom.gba", 0x0052e235, 0x00000003
	.section .rom.0053017e, "ax"
	.incbin "baserom.gba", 0x0053017e, 0x00000002
	.section .rom.00530303, "ax"
	.incbin "baserom.gba", 0x00530303, 0x00000001
	.section .rom.00532e55, "ax"
	.incbin "baserom.gba", 0x00532e55, 0x00000003
	.section .rom.0053551a, "ax"
	.incbin "baserom.gba", 0x0053551a, 0x00000002
	.section .rom.00537c72, "ax"
	.incbin "baserom.gba", 0x00537c72, 0x00000002
	.section .rom.005394c5, "ax"
	.incbin "baserom.gba", 0x005394c5, 0x00000003
	.section .rom.0053bbc5, "ax"
	.incbin "baserom.gba", 0x0053bbc5, 0x00000003
	.section .rom.0053d991, "ax"
	.incbin "baserom.gba", 0x0053d991, 0x00000003
	.section .rom.0053fedb, "ax"
	.incbin "baserom.gba", 0x0053fedb, 0x00000001
	.section .rom.005411a6, "ax"
	.incbin "baserom.gba", 0x005411a6, 0x00000002
	.section .rom.00541e57, "ax"
	.incbin "baserom.gba", 0x00541e57, 0x00000001
	.section .rom.005428c1, "ax"
	.incbin "baserom.gba", 0x005428c1, 0x00000003
	.section .rom.005429b7, "ax"
	.incbin "baserom.gba", 0x005429b7, 0x00000001
	.section .rom.00544416, "ax"
	.incbin "baserom.gba", 0x00544416, 0x00000002
	.section .rom.005451ba, "ax"
	.incbin "baserom.gba", 0x005451ba, 0x00000002
	.section .rom.00546cf3, "ax"
	.incbin "baserom.gba", 0x00546cf3, 0x00000001
	.section .rom.00549be1, "ax"
	.incbin "baserom.gba", 0x00549be1, 0x00000003
	.section .rom.005513c7, "ax"
	.incbin "baserom.gba", 0x005513c7, 0x00000001
	.section .rom.00551493, "ax"
	.incbin "baserom.gba", 0x00551493, 0x00000001
	.section .rom.00556d3e, "ax"
	.incbin "baserom.gba", 0x00556d3e, 0x00000002
	.section .rom.00556e7f, "ax"
	.incbin "baserom.gba", 0x00556e7f, 0x00000001
	.section .rom.005582ad, "ax"
	.incbin "baserom.gba", 0x005582ad, 0x00000003
	.section .rom.0055afea, "ax"
	.incbin "baserom.gba", 0x0055afea, 0x00000002
	.section .rom.0055d0e6, "ax"
	.incbin "baserom.gba", 0x0055d0e6, 0x00000002
	.section .rom.00564aee, "ax"
	.incbin "baserom.gba", 0x00564aee, 0x00000002
	.section .rom.00564c8a, "ax"
	.incbin "baserom.gba", 0x00564c8a, 0x00000002
	.section .rom.00569525, "ax"
	.incbin "baserom.gba", 0x00569525, 0x00000003
	.section .rom.0056ba26, "ax"
	.incbin "baserom.gba", 0x0056ba26, 0x00000002
	.section .rom.00573417, "ax"
	.incbin "baserom.gba", 0x00573417, 0x00000001
	.section .rom.005735b2, "ax"
	.incbin "baserom.gba", 0x005735b2, 0x00000002
	.section .rom.00576161, "ax"
	.incbin "baserom.gba", 0x00576161, 0x00000003
	.section .rom.00578235, "ax"
	.incbin "baserom.gba", 0x00578235, 0x00000003
	.section .rom.0057a28e, "ax"
	.incbin "baserom.gba", 0x0057a28e, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057a290, 0x00001cec
	.section .rom.0057d725, "ax"
	.incbin "baserom.gba", 0x0057d725, 0x00000003
	.section .rom.00580401, "ax"
	.incbin "baserom.gba", 0x00580401, 0x00000003
	.section .rom.005821d3, "ax"
	.incbin "baserom.gba", 0x005821d3, 0x00000001
	.section .rom.00582313, "ax"
	.incbin "baserom.gba", 0x00582313, 0x00000001
	.section .rom.0058536d, "ax"
	.incbin "baserom.gba", 0x0058536d, 0x00000003
	.section .rom.005882ea, "ax"
	.incbin "baserom.gba", 0x005882ea, 0x00000002
	.section .rom.0058abb1, "ax"
	.incbin "baserom.gba", 0x0058abb1, 0x00000003
	.section .rom.00591b19, "ax"
	.incbin "baserom.gba", 0x00591b19, 0x00000003
	.section .rom.005933bf, "ax"
	.incbin "baserom.gba", 0x005933bf, 0x00000001
	.section .rom.00593cbe, "ax"
	.incbin "baserom.gba", 0x00593cbe, 0x00000002
	.section .rom.00594a37, "ax"
	.incbin "baserom.gba", 0x00594a37, 0x00000001
	.section .rom.00594b97, "ax"
	.incbin "baserom.gba", 0x00594b97, 0x00000001
	.section .rom.005967ef, "ax"
	.incbin "baserom.gba", 0x005967ef, 0x00000001
	.section .rom.0059808b, "ax"
	.incbin "baserom.gba", 0x0059808b, 0x00000001
	.section .rom.0059820f, "ax"
	.incbin "baserom.gba", 0x0059820f, 0x00000001
	.section .rom.0059ad4b, "ax"
	.incbin "baserom.gba", 0x0059ad4b, 0x00000001
	.section .rom.0059d4c5, "ax"
	.incbin "baserom.gba", 0x0059d4c5, 0x00000003
	.section .rom.0059e4b2, "ax"
	.incbin "baserom.gba", 0x0059e4b2, 0x00000002
	.section .rom.0059fb7f, "ax"
	.incbin "baserom.gba", 0x0059fb7f, 0x00000001
	.section .rom.0059fd29, "ax"
	.incbin "baserom.gba", 0x0059fd29, 0x00000003
	.section .rom.005a2abd, "ax"
	.incbin "baserom.gba", 0x005a2abd, 0x00000003
	.section .rom.005a6b52, "ax"
	.incbin "baserom.gba", 0x005a6b52, 0x00000002
	.section .rom.005a6c93, "ax"
	.incbin "baserom.gba", 0x005a6c93, 0x00000001
	.section .rom.005a8b5b, "ax"
	.incbin "baserom.gba", 0x005a8b5b, 0x00000001
	.section .rom.005a8cfd, "ax"
	.incbin "baserom.gba", 0x005a8cfd, 0x00000003
	.section .rom.005aad9f, "ax"
	.incbin "baserom.gba", 0x005aad9f, 0x00000001
	.section .rom.005abd2c, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005abd2c, 0x000022d8
	.section .rom.005b0af5, "ax"
	.incbin "baserom.gba", 0x005b0af5, 0x00000003
	.section .rom.005b0c7a, "ax"
	.incbin "baserom.gba", 0x005b0c7a, 0x00000002
	.section .rom.005b83c7, "ax"
	.incbin "baserom.gba", 0x005b83c7, 0x00000001
	.section .rom.005b88d1, "ax"
	.incbin "baserom.gba", 0x005b88d1, 0x00000003
	.section .rom.005b9fa9, "ax"
	.incbin "baserom.gba", 0x005b9fa9, 0x00000003
	.section .rom.005ba14d, "ax"
	.incbin "baserom.gba", 0x005ba14d, 0x00000003
	.section .rom.005bba52, "ax"
	.incbin "baserom.gba", 0x005bba52, 0x00000002
	.section .rom.005bd27f, "ax"
	.incbin "baserom.gba", 0x005bd27f, 0x00000001
	.section .rom.005bd3fd, "ax"
	.incbin "baserom.gba", 0x005bd3fd, 0x00000003
	.section .rom.005c1cf1, "ax"
	.incbin "baserom.gba", 0x005c1cf1, 0x00000003
	.section .rom.005c2d36, "ax"
	.incbin "baserom.gba", 0x005c2d36, 0x00000002
	.section .rom.005c3b5a, "ax"
	.incbin "baserom.gba", 0x005c3b5a, 0x00000002
	.section .rom.005d783e, "ax"
	.incbin "baserom.gba", 0x005d783e, 0x00000002
	.section .rom.005edc56, "ax"
	.incbin "baserom.gba", 0x005edc56, 0x00000002
	.section .rom.00609718, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00609718, 0x00002904
	.section .rom.0069132d, "ax"
	.incbin "baserom.gba", 0x0069132d, 0x00000003
	.section .rom.006d5aed, "ax"
	.incbin "baserom.gba", 0x006d5aed, 0x00000003
	.section .rom.00717db0, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00717db0, 0x00001164
	.section .rom.00745b17, "ax"
	.incbin "baserom.gba", 0x00745b17, 0x00000001
	.section .rom.00755d90, "ax"
	.global Tileset_Palette47
Tileset_Palette47:
	.incbin "baserom.gba", 0x00755d90, 0x000000e4
	.section .rom.0075c010, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x0075c010, 0x00004604
