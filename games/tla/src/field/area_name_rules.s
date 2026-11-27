	.include "enums/area_ids.inc"

@ The area each scene is named for (AREA_NAME_RULE): the first rule that
@ matches the scene or its area, and the entrance (-1 for any), gives the area
@ name, an offset most readers add to MsgDebugAreaName. A scene's row sets
@ useSceneId, the entrance's top bit; id -1 ends the table, and a row of zeros
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
	area_name AREA_STAFF_CREDIT, -1, 3
	area_name AREA_VENUS_LIGHTHOUSE, -1, 4
	area_name AREA_SUHALLA_GATE, -1, 5
	area_name AREA_IDEJIMA, -1, 6
	scene_name DailaRoomsHouses, 7, 7
	area_name AREA_DAILA, -1, 8
	area_name AREA_KANDOREAN_TEMPLE, -1, 9
	area_name AREA_KANDOREAN_TEMPLE_GATE, -1, 9
	area_name AREA_SHRINE_OF_THE_SEA_GOD, -1, 10
	area_name AREA_DEHKAN_PLATEAU, -1, 11
	area_name AREA_MADRA, -1, 12
	area_name AREA_MADRA_CATACOMBS, -1, 13
	area_name AREA_MADRA_DRAWBRIDGE, -1, 14
	area_name AREA_OSENIA_CLIFFS, -1, 15
	scene_name MikasallaRoomsHouses, 5, 16
	scene_name MikasallaRoomsSanctum, 4, 17
	scene_name MikasallaRoomsSanctum, 5, 18
	area_name AREA_MIKASALLA, -1, 19
	area_name AREA_GAROH, -1, 20
	area_name AREA_YAMPI_DESERT, -1, 21
	area_name AREA_YAMPI_DESERT_CAVE, -1, 22
	area_name AREA_AIRS_ROCK, -1, 23
	area_name AREA_AIRS_ROCK_INSIDE, -1, 23
	area_name AREA_ALHAFRA, -1, 24
	area_name AREA_EASTERN_ALHAFRA, -1, 25
	area_name AREA_ALHAFRAN_CAVE, -1, 26
	area_name AREA_GONDOWAN_CLIFFS, -1, 27
	area_name AREA_NARIBWE, -1, 28
	area_name AREA_KIBOMBO_MOUNTAINS, -1, 29
	area_name AREA_KIBOMBO, -1, 30
	area_name AREA_KIBOMBO_CEREMONY, -1, 30
	area_name AREA_GABOMBA_STATUE, -1, 31
	area_name AREA_GABOMBA_CATACOMBS, -1, 32
	area_name AREA_LEMURIAN_SHIP, -1, 33
	area_name AREA_EAST_INDRA_SHORE, -1, 34
	area_name AREA_WEST_INDRA_ISLET, -1, 35
	area_name AREA_EAST_TUNDARIA_ISLET, -1, 36
	area_name AREA_NORTH_OSENIA_ISLET, -1, 37
	area_name AREA_SOUTHEAST_ANGARA_ISLET, -1, 38
	area_name AREA_SEA_OF_TIME_ISLET, -1, 39
	area_name AREA_YALLAM, -1, 40
	area_name AREA_TAOPO_SWAMP_ENTRANCE, -1, 41
	area_name AREA_TAOPO_SWAMP, -1, 41
	scene_name ApojiiIslandsShops, 6, 42
	area_name AREA_APOJII_ISLANDS, -1, 43
	area_name AREA_APOJII_ISLANDS_CAVE, -1, 44
	area_name AREA_GAIA_ROCK, -1, 45
	area_name AREA_GAIA_ROCK_INSIDE, -1, 45
	area_name AREA_AQUA_ROCK, -1, 46
	area_name AREA_AQUA_ROCK_INSIDE, -1, 46
	area_name AREA_IZUMO, -1, 47
	area_name AREA_IZUMO_RUINS, -1, 48
	scene_name ChampaRooms, 8, 49
	scene_name ChampaRooms, 9, 50
	area_name AREA_CHAMPA, -1, 51
	area_name AREA_SEA_OF_TIME, -1, 52
	area_name AREA_ANKOHL_RUINS, -1, 53
	scene_name AncientLemuria, 1, 54
	area_name AREA_LEMURIA_ENTRANCE, -1, 55
	area_name AREA_LEMURIA, -1, 55
	area_name AREA_TUNDARIA_TOWER, -1, 56
	area_name AREA_SOUTHWEST_ATTEKA_ISLET, -1, 57
	area_name AREA_GONDOWAN_SETTLEMENT, -1, 58
	area_name AREA_HESPERIA_SETTLEMENT, -1, 59
	area_name AREA_KALT_ISLAND, -1, 60
	area_name AREA_MAGMA_ROCK, -1, 61
	area_name AREA_MAGMA_ROCK_INSIDE, -1, 61
	area_name AREA_ATTEKA_INLET, -1, 62
	area_name AREA_SHAMAN_VILLAGE_CAVE, -1, 63
	area_name AREA_SHAMAN_VILLAGE, -1, 64
	area_name AREA_TRIAL_ROAD, -1, 65
	area_name AREA_TRIAL_ROAD_CAVE, -1, 65
	scene_name ContigoShops, 21, 66
	area_name AREA_CONTIGO, -1, 67
	area_name AREA_ANEMOS_SANCTUM, -1, 68
	area_name AREA_JUPITER_LIGHTHOUSE, -1, 69
	area_name AREA_LOHO, -1, 70
	area_name AREA_NORTHERN_REACHES, -1, 71
	area_name AREA_PROX, -1, 72
	area_name AREA_INDRA_CAVERN, -1, 73
	area_name AREA_OSENIA_CAVERN, -1, 74
	area_name AREA_ANGARA_CAVERN, -1, 75
	area_name AREA_ATTEKA_CAVERN, -1, 76
	area_name AREA_ISLET_CAVE, -1, 77
	area_name AREA_MARS_LIGHTHOUSE, -1, 78
	area_name AREA_TREASURE_ISLE, -1, 79
	area_name AREA_ANEMOS_INNER_SANCTUM, -1, 80
	area_name AREA_MERCURY_LIGHTHOUSE, -1, 81
	area_name AREA_EPILOGUE, -1, 82
	area_name AREA_MOUNT_ALEPH_SUMMIT, -1, 83
	area_name AREA_LINK_LOBBY, -1, 84
	area_name AREA_TEST_0, -1, 85
	area_name AREA_TEST_1, -1, 86
	area_name AREA_TEST_2, -1, 87
	area_name AREA_TEST_3, -1, 88
	area_name AREA_TEST_4, -1, 89
	area_name AREA_TEST_5, -1, 90
	area_name AREA_TEST_6, -1, 91
	area_name AREA_TEST_7, -1, 92
	scene_name IconTest, 96, 93
	scene_name IconTest, 97, 93
	scene_name IconTest, 98, 93
	scene_name IconTest, 99, 93
	area_name AREA_ICON_TEST, -1, 94
	area_name AREA_PSYNERGY_TEST, -1, 95
	area_name AREA_PSYNERGY_TEST_2, -1, 96
	area_name AREA_SHOP_TEST, -1, 97
	.2byte -1, -1 @ the end
	.4byte -1
	.4byte 0, 0
