/* The debug parameter sets: runs of party members, each run ended by
   DEBUG_PARAMETER_SET_END, that btDebugParmSetLoad gives the party. */

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/debug_parameter_set.h"
#include "game/character_ids.h"

/* clang-format off */
DEBUG_PARM_SET_ROW Debug_ParameterSets[] = {
	/* Set 0 */
	{
		.unitId = ISAAC,
		.level = 35,
		.djinnCnts = {7},
		.items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 35,
		.djinnCnts = {0, 0, 7},
		.items = {ITEM_BURNING_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 35,
		.djinnCnts = {0, 0, 0, 7},
		.items = {ITEM_POWER_BREAD, ITEM_SACRED_FEATHER, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 35,
		.djinnCnts = {0, 7},
		.items = {ITEM_FROST_WAND, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 1 */
	{
		.unitId = ISAAC,
		.level = 3,
		.djinnCnts = {1},
		.items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 3,
		.items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 2 */
	{
		.unitId = ISAAC,
		.level = 6,
		.djinnCnts = {1},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 6,
		.djinnCnts = {0, 0, 0, 1},
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 6,
		.djinnCnts = {0, 1},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_ARMLET, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 3 */
	{
		.unitId = ISAAC,
		.level = 11,
		.djinnCnts = {2},
		.items = {ITEM_BROAD_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 11,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_BROAD_AXE, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 11,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 11,
		.djinnCnts = {0, 2},
		.items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 4 */
	{
		.unitId = ISAAC,
		.level = 15,
		.djinnCnts = {3},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 15,
		.djinnCnts = {0, 0, 3},
		.items = {ITEM_GREAT_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 15,
		.djinnCnts = {0, 4},
		.items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = IVAN,
		.level = 15,
		.djinnCnts = {0, 0, 0, 4},
		.items = {ITEM_PSYNERGY_ROD, ITEM_JERKIN, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 5 */
	{
		.unitId = ISAAC,
		.level = 18,
		.djinnCnts = {4},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 18,
		.djinnCnts = {0, 0, 4},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 18,
		.djinnCnts = {0, 5},
		.items = {ITEM_ANGELIC_ANKH, ITEM_COCKTAIL_DRESS, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = IVAN,
		.level = 18,
		.djinnCnts = {0, 0, 0, 5},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 6 */
	{
		.unitId = ISAAC,
		.level = 20,
		.djinnCnts = {6},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 20,
		.djinnCnts = {0, 0, 5},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 20,
		.djinnCnts = {0, 6},
		.items = {ITEM_ANGELIC_ANKH, ITEM_COCKTAIL_DRESS, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = IVAN,
		.level = 20,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 7 */
	{
		.unitId = ISAAC,
		.level = 35,
		.djinnCnts = {0, 0, 3, 4},
		.items = {ITEM_NINJA_BLADE, ITEM_NINJA_GARB, ITEM_SPIRIT_ARMLET, ITEM_NINJA_HOOD},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 35,
		.djinnCnts = {7},
		.items = {ITEM_GIANT_AXE, ITEM_DRAGON_SCALES, ITEM_MIRRORED_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 35,
		.djinnCnts = {0, 0, 4, 3},
		.items = {ITEM_ANGELIC_ANKH, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_GLOVES, ITEM_JEWELED_CROWN},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 35,
		.djinnCnts = {0, 7},
		.items = {ITEM_ANGELIC_ANKH, ITEM_ORACLES_ROBE, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 8 */
	{
		.unitId = ISAAC,
		.level = 35,
		.djinnCnts = {0, 0, 4, 3},
		.items = {ITEM_SHAMSHIR, ITEM_ASURAS_ARMOR, ITEM_SPIRIT_ARMLET, ITEM_WARRIORS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 35,
		.djinnCnts = {5, 0, 2},
		.items = {ITEM_GIANT_AXE, ITEM_DRAGON_SCALES, ITEM_MIRRORED_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 35,
		.djinnCnts = {0, 5, 0, 2},
		.items = {ITEM_ANGELIC_ANKH, ITEM_MAGICAL_CASSOCK, ITEM_SPIRIT_GLOVES, ITEM_JEWELED_CROWN},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 35,
		.djinnCnts = {2, 2, 1, 2},
		.items = {ITEM_ANGELIC_ANKH, ITEM_ORACLES_ROBE, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 9 */
	{
		.unitId = ISAAC,
		.level = 35,
		.djinnCnts = {0, 4, 3},
		.items = {ITEM_ARCTIC_BLADE, ITEM_ARMORED_SHELL, ITEM_EARTH_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 35,
		.djinnCnts = {4, 0, 3},
		.items = {ITEM_BURNING_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 35,
		.djinnCnts = {3, 0, 0, 4},
		.items = {ITEM_ASSASSIN_BLADE, ITEM_MAGICAL_CASSOCK, ITEM_GUARDIAN_ARMLET, ITEM_JEWELED_CROWN},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 35,
		.djinnCnts = {0, 3, 1, 3},
		.items = {ITEM_FROST_WAND, ITEM_COCKTAIL_DRESS, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 10 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_MACHETE, ITEM_COTTON_SHIRT, ITEM_PADDED_GLOVES},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_MACHETE, ITEM_COTTON_SHIRT, ITEM_PADDED_GLOVES},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = JENNA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_ONE_PIECE_DRESS, ITEM_PADDED_GLOVES},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 11 */
	{
		.unitId = ISAAC,
		.level = 2,
		.items = {ITEM_SHORT_SWORD, ITEM_COTTON_SHIRT, ITEM_PADDED_GLOVES},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 2,
		.items = {ITEM_SHORT_SWORD, ITEM_COTTON_SHIRT, ITEM_PADDED_GLOVES},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = JENNA,
		.level = 2,
		.items = {ITEM_WOODEN_STICK, ITEM_ONE_PIECE_DRESS, ITEM_PADDED_GLOVES},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 12 */
	{
		.unitId = ISAAC,
		.level = 3,
		.items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 3,
		.items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 13 */
	{
		.unitId = ISAAC,
		.level = 4,
		.items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 4,
		.items = {ITEM_BATTLE_AXE, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 4,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 14 */
	{
		.unitId = ISAAC,
		.level = 5,
		.djinnCnts = {1},
		.items = {ITEM_LONG_SWORD, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 5,
		.items = {ITEM_BATTLE_AXE, ITEM_TRAVEL_VEST, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 5,
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_VEST, ITEM_LEATHER_ARMLET, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 15 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 16 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 17 */
	{
		.unitId = GARET,
		.level = 30,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = ISAAC,
		.level = 30,
		.djinnCnts = {2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE, ABILITY_RETREAT},
	},
	{
		.unitId = MIA,
		.level = 30,
		.djinnCnts = {0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = IVAN,
		.level = 30,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 18 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 19 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 20 */
	{
		.unitId = ISAAC,
		.level = 6,
		.djinnCnts = {1},
		.items = {ITEM_BANDITS_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 6,
		.djinnCnts = {0, 0, 1},
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 6,
		.djinnCnts = {0, 0, 0, 1},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_ARMLET, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 21 */
	{
		.unitId = ISAAC,
		.level = 7,
		.djinnCnts = {1},
		.items = {ITEM_BANDITS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 7,
		.djinnCnts = {0, 0, 1},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 7,
		.djinnCnts = {0, 0, 0, 1},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_ARMLET, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 22 */
	{
		.unitId = ISAAC,
		.level = 8,
		.djinnCnts = {2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 8,
		.djinnCnts = {0, 0, 1},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 8,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 23 */
	{
		.unitId = ISAAC,
		.level = 9,
		.djinnCnts = {2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 9,
		.djinnCnts = {0, 0, 1},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 9,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 24 */
	{
		.unitId = ISAAC,
		.level = 9,
		.djinnCnts = {2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 9,
		.djinnCnts = {0, 0, 1},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_LEATHER_GLOVES, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 9,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 25 */
	{
		.unitId = ISAAC,
		.level = 10,
		.djinnCnts = {2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 10,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 10,
		.djinnCnts = {0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 10,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 26 */
	{
		.unitId = ISAAC,
		.level = 10,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 10,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 10,
		.djinnCnts = {2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 10,
		.djinnCnts = {0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 27 */
	{
		.unitId = ISAAC,
		.level = 10,
		.djinnCnts = {0, 2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 10,
		.djinnCnts = {2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 10,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 10,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 28 */
	{
		.unitId = ISAAC,
		.level = 10,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 10,
		.djinnCnts = {0, 2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 10,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 10,
		.djinnCnts = {2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 29 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 30 */
	{
		.unitId = ISAAC,
		.level = 11,
		.djinnCnts = {2},
		.items = {ITEM_BROAD_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 11,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_BROAD_AXE, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 11,
		.djinnCnts = {0, 2},
		.items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 11,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 31 */
	{
		.unitId = ISAAC,
		.level = 11,
		.djinnCnts = {2},
		.items = {ITEM_BROAD_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 11,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_BROAD_AXE, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 11,
		.djinnCnts = {0, 2},
		.items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 11,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 32 */
	{
		.unitId = ISAAC,
		.level = 12,
		.djinnCnts = {2},
		.items = {ITEM_BROAD_SWORD, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 12,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_BROAD_AXE, ITEM_ADEPTS_CLOTHES, ITEM_BRONZE_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 12,
		.djinnCnts = {0, 2},
		.items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 12,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_BLESSED_ANKH, ITEM_ADEPTS_CLOTHES, ITEM_ARMLET, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 33 */
	{
		.unitId = ISAAC,
		.level = 13,
		.djinnCnts = {3},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 13,
		.djinnCnts = {0, 0, 3},
		.items = {ITEM_BROAD_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_BRONZE_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 13,
		.djinnCnts = {0, 3},
		.items = {ITEM_BATTLE_RAPIER, ITEM_CHINA_DRESS, ITEM_ARMLET, ITEM_SILVER_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 13,
		.djinnCnts = {0, 0, 0, 3},
		.items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_ARMLET, ITEM_SILVER_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 34 */
	{
		.unitId = ISAAC,
		.level = 14,
		.djinnCnts = {3},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 14,
		.djinnCnts = {0, 0, 3},
		.items = {ITEM_BROAD_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 14,
		.djinnCnts = {0, 3},
		.items = {ITEM_BATTLE_RAPIER, ITEM_CHINA_DRESS, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 14,
		.djinnCnts = {0, 0, 0, 3},
		.items = {ITEM_PSYNERGY_ROD, ITEM_SILK_ROBE, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 35 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 36 */
	{
		.unitId = ISAAC,
		.level = 15,
		.djinnCnts = {2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 15,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 15,
		.djinnCnts = {0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 15,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 37 */
	{
		.unitId = ISAAC,
		.level = 15,
		.djinnCnts = {0, 2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 15,
		.djinnCnts = {2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 15,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_RIGHTEOUS_MACE, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 15,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 38 */
	{
		.unitId = ISAAC,
		.level = 15,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 15,
		.djinnCnts = {0, 2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 15,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 15,
		.djinnCnts = {2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 39 */
	{
		.unitId = ISAAC,
		.level = 15,
		.djinnCnts = {0, 0, 2},
		.items = {ITEM_BROAD_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 15,
		.djinnCnts = {0, 0, 0, 2},
		.items = {ITEM_HUNTERS_SWORD, ITEM_LEATHER_ARMOR, ITEM_BRONZE_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 15,
		.djinnCnts = {2},
		.items = {ITEM_WITCHS_WAND, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
	},
	{
		.unitId = IVAN,
		.level = 15,
		.djinnCnts = {0, 2},
		.items = {ITEM_MAGIC_ROD, ITEM_TRAVEL_ROBE, ITEM_LEATHER_GLOVES, ITEM_WOODEN_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 40 */
	{
		.unitId = ISAAC,
		.level = 15,
		.djinnCnts = {4},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 15,
		.djinnCnts = {0, 0, 4},
		.items = {ITEM_GREAT_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 15,
		.djinnCnts = {0, 4},
		.items = {ITEM_FROST_WAND, ITEM_ADEPTS_CLOTHES, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 15,
		.djinnCnts = {0, 0, 0, 4},
		.items = {ITEM_PSYNERGY_ROD, ITEM_ADEPTS_CLOTHES, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 41 */
	{
		.unitId = ISAAC,
		.level = 15,
		.djinnCnts = {4},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 15,
		.djinnCnts = {0, 0, 4},
		.items = {ITEM_GREAT_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 15,
		.djinnCnts = {0, 4},
		.items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 15,
		.djinnCnts = {0, 0, 0, 4},
		.items = {ITEM_PSYNERGY_ROD, ITEM_JERKIN, ITEM_GAUNTLETS, ITEM_SILVER_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 42 */
	{
		.unitId = ISAAC,
		.level = 16,
		.djinnCnts = {4},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 16,
		.djinnCnts = {0, 0, 4},
		.items = {ITEM_GREAT_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_IRON_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 16,
		.djinnCnts = {0, 4},
		.items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_SILVER_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 16,
		.djinnCnts = {0, 0, 0, 5},
		.items = {ITEM_PSYNERGY_ROD, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_SILVER_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 43 */
	{
		.unitId = ISAAC,
		.level = 17,
		.djinnCnts = {5},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 17,
		.djinnCnts = {0, 0, 4},
		.items = {ITEM_GREAT_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 17,
		.djinnCnts = {0, 4},
		.items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP},
	},
	{
		.unitId = IVAN,
		.level = 17,
		.djinnCnts = {0, 0, 0, 5},
		.items = {ITEM_PSYNERGY_ROD, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 44 */
	{
		.unitId = ISAAC,
		.level = 17,
		.djinnCnts = {5},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 17,
		.djinnCnts = {0, 0, 4},
		.items = {ITEM_GREAT_AXE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 17,
		.djinnCnts = {0, 4},
		.items = {ITEM_FROST_WAND, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP},
	},
	{
		.unitId = IVAN,
		.level = 17,
		.djinnCnts = {0, 0, 0, 5},
		.items = {ITEM_PSYNERGY_ROD, ITEM_JERKIN, ITEM_HEAVY_ARMLET, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 45 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 46 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 47 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 48 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 49 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 50 */
	{
		.unitId = ISAAC,
		.level = 17,
		.djinnCnts = {5},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 17,
		.djinnCnts = {0, 0, 5},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_MAIL_CAP},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 17,
		.djinnCnts = {0, 5},
		.items = {ITEM_ANGELIC_ANKH, ITEM_COCKTAIL_DRESS, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 17,
		.djinnCnts = {0, 0, 0, 5},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 51 */
	{
		.unitId = ISAAC,
		.level = 18,
		.djinnCnts = {6},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 18,
		.djinnCnts = {0, 0, 5},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 18,
		.djinnCnts = {0, 5},
		.items = {ITEM_ANGELIC_ANKH, ITEM_COCKTAIL_DRESS, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 18,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 52 */
	{
		.unitId = ISAAC,
		.level = 18,
		.djinnCnts = {6},
		.items = {ITEM_SHORT_SWORD, 0, ITEM_IRON_SHIELD},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 53 */
	{
		.unitId = ISAAC,
		.level = 19,
		.djinnCnts = {6},
		.items = {ITEM_SHORT_SWORD, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 54 */
	{
		.unitId = ISAAC,
		.level = 19,
		.djinnCnts = {6},
		.items = {ITEM_CLAYMORE, ITEM_CHAIN_MAIL, ITEM_IRON_SHIELD},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 55 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 56 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 57 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 58 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 59 */
	{
		.unitId = ISAAC,
		.level = 1,
		.items = {ITEM_LONG_SWORD, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 1,
		.items = {ITEM_BATTLE_AXE, ITEM_LEATHER_ARMOR, ITEM_WOODEN_SHIELD, ITEM_OPEN_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = IVAN,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = MIA,
		.level = 1,
		.items = {ITEM_WOODEN_STICK, ITEM_TRAVEL_ROBE, ITEM_PADDED_GLOVES, ITEM_LEATHER_CAP},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 60 */
	{
		.unitId = ISAAC,
		.level = 20,
		.djinnCnts = {6},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 20,
		.djinnCnts = {0, 0, 5},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 20,
		.djinnCnts = {0, 5},
		.items = {ITEM_ANGELIC_ANKH, ITEM_COCKTAIL_DRESS, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 20,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 61 */
	{
		.unitId = ISAAC,
		.level = 20,
		.djinnCnts = {6},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 20,
		.djinnCnts = {0, 0, 5},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 20,
		.djinnCnts = {0, 5},
		.items = {ITEM_ANGELIC_ANKH, ITEM_COCKTAIL_DRESS, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 20,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 62 */
	{
		.unitId = ISAAC,
		.level = 21,
		.djinnCnts = {6},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 21,
		.djinnCnts = {0, 0, 5},
		.items = {ITEM_GREAT_SWORD, ITEM_ARMORED_SHELL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 21,
		.djinnCnts = {0, 6},
		.items = {ITEM_ANGELIC_ANKH, ITEM_COCKTAIL_DRESS, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 21,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_HEAVY_ARMLET, ITEM_GUARDIAN_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 63 */
	{
		.unitId = ISAAC,
		.level = 22,
		.djinnCnts = {6},
		.items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 22,
		.djinnCnts = {0, 0, 6},
		.items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_STEEL_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 22,
		.djinnCnts = {0, 6},
		.items = {ITEM_ANGELIC_ANKH, ITEM_BLESSED_ROBE, ITEM_HEAVY_ARMLET, ITEM_PLATINUM_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 22,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 64 */
	{
		.unitId = ISAAC,
		.level = 23,
		.djinnCnts = {7},
		.items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 23,
		.djinnCnts = {0, 0, 6},
		.items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 23,
		.djinnCnts = {0, 6},
		.items = {ITEM_ANGELIC_ANKH, ITEM_BLESSED_ROBE, ITEM_HEAVY_ARMLET, ITEM_PLATINUM_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 23,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 65 */
	{
		.unitId = ISAAC,
		.level = 24,
		.djinnCnts = {7},
		.items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 24,
		.djinnCnts = {0, 0, 6},
		.items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 24,
		.djinnCnts = {0, 7},
		.items = {ITEM_ANGELIC_ANKH, ITEM_BLESSED_ROBE, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 24,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_SILVER_VEST, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 66 */
	{
		.unitId = ISAAC,
		.level = 25,
		.djinnCnts = {7},
		.items = {ITEM_SHAMSHIR, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 25,
		.djinnCnts = {0, 0, 7},
		.items = {ITEM_DRAGON_AXE, ITEM_PLATE_MAIL, ITEM_KNIGHTS_SHIELD, ITEM_SILVER_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 25,
		.djinnCnts = {0, 7},
		.items = {ITEM_ANGELIC_ANKH, ITEM_BLESSED_ROBE, ITEM_HEAVY_ARMLET, ITEM_PLATINUM_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 25,
		.djinnCnts = {0, 0, 0, 6},
		.items = {ITEM_FROST_WAND, ITEM_MAGICAL_CASSOCK, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 67 */
	{
		.unitId = ISAAC,
		.level = 27,
		.djinnCnts = {7},
		.items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 27,
		.djinnCnts = {0, 0, 7},
		.items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 26,
		.djinnCnts = {0, 7},
		.items = {ITEM_ANGELIC_ANKH, ITEM_BLESSED_ROBE, ITEM_SPIRIT_ARMLET, ITEM_PLATINUM_CIRCLET},
	},
	{
		.unitId = IVAN,
		.level = 26,
		.djinnCnts = {0, 0, 0, 7},
		.items = {ITEM_FROST_WAND, ITEM_MAGICAL_CASSOCK, ITEM_SILVER_ARMLET, ITEM_PLATINUM_CIRCLET},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 68 */
	{
		.unitId = ISAAC,
		.level = 28,
		.djinnCnts = {7},
		.items = {ITEM_SILVER_BLADE, ITEM_STEEL_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 28,
		.djinnCnts = {0, 0, 7},
		.items = {ITEM_DRAGON_AXE, ITEM_STEEL_ARMOR, ITEM_KNIGHTS_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 28,
		.djinnCnts = {0, 7},
		.items = {ITEM_ANGELIC_ANKH, ITEM_BLESSED_ROBE, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
	},
	{
		.unitId = IVAN,
		.level = 28,
		.djinnCnts = {0, 0, 0, 7},
		.items = {ITEM_FROST_WAND, ITEM_MAGICAL_CASSOCK, ITEM_VIRTUOUS_ARMLET, ITEM_JEWELED_CROWN},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 69 */
	{
		.unitId = ISAAC,
		.level = 29,
		.djinnCnts = {7},
		.items = {ITEM_GAIA_BLADE, ITEM_ASURAS_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 29,
		.djinnCnts = {0, 0, 7},
		.items = {ITEM_GIANT_AXE, ITEM_STEEL_ARMOR, ITEM_EARTH_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 29,
		.djinnCnts = {0, 7},
		.items = {ITEM_ANGELIC_ANKH, ITEM_ORACLES_ROBE, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
	},
	{
		.unitId = IVAN,
		.level = 29,
		.djinnCnts = {0, 0, 0, 7},
		.items = {ITEM_CRYSTAL_ROD, ITEM_FEATHERED_ROBE, ITEM_VIRTUOUS_ARMLET, ITEM_JEWELED_CROWN},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 70 */
	{
		.unitId = ISAAC,
		.level = 33,
		.djinnCnts = {7},
		.items = {ITEM_GAIA_BLADE, ITEM_ASURAS_ARMOR, ITEM_MIRRORED_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = GARET,
		.level = 33,
		.djinnCnts = {0, 0, 7},
		.items = {ITEM_GIANT_AXE, ITEM_STEEL_ARMOR, ITEM_EARTH_SHIELD, ITEM_KNIGHTS_HELM},
		.abilities = {ABILITY_MOVE},
	},
	{
		.unitId = MIA,
		.level = 33,
		.djinnCnts = {0, 7},
		.items = {ITEM_ANGELIC_ANKH, ITEM_ORACLES_ROBE, ITEM_SPIRIT_ARMLET, ITEM_GLITTERING_TIARA},
	},
	{
		.unitId = IVAN,
		.level = 33,
		.djinnCnts = {0, 0, 0, 7},
		.items = {ITEM_CRYSTAL_ROD, ITEM_FEATHERED_ROBE, ITEM_VIRTUOUS_ARMLET, ITEM_JEWELED_CROWN},
		.abilities = {ABILITY_MIND_READ},
	},
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
	/* Set 71, empty */
	{
		.unitId = DEBUG_PARAMETER_SET_END,
	},
};
/* clang-format on */
