	.section .rom.00007a68, "ax"
	.incbin "baserom.gba", 0x00007a68, 0x00000024
	.section .rom.00007b38, "ax"
	.incbin "baserom.gba", 0x00007b38, 0x00000004
	.section .rom.0000caca, "ax"
	.incbin "baserom.gba", 0x0000caca, 0x00000002
	.section .rom.00029f3c, "ax"
	.global UiIcon_ItemIconPointers
UiIcon_ItemIconPointers:
	.incbin "baserom.gba", 0x00029f3c, 0x000003fc
	.global UiIcon_ItemIconPointersEnd
UiIcon_ItemIconPointersEnd:
	.incbin "baserom.gba", 0x0002a338, 0x00003b9c
	.global UiIcon_PsynergyIconPointers
UiIcon_PsynergyIconPointers:
	.incbin "baserom.gba", 0x0002ded4, 0x00000280
	.global UiIcon_PsynergyIconPointersEnd
UiIcon_PsynergyIconPointersEnd:
	.incbin "baserom.gba", 0x0002e154, 0x00002798
	.global UiIcon_MiscIconPointers
UiIcon_MiscIconPointers:
	.incbin "baserom.gba", 0x000308ec, 0x00000800
	.section .rom.0006c1ea, "ax"
	.incbin "baserom.gba", 0x0006c1ea, 0x00000002
	.global Menu_PartySpriteResourceIds
Menu_PartySpriteResourceIds:
	.incbin "baserom.gba", 0x0006c1ec, 0x00000010
	.global NameEntry_KeyboardTilemap
NameEntry_KeyboardTilemap:
	.incbin "baserom.gba", 0x0006c1fc, 0x0000015a
	.global NameEntry_AlternateKeyboardTilemap
NameEntry_AlternateKeyboardTilemap:
	.incbin "baserom.gba", 0x0006c356, 0x0000023e
	.global Ui_FixedTileBlocks
Ui_FixedTileBlocks:
	.incbin "baserom.gba", 0x0006c594, 0x00000004
	.section .rom.00094164, "ax"
	.global Battle_LocationRules
Battle_LocationRules:
	.incbin "baserom.gba", 0x00094164, 0x00000638
	.section .rom.000949e4, "ax"
	.global Scene_InteractionRuleTable
Scene_InteractionRuleTable:
	.incbin "baserom.gba", 0x000949e4, 0x000003e8
	.global Scene_AreaNameRules
Scene_AreaNameRules:
	.incbin "baserom.gba", 0x00094dcc, 0x000003f0
	.global Party_PairResolveRules
Party_PairResolveRules:
	.incbin "baserom.gba", 0x000951bc, 0x00000098
	.global RomWords_0809e270
RomWords_0809e270:
	.incbin "baserom.gba", 0x00095254, 0x00000218
	.section .rom.000954b2, "ax"
	.global Debug_PaletteSwatchTiles
Debug_PaletteSwatchTiles:
	.incbin "baserom.gba", 0x000954b2, 0x000001b2
	.section .rom.000ab124, "ax"
	.global RomBytes_080b4100
RomBytes_080b4100:
	.incbin "baserom.gba", 0x000ab124, 0x0000003c
	.global RomBytes_080b413c
RomBytes_080b413c:
	.incbin "baserom.gba", 0x000ab160, 0x0000000a
	.global Shop_SpecialItemPrices
Shop_SpecialItemPrices:
	.incbin "baserom.gba", 0x000ab16a, 0x00000066
	.global EventTable_AbilityLoadouts
EventTable_AbilityLoadouts:
	.incbin "baserom.gba", 0x000ab1d0, 0x00000906
	.global Data_080b4ab2
Data_080b4ab2:
	.incbin "baserom.gba", 0x000abad6, 0x00000004
	.global Inn_PriceMultipliers
Inn_PriceMultipliers:
	.incbin "baserom.gba", 0x000abada, 0x0000000c
	.section .rom.000ac204, "ax"
	.incbin "baserom.gba", 0x000ac204, 0x00000008
	.section .rom.000ac218, "ax"
	.incbin "baserom.gba", 0x000ac218, 0x00000040
	.section .rom.000b9a0a, "ax"
	.incbin "baserom.gba", 0x000b9a0a, 0x00000006
	.global BattleParty_CenterOrderOffsets
BattleParty_CenterOrderOffsets:
	.incbin "baserom.gba", 0x000b9a10, 0x0000000c
	.global RomBytes_080c2a1c
RomBytes_080c2a1c:
	.incbin "baserom.gba", 0x000b9a1c, 0x0000000e
	.global BattleUnit_WeaponAnimsClass1
BattleUnit_WeaponAnimsClass1:
	.incbin "baserom.gba", 0x000b9a2a, 0x0000000e
	.global BattleUnit_WeaponAnimsClass2
BattleUnit_WeaponAnimsClass2:
	.incbin "baserom.gba", 0x000b9a38, 0x0000000e
	.global BattleUnit_WeaponAnimsClass3
BattleUnit_WeaponAnimsClass3:
	.incbin "baserom.gba", 0x000b9a46, 0x0000000e
	.global BattleUnit_WeaponAnimsClass5
BattleUnit_WeaponAnimsClass5:
	.incbin "baserom.gba", 0x000b9a54, 0x0000000e
	.global BattlePlacement_StepPairs
BattlePlacement_StepPairs:
	.incbin "baserom.gba", 0x000b9a62, 0x0000001a
	.global Camera_FlagTransformWork
Camera_FlagTransformWork:
	.incbin "baserom.gba", 0x000b9a7c, 0x0000003c
	.global HitTbl
HitTbl:
	.incbin "baserom.gba", 0x000b9ab8, 0x00000008
	.global EpLossTbl
EpLossTbl:
	.incbin "baserom.gba", 0x000b9ac0, 0x00000018
	.global HpRcvTbl
HpRcvTbl:
	.incbin "baserom.gba", 0x000b9ad8, 0x00000018
	.global EpDmgTbl
EpDmgTbl:
	.incbin "baserom.gba", 0x000b9af0, 0x00000018
	.global HpDmgTbl5
HpDmgTbl5:
	.incbin "baserom.gba", 0x000b9b08, 0x00000018
	.global HpDmgTbl8
HpDmgTbl8:
	.incbin "baserom.gba", 0x000b9b20, 0x00000018
	.global HpDmgTbl6
HpDmgTbl6:
	.incbin "baserom.gba", 0x000b9b38, 0x00000018
	.global EpRcvTbl
EpRcvTbl:
	.incbin "baserom.gba", 0x000b9b50, 0x00000018
	.global HpDmgTbl
HpDmgTbl:
	.incbin "baserom.gba", 0x000b9b68, 0x00000018
	.global BattleCommand_SelectionWeights
BattleCommand_SelectionWeights:
	.incbin "baserom.gba", 0x000b9b80, 0x00000018
	.global Battle_ActionStatus
Battle_ActionStatus:
	.incbin "baserom.gba", 0x000b9b98, 0x00000208
	.global Battle_ActionFlags
Battle_ActionFlags:
	.incbin "baserom.gba", 0x000b9da0, 0x0000081c
	.global BattleParty_RoundEndGroupOrder
BattleParty_RoundEndGroupOrder:
	.incbin "baserom.gba", 0x000ba5bc, 0x00000048
	.global Data_080c3604
Data_080c3604:
	.incbin "baserom.gba", 0x000ba604, 0x0000001c
	.global Data_080c3620
Data_080c3620:
	.incbin "baserom.gba", 0x000ba620, 0x00000008
	.global Data_080c3628
Data_080c3628:
	.incbin "baserom.gba", 0x000ba628, 0x0000010c
	.global BattlePres_AdvanceArrowTiles
BattlePres_AdvanceArrowTiles:
	.incbin "baserom.gba", 0x000ba734, 0x00000800
	.global Debug_PartyPresets
Debug_PartyPresets:
	.incbin "baserom.gba", 0x000baf34, 0x00001a04
	.global BattlePres_ActorObjectScript
BattlePres_ActorObjectScript:
	.incbin "baserom.gba", 0x000bc938, 0x00000004
	.global Resource_SlotAssignments
Resource_SlotAssignments:
	.incbin "baserom.gba", 0x000bc93c, 0x00000068
	.global BattleMotion_VariantAcceleration
BattleMotion_VariantAcceleration:
	.incbin "baserom.gba", 0x000bc9a4, 0x00000020
	.global BattleMotion_VariantSpeedLimit
BattleMotion_VariantSpeedLimit:
	.incbin "baserom.gba", 0x000bc9c4, 0x00000020
	.global BattleMotion_VariantVelocityY
BattleMotion_VariantVelocityY:
	.incbin "baserom.gba", 0x000bc9e4, 0x00000020
	.global BattleMotion_VariantDistancePercent
BattleMotion_VariantDistancePercent:
	.incbin "baserom.gba", 0x000bca04, 0x0000002c
	.global BattlePres_TileVariants
BattlePres_TileVariants:
	.incbin "baserom.gba", 0x000bca30, 0x00000100
	.global BattlePres_CurtainTiles
BattlePres_CurtainTiles:
	.incbin "baserom.gba", 0x000bcb30, 0x000000e0
	.global Data_080c5c10
Data_080c5c10:
	.incbin "baserom.gba", 0x000bcc10, 0x00000028
	.global BattleFormation_Records
BattleFormation_Records:
	.incbin "baserom.gba", 0x000bcc38, 0x000017c0
	.global RomBytes_080c73f8
RomBytes_080c73f8:
	.incbin "baserom.gba", 0x000be3f8, 0x00000028
	.global Summon_EntryTable
Summon_EntryTable:
	.incbin "baserom.gba", 0x000be420, 0x0000055b
	.section .rom.000d9972, "ax"
	.incbin "baserom.gba", 0x000d9972, 0x00000002
	.section .rom.000e4a78, "ax"
	.global Data_080eda78
Data_080eda78:
	.incbin "baserom.gba", 0x000e4a78, 0x00000008
	.global BattleFx10_UnitScale
BattleFx10_UnitScale:
	.incbin "baserom.gba", 0x000e4a80, 0x00000008
	.global RockWall_Heights
RockWall_Heights:
	.incbin "baserom.gba", 0x000e4a88, 0x00000010
	.global Mode12_Points
Mode12_Points:
	.incbin "baserom.gba", 0x000e4a98, 0x00000018
	.global RisingWall_UnitScale
RisingWall_UnitScale:
	.incbin "baserom.gba", 0x000e4ab0, 0x00000008
	.global Formation_NormalScale
Formation_NormalScale:
	.incbin "baserom.gba", 0x000e4ab8, 0x00000008
	.global Formation_SmallScale
Formation_SmallScale:
	.incbin "baserom.gba", 0x000e4ac0, 0x00000008
	.global CirclingScene_UnitScale
CirclingScene_UnitScale:
	.incbin "baserom.gba", 0x000e4ac8, 0x00000008
	.global ShockwaveScene_UnitScale
ShockwaveScene_UnitScale:
	.incbin "baserom.gba", 0x000e4ad0, 0x00000008
	.global ObjectRow_SweepPair
ObjectRow_SweepPair:
	.incbin "baserom.gba", 0x000e4ad8, 0x00000008
	.global ObjectRow_RisePair
ObjectRow_RisePair:
	.incbin "baserom.gba", 0x000e4ae0, 0x00000008
	.global BattleFx6_UnitScale
BattleFx6_UnitScale:
	.incbin "baserom.gba", 0x000e4ae8, 0x00000008
	.section .rom.000e4e48, "ax"
	.global ParticleStreams_CellOffsets
ParticleStreams_CellOffsets:
	.incbin "baserom.gba", 0x000e4e48, 0x00000014
	.global BattleFx6_FlareCells
BattleFx6_FlareCells:
	.incbin "baserom.gba", 0x000e4e5c, 0x00000028
	.global BattleFx_PuffCells
BattleFx_PuffCells:
	.incbin "baserom.gba", 0x000e4e84, 0x00000012
	.global BattleFx_PuffSizes
BattleFx_PuffSizes:
	.incbin "baserom.gba", 0x000e4e96, 0x00000009
	.global PuffArc_CellWidths
PuffArc_CellWidths:
	.incbin "baserom.gba", 0x000e4e9f, 0x00000006
	.global PuffArc_CellHeights
PuffArc_CellHeights:
	.incbin "baserom.gba", 0x000e4ea5, 0x00000006
	.global PuffArc_CellBiasY
PuffArc_CellBiasY:
	.incbin "baserom.gba", 0x000e4eab, 0x00000007
	.global PuffArc_CellSourceOffsets
PuffArc_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e4eb2, 0x0000000c
	.global BattleFx_GlintCellOffsets
BattleFx_GlintCellOffsets:
	.incbin "baserom.gba", 0x000e4ebe, 0x0000000c
	.global BattleFx_GlintCellWidths
BattleFx_GlintCellWidths:
	.incbin "baserom.gba", 0x000e4eca, 0x00000006
	.global BattleFx_GlintCellHeights
BattleFx_GlintCellHeights:
	.incbin "baserom.gba", 0x000e4ed0, 0x00000006
	.global FallingShards_Counts
FallingShards_Counts:
	.incbin "baserom.gba", 0x000e4ed6, 0x00000006
	.global FallingBolts_Counts
FallingBolts_Counts:
	.incbin "baserom.gba", 0x000e4edc, 0x0000000c
	.global FiveMode_Records
FiveMode_Records:
	.incbin "baserom.gba", 0x000e4ee8, 0x00000014
	.global FiveMode_Tremble
FiveMode_Tremble:
	.incbin "baserom.gba", 0x000e4efc, 0x00000008
	.global TwelveMode_Records
TwelveMode_Records:
	.incbin "baserom.gba", 0x000e4f04, 0x00000054
	.global TwelveMode_StrikeWidths
TwelveMode_StrikeWidths:
	.incbin "baserom.gba", 0x000e4f58, 0x00000006
	.global TwelveMode_StrikeHeights
TwelveMode_StrikeHeights:
	.incbin "baserom.gba", 0x000e4f5e, 0x00000006
	.global TwelveMode_StrikeOffsets
TwelveMode_StrikeOffsets:
	.incbin "baserom.gba", 0x000e4f64, 0x0000000c
	.global TwelveMode_StrikeReach
TwelveMode_StrikeReach:
	.incbin "baserom.gba", 0x000e4f70, 0x00000006
	.global TwelveMode_ColumnStages
TwelveMode_ColumnStages:
	.incbin "baserom.gba", 0x000e4f76, 0x00000005
	.global TwelveMode_GlintDrawFlags
TwelveMode_GlintDrawFlags:
	.incbin "baserom.gba", 0x000e4f7b, 0x00000004
	.global BladeRain_CellWidths
BladeRain_CellWidths:
	.incbin "baserom.gba", 0x000e4f7f, 0x00000004
	.global BladeRain_CellHeights
BladeRain_CellHeights:
	.incbin "baserom.gba", 0x000e4f83, 0x00000005
	.global BladeRain_CellSourceOffsets
BladeRain_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e4f88, 0x00000008
	.global BattlePieceWidth
BattlePieceWidth:
	.incbin "baserom.gba", 0x000e4f90, 0x00000021
	.global BattlePieceHeight
BattlePieceHeight:
	.incbin "baserom.gba", 0x000e4fb1, 0x00000021
	.global BattlePieceOffset
BattlePieceOffset:
	.incbin "baserom.gba", 0x000e4fd2, 0x00000044
	.global BattlePieceStartX
BattlePieceStartX:
	.incbin "baserom.gba", 0x000e5016, 0x00000021
	.global BattlePieceStartY
BattlePieceStartY:
	.incbin "baserom.gba", 0x000e5037, 0x00000021
	.global CornerSparks_X
CornerSparks_X:
	.incbin "baserom.gba", 0x000e5058, 0x00000004
	.global CornerSparks_Y
CornerSparks_Y:
	.incbin "baserom.gba", 0x000e505c, 0x00000004
	.global CornerSparks_DrawFlags
CornerSparks_DrawFlags:
	.incbin "baserom.gba", 0x000e5060, 0x00000004
	.global TwoResource_CellWidths
TwoResource_CellWidths:
	.incbin "baserom.gba", 0x000e5064, 0x00000006
	.global TwoResource_CellHeights
TwoResource_CellHeights:
	.incbin "baserom.gba", 0x000e506a, 0x00000006
	.global TwoResource_CellSourceOffsets
TwoResource_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e5070, 0x0000000c
	.global TwoResource_CellX
TwoResource_CellX:
	.incbin "baserom.gba", 0x000e507c, 0x0000000c
	.global TwoResource_CellBiasY
TwoResource_CellBiasY:
	.incbin "baserom.gba", 0x000e5088, 0x00000008
	.global MemberBurst_Counts
MemberBurst_Counts:
	.incbin "baserom.gba", 0x000e5090, 0x00000006
	.global EarthWall_CellSourceOffsets
EarthWall_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e5096, 0x00000006
	.global EarthWall_CellWidths
EarthWall_CellWidths:
	.incbin "baserom.gba", 0x000e509c, 0x00000003
	.global EarthWall_CellHeights
EarthWall_CellHeights:
	.incbin "baserom.gba", 0x000e509f, 0x00000003
	.global MemberBeam_MotePictures
MemberBeam_MotePictures:
	.incbin "baserom.gba", 0x000e50a2, 0x00000008
	.global MemberBeam_HeadOffsets
MemberBeam_HeadOffsets:
	.incbin "baserom.gba", 0x000e50aa, 0x00000006
	.global MemberBeam_HeadWidths
MemberBeam_HeadWidths:
	.incbin "baserom.gba", 0x000e50b0, 0x00000003
	.global MemberBeam_HeadHeights
MemberBeam_HeadHeights:
	.incbin "baserom.gba", 0x000e50b3, 0x00000003
	.global SevenMode_Records
SevenMode_Records:
	.incbin "baserom.gba", 0x000e50b6, 0x0000000e
	.global SevenMode_ShardWidths
SevenMode_ShardWidths:
	.incbin "baserom.gba", 0x000e50c4, 0x00000012
	.global SevenMode_ShardHeights
SevenMode_ShardHeights:
	.incbin "baserom.gba", 0x000e50d6, 0x00000012
	.global SevenMode_ShardOffsets
SevenMode_ShardOffsets:
	.incbin "baserom.gba", 0x000e50e8, 0x00000024
	.global CounterReveal_PanelX
CounterReveal_PanelX:
	.incbin "baserom.gba", 0x000e510c, 0x0000000e
	.global CounterReveal_PanelY
CounterReveal_PanelY:
	.incbin "baserom.gba", 0x000e511a, 0x0000000e
	.global SpinningTriangle_Vertex
SpinningTriangle_Vertex:
	.incbin "baserom.gba", 0x000e5128, 0x0000000c
	.global TriangleStrike_Vertex
TriangleStrike_Vertex:
	.incbin "baserom.gba", 0x000e5134, 0x0000000c
	.global RingBolts_Points
RingBolts_Points:
	.incbin "baserom.gba", 0x000e5140, 0x00000018
	.global SpinningStars_Radii
SpinningStars_Radii:
	.incbin "baserom.gba", 0x000e5158, 0x00000002
	.global Data_080ee15a
Data_080ee15a:
	.incbin "baserom.gba", 0x000e515a, 0x00000009
	.global Data_080ee163
Data_080ee163:
	.incbin "baserom.gba", 0x000e5163, 0x00000009
	.global Data_080ee16c
Data_080ee16c:
	.incbin "baserom.gba", 0x000e516c, 0x00000008
	.global Data_080ee174
Data_080ee174:
	.incbin "baserom.gba", 0x000e5174, 0x00000003
	.global Data_080ee177
Data_080ee177:
	.incbin "baserom.gba", 0x000e5177, 0x00000003
	.global Data_080ee17a
Data_080ee17a:
	.incbin "baserom.gba", 0x000e517a, 0x00000004
	.global Data_080ee17e
Data_080ee17e:
	.incbin "baserom.gba", 0x000e517e, 0x00000006
	.global PaletteParticles_DropSpeeds
PaletteParticles_DropSpeeds:
	.incbin "baserom.gba", 0x000e5184, 0x00000004
	.global PaletteParticles_SparkPixels
PaletteParticles_SparkPixels:
	.incbin "baserom.gba", 0x000e5188, 0x00000002
	.global PaletteParticles_DropSizes
PaletteParticles_DropSizes:
	.incbin "baserom.gba", 0x000e518a, 0x00000004
	.global PaletteParticles_StreakWidths
PaletteParticles_StreakWidths:
	.incbin "baserom.gba", 0x000e518e, 0x00000006
	.global PaletteParticles_StreakHeights
PaletteParticles_StreakHeights:
	.incbin "baserom.gba", 0x000e5194, 0x00000006
	.global PaletteParticles_StreakDrops
PaletteParticles_StreakDrops:
	.incbin "baserom.gba", 0x000e519a, 0x00000006
	.global PaletteParticles_StreakOffsets
PaletteParticles_StreakOffsets:
	.incbin "baserom.gba", 0x000e51a0, 0x0000000c
	.global EmberColumns_Columns
EmberColumns_Columns:
	.incbin "baserom.gba", 0x000e51ac, 0x00000008
	.global EmberColumns_Gravity
EmberColumns_Gravity:
	.incbin "baserom.gba", 0x000e51b4, 0x00000010
	.global VortexMotes_Counts
VortexMotes_Counts:
	.incbin "baserom.gba", 0x000e51c4, 0x00000006
	.global Tornado_Shapes
Tornado_Shapes:
	.incbin "baserom.gba", 0x000e51ca, 0x00000009
	.global Crystal_ShardStarts
Crystal_ShardStarts:
	.incbin "baserom.gba", 0x000e51d3, 0x00000022
	.global Crystal_Counts
Crystal_Counts:
	.incbin "baserom.gba", 0x000e51f5, 0x00000006
	.global Crystal_ShardWidths
Crystal_ShardWidths:
	.incbin "baserom.gba", 0x000e51fb, 0x0000000c
	.global Crystal_ShardHeights
Crystal_ShardHeights:
	.incbin "baserom.gba", 0x000e5207, 0x0000000d
	.global Crystal_ShardOffsets
Crystal_ShardOffsets:
	.incbin "baserom.gba", 0x000e5214, 0x00000030
	.global LightningPillar_Sparks
LightningPillar_Sparks:
	.incbin "baserom.gba", 0x000e5244, 0x0000000c
	.global LightningPillar_Columns
LightningPillar_Columns:
	.incbin "baserom.gba", 0x000e5250, 0x0000000e
	.global LightningPillar_Counts
LightningPillar_Counts:
	.incbin "baserom.gba", 0x000e525e, 0x00000004
	.global SparkGroups_Shapes
SparkGroups_Shapes:
	.incbin "baserom.gba", 0x000e5262, 0x00000032
	.global SparkGroups_FlashCells
SparkGroups_FlashCells:
	.incbin "baserom.gba", 0x000e5294, 0x00000006
	.global FirePillars_Counts
FirePillars_Counts:
	.incbin "baserom.gba", 0x000e529a, 0x00000003
	.global FirePillars_Depths
FirePillars_Depths:
	.incbin "baserom.gba", 0x000e529d, 0x0000000c
	.global FirePillars_StartFrames
FirePillars_StartFrames:
	.incbin "baserom.gba", 0x000e52a9, 0x00000005
	.global RenderMode_GlintFlips
RenderMode_GlintFlips:
	.incbin "baserom.gba", 0x000e52ae, 0x00000006
	.global BattleFx_ModeHandlers
BattleFx_ModeHandlers:
	.incbin "baserom.gba", 0x000e52b4, 0x0000065c
	.global ThornVines_ObjectPoses
ThornVines_ObjectPoses:
	.incbin "baserom.gba", 0x000e5910, 0x00000006
	.global ThornVines_ThornOffsets
ThornVines_ThornOffsets:
	.incbin "baserom.gba", 0x000e5916, 0x0000000a
	.global ThornVines_ThornWidths
ThornVines_ThornWidths:
	.incbin "baserom.gba", 0x000e5920, 0x00000005
	.global ThornVines_ThornHeights
ThornVines_ThornHeights:
	.incbin "baserom.gba", 0x000e5925, 0x00000005
	.global ThornVines_LeafOffsets
ThornVines_LeafOffsets:
	.incbin "baserom.gba", 0x000e592a, 0x00000006
	.global ThornVines_LeafSizes
ThornVines_LeafSizes:
	.incbin "baserom.gba", 0x000e5930, 0x00000004
	.global ThornVines_ChipOffsets
ThornVines_ChipOffsets:
	.incbin "baserom.gba", 0x000e5934, 0x0000000a
	.global ThornVines_ChipWidths
ThornVines_ChipWidths:
	.incbin "baserom.gba", 0x000e593e, 0x00000005
	.global ThornVines_ChipHeights
ThornVines_ChipHeights:
	.incbin "baserom.gba", 0x000e5943, 0x00000005
	.global ThornVines_SparkOffsets
ThornVines_SparkOffsets:
	.incbin "baserom.gba", 0x000e5948, 0x0000000a
	.global ThornVines_SparkSizes
ThornVines_SparkSizes:
	.incbin "baserom.gba", 0x000e5952, 0x00000006
	.global ThornVines_EmberOffsets
ThornVines_EmberOffsets:
	.incbin "baserom.gba", 0x000e5958, 0x0000000e
	.global ThornVines_EmberSizes
ThornVines_EmberSizes:
	.incbin "baserom.gba", 0x000e5966, 0x0000000e
	.global BattleFx10_Points
BattleFx10_Points:
	.incbin "baserom.gba", 0x000e5974, 0x00000020
	.global BattleFx10_ShakeOffsets
BattleFx10_ShakeOffsets:
	.incbin "baserom.gba", 0x000e5994, 0x00000004
	.global BattleFx10_RockCells
BattleFx10_RockCells:
	.incbin "baserom.gba", 0x000e5998, 0x00000006
	.global BattleFx10_RockWidths
BattleFx10_RockWidths:
	.incbin "baserom.gba", 0x000e599e, 0x00000003
	.global BattleFx10_RockHeights
BattleFx10_RockHeights:
	.incbin "baserom.gba", 0x000e59a1, 0x00000003
	.global BattleFx10_Animations
BattleFx10_Animations:
	.incbin "baserom.gba", 0x000e59a4, 0x00000004
	.global BattleFx10_DebrisWidths
BattleFx10_DebrisWidths:
	.incbin "baserom.gba", 0x000e59a8, 0x0000000b
	.global BattleFx10_DebrisHeights
BattleFx10_DebrisHeights:
	.incbin "baserom.gba", 0x000e59b3, 0x0000000b
	.global BattleFx10_DebrisCells
BattleFx10_DebrisCells:
	.incbin "baserom.gba", 0x000e59be, 0x00000016
	.global BattleFx10_SprayWidths
BattleFx10_SprayWidths:
	.incbin "baserom.gba", 0x000e59d4, 0x00000003
	.global BattleFx10_SprayHeights
BattleFx10_SprayHeights:
	.incbin "baserom.gba", 0x000e59d7, 0x00000003
	.global BattleFx10_SprayCells
BattleFx10_SprayCells:
	.incbin "baserom.gba", 0x000e59da, 0x00000006
	.global BattleFx10_BoulderCells
BattleFx10_BoulderCells:
	.incbin "baserom.gba", 0x000e59e0, 0x00000006
	.global BattleFx10_BoulderWidths
BattleFx10_BoulderWidths:
	.incbin "baserom.gba", 0x000e59e6, 0x00000003
	.global BattleFx10_BoulderHeights
BattleFx10_BoulderHeights:
	.incbin "baserom.gba", 0x000e59e9, 0x00000003
	.global BattleFx10_FallWidths
BattleFx10_FallWidths:
	.incbin "baserom.gba", 0x000e59ec, 0x00000003
	.global BattleFx10_FallHeights
BattleFx10_FallHeights:
	.incbin "baserom.gba", 0x000e59ef, 0x00000003
	.global BattleFx10_FallCells
BattleFx10_FallCells:
	.incbin "baserom.gba", 0x000e59f2, 0x00000006
	.global IceShardBursts_Gravities
IceShardBursts_Gravities:
	.incbin "baserom.gba", 0x000e59f8, 0x00000010
	.global PaletteRamp_ShardCells
PaletteRamp_ShardCells:
	.incbin "baserom.gba", 0x000e5a08, 0x00000018
	.global PaletteRamp_ShardWidths
PaletteRamp_ShardWidths:
	.incbin "baserom.gba", 0x000e5a20, 0x0000000c
	.global PaletteRamp_ShardHeights
PaletteRamp_ShardHeights:
	.incbin "baserom.gba", 0x000e5a2c, 0x0000000c
	.global RockWall_Timings
RockWall_Timings:
	.incbin "baserom.gba", 0x000e5a38, 0x00000009
	.global HomingEmbers_Counts
HomingEmbers_Counts:
	.incbin "baserom.gba", 0x000e5a41, 0x00000003
	.global HomingEmbers_FlareWidths
HomingEmbers_FlareWidths:
	.incbin "baserom.gba", 0x000e5a44, 0x00000006
	.global HomingEmbers_FlareHeights
HomingEmbers_FlareHeights:
	.incbin "baserom.gba", 0x000e5a4a, 0x00000006
	.global HomingEmbers_FlareBiasY
HomingEmbers_FlareBiasY:
	.incbin "baserom.gba", 0x000e5a50, 0x00000006
	.global HomingEmbers_FlareCells
HomingEmbers_FlareCells:
	.incbin "baserom.gba", 0x000e5a56, 0x0000000c
	.global RisingMotes_ColumnSpots
RisingMotes_ColumnSpots:
	.incbin "baserom.gba", 0x000e5a62, 0x00000026
	.global RisingMotes_Timings
RisingMotes_Timings:
	.incbin "baserom.gba", 0x000e5a88, 0x00000009
	.global RisingMotes_MoteWidths
RisingMotes_MoteWidths:
	.incbin "baserom.gba", 0x000e5a91, 0x00000008
	.global RisingMotes_MoteHeights
RisingMotes_MoteHeights:
	.incbin "baserom.gba", 0x000e5a99, 0x00000009
	.global RisingMotes_MoteCells
RisingMotes_MoteCells:
	.incbin "baserom.gba", 0x000e5aa2, 0x00000010
	.global RisingMotes_ColumnCells
RisingMotes_ColumnCells:
	.incbin "baserom.gba", 0x000e5ab2, 0x00000006
	.global RisingMotes_ColumnHeights
RisingMotes_ColumnHeights:
	.incbin "baserom.gba", 0x000e5ab8, 0x00000003
	.global RisingMotes_EmberWidths
RisingMotes_EmberWidths:
	.incbin "baserom.gba", 0x000e5abb, 0x00000008
	.global RisingMotes_EmberHeights
RisingMotes_EmberHeights:
	.incbin "baserom.gba", 0x000e5ac3, 0x00000009
	.global RisingMotes_EmberCells
RisingMotes_EmberCells:
	.incbin "baserom.gba", 0x000e5acc, 0x00000010
	.global LightningBolts_Sparks
LightningBolts_Sparks:
	.incbin "baserom.gba", 0x000e5adc, 0x00000006
	.global ParticleField_Counts
ParticleField_Counts:
	.incbin "baserom.gba", 0x000e5ae2, 0x0000000a
	.global ParticleField_PuffCells
ParticleField_PuffCells:
	.incbin "baserom.gba", 0x000e5aec, 0x0000000e
	.global ParticleField_PuffSizes
ParticleField_PuffSizes:
	.incbin "baserom.gba", 0x000e5afa, 0x00000046
	.global StagedParticles_UnitScale
StagedParticles_UnitScale:
	.incbin "baserom.gba", 0x000e5b40, 0x00000008
	.global DualTable_VineWidths
DualTable_VineWidths:
	.incbin "baserom.gba", 0x000e5b48, 0x00000003
	.global DualTable_HeightLimits
DualTable_HeightLimits:
	.incbin "baserom.gba", 0x000e5b4b, 0x00000003
	.global DualTable_VineCells
DualTable_VineCells:
	.incbin "baserom.gba", 0x000e5b4e, 0x00000006
	.global DualTable_PillarWidths
DualTable_PillarWidths:
	.incbin "baserom.gba", 0x000e5b54, 0x00000004
	.global DualTable_PillarCells
DualTable_PillarCells:
	.incbin "baserom.gba", 0x000e5b58, 0x00000006
	.global DualTable_ColumnCounts
DualTable_ColumnCounts:
	.incbin "baserom.gba", 0x000e5b5e, 0x00000003
	.global DualTable_ColumnX
DualTable_ColumnX:
	.incbin "baserom.gba", 0x000e5b61, 0x00000010
	.global DualTable_PillarHeights
DualTable_PillarHeights:
	.incbin "baserom.gba", 0x000e5b71, 0x00000008
	.global DualTable_PuffWidths
DualTable_PuffWidths:
	.incbin "baserom.gba", 0x000e5b79, 0x00000007
	.global DualTable_PuffHeights
DualTable_PuffHeights:
	.incbin "baserom.gba", 0x000e5b80, 0x00000008
	.global DualTable_PuffCells
DualTable_PuffCells:
	.incbin "baserom.gba", 0x000e5b88, 0x0000000e
	.global RisingColumns_ColumnOffsets
RisingColumns_ColumnOffsets:
	.incbin "baserom.gba", 0x000e5b96, 0x00000010
	.global BattleFxPillar_Kinds
BattleFxPillar_Kinds:
	.incbin "baserom.gba", 0x000e5ba6, 0x00000008
	.global BattleFxPillar_X
BattleFxPillar_X:
	.incbin "baserom.gba", 0x000e5bae, 0x00000008
	.global BattleFxPillar_Counts
BattleFxPillar_Counts:
	.incbin "baserom.gba", 0x000e5bb6, 0x00000003
	.global BattleFxPillar_PuffWidths
BattleFxPillar_PuffWidths:
	.incbin "baserom.gba", 0x000e5bb9, 0x00000007
	.global BattleFxPillar_PuffHeights
BattleFxPillar_PuffHeights:
	.incbin "baserom.gba", 0x000e5bc0, 0x00000008
	.global BattleFxPillar_PuffCells
BattleFxPillar_PuffCells:
	.incbin "baserom.gba", 0x000e5bc8, 0x0000000e
	.global LightningBolts_Counts
LightningBolts_Counts:
	.incbin "baserom.gba", 0x000e5bd6, 0x0000000c
	.global LightningBolts_GlintPalettes
LightningBolts_GlintPalettes:
	.incbin "baserom.gba", 0x000e5be2, 0x00000004
	.global LightningBolts_GlintModes
LightningBolts_GlintModes:
	.incbin "baserom.gba", 0x000e5be6, 0x00000003
	.global Volley_CometHeights
Volley_CometHeights:
	.incbin "baserom.gba", 0x000e5be9, 0x00000003
	.global Volley_Modes
Volley_Modes:
	.incbin "baserom.gba", 0x000e5bec, 0x0000003c
	.global Volley_FlashWidths
Volley_FlashWidths:
	.incbin "baserom.gba", 0x000e5c28, 0x00000007
	.global Volley_FlashHeights
Volley_FlashHeights:
	.incbin "baserom.gba", 0x000e5c2f, 0x00000007
	.global Volley_FlashSteps
Volley_FlashSteps:
	.incbin "baserom.gba", 0x000e5c36, 0x00000007
	.global Volley_FlashRises
Volley_FlashRises:
	.incbin "baserom.gba", 0x000e5c3d, 0x00000007
	.global Volley_FlashOffsets
Volley_FlashOffsets:
	.incbin "baserom.gba", 0x000e5c44, 0x0000000e
	.global Volley_PelletOffsets
Volley_PelletOffsets:
	.incbin "baserom.gba", 0x000e5c52, 0x00000008
	.global MercuryDjinnFlames_LaunchFrames
MercuryDjinnFlames_LaunchFrames:
	.incbin "baserom.gba", 0x000e5c5a, 0x00000005
	.global ParticleReveal_CellWidths
ParticleReveal_CellWidths:
	.incbin "baserom.gba", 0x000e5c5f, 0x00000004
	.global ParticleReveal_CellHeights
ParticleReveal_CellHeights:
	.incbin "baserom.gba", 0x000e5c63, 0x00000005
	.global ParticleReveal_CellSourceOffsets
ParticleReveal_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e5c68, 0x00000008
	.global RockToss_ChipFlips
RockToss_ChipFlips:
	.incbin "baserom.gba", 0x000e5c70, 0x00000004
	.global RockToss_ChipWidths
RockToss_ChipWidths:
	.incbin "baserom.gba", 0x000e5c74, 0x00000009
	.global RockToss_ChipHeights
RockToss_ChipHeights:
	.incbin "baserom.gba", 0x000e5c7d, 0x00000009
	.global RockToss_ChipCells
RockToss_ChipCells:
	.incbin "baserom.gba", 0x000e5c86, 0x00000012
	.global RockToss_ChipX
RockToss_ChipX:
	.incbin "baserom.gba", 0x000e5c98, 0x00000009
	.global RockToss_ChipY
RockToss_ChipY:
	.incbin "baserom.gba", 0x000e5ca1, 0x00000009
	.global Mode12_Animations
Mode12_Animations:
	.incbin "baserom.gba", 0x000e5caa, 0x00000004
	.global Mode12_SmokeFlips
Mode12_SmokeFlips:
	.incbin "baserom.gba", 0x000e5cae, 0x00000004
	.global ShatterRocks_ShardOffsets
ShatterRocks_ShardOffsets:
	.incbin "baserom.gba", 0x000e5cb2, 0x00000040
	.global ShatterRocks_RockX
ShatterRocks_RockX:
	.incbin "baserom.gba", 0x000e5cf2, 0x00000005
	.global ShatterRocks_DropFrames
ShatterRocks_DropFrames:
	.incbin "baserom.gba", 0x000e5cf7, 0x00000005
	.global ShatterRocks_RockCounts
ShatterRocks_RockCounts:
	.incbin "baserom.gba", 0x000e5cfc, 0x00000003
	.global ShatterRocks_ShardWidths
ShatterRocks_ShardWidths:
	.incbin "baserom.gba", 0x000e5cff, 0x0000000f
	.global ShatterRocks_ShardHeights
ShatterRocks_ShardHeights:
	.incbin "baserom.gba", 0x000e5d0e, 0x00000010
	.global ShatterRocks_ShardCells
ShatterRocks_ShardCells:
	.incbin "baserom.gba", 0x000e5d1e, 0x00000020
	.global BurstScene_Records
BurstScene_Records:
	.incbin "baserom.gba", 0x000e5d3e, 0x00000040
	.global RisingWall_RowSheetOffsets
RisingWall_RowSheetOffsets:
	.incbin "baserom.gba", 0x000e5d7e, 0x00000012
	.global RisingWall_RowWidths
RisingWall_RowWidths:
	.incbin "baserom.gba", 0x000e5d90, 0x0000000a
	.global RisingWall_MoundSheetOffsets
RisingWall_MoundSheetOffsets:
	.incbin "baserom.gba", 0x000e5d9a, 0x00000006
	.global RisingWall_MoundWidths
RisingWall_MoundWidths:
	.incbin "baserom.gba", 0x000e5da0, 0x00000003
	.global RisingWall_MoundHeights
RisingWall_MoundHeights:
	.incbin "baserom.gba", 0x000e5da3, 0x00000003
	.global RisingWall_Bg2Shifts
RisingWall_Bg2Shifts:
	.incbin "baserom.gba", 0x000e5da6, 0x00000006
	.global RisingWall_LateRockColumns
RisingWall_LateRockColumns:
	.incbin "baserom.gba", 0x000e5dac, 0x00000006
	.global BeamSequence_CellWidths
BeamSequence_CellWidths:
	.incbin "baserom.gba", 0x000e5db2, 0x00000006
	.global BeamSequence_CellHeights
BeamSequence_CellHeights:
	.incbin "baserom.gba", 0x000e5db8, 0x00000006
	.global BeamSequence_CellSheetOffsets
BeamSequence_CellSheetOffsets:
	.incbin "baserom.gba", 0x000e5dbe, 0x0000000c
	.global BeamSequence_CellReaches
BeamSequence_CellReaches:
	.incbin "baserom.gba", 0x000e5dca, 0x00000006
	.global CastingImpact_GlintDrawFlags
CastingImpact_GlintDrawFlags:
	.incbin "baserom.gba", 0x000e5dd0, 0x00000004
	.global CastingImpact_ImageX
CastingImpact_ImageX:
	.incbin "baserom.gba", 0x000e5dd4, 0x0000000e
	.global CastingImpact_ImageY
CastingImpact_ImageY:
	.incbin "baserom.gba", 0x000e5de2, 0x00000008
	.global CastingImpact_OrbitCells
CastingImpact_OrbitCells:
	.incbin "baserom.gba", 0x000e5dea, 0x0000000a
	.global FireSwirl_CellWidths
FireSwirl_CellWidths:
	.incbin "baserom.gba", 0x000e5df4, 0x00000007
	.global FireSwirl_CellHeights
FireSwirl_CellHeights:
	.incbin "baserom.gba", 0x000e5dfb, 0x00000007
	.global FireSwirl_CellSourceOffsets
FireSwirl_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e5e02, 0x0000000e
	.global FireSwirl_CellBiasX
FireSwirl_CellBiasX:
	.incbin "baserom.gba", 0x000e5e10, 0x00000007
	.global FireSwirl_CellBiasY
FireSwirl_CellBiasY:
	.incbin "baserom.gba", 0x000e5e17, 0x00000007
	.global Formation_TwelveX
Formation_TwelveX:
	.incbin "baserom.gba", 0x000e5e1e, 0x0000000c
	.global Formation_TwelveZ
Formation_TwelveZ:
	.incbin "baserom.gba", 0x000e5e2a, 0x0000000c
	.global Formation_RightEightX
Formation_RightEightX:
	.incbin "baserom.gba", 0x000e5e36, 0x00000008
	.global Formation_RightEightZ
Formation_RightEightZ:
	.incbin "baserom.gba", 0x000e5e3e, 0x00000008
	.global Formation_SmallEightX
Formation_SmallEightX:
	.incbin "baserom.gba", 0x000e5e46, 0x00000008
	.global Formation_SmallEightZ
Formation_SmallEightZ:
	.incbin "baserom.gba", 0x000e5e4e, 0x00000008
	.global ImpactBurst_CellWidths
ImpactBurst_CellWidths:
	.incbin "baserom.gba", 0x000e5e56, 0x00000008
	.global ImpactBurst_CellHeights
ImpactBurst_CellHeights:
	.incbin "baserom.gba", 0x000e5e5e, 0x00000008
	.global ImpactBurst_CellSourceOffsets
ImpactBurst_CellSourceOffsets:
	.incbin "baserom.gba", 0x000e5e66, 0x00000010
	.global ParticleStreams_OrbitPoints
ParticleStreams_OrbitPoints:
	.incbin "baserom.gba", 0x000e5e76, 0x0000002a
	.global ParticleStreams_DropPoints
ParticleStreams_DropPoints:
	.incbin "baserom.gba", 0x000e5ea0, 0x0000001c
	.global ParticleStreams_FlashSheetOffsets
ParticleStreams_FlashSheetOffsets:
	.incbin "baserom.gba", 0x000e5ebc, 0x0000000e
	.global ParticleStreams_FlashSizes
ParticleStreams_FlashSizes:
	.incbin "baserom.gba", 0x000e5eca, 0x0000000e
	.global CirclingScene_ObjectColumns
CirclingScene_ObjectColumns:
	.incbin "baserom.gba", 0x000e5ed8, 0x00000009
	.global CirclingScene_ObjectRows
CirclingScene_ObjectRows:
	.incbin "baserom.gba", 0x000e5ee1, 0x00000009
	.global CirclingScene_PuffSheetOffsets
CirclingScene_PuffSheetOffsets:
	.incbin "baserom.gba", 0x000e5eea, 0x0000000e
	.global CirclingScene_PuffSizes
CirclingScene_PuffSizes:
	.incbin "baserom.gba", 0x000e5ef8, 0x0000000e
	.global FlameBlade_StrikeColumns
FlameBlade_StrikeColumns:
	.incbin "baserom.gba", 0x000e5f06, 0x00000006
	.global FlameBlade_FlashCells
FlameBlade_FlashCells:
	.incbin "baserom.gba", 0x000e5f0c, 0x00000006
	.global FallingSword_FlashCells
FallingSword_FlashCells:
	.incbin "baserom.gba", 0x000e5f12, 0x00000006
	.global FallingSword_DustGravity
FallingSword_DustGravity:
	.incbin "baserom.gba", 0x000e5f18, 0x00000010
	.global ShockwaveScene_BurstHeights
ShockwaveScene_BurstHeights:
	.incbin "baserom.gba", 0x000e5f28, 0x00000008
	.global ShockwaveScene_BurstCells
ShockwaveScene_BurstCells:
	.incbin "baserom.gba", 0x000e5f30, 0x0000000e
	.global ShockwaveScene_ShardCells
ShockwaveScene_ShardCells:
	.incbin "baserom.gba", 0x000e5f3e, 0x0000000c
	.global ShockwaveScene_ShardWidths
ShockwaveScene_ShardWidths:
	.incbin "baserom.gba", 0x000e5f4a, 0x00000006
	.global ShockwaveScene_ShardHeights
ShockwaveScene_ShardHeights:
	.incbin "baserom.gba", 0x000e5f50, 0x00000006
	.global ObjectRow_Columns
ObjectRow_Columns:
	.incbin "baserom.gba", 0x000e5f56, 0x00000009
	.global ObjectRow_Rows
ObjectRow_Rows:
	.incbin "baserom.gba", 0x000e5f5f, 0x00000009
	.global BattleFx6_ObjectX
BattleFx6_ObjectX:
	.incbin "baserom.gba", 0x000e5f68, 0x00000008
	.global BattleFx6_ObjectY
BattleFx6_ObjectY:
	.incbin "baserom.gba", 0x000e5f70, 0x00000008
	.global BattleFx6_Gravity
BattleFx6_Gravity:
	.incbin "baserom.gba", 0x000e5f78, 0x00000010
	.global RisingBurst_SparkCells
RisingBurst_SparkCells:
	.incbin "baserom.gba", 0x000e5f88, 0x0000000e
	.global RisingBurst_SparkSizes
RisingBurst_SparkSizes:
	.incbin "baserom.gba", 0x000e5f96, 0x0000000e
	.section .rom.000e6014, "ax"
	.incbin "baserom.gba", 0x000e6014, 0x00000020
	.global SentouKouka_BitOperands
SentouKouka_BitOperands:
	.incbin "baserom.gba", 0x000e6034, 0x00000020
	.section .rom.000e7a5c, "ax"
	.global DisplayScroll_SlideResources
DisplayScroll_SlideResources:
	.incbin "baserom.gba", 0x000e7a5c, 0x00000909
	.global DisplayScroll_GlyphWidths
DisplayScroll_GlyphWidths:
	.incbin "baserom.gba", 0x000e8365, 0x00000063
	.global DisplayScroll_LineTable
DisplayScroll_LineTable:
	.incbin "baserom.gba", 0x000e83c8, 0x00000540
	.global DisplayScroll_Font
DisplayScroll_Font:
	.incbin "baserom.gba", 0x000e8908, 0x000002f3
	.section .rom.000ea8bc, "ax"
	.global Title_PromptTiles
Title_PromptTiles:
	.incbin "baserom.gba", 0x000ea8bc, 0x000000ef
	.global Data_080f39ab
Data_080f39ab:
	.incbin "baserom.gba", 0x000ea9ab, 0x00000006
	.global Title_PromptBlendLevels
Title_PromptBlendLevels:
	.incbin "baserom.gba", 0x000ea9b1, 0x0000003d
	.global Palette_ChannelCurve
Palette_ChannelCurve:
	.incbin "baserom.gba", 0x000ea9ee, 0x00000040
	.global Palette_ChannelCurveLight
Palette_ChannelCurveLight:
	.incbin "baserom.gba", 0x000eaa2e, 0x00000040
	.global Palette_ChannelCurveContrast
Palette_ChannelCurveContrast:
	.incbin "baserom.gba", 0x000eaa6e, 0x00000040
	.section .rom.000ec3a4, "ax"
	.incbin "baserom.gba", 0x000ec3a4, 0x00000020
	.global Data_080f53fc
Data_080f53fc:
	.incbin "baserom.gba", 0x000ec3c4, 0x00000004
	.global Data_080f5400
Data_080f5400:
	.incbin "baserom.gba", 0x000ec3c8, 0x00000004
	.global Data_080f5408
Data_080f5408:
	.incbin "baserom.gba", 0x000ec3cc, 0x00000012
	.global Data_080f541a
Data_080f541a:
	.incbin "baserom.gba", 0x000ec3de, 0x0000000c
	.section .rom.000ef61c, "ax"
	.global ReelGame_SparkCellOffsets
ReelGame_SparkCellOffsets:
	.incbin "baserom.gba", 0x000ef61c, 0x00000014
	.global ReelGame_SymbolPrizes
ReelGame_SymbolPrizes:
	.incbin "baserom.gba", 0x000ef630, 0x00000006
	.global ReelGame_TitleLetterTiles
ReelGame_TitleLetterTiles:
	.incbin "baserom.gba", 0x000ef636, 0x00000008
	.global ReelGame_FrameSpriteX
ReelGame_FrameSpriteX:
	.incbin "baserom.gba", 0x000ef63e, 0x0000000e
	.global ReelGame_FrameSpriteY
ReelGame_FrameSpriteY:
	.incbin "baserom.gba", 0x000ef64c, 0x0000000e
	.global ReelGame_TitleLetterWidths
ReelGame_TitleLetterWidths:
	.incbin "baserom.gba", 0x000ef65a, 0x00000007
	.section .rom.000f27a0, "ax"
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x000f27a0, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x000f2830, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x000f28e4, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x000f2914, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x000f292c, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x000f29b0, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x000f29c8, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x000f2a04, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x000f2a14, 0x00000034
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x000f2a48, 0x00000030
	.section .rom.000f3504, "ax"
	.incbin "baserom.gba", 0x000f3504, 0x00000090
	.section .rom.000f3624, "ax"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x000f3624, 0x00000060
	.section .rom.00317fa0, "ax"
	.global Resource_BuildStamp
Resource_BuildStamp:
	.incbin "baserom.gba", 0x00317fa0, 0x00000010
	.section .rom.0031b9e7, "ax"
	.incbin "baserom.gba", 0x0031b9e7, 0x00000001
	.section .rom.00322693, "ax"
	.incbin "baserom.gba", 0x00322693, 0x00000001
	.global Title_IntroGraphicsC
Title_IntroGraphicsC:
	.incbin "baserom.gba", 0x00322694, 0x000086f8
	.section .rom.00332335, "ax"
	.incbin "baserom.gba", 0x00332335, 0x00000003
	.global Title_IntroTilesB
Title_IntroTilesB:
	.incbin "baserom.gba", 0x00332338, 0x000002b4
	.section .rom.00336a2e, "ax"
	.incbin "baserom.gba", 0x00336a2e, 0x00000002
	.section .rom.0034719e, "ax"
	.incbin "baserom.gba", 0x0034719e, 0x00000002
	.section .rom.0034a78e, "ax"
	.incbin "baserom.gba", 0x0034a78e, 0x00000002
	.section .rom.00356232, "ax"
	.incbin "baserom.gba", 0x00356232, 0x00000002
	.section .rom.003668e2, "ax"
	.incbin "baserom.gba", 0x003668e2, 0x00000002
	.section .rom.0036e382, "ax"
	.incbin "baserom.gba", 0x0036e382, 0x00000002
	.section .rom.00371bc2, "ax"
	.incbin "baserom.gba", 0x00371bc2, 0x00000002
	.section .rom.0037aff6, "ax"
	.incbin "baserom.gba", 0x0037aff6, 0x00000002
	.section .rom.0038641e, "ax"
	.incbin "baserom.gba", 0x0038641e, 0x00000002
	.section .rom.0038e2da, "ax"
	.incbin "baserom.gba", 0x0038e2da, 0x00000002
	.section .rom.00392d6a, "ax"
	.incbin "baserom.gba", 0x00392d6a, 0x00000002
	.section .rom.0039701e, "ax"
	.incbin "baserom.gba", 0x0039701e, 0x00000002
	.section .rom.0039a99e, "ax"
	.incbin "baserom.gba", 0x0039a99e, 0x00000002
	.section .rom.003a71da, "ax"
	.incbin "baserom.gba", 0x003a71da, 0x00000002
	.section .rom.003b5b22, "ax"
	.incbin "baserom.gba", 0x003b5b22, 0x00000002
	.section .rom.003bb425, "ax"
	.incbin "baserom.gba", 0x003bb425, 0x00000003
	.section .rom.003bcdc7, "ax"
	.incbin "baserom.gba", 0x003bcdc7, 0x00000001
	.section .rom.003bfbe5, "ax"
	.incbin "baserom.gba", 0x003bfbe5, 0x00000003
	.section .rom.003c3339, "ax"
	.incbin "baserom.gba", 0x003c3339, 0x00000003
	.section .rom.003c4b4f, "ax"
	.incbin "baserom.gba", 0x003c4b4f, 0x00000001
	.section .rom.003c513a, "ax"
	.incbin "baserom.gba", 0x003c513a, 0x00000002
	.section .rom.003c57fe, "ax"
	.incbin "baserom.gba", 0x003c57fe, 0x00000002
	.section .rom.003c5ae5, "ax"
	.incbin "baserom.gba", 0x003c5ae5, 0x00000003
	.section .rom.003c6e4b, "ax"
	.incbin "baserom.gba", 0x003c6e4b, 0x00000001
	.section .rom.003c7235, "ax"
	.incbin "baserom.gba", 0x003c7235, 0x00000003
	.section .rom.003c7607, "ax"
	.incbin "baserom.gba", 0x003c7607, 0x00000001
	.section .rom.003c7ea7, "ax"
	.incbin "baserom.gba", 0x003c7ea7, 0x00000001
	.section .rom.003c834a, "ax"
	.incbin "baserom.gba", 0x003c834a, 0x00000002
	.section .rom.003c9503, "ax"
	.incbin "baserom.gba", 0x003c9503, 0x00000001
	.section .rom.003ca9c6, "ax"
	.incbin "baserom.gba", 0x003ca9c6, 0x00000002
	.section .rom.003cb98f, "ax"
	.incbin "baserom.gba", 0x003cb98f, 0x00000001
	.section .rom.003cbbea, "ax"
	.incbin "baserom.gba", 0x003cbbea, 0x00000002
	.section .rom.003cd645, "ax"
	.incbin "baserom.gba", 0x003cd645, 0x00000003
	.section .rom.003cdb91, "ax"
	.incbin "baserom.gba", 0x003cdb91, 0x00000003
	.section .rom.003cf91a, "ax"
	.incbin "baserom.gba", 0x003cf91a, 0x00000002
	.section .rom.003cfb93, "ax"
	.incbin "baserom.gba", 0x003cfb93, 0x00000001
	.section .rom.003d005a, "ax"
	.incbin "baserom.gba", 0x003d005a, 0x00000002
	.section .rom.003d1c2f, "ax"
	.incbin "baserom.gba", 0x003d1c2f, 0x00000001
	.section .rom.003d382d, "ax"
	.incbin "baserom.gba", 0x003d382d, 0x00000003
	.section .rom.003d3a4e, "ax"
	.incbin "baserom.gba", 0x003d3a4e, 0x00000002
	.section .rom.003d3e8b, "ax"
	.incbin "baserom.gba", 0x003d3e8b, 0x00000001
	.section .rom.003d3f9d, "ax"
	.incbin "baserom.gba", 0x003d3f9d, 0x00000003
	.section .rom.003d471b, "ax"
	.incbin "baserom.gba", 0x003d471b, 0x00000001
	.section .rom.003d4bb9, "ax"
	.incbin "baserom.gba", 0x003d4bb9, 0x00000003
	.section .rom.003d58ce, "ax"
	.incbin "baserom.gba", 0x003d58ce, 0x00000002
	.section .rom.003d664e, "ax"
	.incbin "baserom.gba", 0x003d664e, 0x00000002
	.section .rom.003d69df, "ax"
	.incbin "baserom.gba", 0x003d69df, 0x00000001
	.section .rom.003d8726, "ax"
	.incbin "baserom.gba", 0x003d8726, 0x00000002
	.section .rom.003d94fd, "ax"
	.incbin "baserom.gba", 0x003d94fd, 0x00000003
	.section .rom.003d9d2a, "ax"
	.incbin "baserom.gba", 0x003d9d2a, 0x00000002
	.section .rom.003da3b5, "ax"
	.incbin "baserom.gba", 0x003da3b5, 0x00000003
	.section .rom.003da573, "ax"
	.incbin "baserom.gba", 0x003da573, 0x00000001
	.section .rom.003db2cb, "ax"
	.incbin "baserom.gba", 0x003db2cb, 0x00000001
	.section .rom.003db817, "ax"
	.incbin "baserom.gba", 0x003db817, 0x00000001
	.section .rom.003dbbf1, "ax"
	.incbin "baserom.gba", 0x003dbbf1, 0x00000003
	.section .rom.003dbf9f, "ax"
	.incbin "baserom.gba", 0x003dbf9f, 0x00000001
	.section .rom.003ddf43, "ax"
	.incbin "baserom.gba", 0x003ddf43, 0x00000001
	.section .rom.003decdf, "ax"
	.incbin "baserom.gba", 0x003decdf, 0x00000001
	.section .rom.003deefb, "ax"
	.incbin "baserom.gba", 0x003deefb, 0x00000001
	.section .rom.003df1f7, "ax"
	.incbin "baserom.gba", 0x003df1f7, 0x00000001
	.section .rom.003e13a5, "ax"
	.incbin "baserom.gba", 0x003e13a5, 0x00000003
	.section .rom.003e2173, "ax"
	.incbin "baserom.gba", 0x003e2173, 0x00000001
	.section .rom.003e32d5, "ax"
	.incbin "baserom.gba", 0x003e32d5, 0x00000003
	.section .rom.003e382f, "ax"
	.incbin "baserom.gba", 0x003e382f, 0x00000001
	.section .rom.003e5119, "ax"
	.incbin "baserom.gba", 0x003e5119, 0x00000003
	.section .rom.003e59ba, "ax"
	.incbin "baserom.gba", 0x003e59ba, 0x00000002
	.section .rom.003e5d97, "ax"
	.incbin "baserom.gba", 0x003e5d97, 0x00000001
	.section .rom.003e6021, "ax"
	.incbin "baserom.gba", 0x003e6021, 0x00000003
	.section .rom.003e63c7, "ax"
	.incbin "baserom.gba", 0x003e63c7, 0x00000001
	.section .rom.003e6622, "ax"
	.incbin "baserom.gba", 0x003e6622, 0x00000002
	.section .rom.003e69da, "ax"
	.incbin "baserom.gba", 0x003e69da, 0x00000002
	.section .rom.003e7ec3, "ax"
	.incbin "baserom.gba", 0x003e7ec3, 0x00000001
	.section .rom.003e9486, "ax"
	.incbin "baserom.gba", 0x003e9486, 0x00000002
	.global BattleFx_LavaOrbSheet
BattleFx_LavaOrbSheet:
	.incbin "baserom.gba", 0x003e9488, 0x00000a6c
	.section .rom.003eacca, "ax"
	.incbin "baserom.gba", 0x003eacca, 0x00000002
	.section .rom.003eb04d, "ax"
	.incbin "baserom.gba", 0x003eb04d, 0x00000003
	.section .rom.003ebcfd, "ax"
	.incbin "baserom.gba", 0x003ebcfd, 0x00000003
	.section .rom.003ec845, "ax"
	.incbin "baserom.gba", 0x003ec845, 0x00000003
	.section .rom.003ec9de, "ax"
	.incbin "baserom.gba", 0x003ec9de, 0x00000002
	.section .rom.003ed26b, "ax"
	.incbin "baserom.gba", 0x003ed26b, 0x00000001
	.section .rom.003edd32, "ax"
	.incbin "baserom.gba", 0x003edd32, 0x00000002
	.section .rom.003ee24a, "ax"
	.incbin "baserom.gba", 0x003ee24a, 0x00000002
	.section .rom.003ee86e, "ax"
	.incbin "baserom.gba", 0x003ee86e, 0x00000002
	.section .rom.003ef0d5, "ax"
	.incbin "baserom.gba", 0x003ef0d5, 0x00000003
	.section .rom.003ef4f9, "ax"
	.incbin "baserom.gba", 0x003ef4f9, 0x00000003
	.section .rom.003ef793, "ax"
	.incbin "baserom.gba", 0x003ef793, 0x00000001
	.section .rom.003f112f, "ax"
	.incbin "baserom.gba", 0x003f112f, 0x00000001
	.section .rom.003f2ea6, "ax"
	.incbin "baserom.gba", 0x003f2ea6, 0x00000002
	.section .rom.003f4e1b, "ax"
	.incbin "baserom.gba", 0x003f4e1b, 0x00000001
	.section .rom.003f52eb, "ax"
	.incbin "baserom.gba", 0x003f52eb, 0x00000001
	.section .rom.003f597d, "ax"
	.incbin "baserom.gba", 0x003f597d, 0x00000003
	.global BattleFx_IceShardSheet
BattleFx_IceShardSheet:
	.incbin "baserom.gba", 0x003f5980, 0x00000a34
	.section .rom.003f7785, "ax"
	.incbin "baserom.gba", 0x003f7785, 0x00000003
	.section .rom.003f8256, "ax"
	.incbin "baserom.gba", 0x003f8256, 0x00000002
	.section .rom.003f8d93, "ax"
	.incbin "baserom.gba", 0x003f8d93, 0x00000001
	.section .rom.003f93d3, "ax"
	.incbin "baserom.gba", 0x003f93d3, 0x00000001
	.global BattleFx_IceBlockSheet
BattleFx_IceBlockSheet:
	.incbin "baserom.gba", 0x003f93d4, 0x00001588
	.section .rom.003fa9bd, "ax"
	.incbin "baserom.gba", 0x003fa9bd, 0x00000003
	.section .rom.003fad2b, "ax"
	.incbin "baserom.gba", 0x003fad2b, 0x00000001
	.section .rom.003fb2c9, "ax"
	.incbin "baserom.gba", 0x003fb2c9, 0x00000003
	.section .rom.003fb5fd, "ax"
	.incbin "baserom.gba", 0x003fb5fd, 0x00000003
	.section .rom.003fb939, "ax"
	.incbin "baserom.gba", 0x003fb939, 0x00000003
	.section .rom.003fc952, "ax"
	.incbin "baserom.gba", 0x003fc952, 0x00000002
	.section .rom.003fff5e, "ax"
	.incbin "baserom.gba", 0x003fff5e, 0x00000002
	.section .rom.00400701, "ax"
	.incbin "baserom.gba", 0x00400701, 0x00000003
	.section .rom.00401a93, "ax"
	.incbin "baserom.gba", 0x00401a93, 0x00000001
	.section .rom.00401ec3, "ax"
	.incbin "baserom.gba", 0x00401ec3, 0x00000001
	.section .rom.00403cb9, "ax"
	.incbin "baserom.gba", 0x00403cb9, 0x00000003
	.section .rom.004045ce, "ax"
	.incbin "baserom.gba", 0x004045ce, 0x00000002
	.section .rom.00406102, "ax"
	.incbin "baserom.gba", 0x00406102, 0x00000002
	.section .rom.00407151, "ax"
	.incbin "baserom.gba", 0x00407151, 0x00000003
	.section .rom.00407807, "ax"
	.incbin "baserom.gba", 0x00407807, 0x00000001
	.section .rom.004088ad, "ax"
	.incbin "baserom.gba", 0x004088ad, 0x00000003
	.section .rom.0041bcbf, "ax"
	.incbin "baserom.gba", 0x0041bcbf, 0x00000001
	.section .rom.0041be11, "ax"
	.incbin "baserom.gba", 0x0041be11, 0x00000003
	.section .rom.0041c2a2, "ax"
	.incbin "baserom.gba", 0x0041c2a2, 0x00000002
	.section .rom.0041c499, "ax"
	.incbin "baserom.gba", 0x0041c499, 0x00000003
	.section .rom.0041db56, "ax"
	.incbin "baserom.gba", 0x0041db56, 0x00000002
	.section .rom.0041f075, "ax"
	.incbin "baserom.gba", 0x0041f075, 0x00000003
	.section .rom.0041fc41, "ax"
	.incbin "baserom.gba", 0x0041fc41, 0x00000003
	.section .rom.004207b6, "ax"
	.incbin "baserom.gba", 0x004207b6, 0x00000002
	.section .rom.0042265f, "ax"
	.incbin "baserom.gba", 0x0042265f, 0x00000001
	.section .rom.00423ded, "ax"
	.incbin "baserom.gba", 0x00423ded, 0x00000003
	.section .rom.0042529e, "ax"
	.incbin "baserom.gba", 0x0042529e, 0x00000002
	.section .rom.004279eb, "ax"
	.incbin "baserom.gba", 0x004279eb, 0x00000001
	.section .rom.00429285, "ax"
	.incbin "baserom.gba", 0x00429285, 0x00000003
	.section .rom.0042a84e, "ax"
	.incbin "baserom.gba", 0x0042a84e, 0x00000002
	.section .rom.0042c2de, "ax"
	.incbin "baserom.gba", 0x0042c2de, 0x00000002
	.section .rom.0042d1e2, "ax"
	.incbin "baserom.gba", 0x0042d1e2, 0x00000002
	.section .rom.00436e66, "ax"
	.incbin "baserom.gba", 0x00436e66, 0x00000002
	.section .rom.0043901a, "ax"
	.incbin "baserom.gba", 0x0043901a, 0x00000002
	.section .rom.0043d181, "ax"
	.incbin "baserom.gba", 0x0043d181, 0x00000003
	.section .rom.004403a6, "ax"
	.incbin "baserom.gba", 0x004403a6, 0x00000002
	.section .rom.00441aaa, "ax"
	.incbin "baserom.gba", 0x00441aaa, 0x00000002
	.section .rom.00448692, "ax"
	.incbin "baserom.gba", 0x00448692, 0x00000002
	.section .rom.00451016, "ax"
	.incbin "baserom.gba", 0x00451016, 0x00000002
	.section .rom.00458af2, "ax"
	.incbin "baserom.gba", 0x00458af2, 0x00000002
	.section .rom.0045bb86, "ax"
	.incbin "baserom.gba", 0x0045bb86, 0x00000002
	.section .rom.0045ef95, "ax"
	.incbin "baserom.gba", 0x0045ef95, 0x00000003
	.section .rom.00461801, "ax"
	.incbin "baserom.gba", 0x00461801, 0x00000003
	.section .rom.004632f2, "ax"
	.incbin "baserom.gba", 0x004632f2, 0x00000002
	.section .rom.0046410a, "ax"
	.incbin "baserom.gba", 0x0046410a, 0x00000002
	.section .rom.00464df5, "ax"
	.incbin "baserom.gba", 0x00464df5, 0x00000003
	.section .rom.0046e2b2, "ax"
	.incbin "baserom.gba", 0x0046e2b2, 0x00000002
	.section .rom.0046efbd, "ax"
	.incbin "baserom.gba", 0x0046efbd, 0x00000003
	.section .rom.004710d1, "ax"
	.incbin "baserom.gba", 0x004710d1, 0x00000003
	.section .rom.00471d43, "ax"
	.incbin "baserom.gba", 0x00471d43, 0x00000001
	.section .rom.0047295b, "ax"
	.incbin "baserom.gba", 0x0047295b, 0x00000001
	.section .rom.004730cf, "ax"
	.incbin "baserom.gba", 0x004730cf, 0x00000001
	.section .rom.0047495f, "ax"
	.incbin "baserom.gba", 0x0047495f, 0x00000001
	.section .rom.0047532d, "ax"
	.incbin "baserom.gba", 0x0047532d, 0x00000003
	.section .rom.00475f4b, "ax"
	.incbin "baserom.gba", 0x00475f4b, 0x00000001
	.section .rom.004785ff, "ax"
	.incbin "baserom.gba", 0x004785ff, 0x00000001
	.section .rom.0047ad12, "ax"
	.incbin "baserom.gba", 0x0047ad12, 0x00000002
	.section .rom.0047fc67, "ax"
	.incbin "baserom.gba", 0x0047fc67, 0x00000001
	.section .rom.00483f91, "ax"
	.incbin "baserom.gba", 0x00483f91, 0x00000003
	.section .rom.00484b7e, "ax"
	.incbin "baserom.gba", 0x00484b7e, 0x00000002
	.section .rom.00489733, "ax"
	.incbin "baserom.gba", 0x00489733, 0x00000001
	.section .rom.0048ba9d, "ax"
	.incbin "baserom.gba", 0x0048ba9d, 0x00000003
	.section .rom.0048d43f, "ax"
	.incbin "baserom.gba", 0x0048d43f, 0x00000001
	.section .rom.00491d01, "ax"
	.incbin "baserom.gba", 0x00491d01, 0x00000003
	.section .rom.00495dc9, "ax"
	.incbin "baserom.gba", 0x00495dc9, 0x00000003
	.section .rom.00499492, "ax"
	.incbin "baserom.gba", 0x00499492, 0x00000002
	.section .rom.004a1463, "ax"
	.incbin "baserom.gba", 0x004a1463, 0x00000001
	.section .rom.004a8773, "ax"
	.incbin "baserom.gba", 0x004a8773, 0x00000001
	.section .rom.004ad6f3, "ax"
	.incbin "baserom.gba", 0x004ad6f3, 0x00000001
	.section .rom.004b20a3, "ax"
	.incbin "baserom.gba", 0x004b20a3, 0x00000001
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x004b20a4, 0x0000000c
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x004b20b0, 0x00000150
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x004b2200, 0x00000140
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x004b2340, 0x00000140
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x004b2480, 0x00000140
	.section .rom.004b79cf, "ax"
	.incbin "baserom.gba", 0x004b79cf, 0x00000001
	.section .rom.004b7b5d, "ax"
	.incbin "baserom.gba", 0x004b7b5d, 0x00000003
	.section .rom.004bcbbd, "ax"
	.incbin "baserom.gba", 0x004bcbbd, 0x00000003
	.section .rom.004c1129, "ax"
	.incbin "baserom.gba", 0x004c1129, 0x00000003
	.section .rom.004c62f1, "ax"
	.incbin "baserom.gba", 0x004c62f1, 0x00000003
	.section .rom.004c6487, "ax"
	.incbin "baserom.gba", 0x004c6487, 0x00000001
	.section .rom.004c90db, "ax"
	.incbin "baserom.gba", 0x004c90db, 0x00000001
	.section .rom.004cfd8f, "ax"
	.incbin "baserom.gba", 0x004cfd8f, 0x00000001
	.section .rom.004d2eca, "ax"
	.incbin "baserom.gba", 0x004d2eca, 0x00000002
	.section .rom.004d304b, "ax"
	.incbin "baserom.gba", 0x004d304b, 0x00000001
	.section .rom.004d5805, "ax"
	.incbin "baserom.gba", 0x004d5805, 0x00000003
	.section .rom.004d7e4e, "ax"
	.incbin "baserom.gba", 0x004d7e4e, 0x00000002
	.section .rom.004da532, "ax"
	.incbin "baserom.gba", 0x004da532, 0x00000002
	.section .rom.004db5a7, "ax"
	.incbin "baserom.gba", 0x004db5a7, 0x00000001
	.section .rom.004dd6fa, "ax"
	.incbin "baserom.gba", 0x004dd6fa, 0x00000002
	.section .rom.004dd88d, "ax"
	.incbin "baserom.gba", 0x004dd88d, 0x00000003
	.section .rom.004e00cf, "ax"
	.incbin "baserom.gba", 0x004e00cf, 0x00000001
	.section .rom.004e1ee5, "ax"
	.incbin "baserom.gba", 0x004e1ee5, 0x00000003
	.section .rom.004e45c6, "ax"
	.incbin "baserom.gba", 0x004e45c6, 0x00000002
	.section .rom.004e570d, "ax"
	.incbin "baserom.gba", 0x004e570d, 0x00000003
	.section .rom.004e8821, "ax"
	.incbin "baserom.gba", 0x004e8821, 0x00000003
	.section .rom.004ec989, "ax"
	.incbin "baserom.gba", 0x004ec989, 0x00000003
	.section .rom.004edf86, "ax"
	.incbin "baserom.gba", 0x004edf86, 0x00000002
	.section .rom.004ef24f, "ax"
	.incbin "baserom.gba", 0x004ef24f, 0x00000001
	.section .rom.004f2667, "ax"
	.incbin "baserom.gba", 0x004f2667, 0x00000001
	.section .rom.004f27f5, "ax"
	.incbin "baserom.gba", 0x004f27f5, 0x00000003
	.section .rom.004f45a6, "ax"
	.incbin "baserom.gba", 0x004f45a6, 0x00000002
	.section .rom.004f811d, "ax"
	.incbin "baserom.gba", 0x004f811d, 0x00000003
	.section .rom.004f962b, "ax"
	.incbin "baserom.gba", 0x004f962b, 0x00000001
	.section .rom.004fa1ff, "ax"
	.incbin "baserom.gba", 0x004fa1ff, 0x00000001
	.section .rom.004fa2fd, "ax"
	.incbin "baserom.gba", 0x004fa2fd, 0x00000003
	.section .rom.004fb6e2, "ax"
	.incbin "baserom.gba", 0x004fb6e2, 0x00000002
	.section .rom.004fc865, "ax"
	.incbin "baserom.gba", 0x004fc865, 0x00000003
	.section .rom.004fd1ee, "ax"
	.incbin "baserom.gba", 0x004fd1ee, 0x00000002
	.section .rom.004ffe6b, "ax"
	.incbin "baserom.gba", 0x004ffe6b, 0x00000001
	.section .rom.004fff4e, "ax"
	.incbin "baserom.gba", 0x004fff4e, 0x00000002
	.section .rom.00501139, "ax"
	.incbin "baserom.gba", 0x00501139, 0x00000003
	.section .rom.00502dc1, "ax"
	.incbin "baserom.gba", 0x00502dc1, 0x00000003
	.section .rom.005032cf, "ax"
	.incbin "baserom.gba", 0x005032cf, 0x00000001
	.section .rom.0050340f, "ax"
	.incbin "baserom.gba", 0x0050340f, 0x00000001
	.section .rom.0050462a, "ax"
	.incbin "baserom.gba", 0x0050462a, 0x00000002
	.section .rom.00504785, "ax"
	.incbin "baserom.gba", 0x00504785, 0x00000003
	.section .rom.00508bab, "ax"
	.incbin "baserom.gba", 0x00508bab, 0x00000001
	.section .rom.0050a8da, "ax"
	.incbin "baserom.gba", 0x0050a8da, 0x00000002
	.section .rom.0050b653, "ax"
	.incbin "baserom.gba", 0x0050b653, 0x00000001
	.section .rom.0050b787, "ax"
	.incbin "baserom.gba", 0x0050b787, 0x00000001
	.section .rom.0050dc07, "ax"
	.incbin "baserom.gba", 0x0050dc07, 0x00000001
	.section .rom.0050e983, "ax"
	.incbin "baserom.gba", 0x0050e983, 0x00000001
	.section .rom.0050f462, "ax"
	.incbin "baserom.gba", 0x0050f462, 0x00000002
	.section .rom.005102da, "ax"
	.incbin "baserom.gba", 0x005102da, 0x00000002
	.section .rom.005123ae, "ax"
	.incbin "baserom.gba", 0x005123ae, 0x00000002
	.section .rom.00512cf5, "ax"
	.incbin "baserom.gba", 0x00512cf5, 0x00000003
	.section .rom.00513942, "ax"
	.incbin "baserom.gba", 0x00513942, 0x00000002
	.section .rom.00513b69, "ax"
	.incbin "baserom.gba", 0x00513b69, 0x00000003
	.section .rom.005154ab, "ax"
	.incbin "baserom.gba", 0x005154ab, 0x00000001
	.section .rom.005155bf, "ax"
	.incbin "baserom.gba", 0x005155bf, 0x00000001
	.section .rom.00516f85, "ax"
	.incbin "baserom.gba", 0x00516f85, 0x00000003
	.section .rom.005170cf, "ax"
	.incbin "baserom.gba", 0x005170cf, 0x00000001
	.section .rom.0051919b, "ax"
	.incbin "baserom.gba", 0x0051919b, 0x00000001
	.section .rom.005192c3, "ax"
	.incbin "baserom.gba", 0x005192c3, 0x00000001
	.section .rom.0051ab4b, "ax"
	.incbin "baserom.gba", 0x0051ab4b, 0x00000001
	.section .rom.0051cded, "ax"
	.incbin "baserom.gba", 0x0051cded, 0x00000003
	.section .rom.0051cf2f, "ax"
	.incbin "baserom.gba", 0x0051cf2f, 0x00000001
	.section .rom.0051ef37, "ax"
	.incbin "baserom.gba", 0x0051ef37, 0x00000001
	.section .rom.0051f042, "ax"
	.incbin "baserom.gba", 0x0051f042, 0x00000002
	.section .rom.00521026, "ax"
	.incbin "baserom.gba", 0x00521026, 0x00000002
	.section .rom.0052274b, "ax"
	.incbin "baserom.gba", 0x0052274b, 0x00000001
	.section .rom.005237ff, "ax"
	.incbin "baserom.gba", 0x005237ff, 0x00000001
	.section .rom.005254e6, "ax"
	.incbin "baserom.gba", 0x005254e6, 0x00000002
	.section .rom.00525641, "ax"
	.incbin "baserom.gba", 0x00525641, 0x00000003
	.section .rom.0052758a, "ax"
	.incbin "baserom.gba", 0x0052758a, 0x00000002
	.section .rom.0052770f, "ax"
	.incbin "baserom.gba", 0x0052770f, 0x00000001
	.section .rom.0052a261, "ax"
	.incbin "baserom.gba", 0x0052a261, 0x00000003
	.section .rom.0052c926, "ax"
	.incbin "baserom.gba", 0x0052c926, 0x00000002
	.section .rom.0052f07e, "ax"
	.incbin "baserom.gba", 0x0052f07e, 0x00000002
	.section .rom.005308d1, "ax"
	.incbin "baserom.gba", 0x005308d1, 0x00000003
	.section .rom.00532fd1, "ax"
	.incbin "baserom.gba", 0x00532fd1, 0x00000003
	.section .rom.00534d9d, "ax"
	.incbin "baserom.gba", 0x00534d9d, 0x00000003
	.section .rom.005372e7, "ax"
	.incbin "baserom.gba", 0x005372e7, 0x00000001
	.section .rom.005385b2, "ax"
	.incbin "baserom.gba", 0x005385b2, 0x00000002
	.section .rom.00539263, "ax"
	.incbin "baserom.gba", 0x00539263, 0x00000001
	.section .rom.00539ccd, "ax"
	.incbin "baserom.gba", 0x00539ccd, 0x00000003
	.section .rom.00539dc3, "ax"
	.incbin "baserom.gba", 0x00539dc3, 0x00000001
	.section .rom.0053b822, "ax"
	.incbin "baserom.gba", 0x0053b822, 0x00000002
	.section .rom.0053c5c6, "ax"
	.incbin "baserom.gba", 0x0053c5c6, 0x00000002
	.section .rom.0053e0ff, "ax"
	.incbin "baserom.gba", 0x0053e0ff, 0x00000001
	.section .rom.00540fed, "ax"
	.incbin "baserom.gba", 0x00540fed, 0x00000003
	.section .rom.005487d3, "ax"
	.incbin "baserom.gba", 0x005487d3, 0x00000001
	.section .rom.0054889f, "ax"
	.incbin "baserom.gba", 0x0054889f, 0x00000001
	.section .rom.0054e14a, "ax"
	.incbin "baserom.gba", 0x0054e14a, 0x00000002
	.section .rom.0054e28b, "ax"
	.incbin "baserom.gba", 0x0054e28b, 0x00000001
	.section .rom.0054f6b9, "ax"
	.incbin "baserom.gba", 0x0054f6b9, 0x00000003
	.section .rom.005523f6, "ax"
	.incbin "baserom.gba", 0x005523f6, 0x00000002
	.section .rom.005544f2, "ax"
	.incbin "baserom.gba", 0x005544f2, 0x00000002
	.section .rom.0055befa, "ax"
	.incbin "baserom.gba", 0x0055befa, 0x00000002
	.section .rom.0055c096, "ax"
	.incbin "baserom.gba", 0x0055c096, 0x00000002
	.section .rom.00560931, "ax"
	.incbin "baserom.gba", 0x00560931, 0x00000003
	.section .rom.00562e32, "ax"
	.incbin "baserom.gba", 0x00562e32, 0x00000002
	.section .rom.0056a823, "ax"
	.incbin "baserom.gba", 0x0056a823, 0x00000001
	.section .rom.0056a9be, "ax"
	.incbin "baserom.gba", 0x0056a9be, 0x00000002
	.section .rom.0056d56d, "ax"
	.incbin "baserom.gba", 0x0056d56d, 0x00000003
	.section .rom.0056f641, "ax"
	.incbin "baserom.gba", 0x0056f641, 0x00000003
	.section .rom.0057169a, "ax"
	.incbin "baserom.gba", 0x0057169a, 0x00000002
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0057169c, 0x00001cec
	.section .rom.00574b31, "ax"
	.incbin "baserom.gba", 0x00574b31, 0x00000003
	.section .rom.0057780d, "ax"
	.incbin "baserom.gba", 0x0057780d, 0x00000003
	.section .rom.005795df, "ax"
	.incbin "baserom.gba", 0x005795df, 0x00000001
	.section .rom.0057971f, "ax"
	.incbin "baserom.gba", 0x0057971f, 0x00000001
	.section .rom.0057c779, "ax"
	.incbin "baserom.gba", 0x0057c779, 0x00000003
	.section .rom.0057f6f6, "ax"
	.incbin "baserom.gba", 0x0057f6f6, 0x00000002
	.section .rom.00581fbd, "ax"
	.incbin "baserom.gba", 0x00581fbd, 0x00000003
	.section .rom.00588f25, "ax"
	.incbin "baserom.gba", 0x00588f25, 0x00000003
	.section .rom.0058a7cb, "ax"
	.incbin "baserom.gba", 0x0058a7cb, 0x00000001
	.section .rom.0058b0ca, "ax"
	.incbin "baserom.gba", 0x0058b0ca, 0x00000002
	.section .rom.0058be43, "ax"
	.incbin "baserom.gba", 0x0058be43, 0x00000001
	.section .rom.0058bfa3, "ax"
	.incbin "baserom.gba", 0x0058bfa3, 0x00000001
	.section .rom.0058dbfb, "ax"
	.incbin "baserom.gba", 0x0058dbfb, 0x00000001
	.section .rom.0058f497, "ax"
	.incbin "baserom.gba", 0x0058f497, 0x00000001
	.section .rom.0058f61b, "ax"
	.incbin "baserom.gba", 0x0058f61b, 0x00000001
	.section .rom.00592157, "ax"
	.incbin "baserom.gba", 0x00592157, 0x00000001
	.section .rom.005948d1, "ax"
	.incbin "baserom.gba", 0x005948d1, 0x00000003
	.section .rom.005958be, "ax"
	.incbin "baserom.gba", 0x005958be, 0x00000002
	.section .rom.00596f8b, "ax"
	.incbin "baserom.gba", 0x00596f8b, 0x00000001
	.section .rom.00597135, "ax"
	.incbin "baserom.gba", 0x00597135, 0x00000003
	.section .rom.00599ec9, "ax"
	.incbin "baserom.gba", 0x00599ec9, 0x00000003
	.section .rom.0059df5e, "ax"
	.incbin "baserom.gba", 0x0059df5e, 0x00000002
	.section .rom.0059e09f, "ax"
	.incbin "baserom.gba", 0x0059e09f, 0x00000001
	.section .rom.0059ff67, "ax"
	.incbin "baserom.gba", 0x0059ff67, 0x00000001
	.section .rom.005a0109, "ax"
	.incbin "baserom.gba", 0x005a0109, 0x00000003
	.section .rom.005a21ab, "ax"
	.incbin "baserom.gba", 0x005a21ab, 0x00000001
	.section .rom.005a3138, "ax"
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x005a3138, 0x000022d8
	.section .rom.005a7f01, "ax"
	.incbin "baserom.gba", 0x005a7f01, 0x00000003
	.section .rom.005a8086, "ax"
	.incbin "baserom.gba", 0x005a8086, 0x00000002
	.section .rom.005af7d3, "ax"
	.incbin "baserom.gba", 0x005af7d3, 0x00000001
	.section .rom.005afcdd, "ax"
	.incbin "baserom.gba", 0x005afcdd, 0x00000003
	.section .rom.005b13b5, "ax"
	.incbin "baserom.gba", 0x005b13b5, 0x00000003
	.section .rom.005b1559, "ax"
	.incbin "baserom.gba", 0x005b1559, 0x00000003
	.section .rom.005b2e5e, "ax"
	.incbin "baserom.gba", 0x005b2e5e, 0x00000002
	.section .rom.005b468b, "ax"
	.incbin "baserom.gba", 0x005b468b, 0x00000001
	.section .rom.005b4809, "ax"
	.incbin "baserom.gba", 0x005b4809, 0x00000003
	.section .rom.005b90fd, "ax"
	.incbin "baserom.gba", 0x005b90fd, 0x00000003
	.section .rom.005ba142, "ax"
	.incbin "baserom.gba", 0x005ba142, 0x00000002
	.section .rom.005baf66, "ax"
	.incbin "baserom.gba", 0x005baf66, 0x00000002
	.section .rom.005cec4a, "ax"
	.incbin "baserom.gba", 0x005cec4a, 0x00000002
	.section .rom.005e5062, "ax"
	.incbin "baserom.gba", 0x005e5062, 0x00000002
	.section .rom.00600b20, "ax"
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x00600b20, 0x00002904
	.section .rom.00688735, "ax"
	.incbin "baserom.gba", 0x00688735, 0x00000003
	.section .rom.006ccef5, "ax"
	.incbin "baserom.gba", 0x006ccef5, 0x00000003
	.section .rom.0071012c, "ax"
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x0071012c, 0x00001164
	.section .rom.0073de87, "ax"
	.incbin "baserom.gba", 0x0073de87, 0x00000001
	.section .rom.00754368, "ax"
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x00754368, 0x00004604
