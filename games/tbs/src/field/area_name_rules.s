@ The area each scene is named for (struct AreaNameRule): the first rule that
@ matches the scene or its area, and the entrance (-1 for any), gives the area
@ name, an offset most readers add to MsgDebugAreaName. A scene's row sets
@ useMapId, the entrance's top bit; id -1 ends the table, and a row of zeros
@ follows the end.
	.macro scene_name scene:req, entrance:req, name:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro area_name area:req, entrance:req, name:req
	.2byte \area, \entrance & 0x7fff
	.4byte \name
	.endm

	.section .rodata
	.balign 4
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
	.ifndef TBS_EDITION_JA
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
	.2byte -1, -1 @ the end
	.4byte -1
	.4byte 0, 0
