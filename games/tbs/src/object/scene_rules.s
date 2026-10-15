	.include "games/common/include/game/ed_asm.h"

@ シーンの曲: シーンかグループと入口 (-1 はどれでも) から、
@ フラグ (-1 は常に) が立っている最初の行の曲
	.macro scene_cue scene:req, entrance:req, flag:req, cue:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \cue
	.endm
	.macro group_cue group:req, entrance:req, flag:req, cue:req
	.2byte \group, \entrance & 0x7fff, \flag, \cue
	.endm

@ 地名: シーンかグループと入口 (-1 はどれでも) から地名番号
	.macro scene_name scene:req, entrance:req, name:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro group_name group:req, entrance:req, name:req
	.2byte \group, \entrance & 0x7fff
	.4byte \name
	.endm

@ 戻り先: シーンと入口 (-1 はどれでも) から、記録するシーンと入口
	.macro return_to scene:req, entrance:req, saved:req, saved_entrance:req
	.2byte SceneId_\scene, \entrance, SceneId_\saved, \saved_entrance
	.endm

@ 戦闘の背景と、それを使うシーン
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
	scene_cue ValeBabi, 19, -1, 3
	group_cue 3, -1, 0x9f0, 0
	group_cue 3, -1, 0x814, 2
	group_cue 3, -1, 0x901, 0
	group_cue 3, -1, 0x900, 23
	group_cue 3, -1, -1, 19
	scene_cue SolStar, -1, -1, 22
	scene_cue SolEruption, -1, -1, 63
	group_cue 4, -1, 0x9f0, 2
	group_cue 4, -1, 0x814, 63
	scene_cue SolEntrance2, -1, -1, 20
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
	scene_cue Kolima2, -1, -1, 2
	group_cue 11, -1, 0x845, 1
	group_cue 11, -1, -1, 2
	group_cue 12, -1, 0x845, 1
	group_cue 12, -1, -1, 2
	group_cue 13, -1, 0x845, 1
	group_cue 13, -1, -1, 21
	group_cue 14, -1, 0x845, 1
	group_cue 14, -1, 0x844, 21
	group_cue 14, -1, -1, 4
	scene_cue BilibinCave3, -1, -1, 42
	group_cue 15, -1, -1, 24
	group_cue 17, -1, 0x881, 26
	group_cue 17, -1, -1, 2
	group_cue 18, -1, -1, 24
	scene_cue MercuryAerie1, -1, -1, 22
	group_cue 19, -1, 0x881, 22
	group_cue 19, -1, -1, 27
	scene_cue FuchinTemple2, 3, -1, 60
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
	scene_cue CrossboneIsle1, -1, -1, 42
	scene_cue CrossboneIsle5, -1, -1, 61
	group_cue 39, -1, -1, 28
	group_cue 40, -1, 0x950, 1
	group_cue 40, -1, -1, 10
	group_cue 41, -1, -1, 3
	scene_cue ColossoRiver, 1, -1, 7
	scene_cue ColossoWall, 1, -1, 7
	scene_cue ColossoLog, 1, -1, 7
	scene_cue ColossoRiver, -1, -1, 8
	scene_cue ColossoWall, -1, -1, 8
	scene_cue ColossoLog, -1, -1, 8
	scene_cue ColosseumEntrance1, 2, -1, 3
	scene_cue ColosseumEntrance1, 4, -1, 3
	scene_cue ColosseumEntrance1, 8, -1, 3
	scene_cue ColosseumEntrance1, 12, -1, 3
	scene_cue ColosseumEntrance1, 21, -1, 3
	scene_cue ColosseumEntrance1, 31, -1, 3
	scene_cue ColosseumEntrance1, 64, -1, 3
	scene_cue ColosseumEntrance3, -1, 0x950, 1
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
	scene_cue TunnelRuins2, 1, -1, 28
	scene_cue TunnelRuins2, 2, -1, 28
	scene_cue TunnelRuins2, 3, -1, 28
	group_cue 53, -1, -1, 29
	group_cue 54, -1, -1, 28
	group_cue 55, -1, 0x9a7, 1
	group_cue 55, -1, -1, 2
	group_cue 56, -1, -1, 27
	scene_cue VenusAerie, -1, 0x351, 22
	scene_cue VenusAerie, -1, 0x350, 30
	scene_cue VenusAerie, -1, -1, 19
	group_cue 57, -1, -1, 43
	group_cue 58, -1, -1, 22
	group_cue 59, -1, -1, 9
	group_cue 60, -1, -1, 9
	.2byte -1, -1, -1, 0	@ 終わり

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
	scene_name ValeBabi, 19, 9
	group_name 3, -1, 10
	scene_name SolEntrance2, -1, 11
	scene_name SolStar, -1, 12
	scene_name SolEruption, -1, 12
	group_name 4, -1, 13
	scene_name LunpaSuhalla, 7, 14
	scene_name LunpaSuhalla, 11, 14
	scene_name VaultJail, 2, 15
	group_name 5, -1, 16
	group_name 6, -1, 17
	group_name 7, -1, 18
	scene_name Bilibin2, -1, 19
	group_name 8, -1, 20
	scene_name McCoyPalaceGarden, -1, 21
	group_name 9, -1, 21
	group_name 10, -1, 22
	scene_name Kolima2, -1, 23
	group_name 11, -1, 24
	group_name 12, -1, 25
	group_name 13, -1, 26
	group_name 14, -1, 27
	scene_name BilibinCave3, -1, 28
	group_name 15, -1, 29
	scene_name Imil2, 7, 30
	group_name 17, -1, 31
	group_name 18, -1, 32
	scene_name MercuryAerie1, -1, 33
	group_name 19, -1, 34
	scene_name FuchinTemple2, 2, 35
	scene_name FuchinTemple2, 3, 36
	group_name 20, -1, 37
	group_name 21, -1, 38
	group_name 22, -1, 39
	group_name 23, -1, 40
	group_name 24, -1, 41
	.if EDITION_INTERNATIONAL
	scene_name AltinPeak4, 6, 41
	scene_name AltinPeak4, 7, 41
	.endif
	group_name 25, -1, 42
	group_name 26, -1, 43
	group_name 27, -1, 44
	group_name 28, -1, 45
	group_name 29, -1, 46
	group_name 30, -1, 47
	group_name 31, -1, 48
	scene_name KalayInteriors2, -1, 49
	group_name 32, -1, 50
	scene_name HammetPalace, -1, 51
	group_name 33, -1, 51
	scene_name LunpaSuhalla, 10, 52
	group_name 34, -1, 53
	group_name 35, -1, 54
	scene_name KalayTolbiDocks2, 2, 55
	group_name 36, -1, 56
	group_name 37, -1, 57
	group_name 38, -1, 58
	scene_name CrossboneIsle1, -1, 59
	scene_name CrossboneIsle5, -1, 60
	group_name 39, -1, 61
	scene_name TolbiInteriors, 8, 62
	scene_name TolbiInteriors, 30, 62
	group_name 40, -1, 63
	scene_name BabiPalace2, -1, 64
	scene_name BabiPalace1, -1, 64
	scene_name ColosseumEntrance2, -1, 65
	scene_name ColosseumEntrance3, -1, 66
	scene_name ColosseumEntrance1, 2, 67
	scene_name ColosseumEntrance1, 4, 67
	scene_name ColosseumEntrance1, -1, 68
	scene_name ColossoRiver, -1, 69
	scene_name ColossoWall, -1, 70
	scene_name ColossoLog, -1, 71
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
	scene_name SuhallaGate3, -1, 82
	group_name 52, -1, 83
	scene_name TunnelRuins2, 1, 84
	scene_name TunnelRuins2, 2, 84
	scene_name TunnelRuins2, 3, 84
	group_name 53, -1, 85
	scene_name BabiLighthouse3, -1, 86
	scene_name BabiLighthouse4, -1, 87
	group_name 54, -1, 88
	scene_name LaliveroInteriors1, 11, 89
	scene_name LaliveroInteriors1, 12, 89
	group_name 55, -1, 90
	scene_name VenusLighthouse2, -1, 91
	group_name 56, -1, 92
	scene_name VenusAerie, -1, 93
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
	.2byte -1, -1	@ 終わり
	.4byte -1
	.2byte 0, 0	@ 終わりの後の 0 の行
	.4byte 0

	.global gParty_PairResolveRules
gParty_PairResolveRules:
	return_to ValeStorm, 15, ValeBabi, 16
	return_to Vale, 16, Sanctum, 2
	return_to Vault, 1, Sanctum, 12
	return_to Vault, 3, Sanctum, 12
	return_to Bilibin1, -1, Sanctum, 35
	return_to Kolima1, -1, Kolima1, 12
	return_to Imil1, -1, Imil2, 7
	return_to FuchinTemple1, -1, FuchinTemple1, 5
	return_to Xian, -1, Sanctum, 32
	return_to Altin1, -1, Sanctum, 29
	return_to AlpineCrossingLamaTemple2, -1, AlpineCrossingLamaTemple2, 5
	return_to VaultRevisit, -1, Sanctum, 11
	return_to Kalay1, -1, KalayInteriors2, 2
	return_to Lunpa1, 3, LunpaSuhalla, 11
	return_to Tolbi, -1, TolbiInteriors, 30
	return_to Suhalla, -1, SuhallaInteriors, 6
	return_to Lalivero, -1, LaliveroInteriors1, 12
	return_to LinkLobby, -1, LinkLobby, 1
	.2byte -1, -1, -1, -1	@ 終わり

@ 合うシーンがなければ最後の背景
	.global gBattle_BackdropScenes
gBattle_BackdropScenes:
	backdrop ValeStormBackdrop, ValeStorm, Vale, ValeHome, ValeKraden
	backdrop SolSanctumBackdrop, SolCorridor, SolEntrance1, SolMaze, SolLunaRooms, AltinPeak10, CrossbonePillar1
	backdrop InteriorBackdrop, VaultInteriors
	backdrop GomaCaveBackdrop, GomaEntrance
	backdrop GomaCaveBackdrop, GomaPond, GomaPillar, GomaWaterway1, Kolima2
	backdrop KolimaForestBackdrop, KolimaForestBridge, KolimaForestClearing, KolimaForestRiver, KolimaForestBend, KolimaForestTree
	backdrop TretBranchBackdrop, TretBranch
	backdrop TretInteriorsBackdrop, TretInteriors
	backdrop GomaCaveBackdrop, BilibinCave3, BilibinCave2, BilibinCave1
	backdrop MercuryLighthouseBackdrop, MercuryEntrance, MercuryLighthouse1, MercuryLighthouse2, MercuryLighthouse3, MercuryLighthouse4
	backdrop MercuryAerieBackdrop, MercuryAerie1
	backdrop ImilFuchinFallsBackdrop, ImilFuchinFalls2, ImilFuchinFalls3, ImilFuchinFalls4, ImilFuchinFalls5, ImilFuchinFalls6, ImilFuchinFalls7, CrossboneIsle3
	backdrop MogallForestBackdrop, MogallForest1, MogallForest2, MogallForest3
	backdrop AltinPeakBackdrop, AltinPeak1, AltinPeak2, AltinPeak3, AltinPeak4, AltinPeak5, AltinPeak6, AltinPeak7, AltinPeak8, AltinPeak9, AltinPeak11
	backdrop LamakanDesertBackdrop, LamakanDesert1, LamakanDesert2, LamakanDesert3
	backdrop TolbiShipDeckBackdrop, TolbiShipDeck, TolbiShipMast
	backdrop TolbiShipInteriorsBackdrop, TolbiShipInteriors
	backdrop ColossoBackdrop, ColossoRiver, ColossoWall, ColossoLog, LinkLobby
	backdrop AltmillerCaveBackdrop, AltmillerCave1, AltmillerCave2, AltmillerCave3, AltmillerCave4, AltmillerCave5, AltmillerCave6
	backdrop AltmillerCaveBackdrop, LunpaFortress1, LunpaFortress2, LunpaFortress3, LunpaFortress4
	backdrop SuhallaDesertBackdrop, SuhallaDesert1, SuhallaDesert2
	backdrop CrossboneIsleCaveBackdrop, CrossboneIsle6, CrossboneIsle7, CrossboneIsle8, CrossboneIsle9, CrossboneIsle10, CrossboneIsle11, CrossboneIsle12, CrossboneIsle13, CrossboneIsle14
	backdrop PirateShipBackdrop, CrossboneIsle5
	backdrop SuhallaGateBackdrop, SuhallaGate1, SuhallaGate2
	backdrop GomaCaveBackdrop, SuhallaGate3
	backdrop BabiLighthouseBackdrop, BabiLighthouse3, BabiLighthouse2, BabiLighthouse1, CrossbonePlatform3
	backdrop TunnelRuinsBackdrop, TunnelRuins1, TunnelRuins2
	backdrop InteriorBackdrop, Lalivero, LaliveroInteriors1, LaliveroInteriors2
	backdrop VenusLighthouseBackdrop, VenusLighthouse2, VenusLighthouse3, VenusLighthouse4, VenusLighthouse5, VenusLighthouse6
	backdrop VenusAerieBackdrop, VenusAerie
	backdrop GomaCaveBackdrop
	.4byte 0	@ 終わり
