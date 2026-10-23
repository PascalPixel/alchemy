	.include "games/tbs/include/game/ed_asm.h"

@ Field music: the music of the first rule matching the map or its group and
@ the warp destination (-1 for any) whose flag (-1 for always) is set.
	.macro map_music map:req, warpDest:req, flag:req, cue:req
	.2byte MapId_\map, (\warpDest & 0x7fff) | 0x8000, \flag, \cue
	.endm
	.macro mapsec_music group:req, warpDest:req, flag:req, cue:req
	.2byte \group, \warpDest & 0x7fff, \flag, \cue
	.endm

@ Area names: the name of the map or its group and the warp destination (-1
@ for any).
	.macro map_name map:req, warpDest:req, name:req
	.2byte MapId_\map, (\warpDest & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro group_name group:req, warpDest:req, name:req
	.2byte \group, \warpDest & 0x7fff
	.4byte \name
	.endm

@ Defeat return points: the map and warp destination (-1 for any), then the
@ map and warp destination the party returns to after a defeat there.
	.macro return_to map:req, warpDest:req, saved:req, saved_warp_dest:req
	.2byte MapId_\map, \warpDest, MapId_\saved, \saved_warp_dest
	.endm

@ A battle backdrop and the maps that use it.
	.macro backdrop resource:req, maps:vararg
	.2byte ResourceId_\resource, 0x8000
	.ifnb \maps
	.irp map, \maps
	.4byte MapId_\map
	.endr
	.endif
	.endm

	.section .rodata
	.balign 4
	.global gMap_MusicRules
gMap_MusicRules:
	mapsec_music 1, -1, -1, 74
	mapsec_music 2, 90, -1, 19
	mapsec_music 2, 91, -1, 19
	mapsec_music 2, 65, -1, 41
	mapsec_music 2, 66, -1, 41
	mapsec_music 2, 67, -1, 41
	mapsec_music 2, 68, -1, 41
	mapsec_music 2, 69, -1, 41
	mapsec_music 2, 70, -1, 41
	mapsec_music 2, 75, -1, 41
	mapsec_music 2, 73, -1, 22
	mapsec_music 2, -1, 0x16e, 9
	mapsec_music 2, -1, -1, 42
	map_music Sanctum, -1, 0x9f0, 74
	map_music Sanctum, -1, 0x814, 2
	map_music Sanctum, -1, -1, 74
	map_music ValeInteriorsInn, 19, -1, 3
	mapsec_music 3, -1, 0x9f0, 0
	mapsec_music 3, -1, 0x814, 2
	mapsec_music 3, -1, 0x901, 0
	mapsec_music 3, -1, 0x900, 23
	mapsec_music 3, -1, -1, 19
	map_music SolSanctumElementalStarRoom, -1, -1, 22
	map_music SolSanctumEruption, -1, -1, 63
	mapsec_music 4, -1, 0x9f0, 2
	mapsec_music 4, -1, 0x814, 63
	map_music SolEntrance, -1, -1, 20
	mapsec_music 4, -1, -1, 20
	map_music VaultJail, 2, -1, 1
	mapsec_music 5, -1, -1, 0
	mapsec_music 6, -1, -1, 42
	mapsec_music 7, -1, -1, 24
	mapsec_music 8, -1, 0x845, 1
	mapsec_music 8, -1, -1, 2
	mapsec_music 9, -1, -1, 3
	mapsec_music 10, -1, 0x845, 1
	mapsec_music 10, -1, -1, 2
	map_music KolimaTunnel, -1, -1, 2
	mapsec_music 11, -1, 0x845, 1
	mapsec_music 11, -1, -1, 2
	mapsec_music 12, -1, 0x845, 1
	mapsec_music 12, -1, -1, 2
	mapsec_music 13, -1, 0x845, 1
	mapsec_music 13, -1, -1, 21
	mapsec_music 14, -1, 0x845, 1
	mapsec_music 14, -1, 0x844, 21
	mapsec_music 14, -1, -1, 4
	map_music BilibinCaveEntrance, -1, -1, 42
	mapsec_music 15, -1, -1, 24
	mapsec_music 17, -1, 0x881, 26
	mapsec_music 17, -1, -1, 2
	mapsec_music 18, -1, -1, 24
	map_music MercuryLighthouseAerie, -1, -1, 22
	mapsec_music 19, -1, 0x881, 22
	mapsec_music 19, -1, -1, 27
	map_music FuchinTempleInteriors, 3, -1, 60
	mapsec_music 20, -1, -1, 5
	mapsec_music 21, -1, -1, 28
	mapsec_music 22, -1, -1, 21
	mapsec_music 23, -1, -1, 5
	mapsec_music 24, -1, -1, 42
	mapsec_music 25, -1, 0x909, 0
	mapsec_music 25, -1, -1, 4
	mapsec_music 26, -1, -1, 23
	mapsec_music 27, -1, -1, 5
	mapsec_music 28, -1, -1, 25
	mapsec_music 29, -1, -1, 0
	mapsec_music 30, -1, -1, 24
	mapsec_music 31, -1, -1, 24
	mapsec_music 32, -1, -1, 6
	mapsec_music 33, -1, -1, 3
	map_music LunpaSuhalla, 10, -1, 24
	mapsec_music 34, -1, -1, 1
	mapsec_music 35, -1, -1, 24
	mapsec_music 36, -1, -1, 6
	mapsec_music 37, -1, 0x8f0, 29
	mapsec_music 37, -1, -1, 41
	mapsec_music 38, -1, -1, 6
	map_music CrossboneIsle, -1, -1, 42
	map_music PirateShip, -1, -1, 61
	mapsec_music 39, -1, -1, 28
	mapsec_music 40, -1, 0x950, 1
	mapsec_music 40, -1, -1, 10
	mapsec_music 41, -1, -1, 3
	map_music ColosseumAzartMatch, 1, -1, 7
	map_music ColosseumSatrageMatch, 1, -1, 7
	map_music ColosseumNavampaMatch, 1, -1, 7
	map_music ColosseumAzartMatch, -1, -1, 8
	map_music ColosseumSatrageMatch, -1, -1, 8
	map_music ColosseumNavampaMatch, -1, -1, 8
	map_music Colosseum, 2, -1, 3
	map_music Colosseum, 4, -1, 3
	map_music Colosseum, 8, -1, 3
	map_music Colosseum, 12, -1, 3
	map_music Colosseum, 21, -1, 3
	map_music Colosseum, 31, -1, 3
	map_music Colosseum, 64, -1, 3
	map_music ColosseumEntrance, -1, 0x950, 1
	mapsec_music 42, -1, -1, 8
	mapsec_music 43, -1, -1, 29
	mapsec_music 44, -1, -1, 42
	mapsec_music 45, -1, -1, 42
	mapsec_music 46, -1, -1, 24
	mapsec_music 47, -1, -1, 24
	mapsec_music 48, -1, -1, 1
	mapsec_music 49, -1, -1, 23
	mapsec_music 50, -1, -1, 25
	mapsec_music 51, -1, -1, 26
	mapsec_music 52, -1, -1, 24
	map_music TunnelRuinsLadyStatues, 1, -1, 28
	map_music TunnelRuinsLadyStatues, 2, -1, 28
	map_music TunnelRuinsLadyStatues, 3, -1, 28
	mapsec_music 53, -1, -1, 29
	mapsec_music 54, -1, -1, 28
	mapsec_music 55, -1, 0x9a7, 1
	mapsec_music 55, -1, -1, 2
	mapsec_music 56, -1, -1, 27
	map_music VenusLighthouseAerie, -1, 0x351, 22
	map_music VenusLighthouseAerie, -1, 0x350, 30
	map_music VenusLighthouseAerie, -1, -1, 19
	mapsec_music 57, -1, -1, 43
	mapsec_music 58, -1, -1, 22
	mapsec_music 59, -1, -1, 9
	mapsec_music 60, -1, -1, 9
	.2byte -1, -1, -1, 0	@ end

	.global gMapSec_NameRules
gMapSec_NameRules:
	group_name 0, -1, 0
	group_name 1, -1, 1
	group_name 2, -1, 2
	map_name Sanctum, 1, 3
	map_name Sanctum, 2, 3
	map_name Sanctum, 20, 3
	map_name Sanctum, 21, 3
	map_name Sanctum, 12, 4
	map_name Sanctum, 11, 4
	map_name Sanctum, 10, 4
	map_name Sanctum, 35, 5
	map_name Sanctum, 32, 6
	map_name Sanctum, 29, 7
	map_name Sanctum, -1, 8
	map_name ValeInteriorsInn, 19, 9
	group_name 3, -1, 10
	map_name SolEntrance, -1, 11
	map_name SolSanctumElementalStarRoom, -1, 12
	map_name SolSanctumEruption, -1, 12
	group_name 4, -1, 13
	map_name LunpaSuhalla, 7, 14
	map_name LunpaSuhalla, 11, 14
	map_name VaultJail, 2, 15
	group_name 5, -1, 16
	group_name 6, -1, 17
	group_name 7, -1, 18
	map_name BilibinTreasury, -1, 19
	group_name 8, -1, 20
	map_name McCoyPalaceGrounds, -1, 21
	group_name 9, -1, 21
	group_name 10, -1, 22
	map_name KolimaTunnel, -1, 23
	group_name 11, -1, 24
	group_name 12, -1, 25
	group_name 13, -1, 26
	group_name 14, -1, 27
	map_name BilibinCaveEntrance, -1, 28
	group_name 15, -1, 29
	map_name ImilInteriors, 7, 30
	group_name 17, -1, 31
	group_name 18, -1, 32
	map_name MercuryLighthouseAerie, -1, 33
	group_name 19, -1, 34
	map_name FuchinTempleInteriors, 2, 35
	map_name FuchinTempleInteriors, 3, 36
	group_name 20, -1, 37
	group_name 21, -1, 38
	group_name 22, -1, 39
	group_name 23, -1, 40
	group_name 24, -1, 41
	.if EDITION_INTERNATIONAL
	map_name AltinPeakSouthEntrance, 6, 41
	map_name AltinPeakSouthEntrance, 7, 41
	.endif
	group_name 25, -1, 42
	group_name 26, -1, 43
	group_name 27, -1, 44
	group_name 28, -1, 45
	group_name 29, -1, 46
	group_name 30, -1, 47
	group_name 31, -1, 48
	map_name KalaySanctum, -1, 49
	group_name 32, -1, 50
	map_name HammetPalace, -1, 51
	group_name 33, -1, 51
	map_name LunpaSuhalla, 10, 52
	group_name 34, -1, 53
	group_name 35, -1, 54
	map_name KalayTolbiDocksInteriors, 2, 55
	group_name 36, -1, 56
	group_name 37, -1, 57
	group_name 38, -1, 58
	map_name CrossboneIsle, -1, 59
	map_name PirateShip, -1, 60
	group_name 39, -1, 61
	map_name TolbiInteriors, 8, 62
	map_name TolbiInteriors, 30, 62
	group_name 40, -1, 63
	map_name BabiPalaceEntrance, -1, 64
	map_name BabiPalace, -1, 64
	map_name ColosseumTunnel, -1, 65
	map_name ColosseumEntrance, -1, 66
	map_name Colosseum, 2, 67
	map_name Colosseum, 4, 67
	map_name Colosseum, -1, 68
	map_name ColosseumAzartMatch, -1, 69
	map_name ColosseumSatrageMatch, -1, 70
	map_name ColosseumNavampaMatch, -1, 71
	group_name 43, -1, 72
	group_name 44, -1, 73
	group_name 45, -1, 74
	group_name 46, -1, 75
	group_name 47, -1, 76
	group_name 48, -1, 77
	group_name 49, -1, 78
	group_name 50, -1, 79
	map_name SuhallaInteriors, 2, 80
	map_name SuhallaInteriors, 6, 80
	group_name 51, -1, 81
	map_name GatewayCave, -1, 82
	group_name 52, -1, 83
	map_name TunnelRuinsLadyStatues, 1, 84
	map_name TunnelRuinsLadyStatues, 2, 84
	map_name TunnelRuinsLadyStatues, 3, 84
	group_name 53, -1, 85
	map_name BabiLighthouseEntry, -1, 86
	map_name RoadToBabiLighthouse, -1, 87
	group_name 54, -1, 88
	map_name LaliveroShops, 11, 89
	map_name LaliveroShops, 12, 89
	group_name 55, -1, 90
	map_name VenusLighthouseGroundFloor, -1, 91
	group_name 56, -1, 92
	map_name VenusLighthouseAerie, -1, 93
	group_name 57, -1, 94
	group_name 58, -1, 95
	group_name 59, -1, 96
	map_name LinkLobby, 1, 97
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

@ A map not listed takes the last backdrop.
	.global gBattleBackground_Maps
gBattleBackground_Maps:
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
