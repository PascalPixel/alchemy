	.include "enums/flags.inc"
	.include "enums/sounds.inc"
	.include "enums/area_ids.inc"

@ Field music: the music (a MUS_ id) of the first rule matching the scene or
@ its area and the entrance (-1 for any) whose flag (-1 for always) is set.
	.macro scene_music scene:req, entrance:req, flag:req, music:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \music
	.endm
	.macro area_music area:req, entrance:req, flag:req, music:req
	.2byte \area, \entrance & 0x7fff, \flag, \music
	.endm

	.section .rodata
	.global gScene_CueRuleTable
gScene_CueRuleTable:
	area_music AREA_CLEAR_DATA, -1, -1, MUS_CLEAR
	area_music AREA_WORLD_MAP, 64, -1, MUS_ALEX
	area_music AREA_WORLD_MAP, 78, -1, SOUND_KEEP_BGM
	area_music AREA_WORLD_MAP, 77, -1, MUS_IDEJIMA
	area_music AREA_WORLD_MAP, -1, FLAG_SAILING, MUS_LEMURIAN_SHIP
	area_music AREA_WORLD_MAP, -1, FLAG_REUNITED, MUS_WORLD_MAP_REUNITED
	area_music AREA_WORLD_MAP, -1, -1, MUS_WORLD_MAP
	area_music AREA_STAFF_CREDIT, -1, -1, MUS_STAFF_CREDIT
	scene_music VenusLighthouseEntrance, -1, -1, MUS_VENUS_LIGHTHOUSE_ENTRANCE
	area_music AREA_VENUS_LIGHTHOUSE, -1, -1, MUS_VENUS_LIGHTHOUSE
	area_music AREA_SUHALLA_GATE, -1, -1, MUS_SUHALLA_GATE
	area_music AREA_IDEJIMA, -1, -1, MUS_IDEJIMA
	scene_music DailaInteriorsHouses, 7, -1, MUS_MADRA
	area_music AREA_DAILA, -1, -1, MUS_DAILA
	area_music AREA_KANDOREAN_TEMPLE, -1, -1, MUS_KANDOREAN_TEMPLE
	area_music AREA_KANDOREAN_TEMPLE_GATE, -1, -1, MUS_CHAMPA
	area_music AREA_SHRINE_OF_THE_SEA_GOD, -1, -1, MUS_SHRINE_OF_THE_SEA_GOD
	scene_music DehkanPlateauMintChest, -1, -1, MUS_DEHKAN_PLATEAU
	scene_music DehkanPlateauPoundCube, -1, -1, MUS_DEHKAN_PLATEAU
	scene_music DehkanPlateauCannon, -1, -1, MUS_DEHKAN_PLATEAU
	area_music AREA_DEHKAN_PLATEAU, -1, -1, MUS_DEHKAN_PLATEAU
	area_music AREA_GONDOWAN_CLIFFS, -1, FLAG_REUNITED, MUS_WORLD_MAP_REUNITED
	area_music AREA_GONDOWAN_CLIFFS, -1, -1, MUS_WORLD_MAP
	area_music AREA_MADRA, -1, -1, MUS_MADRA
	area_music AREA_MADRA_CATACOMBS, -1, -1, MUS_MADRA_CATACOMBS
	area_music AREA_OSENIA_CLIFFS, -1, FLAG_REUNITED, MUS_WORLD_MAP_REUNITED
	area_music AREA_OSENIA_CLIFFS, -1, -1, MUS_WORLD_MAP
	scene_music MikasallaInteriorsSanctum, 4, -1, MUS_YALLAM
	scene_music MikasallaInteriorsSanctum, 5, -1, MUS_APOJII_ISLANDS
	scene_music MikasallaInteriorsHouses, 5, -1, MUS_SUHALLA
	scene_music MikasallaNorth, -1, -1, MUS_MADRA_CATACOMBS
	area_music AREA_MIKASALLA, -1, -1, MUS_MADRA
	scene_music GarohInteriorsSanctum, -1, -1, MUS_GAROH
	area_music AREA_GAROH, -1, -1, MUS_GAROH
	area_music AREA_YAMPI_DESERT, -1, -1, MUS_YAMPI_DESERT
	area_music AREA_YAMPI_DESERT_CAVE, -1, -1, MUS_SUHALLA_GATE
	area_music AREA_AIRS_ROCK, -1, -1, MUS_AIRS_ROCK
	area_music AREA_AIRS_ROCK_INSIDE, -1, -1, MUS_AIRS_ROCK
	area_music AREA_ALHAFRA, -1, -1, MUS_ALHAFRA
	area_music AREA_EASTERN_ALHAFRA, -1, -1, MUS_ALHAFRA
	scene_music AlhafraJail, 8, -1, MUS_ALHAFRA
	area_music AREA_ALHAFRAN_CAVE, -1, -1, MUS_MADRA_CATACOMBS
	area_music AREA_GONDOWAN_CLIFFS, -1, -1, MUS_CHAMPA
	area_music AREA_NARIBWE, -1, -1, MUS_NARIBWE
	area_music AREA_KIBOMBO_MOUNTAINS, -1, -1, MUS_NARIBWE
	area_music AREA_KIBOMBO, -1, -1, MUS_NARIBWE
	area_music AREA_KIBOMBO_CEREMONY, -1, FLAG_GABOMBA_MUSIC_SILENCED, MUS_SILENCE
	area_music AREA_KIBOMBO_CEREMONY, -1, FLAG_GABOMBA_CEREMONY_UNDER_WAY, MUS_KIBOMBO
	area_music AREA_KIBOMBO_CEREMONY, -1, -1, MUS_KIBOMBO
	scene_music KibomboStatueFace, -1, -1, MUS_GABOMBA_STATUE
	area_music AREA_GABOMBA_STATUE, -1, FLAG_GABOMBA_MUSIC_SILENCED, MUS_SILENCE
	area_music AREA_GABOMBA_STATUE, -1, -1, MUS_GABOMBA_STATUE
	area_music AREA_GABOMBA_CATACOMBS, -1, -1, MUS_GABOMBA_CATACOMBS
	scene_music LemurianShipMainDeck, -1, -1, MUS_LEMURIAN_SHIP
	scene_music LemurianShipTiller, -1, -1, MUS_LEMURIAN_SHIP
	area_music AREA_LEMURIAN_SHIP, -1, -1, MUS_LEMURIAN_SHIP
	scene_music IsletsTurtle, -1, -1, MUS_CHAMPA
	scene_music IsletsPier, -1, -1, MUS_KALT_ISLAND
	scene_music IsletsCow, -1, -1, MUS_MADRA
	scene_music IsletsPrettyStone, -1, -1, MUS_SUHALLA
	area_music AREA_WEST_INDRA_ISLET, -1, -1, MUS_DAILA
	area_music AREA_ISLET_CAVE, -1, -1, MUS_MADRA_CATACOMBS
	area_music AREA_YALLAM, -1, -1, MUS_YALLAM
	area_music AREA_TAOPO_SWAMP_ENTRANCE, -1, -1, MUS_TAOPO_SWAMP
	area_music AREA_TAOPO_SWAMP, -1, -1, MUS_TAOPO_SWAMP
	scene_music ApojiiIslandsShops, 6, -1, MUS_CHAMPA
	area_music AREA_APOJII_ISLANDS, -1, -1, MUS_APOJII_ISLANDS
	area_music AREA_APOJII_ISLANDS_CAVE, -1, -1, MUS_MADRA_CATACOMBS
	area_music AREA_AQUA_ROCK, 5, -1, SOUND_KEEP_BGM
	area_music AREA_AQUA_ROCK, -1, -1, MUS_AQUA_ROCK
	area_music AREA_AQUA_ROCK_INSIDE, -1, -1, MUS_AQUA_ROCK
	area_music AREA_IZUMO, -1, FLAG_SERPENT_SLAIN, MUS_IZUMO_CELEBRATION
	area_music AREA_IZUMO, -1, -1, MUS_IZUMO
	area_music AREA_IZUMO_RUINS, -1, -1, MUS_IZUMO
	area_music AREA_GAIA_ROCK, -1, -1, MUS_GAIA_ROCK
	area_music AREA_GAIA_ROCK_INSIDE, -1, -1, MUS_GAIA_ROCK
	scene_music ChampaInteriors, 8, -1, MUS_KALT_ISLAND
	scene_music ChampaInteriors, 9, -1, MUS_DAILA
	area_music AREA_CHAMPA, -1, -1, MUS_CHAMPA
	area_music AREA_ANKOHL_RUINS, -1, -1, MUS_ANKOHL_RUINS
	area_music AREA_SEA_OF_TIME, -1, -1, MUS_SEA_OF_TIME
	area_music AREA_LEMURIA_ENTRANCE, -1, -1, MUS_LEMURIA
	scene_music LemuriaSpring, -1, -1, MUS_LEMURIA_SPRING
	scene_music LemuriaPalace, -1, -1, MUS_LEMURIA_PALACE
	area_music AREA_LEMURIA, -1, -1, MUS_LEMURIA
	area_music AREA_TUNDARIA_TOWER, -1, -1, MUS_TUNDARIA_TOWER
	area_music AREA_GONDOWAN_SETTLEMENT, -1, -1, MUS_GONDOWAN_SETTLEMENT
	area_music AREA_HESPERIA_SETTLEMENT, -1, -1, MUS_MADRA
	area_music AREA_KALT_ISLAND, -1, -1, MUS_KALT_ISLAND
	area_music AREA_SOUTHWEST_ATTEKA_ISLET, -1, -1, MUS_APOJII_ISLANDS
	area_music AREA_MAGMA_ROCK, -1, -1, MUS_MAGMA_ROCK
	area_music AREA_MAGMA_ROCK_INSIDE, -1, -1, MUS_MAGMA_ROCK
	scene_music ContigoShops, 21, FLAG_REUNITED, MUS_WORLD_MAP_REUNITED
	scene_music ContigoShops, 21, -1, MUS_WORLD_MAP
	area_music AREA_ATTEKA_INLET, -1, FLAG_REUNITED, MUS_WORLD_MAP_REUNITED
	area_music AREA_ATTEKA_INLET, -1, -1, MUS_WORLD_MAP
	area_music AREA_CONTIGO, -1, -1, MUS_CONTIGO
	area_music AREA_ANEMOS_SANCTUM, -1, -1, MUS_CONTIGO
	area_music AREA_SHAMAN_VILLAGE_CAVE, -1, -1, MUS_SHRINE_OF_THE_SEA_GOD
	area_music AREA_SHAMAN_VILLAGE, -1, -1, MUS_SHAMAN_VILLAGE
	area_music AREA_TRIAL_ROAD, -1, FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT, MUS_TRIAL_ROAD
	area_music AREA_TRIAL_ROAD, -1, -1, MUS_CONTIGO
	area_music AREA_TRIAL_ROAD_CAVE, -1, -1, MUS_CONTIGO
	scene_music JupiterLighthouseRoomM, -1, FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED, MUS_KARST_AND_AGATIO
	area_music AREA_JUPITER_LIGHTHOUSE, -1, -1, MUS_JUPITER_LIGHTHOUSE
	area_music AREA_LOHO, -1, -1, MUS_SHAMAN_VILLAGE
	area_music AREA_NORTHERN_REACHES, -1, -1, MUS_LEMURIAN_SHIP
	area_music AREA_PROX, -1, FLAG_MARS_BEACON_LIT, MUS_SUHALLA
	area_music AREA_PROX, -1, -1, MUS_PROX
	area_music AREA_INDRA_CAVERN, -1, -1, MUS_SHRINE_OF_THE_SEA_GOD
	area_music AREA_OSENIA_CAVERN, -1, -1, MUS_SHRINE_OF_THE_SEA_GOD
	area_music AREA_ATTEKA_CAVERN, -1, -1, MUS_SHRINE_OF_THE_SEA_GOD
	area_music AREA_ANGARA_CAVERN, -1, -1, MUS_SHRINE_OF_THE_SEA_GOD
	scene_music MarsLighthouseAerie, 2, -1, MUS_SILENCE
	scene_music MarsLighthouseAerie, 3, FLAG_MARS_BEACON_LIT, MUS_MARS_LIGHTHOUSE
	scene_music MarsLighthouseAerie, 3, -1, MUS_SHEBAS_PAST
	scene_music MarsLighthouseAerie, 5, -1, MUS_MERCURY_LIGHTHOUSE
	area_music AREA_MARS_LIGHTHOUSE, -1, -1, MUS_MARS_LIGHTHOUSE
	area_music AREA_MERCURY_LIGHTHOUSE, -1, -1, MUS_MERCURY_LIGHTHOUSE
	area_music AREA_TREASURE_ISLE, -1, -1, MUS_DEHKAN_PLATEAU
	scene_music AnemosInnerSanctumDragonSkinChest, 1, -1, MUS_ELEMENTAL_STARS
	scene_music AnemosInnerSanctumDragonSkinChest, 2, -1, MUS_ELEMENTAL_STARS
	scene_music AnemosInnerSanctumDragonSkinChest, 99, -1, MUS_ELEMENTAL_STARS
	area_music AREA_ANEMOS_INNER_SANCTUM, -1, -1, MUS_ANEMOS_INNER_SANCTUM
	area_music AREA_MERCURY_LIGHTHOUSE, -1, -1, SOUND_KEEP_BGM
	area_music AREA_STAFF_CREDIT, -1, -1, SOUND_KEEP_BGM
	area_music AREA_MOUNT_ALEPH_SUMMIT, -1, -1, SOUND_KEEP_BGM
	area_music AREA_EPILOGUE, -1, -1, MUS_SILENCE
	area_music AREA_LINK_LOBBY, -1, -1, MUS_LEMURIA_SPRING
	.2byte 0xffff, 0xffff, 0xffff, 0x0000	@ End.
