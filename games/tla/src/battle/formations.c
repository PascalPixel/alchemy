// The 660 enemy formations, by formation id, each run of them headed by the battle or place it is
// for, and the enemies whose level follows the party's.

#include "enemy_ids.h"
#include "battle/formations.h"

// clang-format off
FORMATION Formations[] = {
	[0] = {0},
	// Djinni battles, each Djinni alone, its element's Djinn in a run of eleven after an empty row: Venus, Mercury, Mars, then Jupiter (EncounterDjinnBattles gives each its zone and slot).
	[1] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_UNUSED_ECHO)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[2] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_IRON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[3] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_STEEL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[4] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_MUD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[5] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_FLOWER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[6] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_MELD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[7] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_PETRA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[8] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_UNUSED_SALT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[9] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_UNUSED_GEODE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[10] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_UNUSED_MOLD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[11] = {
		.memberIds = {ENEMY_ROW(ENEMY_VENUS_DJINNI_CRYSTAL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[12] = {0},
	[13] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_FOG)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[14] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_SOUR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[15] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_UNUSED_SPRING)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[16] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_UNUSED_SHADE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[17] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_CHILL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[18] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_STEAM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[19] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_UNUSED_RIME)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[20] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_UNUSED_GEL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[21] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_EDDY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[22] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_BALM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[23] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERCURY_DJINNI_SERAC)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[24] = {0},
	[25] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_CANNON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[26] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_UNUSED_SPARK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[27] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_KINDLE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[28] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_UNUSED_CHAR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[29] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_UNUSED_COAL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[30] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_REFLUX)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[31] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_CORE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[32] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_UNUSED_TINDER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[33] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_UNUSED_SHINE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[34] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_FURY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[35] = {
		.memberIds = {ENEMY_ROW(ENEMY_MARS_DJINNI_FUGUE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[36] = {0},
	[37] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_BREATH)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[38] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_BLITZ)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[39] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_UNUSED_ETHER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[40] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_WAFT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[41] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_UNUSED_HAZE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[42] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_WHEEZE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[43] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_UNUSED_AROMA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[44] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_WHORL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[45] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_GASP)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[46] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_UNUSED_LULL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[47] = {
		.memberIds = {ENEMY_ROW(ENEMY_JUPITER_DJINNI_GALE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[48] = {0},
	// Story battles: the Ruffian at Venus Lighthouse's entrance, the Ruffian, the three Ruffians and the Punch Ant of Suhalla Gate, and the three Chestbeaters in the Kandorean Temple.
	[49] = {
		.memberIds = {ENEMY_ROW(ENEMY_RUFFIAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[50] = {
		.memberIds = {ENEMY_ROW(ENEMY_RUFFIAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[51] = {
		.memberIds = {ENEMY_ROW(ENEMY_RUFFIAN)},
		.minCounts = {3},
		.maxCounts = {3},
	},
	[52] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUNCH_ANT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[53] = {
		.memberIds = {ENEMY_ROW(ENEMY_CHESTBEATER)},
		.minCounts = {3},
		.maxCounts = {3},
	},
	[54] = {0},
	// The King Scorpion in the Yampi Desert, and Briggs and a Sea Fighter on Briggs's ship at Alhafra.
	[55] = {
		.memberIds = {ENEMY_ROW(ENEMY_KING_SCORPION)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[56] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_BRIGGS), ENEMY_ROW(ENEMY_SEA_FIGHTER)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[57] = {0},
	// An Aqua Jelly below deck of the Lemurian Ship, then the Aqua Hydra.
	[58] = {
		.memberIds = {ENEMY_ROW(ENEMY_AQUA_JELLY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[59] = {
		.memberIds = {ENEMY_ROW(ENEMY_AQUA_HYDRA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[60] = {0},
	// The Serpent in Gaia Rock, one row for each beam of sunlight let into its lair from none to four; then Avimander in Champa and Poseidon at the Sea of Time.
	[61] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[62] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_ONE_BEAM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[63] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_TWO_BEAMS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[64] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_THREE_BEAMS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[65] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_FOUR_BEAMS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[66] = {
		.memberIds = {ENEMY_ROW(ENEMY_AVIMANDER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[67] = {
		.memberIds = {ENEMY_ROW(ENEMY_POSEIDON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[68] = {0},
	// Moapa and his two Knights at the Trial Road's peak, one row for each stage door the party reached first.
	[69] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA), ENEMY_ROW(ENEMY_KNIGHT), ENEMY_ROW(ENEMY_KNIGHT)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[70] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA), ENEMY_ROW(ENEMY_KNIGHT_B), ENEMY_ROW(ENEMY_KNIGHT)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[71] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA), ENEMY_ROW(ENEMY_KNIGHT_B), ENEMY_ROW(ENEMY_KNIGHT_B)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[72] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA_B), ENEMY_ROW(ENEMY_KNIGHT_B), ENEMY_ROW(ENEMY_KNIGHT_B)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[73] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA_C), ENEMY_ROW(ENEMY_KNIGHT_B), ENEMY_ROW(ENEMY_KNIGHT_B)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	// Karst and Agatio on Jupiter Lighthouse's aerie.
	[FORMATION_KARST_AND_AGATIO_A] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_KARST), ENEMY_ROW(ENEMY_AGATIO)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[FORMATION_KARST_AND_AGATIO_B] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_KARST), ENEMY_ROW(ENEMY_AGATIO)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[FORMATION_KARST_AND_AGATIO_C] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_KARST_B), ENEMY_ROW(ENEMY_AGATIO_B)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	// The two Flame Dragons in Mars Lighthouse, then the Doom Dragon's three forms on its aerie.
	[77] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_FLAME_DRAGON_KARST), ENEMY_ROW(ENEMY_FLAME_DRAGON_AGATIO)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[FORMATION_DOOM_DRAGON_A] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_DOOM_DRAGON_A)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[FORMATION_DOOM_DRAGON_B] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_DOOM_DRAGON_E)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[FORMATION_DOOM_DRAGON_C] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_DOOM_DRAGON_H)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	// One enemy class a row, most of them alone (the Star Magician with his four balls). No encounter zone holds these: only a Battle Arena monster battle, which may choose any formation (btFormationSelectLevelMatchedCandidate), and the debug battle menu fight them.
	[81] = {
		.memberIds = {ENEMY_ROW(ENEMY_RUFFIAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[82] = {
		.memberIds = {ENEMY_ROW(ENEMY_RUFFIAN_2)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[83] = {
		.memberIds = {ENEMY_ROW(ENEMY_RUFFIAN_3)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[84] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUNCH_ANT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[85] = {
		.memberIds = {ENEMY_ROW(ENEMY_FLASH_ANT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[86] = {
		.memberIds = {ENEMY_ROW(ENEMY_NUMB_ANT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[87] = {
		.memberIds = {ENEMY_ROW(ENEMY_CHESTBEATER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[88] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_GORILLA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[89] = {
		.memberIds = {ENEMY_ROW(ENEMY_CRAZY_GORILLA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[90] = {
		.memberIds = {ENEMY_ROW(ENEMY_KING_SCORPION)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[91] = {
		.memberIds = {ENEMY_ROW(ENEMY_DEVIL_SCORPION)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[92] = {
		.memberIds = {ENEMY_ROW(ENEMY_SAND_SCORPION)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[93] = {
		.memberIds = {ENEMY_ROW(ENEMY_BRIGGS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[94] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEA_FIGHTER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[95] = {
		.memberIds = {ENEMY_ROW(ENEMY_CHAMPA_2)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[96] = {
		.memberIds = {ENEMY_ROW(ENEMY_CHAMPA_3)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[97] = {
		.memberIds = {ENEMY_ROW(ENEMY_CUTTLE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[98] = {
		.memberIds = {ENEMY_ROW(ENEMY_CALAMAR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[99] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAN_O_WAR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[100] = {
		.memberIds = {ENEMY_ROW(ENEMY_AQUA_JELLY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[101] = {
		.memberIds = {ENEMY_ROW(ENEMY_AQUA_HYDRA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[102] = {
		.memberIds = {ENEMY_ROW(ENEMY_HYDRA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[103] = {
		.memberIds = {ENEMY_ROW(ENEMY_PYRODRA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[104] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[105] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_ONE_BEAM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[106] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_TWO_BEAMS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[107] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_THREE_BEAMS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[108] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_FOUR_BEAMS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[109] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_6)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[110] = {
		.memberIds = {ENEMY_ROW(ENEMY_SERPENT_7)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[111] = {
		.memberIds = {ENEMY_ROW(ENEMY_AVIMANDER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[112] = {
		.memberIds = {ENEMY_ROW(ENEMY_MACETAIL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[113] = {
		.memberIds = {ENEMY_ROW(ENEMY_BOMBANDER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[114] = {
		.memberIds = {ENEMY_ROW(ENEMY_POSEIDON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[115] = {
		.memberIds = {ENEMY_ROW(ENEMY_POSEIDON_SECOND_ACTION)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[116] = {
		.memberIds = {ENEMY_ROW(ENEMY_POSEIDON_3)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[117] = {
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[118] = {
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA_B)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[119] = {
		.memberIds = {ENEMY_ROW(ENEMY_MOAPA_C)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[120] = {
		.memberIds = {ENEMY_ROW(ENEMY_KNIGHT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[121] = {
		.memberIds = {ENEMY_ROW(ENEMY_KNIGHT_B)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[122] = {
		.memberIds = {ENEMY_ROW(ENEMY_KNIGHT_3)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[123] = {
		.memberIds = {ENEMY_ROW(ENEMY_AGATIO)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[124] = {
		.memberIds = {ENEMY_ROW(ENEMY_AGATIO_B)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[125] = {
		.memberIds = {ENEMY_ROW(ENEMY_AGATIO_VS_ISAAC)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[126] = {
		.memberIds = {ENEMY_ROW(ENEMY_KARST)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[127] = {
		.memberIds = {ENEMY_ROW(ENEMY_KARST_B)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[128] = {
		.memberIds = {ENEMY_ROW(ENEMY_KARST_VS_ISAAC)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[129] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[130] = {
		.memberIds = {ENEMY_ROW(ENEMY_DIRE_WOLF)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[131] = {
		.memberIds = {ENEMY_ROW(ENEMY_WHITE_WOLF)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[132] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[133] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[134] = {
		.memberIds = {ENEMY_ROW(ENEMY_CHIMERA_WORM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[135] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN)},
		.minCounts = {3},
		.maxCounts = {3},
	},
	[136] = {
		.memberIds = {ENEMY_ROW(ENEMY_ALEC_GOBLIN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[137] = {
		.memberIds = {ENEMY_ROW(ENEMY_BABOON_GOBLIN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[138] = {
		.memberIds = {ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[139] = {
		.memberIds = {ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[140] = {
		.memberIds = {ENEMY_ROW(ENEMY_MOMANGLER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[141] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD)},
		.minCounts = {3},
		.maxCounts = {3},
	},
	[142] = {
		.memberIds = {ENEMY_ROW(ENEMY_WARGOLD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[143] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD_KING)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[144] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_GABOMBA_CATACOMBS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[145] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_TAOPO_SWAMP)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[146] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_GAIA_ROCK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[147] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_JUPITER_LIGHTHOUSE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[148] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_TRIAL_ROAD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[149] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[150] = {
		.memberIds = {ENEMY_ROW(ENEMY_TALON_RUNNER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[151] = {
		.memberIds = {ENEMY_ROW(ENEMY_WINGED_RUNNER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[152] = {
		.memberIds = {ENEMY_ROW(ENEMY_MUMMY)},
		.minCounts = {3},
		.maxCounts = {3},
	},
	[153] = {
		.memberIds = {ENEMY_ROW(ENEMY_FOUL_MUMMY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[154] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRAVE_WIGHT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[155] = {
		.memberIds = {ENEMY_ROW(ENEMY_WYVERN_CHICK)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[156] = {
		.memberIds = {ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[157] = {
		.memberIds = {ENEMY_ROW(ENEMY_WINGED_LIZARD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[158] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN_CUB)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[159] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[160] = {
		.memberIds = {ENEMY_ROW(ENEMY_SKINWALKER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[161] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[162] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINOX)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[163] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINOSAURUS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[164] = {
		.memberIds = {ENEMY_ROW(ENEMY_ASSASSIN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[165] = {
		.memberIds = {ENEMY_ROW(ENEMY_SLAYER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[166] = {
		.memberIds = {ENEMY_ROW(ENEMY_DARK_MURDER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[167] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOOMSAYER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[168] = {
		.memberIds = {ENEMY_ROW(ENEMY_LICH)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[169] = {
		.memberIds = {ENEMY_ROW(ENEMY_BANE_WIGHT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[170] = {
		.memberIds = {ENEMY_ROW(ENEMY_PIXIE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[171] = {
		.memberIds = {ENEMY_ROW(ENEMY_FAERY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[172] = {
		.memberIds = {ENEMY_ROW(ENEMY_WEIRD_NYPMH)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[173] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOOD_WALKER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[174] = {
		.memberIds = {ENEMY_ROW(ENEMY_ELDER_WOOD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[175] = {
		.memberIds = {ENEMY_ROW(ENEMY_ESTRE_WOOD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[176] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_STAR_MAGICIAN), ENEMY_ROW(ENEMY_REFRESH_BALL), ENEMY_ROW(ENEMY_GUARDIAN_BALL), ENEMY_ROW(ENEMY_THUNDER_BALL), ENEMY_ROW(ENEMY_ANGER_BALL)},
		.minCounts = {1, 1, 1, 1, 1},
		.maxCounts = {1, 1, 1, 1, 1},
	},
	[177] = {
		.memberIds = {ENEMY_ROW(ENEMY_DARK_WIZARD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[178] = {
		.memberIds = {ENEMY_ROW(ENEMY_EVIL_SHAMAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[179] = {
		.memberIds = {ENEMY_ROW(ENEMY_URCHIN_BEAST)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[180] = {
		.memberIds = {ENEMY_ROW(ENEMY_NEEDLE_EGG)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[181] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEA_HEDGEHOG)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[182] = {
		.memberIds = {ENEMY_ROW(ENEMY_CONCH_SHELL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[183] = {
		.memberIds = {ENEMY_ROW(ENEMY_SPIRAL_SHELL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[184] = {
		.memberIds = {ENEMY_ROW(ENEMY_POISON_SHELL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[185] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERMAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[186] = {
		.memberIds = {ENEMY_ROW(ENEMY_GILLMAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[187] = {
		.memberIds = {ENEMY_ROW(ENEMY_GILLMAN_LORD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[188] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEA_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[189] = {
		.memberIds = {ENEMY_ROW(ENEMY_TURTLE_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[190] = {
		.memberIds = {ENEMY_ROW(ENEMY_OCEAN_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[191] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEABIRD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[192] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEAFOWL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[193] = {
		.memberIds = {ENEMY_ROW(ENEMY_GREAT_SEAGULL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[194] = {
		.memberIds = {ENEMY_ROW(ENEMY_ROC)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[195] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAPTOR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[196] = {
		.memberIds = {ENEMY_ROW(ENEMY_FELL_RAPTOR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[197] = {
		.memberIds = {ENEMY_ROW(ENEMY_WYVERN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[198] = {
		.memberIds = {ENEMY_ROW(ENEMY_SKY_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[199] = {
		.memberIds = {ENEMY_ROW(ENEMY_WINGED_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[200] = {
		.memberIds = {ENEMY_ROW(ENEMY_PHOENIX)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[201] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_BIRD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[202] = {
		.memberIds = {ENEMY_ROW(ENEMY_WONDER_BIRD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[203] = {
		.memberIds = {ENEMY_ROW(ENEMY_BLUE_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[204] = {
		.memberIds = {ENEMY_ROW(ENEMY_CRUEL_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[205] = {
		.memberIds = {ENEMY_ROW(ENEMY_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[206] = {
		.memberIds = {ENEMY_ROW(ENEMY_FLAME_DRAGON_AGATIO)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[207] = {
		.memberIds = {ENEMY_ROW(ENEMY_FLAME_DRAGON_KARST)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[208] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[209] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOTAURUS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[210] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOS_WARRIOR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[211] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOS_KNIGHT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[212] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRESSIL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[213] = {
		.memberIds = {ENEMY_ROW(ENEMY_LITTLE_DEATH)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[214] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_DEATH)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[215] = {
		.memberIds = {ENEMY_ROW(ENEMY_RED_DEMON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[216] = {
		.memberIds = {ENEMY_ROW(ENEMY_LESSER_DEMON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[217] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_DEMON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[218] = {
		.memberIds = {ENEMY_ROW(ENEMY_AKA_MANAH)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[219] = {
		.memberIds = {ENEMY_ROW(ENEMY_DRUJ)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[220] = {
		.memberIds = {ENEMY_ROW(ENEMY_AESHMA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[221] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_VALUKAR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[222] = {
		.memberIds = {ENEMY_ROW(ENEMY_VALUKAR_UNUSED_A)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[223] = {
		.memberIds = {ENEMY_ROW(ENEMY_VALUKAR_UNUSED_B)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[224] = {
		.memberIds = {ENEMY_ROW(ENEMY_LIVING_ARMOR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[225] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUPPET_WARRIOR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[226] = {
		.memberIds = {ENEMY_ROW(ENEMY_ESTRE_BARON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[227] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST_ARMY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[228] = {
		.memberIds = {ENEMY_ROW(ENEMY_SOUL_ARMY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[229] = {
		.memberIds = {ENEMY_ROW(ENEMY_SPIRIT_ARMY)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[230] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_DULLAHAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[231] = {
		.memberIds = {ENEMY_ROW(ENEMY_DULLAHAN_B)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[232] = {
		.memberIds = {ENEMY_ROW(ENEMY_DULLAHAN_C)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[233] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_SENTINEL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[234] = {
		.memberIds = {ENEMY_ROW(ENEMY_SENTINEL_UNUSED_A)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[235] = {
		.memberIds = {ENEMY_ROW(ENEMY_SENTINEL_UNUSED_B)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[236] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOOM_DRAGON_A)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[237] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOOM_DRAGON_E)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[238] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOOM_DRAGON_H)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[239] = {0},
	[240] = {0},
	// From here to 628, a place's random battles in a run of up to eight, the encounter zones its scenes take choosing among them with weights of their own.
	//
	// The World Map's encounter region 1, on foot; the Psynergy test room's battles take its first.
	[241] = {
		.memberIds = {ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[242] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUNCH_ANT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[243] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[244] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUNCH_ANT)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[245] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUNCH_ANT), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[246] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[247] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF), ENEMY_ROW(ENEMY_PUNCH_ANT)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[248] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF), ENEMY_ROW(ENEMY_PUNCH_ANT), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[249] = {0},
	[250] = {0},
	// The Kandorean Temple.
	[251] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[252] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUNCH_ANT), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[253] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[254] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[255] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF), ENEMY_ROW(ENEMY_PUNCH_ANT)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[256] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[257] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM), ENEMY_ROW(ENEMY_PUNCH_ANT)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[258] = {
		.memberIds = {ENEMY_ROW(ENEMY_AMAZE_KANDOREAN_TEMPLE)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[259] = {0},
	[260] = {0},
	// The Shrine of the Sea God.
	[261] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[262] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[263] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[264] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM), ENEMY_ROW(ENEMY_GIANT_BAT)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[265] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[266] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM), ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[267] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[268] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[269] = {0},
	[270] = {0},
	// The Dehkan Plateau. Its zones take 274 again in their last slot, so none holds 278.
	[271] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[272] = {
		.memberIds = {ENEMY_ROW(ENEMY_ANGLE_WORM), ENEMY_ROW(ENEMY_GHOST)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[273] = {
		.memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_GHOST)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[274] = {
		.memberIds = {ENEMY_ROW(ENEMY_SKELETON), ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[275] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[276] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN), ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[277] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[278] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[279] = {0},
	[280] = {0},
	// The World Map's encounter region 11, on foot.
	[281] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[282] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN), ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[283] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN), ENEMY_ROW(ENEMY_ANGLE_WORM)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[284] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN), ENEMY_ROW(ENEMY_RAT_SOLDIER)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[285] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAT)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[286] = {
		.memberIds = {ENEMY_ROW(ENEMY_SKELETON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[287] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILL_HEAD)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[288] = {
		.memberIds = {ENEMY_ROW(ENEMY_TROLL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[289] = {0},
	[290] = {0},
	// The Madra Catacombs.
	[291] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN), ENEMY_ROW(ENEMY_DRONE_BEE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[292] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINI_GOBLIN), ENEMY_ROW(ENEMY_DRONE_BEE)},
		.minCounts = {0, 1},
		.maxCounts = {2, 1},
	},
	[293] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_DRONE_BEE)},
		.minCounts = {0, 1},
		.maxCounts = {1, 1},
	},
	[294] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAT_SOLDIER), ENEMY_ROW(ENEMY_MINI_GOBLIN)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[295] = {
		.memberIds = {ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_RAT)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[296] = {
		.memberIds = {ENEMY_ROW(ENEMY_TROLL)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[297] = {0},
	[298] = {0},
	[299] = {0},
	[300] = {0},
	// The World Map's encounter regions 2 and 12 (the desert round Air's Rock), on foot.
	[301] = {
		.memberIds = {ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[302] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[303] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[304] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINO), ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {0, 1},
		.maxCounts = {1, 1},
	},
	[305] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINO), ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[306] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[307] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_DINO), ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {1},
		.maxCounts = {1, 1, 1},
	},
	[308] = {
		.memberIds = {ENEMY_ROW(ENEMY_TROLL), ENEMY_ROW(ENEMY_KOBOLD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[309] = {0},
	[310] = {0},
	// The Yampi Desert.
	[311] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[312] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[313] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU), ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[314] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[315] = {
		.memberIds = {ENEMY_ROW(ENEMY_SPIDER), ENEMY_ROW(ENEMY_MOMONGA)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[316] = {
		.memberIds = {ENEMY_ROW(ENEMY_SPIDER), ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[317] = {
		.memberIds = {ENEMY_ROW(ENEMY_GNOME)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[318] = {
		.memberIds = {ENEMY_ROW(ENEMY_GNOME), ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[319] = {0},
	[320] = {0},
	// Air's Rock outside, then inside.
	[321] = {
		.memberIds = {ENEMY_ROW(ENEMY_KOBOLD), ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[322] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU), ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[323] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU), ENEMY_ROW(ENEMY_KOBOLD)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[324] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU), ENEMY_ROW(ENEMY_KOBOLD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[325] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOUL), ENEMY_ROW(ENEMY_DINO)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[326] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARPY), ENEMY_ROW(ENEMY_KOBOLD)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[327] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARPY), ENEMY_ROW(ENEMY_EMU)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[328] = {0},
	[329] = {0},
	[330] = {0},
	[331] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[332] = {
		.memberIds = {ENEMY_ROW(ENEMY_EMU), ENEMY_ROW(ENEMY_GHOUL)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[333] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARPY), ENEMY_ROW(ENEMY_EMU), ENEMY_ROW(ENEMY_GHOUL)},
		.minCounts = {1, 0, 1},
		.maxCounts = {1, 1, 1},
	},
	[334] = {
		.memberIds = {ENEMY_ROW(ENEMY_CREEPER), ENEMY_ROW(ENEMY_EMU)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[335] = {
		.memberIds = {ENEMY_ROW(ENEMY_MUMMY), ENEMY_ROW(ENEMY_GHOUL)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[336] = {
		.memberIds = {ENEMY_ROW(ENEMY_MUMMY), ENEMY_ROW(ENEMY_HARPY)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[337] = {
		.memberIds = {ENEMY_ROW(ENEMY_CREEPER), ENEMY_ROW(ENEMY_HARPY)},
		.minCounts = {0, 1},
		.maxCounts = {1, 1},
	},
	[338] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARPY), ENEMY_ROW(ENEMY_MUMMY)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[339] = {0},
	[340] = {0},
	// The Gondowan Cliffs.
	[341] = {
		.memberIds = {ENEMY_ROW(ENEMY_FLASH_ANT)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[342] = {
		.memberIds = {ENEMY_ROW(ENEMY_WYVERN_CHICK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[343] = {
		.memberIds = {ENEMY_ROW(ENEMY_FLASH_ANT), ENEMY_ROW(ENEMY_WYVERN_CHICK)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[344] = {
		.memberIds = {ENEMY_ROW(ENEMY_WYVERN_CHICK), ENEMY_ROW(ENEMY_FLASH_ANT)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[345] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN_CUB)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[346] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN_CUB), ENEMY_ROW(ENEMY_WYVERN_CHICK)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[347] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_GORILLA)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[348] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_GORILLA), ENEMY_ROW(ENEMY_WOLFKIN_CUB)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[349] = {0},
	[350] = {0},
	// The World Map's encounter regions 3 and 18 (the ground darkened until the Gabomba ceremony ends), on foot.
	[351] = {
		.memberIds = {ENEMY_ROW(ENEMY_FLASH_ANT)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[352] = {
		.memberIds = {ENEMY_ROW(ENEMY_FLASH_ANT), ENEMY_ROW(ENEMY_WYVERN_CHICK)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[353] = {
		.memberIds = {ENEMY_ROW(ENEMY_WYVERN_CHICK), ENEMY_ROW(ENEMY_MOLE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[354] = {
		.memberIds = {ENEMY_ROW(ENEMY_WYVERN_CHICK), ENEMY_ROW(ENEMY_FLASH_ANT)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[355] = {
		.memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_MOLE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[356] = {
		.memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_DEATH_HEAD)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[357] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN_CUB), ENEMY_ROW(ENEMY_WYVERN_CHICK)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[358] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN_CUB), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[359] = {0},
	[360] = {0},
	// The Kibombo Mountains.
	[361] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_GORILLA), ENEMY_ROW(ENEMY_DIRGE)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[362] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_GORILLA), ENEMY_ROW(ENEMY_PIXIE)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[363] = {
		.memberIds = {ENEMY_ROW(ENEMY_PIXIE), ENEMY_ROW(ENEMY_SPIRIT)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[364] = {
		.memberIds = {ENEMY_ROW(ENEMY_PIXIE), ENEMY_ROW(ENEMY_WILD_GORILLA)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[365] = {
		.memberIds = {ENEMY_ROW(ENEMY_DIRGE), ENEMY_ROW(ENEMY_PIXIE)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[366] = {
		.memberIds = {ENEMY_ROW(ENEMY_ASSASSIN), ENEMY_ROW(ENEMY_PIXIE)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[367] = {
		.memberIds = {ENEMY_ROW(ENEMY_ASSASSIN), ENEMY_ROW(ENEMY_WILD_GORILLA)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[368] = {
		.memberIds = {ENEMY_ROW(ENEMY_PIXIE)},
		.minCounts = {2},
		.maxCounts = {3},
	},
	[369] = {0},
	[370] = {0},
	// The Gabomba Statue.
	[371] = {
		.memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[372] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOOMSAYER), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[373] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOOMSAYER), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[374] = {
		.memberIds = {ENEMY_ROW(ENEMY_SALAMANDER)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[375] = {
		.memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[376] = {
		.memberIds = {ENEMY_ROW(ENEMY_SALAMANDER), ENEMY_ROW(ENEMY_SPIRIT)},
		.minCounts = {0, 1},
		.maxCounts = {1, 1},
	},
	[377] = {
		.memberIds = {ENEMY_ROW(ENEMY_RED_DEMON), ENEMY_ROW(ENEMY_DOOMSAYER)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[378] = {
		.memberIds = {ENEMY_ROW(ENEMY_RED_DEMON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[379] = {0},
	[380] = {0},
	// One to four Conch Shells in the Lemurian Ship's cabins.
	[381] = {
		.memberIds = {ENEMY_ROW(ENEMY_CONCH_SHELL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[382] = {
		.memberIds = {ENEMY_ROW(ENEMY_CONCH_SHELL)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[383] = {
		.memberIds = {ENEMY_ROW(ENEMY_CONCH_SHELL)},
		.minCounts = {3},
		.maxCounts = {3},
	},
	[384] = {
		.memberIds = {ENEMY_ROW(ENEMY_CONCH_SHELL)},
		.minCounts = {4},
		.maxCounts = {4},
	},
	[385] = {0},
	[386] = {0},
	[387] = {0},
	[388] = {0},
	[389] = {0},
	[390] = {0},
	// The World Map's encounter regions 7 and 9, aboard the ship.
	[391] = {
		.memberIds = {ENEMY_ROW(ENEMY_URCHIN_BEAST)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[392] = {
		.memberIds = {ENEMY_ROW(ENEMY_URCHIN_BEAST), ENEMY_ROW(ENEMY_CALAMAR)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[393] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEABIRD)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[394] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEABIRD), ENEMY_ROW(ENEMY_URCHIN_BEAST)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[395] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[396] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIGHTER_BEE), ENEMY_ROW(ENEMY_SEABIRD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[397] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERMAN), ENEMY_ROW(ENEMY_CALAMAR)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[398] = {
		.memberIds = {ENEMY_ROW(ENEMY_MERMAN)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[399] = {0},
	[400] = {0},
	// The World Map's encounter region 4, on foot (its snowfield takes Tundaria Tower's).
	[401] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_DIRE_WOLF)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[402] = {
		.memberIds = {ENEMY_ROW(ENEMY_NUMB_ANT), ENEMY_ROW(ENEMY_DEATH_CAP)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[403] = {
		.memberIds = {ENEMY_ROW(ENEMY_DIRE_WOLF)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[404] = {
		.memberIds = {ENEMY_ROW(ENEMY_DIRE_WOLF), ENEMY_ROW(ENEMY_NUMB_ANT)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[405] = {
		.memberIds = {ENEMY_ROW(ENEMY_ALEC_GOBLIN), ENEMY_ROW(ENEMY_RAVAGER)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[406] = {
		.memberIds = {ENEMY_ROW(ENEMY_ALEC_GOBLIN), ENEMY_ROW(ENEMY_DIRE_WOLF)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[407] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST_MAGE), ENEMY_ROW(ENEMY_UNDEAD)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[408] = {
		.memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_NUMB_ANT)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[409] = {0},
	[410] = {0},
	// Taopo Swamp.
	[411] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_DEATH_CAP)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[412] = {
		.memberIds = {ENEMY_ROW(ENEMY_CAVE_TROLL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[413] = {
		.memberIds = {ENEMY_ROW(ENEMY_DIRE_WOLF)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[414] = {
		.memberIds = {ENEMY_ROW(ENEMY_DIRE_WOLF), ENEMY_ROW(ENEMY_GHOST_MAGE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[415] = {
		.memberIds = {ENEMY_ROW(ENEMY_UNDEAD), ENEMY_ROW(ENEMY_CAVE_TROLL)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[416] = {
		.memberIds = {ENEMY_ROW(ENEMY_DEATH_CAP), ENEMY_ROW(ENEMY_DIRE_WOLF)},
		.minCounts = {0, 1},
		.maxCounts = {2, 1},
	},
	[417] = {
		.memberIds = {ENEMY_ROW(ENEMY_FAERY), ENEMY_ROW(ENEMY_DIRE_WOLF)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[418] = {
		.memberIds = {ENEMY_ROW(ENEMY_FAERY), ENEMY_ROW(ENEMY_MAD_VERMIN)},
		.minCounts = {1, 1},
		.maxCounts = {2, 1},
	},
	[419] = {0},
	[420] = {0},
	// The Apojii Islands Cave and Aqua Rock.
	[421] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAN_O_WAR), ENEMY_ROW(ENEMY_FAERY)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[422] = {
		.memberIds = {ENEMY_ROW(ENEMY_ROC), ENEMY_ROW(ENEMY_FAERY)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[423] = {
		.memberIds = {ENEMY_ROW(ENEMY_ROC)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[424] = {
		.memberIds = {ENEMY_ROW(ENEMY_VIRAGO), ENEMY_ROW(ENEMY_FAERY)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[425] = {
		.memberIds = {ENEMY_ROW(ENEMY_VIRAGO), ENEMY_ROW(ENEMY_GHOST_MAGE)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[426] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEA_DRAGON), ENEMY_ROW(ENEMY_LIZARD_FIGHTER)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[427] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEA_DRAGON), ENEMY_ROW(ENEMY_ROC)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[428] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEA_DRAGON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[429] = {0},
	[430] = {0},
	// Gaia Rock.
	[431] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[432] = {
		.memberIds = {ENEMY_ROW(ENEMY_WIGHT), ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[433] = {
		.memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[434] = {
		.memberIds = {ENEMY_ROW(ENEMY_GOLEM), ENEMY_ROW(ENEMY_CLAY_GARGOYLE)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[435] = {
		.memberIds = {ENEMY_ROW(ENEMY_DREAD_HOUND), ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[436] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOOD_WALKER), ENEMY_ROW(ENEMY_MAD_VERMIN), ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[437] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOOD_WALKER), ENEMY_ROW(ENEMY_CLAY_GARGOYLE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[438] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOOD_WALKER)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[439] = {0},
	[440] = {0},
	// The Ankohl Ruins.
	[441] = {
		.memberIds = {ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[442] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINOX)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[443] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOOD_WALKER), ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[444] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOTAURUS), ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[445] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOTAURUS), ENEMY_ROW(ENEMY_DINOX)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[446] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARRIDAN)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[447] = {
		.memberIds = {ENEMY_ROW(ENEMY_LIVING_ARMOR), ENEMY_ROW(ENEMY_MINOTAURUS)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[448] = {
		.memberIds = {ENEMY_ROW(ENEMY_LIVING_ARMOR)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[449] = {0},
	[450] = {0},
	// Tundaria Tower, and the snowfield of the World Map's encounter region 4.
	[451] = {
		.memberIds = {ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[452] = {
		.memberIds = {ENEMY_ROW(ENEMY_NEEDLE_EGG)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[453] = {
		.memberIds = {ENEMY_ROW(ENEMY_NEEDLE_EGG), ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {2, 2},
	},
	[454] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINOX), ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[455] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOTAURUS)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[456] = {
		.memberIds = {ENEMY_ROW(ENEMY_LIVING_ARMOR), ENEMY_ROW(ENEMY_HARRIDAN)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[457] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAGICORE), ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[458] = {
		.memberIds = {ENEMY_ROW(ENEMY_STONE_SOLDIER), ENEMY_ROW(ENEMY_NEEDLE_EGG)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[459] = {0},
	[460] = {0},
	// The World Map's encounter region 8, aboard the ship.
	[461] = {
		.memberIds = {ENEMY_ROW(ENEMY_SPIRAL_SHELL)},
		.minCounts = {2},
		.maxCounts = {4},
	},
	[462] = {
		.memberIds = {ENEMY_ROW(ENEMY_GILLMAN), ENEMY_ROW(ENEMY_SPIRAL_SHELL)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[463] = {
		.memberIds = {ENEMY_ROW(ENEMY_GILLMAN), ENEMY_ROW(ENEMY_SEAFOWL)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[464] = {
		.memberIds = {ENEMY_ROW(ENEMY_VILE_DIRGE), ENEMY_ROW(ENEMY_SEAFOWL)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[465] = {
		.memberIds = {ENEMY_ROW(ENEMY_HYDRA), ENEMY_ROW(ENEMY_SEAFOWL)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[466] = {
		.memberIds = {ENEMY_ROW(ENEMY_HYDRA), ENEMY_ROW(ENEMY_GILLMAN)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[467] = {
		.memberIds = {ENEMY_ROW(ENEMY_TURTLE_DRAGON)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[468] = {
		.memberIds = {ENEMY_ROW(ENEMY_TURTLE_DRAGON), ENEMY_ROW(ENEMY_HYDRA)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[469] = {0},
	[470] = {0},
	// The World Map's encounter regions 5, 15 (the sandy shore north of Shaman Village) and 16 (the snow round Prox), on foot.
	[471] = {
		.memberIds = {ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[472] = {
		.memberIds = {ENEMY_ROW(ENEMY_WARGOLD), ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[473] = {
		.memberIds = {ENEMY_ROW(ENEMY_TALON_RUNNER), ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[474] = {
		.memberIds = {ENEMY_ROW(ENEMY_TALON_RUNNER)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[475] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN), ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[476] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN), ENEMY_ROW(ENEMY_WARGOLD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[477] = {
		.memberIds = {ENEMY_ROW(ENEMY_SLAYER), ENEMY_ROW(ENEMY_WARGOLD)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[478] = {
		.memberIds = {ENEMY_ROW(ENEMY_SLAYER), ENEMY_ROW(ENEMY_WOLFKIN)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[479] = {0},
	[480] = {0},
	// Jupiter Lighthouse.
	[481] = {
		.memberIds = {ENEMY_ROW(ENEMY_DEVIL_SCORPION)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[482] = {
		.memberIds = {ENEMY_ROW(ENEMY_FOUL_MUMMY), ENEMY_ROW(ENEMY_DEVIL_SCORPION)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[483] = {
		.memberIds = {ENEMY_ROW(ENEMY_MACETAIL), ENEMY_ROW(ENEMY_DEVIL_SCORPION)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[484] = {
		.memberIds = {ENEMY_ROW(ENEMY_WYVERN)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[485] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST_ARMY), ENEMY_ROW(ENEMY_WYVERN)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[486] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOST_ARMY), ENEMY_ROW(ENEMY_FOUL_MUMMY)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[487] = {
		.memberIds = {ENEMY_ROW(ENEMY_BLUE_DRAGON), ENEMY_ROW(ENEMY_WYVERN)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[488] = {
		.memberIds = {ENEMY_ROW(ENEMY_BLUE_DRAGON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[489] = {0},
	[490] = {0},
	// Magma Rock, outside and in.
	[491] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAGING_ROCK)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[492] = {
		.memberIds = {ENEMY_ROW(ENEMY_WISE_GRYPHON), ENEMY_ROW(ENEMY_RAGING_ROCK)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[493] = {
		.memberIds = {ENEMY_ROW(ENEMY_LICH), ENEMY_ROW(ENEMY_WISE_GRYPHON)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[494] = {
		.memberIds = {ENEMY_ROW(ENEMY_LITTLE_DEATH), ENEMY_ROW(ENEMY_LICH)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[495] = {
		.memberIds = {ENEMY_ROW(ENEMY_PHOENIX), ENEMY_ROW(ENEMY_LITTLE_DEATH), ENEMY_ROW(ENEMY_LICH)},
		.minCounts = {1},
		.maxCounts = {1, 1, 1},
	},
	[496] = {
		.memberIds = {ENEMY_ROW(ENEMY_PHOENIX)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[497] = {
		.memberIds = {ENEMY_ROW(ENEMY_LESSER_DEMON), ENEMY_ROW(ENEMY_LITTLE_DEATH)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[498] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRAND_CHIMERA)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[499] = {0},
	[500] = {0},
	// Mars Lighthouse. Two encounter zones that no rule or scene names take 495 to 498 and 501 to 504 as well.
	[501] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOODLE_BUG)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[502] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_BIRD)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[503] = {
		.memberIds = {ENEMY_ROW(ENEMY_RAPTOR)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[504] = {
		.memberIds = {ENEMY_ROW(ENEMY_DEVIL_FROG), ENEMY_ROW(ENEMY_RAPTOR)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[505] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOS_WARRIOR), ENEMY_ROW(ENEMY_FIRE_BIRD)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[506] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOS_WARRIOR)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[507] = {
		.memberIds = {ENEMY_ROW(ENEMY_AKA_MANAH), ENEMY_ROW(ENEMY_MINOS_WARRIOR)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[508] = {
		.memberIds = {ENEMY_ROW(ENEMY_AKA_MANAH)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[509] = {0},
	[510] = {0},
	// The Alhafran Cave.
	[511] = {
		.memberIds = {ENEMY_ROW(ENEMY_OOZE)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[512] = {
		.memberIds = {ENEMY_ROW(ENEMY_OOZE), ENEMY_ROW(ENEMY_GNOME)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[513] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOUL)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[514] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOUL), ENEMY_ROW(ENEMY_OOZE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[515] = {
		.memberIds = {ENEMY_ROW(ENEMY_GHOUL), ENEMY_ROW(ENEMY_GNOME)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[516] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARPY), ENEMY_ROW(ENEMY_OOZE)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[517] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARPY), ENEMY_ROW(ENEMY_GHOUL)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[518] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARPY)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[519] = {0},
	[520] = {0},
	// The Gabomba Catacombs.
	[521] = {
		.memberIds = {ENEMY_ROW(ENEMY_DOOMSAYER), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[522] = {
		.memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[523] = {
		.memberIds = {ENEMY_ROW(ENEMY_BONE_FIGHTER), ENEMY_ROW(ENEMY_DEATH_HEAD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[524] = {
		.memberIds = {ENEMY_ROW(ENEMY_SALAMANDER)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[525] = {
		.memberIds = {ENEMY_ROW(ENEMY_SPIRIT), ENEMY_ROW(ENEMY_BONE_FIGHTER)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[526] = {
		.memberIds = {ENEMY_ROW(ENEMY_DEATH_HEAD), ENEMY_ROW(ENEMY_SPIRIT)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[527] = {
		.memberIds = {ENEMY_ROW(ENEMY_RED_DEMON), ENEMY_ROW(ENEMY_DOOMSAYER)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[528] = {
		.memberIds = {ENEMY_ROW(ENEMY_RED_DEMON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[529] = {0},
	[530] = {0},
	// The Izumo Ruins.
	[531] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[532] = {
		.memberIds = {ENEMY_ROW(ENEMY_WIGHT), ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1, 1},
		.maxCounts = {1, 2},
	},
	[533] = {
		.memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_FIRE_WORM)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[534] = {
		.memberIds = {ENEMY_ROW(ENEMY_GOLEM), ENEMY_ROW(ENEMY_WIGHT)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[535] = {
		.memberIds = {ENEMY_ROW(ENEMY_GNOME_MAGE), ENEMY_ROW(ENEMY_CLAY_GARGOYLE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[536] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRESSIL), ENEMY_ROW(ENEMY_DREAD_HOUND)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[537] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRESSIL), ENEMY_ROW(ENEMY_WOOD_WALKER)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[538] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOOD_WALKER)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[539] = {0},
	[540] = {0},
	// The Islet Cave's halls.
	[541] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARRIDAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[542] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAGICORE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[543] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAGICORE), ENEMY_ROW(ENEMY_STONE_SOLDIER)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[544] = {
		.memberIds = {ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[545] = {
		.memberIds = {ENEMY_ROW(ENEMY_SQUIRRELFANG), ENEMY_ROW(ENEMY_HARRIDAN)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[546] = {
		.memberIds = {ENEMY_ROW(ENEMY_SQUIRRELFANG), ENEMY_ROW(ENEMY_MAGICORE)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[547] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINOX), ENEMY_ROW(ENEMY_SQUIRRELFANG)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[548] = {
		.memberIds = {ENEMY_ROW(ENEMY_DINOX)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[549] = {0},
	[550] = {0},
	// Treasure Isle, short of the rock that Lift raises.
	[551] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARRIDAN)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[552] = {
		.memberIds = {ENEMY_ROW(ENEMY_TURTLE_DRAGON)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[553] = {
		.memberIds = {ENEMY_ROW(ENEMY_STONE_SOLDIER)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[554] = {
		.memberIds = {ENEMY_ROW(ENEMY_HORNED_GHOST), ENEMY_ROW(ENEMY_CANNIBAL_GHOUL)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[555] = {
		.memberIds = {ENEMY_ROW(ENEMY_HARRIDAN), ENEMY_ROW(ENEMY_TURTLE_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[556] = {
		.memberIds = {ENEMY_ROW(ENEMY_STONE_SOLDIER)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[557] = {
		.memberIds = {ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_CANNIBAL_GHOUL)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[558] = {
		.memberIds = {ENEMY_ROW(ENEMY_EARTH_GOLEM), ENEMY_ROW(ENEMY_STONE_SOLDIER)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[559] = {0},
	[560] = {0},
	// The Shaman Village Cave.
	[561] = {
		.memberIds = {ENEMY_ROW(ENEMY_TALON_RUNNER)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[562] = {
		.memberIds = {ENEMY_ROW(ENEMY_WARGOLD), ENEMY_ROW(ENEMY_WOLFKIN)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[563] = {
		.memberIds = {ENEMY_ROW(ENEMY_WARGOLD), ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[564] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN), ENEMY_ROW(ENEMY_WARGOLD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[565] = {
		.memberIds = {ENEMY_ROW(ENEMY_TALON_RUNNER), ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[566] = {
		.memberIds = {ENEMY_ROW(ENEMY_SLAYER), ENEMY_ROW(ENEMY_WOLFKIN)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[567] = {
		.memberIds = {ENEMY_ROW(ENEMY_SLAYER), ENEMY_ROW(ENEMY_WARGOLD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[568] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_GRYPHON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[569] = {0},
	[570] = {0},
	// The Trial Road.
	[571] = {
		.memberIds = {ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[572] = {
		.memberIds = {ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[573] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN), ENEMY_ROW(ENEMY_WARGOLD)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[574] = {
		.memberIds = {ENEMY_ROW(ENEMY_WOLFKIN), ENEMY_ROW(ENEMY_MOLE_MAGE)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[575] = {
		.memberIds = {ENEMY_ROW(ENEMY_WARGOLD), ENEMY_ROW(ENEMY_WOLFKIN)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[576] = {
		.memberIds = {ENEMY_ROW(ENEMY_WARGOLD), ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[577] = {
		.memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE), ENEMY_ROW(ENEMY_PTERANODON)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[578] = {
		.memberIds = {ENEMY_ROW(ENEMY_NIGHTMARE)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[579] = {0},
	[580] = {0},
	// The Northern Reaches.
	[581] = {
		.memberIds = {ENEMY_ROW(ENEMY_SPIRAL_SHELL)},
		.minCounts = {4},
		.maxCounts = {5},
	},
	[582] = {
		.memberIds = {ENEMY_ROW(ENEMY_HYDRA)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[583] = {
		.memberIds = {ENEMY_ROW(ENEMY_TURTLE_DRAGON)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[584] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRAND_CHIMERA)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[585] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRAND_CHIMERA), ENEMY_ROW(ENEMY_TURTLE_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[586] = {
		.memberIds = {ENEMY_ROW(ENEMY_PHOENIX)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[587] = {
		.memberIds = {ENEMY_ROW(ENEMY_PHOENIX), ENEMY_ROW(ENEMY_GRAND_CHIMERA)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[588] = {
		.memberIds = {ENEMY_ROW(ENEMY_PHOENIX), ENEMY_ROW(ENEMY_TURTLE_DRAGON)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[589] = {0},
	[590] = {0},
	// Treasure Isle past the rock that Lift raises, the scene where Gale waits.
	[591] = {
		.memberIds = {ENEMY_ROW(ENEMY_SEA_HEDGEHOG)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[592] = {
		.memberIds = {ENEMY_ROW(ENEMY_GREAT_SEAGULL), ENEMY_ROW(ENEMY_SEA_HEDGEHOG)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[593] = {
		.memberIds = {ENEMY_ROW(ENEMY_GILLMAN_LORD), ENEMY_ROW(ENEMY_PUPPET_WARRIOR)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[594] = {
		.memberIds = {ENEMY_ROW(ENEMY_PUPPET_WARRIOR)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[595] = {
		.memberIds = {ENEMY_ROW(ENEMY_PYRODRA), ENEMY_ROW(ENEMY_GREAT_SEAGULL)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[596] = {
		.memberIds = {ENEMY_ROW(ENEMY_PYRODRA), ENEMY_ROW(ENEMY_SEA_HEDGEHOG)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[597] = {
		.memberIds = {ENEMY_ROW(ENEMY_OCEAN_DRAGON), ENEMY_ROW(ENEMY_PYRODRA)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[598] = {
		.memberIds = {ENEMY_ROW(ENEMY_OCEAN_DRAGON)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[599] = {0},
	[600] = {0},
	// The Yampi Desert Cave.
	[601] = {
		.memberIds = {ENEMY_ROW(ENEMY_SAND_SCORPION)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[602] = {
		.memberIds = {ENEMY_ROW(ENEMY_WINGED_LIZARD)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[603] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOS_KNIGHT), ENEMY_ROW(ENEMY_SAND_SCORPION)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[604] = {
		.memberIds = {ENEMY_ROW(ENEMY_MINOS_KNIGHT), ENEMY_ROW(ENEMY_WINGED_LIZARD)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[605] = {
		.memberIds = {ENEMY_ROW(ENEMY_SOUL_ARMY), ENEMY_ROW(ENEMY_SAND_SCORPION)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[606] = {
		.memberIds = {ENEMY_ROW(ENEMY_SOUL_ARMY)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[607] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_DRAGON), ENEMY_ROW(ENEMY_WINGED_LIZARD)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[608] = {
		.memberIds = {ENEMY_ROW(ENEMY_FIRE_DRAGON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[609] = {0},
	[610] = {0},
	// The Islet Cave's corridors.
	[611] = {
		.memberIds = {ENEMY_ROW(ENEMY_CHIMERA_WORM)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[612] = {
		.memberIds = {ENEMY_ROW(ENEMY_CHIMERA_WORM), ENEMY_ROW(ENEMY_DRUJ)},
		.minCounts = {1},
		.maxCounts = {2, 1},
	},
	[613] = {
		.memberIds = {ENEMY_ROW(ENEMY_WONDER_BIRD), ENEMY_ROW(ENEMY_CHIMERA_WORM)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[614] = {
		.memberIds = {ENEMY_ROW(ENEMY_WONDER_BIRD)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[615] = {
		.memberIds = {ENEMY_ROW(ENEMY_DRUJ), ENEMY_ROW(ENEMY_CHIMERA_WORM)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[616] = {
		.memberIds = {ENEMY_ROW(ENEMY_DRUJ), ENEMY_ROW(ENEMY_CRUEL_DRAGON)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[617] = {
		.memberIds = {ENEMY_ROW(ENEMY_CRUEL_DRAGON), ENEMY_ROW(ENEMY_WONDER_BIRD)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[618] = {
		.memberIds = {ENEMY_ROW(ENEMY_CRUEL_DRAGON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[619] = {0},
	[620] = {0},
	// The Anemos Inner Sanctum.
	[621] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRAVE_WIGHT)},
		.minCounts = {1},
		.maxCounts = {3},
	},
	[622] = {
		.memberIds = {ENEMY_ROW(ENEMY_GRAVE_WIGHT), ENEMY_ROW(ENEMY_BOMBANDER)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[623] = {
		.memberIds = {ENEMY_ROW(ENEMY_BOMBANDER), ENEMY_ROW(ENEMY_SKY_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[624] = {
		.memberIds = {ENEMY_ROW(ENEMY_BOMBANDER), ENEMY_ROW(ENEMY_MAD_DEMON)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[625] = {
		.memberIds = {ENEMY_ROW(ENEMY_SKY_DRAGON), ENEMY_ROW(ENEMY_GRAVE_WIGHT)},
		.minCounts = {1},
		.maxCounts = {1, 2},
	},
	[626] = {
		.memberIds = {ENEMY_ROW(ENEMY_SKY_DRAGON)},
		.minCounts = {1},
		.maxCounts = {2},
	},
	[627] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_DEMON), ENEMY_ROW(ENEMY_SKY_DRAGON)},
		.minCounts = {1},
		.maxCounts = {1, 1},
	},
	[628] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_DEMON)},
		.minCounts = {2},
		.maxCounts = {2},
	},
	[629] = {0},
	[630] = {0},
	// The guardians of four hidden places (HIDDEN_BOSS): the Star Magician at the end of Treasure Isle, Valukar in the Yampi Desert Cave, the Sentinel at the end of the Islet Cave and the Dullahan, the shadow at the Anemos Inner Sanctum's entrance.
	[631] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_STAR_MAGICIAN), ENEMY_ROW(ENEMY_REFRESH_BALL), ENEMY_ROW(ENEMY_GUARDIAN_BALL), ENEMY_ROW(ENEMY_THUNDER_BALL), ENEMY_ROW(ENEMY_ANGER_BALL)},
		.minCounts = {1, 1, 1, 1, 1},
		.maxCounts = {1, 1, 1, 1, 1},
	},
	[632] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_VALUKAR)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[633] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_SENTINEL)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[634] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_DULLAHAN)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	// Event battles: Navampa, Azart and Satrage in the Shaman Village Cave, the World Map's bandits, the Wild Wolf of Echo's Djinn lesson on the World Map, and Karst and Agatio against Isaac's party in Jupiter Lighthouse.
	[635] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_NAVAMPA_SHAMAN_VILLAGE_CAVE), ENEMY_ROW(ENEMY_AZART_SHAMAN_VILLAGE_CAVE), ENEMY_ROW(ENEMY_SATRAGE_SHAMAN_VILLAGE_CAVE)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[636] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_BANDIT_WORLD_MAP), ENEMY_ROW(ENEMY_THIEF_WORLD_MAP), ENEMY_ROW(ENEMY_THIEF_WORLD_MAP)},
		.minCounts = {1, 1, 1},
		.maxCounts = {1, 1, 1},
	},
	[637] = {
		.memberIds = {ENEMY_ROW(ENEMY_WILD_WOLF)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[FORMATION_KARST_AND_AGATIO_VS_ISAAC] = {
		.layout = FORMATION_LAYOUT_CENTERED,
		.memberIds = {ENEMY_ROW(ENEMY_AGATIO_VS_ISAAC), ENEMY_ROW(ENEMY_KARST_VS_ISAAC)},
		.minCounts = {1, 1},
		.maxCounts = {1, 1},
	},
	[639] = {0},
	[640] = {0},
	// The hidden Mad Plants, in FLAG_MAD_PLANT's order.
	[641] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_GABOMBA_CATACOMBS)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[642] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_TAOPO_SWAMP)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[643] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_GAIA_ROCK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[644] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_JUPITER_LIGHTHOUSE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[645] = {
		.memberIds = {ENEMY_ROW(ENEMY_MAD_PLANT_TRIAL_ROAD)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[646] = {0},
	[647] = {0},
	[648] = {0},
	[649] = {0},
	[650] = {0},
	// The Mimics, by the place each waits in.
	[651] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_KANDOREAN_TEMPLE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[652] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_AIRS_ROCK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[653] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_GABOMBA_STATUE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[654] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_AQUA_ROCK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[655] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_GAIA_ROCK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[656] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_TREASURE_ISLE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[657] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_JUPITER_LIGHTHOUSE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[658] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_MAGMA_ROCK)},
		.minCounts = {1},
		.maxCounts = {1},
	},
	[659] = {
		.memberIds = {ENEMY_ROW(ENEMY_MIMIC_MARS_LIGHTHOUSE)},
		.minCounts = {1},
		.maxCounts = {1},
	},
};

// The 87 enemies whose defeated flags a monster battle clears before it picks a formation (btFormationSelectLevelMatchedCandidate).
Uint16 BattleFormationExcludedEnemies[] = {
	ENEMY_VENUS_DJINNI_UNUSED_QUARTZ,
	ENEMY_VENUS_DJINNI_UNUSED_VINE,
	ENEMY_VENUS_DJINNI_UNUSED_BANE,
	ENEMY_VENUS_DJINNI_UNUSED_ECHO,
	ENEMY_VENUS_DJINNI_IRON,
	ENEMY_VENUS_DJINNI_STEEL,
	ENEMY_VENUS_DJINNI_MUD,
	ENEMY_VENUS_DJINNI_FLOWER,
	ENEMY_VENUS_DJINNI_MELD,
	ENEMY_VENUS_DJINNI_PETRA,
	ENEMY_VENUS_DJINNI_UNUSED_SALT,
	ENEMY_VENUS_DJINNI_UNUSED_GEODE,
	ENEMY_VENUS_DJINNI_UNUSED_MOLD,
	ENEMY_VENUS_DJINNI_CRYSTAL,
	ENEMY_MERCURY_DJINNI_UNUSED_SLEET,
	ENEMY_MERCURY_DJINNI_UNUSED_SPRITZ,
	ENEMY_MERCURY_DJINNI_UNUSED_HAIL,
	ENEMY_MERCURY_DJINNI_UNUSED_DEW,
	ENEMY_MERCURY_DJINNI_FOG,
	ENEMY_MERCURY_DJINNI_SOUR,
	ENEMY_MERCURY_DJINNI_UNUSED_SPRING,
	ENEMY_MERCURY_DJINNI_UNUSED_SHADE,
	ENEMY_MERCURY_DJINNI_CHILL,
	ENEMY_MERCURY_DJINNI_STEAM,
	ENEMY_MERCURY_DJINNI_UNUSED_RIME,
	ENEMY_MERCURY_DJINNI_UNUSED_GEL,
	ENEMY_MERCURY_DJINNI_EDDY,
	ENEMY_MERCURY_DJINNI_BALM,
	ENEMY_MERCURY_DJINNI_SERAC,
	ENEMY_MARS_DJINNI_UNUSED_FORGE,
	ENEMY_MARS_DJINNI_UNUSED_CORONA,
	ENEMY_MARS_DJINNI_UNUSED_FLASH,
	ENEMY_MARS_DJINNI_CANNON,
	ENEMY_MARS_DJINNI_UNUSED_SPARK,
	ENEMY_MARS_DJINNI_KINDLE,
	ENEMY_MARS_DJINNI_UNUSED_CHAR,
	ENEMY_MARS_DJINNI_UNUSED_COAL,
	ENEMY_MARS_DJINNI_REFLUX,
	ENEMY_MARS_DJINNI_CORE,
	ENEMY_MARS_DJINNI_UNUSED_TINDER,
	ENEMY_MARS_DJINNI_UNUSED_SHINE,
	ENEMY_MARS_DJINNI_FURY,
	ENEMY_MARS_DJINNI_FUGUE,
	ENEMY_JUPITER_DJINNI_UNUSED_BREEZE,
	ENEMY_JUPITER_DJINNI_UNUSED_ZEPHYR,
	ENEMY_JUPITER_DJINNI_UNUSED_SQUALL,
	ENEMY_JUPITER_DJINNI_UNUSED_LUFF,
	ENEMY_JUPITER_DJINNI_BREATH,
	ENEMY_JUPITER_DJINNI_BLITZ,
	ENEMY_JUPITER_DJINNI_UNUSED_ETHER,
	ENEMY_JUPITER_DJINNI_WAFT,
	ENEMY_JUPITER_DJINNI_UNUSED_HAZE,
	ENEMY_JUPITER_DJINNI_WHEEZE,
	ENEMY_JUPITER_DJINNI_UNUSED_AROMA,
	ENEMY_JUPITER_DJINNI_WHORL,
	ENEMY_JUPITER_DJINNI_GASP,
	ENEMY_JUPITER_DJINNI_UNUSED_LULL,
	ENEMY_JUPITER_DJINNI_GALE,
	ENEMY_SERPENT,
	ENEMY_SERPENT_ONE_BEAM,
	ENEMY_SERPENT_TWO_BEAMS,
	ENEMY_SERPENT_THREE_BEAMS,
	ENEMY_POSEIDON_SECOND_ACTION,
	ENEMY_MOAPA,
	ENEMY_MOAPA_B,
	ENEMY_KNIGHT,
	ENEMY_AGATIO,
	ENEMY_AGATIO_VS_ISAAC,
	ENEMY_KARST,
	ENEMY_KARST_VS_ISAAC,
	ENEMY_DULLAHAN_B,
	ENEMY_DULLAHAN_C,
	ENEMY_AZART_PADDED_GLOVES,
	ENEMY_SATRAGE_COTTON_SHIRT,
	ENEMY_NAVAMPA_HUNTERS_SWORD,
	ENEMY_MYSTERY_MAN,
	ENEMY_MYSTERY_WOMAN,
	ENEMY_ZOMBIE_DJINN_LESSON,
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

// Unused: a 0 after the list that no code reads.
Uint16 BattleFormationUnusedListEnd[] = {0};
// clang-format on
