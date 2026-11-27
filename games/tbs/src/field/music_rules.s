	.include "enums/area_ids.inc"
	.include "enums/flags.inc"
	.include "enums/sounds.inc"

@ The field music of each scene (MAP_MUSIC_RULE): the first rule that
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
	.global Scene_MusicRules
Scene_MusicRules:
	area_music AREA_CLEAR_DATA, -1, -1, MUS_SETTING_OFF
	area_music AREA_WORLD_MAP, 90, -1, MUS_SILENCE
	area_music AREA_WORLD_MAP, 91, -1, MUS_SILENCE
	area_music AREA_WORLD_MAP, 65, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_WORLD_MAP, 66, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_WORLD_MAP, 67, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_WORLD_MAP, 68, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_WORLD_MAP, 69, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_WORLD_MAP, 70, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_WORLD_MAP, 75, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_WORLD_MAP, 73, -1, MUS_ELEMENTAL_STARS
	area_music AREA_WORLD_MAP, -1, FLAG_DJINN_LESSON, MUS_TRY_YOUR_LUCK
	area_music AREA_WORLD_MAP, -1, -1, MUS_A_LONG_JOURNEY
	scene_music Sanctum, -1, FLAG_SANCTUM_QUEST_ACCEPTED, MUS_SETTING_OFF
	scene_music Sanctum, -1, FLAG_SOL_SANCTUM_ERUPTED, MUS_AN_UNSETTLING_FEELING
	scene_music Sanctum, -1, -1, MUS_SETTING_OFF
	scene_music CottageRoomsB, 19, -1, MUS_PALACE_THEME
	area_music AREA_VALE, -1, FLAG_SANCTUM_QUEST_ACCEPTED, MUS_VILLAGE_THEME
	area_music AREA_VALE, -1, FLAG_SOL_SANCTUM_ERUPTED, MUS_AN_UNSETTLING_FEELING
	area_music AREA_VALE, -1, FLAG_VALE_THREE_YEARS_LATER, MUS_VILLAGE_THEME
	area_music AREA_VALE, -1, FLAG_VALE_STORM_MONSTER_FOUGHT, MUS_DUNGEON_THEME
	area_music AREA_VALE, -1, -1, MUS_SILENCE
	scene_music SolSanctumElementalStarRoom, -1, -1, MUS_ELEMENTAL_STARS
	scene_music SolSanctumEruption, -1, -1, MUS_MOUNT_ALEPH_ERUPTS
	area_music AREA_SOL_SANCTUM, -1, FLAG_SANCTUM_QUEST_ACCEPTED, MUS_AN_UNSETTLING_FEELING
	area_music AREA_SOL_SANCTUM, -1, FLAG_SOL_SANCTUM_ERUPTED, MUS_MOUNT_ALEPH_ERUPTS
	scene_music SolEntrance, -1, -1, MUS_SOL_SANCTUM
	area_music AREA_SOL_SANCTUM, -1, -1, MUS_SOL_SANCTUM
	scene_music StoneRooms, 2, -1, MUS_TOWN_THEME
	area_music AREA_VAULT, -1, -1, MUS_VILLAGE_THEME
	area_music AREA_GOMA_CAVE_ENTRANCE, -1, -1, MUS_A_LONG_JOURNEY
	area_music AREA_GOMA_CAVE, -1, -1, MUS_CAVE_THEME
	area_music AREA_BILIBIN, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music AREA_BILIBIN, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music AREA_MCCOYS_PALACE, -1, -1, MUS_PALACE_THEME
	area_music AREA_BILIBIN_BARRICADE, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music AREA_BILIBIN_BARRICADE, -1, -1, MUS_AN_UNSETTLING_FEELING
	scene_music KolimaTunnel, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music AREA_KOLIMA, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music AREA_KOLIMA, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music AREA_KOLIMA_BRIDGE, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music AREA_KOLIMA_BRIDGE, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music AREA_KOLIMA_FOREST, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music AREA_KOLIMA_FOREST, -1, -1, MUS_FOREST_THEME
	area_music AREA_TRET_TREE, -1, FLAG_KOLIMA_CURSE_LIFTED, MUS_TOWN_THEME
	area_music AREA_TRET_TREE, -1, FLAG_TRET_DEFEATED, MUS_FOREST_THEME
	area_music AREA_TRET_TREE, -1, -1, MUS_SINKING_INTO_SORROW
	scene_music BilibinCaveEntrance, -1, -1, MUS_A_LONG_JOURNEY
	area_music AREA_BILIBIN_CAVE, -1, -1, MUS_CAVE_THEME
	area_music AREA_IMIL, -1, FLAG_SATUROS_BEATEN_AT_MERCURY, MUS_REMOTE_VILLAGE
	area_music AREA_IMIL, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music AREA_IMIL_FALLS_CAVE, -1, -1, MUS_CAVE_THEME
	scene_music MercuryLighthouseAerie, -1, -1, MUS_ELEMENTAL_STARS
	area_music AREA_MERCURY_LIGHTHOUSE, -1, FLAG_SATUROS_BEATEN_AT_MERCURY, MUS_ELEMENTAL_STARS
	area_music AREA_MERCURY_LIGHTHOUSE, -1, -1, MUS_MERCURY_LIGHTHOUSE
	scene_music LacquerRoomsA, 3, -1, MUS_WIND_ADEPTS
	area_music AREA_FUCHIN_TEMPLE, -1, -1, MUS_TEMPLE_THEME
	area_music AREA_FUCHIN_FALLS_CAVE, -1, -1, MUS_DUNGEON
	area_music AREA_MOGALL_FOREST, -1, -1, MUS_FOREST_THEME
	area_music AREA_XIAN, -1, -1, MUS_TEMPLE_THEME
	area_music AREA_ALPINE_CROSSING, -1, -1, MUS_A_LONG_JOURNEY
	area_music AREA_ALTIN, -1, FLAG_HYDROS_STATUES_DEFEATED, MUS_VILLAGE_THEME
	area_music AREA_ALTIN, -1, -1, MUS_SINKING_INTO_SORROW
	area_music AREA_ALTIN_PEAK, -1, -1, MUS_DUNGEON_THEME
	area_music AREA_LAMA_TEMPLE, -1, -1, MUS_TEMPLE_THEME
	area_music AREA_LAMAKAN_DESERT, -1, -1, MUS_DESERT_THEME
	area_music AREA_VALE_CAVE_MOUTH, -1, -1, MUS_VILLAGE_THEME
	area_music AREA_VALE_CAVE, -1, -1, MUS_CAVE_THEME
	area_music AREA_VAULT_CAVE, -1, -1, MUS_CAVE_THEME
	area_music AREA_KALAY, -1, -1, MUS_PORT_TOWN_THEME
	area_music AREA_HAMMET_PALACE, -1, -1, MUS_PALACE_THEME
	scene_music RockRooms, 10, -1, MUS_CAVE_THEME
	area_music AREA_LUNPA, -1, -1, MUS_TOWN_THEME
	area_music AREA_WEST_LUNPA_CAVE, -1, -1, MUS_CAVE_THEME
	area_music AREA_KALAY_DOCKS, -1, -1, MUS_PORT_TOWN_THEME
	area_music AREA_TOLBI_BOUND_SHIP, -1, FLAG_TOLBI_SHIP_MONSTERS_SIGHTED, MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE
	area_music AREA_TOLBI_BOUND_SHIP, -1, -1, MUS_SET_SAIL_ON_THE_KARAGOL_SEA
	area_music AREA_TOLBI_DOCKS, -1, -1, MUS_PORT_TOWN_THEME
	scene_music CrossboneIsle, -1, -1, MUS_A_LONG_JOURNEY
	scene_music PirateShip, -1, -1, MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES
	area_music AREA_CROSSBONE_ISLE_CAVE, -1, -1, MUS_DUNGEON
	area_music AREA_TOLBI, -1, FLAG_COLOSSO_OVER, MUS_TOWN_THEME
	area_music AREA_TOLBI, -1, -1, MUS_TOLBI
	area_music AREA_BABI_PALACE, -1, -1, MUS_PALACE_THEME
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
	area_music AREA_COLOSSEUM, -1, -1, MUS_COLOSSEUM
	area_music AREA_ALTMILLER_CAVE, -1, -1, MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE
	area_music AREA_GONDOWAN_PASSAGE, -1, -1, MUS_A_LONG_JOURNEY
	area_music AREA_KALAY_TUNNEL_ENTRANCE, -1, -1, MUS_A_LONG_JOURNEY
	area_music AREA_KALAY_TUNNEL, -1, -1, MUS_CAVE_THEME
	area_music AREA_GONDOWAN_CAVE, -1, -1, MUS_CAVE_THEME
	area_music AREA_LUNPA_FORTRESS_GATES, -1, -1, MUS_TOWN_THEME
	area_music AREA_LUNPA_FORTRESS, -1, -1, MUS_DUNGEON_THEME
	area_music AREA_SUHALLA_DESERT, -1, -1, MUS_DESERT_THEME
	area_music AREA_SUHALLA, -1, -1, MUS_REMOTE_VILLAGE
	area_music AREA_SUHALLA_GATE, -1, -1, MUS_CAVE_THEME
	scene_music TunnelRuinsLadyStatues, 1, -1, MUS_DUNGEON
	scene_music TunnelRuinsLadyStatues, 2, -1, MUS_DUNGEON
	scene_music TunnelRuinsLadyStatues, 3, -1, MUS_DUNGEON
	area_music AREA_TUNNEL_RUINS, -1, -1, MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE
	area_music AREA_BABI_LIGHTHOUSE, -1, -1, MUS_DUNGEON
	area_music AREA_LALIVERO, -1, FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, MUS_TOWN_THEME
	area_music AREA_LALIVERO, -1, -1, MUS_AN_UNSETTLING_FEELING
	area_music AREA_VENUS_LIGHTHOUSE_ENTRY, -1, -1, MUS_MERCURY_LIGHTHOUSE
	scene_music VenusLighthouseAerie, -1, FLAG_AREA_TEMP_51, MUS_ELEMENTAL_STARS
	scene_music VenusLighthouseAerie, -1, FLAG_AREA_TEMP_50, MUS_BABIS_THEME
	scene_music VenusLighthouseAerie, -1, -1, MUS_SILENCE
	area_music AREA_VENUS_LIGHTHOUSE, -1, -1, MUS_VENUS_LIGHTHOUSE
	area_music AREA_LEMURIAN_SHIP, -1, -1, MUS_ELEMENTAL_STARS
	area_music AREA_TOLBI_SPRING, -1, -1, MUS_TRY_YOUR_LUCK
	area_music AREA_BATTLE_ARENA, -1, -1, MUS_TRY_YOUR_LUCK
	.2byte -1, -1, -1, 0 @ the end
