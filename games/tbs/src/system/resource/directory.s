	.macro resource name, place:req
	.ifnb \name
	.global ResourceId_\name
	.equiv ResourceId_\name, (. - Resource_DirectoryTable) / 4
	.endif
	.4byte \place
	.endm

@ Directory syntax:
@   resource NAME, PLACE   One entry; NAME defines its ResourceId_NAME.
@   group KIND, names...   Consecutive named entries KIND_name.
@   area SET, count, maps... Maps, then palette and tiles A onward; count is ABCD or -.
@   recolor NN, maps...    Maps followed by a palette; reuse the preceding tiles.
@   overlays ID...         Map overlays in ID order.
@   unused N               N unused entries.
	.macro group kind:req, names:vararg
	.irp n, \names
	resource \n, \kind\()_\n
	.endr
	.endm

	.macro area set:req, sheets:req, layout_ids:vararg
	.irp m, \layout_ids
	resource , Field_Layout\m
	.endr
	resource , Tileset_\set\()Palette
	.ifnc \sheets, -
	.irpc s, \sheets
	resource , Tileset_\set\()Tiles\s
	.endr
	.endif
	.endm

	.macro recolor palette:req, layout_ids:vararg
	.irp m, \layout_ids
	resource , Field_Layout\m
	.endr
	resource , Tileset_Palette\palette
	.endm

	.macro overlays ids:vararg
	.irp id, \ids
	resource , Resource_Overlay\id
	.endr
	.endm

	.macro unused count:req
	.rept \count
	resource , 0
	.endr
	.endm

	.section .rodata
	.balign 4
	.global Resource_DirectoryTable
Resource_DirectoryTable:
	resource , Rom_Start
	resource , Resource_DirectoryTable
	group Resource, BuildStamp
	resource , FarCall_FieldTable
	resource , FarCall_WindowTable
	resource , FarCall_GameTable
	resource , FarCall_ObjectTable
	resource , FarCall_MenuTable
	resource , FarCall_ShopTable
	resource , FarCall_BattleTable
	resource , FarCall_ScrollTable
	resource PaletteFarCalls, FarCall_PaletteTable
	resource EffectFarCalls, FarCall_EffectTable
	resource , FarCall_BlankRunTable
	resource , FarCall_BlankLoadTable
	resource , FarCall_SoundTable
	resource , FarCall_SoundTable
	resource ResourceFarCalls, FarCall_ResourceTable
	resource ResourceFarCallsAlternate, FarCall_ResourceTable
	resource WindowTiles, Window_Tiles
	resource SmallFont, Text_SmallFont
	group Title, IntroGraphicsA, IntroGraphicsB, IntroGraphicsC, NintendoLogo, CamelotLogo
	group Title, GoldenSunLogo
	group Field, WorldMapPicture
	group Title, IntroTilesB
	group Battle, SuhallaGateBackdrop, PirateShipBackdrop, CrossboneIsleCaveBackdrop, ColossoBackdrop, VenusLighthouseAerieStormBackdrop
	group Battle, VenusLighthouseAerieBackdrop, VenusLighthouseBackdrop, SuhallaDesertBackdrop, AltmillerCaveBackdrop, TunnelRuinsBackdrop
	group Battle, BabiLighthouseBackdrop, LamakanDesertBackdrop, MogallForestBackdrop, MercuryLighthouseAerieBackdrop, TolbiShipInteriorsBackdrop
	group Battle, ImilFuchinFallsBackdrop, KolimaForestBackdrop, TretTreeInteriorsBackdrop, GomaCaveBackdrop, InteriorBackdrop
	group Battle, AltinPeakBackdrop, TolbiShipDeckBackdrop, MercuryLighthouseBackdrop, ValeStormBackdrop, TretTreeBranchBackdrop
	group Battle, BeachBackdrop, SnowfieldBackdrop, GrasslandBackdrop, SolSanctumBackdrop, ForestBackdrop
	group Battle, VioletSkyBackdrop, DesertBackdrop, DuskCloudsBackdrop, AerialViewBackdrop
	resource LuckyWheelsFramePicture, LuckyWheels_FramePicture
	resource LuckyWheelsIcons, LuckyWheels_Icons
	resource LuckyWheelsJackpot, LuckyWheels_Jackpot
	resource LuckyDiceBoard, LuckyDice_Board
	resource LuckyDiceSprites, LuckyDice_Sprites
	group BattleAnim, TileAnimationMask, CyanSparkSheet, VioletPaletteA, RedPaletteA, YellowPaletteA
	group BattleAnim, StarBurstSheet, CrescentSheet, RevealMaskA, RevealMaskB, RevealMaskC
	group BattleAnim, RevealMaskD, RevealMaskE, RevealMaskF, LimeColumnSheet, RainSheet
	group BattleAnim, FlameSheetA, DemonFaceSheet, FlameBladeSheet, SwordSheet, IceTileSheet
	group BattleAnim, FiveModeImage, SpiderWebSheet, VioletLightningSheetA, YellowSparkSheet, ProjectileVolleyImage
	group BattleAnim, ShurikenSheet, FlareSheet, FlareImage, VioletPaletteB, FireSwirlSheet
	group BattleAnim, VioletLightningSheetB, FireBurstSheet, BlueCrescentSheet, OverlayImageA, VioletMoteSheet
	group BattleAnim, OverlayImageB, FireballSheet, MemberBurstImage, SlashSheet, WaveSheet
	group BattleAnim, TargetBurstImage, PinkStarSheet, WaterSpraySheet, BlueFlameSheet, ExplosionSheet
	group BattleAnim, MagentaSwirlSheet, MagentaTailSheet, ParticleSpritesA, ParticleSpritesB, ParticleSpritesC
	group BattleAnim, ParticleSpritesD, GreenPalette, SwordSlashSheet, RuneSheet, FlameColumnSheet
	group BattleAnim, EarthWallSheet, OrangePaletteA, FlashBurstSheet, ThornSheet, CometSheetA
	group BattleAnim, CometSheetB, CometSheetC, WhirlwindSheet, GreenPillarSheet, VineSheet
	group BattleAnim, FirePillarSheetA, RedPaletteB, OrangePaletteB, VioletCometSheet, FlameballSheet
	group BattleAnim, BoulderSheet, RockWallSheet, StarDotSheet, MarsDjinnSheet, MarsDjinnSmallSheet
	group BattleAnim, JupiterDjinnSheet, JupiterDjinnSmallSheet, MercuryDjinnSheet, MercuryDjinnSmallSheet, VenusDjinnSheet
	group BattleAnim, VenusDjinnSmallSheet, FireBlobSheet, FireStreakSheet, LavaOrbSheet, YellowRingSheet
	group BattleAnim, YellowOrbSheet, BubbleSheet, ShieldSheet, DaggerSheet, PaletteRampImage
	group BattleAnim, SmokeSheet, BluePalette, LimePalette, PinkPalette, VioletPaletteC
	group BattleAnim, CyanPalette, TanPalette, WispSheet, GreenVineSheet, PortalSheet
	group BattleAnim, SheepSheet, SkullSheet, HeartSheet, CounterRevealSheet, RedCrescentSheetA
	group BattleAnim, BlueBeastSheet, RedCrescentSheetB, SpiralSheet, VioletStreakSheet, CrescentMoonSheet
	group BattleAnim, ThornVineSheet, IceShardSheet, EmberStreakSheet, BlueArcSheetA, BlueArcSheetB
	group BattleAnim, GlowOrbSheet, IceChipSheet, IceBlockSheet, SparkleDots, PinkBurstSheet
	group BattleAnim, LightFanSheet, BlueBurstSheet, DustPuffSheet, GoldShellSheet, BlastSheet
	group BattleAnim, FlameSheetB, FirePillarSheetB, RockSpireSheet, LightningBoltSheet, BeamSequenceImage
	group BattleAnim, TwelveModeImage, WindStreakSheet, YellowPaletteB, BlueRingSheet, RedPaletteC
	group BattleAnim, VioletPaletteD, BlueBeamSheet, VortexSheet, TornadoSheet, CrystalSheet
	group BattleAnim, WaterSparkleSheet, LightningPillarSheet, StagedParticlePath, DualStreamPath
	group Field, PerspectiveDataA, DefaultGridCells, DefaultTileAnimation, DefaultMetatileAttributes
	resource WorldMapPaletteA, WorldMap_PaletteA
	resource WorldMapTilesA, WorldMap_TilesA
	resource WorldMapTilesB, WorldMap_TilesB
	resource WorldMapTilesC, WorldMap_TilesC
	resource WorldMapTilesD, WorldMap_TilesD
	resource WorldMapTilesE, WorldMap_TilesE
	resource WorldMapPaletteB, WorldMap_PaletteB
	resource WorldMapTilesF, WorldMap_TilesF
	resource WorldMapTilesG, WorldMap_TilesG
	resource WorldMapTilesH, WorldMap_TilesH
	resource WorldMapTilesI, WorldMap_TilesI
	resource WorldMapTilesJ, WorldMap_TilesJ
	group Colosso, OpponentPathA, OpponentPathB, OpponentPathC, Lettering
	resource MenuCursorFrames, MenuCursor_FrameA
	resource , MenuCursor_FrameB
	resource , MenuCursor_FrameC
	resource , MenuCursor_FrameD
	resource , MenuCursor_FrameE
	resource , MenuCursor_FrameF
	resource , MenuCursor_FrameG
	resource , MenuCursor_FrameH
	group Ui, Icons, CommandIcons
	group Monster, AxeGoblinSprites, GhoulSprites, HarpySprites, TrollSprites, WitchSprites
	group Monster, GriffinSprites, ShieldKnightSprites, OrcSprites, ChimeraSprites, LivingArmorSprites
	group Monster, ManticoreSprites, OrcLordSprites, GargoyleSprites, LizardManSprites, OgreSprites
	group Monster, TailedImpSprites, WispGhostSprites, SalamanderSprites, AquaBeastSprites, HoundSprites
	group Monster, BearSprites, SirenSprites, BlueWispSprites, MushroomSprites, RedWormSprites
	group Monster, FairySprites, BatSprites, SmallLizardSprites, WalkingPlantSprites, FlySprites
	group Monster, CrabSprites, SmallBirdSprites, MothSprites, FrogSprites, ManedLionSprites
	group Monster, SerpentSprites, WingedDragonSprites, BlueMageSprites, SkeletonSprites, FlameSkullSprites
	group Monster, FireDragonSprites, CrawlerSprites, MimicSprites, SeaBeastSprites, SpiritHorseSprites
	group Monster, KrakenSprites, SlimeSprites, SaturosSprites, MenardiSprites, GhostHeadSprites
	group Monster, FusionDragonSprites, PurpleBeastSprites, RedWarriorSprites, SwordGirlSprites
	group Field, Layout001
	resource , Tileset_BlankPalette
	resource , Tileset_BlankTilesA
	resource , Tileset_BlankTilesB
	resource , Tileset_BlankTilesC
	resource , Tileset_BlankTilesD
	area Set00, ABCD, 000
	area Set01, ABCD, 002
	area Set02, ABCD, 003
	area Set03, ABCD, 004
	area Set04, ABCD, 005
	area Set05, ABCD, 006
	area Set06, ABCD, 007
	area Set07, ABCD, 008
	area Set08, ABCD, 009
	area Set09, ABC, 010
	area Set10, ABCD, 011
	area Set11, ABCD, 012, 013
	area Set12, ABCD, 014
	area Set13, ABCD, 015
	recolor 00, 016
	area Set14, ABCD, 017
	area Set15, ABCD, 018
	area Set16, ABCD, 019
	area Set17, ABCD, 020
	area Set18, ABCD, 021
	area Set19, ABCD, 022
	area Set20, ABCD, 023, 024, 025
	area Set21, ABCD, 026, 027, 028
	area Set22, ABC, 029
	area Set23, ABCD, 030, 031
	area Set24, AB, 032
	area Set25, A, 033
	area Set26, ABC, 034
	area Set27, ABCD, 035
	area Set28, ABCD, 036
	area Set29, ABCD, 037
	area Set30, A, 038
	area Set31, ABCD, 039
	recolor 01, 040
	area Set32, ABCD, 041
	area Set33, A, 042
	recolor 02, 043
	area Set34, ABCD, 044
	area Set35, ABCD, 045
	area Set36, ABCD, 046
	area Set37, ABCD, 047
	area Set38, A, 048
	area Set39, A, 049
	area Set40, A, 050
	area Set41, ABCD, 051
	area Set42, ABCD, 052
	area Set43, ABCD, 053, 054
	area Set44, A, 055, 056, 057, 058, 059, 060
	area Set45, ABCD, 061
	recolor 03, 062
	area Set46, ABCD, 063, 064, 065, 066, 067, 068
	area Set47, ABCD, 069, 070, 071, 072, 073
	recolor 04, 074
	area Set48, ABCD, 075
	area Set49, A, 076
	recolor 05, 077
	recolor 06, 078, 079, 080, 081, 082, 083, 084, 085, 086
	recolor 07, 087, 088
	area Set50, ABCD, 089, 090
	area Set51, ABCD, 091, 092, 093
	area Set52, ABCD, 094
	recolor 08, 095, 096, 097
	area Set53, AB, 098
	recolor 09, 099
	recolor 10, 100
	area Set54, ABC, 101
	area Set55, A, 102
	area Set56, ABCD, 103
	recolor 11, 104
	area Set57, ABCD, 105
	recolor 12, 106
	recolor 13, 107
	area Set58, ABCD, 108
	recolor 14, 109
	recolor 15, 110
	recolor 16, 111
	recolor 17, 112
	resource , Tileset_Palette18
	recolor 19, 113
	recolor 20, 114
	recolor 21, 115
	recolor 22, 116
	recolor 23, 117
	recolor 24, 118
	recolor 25, 119
	recolor 26, 120
	recolor 27, 121, 122
	recolor 28, 123
	area Set59, ABCD, 124
	recolor 29, 125
	recolor 30, 126
	area Set60, ABCD, 127
	area Set61, ABCD, 128
	area Set62, ABCD, 129
	recolor 31, 130
	recolor 32, 131, 132, 133
	resource , Field_Layout134
	resource , Field_Layout135
	resource , Field_GondowanPassage
	resource , Tileset_Palette33
	recolor 34, 136
	recolor 35, 137, 138
	recolor 36, 139
	recolor 37, 140, 141, 142, 143
	recolor 38, 144
	area Set63, A, 145
	recolor 39, 146, 147
	recolor 40, 148, 149
	area Set64, ABCD, 150
	area Set65, ABCD, 151
	recolor 41, 152
	recolor 42, 153, 154
	recolor 43, 155
	recolor 44, 156, 157
	resource , Tileset_Palette45
	recolor 46, 158
	recolor 47, 159
	recolor 48, 160
	resource , Field_Layout161
	resource , Field_Layout162
	resource , Field_VenusEntrance
	resource , Tileset_Palette49
	recolor 50, 163
	recolor 51, 164
	recolor 52, 165, 166, 167, 168
	area Set66, ABCD, 169
	area Set67, ABCD, 170
	recolor 53, 171
	area Set68, ABCD, 172
	area Set69, ABCD, 173
	area Set70, ABCD, 174
	area Set71, ABCD, 175
	area Set72, ABCD, 176
	area Set73, ABCD, 177
	area Set74, ABCD, 178
	area Set75, ABCD, 179
	overlays 36F, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 37A
	overlays 37B, 37C, 37D, 37E, 37F, 380, 381, 382, 383, 384, 385, 386
	overlays 387, 388, 389, 38A, 38B, 38C, 38D, 38E, 38F, 390, 391, 392
	overlays 393, 394, 395, 396, 397, 398, 399, 39A, 39B, 39C, 39D, 39E
	overlays 39F, 3A0, 3A1, 3A2, 3A3, 3A4, 3A5, 3A6, 3A7, 3A8, 3A9, 3AA
	overlays 3AB, 3AC, 3AD, 3AE, 3AF, 3B0, 3B1, 3B2, 3B3, 3B4, 3B5, 3B6
	overlays 3B7, 3B8, 3B9, 3BA, 3BB, 3BC, 3BD, 3BE, 3BF, 3C0, 3C1, 3C2
	overlays 3C3, 3C4, 3C5, 3C6, 3C7, 3C8, 3C9, 3CA, 3CB, 3CC, 3CD, 3CE
	unused 25

@ Build timestamp: year, month, day, hour, minute and second, two digits each
	.global Resource_BuildStamp
Resource_BuildStamp:
	.ifdef TBS_EDITION_JA
	.asciz "010628184502\n"
	.endif
	.ifdef TBS_EDITION_EN
	.asciz "010918151347\n"
	.endif
	.ifdef TBS_EDITION_FR
	.asciz "011219190202\n"
	.endif
	.ifdef TBS_EDITION_DE
	.asciz "020109152237\n"
	.endif
	.ifdef TBS_EDITION_ES
	.asciz "020205181231\n"
	.endif
	.ifdef TBS_EDITION_IT
	.asciz "020206164027\n"
	.endif
	.balign 4
