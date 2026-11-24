	.include "enums/flags.inc"
	.include "enums/sounds.inc"

@ The field music of each scene (struct MapMusicRule): the first rule that
@ matches the scene or its area, and the entrance (-1 for any), and whose flag
@ (-1 for none) is set, gives the music. A scene's row sets idIsMap, the
@ entrance's top bit; id -1 ends the table.
	.macro scene_music scene:req, entrance:req, flag:req, music:req
	.2byte SceneId_\scene, (\entrance & 0x7fff) | 0x8000, \flag, \music
	.endm
	.macro area_music area:req, entrance:req, flag:req, music:req
	.2byte \area, \entrance & 0x7fff, \flag, \music
	.endm

	.section .rodata
	.balign 4
	.global gScene_MusicRules
gScene_MusicRules:
	area_music 1, -1, -1, MUS_SETTING_OFF
	area_music 2, 90, -1, MUS_SILENCE
	area_music 2, 91, -1, MUS_SILENCE
	area_music 2, 65, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 2, 66, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 2, 67, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 2, 68, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 2, 69, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 2, 70, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 2, 75, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 2, 73, -1, MUS_ELEMENTAL_STARS
	area_music 2, -1, FLAG_DJINN_LESSON, MUS_TRY_YOUR_LUCK
	area_music 2, -1, -1, MUS_A_LONG_JOURNEY
	scene_music Sanctum, -1, FLAG_SANCTUM_QUEST_ACCEPTED, MUS_SETTING_OFF
	scene_music Sanctum, -1, FLAG_SOL_SANCTUM_ERUPTED, MUS_AN_UNSETTLING_FEELING
	scene_music Sanctum, -1, -1, MUS_SETTING_OFF
	scene_music CottageRoomsB, 19, -1, MUS_PALACE_THEME
	area_music 3, -1, FLAG_SANCTUM_QUEST_ACCEPTED, MUS_VILLAGE_THEME
	area_music 3, -1, FLAG_SOL_SANCTUM_ERUPTED, MUS_AN_UNSETTLING_FEELING
	area_music 3, -1, FLAG_VALE_THREE_YEARS_LATER, MUS_VILLAGE_THEME
	area_music 3, -1, FLAG_VALE_STORM_MONSTER_FOUGHT, MUS_DUNGEON_THEME
	area_music 3, -1, -1, MUS_SILENCE
	scene_music SolSanctumElementalStarRoom, -1, -1, MUS_ELEMENTAL_STARS
	scene_music SolSanctumEruption, -1, -1, MUS_MOUNT_ALEPH_ERUPTS
	area_music 4, -1, FLAG_SANCTUM_QUEST_ACCEPTED, MUS_AN_UNSETTLING_FEELING
	area_music 4, -1, FLAG_SOL_SANCTUM_ERUPTED, MUS_MOUNT_ALEPH_ERUPTS
	scene_music SolEntrance, -1, -1, MUS_SOL_SANCTUM
	area_music 4, -1, -1, MUS_SOL_SANCTUM
	scene_music StoneRooms, 2, -1, MUS_TOWN_THEME
	area_music 5, -1, -1, MUS_VILLAGE_THEME
	area_music 6, -1, -1, MUS_A_LONG_JOURNEY
	area_music 7, -1, -1, MUS_CAVE_THEME
	area_music 8, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music 8, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music 9, -1, -1, MUS_PALACE_THEME
	area_music 10, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music 10, -1, -1, MUS_AN_UNSETTLING_FEELING
	scene_music KolimaTunnel, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music 11, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music 11, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music 12, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music 12, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music 13, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music 13, -1, -1, MUS_FOREST_THEME
	area_music 14, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music 14, -1, FLAG_TRET_DEFEATED, MUS_FOREST_THEME
	area_music 14, -1, -1, MUS_SINKING_INTO_SORROW
	scene_music BilibinCaveEntrance, -1, -1, MUS_A_LONG_JOURNEY
	area_music 15, -1, -1, MUS_CAVE_THEME
	area_music 17, -1, FLAG_SATUROS_BEATEN_AT_MERCURY, MUS_REMOTE_VILLAGE
	area_music 17, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music 18, -1, -1, MUS_CAVE_THEME
	scene_music MercuryLighthouseAerie, -1, -1, MUS_ELEMENTAL_STARS
	area_music 19, -1, FLAG_SATUROS_BEATEN_AT_MERCURY, MUS_ELEMENTAL_STARS
	area_music 19, -1, -1, MUS_MERCURY_LIGHTHOUSE
	scene_music LacquerRoomsA, 3, -1, MUS_WIND_ADEPTS
	area_music 20, -1, -1, MUS_TEMPLE_THEME
	area_music 21, -1, -1, MUS_DUNGEON
	area_music 22, -1, -1, MUS_FOREST_THEME
	area_music 23, -1, -1, MUS_TEMPLE_THEME
	area_music 24, -1, -1, MUS_A_LONG_JOURNEY
	area_music 25, -1, FLAG_HYDROS_STATUES_DEFEATED, MUS_VILLAGE_THEME
	area_music 25, -1, -1, MUS_SINKING_INTO_SORROW
	area_music 26, -1, -1, MUS_DUNGEON_THEME
	area_music 27, -1, -1, MUS_TEMPLE_THEME
	area_music 28, -1, -1, MUS_DESERT_THEME
	area_music 29, -1, -1, MUS_VILLAGE_THEME
	area_music 30, -1, -1, MUS_CAVE_THEME
	area_music 31, -1, -1, MUS_CAVE_THEME
	area_music 32, -1, -1, MUS_PORT_TOWN_THEME
	area_music 33, -1, -1, MUS_PALACE_THEME
	scene_music RockRooms, 10, -1, MUS_CAVE_THEME
	area_music 34, -1, -1, MUS_TOWN_THEME
	area_music 35, -1, -1, MUS_CAVE_THEME
	area_music 36, -1, -1, MUS_PORT_TOWN_THEME
	area_music 37, -1, FLAG_TOLBI_SHIP_MONSTERS_SIGHTED, MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE
	area_music 37, -1, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music 38, -1, -1, MUS_PORT_TOWN_THEME
	scene_music CrossboneIsle, -1, -1, MUS_A_LONG_JOURNEY
	scene_music PirateShip, -1, -1, MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES
	area_music 39, -1, -1, MUS_DUNGEON
	area_music 40, -1, FLAG_COLOSSO_OVER, MUS_TOWN_THEME
	area_music 40, -1, -1, MUS_TOLBI
	area_music 41, -1, -1, MUS_PALACE_THEME
	scene_music ColosseumAzartMatch, 1, -1, MUS_TRIAL_ROAD
	scene_music ColosseumSatrageMatch, 1, -1, MUS_TRIAL_ROAD
	scene_music ColosseumNavampaMatch, 1, -1, MUS_TRIAL_ROAD
	scene_music ColosseumAzartMatch, -1, -1, MUS_COLOSSEUM
	scene_music ColosseumSatrageMatch, -1, -1, MUS_COLOSSEUM
	scene_music ColosseumNavampaMatch, -1, -1, MUS_COLOSSEUM
	scene_music Colosseum, 2, -1, MUS_PALACE_THEME
	scene_music Colosseum, 4, -1, MUS_PALACE_THEME
	scene_music Colosseum, 8, -1, MUS_PALACE_THEME
	scene_music Colosseum, 12, -1, MUS_PALACE_THEME
	scene_music Colosseum, 21, -1, MUS_PALACE_THEME
	scene_music Colosseum, 31, -1, MUS_PALACE_THEME
	scene_music Colosseum, 64, -1, MUS_PALACE_THEME
	scene_music ColosseumEntrance, -1, FLAG_COLOSSO_OVER, MUS_TOWN_THEME
	area_music 42, -1, -1, MUS_COLOSSEUM
	area_music 43, -1, -1, MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE
	area_music 44, -1, -1, MUS_A_LONG_JOURNEY
	area_music 45, -1, -1, MUS_A_LONG_JOURNEY
	area_music 46, -1, -1, MUS_CAVE_THEME
	area_music 47, -1, -1, MUS_CAVE_THEME
	area_music 48, -1, -1, MUS_TOWN_THEME
	area_music 49, -1, -1, MUS_DUNGEON_THEME
	area_music 50, -1, -1, MUS_DESERT_THEME
	area_music 51, -1, -1, MUS_REMOTE_VILLAGE
	area_music 52, -1, -1, MUS_CAVE_THEME
	scene_music TunnelRuinsLadyStatues, 1, -1, MUS_DUNGEON
	scene_music TunnelRuinsLadyStatues, 2, -1, MUS_DUNGEON
	scene_music TunnelRuinsLadyStatues, 3, -1, MUS_DUNGEON
	area_music 53, -1, -1, MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE
	area_music 54, -1, -1, MUS_DUNGEON
	area_music 55, -1, FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, MUS_TOWN_THEME
	area_music 55, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music 56, -1, -1, MUS_MERCURY_LIGHTHOUSE
	scene_music VenusLighthouseAerie, -1, FLAG_AREA_TEMP_51, MUS_ELEMENTAL_STARS
	scene_music VenusLighthouseAerie, -1, FLAG_AREA_TEMP_50, MUS_BABIS_THEME
	scene_music VenusLighthouseAerie, -1, -1, MUS_SILENCE
	area_music 57, -1, -1, MUS_VENUS_LIGHTHOUSE
	area_music 58, -1, -1, MUS_ELEMENTAL_STARS
	area_music 59, -1, -1, MUS_TRY_YOUR_LUCK
	area_music 60, -1, -1, MUS_TRY_YOUR_LUCK
	.2byte -1, -1, -1, 0 @ the end
