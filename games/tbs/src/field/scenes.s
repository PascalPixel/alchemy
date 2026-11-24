	.include "enums/area_ids.inc"

@ Every scene (struct Scene), in the order that numbers them: a named row gives
@ SceneId_NAME its number, which C reads at link time (scene_ids.h). Each row is
@ the overlay the scene loads, by the name its row of the resource directory
@ gives it (OverlayId_NAME), its area (enum AreaId), its type (enum SceneType)
@ and its map, by the name its row of gMaps gives it (MapIndex_NAME,
@ map/resources.s).
	.macro scene name, overlay:req, area:req, kind:req, map:req
	.ifnb \name
	.global SceneId_\name
	.equiv SceneId_\name, (. - gScenes) / 8
	.endif
	.2byte OverlayId_\overlay
	.byte \area, \kind
	.2byte MapIndex_\map, 0
	.endm

	.section .rodata
	.balign 4
	.global gScenes
gScenes:
	scene Title, Title, AREA_TITLE_SCREEN, 1, EmptyMap
	scene FileScreen, FileScreen, AREA_CLEAR_DATA, 1, EmptyMap
	scene WorldMap, WorldMap, AREA_WORLD_MAP, 1, World
	scene ValePrologue, ValePrologue, AREA_VALE, 2, ValePrologue
	scene Vale, Vale, AREA_VALE, 2, Vale
	scene ValePlaza, ValePlaza, AREA_VALE, 2, ValePlaza
	scene ValeKradensCottage, ValeKradensCottage, AREA_VALE, 2, ValeKradensCottage
	scene CottageRoomsA, CottageRoomsA, AREA_VALE, 2, CottageRoomsA
	scene CottageRoomsB, CottageRoomsB, AREA_VALE, 2, CottageRoomsB
	scene Sanctum, Sanctum, AREA_VALE, 2, Sanctum
	scene ValeEruption, ValeEruption, AREA_VALE, 2, ValeEruption
	scene SolSanctumLunaSolRooms, SolSanctumLunaSolRooms, AREA_SOL_SANCTUM, 1, SolSanctumLunaSolRooms
	scene SolSanctumStatue, SolSanctumStatue, AREA_SOL_SANCTUM, 1, SolSanctumStatue
	scene SolSanctumPsynergyStone, SolSanctumPsynergyStone, AREA_SOL_SANCTUM, 1, SolSanctumPsynergyStone
	scene SolSanctumMaze, SolSanctumMaze, AREA_SOL_SANCTUM, 1, SolSanctumMaze
	scene SolSanctumCorridor, SolSanctumCorridor, AREA_SOL_SANCTUM, 1, SolSanctumCorridor
	scene SolSanctumReliefs, SolSanctumEntrance, AREA_SOL_SANCTUM, 1, SolSanctumReliefs
	scene SolSanctumElementalStarRoom, SolSanctumElementalStarRoom, AREA_SOL_SANCTUM, 1, SolSanctumElementalStarRoom
	scene SolSanctumEruption, SolSanctumEruption, AREA_SOL_SANCTUM, 1, SolSanctumEruption
	scene SolEntrance, SolSanctumEntrance, AREA_SOL_SANCTUM, 1, SolEntrance
	scene Vault, Vault, AREA_VAULT, 2, Vault
	scene LogRoomsA, LogRoomsA, AREA_VAULT, 2, VaultLogRooms
	scene StoneRooms, StoneRooms, AREA_VAULT, 2, StoneRooms
	scene VaultRevisit, VaultRevisit, AREA_VAULT, 2, Vault
	scene LogRoomsB, LogRoomsB, AREA_VAULT, 2, VaultLogRooms
	scene GomaCaveEntrance, GomaCaveEntrance, AREA_GOMA_CAVE_ENTRANCE, 1, GomaCaveEntrance
	scene GomaCavePond, GomaCavePond, AREA_GOMA_CAVE, 1, GomaCavePond
	scene GomaCavePillar, GomaCavePillar, AREA_GOMA_CAVE, 1, GomaCavePillar
	scene GomaCaveStream, GomaCaveStream, AREA_GOMA_CAVE, 1, GomaCaveStream
	scene GomaCaveBridge, GomaCaveStream, AREA_GOMA_CAVE, 1, GomaCaveBridge
	scene Bilibin, Bilibin, AREA_BILIBIN, 2, Bilibin
	scene CottageRoomsC, CottageRoomsC, AREA_BILIBIN, 2, CottageRoomsC
	scene BilibinTreasury, Bilibin, AREA_BILIBIN, 2, BilibinTreasury
	scene McCoyPalace, McCoyPalace, AREA_MCCOYS_PALACE, 2, McCoyPalace
	scene McCoyPalaceGrounds, McCoyPalaceGrounds, AREA_MCCOYS_PALACE, 2, McCoyPalaceGrounds
	scene BilibinBarricade, Bilibin, AREA_BILIBIN_BARRICADE, 1, BilibinBarricade
	scene Kolima, Kolima, AREA_KOLIMA, 2, Kolima
	scene LogRoomsC, LogRoomsC, AREA_KOLIMA, 2, LogRoomsC
	scene KolimaTunnel, Kolima, AREA_KOLIMA, 2, KolimaTunnel
	scene KolimaBridge, Kolima, AREA_KOLIMA_BRIDGE, 1, KolimaBridge
	scene KolimaForestEntrance, KolimaForestEntrance, AREA_KOLIMA_FOREST, 1, KolimaForestEntrance
	scene KolimaForestGlade, KolimaForestGlade, AREA_KOLIMA_FOREST, 1, KolimaForestGlade
	scene KolimaForestCliff, KolimaForestCliff, AREA_KOLIMA_FOREST, 1, KolimaForestCliff
	scene KolimaForestFloodgate, KolimaForestFloodgate, AREA_KOLIMA_FOREST, 1, KolimaForestFloodgate
	scene KolimaForestTree, KolimaForestTree, AREA_KOLIMA_FOREST, 1, KolimaForestTree
	scene TreeRooms, TreeRooms, AREA_TRET_TREE, 1, TreeRooms
	scene TretTreeBranch, TretTreeBranch, AREA_TRET_TREE, 1, TretTreeBranch
	scene BilibinCaveIce, BilibinCave, AREA_BILIBIN_CAVE, 1, BilibinCaveIce
	scene BilibinCaveRapier, BilibinCave, AREA_BILIBIN_CAVE, 1, BilibinCaveRapier
	scene BilibinCaveEntrance, BilibinCave, AREA_BILIBIN_CAVE_ENTRANCE, 1, BilibinCaveEntrance
	scene Imil, Imil, AREA_IMIL, 2, Imil
	scene IceRooms, Imil, AREA_IMIL, 2, IceRooms
	scene ImilFallsCave, ImilFuchinFalls, AREA_IMIL_FALLS_CAVE, 1, ImilFallsCave
	scene MercuryLighthouseEntrance, MercuryLighthouseEntrance, AREA_MERCURY_LIGHTHOUSE, 1, MercuryLighthouseEntrance
	scene MercuryLighthouseLower, MercuryLighthouse, AREA_MERCURY_LIGHTHOUSE, 1, MercuryLighthouseLower
	scene MercuryLighthouseMiddle, MercuryLighthouse, AREA_MERCURY_LIGHTHOUSE, 1, MercuryLighthouseMiddle
	scene MercuryLighthouseUpper, MercuryLighthouse, AREA_MERCURY_LIGHTHOUSE, 1, MercuryLighthouseUpper
	scene MercuryLighthouseExterior, MercuryLighthouse, AREA_MERCURY_LIGHTHOUSE, 1, MercuryLighthouseExterior
	scene MercuryLighthouseAerie, MercuryLighthouseAerie, AREA_MERCURY_LIGHTHOUSE, 1, MercuryLighthouseAerie
	scene UnusedMercuryLighthouseAerieCopy, MercuryLighthouseAerie, AREA_MERCURY_LIGHTHOUSE, 1, MercuryLighthouseAerie
	scene FuchinTempleGrounds, FuchinTemple, AREA_FUCHIN_TEMPLE, 2, FuchinTempleGrounds
	scene LacquerRoomsA, FuchinTemple, AREA_FUCHIN_TEMPLE, 2, LacquerRoomsA
	scene FuchinFallsCaveEntrance, ImilFuchinFalls, AREA_FUCHIN_FALLS_CAVE, 1, FuchinFallsCaveEntrance
	scene FuchinFallsCaveZephyr, ImilFuchinFalls, AREA_FUCHIN_FALLS_CAVE, 1, FuchinFallsCaveZephyr
	scene FuchinFallsCaveDragonsEye, ImilFuchinFalls, AREA_FUCHIN_FALLS_CAVE, 1, FuchinFallsCaveDragonsEye
	scene FuchinFallsCaveArcticBlade, ImilFuchinFalls, AREA_FUCHIN_FALLS_CAVE, 1, FuchinFallsCaveArcticBlade
	scene FuchinFallsCaveOrbOfForce, ImilFuchinFalls, AREA_FUCHIN_FALLS_CAVE, 1, FuchinFallsCaveOrbOfForce
	scene FuchinFallsCaveDragonStatue, ImilFuchinFalls, AREA_FUCHIN_FALLS_CAVE, 1, FuchinFallsCaveDragonStatue
	scene MogallForestSign, MogallForest, AREA_MOGALL_FOREST, 1, MogallForestSign
	scene MogallForestQuartz, MogallForest, AREA_MOGALL_FOREST, 1, MogallForestQuartz
	scene MogallForestKillerApe, MogallForest, AREA_MOGALL_FOREST, 1, MogallForestKillerApe
	scene MogallForestSidePaths, MogallForest, AREA_MOGALL_FOREST, 1, MogallForestSidePaths
	scene Xian, Xian, AREA_XIAN, 2, Xian
	scene LacquerRoomsB, LacquerRoomsB, AREA_XIAN, 2, LacquerRoomsB
	scene AlpineCrossing, AlpineCrossingLamaTemple, AREA_ALPINE_CROSSING, 1, AlpineCrossing
	scene Altin, Altin, AREA_ALTIN, 2, Altin
	scene CottageRoomsD, Altin, AREA_ALTIN, 2, CottageRoomsD
	scene AltinPeakFirstWaterBeast, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakFirstWaterBeast
	scene AltinPeakRailTunnel, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakRailTunnel
	scene AltinPeakSecondWaterBeast, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakSecondWaterBeast
	scene AltinPeakSouthEntrance, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakSouthEntrance
	scene AltinPeakThirdWaterBeast, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakThirdWaterBeast
	scene AltinPeakPool, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakPool
	scene AltinPeakSpritz, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakSpritz
	scene AltinPeakJunction, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakJunction
	scene AltinPeakFallingRocks, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakFallingRocks
	scene AltinPeakHydrosStatue, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakHydrosStatue
	scene AltinPeakWestExit, AltinPeak, AREA_ALTIN_PEAK, 1, AltinPeakWestExit
	scene LamaTemple, AlpineCrossingLamaTemple, AREA_LAMA_TEMPLE, 2, LamaTemple
	scene LamakanDesertEast, LamakanDesert, AREA_LAMAKAN_DESERT, 1, LamakanDesertEast
	scene LamakanDesertSmog, LamakanDesert, AREA_LAMAKAN_DESERT, 1, LamakanDesertSmog
	scene LamakanDesertManticore, LamakanDesert, AREA_LAMAKAN_DESERT, 1, LamakanDesertManticore
	scene LamakanDesertCave, LamakanDesert, AREA_LAMAKAN_DESERT, 1, LamakanDesertCave
	scene ValeCaveMouth, ValeCave, AREA_VALE_CAVE_MOUTH, 1, ValeCaveMouth
	scene ValeCaveKite, ValeCave, AREA_VALE_CAVE, 1, ValeCaveKite
	scene ValeCaveLogs, ValeCave, AREA_VALE_CAVE, 1, ValeCaveLogs
	scene VaultCavePassage, VaultCave, AREA_VAULT_CAVE, 1, VaultCavePassage
	scene VaultCaveVambrace, VaultCave, AREA_VAULT_CAVE, 1, VaultCaveVambrace
	scene VaultCaveMouth, VaultCave, AREA_VAULT_CAVE, 1, VaultCaveMouth
	scene Kalay, Kalay, AREA_KALAY, 2, Kalay
	scene StuccoRoomsA, StuccoRoomsA, AREA_KALAY, 2, StuccoRoomsA
	scene KalaySanctum, StuccoRoomsA, AREA_KALAY, 2, KalaySanctum
	scene HammetPalaceGrounds, Kalay, AREA_HAMMET_PALACE, 2, HammetPalaceGrounds
	scene HammetPalace, HammetPalace, AREA_HAMMET_PALACE, 2, HammetPalace
	scene Lunpa, Lunpa, AREA_LUNPA, 2, Lunpa
	scene RockRooms, RockRooms, AREA_LUNPA, 2, RockRooms
	scene WestLunpaCave, WestLunpaCave, AREA_WEST_LUNPA_CAVE, 1, WestLunpaCave
	scene KalayDocks, KalayTolbiDocks, AREA_KALAY_DOCKS, 2, KalayDocks
	scene CottageRoomsE, KalayTolbiDocks, AREA_KALAY_DOCKS, 2, CottageRoomsE
	scene TolbiShipDeck, TolbiShipDeck, AREA_TOLBI_BOUND_SHIP, 1, TolbiShipDeck
	scene TolbiShipMast, TolbiShipMast, AREA_TOLBI_BOUND_SHIP, 1, TolbiShipMast
	scene ShipRooms, ShipRooms, AREA_TOLBI_BOUND_SHIP, 1, ShipRooms
	scene TolbiDocks, KalayTolbiDocks, AREA_TOLBI_DOCKS, 2, TolbiDocks
	scene CrossboneIsle, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsle
	scene CrossboneIsleCaveEntrance, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleSteppingStones, CrossboneIsleSteppingStones, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleSteppingStones
	scene CrossboneIsleIcePillars, CrossboneIsleSteppingStones, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleIcePillars
	scene CrossboneIsleKeys, CrossboneIsleKeys, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleKeys
	scene CrossboneIsleStatues, CrossboneIsleKeys, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleStatues
	scene CrossboneIsleFloodgate, CrossboneIsleSteppingStones, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleFloodgate
	scene CrossboneIsleBane, CrossboneIsleKeys, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleBane
	scene CrossboneIsleStackedPillars, CrossboneIsleSteppingStones, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleStackedPillars
	scene CrossboneIsleDragonStatue, CrossboneIsleSteppingStones, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleDragonStatue
	scene CrossboneIsleFloatingLogs, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleFloatingLogs
	scene CrossboneIslePier, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIslePier
	scene PirateShip, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, PirateShip
	scene CrossboneIsleFirstFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleSecondFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleThirdFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleFourthFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleFifthFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleSixthFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleSeventhFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleEighthFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene CrossboneIsleNinthFloor, CrossboneIsle, AREA_CROSSBONE_ISLE_CAVE, 1, CrossboneIsleCave
	scene Tolbi, Tolbi, AREA_TOLBI, 2, Tolbi
	scene StuccoRoomsB, StuccoRoomsB, AREA_TOLBI, 2, StuccoRoomsB
	scene StuccoRoomsC, TolbiGames, AREA_TOLBI, 2, StuccoRoomsC
	scene BabiPalace, BabiPalace, AREA_BABI_PALACE, 2, BabiPalace
	scene BabiPalaceEntrance, BabiPalace, AREA_BABI_PALACE, 2, BabiPalaceEntrance
	scene Colosseum, ColosseumEntrance, AREA_COLOSSEUM, 1, Colosseum
	scene ColosseumTunnel, ColosseumEntrance, AREA_COLOSSEUM, 1, ColosseumTunnel
	scene ColosseumEntrance, ColosseumEntrance, AREA_COLOSSEUM, 1, ColosseumEntrance
	scene ColosseumAzartMatch, ColosseumAzartMatch, AREA_COLOSSEUM, 1, ColosseumAzartMatch
	scene ColosseumSatrageMatch, ColosseumSatrageMatch, AREA_COLOSSEUM, 1, ColosseumSatrageMatch
	scene ColosseumNavampaMatch, ColosseumNavampaMatch, AREA_COLOSSEUM, 1, ColosseumNavampaMatch
	scene AltmillerCaveEntrance, AltmillerCave, AREA_ALTMILLER_CAVE, 1, AltmillerCaveEntrance
	scene AltmillerCaveBabi, AltmillerCave, AREA_ALTMILLER_CAVE, 1, AltmillerCaveBabi
	scene AltmillerCaveVialChest, AltmillerCave, AREA_ALTMILLER_CAVE, 1, AltmillerCaveVialChest
	scene AltmillerCaveSquall, AltmillerCave, AREA_ALTMILLER_CAVE, 1, AltmillerCaveSquall
	scene AltmillerCaveCookieChest, AltmillerCave, AREA_ALTMILLER_CAVE, 1, AltmillerCaveCookieChest
	scene AltmillerCaveDraught, AltmillerCave, AREA_ALTMILLER_CAVE, 1, AltmillerCaveDraught
	scene GondowanPassage, GondowanCave, AREA_GONDOWAN_PASSAGE, 1, GondowanPassage
	scene KalayTunnelEntrance, Kalay, AREA_KALAY_TUNNEL_ENTRANCE, 1, KalayTunnelEntrance
	scene KalayTunnelCave, Kalay, AREA_KALAY_TUNNEL, 1, KalayTunnelCave
	scene KalayTunnelPassage, Kalay, AREA_KALAY_TUNNEL, 1, KalayTunnelPassage
	scene KalayTunnelTreasureRoom, Kalay, AREA_KALAY_TUNNEL, 1, KalayTunnelTreasureRoom
	scene GondowanCaveRiver, GondowanCave, AREA_GONDOWAN_CAVE, 1, GondowanCaveRiver
	scene GondowanCave, GondowanCave, AREA_GONDOWAN_CAVE, 1, GondowanCave
	scene LunpaFortressGates, Lunpa, AREA_LUNPA_FORTRESS_GATES, 1, LunpaFortressGates
	scene LunpaFortressGroundFloor, LunpaFortress, AREA_LUNPA_FORTRESS, 1, LunpaFortressGroundFloor
	scene LunpaFortressUpperFloor, LunpaFortress, AREA_LUNPA_FORTRESS, 1, LunpaFortressUpperFloor
	scene LunpaFortressCave, LunpaFortress, AREA_LUNPA_FORTRESS, 1, LunpaFortressCave
	scene LunpaFortressToadonpa, LunpaFortress, AREA_LUNPA_FORTRESS, 1, LunpaFortressToadonpa
	scene SuhallaDesertNorth, SuhallaDesert, AREA_SUHALLA_DESERT, 1, SuhallaDesertNorth
	scene SuhallaDesertSouth, SuhallaDesert, AREA_SUHALLA_DESERT, 1, SuhallaDesertSouth
	scene SuhallaDesertCaves, SuhallaDesert, AREA_SUHALLA_DESERT, 1, SuhallaDesertCaves
	scene Suhalla, Suhalla, AREA_SUHALLA, 2, Suhalla
	scene TimberRoomsA, TimberRoomsA, AREA_SUHALLA, 2, TimberRoomsA
	scene SuhallaGateCheckpoint, SuhallaGate, AREA_SUHALLA_GATE, 1, SuhallaGateCheckpoint
	scene SuhallaGateDew, SuhallaGate, AREA_SUHALLA_GATE, 1, SuhallaGateDew
	scene GatewayCave, SuhallaGate, AREA_SUHALLA_GATE, 1, GatewayCave
	scene TunnelRuinsGreatStatue, TunnelRuins, AREA_TUNNEL_RUINS, 1, TunnelRuinsGreatStatue
	scene TunnelRuinsLadyStatues, TunnelRuins, AREA_TUNNEL_RUINS, 1, TunnelRuinsLadyStatues
	scene BabiLighthouseStoneDoor, BabiLighthouse, AREA_BABI_LIGHTHOUSE, 1, BabiLighthouseStoneDoor
	scene BabiLighthouseLuff, BabiLighthouse, AREA_BABI_LIGHTHOUSE, 1, BabiLighthouseLuff
	scene BabiLighthouseEntry, BabiLighthouse, AREA_BABI_LIGHTHOUSE, 1, BabiLighthouseEntry
	scene RoadToBabiLighthouse, BabiLighthouse, AREA_BABI_LIGHTHOUSE, 1, RoadToBabiLighthouse
	scene Lalivero, Lalivero, AREA_LALIVERO, 2, Lalivero
	scene TimberRoomsB, TimberRoomsBc, AREA_LALIVERO, 2, TimberRoomsB
	scene TimberRoomsC, TimberRoomsBc, AREA_LALIVERO, 2, TimberRoomsC
	scene VenusLighthouseEntry, VenusLighthouse, AREA_VENUS_LIGHTHOUSE_ENTRY, 1, VenusEntrance
	scene VenusLighthouseGroundFloor, VenusLighthouse, AREA_VENUS_LIGHTHOUSE_ENTRY, 1, VenusLighthouseGroundFloor
	scene VenusLighthouseFirstFloor, VenusLighthouse, AREA_VENUS_LIGHTHOUSE, 1, VenusLighthouseFirstFloor
	scene VenusLighthouseSecondFloor, VenusLighthouse, AREA_VENUS_LIGHTHOUSE, 1, VenusLighthouseSecondFloor
	scene VenusLighthouseThirdFloor, VenusLighthouse, AREA_VENUS_LIGHTHOUSE, 1, VenusLighthouseThirdFloor
	scene VenusLighthouseFourthFloor, VenusLighthouse, AREA_VENUS_LIGHTHOUSE, 1, VenusLighthouseFourthFloor
	scene VenusLighthouseAerie, VenusLighthouseAerie, AREA_VENUS_LIGHTHOUSE, 1, VenusLighthouseAerie
	scene LemurianShip, LemurianShip, AREA_LEMURIAN_SHIP, 2, LemurianShip
	scene TolbiSpring, TolbiGames, AREA_TOLBI_SPRING, 2, TolbiSpring
	scene BattleArena, BattleArena, AREA_BATTLE_ARENA, 2, BattleArena
	scene Test0, TestScenes, AREA_TEST_0, 1, Test0
	scene Test1, TestScenes, AREA_TEST_1, 1, Test1
	scene Test2, TestScenes, AREA_TEST_2, 1, Test2
	scene Test3, TestScenes, AREA_TEST_3, 1, Test3
	scene Test4, TestScenes, AREA_TEST_4, 1, Test4
	scene Test5, TestScenes, AREA_TEST_5, 1, Test5
	scene Test6, TestScenes, AREA_TEST_6, 1, Test6
	scene Test7, TestScenes, AREA_TEST_7, 1, Test7
	scene IncreaseItemAndLevel, IncreaseItemAndLevel, AREA_INCREASE_ITEM_AND_LEVEL, 1, EmptyMap
	scene MenuTest, MenuTest, AREA_MENU_TEST, 1, EmptyMap
