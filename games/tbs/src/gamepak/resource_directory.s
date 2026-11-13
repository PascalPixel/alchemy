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
	tileset ValePrologue, ABCD, ValePrologue
	tileset Set01, ABCD, Vale
	tileset Set02, ABCD, ValePlaza
	tileset ValeKradensCottage, ABCD, ValeKradensCottage
	tileset Set04, ABCD, CottageRoomsA
	tileset CottageRoomsB, ABCD, CottageRoomsB
	tileset Sanctum, ABCD, Sanctum
	tileset Set07, ABCD, SolEntrance
	tileset Set08, ABCD, SolSanctumLunaSolRooms
	tileset SolSanctumStatue, ABC, SolSanctumStatue
	tileset SolSanctumPsynergyStone, ABCD, SolSanctumPsynergyStone
	tileset SolSanctumCorridor, ABCD, SolSanctumMaze, SolSanctumCorridor
	tileset SolSanctumElementalStarRoom, ABCD, SolSanctumElementalStarRoom
	tileset SolSanctumEruption, ABCD, SolSanctumEruption
	recolor SolSanctumReliefs, SolSanctumReliefs
	tileset Vault, ABCD, Vault
	tileset Set15, ABCD, VaultLogRooms
	tileset StoneRooms, ABCD, StoneRooms
	tileset Set17, ABCD, GomaCaveEntrance
	tileset GomaCavePond, ABCD, GomaCavePond
	tileset Set19, ABCD, GomaCavePillar
	tileset Set20, ABCD, GomaCaveStream, GomaCaveBridge, VaultCavePassage
	tileset Set21, ABCD, VaultCaveVambrace, VaultCaveMouth, ValeCaveMouth
	tileset Set22, ABC, ValeCaveKite
	tileset Set23, ABCD, ValeCaveLogs, Bilibin
	tileset Set24, AB, CottageRoomsC
	tileset Set25, A, BilibinTreasury
	tileset BilibinBarricade, ABC, BilibinBarricade
	tileset Set27, ABCD, McCoyPalaceGrounds
	tileset Set28, ABCD, McCoyPalace
	tileset Set29, ABCD, Kolima
	tileset LogRoomsC, A, LogRoomsC
	tileset Set31, ABCD, KolimaBridge
	recolor KolimaTunnel, KolimaTunnel
	tileset KolimaForest, ABCD, KolimaForestEntrance
	tileset KolimaForestGlade, A, KolimaForestGlade
	recolor KolimaForestCliff, KolimaForestCliff
	tileset Set34, ABCD, KolimaForestFloodgate
	tileset KolimaForestTree, ABCD, KolimaForestTree
	tileset TreeRooms, ABCD, TreeRooms
	tileset TretTreeBranch, ABCD, TretTreeBranch
	tileset BilibinCaveEntrance, A, BilibinCaveEntrance
	tileset Set39, A, BilibinCaveRapier
	tileset BilibinCaveIce, A, BilibinCaveIce
	tileset Imil, ABCD, Imil
	tileset Set42, ABCD, IceRooms
	tileset Set43, ABCD, ImilFallsCave, MercuryLighthouseEntrance
	tileset Set44, A, MercuryLighthouseLower, MercuryLighthouseMiddle, MercuryLighthouseUpper, MercuryLighthouseExterior, MercuryLighthouseAerie, FuchinTempleGrounds
	tileset LacquerRoomsA, ABCD, LacquerRoomsA
	recolor FuchinFallsCave, FuchinFallsCaveEntrance
	tileset MogallForest, ABCD, FuchinFallsCaveZephyr, FuchinFallsCaveDragonsEye, FuchinFallsCaveArcticBlade, FuchinFallsCaveOrbOfForce, FuchinFallsCaveDragonStatue, MogallForestSign
	tileset LacquerRoomsB, ABCD, MogallForestQuartz, MogallForestKillerApe, MogallForestSidePaths, Xian, LacquerRoomsB
	recolor AlpineCrossing, AlpineCrossing
	tileset Altin, ABCD, Altin
	tileset CottageRoomsD, A, CottageRoomsD
	recolor AltinPeak, AltinPeakFirstWaterBeast
	recolor AltinPeakHydrosStatue, AltinPeakRailTunnel, AltinPeakSecondWaterBeast, AltinPeakSouthEntrance, AltinPeakThirdWaterBeast, AltinPeakPool, AltinPeakSpritz, AltinPeakJunction, AltinPeakFallingRocks, AltinPeakHydrosStatue
	recolor LamakanDesertCave, AltinPeakWestExit, LamakanDesertCave
	tileset Set50, ABCD, LamaTemple, LamakanDesertEast
	tileset Set51, ABCD, LamakanDesertSmog, LamakanDesertManticore, Kalay
	tileset Set52, ABCD, StuccoRoomsA
	recolor HammetPalace, KalaySanctum, HammetPalaceGrounds, HammetPalace
	tileset Lunpa, AB, Lunpa
	recolor RockRooms, RockRooms
	recolor WestLunpaCave, WestLunpaCave
	tileset Set54, ABC, KalayDocks
	tileset CottageRoomsE, A, CottageRoomsE
	tileset Set56, ABCD, TolbiShipDeck
	recolor TolbiShipMast, TolbiShipMast
	tileset ShipRooms, ABCD, ShipRooms
	recolor TolbiDocks, TolbiDocks
	recolor CrossboneIsle, CrossboneIsle
	tileset Set58, ABCD, Map108
	recolor CrossboneIsleIcePillars, CrossboneIsleIcePillars
	recolor 15, Map110
	recolor CrossboneIsleStatues, CrossboneIsleStatues
	recolor CrossboneIsleFloodgate, CrossboneIsleFloodgate
	resource , Tileset_Palette18
	recolor CrossboneIsleStackedPillars, CrossboneIsleStackedPillars
	recolor CrossboneIsleDragonStatue, CrossboneIsleDragonStatue
	recolor CrossboneIsleFloatingLogs, CrossboneIsleFloatingLogs
	recolor CrossboneIslePier, CrossboneIslePier
	recolor PirateShip, PirateShip
	recolor CrossboneIsleCave, CrossboneIsleCave
	recolor Tolbi, Tolbi
	recolor StuccoRooms, StuccoRoomsB
	recolor BabiPalaceEntrance, StuccoRoomsC, BabiPalaceEntrance
	recolor BabiPalace, BabiPalace
	tileset Set59, ABCD, ColosseumTunnel
	recolor Colosseum, Colosseum
	recolor ColosseumEntrance, ColosseumEntrance
	tileset ColosseumAzartMatch, ABCD, ColosseumAzartMatch
	tileset ColosseumSatrageMatch, ABCD, ColosseumSatrageMatch
	tileset ColosseumNavampaMatch, ABCD, ColosseumNavampaMatch
	recolor 31, AltmillerCaveEntrance
	recolor 32, AltmillerCaveBabi, AltmillerCaveVialChest, AltmillerCaveSquall
	resource , Field_AltmillerCaveCookieChest
	resource , Field_AltmillerCaveDraught
	resource , Field_GondowanPassage
	resource , Tileset_PaletteGondowanPassage
	recolor GondowanCave, GondowanCaveRiver
	recolor LunpaFortressGates, GondowanCave, LunpaFortressGates
	recolor LunpaFortress, LunpaFortressGroundFloor
	recolor KalayTunnelEntrance, LunpaFortressUpperFloor, LunpaFortressCave, LunpaFortressToadonpa, KalayTunnelEntrance
	recolor KalayTunnelCave, KalayTunnelCave
	tileset KalayTunnel, A, KalayTunnelPassage
	recolor SuhallaDesert, KalayTunnelTreasureRoom, SuhallaDesertNorth
	recolor SuhallaDesertCaves, SuhallaDesertSouth, SuhallaDesertCaves
	tileset Set64, ABCD, Suhalla
	tileset TimberRooms, ABCD, TimberRoomsA
	recolor SuhallaGate, SuhallaGateCheckpoint
	recolor GatewayCave, SuhallaGateDew, GatewayCave
	recolor TunnelRuins, TunnelRuinsGreatStatue
	recolor 44, TunnelRuinsLadyStatues, BabiLighthouseEntry
	resource , Tileset_Palette45
	recolor BabiLighthouseStoneDoor, BabiLighthouseStoneDoor
	recolor Lalivero, Lalivero
	recolor TimberRooms, TimberRoomsB
	resource , Field_TimberRoomsC
	resource , Field_RoadToBabiLighthouse
	resource , Field_VenusEntrance
	resource , Tileset_PaletteVenusEntrance
	recolor VenusLighthouseGroundFloor, VenusLighthouseGroundFloor
	recolor VenusLighthouse, VenusLighthouseFirstFloor
	recolor VenusLighthouseAerie, VenusLighthouseSecondFloor, VenusLighthouseThirdFloor, VenusLighthouseFourthFloor, VenusLighthouseAerie
	tileset LemurianShip, ABCD, LemurianShip
	tileset TolbiSpring, ABCD, TolbiSpring
	recolor BattleArena, BattleArena
	tileset Test0, ABCD, Test0
	tileset Test1, ABCD, Test1
	tileset Test2, ABCD, Test2
	tileset Test3, ABCD, Test3
	tileset Test4, ABCD, Test4
	tileset Test5, ABCD, Test5
	tileset Test6, ABCD, Test6
	tileset Test7, ABCD, Test7
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
