	.include "enums/flags.inc"
	.include "enums/sprite_ids.inc"
	.include "games/tla/src/field/facing.inc"

@ scene_tables.h's mark on a flag that must be set, not clear; the header
@ computes its enums, so the build cannot hand them to assembly.
	.equiv CONDITION_FLAG_SET, 0x1000

@ The World Map's places (struct WorldMapPlace), which WorldMapPlaces_GetTable
@ returns. Each row: the place, the marker the World Map draws and the one
@ the map screen draws, the sprite the World Map shows there, its x and z in
@ pixels, its facing, the flag it is shown by (-1 always; with
@ CONDITION_FLAG_SET while the flag is set, else while it is clear), the scene
@ and entrance (-1 for any) whose arrival marks it visited (never and -2 when
@ no scene does), the scene (none for none) and entrance choosing it leads
@ to; place 0 ends the table.
	.macro place number:req, marker:req, map_marker:req, sprite:req, x:req, z:req, facing:req, shown_flag:req, visit:req, visit_entrance:req, to:req, to_entrance:req
	.byte \number, \marker | \map_marker << 4
	.2byte \sprite, \x, \z, \facing, \shown_flag
	.ifc \visit, never
	.2byte -1
	.else
	.2byte SceneId_\visit
	.endif
	.2byte \visit_entrance
	.ifc \to, none
	.2byte 0
	.else
	.2byte SceneId_\to
	.endif
	.2byte \to_entrance
	.endm

	.section .rodata
	.balign 4
	.global gWorldMapPlaces
gWorldMapPlaces:
	place 2, 1, 1, SPRITE_WORLD_MAP_DAILA, 10144, 8288, FACING_SOUTH, -1, DailaFlooded, -1, DailaFlooded, -1
	place 4, 1, 0, SPRITE_WORLD_MAP_KANDOREAN_TEMPLE, 10136, 8570, FACING_SOUTH, -1, KandoreanTempleGate, -1, none, -1
	place 5, 0, 0, SPRITE_WORLD_MAP_CAVE, 10424, 8336, FACING_SOUTH, -1, never, -2, none, -1
	place 6, 0, 0, SPRITE_WORLD_MAP_HILL, 10226, 8884, FACING_SOUTH, -1, never, -2, none, -1
	place 7, 0, 0, SPRITE_WORLD_MAP_HILL, 10570, 8928, FACING_SOUTH, -1, never, -2, none, -1
	place 8, 0, 0, SPRITE_UNSEEN_MARKER, 10637, 9075, FACING_SOUTH, -1, LemurianShipMainDeck, -1, none, -1
	place 9, 1, 1, SPRITE_WORLD_MAP_TOWN, 10168, 9603, FACING_SOUTH, -1, Madra, -1, Madra, -1
	place 12, 0, 0, SPRITE_UNSEEN_MARKER, 10394, 9380, FACING_SOUTH, -1, OseniaCliffs, -1, none, -1
	place 13, 0, 0, SPRITE_WORLD_MAP_CLIFFS, 10412, 9395, FACING_SOUTH, -1, OseniaCliffs, -1, none, -1
	place 14, 1, 1, SPRITE_WORLD_MAP_FOREST_VILLAGE, 11125, 10660, FACING_SOUTH, -1, Mikasalla, -1, Mikasalla, -1
	place 15, 1, 1, SPRITE_WORLD_MAP_FOREST_VILLAGE, 12068, 10440, FACING_SOUTH, -1, GarohWayUp, -1, GarohWayUp, -1
	place 16, 2, 0, SPRITE_WORLD_MAP_AIRS_ROCK, 11880, 10064, FACING_SOUTH, -1, AirsRockStormBrandChest, -1, none, -1
	place 19, 0, 0, SPRITE_WORLD_MAP_CAVE, 11910, 9700, FACING_SOUTH, -1, YampiDesertGuardianRingChest, -1, none, -1
	place 20, 1, 1, SPRITE_WORLD_MAP_TOWN, 11633, 9366, FACING_SOUTH, -1, Alhafra, -1, Alhafra, -1
	place 21, 0, 0, SPRITE_UNSEEN_MARKER, 9258, 9520, FACING_EAST, -1, never, -2, none, -1
	place 22, 0, 0, SPRITE_UNSEEN_MARKER, 9316, 9526, FACING_WEST, -1, never, -2, none, -1
	place 23, 1, 1, SPRITE_WORLD_MAP_ROUND_HUT_VILLAGE, 9148, 9018, FACING_SOUTH, -1, Naribwe, -1, Naribwe, -1
	place 25, 0, 0, SPRITE_WORLD_MAP_MOUNTAIN_PATH, 9200, 8380, FACING_SOUTH, -1, never, -2, none, -1
	place 26, 0, 0, SPRITE_WORLD_MAP_MOUNTAIN_PATH, 9077, 8258, FACING_SOUTH, -1, never, -2, none, -1
	place 27, 1, 1, SPRITE_WORLD_MAP_ROUND_HUT_VILLAGE, 9256, 7908, FACING_SOUTH, -1, Kibombo, -1, Kibombo, -1
	place 28, 1, 0, SPRITE_WORLD_MAP_HOUSE, 9546, 8274, FACING_SOUTH, -1, WestIndraIslet, -1, none, -1
	place 29, 1, 0, SPRITE_WORLD_MAP_SNOWY_HOUSE, 12770, 11530, FACING_SOUTH, -1, EastTundariaIslet, -1, none, -1
	place 30, 1, 0, SPRITE_WORLD_MAP_HOUSE, 11724, 8030, FACING_SOUTH, -1, NorthOseniaIslet, -1, none, -1
	place 31, 1, 0, SPRITE_WORLD_MAP_HOUSE, 11680, 6640, FACING_SOUTH, -1, SoutheastAngaraIslet, -1, none, -1
	place 32, 1, 0, SPRITE_WORLD_MAP_HOUSE, 13488, 7519, FACING_SOUTH, -1, SeaOfTimeIslet, -1, none, -1
	place 33, 1, 1, SPRITE_WORLD_MAP_FOREST_VILLAGE, 12923, 9713, FACING_SOUTH, -1, Yallam, -1, Yallam, -1
	place 35, 0, 0, SPRITE_UNSEEN_MARKER, 12695, 10297, FACING_SOUTH, -1, never, -2, none, -1
	place 36, 1, 1, SPRITE_WORLD_MAP_APOJII_ISLANDS_B, 14517, 8413, FACING_SOUTH, -1, ApojiiIslands, -1, ApojiiIslands, -1
	place 37, 2, 0, SPRITE_WORLD_MAP_AQUA_ROCK, 14256, 8708, FACING_SOUTH, -1, AquaRockMoai, -1, none, -1
	place 38, 1, 1, SPRITE_WORLD_MAP_GOLD_ROOFED_TOWN, 13406, 6068, FACING_SOUTH, -1, Izumo, -1, Izumo, -1
	place 39, 2, 0, SPRITE_WORLD_MAP_GAIA_ROCK, 13450, 6178, FACING_SOUTH, -1, GaiaRockSusa, -1, none, -1
	place 40, 1, 1, SPRITE_WORLD_MAP_GOLD_ROOFED_TOWN, 10904, 6258, FACING_SOUTH, -1, Champa, -1, Champa, -1
	place 41, 2, 0, SPRITE_WORLD_MAP_ANKOHL_RUINS, 11204, 6081, FACING_SOUTH, -1, AnkohlRuinsStoneFaces, -1, none, -1
	place 42, 0, 0, SPRITE_ROCK_PILE, 12437, 7523, FACING_SOUTH, -1, SeaOfTimeToLemuria, -1, none, -1
	place 43, 0, 0, SPRITE_ROCK_PILE, 12490, 7523, FACING_SOUTH, -1, SeaOfTimeToLemuria, -1, none, -1
	place 45, 1, 2, SPRITE_UNSEEN_MARKER, 12461, 7142, FACING_SOUTH, -1, LemuriaEntrance, -1, LemuriaEntrance, -1
	place 45, 0, 0, SPRITE_WORLD_MAP_LEMURIA, 13088, 1058, FACING_SOUTH, -1, LemuriaEntrance, -1, LemuriaEntrance, -1
	place 46, 2, 0, SPRITE_WORLD_MAP_TUNDARIA_TOWER, 9490, 10930, FACING_SOUTH, -1, TundariaTowerEntrance, -1, none, -1
	place 47, 1, 0, SPRITE_WORLD_MAP_HOUSE, 5810, 10134, FACING_SOUTH, -1, SouthwestAttekaIslet, -1, none, -1
	place 48, 1, 0, SPRITE_WORLD_MAP_HOUSE, 8582, 7380, FACING_SOUTH, -1, GondowanSettlement, -1, none, -1
	place 49, 1, 0, SPRITE_WORLD_MAP_HOUSE, 6307, 6200, FACING_SOUTH, -1, HesperiaSettlement, -1, none, -1
	place 50, 1, 0, SPRITE_WORLD_MAP_SNOWY_HOUSE, 8848, 4600, FACING_SOUTH, -1, KaltIsland, -1, none, -1
	place 51, 2, 0, SPRITE_WORLD_MAP_MAGMA_ROCK, 8970, 7460, FACING_SOUTH, -1, MagmaRockSlide, -1, none, -1
	place 54, 1, 1, SPRITE_WORLD_MAP_SANDY_TOWN, 7714, 7730, FACING_SOUTH, -1, Contigo, -1, Contigo, -1
	place 55, 2, 0, SPRITE_WORLD_MAP_JUPITER_LIGHTHOUSE, 7279, 7303, FACING_SOUTH, -1, JupiterLighthouseEntrance, -1, JupiterLighthouseAerie, -1
	place 56, 0, 0, SPRITE_WORLD_MAP_CAVE, 7382, 5720, FACING_SOUTH, -1, ShamanVillageTrialGrounds, -1, none, -1
	place 57, 0, 0, SPRITE_WORLD_MAP_CAVE, 7500, 5628, FACING_SOUTH, -1, ShamanVillageTrialGrounds, -1, none, -1
	place 58, 1, 1, SPRITE_WORLD_MAP_SHAMAN_VILLAGE, 7662, 5499, FACING_SOUTH, -1, ShamanVillage, -1, ShamanVillage, -1
	place 59, 1, 1, SPRITE_WORLD_MAP_SANDY_TOWN, 8978, 5838, FACING_SOUTH, -1, Loho, -1, Loho, -1
	.ifdef TLA_EDITION_JA
	place 62, 1, 1, SPRITE_UNSEEN_MARKER, 8893, 3020, FACING_SOUTH, -1, Prox, -1, Prox, -1
	.else
	place 62, 1, 1, SPRITE_UNSEEN_MARKER, 9120, 3180, FACING_SOUTH, -1, Prox, -1, Prox, -1
	.endif
	place 62, 0, 0, SPRITE_WORLD_MAP_SNOWY_VILLAGE, 9055, 1214, FACING_SOUTH, -1, Prox, -1, Prox, -1
	place 82, 0, 0, SPRITE_WORLD_MAP_CAVE, 11596, 10170, FACING_SOUTH, -1, CavernsOsenia, -1, none, -1
	place 83, 0, 0, SPRITE_WORLD_MAP_CAVE, 10240, 9124, FACING_SOUTH, -1, CavernsIndra, -1, none, -1
	place 84, 0, 0, SPRITE_WORLD_MAP_MARS_LIGHTHOUSE_A, 9020, 850, FACING_SOUTH, FLAG_MARS_LIGHTHOUSE_THAWED|CONDITION_FLAG_SET, MarsLighthouseOutside, -1, none, -1
	place 84, 0, 0, SPRITE_WORLD_MAP_MARS_LIGHTHOUSE_B, 9020, 850, FACING_SOUTH, FLAG_MARS_LIGHTHOUSE_THAWED, MarsLighthouseOutside, -1, none, -1
	place 87, 1, 0, SPRITE_WORLD_MAP_CAVE, 12640, 4360, FACING_SOUTH, -1, TreasureIsleEntrance, -1, none, -1
	place 88, 0, 0, SPRITE_WORLD_MAP_CAVE, 7868, 9094, FACING_SOUTH, -1, CavernsAtteka, -1, none, -1
	place 89, 0, 0, SPRITE_WORLD_MAP_CAVE, 9479, 5020, FACING_SOUTH, -1, CavernsAngara, -1, none, -1
	place 90, 2, 0, SPRITE_WORLD_MAP_SHRINE_OF_THE_SEA_GOD, 10424, 8250, FACING_SOUTH, -1, ShrineOfTheSeaGodRope, -1, none, -1
	place 53, 1, 0, SPRITE_WORLD_MAP_ATTEKA_INLET, 7265, 8230, FACING_SOUTH, -1, AttekaInlet, -1, none, -1
	place 22, 1, 0, SPRITE_WORLD_MAP_CLIFFS, 9288, 9530, FACING_WEST, -1, GondowanCliffs, -1, none, -1
	place 36, 0, 0, SPRITE_WORLD_MAP_APOJII_ISLANDS_A, 14530, 8444, FACING_SOUTH, -1, ApojiiIslands, -1, none, -1
	place 36, 0, 0, SPRITE_WORLD_MAP_APOJII_ISLANDS_A, 14617, 8430, FACING_SOUTH, -1, ApojiiIslands, -1, none, -1
	place 36, 0, 0, SPRITE_WORLD_MAP_APOJII_ISLANDS_A, 14637, 8360, FACING_SOUTH, -1, ApojiiIslands, -1, none, -1
	place 42, 0, 0, SPRITE_ROCK_PILE, 12990, 1290, FACING_SOUTH, -1, SeaOfTimeToLemuria, -1, none, -1
	place 43, 0, 0, SPRITE_ROCK_PILE, 13030, 1290, FACING_SOUTH, -1, SeaOfTimeToLemuria, -1, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_ALHAFRA_SHIP, 11680, 9302, FACING_SOUTH, FLAG_ALHAFRA_SHIP_GONE, never, -2, none, -1
	place 120, 0, 0, SPRITE_SEATED_IDOL, 9256, 7890, FACING_SOUTH, -1, SeaOfTimeToLemuria, -1, none, -1
	place 32, 0, 0, SPRITE_WORLD_MAP_CAVE, 13488, 7630, FACING_SOUTH, -1, YampiDesertGuardianRingChest, -1, none, -1
	place 25, 1, 0, SPRITE_UNSEEN_MARKER, 9139, 8320, FACING_SOUTH, -1, KibomboMountainsWaft, -1, none, -1
	place 19, 1, 0, SPRITE_UNSEEN_MARKER, 11428, 9753, FACING_SOUTH, -1, YampiDesertGuardianRingChest, -1, none, -1
	place 6, 1, 0, SPRITE_UNSEEN_MARKER, 10416, 8892, FACING_SOUTH, -1, DehkanPlateauFullMetalVestChest, -1, none, -1
	place 35, 1, 0, SPRITE_WORLD_MAP_TAOPO_SWAMP, 12720, 10319, FACING_SOUTH, -1, TaopoSwampEntrance, -1, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 1992, 7272, FACING_SOUTH, -1, VenusLighthouseEntrance, -1, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_VILLAGE, 9774, 5296, FACING_SOUTH, -1, never, -2, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_SNOWY_VILLAGE, 10024, 4366, FACING_SOUTH, -1, never, -2, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE, 10120, 4264, FACING_SOUTH, -1, never, -2, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_MOUNTAIN, 9752, 5208, FACING_SOUTH, -1, never, -2, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_BABI_LIGHTHOUSE, 10092, 6904, FACING_SOUTH, -1, never, -2, none, -1
	place 120, 0, 0, SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 10184, 7272, FACING_SOUTH, -1, never, -2, none, -1
	place 127, 15, 0, SPRITE_BLANK, 0, 0, FACING_SOUTH, -1, WorldMap, 255, none, -1
	.fill 20 @ the end
