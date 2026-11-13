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
@   tileset SET, count, maps... Maps, then palette and tiles A onward; count is ABCD or -.
@   recolor NN, maps...    Maps followed by a palette; reuse the preceding tiles.
@   overlays ID...         Scene overlays in ID order.
@   unused N               N unused entries.
@ A row's number is its place, so a row added or moved renumbers every row
@ after it. Code naming a resource by its symbol follows, but some numbers
@ travel bare: the scene table (field/scenes.s) names each scene's overlay by
@ number, overlay.ld numbers each overlay's objects, the layouts are counted on
@ from ResourceId_Map001 (MapGrid_LoadMap), and the settings menu's marker
@ steps through eight pictures from ResourceId_MenuCursorPictures.
	.macro group kind:req, names:vararg
	.irp n, \names
	resource \n, \kind\()_\n
	.endr
	.endm

	.macro tileset set:req, sheets:req, map_ids:vararg
	.irp m, \map_ids
	resource , Field_\m
	.endr
	resource , Tileset_\set\()Palette
	.ifnc \sheets, -
	.irpc s, \sheets
	resource , Tileset_\set\()Tiles\s
	.endr
	.endif
	.endm

	.macro recolor palette:req, map_ids:vararg
	.irp m, \map_ids
	resource , Field_\m
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
@ XXX (no evidence): why the first 19 rows point at pieces of the main image;
@ only the build stamp's is read, the far-call rows go only to stubs that do
@ nothing (Stub_NoOpHook, Stub_ReturnZero), and nothing in either game loads a
@ bank through its row.
	resource , Rom_Start
	resource , Resource_DirectoryTable
	group Resource, BuildStamp
	resource , FarCall_FieldTable
	resource , FarCall_WindowTable
	resource , FarCall_GameTable
	resource , FarCall_OverworldTable
	resource , FarCall_MenuTable
	resource , FarCall_ShopTable
	resource , FarCall_BattleTable
	resource , FarCall_CreditsTable
	resource TitleFarCalls, FarCall_TitleTable
	resource BattleEffectFarCalls, FarCall_BattleEffectTable
	resource , FarCall_LuckyDiceTable
	resource , FarCall_LuckyWheelsTable
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
	resource LuckyWheelsMachinePicture, LuckyWheels_MachinePicture
	resource LuckyWheelsIcons, LuckyWheels_Icons
	resource LuckyWheelsJackpot, LuckyWheels_Jackpot
	resource LuckyDiceBoard, LuckyDice_Board
	resource LuckyDiceSprites, LuckyDice_Sprites
	group BattleEffect, TileAnimationMask, CyanSparkSheet, VioletPaletteA, RedPaletteA, YellowPaletteA
	group BattleEffect, StarBurstSheet, CrescentSheet, RevealMaskA, RevealMaskB, RevealMaskC
	group BattleEffect, RevealMaskD, RevealMaskE, RevealMaskF, LimeColumnSheet, RainSheet
	group BattleEffect, FlameSheetA, DemonFaceSheet, FlameBladeSheet, SwordSheet, IceTileSheet
	group BattleEffect, FiveModeImage, SpiderWebSheet, VioletLightningSheetA, YellowSparkSheet, ProjectileVolleyImage
	group BattleEffect, ShurikenSheet, FlareSheet, FlareImage, VioletPaletteB, FireSwirlSheet
	group BattleEffect, VioletLightningSheetB, FireBurstSheet, BlueCrescentSheet, OverlayImageA, VioletMoteSheet
	group BattleEffect, OverlayImageB, FireballSheet, MemberBurstImage, SlashSheet, WaveSheet
	group BattleEffect, TargetBurstImage, PinkStarSheet, WaterSpraySheet, BlueFlameSheet, ExplosionSheet
	group BattleEffect, MagentaSwirlSheet, MagentaTailSheet, ParticleSpritesA, ParticleSpritesB, ParticleSpritesC
	group BattleEffect, ParticleSpritesD, GreenPalette, SwordSlashSheet, RuneSheet, FlameColumnSheet
	group BattleEffect, EarthWallSheet, OrangePaletteA, FlashBurstSheet, ThornSheet, CometSheetA
	group BattleEffect, CometSheetB, CometSheetC, WhirlwindSheet, GreenPillarSheet, VineSheet
	group BattleEffect, FirePillarSheetA, RedPaletteB, OrangePaletteB, VioletCometSheet, FlameballSheet
	group BattleEffect, BoulderSheet, RockWallSheet, StarDotSheet, MarsDjinnSheet, MarsDjinnSmallSheet
	group BattleEffect, JupiterDjinnSheet, JupiterDjinnSmallSheet, MercuryDjinnSheet, MercuryDjinnSmallSheet, VenusDjinnSheet
	group BattleEffect, VenusDjinnSmallSheet, FireBlobSheet, FireStreakSheet, LavaOrbSheet, YellowRingSheet
	group BattleEffect, YellowOrbSheet, BubbleSheet, ShieldSheet, DaggerSheet, PaletteRampImage
	group BattleEffect, SmokeSheet, BluePalette, LimePalette, PinkPalette, VioletPaletteC
	group BattleEffect, CyanPalette, TanPalette, WispSheet, GreenVineSheet, PortalSheet
	group BattleEffect, SheepSheet, SkullSheet, HeartSheet, CounterRevealSheet, RedCrescentSheetA
	group BattleEffect, BlueBeastSheet, RedCrescentSheetB, SpiralSheet, VioletStreakSheet, CrescentMoonSheet
	group BattleEffect, ThornVineSheet, IceShardSheet, EmberStreakSheet, BlueArcSheetA, BlueArcSheetB
	group BattleEffect, GlowOrbSheet, IceChipSheet, IceBlockSheet, SparkleDots, PinkBurstSheet
	group BattleEffect, LightFanSheet, BlueBurstSheet, DustPuffSheet, GoldShellSheet, BlastSheet
	group BattleEffect, FlameSheetB, FirePillarSheetB, RockSpireSheet, LightningBoltSheet, BeamImage
	group BattleEffect, TwelveModeImage, WindStreakSheet, YellowPaletteB, BlueRingSheet, RedPaletteC
	group BattleEffect, VioletPaletteD, BlueBeamSheet, VortexSheet, TornadoSheet, CrystalSheet
	group BattleEffect, WaterSparkleSheet, LightningPillarSheet, StagedParticlePath, DualStreamPath
	group Field, PerspectiveDataA, DefaultGridCells, DefaultTileAnimation, DefaultCellPictureAttributes
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
	resource MenuCursorPictures, MenuCursor_PictureA
	resource , MenuCursor_PictureB
	resource , MenuCursor_PictureC
	resource , MenuCursor_PictureD
	resource , MenuCursor_PictureE
	resource , MenuCursor_PictureF
	resource , MenuCursor_PictureG
	resource , MenuCursor_PictureH
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
	group Field, Map001
	resource , Tileset_BlankPalette
	resource , Tileset_BlankTilesA
	resource , Tileset_BlankTilesB
	resource , Tileset_BlankTilesC
	resource , Tileset_BlankTilesD
	tileset Set00, ABCD, ValePrologue
	tileset Set01, ABCD, Vale
	tileset Set02, ABCD, ValePlaza
	tileset Set03, ABCD, ValeKradensCottage
	tileset Set04, ABCD, CottageRoomsA
	tileset Set05, ABCD, CottageRoomsB
	tileset Set06, ABCD, Sanctum
	tileset Set07, ABCD, SolEntrance
	tileset Set08, ABCD, SolSanctumLunaSolRooms
	tileset Set09, ABC, SolSanctumStatue
	tileset Set10, ABCD, SolSanctumPsynergyStone
	tileset Set11, ABCD, SolSanctumMaze, SolSanctumCorridor
	tileset Set12, ABCD, SolSanctumElementalStarRoom
	tileset Set13, ABCD, SolSanctumEruption
	recolor 00, SolSanctumReliefs
	tileset Set14, ABCD, Vault
	tileset Set15, ABCD, VaultLogRooms
	tileset Set16, ABCD, StoneRooms
	tileset Set17, ABCD, GomaCaveEntrance
	tileset Set18, ABCD, GomaCavePond
	tileset Set19, ABCD, GomaCavePillar
	tileset Set20, ABCD, GomaCaveStream, GomaCaveBridge, VaultCavePassage
	tileset Set21, ABCD, VaultCaveVambrace, VaultCaveMouth, ValeCaveMouth
	tileset Set22, ABC, ValeCaveKite
	tileset Set23, ABCD, ValeCaveLogs, Bilibin
	tileset Set24, AB, CottageRoomsC
	tileset Set25, A, BilibinTreasury
	tileset Set26, ABC, BilibinBarricade
	tileset Set27, ABCD, McCoyPalaceGrounds
	tileset Set28, ABCD, McCoyPalace
	tileset Set29, ABCD, Kolima
	tileset Set30, A, LogRoomsC
	tileset Set31, ABCD, KolimaBridge
	recolor 01, KolimaTunnel
	tileset Set32, ABCD, KolimaForestEntrance
	tileset Set33, A, KolimaForestGlade
	recolor 02, KolimaForestCliff
	tileset Set34, ABCD, KolimaForestFloodgate
	tileset Set35, ABCD, KolimaForestTree
	tileset Set36, ABCD, TreeRooms
	tileset Set37, ABCD, TretTreeBranch
	tileset Set38, A, BilibinCaveEntrance
	tileset Set39, A, BilibinCaveRapier
	tileset Set40, A, BilibinCaveIce
	tileset Set41, ABCD, Imil
	tileset Set42, ABCD, IceRooms
	tileset Set43, ABCD, ImilFallsCave, MercuryLighthouseEntrance
	tileset Set44, A, MercuryLighthouseLower, MercuryLighthouseMiddle, MercuryLighthouseUpper, MercuryLighthouseExterior, MercuryLighthouseAerie, FuchinTempleGrounds
	tileset Set45, ABCD, LacquerRoomsA
	recolor 03, FuchinFallsCaveEntrance
	tileset Set46, ABCD, FuchinFallsCaveZephyr, FuchinFallsCaveDragonsEye, FuchinFallsCaveArcticBlade, FuchinFallsCaveOrbOfForce, FuchinFallsCaveDragonStatue, MogallForestSign
	tileset Set47, ABCD, MogallForestQuartz, MogallForestKillerApe, MogallForestSidePaths, Xian, LacquerRoomsB
	recolor 04, AlpineCrossing
	tileset Set48, ABCD, Altin
	tileset Set49, A, CottageRoomsD
	recolor 05, AltinPeakFirstWaterBeast
	recolor 06, AltinPeakRailTunnel, AltinPeakSecondWaterBeast, AltinPeakSouthEntrance, AltinPeakThirdWaterBeast, AltinPeakPool, AltinPeakSpritz, AltinPeakJunction, AltinPeakFallingRocks, AltinPeakHydrosStatue
	recolor 07, AltinPeakWestExit, LamakanDesertCave
	tileset Set50, ABCD, LamaTemple, LamakanDesertEast
	tileset Set51, ABCD, LamakanDesertSmog, LamakanDesertManticore, Kalay
	tileset Set52, ABCD, StuccoRoomsA
	recolor 08, KalaySanctum, HammetPalaceGrounds, HammetPalace
	tileset Set53, AB, Lunpa
	recolor 09, RockRooms
	recolor 10, WestLunpaCave
	tileset Set54, ABC, KalayDocks
	tileset Set55, A, CottageRoomsE
	tileset Set56, ABCD, TolbiShipDeck
	recolor 11, TolbiShipMast
	tileset Set57, ABCD, ShipRooms
	recolor 12, TolbiDocks
	recolor 13, CrossboneIsle
	tileset Set58, ABCD, Map108
	recolor 14, CrossboneIsleIcePillars
	recolor 15, Map110
	recolor 16, CrossboneIsleStatues
	recolor 17, CrossboneIsleFloodgate
	resource , Tileset_Palette18
	recolor 19, CrossboneIsleStackedPillars
	recolor 20, CrossboneIsleDragonStatue
	recolor 21, CrossboneIsleFloatingLogs
	recolor 22, CrossboneIslePier
	recolor 23, PirateShip
	recolor 24, CrossboneIsleCave
	recolor 25, Tolbi
	recolor 26, StuccoRoomsB
	recolor 27, StuccoRoomsC, BabiPalaceEntrance
	recolor 28, BabiPalace
	tileset Set59, ABCD, ColosseumTunnel
	recolor 29, Colosseum
	recolor 30, ColosseumEntrance
	tileset Set60, ABCD, ColosseumAzartMatch
	tileset Set61, ABCD, ColosseumSatrageMatch
	tileset Set62, ABCD, ColosseumNavampaMatch
	recolor 31, AltmillerCaveEntrance
	recolor 32, AltmillerCaveBabi, AltmillerCaveVialChest, AltmillerCaveSquall
	resource , Field_AltmillerCaveCookieChest
	resource , Field_AltmillerCaveDraught
	resource , Field_GondowanPassage
	resource , Tileset_Palette33
	recolor 34, GondowanCaveRiver
	recolor 35, GondowanCave, LunpaFortressGates
	recolor 36, LunpaFortressGroundFloor
	recolor 37, LunpaFortressUpperFloor, LunpaFortressCave, LunpaFortressToadonpa, KalayTunnelEntrance
	recolor 38, KalayTunnelCave
	tileset Set63, A, KalayTunnelPassage
	recolor 39, KalayTunnelTreasureRoom, SuhallaDesertNorth
	recolor 40, SuhallaDesertSouth, SuhallaDesertCaves
	tileset Set64, ABCD, Suhalla
	tileset Set65, ABCD, TimberRoomsA
	recolor 41, SuhallaGateCheckpoint
	recolor 42, SuhallaGateDew, GatewayCave
	recolor 43, TunnelRuinsGreatStatue
	recolor 44, TunnelRuinsLadyStatues, BabiLighthouseEntry
	resource , Tileset_Palette45
	recolor 46, BabiLighthouseStoneDoor
	recolor 47, Lalivero
	recolor 48, TimberRoomsB
	resource , Field_TimberRoomsC
	resource , Field_RoadToBabiLighthouse
	resource , Field_VenusEntrance
	resource , Tileset_Palette49
	recolor 50, VenusLighthouseGroundFloor
	recolor 51, VenusLighthouseFirstFloor
	recolor 52, VenusLighthouseSecondFloor, VenusLighthouseThirdFloor, VenusLighthouseFourthFloor, VenusLighthouseAerie
	tileset Set66, ABCD, LemurianShip
	tileset Set67, ABCD, TolbiSpring
	recolor 53, BattleArena
	tileset Set68, ABCD, Test0
	tileset Set69, ABCD, Test1
	tileset Set70, ABCD, Test2
	tileset Set71, ABCD, Test3
	tileset Set72, ABCD, Test4
	tileset Set73, ABCD, Test5
	tileset Set74, ABCD, Test6
	tileset Set75, ABCD, Test7
	overlays 36F, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 37A
	overlays 37B, 37C, 37D, 37E, 37F, 380, 381, 382, 383, 384, 385, 386
	overlays 387, 388, 389, 38A, 38B, 38C, 38D, 38E, 38F, 390, 391, 392
	overlays 393, 394, 395, 396, 397, 398, 399, 39A, 39B, 39C, 39D, 39E
	overlays 39F, 3A0, 3A1, 3A2, 3A3, 3A4, 3A5, 3A6, 3A7, 3A8, 3A9, 3AA
	overlays 3AB, 3AC, 3AD, 3AE, 3AF, 3B0, 3B1, 3B2, 3B3, 3B4, 3B5, 3B6
	overlays 3B7, 3B8, 3B9, 3BA, 3BB, 3BC, 3BD, 3BE, 3BF, 3C0, 3C1, 3C2
	overlays 3C3, 3C4, 3C5, 3C6, 3C7, 3C8, 3C9, 3CA, 3CB, 3CC, 3CD, 3CE
@ XXX (no evidence): whether these rows were left for later resources; they
@ are empty and nothing reads them.
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
