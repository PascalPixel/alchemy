	.include "games/tbs/include/game/ed_asm.h"

@ Field music: the music of the first rule matching the scene or its group and
@ the entrance (-1 for any) whose flag (-1 for always) is set.
	.macro scene_cue scene:req, entrance:req, flag:req, cue:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \cue
	.endm
	.macro group_cue group:req, entrance:req, flag:req, cue:req
	.2byte \group, \entrance & 0x7fff, \flag, \cue
	.endm

@ Place names: the name of the scene or its group and the entrance (-1 for any).
	.macro scene_name scene:req, entrance:req, name:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro group_name group:req, entrance:req, name:req
	.2byte \group, \entrance & 0x7fff
	.4byte \name
	.endm

@ Defeat return points: the scene and entrance (-1 for any), then the scene and
@ entrance the party returns to after a defeat there.
	.macro return_to scene:req, entrance:req, saved:req, saved_entrance:req
	.2byte SceneId_\scene, \entrance, SceneId_\saved, \saved_entrance
	.endm

@ A battle backdrop and the scenes that use it.
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
	.global gScene_InteractionRuleTable
gScene_InteractionRuleTable:
	group_cue 1, -1, -1, 74
	group_cue 2, 90, -1, 19
	group_cue 2, 91, -1, 19
	group_cue 2, 65, -1, 41
	group_cue 2, 66, -1, 41
	group_cue 2, 67, -1, 41
	group_cue 2, 68, -1, 41
	group_cue 2, 69, -1, 41
	group_cue 2, 70, -1, 41
	group_cue 2, 75, -1, 41
	group_cue 2, 73, -1, 22
	group_cue 2, -1, 0x16e, 9
	group_cue 2, -1, -1, 42
	scene_cue Sanctum, -1, 0x9f0, 74
	scene_cue Sanctum, -1, 0x814, 2
	scene_cue Sanctum, -1, -1, 74
	scene_cue ValeInteriorsInn, 19, -1, 3
	group_cue 3, -1, 0x9f0, 0
	group_cue 3, -1, 0x814, 2
	group_cue 3, -1, 0x901, 0
	group_cue 3, -1, 0x900, 23
	group_cue 3, -1, -1, 19
	scene_cue SolSanctumElementalStarRoom, -1, -1, 22
	scene_cue SolSanctumEruption, -1, -1, 63
	group_cue 4, -1, 0x9f0, 2
	group_cue 4, -1, 0x814, 63
	scene_cue SolEntrance, -1, -1, 20
	group_cue 4, -1, -1, 20
	scene_cue VaultJail, 2, -1, 1
	group_cue 5, -1, -1, 0
	group_cue 6, -1, -1, 42
	group_cue 7, -1, -1, 24
	group_cue 8, -1, 0x845, 1
	group_cue 8, -1, -1, 2
	group_cue 9, -1, -1, 3
	group_cue 10, -1, 0x845, 1
	group_cue 10, -1, -1, 2
	scene_cue KolimaTunnel, -1, -1, 2
	group_cue 11, -1, 0x845, 1
	group_cue 11, -1, -1, 2
	group_cue 12, -1, 0x845, 1
	group_cue 12, -1, -1, 2
	group_cue 13, -1, 0x845, 1
	group_cue 13, -1, -1, 21
	group_cue 14, -1, 0x845, 1
	group_cue 14, -1, 0x844, 21
	group_cue 14, -1, -1, 4
	scene_cue BilibinCaveEntrance, -1, -1, 42
	group_cue 15, -1, -1, 24
	group_cue 17, -1, 0x881, 26
	group_cue 17, -1, -1, 2
	group_cue 18, -1, -1, 24
	scene_cue MercuryLighthouseAerie, -1, -1, 22
	group_cue 19, -1, 0x881, 22
	group_cue 19, -1, -1, 27
	scene_cue FuchinTempleInteriors, 3, -1, 60
	group_cue 20, -1, -1, 5
	group_cue 21, -1, -1, 28
	group_cue 22, -1, -1, 21
	group_cue 23, -1, -1, 5
	group_cue 24, -1, -1, 42
	group_cue 25, -1, 0x909, 0
	group_cue 25, -1, -1, 4
	group_cue 26, -1, -1, 23
	group_cue 27, -1, -1, 5
	group_cue 28, -1, -1, 25
	group_cue 29, -1, -1, 0
	group_cue 30, -1, -1, 24
	group_cue 31, -1, -1, 24
	group_cue 32, -1, -1, 6
	group_cue 33, -1, -1, 3
	scene_cue LunpaSuhalla, 10, -1, 24
	group_cue 34, -1, -1, 1
	group_cue 35, -1, -1, 24
	group_cue 36, -1, -1, 6
	group_cue 37, -1, 0x8f0, 29
	group_cue 37, -1, -1, 41
	group_cue 38, -1, -1, 6
	scene_cue CrossboneIsle, -1, -1, 42
	scene_cue PirateShip, -1, -1, 61
	group_cue 39, -1, -1, 28
	group_cue 40, -1, 0x950, 1
	group_cue 40, -1, -1, 10
	group_cue 41, -1, -1, 3
	scene_cue ColosseumAzartMatch, 1, -1, 7
	scene_cue ColosseumSatrageMatch, 1, -1, 7
	scene_cue ColosseumNavampaMatch, 1, -1, 7
	scene_cue ColosseumAzartMatch, -1, -1, 8
	scene_cue ColosseumSatrageMatch, -1, -1, 8
	scene_cue ColosseumNavampaMatch, -1, -1, 8
	scene_cue Colosseum, 2, -1, 3
	scene_cue Colosseum, 4, -1, 3
	scene_cue Colosseum, 8, -1, 3
	scene_cue Colosseum, 12, -1, 3
	scene_cue Colosseum, 21, -1, 3
	scene_cue Colosseum, 31, -1, 3
	scene_cue Colosseum, 64, -1, 3
	scene_cue ColosseumEntrance, -1, 0x950, 1
	group_cue 42, -1, -1, 8
	group_cue 43, -1, -1, 29
	group_cue 44, -1, -1, 42
	group_cue 45, -1, -1, 42
	group_cue 46, -1, -1, 24
	group_cue 47, -1, -1, 24
	group_cue 48, -1, -1, 1
	group_cue 49, -1, -1, 23
	group_cue 50, -1, -1, 25
	group_cue 51, -1, -1, 26
	group_cue 52, -1, -1, 24
	scene_cue TunnelRuinsLadyStatues, 1, -1, 28
	scene_cue TunnelRuinsLadyStatues, 2, -1, 28
	scene_cue TunnelRuinsLadyStatues, 3, -1, 28
	group_cue 53, -1, -1, 29
	group_cue 54, -1, -1, 28
	group_cue 55, -1, 0x9a7, 1
	group_cue 55, -1, -1, 2
	group_cue 56, -1, -1, 27
	scene_cue VenusLighthouseAerie, -1, 0x351, 22
	scene_cue VenusLighthouseAerie, -1, 0x350, 30
	scene_cue VenusLighthouseAerie, -1, -1, 19
	group_cue 57, -1, -1, 43
	group_cue 58, -1, -1, 22
	group_cue 59, -1, -1, 9
	group_cue 60, -1, -1, 9
	.2byte -1, -1, -1, 0	@ end

	.global gScene_AreaNameRules
gScene_AreaNameRules:
	group_name 0, -1, 0
	group_name 1, -1, 1
	group_name 2, -1, 2
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
	scene_name ValeInteriorsInn, 19, 9
	group_name 3, -1, 10
	scene_name SolEntrance, -1, 11
	scene_name SolSanctumElementalStarRoom, -1, 12
	scene_name SolSanctumEruption, -1, 12
	group_name 4, -1, 13
	scene_name LunpaSuhalla, 7, 14
	scene_name LunpaSuhalla, 11, 14
	scene_name VaultJail, 2, 15
	group_name 5, -1, 16
	group_name 6, -1, 17
	group_name 7, -1, 18
	scene_name BilibinTreasury, -1, 19
	group_name 8, -1, 20
	scene_name McCoyPalaceGrounds, -1, 21
	group_name 9, -1, 21
	group_name 10, -1, 22
	scene_name KolimaTunnel, -1, 23
	group_name 11, -1, 24
	group_name 12, -1, 25
	group_name 13, -1, 26
	group_name 14, -1, 27
	scene_name BilibinCaveEntrance, -1, 28
	group_name 15, -1, 29
	scene_name ImilInteriors, 7, 30
	group_name 17, -1, 31
	group_name 18, -1, 32
	scene_name MercuryLighthouseAerie, -1, 33
	group_name 19, -1, 34
	scene_name FuchinTempleInteriors, 2, 35
	scene_name FuchinTempleInteriors, 3, 36
	group_name 20, -1, 37
	group_name 21, -1, 38
	group_name 22, -1, 39
	group_name 23, -1, 40
	group_name 24, -1, 41
	.if EDITION_INTERNATIONAL
	scene_name AltinPeakSouthEntrance, 6, 41
	scene_name AltinPeakSouthEntrance, 7, 41
	.endif
	group_name 25, -1, 42
	group_name 26, -1, 43
	group_name 27, -1, 44
	group_name 28, -1, 45
	group_name 29, -1, 46
	group_name 30, -1, 47
	group_name 31, -1, 48
	scene_name KalaySanctum, -1, 49
	group_name 32, -1, 50
	scene_name HammetPalace, -1, 51
	group_name 33, -1, 51
	scene_name LunpaSuhalla, 10, 52
	group_name 34, -1, 53
	group_name 35, -1, 54
	scene_name KalayTolbiDocksInteriors, 2, 55
	group_name 36, -1, 56
	group_name 37, -1, 57
	group_name 38, -1, 58
	scene_name CrossboneIsle, -1, 59
	scene_name PirateShip, -1, 60
	group_name 39, -1, 61
	scene_name TolbiInteriors, 8, 62
	scene_name TolbiInteriors, 30, 62
	group_name 40, -1, 63
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
	group_name 43, -1, 72
	group_name 44, -1, 73
	group_name 45, -1, 74
	group_name 46, -1, 75
	group_name 47, -1, 76
	group_name 48, -1, 77
	group_name 49, -1, 78
	group_name 50, -1, 79
	scene_name SuhallaInteriors, 2, 80
	scene_name SuhallaInteriors, 6, 80
	group_name 51, -1, 81
	scene_name GatewayCave, -1, 82
	group_name 52, -1, 83
	scene_name TunnelRuinsLadyStatues, 1, 84
	scene_name TunnelRuinsLadyStatues, 2, 84
	scene_name TunnelRuinsLadyStatues, 3, 84
	group_name 53, -1, 85
	scene_name BabiLighthouseEntry, -1, 86
	scene_name RoadToBabiLighthouse, -1, 87
	group_name 54, -1, 88
	scene_name LaliveroShops, 11, 89
	scene_name LaliveroShops, 12, 89
	group_name 55, -1, 90
	scene_name VenusLighthouseGroundFloor, -1, 91
	group_name 56, -1, 92
	scene_name VenusLighthouseAerie, -1, 93
	group_name 57, -1, 94
	group_name 58, -1, 95
	group_name 59, -1, 96
	scene_name LinkLobby, 1, 97
	group_name 61, -1, 98
	group_name 62, -1, 99
	group_name 63, -1, 100
	group_name 64, -1, 101
	group_name 65, -1, 102
	group_name 66, -1, 103
	group_name 67, -1, 104
	group_name 68, -1, 105
	group_name 69, -1, 106
	group_name 70, -1, 107
	.2byte -1, -1	@ end
	.4byte -1
	.2byte 0, 0	@ a row of zeros after the end
	.4byte 0

	.global gParty_PairResolveRules
gParty_PairResolveRules:
	return_to ValeStorm, 15, ValeInteriorsInn, 16
	return_to Vale, 16, Sanctum, 2
	return_to Vault, 1, Sanctum, 12
	return_to Vault, 3, Sanctum, 12
	return_to Bilibin, -1, Sanctum, 35
	return_to Kolima, -1, Kolima, 12
	return_to Imil, -1, ImilInteriors, 7
	return_to FuchinTempleGrounds, -1, FuchinTempleGrounds, 5
	return_to Xian, -1, Sanctum, 32
	return_to Altin, -1, Sanctum, 29
	return_to LamaTemple, -1, LamaTemple, 5
	return_to VaultRevisit, -1, Sanctum, 11
	return_to Kalay, -1, KalaySanctum, 2
	return_to Lunpa, 3, LunpaSuhalla, 11
	return_to Tolbi, -1, TolbiInteriors, 30
	return_to Suhalla, -1, SuhallaInteriors, 6
	return_to Lalivero, -1, LaliveroShops, 12
	return_to LinkLobby, -1, LinkLobby, 1
	.2byte -1, -1, -1, -1	@ end

@ A scene not listed takes the last backdrop.
	.global gBattle_BackdropScenes
gBattle_BackdropScenes:
	backdrop ValeStormBackdrop, ValeStorm, Vale, ValeGate, ValeKraden
	backdrop SolSanctumBackdrop, SolSanctumCorridor, SolSanctumReliefs, SolSanctumMaze, SolSanctumLunaSolRooms, AltinPeakHydrosStatue, CrossboneIsleSteppingStones
	backdrop InteriorBackdrop, VaultInteriors
	backdrop GomaCaveBackdrop, GomaCaveEntrance
	backdrop GomaCaveBackdrop, GomaCavePond, GomaCavePillar, GomaCaveStream, KolimaTunnel
	backdrop KolimaForestBackdrop, KolimaForestEntrance, KolimaForestGlade, KolimaForestCliff, KolimaForestFloodgate, KolimaForestTree
	backdrop TretTreeBranchBackdrop, TretTreeBranch
	backdrop TretTreeInteriorsBackdrop, TretTreeInteriors
	backdrop GomaCaveBackdrop, BilibinCaveEntrance, BilibinCaveRapier, BilibinCaveIce
	backdrop MercuryLighthouseBackdrop, MercuryLighthouseEntrance, MercuryLighthouseLower, MercuryLighthouseMiddle, MercuryLighthouseUpper, MercuryLighthouseExterior
	backdrop MercuryLighthouseAerieBackdrop, MercuryLighthouseAerie
	backdrop ImilFuchinFallsBackdrop, FuchinFallsCaveEntrance, FuchinFallsCaveZephyr, FuchinFallsCaveDragonsEye, FuchinFallsCaveArcticBlade, FuchinFallsCaveOrbOfForce, FuchinFallsCaveDragonStatue, CrossboneIsleFloatingLogs
	backdrop MogallForestBackdrop, MogallForestSign, MogallForestQuartz, MogallForestKillerApe
	backdrop AltinPeakBackdrop, AltinPeakFirstWaterBeast, AltinPeakRailTunnel, AltinPeakSecondWaterBeast, AltinPeakSouthEntrance, AltinPeakThirdWaterBeast, AltinPeakPool, AltinPeakSpritz, AltinPeakJunction, AltinPeakFallingRocks, AltinPeakWestExit
	backdrop LamakanDesertBackdrop, LamakanDesertEast, LamakanDesertSmog, LamakanDesertManticore
	backdrop TolbiShipDeckBackdrop, TolbiShipDeck, TolbiShipMast
	backdrop TolbiShipInteriorsBackdrop, TolbiShipInteriors
	backdrop ColossoBackdrop, ColosseumAzartMatch, ColosseumSatrageMatch, ColosseumNavampaMatch, LinkLobby
	backdrop AltmillerCaveBackdrop, AltmillerCaveEntrance, AltmillerCaveBabi, AltmillerCaveVialChest, AltmillerCaveSquall, AltmillerCaveCookieChest, AltmillerCaveDraught
	backdrop AltmillerCaveBackdrop, LunpaFortressGroundFloor, LunpaFortressUpperFloor, LunpaFortressCave, LunpaFortressToadonpa
	backdrop SuhallaDesertBackdrop, SuhallaDesertNorth, SuhallaDesertSouth
	backdrop CrossboneIsleCaveBackdrop, CrossboneIsleFirstFloor, CrossboneIsleSecondFloor, CrossboneIsleThirdFloor, CrossboneIsleFourthFloor, CrossboneIsleFifthFloor, CrossboneIsleSixthFloor, CrossboneIsleSeventhFloor, CrossboneIsleEighthFloor, CrossboneIsleNinthFloor
	backdrop PirateShipBackdrop, PirateShip
	backdrop SuhallaGateBackdrop, SuhallaGateCheckpoint, SuhallaGateDew
	backdrop GomaCaveBackdrop, GatewayCave
	backdrop BabiLighthouseBackdrop, BabiLighthouseEntry, BabiLighthouseLuff, BabiLighthouseStoneDoor, CrossboneIsleBane
	backdrop TunnelRuinsBackdrop, TunnelRuinsGreatStatue, TunnelRuinsLadyStatues
	backdrop InteriorBackdrop, Lalivero, LaliveroShops, LaliveroFaransHouse
	backdrop VenusLighthouseBackdrop, VenusLighthouseGroundFloor, VenusLighthouseFirstFloor, VenusLighthouseSecondFloor, VenusLighthouseThirdFloor, VenusLighthouseFourthFloor
	backdrop VenusLighthouseAerieBackdrop, VenusLighthouseAerie
	backdrop GomaCaveBackdrop
	.4byte 0	@ end
