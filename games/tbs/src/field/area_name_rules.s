	.include "enums/area_ids.inc"

@ The area each scene is named for (AREA_NAME_RULE): the first rule that
@ matches the scene or its area, and the entrance (-1 for any), gives the area
@ name, an offset most readers add to MsgDebugAreaName. A scene's row sets
@ useMapId, the entrance's top bit; id -1 ends the table, and a row of zeros
@ follows the end.
	.macro scene_name scene:req, entrance:req, name:req
	.2byte SceneId\scene, (\entrance & 0x7fff) | 0x8000
	.4byte \name
	.endm
	.macro area_name area:req, entrance:req, name:req
	.2byte \area, \entrance & 0x7fff
	.4byte \name
	.endm

	.section .rodata
	.balign 4
	.global AreaNameRules
AreaNameRules:
	area_name AREA_TITLE_SCREEN, -1, 0
	area_name AREA_CLEAR_DATA, -1, 1
	area_name AREA_WORLD_MAP, -1, 2
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
	area_name AREA_VALE, -1, 10
	scene_name SolEntrance, -1, 11
	scene_name SolSanctumElementalStarRoom, -1, 12
	scene_name SolSanctumEruption, -1, 12
	area_name AREA_SOL_SANCTUM, -1, 13
	scene_name RockRooms, 7, 14
	scene_name RockRooms, 11, 14
	scene_name StoneRooms, 2, 15
	area_name AREA_VAULT, -1, 16
	area_name AREA_GOMA_CAVE_ENTRANCE, -1, 17
	area_name AREA_GOMA_CAVE, -1, 18
	scene_name BilibinTreasury, -1, 19
	area_name AREA_BILIBIN, -1, 20
	scene_name McCoyPalaceGrounds, -1, 21
	area_name AREA_MCCOYS_PALACE, -1, 21
	area_name AREA_BILIBIN_BARRICADE, -1, 22
	scene_name KolimaTunnel, -1, 23
	area_name AREA_KOLIMA, -1, 24
	area_name AREA_KOLIMA_BRIDGE, -1, 25
	area_name AREA_KOLIMA_FOREST, -1, 26
	area_name AREA_TRET_TREE, -1, 27
	scene_name BilibinCaveEntrance, -1, 28
	area_name AREA_BILIBIN_CAVE, -1, 29
	scene_name IceRooms, 7, 30
	area_name AREA_IMIL, -1, 31
	area_name AREA_IMIL_FALLS_CAVE, -1, 32
	scene_name MercuryLighthouseAerie, -1, 33
	area_name AREA_MERCURY_LIGHTHOUSE, -1, 34
	scene_name LacquerRoomsA, 2, 35
	scene_name LacquerRoomsA, 3, 36
	area_name AREA_FUCHIN_TEMPLE, -1, 37
	area_name AREA_FUCHIN_FALLS_CAVE, -1, 38
	area_name AREA_MOGALL_FOREST, -1, 39
	area_name AREA_XIAN, -1, 40
	area_name AREA_ALPINE_CROSSING, -1, 41
	.ifndef TBS_EDITION_JA
	scene_name AltinPeakSouthEntrance, 6, 41
	scene_name AltinPeakSouthEntrance, 7, 41
	.endif
	area_name AREA_ALTIN, -1, 42
	area_name AREA_ALTIN_PEAK, -1, 43
	area_name AREA_LAMA_TEMPLE, -1, 44
	area_name AREA_LAMAKAN_DESERT, -1, 45
	area_name AREA_VALE_CAVE_MOUTH, -1, 46
	area_name AREA_VALE_CAVE, -1, 47
	area_name AREA_VAULT_CAVE, -1, 48
	scene_name KalaySanctum, -1, 49
	area_name AREA_KALAY, -1, 50
	scene_name HammetPalace, -1, 51
	area_name AREA_HAMMET_PALACE, -1, 51
	scene_name RockRooms, 10, 52
	area_name AREA_LUNPA, -1, 53
	area_name AREA_WEST_LUNPA_CAVE, -1, 54
	scene_name CottageRoomsE, 2, 55
	area_name AREA_KALAY_DOCKS, -1, 56
	area_name AREA_TOLBI_BOUND_SHIP, -1, 57
	area_name AREA_TOLBI_DOCKS, -1, 58
	scene_name CrossboneIsle, -1, 59
	scene_name PirateShip, -1, 60
	area_name AREA_CROSSBONE_ISLE_CAVE, -1, 61
	scene_name StuccoRoomsB, 8, 62
	scene_name StuccoRoomsB, 30, 62
	area_name AREA_TOLBI, -1, 63
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
	area_name AREA_ALTMILLER_CAVE, -1, 72
	area_name AREA_GONDOWAN_PASSAGE, -1, 73
	area_name AREA_KALAY_TUNNEL_ENTRANCE, -1, 74
	area_name AREA_KALAY_TUNNEL, -1, 75
	area_name AREA_GONDOWAN_CAVE, -1, 76
	area_name AREA_LUNPA_FORTRESS_GATES, -1, 77
	area_name AREA_LUNPA_FORTRESS, -1, 78
	area_name AREA_SUHALLA_DESERT, -1, 79
	scene_name TimberRoomsA, 2, 80
	scene_name TimberRoomsA, 6, 80
	area_name AREA_SUHALLA, -1, 81
	scene_name GatewayCave, -1, 82
	area_name AREA_SUHALLA_GATE, -1, 83
	scene_name TunnelRuinsLadyStatues, 1, 84
	scene_name TunnelRuinsLadyStatues, 2, 84
	scene_name TunnelRuinsLadyStatues, 3, 84
	area_name AREA_TUNNEL_RUINS, -1, 85
	scene_name BabiLighthouseEntry, -1, 86
	scene_name RoadToBabiLighthouse, -1, 87
	area_name AREA_BABI_LIGHTHOUSE, -1, 88
	scene_name TimberRoomsB, 11, 89
	scene_name TimberRoomsB, 12, 89
	area_name AREA_LALIVERO, -1, 90
	scene_name VenusLighthouseGroundFloor, -1, 91
	area_name AREA_VENUS_LIGHTHOUSE_ENTRY, -1, 92
	scene_name VenusLighthouseAerie, -1, 93
	area_name AREA_VENUS_LIGHTHOUSE, -1, 94
	area_name AREA_LEMURIAN_SHIP, -1, 95
	area_name AREA_TOLBI_SPRING, -1, 96
	scene_name BattleArena, 1, 97
	area_name AREA_TEST_0, -1, 98
	area_name AREA_TEST_1, -1, 99
	area_name AREA_TEST_2, -1, 100
	area_name AREA_TEST_3, -1, 101
	area_name AREA_TEST_4, -1, 102
	area_name AREA_TEST_5, -1, 103
	area_name AREA_TEST_6, -1, 104
	area_name AREA_TEST_7, -1, 105
	area_name AREA_INCREASE_ITEM_AND_LEVEL, -1, 106
	area_name AREA_MENU_TEST, -1, 107
	.2byte -1, -1 @ the end
	.4byte -1
	.4byte 0, 0
