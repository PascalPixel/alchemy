	.include "enums/flags.inc"
	.include "enums/sprite_ids.inc"

@ The World Map's places (struct FlagEntry), which FlagEntries_GetTable
@ returns; a place number of 0 ends the list. Each row: the place number,
@ whose flag is 0x180 + place; its markers (low nibble, and high nibble on
@ the map screen); the actor the World Map shows there, its sprite, place
@ in pixels, facing in degrees and the flag it is shown by (-1 always; with
@ 0x1000 shown while the flag is set, else while it is clear); the scene and
@ entrance whose arrival marks the place visited (WorldMap_MarkPlaceVisited,
@ scene -1 and entrance -2 for none, an entrance of -1 for any); and the
@ scene and entrance choosing it leads to (0 for none).
	.macro place number:req, markers:req, sprite:req, x:req, z:req, facing:req, shown_flag:req, visit_scene:req, visit_entrance:req, scene:req, entrance:req
	.byte \number, \markers
	.2byte \sprite, \x, \z, \facing * 0x10000 / 360, \shown_flag
	.2byte \visit_scene, \visit_entrance, \scene, \entrance
	.endm

	.section .rodata
	.global gFlagEntries
gFlagEntries:
	place 2, 0x11, SPRITE_WORLD_MAP_DAILA, 10144, 8288, 90, -1, SceneId_DailaFlooded, -1, SceneId_DailaFlooded, -1
	place 4, 0x01, SPRITE_WORLD_MAP_KANDOREAN_TEMPLE, 10136, 8570, 90, -1, SceneId_KandoreanTempleGate, -1, 0, -1
	place 5, 0x00, SPRITE_WORLD_MAP_CAVE, 10424, 8336, 90, -1, -1, -2, 0, -1
	place 6, 0x00, SPRITE_WORLD_MAP_HILL, 10226, 8884, 90, -1, -1, -2, 0, -1
	place 7, 0x00, SPRITE_WORLD_MAP_HILL, 10570, 8928, 90, -1, -1, -2, 0, -1
	place 8, 0x00, SPRITE_UNSEEN_MARKER, 10637, 9075, 90, -1, SceneId_LemurianShipMainDeck, -1, 0, -1
	place 9, 0x11, SPRITE_WORLD_MAP_TOWN, 10168, 9603, 90, -1, SceneId_Madra, -1, SceneId_Madra, -1
	place 12, 0x00, SPRITE_UNSEEN_MARKER, 10394, 9380, 90, -1, SceneId_OseniaCliffs, -1, 0, -1
	place 13, 0x00, SPRITE_WORLD_MAP_CLIFFS, 10412, 9395, 90, -1, SceneId_OseniaCliffs, -1, 0, -1
	place 14, 0x11, SPRITE_WORLD_MAP_FOREST_VILLAGE, 11125, 10660, 90, -1, SceneId_Mikasalla, -1, SceneId_Mikasalla, -1
	place 15, 0x11, SPRITE_WORLD_MAP_FOREST_VILLAGE, 12068, 10440, 90, -1, SceneId_GarohWayUp, -1, SceneId_GarohWayUp, -1
	place 16, 0x02, SPRITE_WORLD_MAP_AIRS_ROCK, 11880, 10064, 90, -1, SceneId_AirsRockStormBrandChest, -1, 0, -1
	place 19, 0x00, SPRITE_WORLD_MAP_CAVE, 11910, 9700, 90, -1, SceneId_YampiDesertGuardianRingChest, -1, 0, -1
	place 20, 0x11, SPRITE_WORLD_MAP_TOWN, 11633, 9366, 90, -1, SceneId_Alhafra, -1, SceneId_Alhafra, -1
	place 21, 0x00, SPRITE_UNSEEN_MARKER, 9258, 9520, 0, -1, -1, -2, 0, -1
	place 22, 0x00, SPRITE_UNSEEN_MARKER, 9316, 9526, -180, -1, -1, -2, 0, -1
	place 23, 0x11, SPRITE_WORLD_MAP_ROUND_HUT_VILLAGE, 9148, 9018, 90, -1, SceneId_Naribwe, -1, SceneId_Naribwe, -1
	place 25, 0x00, SPRITE_WORLD_MAP_MOUNTAIN_PATH, 9200, 8380, 90, -1, -1, -2, 0, -1
	place 26, 0x00, SPRITE_WORLD_MAP_MOUNTAIN_PATH, 9077, 8258, 90, -1, -1, -2, 0, -1
	place 27, 0x11, SPRITE_WORLD_MAP_ROUND_HUT_VILLAGE, 9256, 7908, 90, -1, SceneId_Kibombo, -1, SceneId_Kibombo, -1
	place 28, 0x01, SPRITE_WORLD_MAP_HOUSE, 9546, 8274, 90, -1, SceneId_WestIndraIslet, -1, 0, -1
	place 29, 0x01, SPRITE_WORLD_MAP_SNOWY_HOUSE, 12770, 11530, 90, -1, SceneId_EastTundariaIslet, -1, 0, -1
	place 30, 0x01, SPRITE_WORLD_MAP_HOUSE, 11724, 8030, 90, -1, SceneId_NorthOseniaIslet, -1, 0, -1
	place 31, 0x01, SPRITE_WORLD_MAP_HOUSE, 11680, 6640, 90, -1, SceneId_SoutheastAngaraIslet, -1, 0, -1
	place 32, 0x01, SPRITE_WORLD_MAP_HOUSE, 13488, 7519, 90, -1, SceneId_SeaOfTimeIslet, -1, 0, -1
	place 33, 0x11, SPRITE_WORLD_MAP_FOREST_VILLAGE, 12923, 9713, 90, -1, SceneId_Yallam, -1, SceneId_Yallam, -1
	place 35, 0x00, SPRITE_UNSEEN_MARKER, 12695, 10297, 90, -1, -1, -2, 0, -1
	place 36, 0x11, SPRITE_WORLD_MAP_APOJII_ISLANDS_B, 14517, 8413, 90, -1, SceneId_ApojiiIslands, -1, SceneId_ApojiiIslands, -1
	place 37, 0x02, SPRITE_WORLD_MAP_AQUA_ROCK, 14256, 8708, 90, -1, SceneId_AquaRockMoai, -1, 0, -1
	place 38, 0x11, SPRITE_WORLD_MAP_GOLD_ROOFED_TOWN, 13406, 6068, 90, -1, SceneId_Izumo, -1, SceneId_Izumo, -1
	place 39, 0x02, SPRITE_WORLD_MAP_GAIA_ROCK, 13450, 6178, 90, -1, SceneId_GaiaRockSusa, -1, 0, -1
	place 40, 0x11, SPRITE_WORLD_MAP_GOLD_ROOFED_TOWN, 10904, 6258, 90, -1, SceneId_Champa, -1, SceneId_Champa, -1
	place 41, 0x02, SPRITE_WORLD_MAP_ANKOHL_RUINS, 11204, 6081, 90, -1, SceneId_AnkohlRuinsStoneFaces, -1, 0, -1
	place 42, 0x00, SPRITE_ROCK_PILE, 12437, 7523, 90, -1, SceneId_SeaOfTimeToLemuria, -1, 0, -1
	place 43, 0x00, SPRITE_ROCK_PILE, 12490, 7523, 90, -1, SceneId_SeaOfTimeToLemuria, -1, 0, -1
	place 45, 0x21, SPRITE_UNSEEN_MARKER, 12461, 7142, 90, -1, SceneId_LemuriaEntrance, -1, SceneId_LemuriaEntrance, -1
	place 45, 0x00, SPRITE_WORLD_MAP_LEMURIA, 13088, 1058, 90, -1, SceneId_LemuriaEntrance, -1, SceneId_LemuriaEntrance, -1
	place 46, 0x02, SPRITE_WORLD_MAP_TUNDARIA_TOWER, 9490, 10930, 90, -1, SceneId_TundariaTowerEntrance, -1, 0, -1
	place 47, 0x01, SPRITE_WORLD_MAP_HOUSE, 5810, 10134, 90, -1, SceneId_SwAttekaIslet, -1, 0, -1
	place 48, 0x01, SPRITE_WORLD_MAP_HOUSE, 8582, 7380, 90, -1, SceneId_GondowanSettlement, -1, 0, -1
	place 49, 0x01, SPRITE_WORLD_MAP_HOUSE, 6307, 6200, 90, -1, SceneId_HesperiaSettlement, -1, 0, -1
	place 50, 0x01, SPRITE_WORLD_MAP_SNOWY_HOUSE, 8848, 4600, 90, -1, SceneId_KaltIsland, -1, 0, -1
	place 51, 0x02, SPRITE_WORLD_MAP_MAGMA_ROCK, 8970, 7460, 90, -1, SceneId_MagmaRockSlide, -1, 0, -1
	place 54, 0x11, SPRITE_WORLD_MAP_SANDY_TOWN, 7714, 7730, 90, -1, SceneId_Contigo, -1, SceneId_Contigo, -1
	place 55, 0x02, SPRITE_WORLD_MAP_JUPITER_LIGHTHOUSE, 7279, 7303, 90, -1, SceneId_JupiterLighthouseEntrance, -1, SceneId_JupiterLighthouseAerie, -1
	place 56, 0x00, SPRITE_WORLD_MAP_CAVE, 7382, 5720, 90, -1, SceneId_ShamanVillageTrialGrounds, -1, 0, -1
	place 57, 0x00, SPRITE_WORLD_MAP_CAVE, 7500, 5628, 90, -1, SceneId_ShamanVillageTrialGrounds, -1, 0, -1
	place 58, 0x11, SPRITE_WORLD_MAP_SHAMAN_VILLAGE, 7662, 5499, 90, -1, SceneId_ShamanVillage, -1, SceneId_ShamanVillage, -1
	place 59, 0x11, SPRITE_WORLD_MAP_SANDY_TOWN, 8978, 5838, 90, -1, SceneId_Loho, -1, SceneId_Loho, -1
	.ifdef TLA_EDITION_JA
	place 62, 0x11, SPRITE_UNSEEN_MARKER, 8893, 3020, 90, -1, SceneId_Prox, -1, SceneId_Prox, -1
	.else
	place 62, 0x11, SPRITE_UNSEEN_MARKER, 9120, 3180, 90, -1, SceneId_Prox, -1, SceneId_Prox, -1
	.endif
	place 62, 0x00, SPRITE_WORLD_MAP_SNOWY_VILLAGE, 9055, 1214, 90, -1, SceneId_Prox, -1, SceneId_Prox, -1
	place 82, 0x00, SPRITE_WORLD_MAP_CAVE, 11596, 10170, 90, -1, SceneId_CavernsOsenia, -1, 0, -1
	place 83, 0x00, SPRITE_WORLD_MAP_CAVE, 10240, 9124, 90, -1, SceneId_CavernsIndra, -1, 0, -1
	place 84, 0x00, SPRITE_WORLD_MAP_MARS_LIGHTHOUSE_A, 9020, 850, 90, FLAG_MARS_LIGHTHOUSE_THAWED | 0x1000, SceneId_MarsLighthouseOutside, -1, 0, -1
	place 84, 0x00, SPRITE_WORLD_MAP_MARS_LIGHTHOUSE_B, 9020, 850, 90, FLAG_MARS_LIGHTHOUSE_THAWED, SceneId_MarsLighthouseOutside, -1, 0, -1
	place 87, 0x01, SPRITE_WORLD_MAP_CAVE, 12640, 4360, 90, -1, SceneId_TreasureIsleEntrance, -1, 0, -1
	place 88, 0x00, SPRITE_WORLD_MAP_CAVE, 7868, 9094, 90, -1, SceneId_CavernsAtteka, -1, 0, -1
	place 89, 0x00, SPRITE_WORLD_MAP_CAVE, 9479, 5020, 90, -1, SceneId_CavernsAngara, -1, 0, -1
	place 90, 0x02, SPRITE_WORLD_MAP_SHRINE_OF_THE_SEA_GOD, 10424, 8250, 90, -1, SceneId_ShrineOfTheSeaGodRope, -1, 0, -1
	place 53, 0x01, SPRITE_WORLD_MAP_ATTEKA_INLET, 7265, 8230, 90, -1, SceneId_AttekaInlet, -1, 0, -1
	place 22, 0x01, SPRITE_WORLD_MAP_CLIFFS, 9288, 9530, -180, -1, SceneId_GondowanCliffs, -1, 0, -1
	place 36, 0x00, SPRITE_WORLD_MAP_APOJII_ISLANDS_A, 14530, 8444, 90, -1, SceneId_ApojiiIslands, -1, 0, -1
	place 36, 0x00, SPRITE_WORLD_MAP_APOJII_ISLANDS_A, 14617, 8430, 90, -1, SceneId_ApojiiIslands, -1, 0, -1
	place 36, 0x00, SPRITE_WORLD_MAP_APOJII_ISLANDS_A, 14637, 8360, 90, -1, SceneId_ApojiiIslands, -1, 0, -1
	place 42, 0x00, SPRITE_ROCK_PILE, 12990, 1290, 90, -1, SceneId_SeaOfTimeToLemuria, -1, 0, -1
	place 43, 0x00, SPRITE_ROCK_PILE, 13030, 1290, 90, -1, SceneId_SeaOfTimeToLemuria, -1, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_ALHAFRA_SHIP, 11680, 9302, 90, FLAG_ALHAFRA_SHIP_GONE, -1, -2, 0, -1
	place 120, 0x00, SPRITE_SEATED_IDOL, 9256, 7890, 90, -1, SceneId_SeaOfTimeToLemuria, -1, 0, -1
	place 32, 0x00, SPRITE_WORLD_MAP_CAVE, 13488, 7630, 90, -1, SceneId_YampiDesertGuardianRingChest, -1, 0, -1
	place 25, 0x01, SPRITE_UNSEEN_MARKER, 9139, 8320, 90, -1, SceneId_KibomboMountainsWaft, -1, 0, -1
	place 19, 0x01, SPRITE_UNSEEN_MARKER, 11428, 9753, 90, -1, SceneId_YampiDesertGuardianRingChest, -1, 0, -1
	place 6, 0x01, SPRITE_UNSEEN_MARKER, 10416, 8892, 90, -1, SceneId_DehkanPlateauFullMetalVestChest, -1, 0, -1
	place 35, 0x01, SPRITE_WORLD_MAP_TAOPO_SWAMP, 12720, 10319, 90, -1, SceneId_TaopoSwampEntrance, -1, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 1992, 7272, 90, -1, SceneId_VenusLighthouseEntrance, -1, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_VILLAGE, 9774, 5296, 90, -1, -1, -2, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_SNOWY_VILLAGE, 10024, 4366, 90, -1, -1, -2, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE, 10120, 4264, 90, -1, -1, -2, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_MOUNTAIN, 9752, 5208, 90, -1, -1, -2, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_BABI_LIGHTHOUSE, 10092, 6904, 90, -1, -1, -2, 0, -1
	place 120, 0x00, SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 10184, 7272, 90, -1, -1, -2, 0, -1
	place 127, 0x0f, SPRITE_BLANK, 0, 0, 90, -1, SceneId_WorldMap, 255, 0, -1
	.2byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	@ End.
