	.include "games/tbs/include/ed_asm.h"

@ Field music: the music of the first rule matching the scene or its area and
@ the entrance (-1 for any) whose flag (-1 for always) is set.
	.macro scene_music scene:req, entrance:req, flag:req, cue:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \cue
	.endm
	.macro area_music area:req, entrance:req, flag:req, cue:req
	.2byte \area, \entrance & 0x7fff, \flag, \cue
	.endm

@ Area names: the name of the scene or its area and the entrance (-1
@ for any).
	.macro scene_name scene:req, entrance:req, name:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro area_name area:req, entrance:req, name:req
	.2byte \area, \entrance & 0x7fff
	.4byte \name
	.endm

@ Restart points: the scene and entrance (-1 for any), then the
@ scene and entrance the party returns to after a defeat there.
	.macro restart_point scene:req, entrance:req, saved:req, saved_entrance:req
	.2byte SceneId_\scene, \entrance, SceneId_\saved, \saved_entrance
	.endm

@ A battle background and the scenes that use it.
	.macro backdrop resource:req, scenes:vararg
	.2byte ResourceId_\resource, 0x8000
	.ifnb \scenes
	.irp scene, \scenes
	.4byte SceneId_\scene
	.endr
	.endif
	.endm

	.section .rodata
	.balign 4
	.global gScene_MusicRules
gScene_MusicRules:
	area_music 1, -1, -1, 74
	area_music 2, 90, -1, 19
	area_music 2, 91, -1, 19
	area_music 2, 65, -1, 41
	area_music 2, 66, -1, 41
	area_music 2, 67, -1, 41
	area_music 2, 68, -1, 41
	area_music 2, 69, -1, 41
	area_music 2, 70, -1, 41
	area_music 2, 75, -1, 41
	area_music 2, 73, -1, 22
	area_music 2, -1, 0x16e, 9
	area_music 2, -1, -1, 42
	scene_music Sanctum, -1, 0x9f0, 74
	scene_music Sanctum, -1, 0x814, 2
	scene_music Sanctum, -1, -1, 74
	scene_music CottageRoomsB, 19, -1, 3
	area_music 3, -1, 0x9f0, 0
	area_music 3, -1, 0x814, 2
	area_music 3, -1, 0x901, 0
	area_music 3, -1, 0x900, 23
	area_music 3, -1, -1, 19
	scene_music SolSanctumElementalStarRoom, -1, -1, 22
	scene_music SolSanctumEruption, -1, -1, 63
	area_music 4, -1, 0x9f0, 2
	area_music 4, -1, 0x814, 63
	scene_music SolEntrance, -1, -1, 20
	area_music 4, -1, -1, 20
	scene_music StoneRooms, 2, -1, 1
	area_music 5, -1, -1, 0
	area_music 6, -1, -1, 42
	area_music 7, -1, -1, 24
	area_music 8, -1, 0x845, 1
	area_music 8, -1, -1, 2
	area_music 9, -1, -1, 3
	area_music 10, -1, 0x845, 1
	area_music 10, -1, -1, 2
	scene_music KolimaTunnel, -1, -1, 2
	area_music 11, -1, 0x845, 1
	area_music 11, -1, -1, 2
	area_music 12, -1, 0x845, 1
	area_music 12, -1, -1, 2
	area_music 13, -1, 0x845, 1
	area_music 13, -1, -1, 21
	area_music 14, -1, 0x845, 1
	area_music 14, -1, 0x844, 21
	area_music 14, -1, -1, 4
	scene_music BilibinCaveEntrance, -1, -1, 42
	area_music 15, -1, -1, 24
	area_music 17, -1, 0x881, 26
	area_music 17, -1, -1, 2
	area_music 18, -1, -1, 24
	scene_music MercuryLighthouseAerie, -1, -1, 22
	area_music 19, -1, 0x881, 22
	area_music 19, -1, -1, 27
	scene_music LacquerRoomsA, 3, -1, 60
	area_music 20, -1, -1, 5
	area_music 21, -1, -1, 28
	area_music 22, -1, -1, 21
	area_music 23, -1, -1, 5
	area_music 24, -1, -1, 42
	area_music 25, -1, 0x909, 0
	area_music 25, -1, -1, 4
	area_music 26, -1, -1, 23
	area_music 27, -1, -1, 5
	area_music 28, -1, -1, 25
	area_music 29, -1, -1, 0
	area_music 30, -1, -1, 24
	area_music 31, -1, -1, 24
	area_music 32, -1, -1, 6
	area_music 33, -1, -1, 3
	scene_music RockRooms, 10, -1, 24
	area_music 34, -1, -1, 1
	area_music 35, -1, -1, 24
	area_music 36, -1, -1, 6
	area_music 37, -1, 0x8f0, 29
	area_music 37, -1, -1, 41
	area_music 38, -1, -1, 6
	scene_music CrossboneIsle, -1, -1, 42
	scene_music PirateShip, -1, -1, 61
	area_music 39, -1, -1, 28
	area_music 40, -1, 0x950, 1
	area_music 40, -1, -1, 10
	area_music 41, -1, -1, 3
	scene_music ColosseumAzartMatch, 1, -1, 7
	scene_music ColosseumSatrageMatch, 1, -1, 7
	scene_music ColosseumNavampaMatch, 1, -1, 7
	scene_music ColosseumAzartMatch, -1, -1, 8
	scene_music ColosseumSatrageMatch, -1, -1, 8
	scene_music ColosseumNavampaMatch, -1, -1, 8
	scene_music Colosseum, 2, -1, 3
	scene_music Colosseum, 4, -1, 3
	scene_music Colosseum, 8, -1, 3
	scene_music Colosseum, 12, -1, 3
	scene_music Colosseum, 21, -1, 3
	scene_music Colosseum, 31, -1, 3
	scene_music Colosseum, 64, -1, 3
	scene_music ColosseumEntrance, -1, 0x950, 1
	area_music 42, -1, -1, 8
	area_music 43, -1, -1, 29
	area_music 44, -1, -1, 42
	area_music 45, -1, -1, 42
	area_music 46, -1, -1, 24
	area_music 47, -1, -1, 24
	area_music 48, -1, -1, 1
	area_music 49, -1, -1, 23
	area_music 50, -1, -1, 25
	area_music 51, -1, -1, 26
	area_music 52, -1, -1, 24
	scene_music TunnelRuinsLadyStatues, 1, -1, 28
	scene_music TunnelRuinsLadyStatues, 2, -1, 28
	scene_music TunnelRuinsLadyStatues, 3, -1, 28
	area_music 53, -1, -1, 29
	area_music 54, -1, -1, 28
	area_music 55, -1, 0x9a7, 1
	area_music 55, -1, -1, 2
	area_music 56, -1, -1, 27
	scene_music VenusLighthouseAerie, -1, 0x351, 22
	scene_music VenusLighthouseAerie, -1, 0x350, 30
	scene_music VenusLighthouseAerie, -1, -1, 19
	area_music 57, -1, -1, 43
	area_music 58, -1, -1, 22
	area_music 59, -1, -1, 9
	area_music 60, -1, -1, 9
	.2byte -1, -1, -1, 0	@ end

	.global gArea_NameRules
gArea_NameRules:
	area_name 0, -1, 0
	area_name 1, -1, 1
	area_name 2, -1, 2
	scene_name Sanctum, 1, 3
	scene_name Sanctum, 2, 3
	scene_name Sanctum, 20, 3
	scene_name Sanctum, 21, 3
	scene_name Sanctum, 12, 4
	scene_name Sanctum, 11, 4
	scene_name Sanctum, 10, 4
	scene_name Sanctum, 35, 5
	scene_name Sanctum, 32, 6
	scene_name Sanctum, 29, 7
	scene_name Sanctum, -1, 8
	scene_name CottageRoomsB, 19, 9
	area_name 3, -1, 10
	scene_name SolEntrance, -1, 11
	scene_name SolSanctumElementalStarRoom, -1, 12
	scene_name SolSanctumEruption, -1, 12
	area_name 4, -1, 13
	scene_name RockRooms, 7, 14
	scene_name RockRooms, 11, 14
	scene_name StoneRooms, 2, 15
	area_name 5, -1, 16
	area_name 6, -1, 17
	area_name 7, -1, 18
	scene_name BilibinTreasury, -1, 19
	area_name 8, -1, 20
	scene_name McCoyPalaceGrounds, -1, 21
	area_name 9, -1, 21
	area_name 10, -1, 22
	scene_name KolimaTunnel, -1, 23
	area_name 11, -1, 24
	area_name 12, -1, 25
	area_name 13, -1, 26
	area_name 14, -1, 27
	scene_name BilibinCaveEntrance, -1, 28
	area_name 15, -1, 29
	scene_name IceRooms, 7, 30
	area_name 17, -1, 31
	area_name 18, -1, 32
	scene_name MercuryLighthouseAerie, -1, 33
	area_name 19, -1, 34
	scene_name LacquerRoomsA, 2, 35
	scene_name LacquerRoomsA, 3, 36
	area_name 20, -1, 37
	area_name 21, -1, 38
	area_name 22, -1, 39
	area_name 23, -1, 40
	area_name 24, -1, 41
	.if EDITION_INTERNATIONAL
	scene_name AltinPeakSouthEntrance, 6, 41
	scene_name AltinPeakSouthEntrance, 7, 41
	.endif
	area_name 25, -1, 42
	area_name 26, -1, 43
	area_name 27, -1, 44
	area_name 28, -1, 45
	area_name 29, -1, 46
	area_name 30, -1, 47
	area_name 31, -1, 48
	scene_name KalaySanctum, -1, 49
	area_name 32, -1, 50
	scene_name HammetPalace, -1, 51
	area_name 33, -1, 51
	scene_name RockRooms, 10, 52
	area_name 34, -1, 53
	area_name 35, -1, 54
	scene_name CottageRoomsE, 2, 55
	area_name 36, -1, 56
	area_name 37, -1, 57
	area_name 38, -1, 58
	scene_name CrossboneIsle, -1, 59
	scene_name PirateShip, -1, 60
	area_name 39, -1, 61
	scene_name StuccoRoomsB, 8, 62
	scene_name StuccoRoomsB, 30, 62
	area_name 40, -1, 63
	scene_name BabiPalaceEntrance, -1, 64
	scene_name BabiPalace, -1, 64
	scene_name ColosseumTunnel, -1, 65
	scene_name ColosseumEntrance, -1, 66
	scene_name Colosseum, 2, 67
	scene_name Colosseum, 4, 67
	scene_name Colosseum, -1, 68
	scene_name ColosseumAzartMatch, -1, 69
	scene_name ColosseumSatrageMatch, -1, 70
	scene_name ColosseumNavampaMatch, -1, 71
	area_name 43, -1, 72
	area_name 44, -1, 73
	area_name 45, -1, 74
	area_name 46, -1, 75
	area_name 47, -1, 76
	area_name 48, -1, 77
	area_name 49, -1, 78
	area_name 50, -1, 79
	scene_name TimberRoomsA, 2, 80
	scene_name TimberRoomsA, 6, 80
	area_name 51, -1, 81
	scene_name GatewayCave, -1, 82
	area_name 52, -1, 83
	scene_name TunnelRuinsLadyStatues, 1, 84
	scene_name TunnelRuinsLadyStatues, 2, 84
	scene_name TunnelRuinsLadyStatues, 3, 84
	area_name 53, -1, 85
	scene_name BabiLighthouseEntry, -1, 86
	scene_name RoadToBabiLighthouse, -1, 87
	area_name 54, -1, 88
	scene_name TimberRoomsB, 11, 89
	scene_name TimberRoomsB, 12, 89
	area_name 55, -1, 90
	scene_name VenusLighthouseGroundFloor, -1, 91
	area_name 56, -1, 92
	scene_name VenusLighthouseAerie, -1, 93
	area_name 57, -1, 94
	area_name 58, -1, 95
	area_name 59, -1, 96
	scene_name BattleArena, 1, 97
	area_name 61, -1, 98
	area_name 62, -1, 99
	area_name 63, -1, 100
	area_name 64, -1, 101
	area_name 65, -1, 102
	area_name 66, -1, 103
	area_name 67, -1, 104
	area_name 68, -1, 105
	area_name 69, -1, 106
	area_name 70, -1, 107
	.2byte -1, -1	@ end
	.4byte -1
	.2byte 0, 0	@ a row of zeros after the end
	.4byte 0

	.global gScene_RestartRules
gScene_RestartRules:
	restart_point ValePrologue, 15, CottageRoomsB, 16
	restart_point Vale, 16, Sanctum, 2
	restart_point Vault, 1, Sanctum, 12
	restart_point Vault, 3, Sanctum, 12
	restart_point Bilibin, -1, Sanctum, 35
	restart_point Kolima, -1, Kolima, 12
	restart_point Imil, -1, IceRooms, 7
	restart_point FuchinTempleGrounds, -1, FuchinTempleGrounds, 5
	restart_point Xian, -1, Sanctum, 32
	restart_point Altin, -1, Sanctum, 29
	restart_point LamaTemple, -1, LamaTemple, 5
	restart_point VaultRevisit, -1, Sanctum, 11
	restart_point Kalay, -1, KalaySanctum, 2
	restart_point Lunpa, 3, RockRooms, 11
	restart_point Tolbi, -1, StuccoRoomsB, 30
	restart_point Suhalla, -1, TimberRoomsA, 6
	restart_point Lalivero, -1, TimberRoomsB, 12
	restart_point BattleArena, -1, BattleArena, 1
	.2byte -1, -1, -1, -1	@ end

@ A scene not listed takes the last battle background.
	.global gScene_BattleBackgroundRules
gScene_BattleBackgroundRules:
	backdrop ValeStormBackdrop, ValePrologue, Vale, ValePlaza, ValeKradensCottage
	backdrop SolSanctumBackdrop, SolSanctumCorridor, SolSanctumReliefs, SolSanctumMaze, SolSanctumLunaSolRooms, AltinPeakHydrosStatue, CrossboneIsleSteppingStones
	backdrop InteriorBackdrop, LogRoomsA
	backdrop GomaCaveBackdrop, GomaCaveEntrance
	backdrop GomaCaveBackdrop, GomaCavePond, GomaCavePillar, GomaCaveStream, KolimaTunnel
	backdrop KolimaForestBackdrop, KolimaForestEntrance, KolimaForestGlade, KolimaForestCliff, KolimaForestFloodgate, KolimaForestTree
	backdrop TretTreeBranchBackdrop, TretTreeBranch
	backdrop TretTreeInteriorsBackdrop, TreeRooms
	backdrop GomaCaveBackdrop, BilibinCaveEntrance, BilibinCaveRapier, BilibinCaveIce
	backdrop MercuryLighthouseBackdrop, MercuryLighthouseEntrance, MercuryLighthouseLower, MercuryLighthouseMiddle, MercuryLighthouseUpper, MercuryLighthouseExterior
	backdrop MercuryLighthouseAerieBackdrop, MercuryLighthouseAerie
	backdrop ImilFuchinFallsBackdrop, FuchinFallsCaveEntrance, FuchinFallsCaveZephyr, FuchinFallsCaveDragonsEye, FuchinFallsCaveArcticBlade, FuchinFallsCaveOrbOfForce, FuchinFallsCaveDragonStatue, CrossboneIsleFloatingLogs
	backdrop MogallForestBackdrop, MogallForestSign, MogallForestQuartz, MogallForestKillerApe
	backdrop AltinPeakBackdrop, AltinPeakFirstWaterBeast, AltinPeakRailTunnel, AltinPeakSecondWaterBeast, AltinPeakSouthEntrance, AltinPeakThirdWaterBeast, AltinPeakPool, AltinPeakSpritz, AltinPeakJunction, AltinPeakFallingRocks, AltinPeakWestExit
	backdrop LamakanDesertBackdrop, LamakanDesertEast, LamakanDesertSmog, LamakanDesertManticore
	backdrop TolbiShipDeckBackdrop, TolbiShipDeck, TolbiShipMast
	backdrop TolbiShipInteriorsBackdrop, ShipRooms
	backdrop ColossoBackdrop, ColosseumAzartMatch, ColosseumSatrageMatch, ColosseumNavampaMatch, BattleArena
	backdrop AltmillerCaveBackdrop, AltmillerCaveEntrance, AltmillerCaveBabi, AltmillerCaveVialChest, AltmillerCaveSquall, AltmillerCaveCookieChest, AltmillerCaveDraught
	backdrop AltmillerCaveBackdrop, LunpaFortressGroundFloor, LunpaFortressUpperFloor, LunpaFortressCave, LunpaFortressToadonpa
	backdrop SuhallaDesertBackdrop, SuhallaDesertNorth, SuhallaDesertSouth
	backdrop CrossboneIsleCaveBackdrop, CrossboneIsleFirstFloor, CrossboneIsleSecondFloor, CrossboneIsleThirdFloor, CrossboneIsleFourthFloor, CrossboneIsleFifthFloor, CrossboneIsleSixthFloor, CrossboneIsleSeventhFloor, CrossboneIsleEighthFloor, CrossboneIsleNinthFloor
	backdrop PirateShipBackdrop, PirateShip
	backdrop SuhallaGateBackdrop, SuhallaGateCheckpoint, SuhallaGateDew
	backdrop GomaCaveBackdrop, GatewayCave
	backdrop BabiLighthouseBackdrop, BabiLighthouseEntry, BabiLighthouseLuff, BabiLighthouseStoneDoor, CrossboneIsleBane
	backdrop TunnelRuinsBackdrop, TunnelRuinsGreatStatue, TunnelRuinsLadyStatues
	backdrop InteriorBackdrop, Lalivero, TimberRoomsB, TimberRoomsC
	backdrop VenusLighthouseBackdrop, VenusLighthouseGroundFloor, VenusLighthouseFirstFloor, VenusLighthouseSecondFloor, VenusLighthouseThirdFloor, VenusLighthouseFourthFloor
	backdrop VenusLighthouseAerieBackdrop, VenusLighthouseAerie
	backdrop GomaCaveBackdrop
	.4byte 0	@ end
