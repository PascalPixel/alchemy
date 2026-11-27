/* The element tables: class series by element, element power and resistance by
   level, the enemies' element presets, the power and resistance curve, and the
   dragons and sea creatures two weapons' effects look for. Part of game.c. */

/* The class series by a character's strongest and second element, read as
   [strongest][second]; The Broken Seal's Class_SeriesByElements. */
const Sint32 Class_SeriesByElements[4][4] = {
	{CLASS_SERIES_SQUIRE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_BRUTE,
		CLASS_SERIES_APPRENTICE},
	{CLASS_SERIES_SEER, CLASS_SERIES_WATER_SEER, CLASS_SERIES_PILGRIM,
		CLASS_SERIES_HERMIT},
	{CLASS_SERIES_BRUTE, CLASS_SERIES_SWORDSMAN, CLASS_SERIES_GUARD,
		CLASS_SERIES_APPRENTICE},
	{CLASS_SERIES_SEER, CLASS_SERIES_HERMIT, CLASS_SERIES_PILGRIM,
		CLASS_SERIES_WIND_SEER},
};

/* The elemental power and resistance of each element level, 0 to 15; The Broken
   Seal's Element_PowerResistByLevel. */
const ELEMENT_LEVEL_BONUS Element_PowerResistByLevel[16] = {
	{75, 85},
	{80, 90},
	{85, 95},
	{90, 100},
	{95, 105},
	{100, 110},
	{105, 115},
	{110, 120},
	{115, 125},
	{120, 130},
	{125, 135},
	{130, 140},
	{135, 145},
	{140, 150},
	{145, 160},
	{150, 165},
};

/* clang-format off */
/* The 48 enemy element presets, by ENEMY_DEFINITION.elementPreset; each is */
/* named by the enemies that have it, in the English release's words. */
const ENEMY_ELEMENT_PRESET Enemy_ElementPresetTable[] = {
	/* The default: ???, Ruffian, Ruffian 2, Ruffian 3, Briggs, Sea Fighter, */
	/* Champa 2, Champa 3, Moapa, Knight, Knight 3 and the enemy with no name */
	/* message */
	[ELEMENT_PRESET_DEFAULT] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1, 1, 1, 1},
		.stats = {
			{.power = 100, .resistance = 100},
			{.power = 100, .resistance = 100},
			{.power = 100, .resistance = 100},
			{.power = 100, .resistance = 100},
		},
	},
	/* Bandit, Thief and Brigand */
	[ELEMENT_PRESET_BANDIT] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1, 1, 1, 1},
		.stats = {
			{.power = 80, .resistance = 72},
			{.power = 80, .resistance = 72},
			{.power = 80, .resistance = 72},
			{.power = 80, .resistance = 72},
		},
	},
	/* Azart, Satrage and Navampa */
	[ELEMENT_PRESET_AZART] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1, 1, 1, 1},
		.stats = {
			{.power = 80, .resistance = 90},
			{.power = 80, .resistance = 90},
			{.power = 80, .resistance = 90},
			{.power = 80, .resistance = 90},
		},
	},
	/* Chestbeater, Wild Gorilla, Crazy Gorilla, Wild Wolf, Dire Wolf, White */
	/* Wolf, Vermin, Mad Vermin, Rat, Armored Rat, Plated Rat, Troll, Cave */
	/* Troll, Brutal Troll, Mauler, Ravager, Mole, Mad Mole, Mole Mage and Ape */
	[ELEMENT_PRESET_CHESTBEATER] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1},
		.stats = {
			{.power = 100, .resistance = 48},
			{.power = 80, .resistance = 48},
			{.power = 70, .resistance = 25},
			{.power = 80, .resistance = 72},
		},
	},
	/* Grisly, Killer Ape and Dirty Ape */
	[ELEMENT_PRESET_GRISLY] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1},
		.stats = {
			{.power = 100, .resistance = 72},
			{.power = 80, .resistance = 72},
			{.power = 70, .resistance = 25},
			{.power = 80, .resistance = 127},
		},
	},
	/* Punch Ant, Flash Ant, Numb Ant, Angle Worm, Fire Worm, Chimera Worm, */
	/* Spider, Tarantula, Recluse, Grub, Worm and Acid Maggot */
	[ELEMENT_PRESET_PUNCH_ANT] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1},
		.stats = {
			{.power = 95, .resistance = 48},
			{.power = 85, .resistance = 48},
			{.power = 90, .resistance = 25},
			{.power = 85, .resistance = 48},
		},
	},
	/* King Scorpion, Devil Scorpion, Sand Scorpion, Urchin Beast, Needle Egg, */
	/* Sea Hedgehog, Conch Shell, Spiral Shell and Poison Shell */
	[ELEMENT_PRESET_KING_SCORPION] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1},
		.stats = {
			{.power = 95, .resistance = 72},
			{.power = 85, .resistance = 72},
			{.power = 90, .resistance = 48},
			{.power = 85, .resistance = 100},
		},
	},
	/* Mad Plant, Wild Mushroom and Death Cap */
	[ELEMENT_PRESET_MAD_PLANT] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 1},
		.stats = {
			{.power = 105, .resistance = 48},
			{.power = 100, .resistance = 100},
			{.power = 80, .resistance = 7},
			{.power = 70, .resistance = 72},
		},
	},
	/* Wood Walker, Elder Wood, Estre Wood and Tret */
	[ELEMENT_PRESET_WOOD_WALKER] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 1},
		.stats = {
			{.power = 105, .resistance = 72},
			{.power = 100, .resistance = 175},
			{.power = 80, .resistance = 25},
			{.power = 70, .resistance = 100},
		},
	},
	/* Drone Bee, Fighter Bee and Warrior Bee */
	[ELEMENT_PRESET_DRONE_BEE] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1, 0, 0, 2},
		.stats = {
			{.power = 90, .resistance = 100},
			{.power = 90, .resistance = 48},
			{.power = 95, .resistance = 48},
			{.power = 105, .resistance = 7},
		},
	},
	/* Unused: no enemy has it. */
	{
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1, 0, 0, 2},
		.stats = {
			{.power = 90, .resistance = 152},
			{.power = 90, .resistance = 72},
			{.power = 95, .resistance = 72},
			{.power = 105, .resistance = 25},
		},
	},
	/* Momonga, Squirrelfang, Momangler, Emu, Talon Runner, Winged Runner, */
	/* Wyvern Chick, Pteranodon, Winged Lizard, Seabird, Seafowl, Great Seagull, */
	/* Bat, Rabid Bat, Giant Bat, Harpy, Virago, Dirge, Foul Dirge and Vile */
	/* Dirge */
	[ELEMENT_PRESET_MOMONGA] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1, 1, 1, 2},
		.stats = {
			{.power = 95, .resistance = 127},
			{.power = 95, .resistance = 72},
			{.power = 95, .resistance = 48},
			{.power = 110, .resistance = 7},
		},
	},
	/* Pixie, Faery, Weird Nypmh, Roc, Raptor, Fell Raptor, Wyvern, Sky Dragon, */
	/* Winged Dragon and Harridan */
	[ELEMENT_PRESET_PIXIE] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {1, 1, 1, 2},
		.stats = {
			{.power = 95, .resistance = 175},
			{.power = 95, .resistance = 100},
			{.power = 95, .resistance = 72},
			{.power = 110, .resistance = 25},
		},
	},
	/* Mummy, Foul Mummy, Grave Wight, Zombie, Undead, Wight, Ghoul, Fiendish */
	/* Ghoul and Cannibal Ghoul */
	[ELEMENT_PRESET_MUMMY] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 1},
		.stats = {
			{.power = 100, .resistance = 25},
			{.power = 95, .resistance = 72},
			{.power = 80, .resistance = 7},
			{.power = 80, .resistance = 48},
		},
	},
	/* Skeleton, Bone Fighter, Skull Warrior, Will Head, Death Head and */
	/* Willowisp */
	[ELEMENT_PRESET_SKELETON] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 1},
		.stats = {
			{.power = 100, .resistance = 25},
			{.power = 95, .resistance = 127},
			{.power = 80, .resistance = 72},
			{.power = 80, .resistance = 48},
		},
	},
	/* Ghost, Ghost Mage, Horned Ghost, Amaze, Creeper and Spirit */
	[ELEMENT_PRESET_GHOST] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {0, 1, 1, 2},
		.stats = {
			{.power = 100, .resistance = 48},
			{.power = 95, .resistance = 100},
			{.power = 95, .resistance = 72},
			{.power = 95, .resistance = 7},
		},
	},
	/* Living Armor, Puppet Warrior, Estre Baron, Ghost Army, Soul Army, Spirit */
	/* Army and Lich */
	[ELEMENT_PRESET_LIVING_ARMOR] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {0, 1, 1, 2},
		.stats = {
			{.power = 100, .resistance = 72},
			{.power = 95, .resistance = 152},
			{.power = 95, .resistance = 100},
			{.power = 95, .resistance = 25},
		},
	},
	/* Gargoyle, Clay Gargoyle and Ice Gargoyle */
	[ELEMENT_PRESET_GARGOYLE] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 1, 1, 1},
		.stats = {
			{.power = 100, .resistance = 48},
			{.power = 100, .resistance = 100},
			{.power = 100, .resistance = 100},
			{.power = 95, .resistance = 127},
		},
	},
	/* Serpent, Serpent 6, Serpent 7, Dino, Dinox, Dinosaurus, Minotaurus, Minos */
	/* Warrior, Minos Knight and Guardian Ball */
	[ELEMENT_PRESET_SERPENT] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {4, 2, 2},
		.stats = {
			{.power = 110, .resistance = 152},
			{.power = 95, .resistance = 100},
			{.power = 95, .resistance = 100},
			{.power = 90, .resistance = 48},
		},
	},
	/* Aqua Hydra, Hydra, Pyrodra, Poseidon, Poseidon 3, Sea Dragon, Turtle */
	/* Dragon, Ocean Dragon, Blue Dragon, Cruel Dragon, Dragon, Cerberus, */
	/* Fenrir, Toadonpa, Poison Toad, Devil Frog, Kraken and Refresh Ball */
	[ELEMENT_PRESET_AQUA_HYDRA] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 4, 0, 2},
		.stats = {
			{.power = 95, .resistance = 100},
			{.power = 115, .resistance = 175},
			{.power = 85, .resistance = 25},
			{.power = 95, .resistance = 100},
		},
	},
	/* Avimander, Macetail, Bombander, Flame Dragon, Fire Dragon, Manticore, */
	/* Magicore, Manticore King, Dread Hound, Chimera, Chimera Mage, Grand */
	/* Chimera, Ant Lion, Roach, Doodle Bug and Anger Ball */
	[ELEMENT_PRESET_AVIMANDER] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 0, 4, 2},
		.stats = {
			{.power = 95, .resistance = 100},
			{.power = 85, .resistance = 25},
			{.power = 115, .resistance = 175},
			{.power = 95, .resistance = 100},
		},
	},
	/* Tornado Lizard, Storm Lizard, Tempest Lizard and Thunder Ball */
	[ELEMENT_PRESET_TORNADO_LIZARD] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {0, 2, 2, 4},
		.stats = {
			{.power = 90, .resistance = 48},
			{.power = 95, .resistance = 100},
			{.power = 95, .resistance = 100},
			{.power = 110, .resistance = 152},
		},
	},
	/* Unused: no enemy has it. */
	{
		.elementLevels = {6, 2, 2},
		.stats = {
			{.power = 120, .resistance = 100},
			{.power = 95, .resistance = 48},
			{.power = 95, .resistance = 48},
			{.power = 80, .resistance = 7},
		},
	},
	/* Venus Djinni, Earth Lizard, Golem, Earth Golem and Grand Golem */
	[ELEMENT_PRESET_VENUS_DJINNI] = {
		.elementLevels = {6, 2, 2},
		.stats = {
			{.power = 120, .resistance = 193},
			{.power = 95, .resistance = 100},
			{.power = 95, .resistance = 100},
			{.power = 80, .resistance = 25},
		},
	},
	/* Cuttle, Calamar, Man o' War, Aqua Jelly, Slime, Ooze and Slime Beast */
	[ELEMENT_PRESET_CUTTLE] = {
		.attackElement = ELEMENT_MERCURY,
		.elementLevels = {2, 6, 0, 2},
		.stats = {
			{.power = 95, .resistance = 48},
			{.power = 125, .resistance = 127},
			{.power = 75, .resistance = 7},
			{.power = 95, .resistance = 48},
		},
	},
	/* Mercury Djinni, Living Statue and Hydros Statue */
	[ELEMENT_PRESET_MERCURY_DJINNI] = {
		.attackElement = ELEMENT_MERCURY,
		.elementLevels = {2, 6, 0, 2},
		.stats = {
			{.power = 95, .resistance = 100},
			{.power = 125, .resistance = 193},
			{.power = 75, .resistance = 25},
			{.power = 95, .resistance = 100},
		},
	},
	/* Stone Soldier */
	[ELEMENT_PRESET_STONE_SOLDIER] = {
		.attackElement = ELEMENT_MARS,
		.elementLevels = {2, 0, 6, 2},
		.stats = {
			{.power = 95, .resistance = 48},
			{.power = 75, .resistance = 7},
			{.power = 125, .resistance = 127},
			{.power = 95, .resistance = 48},
		},
	},
	/* Mars Djinni, Phoenix, Fire Bird, Wonder Bird, Salamander, Boulder Beast */
	/* and Raging Rock */
	[ELEMENT_PRESET_MARS_DJINNI] = {
		.attackElement = ELEMENT_MARS,
		.elementLevels = {2, 0, 6, 2},
		.stats = {
			{.power = 95, .resistance = 100},
			{.power = 75, .resistance = 25},
			{.power = 125, .resistance = 193},
			{.power = 95, .resistance = 100},
		},
	},
	/* Unused: no enemy has it. */
	{
		.attackElement = ELEMENT_JUPITER,
		.elementLevels = {0, 2, 2, 6},
		.stats = {
			{.power = 80, .resistance = 7},
			{.power = 95, .resistance = 48},
			{.power = 95, .resistance = 48},
			{.power = 120, .resistance = 127},
		},
	},
	/* Jupiter Djinni and Thunder Lizard */
	[ELEMENT_PRESET_JUPITER_DJINNI] = {
		.attackElement = ELEMENT_JUPITER,
		.elementLevels = {0, 2, 2, 6},
		.stats = {
			{.power = 80, .resistance = 25},
			{.power = 95, .resistance = 100},
			{.power = 95, .resistance = 100},
			{.power = 120, .resistance = 193},
		},
	},
	/* Mini-Goblin, Alec Goblin, Baboon Goblin, Kobold, Wargold, Kobold King, */
	/* Gnome Wizard, Orc, Orc Captain and Goblin */
	[ELEMENT_PRESET_MINI_GOBLIN] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {3, 2, 2, 2},
		.stats = {
			{.power = 100, .resistance = 100},
			{.power = 85, .resistance = 72},
			{.power = 85, .resistance = 72},
			{.power = 85, .resistance = 48},
		},
	},
	/* Assassin, Slayer, Dark Murder, Doomsayer, Lich, Bane Wight, Gressil, */
	/* Little Death, Mini-Death, Red Demon, Lesser Demon, Mad Demon, Orc Lord */
	/* and Hobgoblin */
	[ELEMENT_PRESET_ASSASSIN] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {3, 2, 2, 2},
		.stats = {
			{.power = 100, .resistance = 127},
			{.power = 85, .resistance = 100},
			{.power = 85, .resistance = 100},
			{.power = 85, .resistance = 72},
		},
	},
	/* Wolfkin Cub, Wolfkin, Skinwalker, Rat Soldier, Rat Fighter and Rat */
	/* Warrior */
	[ELEMENT_PRESET_WOLFKIN_CUB] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 3, 2, 2},
		.stats = {
			{.power = 85, .resistance = 72},
			{.power = 100, .resistance = 100},
			{.power = 85, .resistance = 48},
			{.power = 85, .resistance = 72},
		},
	},
	/* Merman, Gillman, Gillman Lord, Aka Manah, Druj, Aeshma, Siren, Succubus, */
	/* Lizard Man, Lizard Fighter and Lizard King */
	[ELEMENT_PRESET_MERMAN] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 3, 2, 2},
		.stats = {
			{.power = 85, .resistance = 100},
			{.power = 100, .resistance = 127},
			{.power = 85, .resistance = 72},
			{.power = 85, .resistance = 100},
		},
	},
	/* Gnome */
	[ELEMENT_PRESET_GNOME] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 2, 3, 2},
		.stats = {
			{.power = 85, .resistance = 72},
			{.power = 85, .resistance = 48},
			{.power = 100, .resistance = 100},
			{.power = 85, .resistance = 72},
		},
	},
	/* Unused: no enemy has it. */
	{
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 2, 3, 2},
		.stats = {
			{.power = 85, .resistance = 100},
			{.power = 85, .resistance = 72},
			{.power = 100, .resistance = 127},
			{.power = 85, .resistance = 100},
		},
	},
	/* Gnome Mage */
	[ELEMENT_PRESET_GNOME_MAGE] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 2, 2, 3},
		.stats = {
			{.power = 85, .resistance = 48},
			{.power = 85, .resistance = 72},
			{.power = 85, .resistance = 72},
			{.power = 100, .resistance = 100},
		},
	},
	/* Nightmare */
	[ELEMENT_PRESET_NIGHTMARE] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {2, 2, 2, 3},
		.stats = {
			{.power = 85, .resistance = 72},
			{.power = 85, .resistance = 100},
			{.power = 85, .resistance = 100},
			{.power = 100, .resistance = 127},
		},
	},
	/* Mimic */
	[ELEMENT_PRESET_MIMIC] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {3, 3, 0, 4},
		.stats = {
			{.power = 95, .resistance = 100},
			{.power = 100, .resistance = 127},
			{.power = 75, .resistance = 25},
			{.power = 105, .resistance = 152},
		},
	},
	/* Agatio, Mystery Man and Saturos */
	[ELEMENT_PRESET_AGATIO] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {7, 5, 10, 7},
		.stats = {
			{.power = 100, .resistance = 100},
			{.power = 90, .resistance = 72},
			{.power = 110, .resistance = 175},
			{.power = 100, .resistance = 127},
		},
	},
	/* Karst, Mystery Woman and Menardi */
	[ELEMENT_PRESET_KARST] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {8, 6, 11, 8},
		.stats = {
			{.power = 100, .resistance = 127},
			{.power = 95, .resistance = 72},
			{.power = 105, .resistance = 175},
			{.power = 100, .resistance = 100},
		},
	},
	/* Fusion Dragon */
	[ELEMENT_PRESET_FUSION_DRAGON] = {
		.attackElement = ELEMENT_MARS,
		.elementLevels = {9, 7, 14, 9},
		.stats = {
			{.power = 100, .resistance = 127},
			{.power = 95, .resistance = 72},
			{.power = 110, .resistance = 175},
			{.power = 100, .resistance = 127},
		},
	},
	/* Sentinel and Deadbeard */
	[ELEMENT_PRESET_SENTINEL] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {4, 8, 6, 5},
		.stats = {
			{.power = 100, .resistance = 72},
			{.power = 130, .resistance = 193},
			{.power = 120, .resistance = 127},
			{.power = 110, .resistance = 152},
		},
	},
	/* Gryphon, Wild Gryphon and Wise Gryphon */
	[ELEMENT_PRESET_GRYPHON] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {0, 2, 1, 4},
		.stats = {
			{.power = 90, .resistance = 152},
			{.power = 95, .resistance = 110},
			{.power = 95, .resistance = 72},
			{.power = 110, .resistance = 48},
		},
	},
	/* Star Magician, Dark Wizard and Evil Shaman */
	[ELEMENT_PRESET_STAR_MAGICIAN] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {14, 16, 13, 15},
		.stats = {
			{.power = 90, .resistance = 130},
			{.power = 105, .resistance = 180},
			{.power = 85, .resistance = 72},
			{.power = 95, .resistance = 150},
		},
	},
	/* Valukar */
	[ELEMENT_PRESET_VALUKAR] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {11, 7, 14, 9},
		.stats = {
			{.power = 95, .resistance = 100},
			{.power = 85, .resistance = 60},
			{.power = 105, .resistance = 175},
			{.power = 95, .resistance = 100},
		},
	},
	/* Dullahan */
	[ELEMENT_PRESET_DULLAHAN] = {
		.attackElement = ELEMENT_NONE,
		.elementLevels = {17, 14, 12, 10},
		.stats = {
			{.power = 110, .resistance = 150},
			{.power = 100, .resistance = 190},
			{.power = 100, .resistance = 130},
			{.power = 100, .resistance = 110},
		},
	},
	/* Doom Dragon */
	[ELEMENT_PRESET_DOOM_DRAGON] = {
		.elementLevels = {18, 11, 15, 8},
		.stats = {
			{.power = 110, .resistance = 190},
			{.power = 100, .resistance = 130},
			{.power = 105, .resistance = 175},
			{.power = 95, .resistance = 98},
		},
	},
};
/* clang-format on */

/* The element power and resistance curve, the same in both games: pairs of an
   input and its value in 256ths, from 200 down to -200, between which
   gmBattleGetElementMultiplier interpolates. */
const Sint16 Element_PowerResistCurve[5][2] = {
	{200, 256},
	{100, 128},
	{0, 0},
	{-100, -128},
	{-200, -256},
};

/* The dragons, by enemy id: the Hydras, Serpents, Dinos, Dragons, Lizards,
   Chimeras and the Doom Dragon. Raiden's Wrath (the Cloud Brand's unleash)
   takes effect only on these (gmEnemyIsDragon). */
const Uint16 Enemy_DragonIds[DRAGON_ENEMY_COUNT] = {
	ENEMY_AQUA_HYDRA,
	ENEMY_HYDRA,
	ENEMY_PYRODRA,
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
	ENEMY_WYVERN_CHICK,
	ENEMY_PTERANODON,
	ENEMY_WINGED_LIZARD,
	ENEMY_DINO,
	ENEMY_DINOX,
	ENEMY_DINOSAURUS,
	ENEMY_SEA_DRAGON,
	ENEMY_TURTLE_DRAGON,
	ENEMY_OCEAN_DRAGON,
	ENEMY_WYVERN,
	ENEMY_SKY_DRAGON,
	ENEMY_WINGED_DRAGON,
	ENEMY_BLUE_DRAGON,
	ENEMY_CRUEL_DRAGON,
	ENEMY_DRAGON,
	ENEMY_FLAME_DRAGON_AGATIO,
	ENEMY_FLAME_DRAGON_KARST,
	ENEMY_FIRE_DRAGON,
	ENEMY_SALAMANDER,
	ENEMY_EARTH_LIZARD,
	ENEMY_THUNDER_LIZARD,
	ENEMY_CHIMERA,
	ENEMY_CHIMERA_MAGE,
	ENEMY_GRAND_CHIMERA,
	ENEMY_FUSION_DRAGON,
	ENEMY_DOOM_DRAGON_A,
	ENEMY_DOOM_DRAGON_B,
	ENEMY_DOOM_DRAGON_C,
	ENEMY_DOOM_DRAGON_D,
	ENEMY_DOOM_DRAGON_E,
	ENEMY_DOOM_DRAGON_F,
	ENEMY_DOOM_DRAGON_G,
	ENEMY_DOOM_DRAGON_H,
	ENEMY_DOOM_DRAGON_I,
};

/* The sea creatures, by enemy id: the Cuttles, Jellies, Hydras, Poseidon, the
   shells and urchins, the Mermen, the sea Dragons, the Seabirds, the Lizard Men
   and the Kraken. The Trident's use takes effect only on these
   (gmEnemyIsSeaCreature). */
const Uint16 Enemy_SeaCreatureIds[SEA_CREATURE_ENEMY_COUNT] = {
	ENEMY_CUTTLE,
	ENEMY_CALAMAR,
	ENEMY_MAN_O_WAR,
	ENEMY_AQUA_JELLY,
	ENEMY_AQUA_HYDRA,
	ENEMY_HYDRA,
	ENEMY_PYRODRA,
	ENEMY_POSEIDON,
	ENEMY_POSEIDON_SECOND_ACTION,
	ENEMY_POSEIDON_3,
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
	ENEMY_LIZARD_MAN,
	ENEMY_LIZARD_FIGHTER,
	ENEMY_LIZARD_KING,
	ENEMY_KRAKEN,
};
