#ifndef ALCHEMY_ENEMY_IDS_H
#define ALCHEMY_ENEMY_IDS_H

/* Enemy ids, named from the English release. */

/* The first enemy id: enemy id `ENEMY_ID_FIRST + n` is the enemy in row n of
   enemy_table.c (gmEnemyGetDefinition); the ids below it are the party's
   characters. */
#define ENEMY_ID_FIRST 8

/* How many enemies enemy_table.c defines. */
#define ENEMY_COUNT 380

/* The highest enemy id a battle unit takes; an id past the table's rows takes
   its first row. */
#define ENEMY_ID_MAX 1000

/* Enemy id `id`'s row of enemy_table.c. */
#define ENEMY_ROW(id) ((id) - ENEMY_ID_FIRST)

/* Enemy ids, one for each row of enemy_table.c in order, each named from the
   English release (MsgEnemyName plus the row): an enemy id is the row plus
   ENEMY_ID_FIRST. The rows the text names "?" are unused and have no names
   here. Where several rows share a name, each takes what tells its battle
   apart: the one place it is fought, or the Djinni a Djinni battle is with;
   rows the game's own text numbers ("Serpent 6", "Ruffian 2") keep those
   numbers. */
typedef enum ENEMY_ID {
	/* Row 0: "???", in no formation; a formation's empty places list it. */
	ENEMY_NONE = ENEMY_ID_FIRST,
	/* The Djinn the party fights, each element's in the order of their numbers,
	   The Broken Seal's rows first; gmBattleUnitSet puts them on the party's
	   side. A Djinni's battle is a slot of its element's zone, which two Djinn
	   share (fdEncounterGetDjinnFormation); each row is named for the one
	   Djinni whose scene starts its battle (fdRequestDjinnBattle). */
	/* Unused: The Broken Seal's row for Quartz, level 12, in no formation
	   here. */
	ENEMY_VENUS_DJINNI_UNUSED_QUARTZ,
	/* Unused: The Broken Seal's row for Vine, level 15. */
	ENEMY_VENUS_DJINNI_UNUSED_VINE,
	/* Unused: The Broken Seal's row for Bane, level 23. */
	ENEMY_VENUS_DJINNI_UNUSED_BANE,
	/* Unused: level 3, the battle of Echo's slot, but Echo joins on the World
	   Map without a fight. */
	ENEMY_VENUS_DJINNI_UNUSED_ECHO,
	/* Level 9, on the World Map. */
	ENEMY_VENUS_DJINNI_IRON,
	/* Level 19, in the Gabomba Statue. */
	ENEMY_VENUS_DJINNI_STEEL,
	/* Level 20, in the Gabomba Catacombs. */
	ENEMY_VENUS_DJINNI_MUD,
	/* Level 23, in Taopo Swamp. */
	ENEMY_VENUS_DJINNI_FLOWER,
	/* Level 27, in the Islet Cave. */
	ENEMY_VENUS_DJINNI_MELD,
	/* Level 31, on the World Map. */
	ENEMY_VENUS_DJINNI_PETRA,
	/* Unused: level 3; Salt joins without a fight. */
	ENEMY_VENUS_DJINNI_UNUSED_SALT,
	/* Unused: level 3; Geode joins without a fight. */
	ENEMY_VENUS_DJINNI_UNUSED_GEODE,
	/* Unused: level 3; Mold joins without a fight. */
	ENEMY_VENUS_DJINNI_UNUSED_MOLD,
	/* Level 36, in the Yampi Desert Cave. */
	ENEMY_VENUS_DJINNI_CRYSTAL,
	/* Unused: The Broken Seal's row for Sleet, level 10. */
	ENEMY_MERCURY_DJINNI_UNUSED_SLEET,
	/* Unused: The Broken Seal's row for Spritz, level 13. */
	ENEMY_MERCURY_DJINNI_UNUSED_SPRITZ,
	/* Unused: The Broken Seal's row for Hail, level 18. */
	ENEMY_MERCURY_DJINNI_UNUSED_HAIL,
	/* Unused: The Broken Seal's row for Dew, level 24. */
	ENEMY_MERCURY_DJINNI_UNUSED_DEW,
	/* Level 6, in the Kandorean Temple. */
	ENEMY_MERCURY_DJINNI_FOG,
	/* Level 12, on the World Map. */
	ENEMY_MERCURY_DJINNI_SOUR,
	/* Unused: level 3; Spring joins without a fight. */
	ENEMY_MERCURY_DJINNI_UNUSED_SPRING,
	/* Unused: level 3; Shade joins without a fight. */
	ENEMY_MERCURY_DJINNI_UNUSED_SHADE,
	/* Level 17, on the World Map. */
	ENEMY_MERCURY_DJINNI_CHILL,
	/* Level 24, in Aqua Rock. */
	ENEMY_MERCURY_DJINNI_STEAM,
	/* Unused: level 3; Rime joins without a fight. */
	ENEMY_MERCURY_DJINNI_UNUSED_RIME,
	/* Unused: level 3; Gel joins without a fight. */
	ENEMY_MERCURY_DJINNI_UNUSED_GEL,
	/* Level 32, in the Shaman Village Cave. */
	ENEMY_MERCURY_DJINNI_EDDY,
	/* Level 35, at Mars Lighthouse's entrance. */
	ENEMY_MERCURY_DJINNI_BALM,
	/* Level 34, in the Islet Cave. */
	ENEMY_MERCURY_DJINNI_SERAC,
	/* Unused: The Broken Seal's row for Forge, level 5. */
	ENEMY_MARS_DJINNI_UNUSED_FORGE,
	/* Unused: The Broken Seal's row for Corona, level 13. */
	ENEMY_MARS_DJINNI_UNUSED_CORONA,
	/* Unused: The Broken Seal's row for Flash, level 22. */
	ENEMY_MARS_DJINNI_UNUSED_FLASH,
	/* Level 8, on the Dehkan Plateau. */
	ENEMY_MARS_DJINNI_CANNON,
	/* Unused: level 3; Spark joins without a fight. */
	ENEMY_MARS_DJINNI_UNUSED_SPARK,
	/* Level 16, on the Gondowan Cliffs. */
	ENEMY_MARS_DJINNI_KINDLE,
	/* Unused: level 3; Char joins without a fight. */
	ENEMY_MARS_DJINNI_UNUSED_CHAR,
	/* Unused: level 3; Coal joins without a fight. */
	ENEMY_MARS_DJINNI_UNUSED_COAL,
	/* Level 28, in Tundaria Tower. */
	ENEMY_MARS_DJINNI_REFLUX,
	/* Level 31, on the World Map. */
	ENEMY_MARS_DJINNI_CORE,
	/* Unused: level 3; Tinder joins without a fight. */
	ENEMY_MARS_DJINNI_UNUSED_TINDER,
	/* Unused: level 3; Shine joins without a fight. */
	ENEMY_MARS_DJINNI_UNUSED_SHINE,
	/* Level 33, in Magma Rock. */
	ENEMY_MARS_DJINNI_FURY,
	/* Level 35, at Mars Lighthouse's entrance. */
	ENEMY_MARS_DJINNI_FUGUE,
	/* Unused: The Broken Seal's row for Breeze, level 8. */
	ENEMY_JUPITER_DJINNI_UNUSED_BREEZE,
	/* Unused: The Broken Seal's row for Zephyr, level 11. */
	ENEMY_JUPITER_DJINNI_UNUSED_ZEPHYR,
	/* Unused: The Broken Seal's row for Squall, level 18. */
	ENEMY_JUPITER_DJINNI_UNUSED_SQUALL,
	/* Unused: The Broken Seal's row for Luff, level 25. */
	ENEMY_JUPITER_DJINNI_UNUSED_LUFF,
	/* Level 7, in the Shrine of the Sea God. */
	ENEMY_JUPITER_DJINNI_BREATH,
	/* Level 11, in the Yampi Desert. */
	ENEMY_JUPITER_DJINNI_BLITZ,
	/* Unused: level 3; Ether joins without a fight. */
	ENEMY_JUPITER_DJINNI_UNUSED_ETHER,
	/* Level 18, in the Kibombo Mountains. */
	ENEMY_JUPITER_DJINNI_WAFT,
	/* Unused: level 3; Haze joins without a fight. */
	ENEMY_JUPITER_DJINNI_UNUSED_HAZE,
	/* Level 28, on the World Map. */
	ENEMY_JUPITER_DJINNI_WHEEZE,
	/* Unused: level 3; Aroma joins without a fight. */
	ENEMY_JUPITER_DJINNI_UNUSED_AROMA,
	/* Level 32, in Jupiter Lighthouse. */
	ENEMY_JUPITER_DJINNI_WHORL,
	/* Level 32, on the Trial Road. */
	ENEMY_JUPITER_DJINNI_GASP,
	/* Unused: level 3; Lull joins without a fight. */
	ENEMY_JUPITER_DJINNI_UNUSED_LULL,
	/* Level 36, on Treasure Isle. */
	ENEMY_JUPITER_DJINNI_GALE,
	ENEMY_RUFFIAN = ENEMY_ID_FIRST + 60,
	ENEMY_RUFFIAN_2,
	ENEMY_RUFFIAN_3,
	ENEMY_PUNCH_ANT,
	ENEMY_FLASH_ANT,
	ENEMY_NUMB_ANT,
	ENEMY_CHESTBEATER,
	ENEMY_WILD_GORILLA,
	ENEMY_CRAZY_GORILLA,
	ENEMY_KING_SCORPION,
	ENEMY_DEVIL_SCORPION,
	ENEMY_SAND_SCORPION,
	ENEMY_BRIGGS,
	ENEMY_SEA_FIGHTER,
	ENEMY_CHAMPA_2,
	ENEMY_CHAMPA_3,
	ENEMY_CUTTLE,
	ENEMY_CALAMAR,
	ENEMY_MAN_O_WAR,
	ENEMY_AQUA_JELLY,
	ENEMY_AQUA_HYDRA,
	ENEMY_HYDRA,
	ENEMY_PYRODRA,
	/* Gaia Rock's Serpent: a row for each number of the four beams of sunlight
	   let into its lair as it is fought, counted by
	   scGaiaRockInsideChallengeSerpent, this one for none. Each beam lights a
	   band across the lair's floor to the Serpent. */
	ENEMY_SERPENT,
	ENEMY_SERPENT_ONE_BEAM,
	ENEMY_SERPENT_TWO_BEAMS,
	ENEMY_SERPENT_THREE_BEAMS,
	ENEMY_SERPENT_FOUR_BEAMS,
	ENEMY_SERPENT_6,
	ENEMY_SERPENT_7,
	ENEMY_AVIMANDER,
	ENEMY_MACETAIL,
	ENEMY_BOMBANDER,
	ENEMY_POSEIDON,
	/* The abilities Poseidon chooses from for his second action of each turn
	   (btAIChooseAction); his first takes ENEMY_POSEIDON's. */
	ENEMY_POSEIDON_SECOND_ACTION,
	ENEMY_POSEIDON_3,
	/* Moapa at the Trial Road's peak. He and his two Knights come weaker, with
	   less defense and fewer Bosca Hits or Strong Hits, the more stage doors
	   the party reached before them in the race (scTrialRoadChallengeMoapa,
	   ENCOUNTER_ZONE_MOAPA): this row while it reached two or fewer,
	   ENEMY_MOAPA_B at three and ENEMY_MOAPA_C at all four. */
	ENEMY_MOAPA,
	ENEMY_MOAPA_B,
	ENEMY_MOAPA_C,
	/* One of Moapa's Knights: both are this row while the party reached no
	   stage door first, one at one door, and neither from two doors on, when
	   both are ENEMY_KNIGHT_B. */
	ENEMY_KNIGHT,
	ENEMY_KNIGHT_B,
	ENEMY_KNIGHT_3,
	/* Agatio in the first two of the three battles on Jupiter Lighthouse's
	   aerie (ENCOUNTER_ZONE_MOAPA, slots 5 and 6), as ENEMY_KARST is Karst. */
	ENEMY_AGATIO,
	/* Agatio in the third battle (slot 7), the same stats as ENEMY_AGATIO, as
	   ENEMY_KARST_B is Karst. */
	ENEMY_AGATIO_B,
	ENEMY_AGATIO_VS_ISAAC,
	ENEMY_KARST,
	ENEMY_KARST_B,
	ENEMY_KARST_VS_ISAAC,
	ENEMY_WILD_WOLF = ENEMY_ID_FIRST + 109,
	ENEMY_DIRE_WOLF,
	ENEMY_WHITE_WOLF,
	ENEMY_ANGLE_WORM,
	ENEMY_FIRE_WORM,
	ENEMY_CHIMERA_WORM,
	ENEMY_MINI_GOBLIN,
	ENEMY_ALEC_GOBLIN,
	ENEMY_BABOON_GOBLIN,
	ENEMY_MOMONGA,
	ENEMY_SQUIRRELFANG,
	ENEMY_MOMANGLER,
	ENEMY_KOBOLD,
	ENEMY_WARGOLD,
	ENEMY_KOBOLD_KING,
	/* The Mad Plant of the Gabomba Catacombs (ENCOUNTER_ZONE_MAD_PLANTS, slot
	   0); each Mad Plant row is the one fought in one place. */
	ENEMY_MAD_PLANT_GABOMBA_CATACOMBS,
	/* The Mad Plant of Taopo Swamp (slot 1). */
	ENEMY_MAD_PLANT_TAOPO_SWAMP,
	/* The Mad Plant of Gaia Rock (slot 2). */
	ENEMY_MAD_PLANT_GAIA_ROCK,
	/* The Mad Plant of Jupiter Lighthouse's basement (slot 3). */
	ENEMY_MAD_PLANT_JUPITER_LIGHTHOUSE,
	/* The Mad Plant of the Trial Road (slot 4). */
	ENEMY_MAD_PLANT_TRIAL_ROAD,
	ENEMY_EMU,
	ENEMY_TALON_RUNNER,
	ENEMY_WINGED_RUNNER,
	ENEMY_MUMMY,
	ENEMY_FOUL_MUMMY,
	ENEMY_GRAVE_WIGHT,
	ENEMY_WYVERN_CHICK,
	ENEMY_PTERANODON,
	ENEMY_WINGED_LIZARD,
	ENEMY_WOLFKIN_CUB,
	ENEMY_WOLFKIN,
	ENEMY_SKINWALKER,
	ENEMY_DINO,
	ENEMY_DINOX,
	ENEMY_DINOSAURUS,
	ENEMY_ASSASSIN,
	ENEMY_SLAYER,
	ENEMY_DARK_MURDER,
	ENEMY_DOOMSAYER,
	ENEMY_LICH,
	ENEMY_BANE_WIGHT,
	ENEMY_PIXIE,
	ENEMY_FAERY,
	/* Spelled as the English release spells it. */
	ENEMY_WEIRD_NYPMH,
	ENEMY_WOOD_WALKER,
	ENEMY_ELDER_WOOD,
	ENEMY_ESTRE_WOOD,
	ENEMY_STAR_MAGICIAN,
	ENEMY_DARK_WIZARD,
	ENEMY_EVIL_SHAMAN,
	ENEMY_URCHIN_BEAST,
	ENEMY_NEEDLE_EGG,
	ENEMY_SEA_HEDGEHOG,
	ENEMY_CONCH_SHELL,
	ENEMY_SPIRAL_SHELL,
	ENEMY_POISON_SHELL,
	ENEMY_MERMAN,
	ENEMY_GILLMAN,
	ENEMY_GILLMAN_LORD,
	ENEMY_SEA_DRAGON,
	ENEMY_TURTLE_DRAGON,
	ENEMY_OCEAN_DRAGON,
	ENEMY_SEABIRD,
	ENEMY_SEAFOWL,
	ENEMY_GREAT_SEAGULL,
	ENEMY_ROC,
	ENEMY_RAPTOR,
	ENEMY_FELL_RAPTOR,
	ENEMY_WYVERN,
	ENEMY_SKY_DRAGON,
	ENEMY_WINGED_DRAGON,
	ENEMY_PHOENIX,
	ENEMY_FIRE_BIRD,
	ENEMY_WONDER_BIRD,
	ENEMY_BLUE_DRAGON,
	ENEMY_CRUEL_DRAGON,
	ENEMY_DRAGON,
	ENEMY_FLAME_DRAGON_AGATIO,
	ENEMY_FLAME_DRAGON_KARST,
	ENEMY_FIRE_DRAGON,
	ENEMY_MINOTAURUS,
	ENEMY_MINOS_WARRIOR,
	ENEMY_MINOS_KNIGHT,
	ENEMY_GRESSIL,
	ENEMY_LITTLE_DEATH,
	ENEMY_MINI_DEATH,
	ENEMY_RED_DEMON,
	ENEMY_LESSER_DEMON,
	ENEMY_MAD_DEMON,
	ENEMY_AKA_MANAH,
	ENEMY_DRUJ,
	ENEMY_AESHMA,
	ENEMY_VALUKAR,
	ENEMY_VALUKAR_UNUSED_A,
	ENEMY_VALUKAR_UNUSED_B,
	ENEMY_LIVING_ARMOR,
	ENEMY_PUPPET_WARRIOR,
	ENEMY_ESTRE_BARON,
	ENEMY_GHOST_ARMY,
	ENEMY_SOUL_ARMY,
	ENEMY_SPIRIT_ARMY,
	/* The Dullahan's three sets of abilities, one row each
	   (BattleAIDullahanForms): it starts its battle on one at random and moves
	   to the next each time it has gone through one (btAIChooseAction). */
	ENEMY_DULLAHAN,
	ENEMY_DULLAHAN_B,
	ENEMY_DULLAHAN_C,
	ENEMY_SENTINEL,
	ENEMY_SENTINEL_UNUSED_A,
	ENEMY_SENTINEL_UNUSED_B,
	ENEMY_VERMIN,
	/* The Broken Seal's ENEMY_VERMIN_WORLD_MAP, row for row the same stats; no
	   formation here lists it. */
	ENEMY_VERMIN_WORLD_MAP,
	ENEMY_MAD_VERMIN,
	ENEMY_BAT,
	/* The Broken Seal's ENEMY_BAT_WORLD_MAP, the same stats; no formation here
	   lists it. */
	ENEMY_BAT_WORLD_MAP,
	ENEMY_RABID_BAT,
	ENEMY_GIANT_BAT,
	ENEMY_WILD_MUSHROOM,
	/* The Broken Seal's ENEMY_WILD_MUSHROOM_WORLD_MAP, the same stats; no
	   formation here lists it. */
	ENEMY_WILD_MUSHROOM_WORLD_MAP,
	ENEMY_DEATH_CAP,
	ENEMY_SLIME_SOL_SANCTUM,
	ENEMY_SLIME_WORLD_MAP,
	ENEMY_OOZE,
	ENEMY_SLIME_BEAST,
	ENEMY_GHOST,
	ENEMY_GHOST_MAGE,
	ENEMY_HORNED_GHOST,
	/* Unused: The Broken Seal's ENEMY_LICH, the same stats, in no formation
	   here; ENEMY_LICH is this game's own. */
	ENEMY_LICH_UNUSED,
	ENEMY_ZOMBIE,
	ENEMY_UNDEAD,
	ENEMY_WIGHT,
	ENEMY_BANDIT,
	ENEMY_THIEF,
	ENEMY_BRIGAND,
	ENEMY_SKELETON,
	ENEMY_BONE_FIGHTER,
	ENEMY_SKULL_WARRIOR,
	ENEMY_WILL_HEAD,
	ENEMY_DEATH_HEAD,
	ENEMY_WILLOWISP,
	ENEMY_RAT,
	ENEMY_ARMORED_RAT,
	ENEMY_PLATED_RAT,
	ENEMY_RAT_SOLDIER,
	ENEMY_RAT_FIGHTER,
	ENEMY_RAT_WARRIOR,
	ENEMY_DRONE_BEE,
	ENEMY_FIGHTER_BEE,
	ENEMY_WARRIOR_BEE,
	ENEMY_TROLL,
	ENEMY_CAVE_TROLL,
	ENEMY_BRUTAL_TROLL,
	ENEMY_SPIDER,
	ENEMY_TARANTULA,
	ENEMY_RECLUSE,
	ENEMY_GNOME,
	ENEMY_GNOME_MAGE,
	ENEMY_GNOME_WIZARD,
	ENEMY_GHOUL,
	ENEMY_FIENDISH_GHOUL,
	ENEMY_CANNIBAL_GHOUL,
	ENEMY_MAULER,
	ENEMY_RAVAGER,
	ENEMY_GRISLY,
	ENEMY_HARPY,
	ENEMY_VIRAGO,
	ENEMY_HARRIDAN,
	ENEMY_SIREN,
	ENEMY_SUCCUBUS,
	ENEMY_NIGHTMARE,
	ENEMY_MOLE,
	ENEMY_MAD_MOLE,
	ENEMY_MOLE_MAGE,
	ENEMY_DIRGE,
	ENEMY_FOUL_DIRGE,
	ENEMY_VILE_DIRGE,
	ENEMY_APE,
	ENEMY_KILLER_APE,
	ENEMY_DIRTY_APE,
	ENEMY_GRUB,
	ENEMY_WORM,
	ENEMY_ACID_MAGGOT,
	ENEMY_ORC,
	ENEMY_ORC_CAPTAIN,
	ENEMY_ORC_LORD,
	ENEMY_SALAMANDER,
	ENEMY_EARTH_LIZARD,
	ENEMY_THUNDER_LIZARD,
	ENEMY_MANTICORE,
	ENEMY_MAGICORE,
	ENEMY_MANTICORE_KING,
	ENEMY_KOBOLD_UNUSED,
	ENEMY_GOBLIN,
	ENEMY_HOBGOBLIN,
	ENEMY_GARGOYLE,
	ENEMY_CLAY_GARGOYLE,
	ENEMY_ICE_GARGOYLE,
	ENEMY_GRYPHON,
	ENEMY_WILD_GRYPHON,
	ENEMY_WISE_GRYPHON,
	ENEMY_GOLEM,
	ENEMY_EARTH_GOLEM,
	ENEMY_GRAND_GOLEM,
	ENEMY_DREAD_HOUND,
	ENEMY_CERBERUS,
	ENEMY_FENRIR,
	ENEMY_STONE_SOLDIER,
	ENEMY_BOULDER_BEAST,
	ENEMY_RAGING_ROCK,
	ENEMY_CHIMERA,
	ENEMY_CHIMERA_MAGE,
	ENEMY_GRAND_CHIMERA,
	/* The Broken Seal's ENEMY_AMAZE_SOL_SANCTUM, the same stats; no formation
	   here lists it. */
	ENEMY_AMAZE_SOL_SANCTUM,
	/* The Amaze of the Kandorean Temple's encounters (zones 16 to 18). */
	ENEMY_AMAZE_KANDOREAN_TEMPLE,
	ENEMY_CREEPER,
	ENEMY_SPIRIT,
	ENEMY_LIZARD_MAN,
	ENEMY_LIZARD_FIGHTER,
	ENEMY_LIZARD_KING,
	ENEMY_ANT_LION,
	ENEMY_ROACH,
	ENEMY_DOODLE_BUG,
	ENEMY_TOADONPA,
	ENEMY_POISON_TOAD,
	ENEMY_DEVIL_FROG,
	ENEMY_LIVING_STATUE,
	ENEMY_HYDROS_STATUE,
	ENEMY_AZART_IRON_SHIELD,
	ENEMY_AZART_PADDED_GLOVES,
	ENEMY_SATRAGE_CHAIN_MAIL,
	ENEMY_SATRAGE_COTTON_SHIRT,
	ENEMY_NAVAMPA_CLAYMORE,
	ENEMY_NAVAMPA_HUNTERS_SWORD,
	ENEMY_TRET,
	ENEMY_KRAKEN,
	ENEMY_TORNADO_LIZARD,
	ENEMY_STORM_LIZARD,
	ENEMY_TEMPEST_LIZARD,
	ENEMY_MYSTERY_MAN,
	ENEMY_SATUROS_MERCURY_LIGHTHOUSE,
	ENEMY_SATUROS_VENUS_LIGHTHOUSE,
	ENEMY_MYSTERY_WOMAN,
	ENEMY_MENARDI,
	ENEMY_FUSION_DRAGON,
	ENEMY_DEADBEARD,
	ENEMY_MIMIC_KANDOREAN_TEMPLE,
	ENEMY_MIMIC_AIRS_ROCK,
	ENEMY_MIMIC_GABOMBA_STATUE,
	ENEMY_MIMIC_AQUA_ROCK,
	ENEMY_MIMIC_GAIA_ROCK,
	ENEMY_MIMIC_TREASURE_ISLE,
	ENEMY_MIMIC_JUPITER_LIGHTHOUSE,
	ENEMY_MIMIC_MAGMA_ROCK,
	ENEMY_MIMIC_MARS_LIGHTHOUSE,
	/* The Broken Seal's ENEMY_ZOMBIE_DJINN_LESSON, the same stats. */
	ENEMY_ZOMBIE_DJINN_LESSON,
	/* The Doom Dragon's three forms are rows A, E and H, and each acts again in
	   a turn as the rows after it (btAIChooseAction): A's second, third and
	   fourth actions are C, B and D, E's G and F, and H's second I.
	   btChangeEnemyForm brings on E, then H. */
	ENEMY_DOOM_DRAGON_A,
	ENEMY_DOOM_DRAGON_B,
	ENEMY_DOOM_DRAGON_C,
	ENEMY_DOOM_DRAGON_D,
	ENEMY_DOOM_DRAGON_E,
	ENEMY_DOOM_DRAGON_F,
	ENEMY_DOOM_DRAGON_G,
	ENEMY_DOOM_DRAGON_H,
	ENEMY_DOOM_DRAGON_I,
	ENEMY_REFRESH_BALL,
	ENEMY_THUNDER_BALL,
	ENEMY_ANGER_BALL,
	ENEMY_GUARDIAN_BALL,
	/* Fought with ENEMY_SATRAGE_SHAMAN_VILLAGE_CAVE and
	   ENEMY_NAVAMPA_SHAMAN_VILLAGE_CAVE in the Shaman Village Cave
	   (ENCOUNTER_ZONE_SCRIPTED, slot 2). */
	ENEMY_AZART_SHAMAN_VILLAGE_CAVE,
	ENEMY_SATRAGE_SHAMAN_VILLAGE_CAVE,
	ENEMY_NAVAMPA_SHAMAN_VILLAGE_CAVE,
	ENEMY_BANDIT_WORLD_MAP,
	ENEMY_THIEF_WORLD_MAP,
	ENEMY_NOTHING,
} ENEMY_ID;

#endif
