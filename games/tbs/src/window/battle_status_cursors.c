/* The Status screen's cursor stops. */

#include "window/battle_status.h"

/* clang-format off */
/* Rows 0 to 8 with Djinn in the party, 9 to 15 without, which leave out the */
/* class and Djinn stops: {help, column, row, highlight column, highlight row, */
/* highlight width}. */
const BATTLE_SUMMARY_STAT_CURSOR BattleStatusScreenStatCursors[] = {
	{OWNER_STATUS_HELP_NAME,          2,  0,  0, 0,  12},
	{OWNER_STATUS_HELP_EXPERIENCE,    8,  1,  0, 1,  13},
	{OWNER_STATUS_HELP_HP,            12, 3,  5, 3,  11},
	{OWNER_STATUS_HELP_PP,            12, 4,  5, 4,  11},
	{OWNER_STATUS_HELP_CLASS,         3,  6,  0, 6,  10},
	{OWNER_STATUS_HELP_DJINN,         13, 9,  0, 9,  20},
	{OWNER_STATUS_HELP_ELEMENT_LEVEL, 13, 10, 0, 10, 20},
	{OWNER_STATUS_HELP_POWER,         13, 11, 0, 11, 20},
	{OWNER_STATUS_HELP_RESIST,        13, 12, 0, 12, 20},
	{OWNER_STATUS_HELP_NAME,          2,  0,  0, 0,  12},
	{OWNER_STATUS_HELP_EXPERIENCE,    8,  1,  0, 1,  13},
	{OWNER_STATUS_HELP_HP,            12, 3,  5, 3,  11},
	{OWNER_STATUS_HELP_PP,            12, 4,  5, 4,  11},
	{OWNER_STATUS_HELP_ELEMENT_LEVEL, 13, 10, 0, 10, 20},
	{OWNER_STATUS_HELP_POWER,         13, 11, 0, 11, 20},
	{OWNER_STATUS_HELP_RESIST,        13, 12, 0, 12, 20},
};

/* {help, column, row} */
const BATTLE_SUMMARY_CURSOR BattleStatusScreenEquipmentCursors[] = {
	{OWNER_STATUS_HELP_ATTACK,  19, 1},
	{OWNER_STATUS_HELP_DEFENSE, 19, 2},
	{OWNER_STATUS_HELP_AGILITY, 19, 3},
	{OWNER_STATUS_HELP_LUCK,    19, 4},
};

/* The eight status icons' places: {help, column, row} */
const BATTLE_SUMMARY_CURSOR BattleStatusScreenConditionCursors[] = {
	{OWNER_STATUS_HELP_CONDITION + 0, 13, 0},
	{OWNER_STATUS_HELP_CONDITION + 1, 15, 0},
	{OWNER_STATUS_HELP_CONDITION + 2, 17, 0},
	{OWNER_STATUS_HELP_CONDITION + 3, 19, 0},
	{OWNER_STATUS_HELP_CONDITION + 4, 21, 0},
	{OWNER_STATUS_HELP_CONDITION + 5, 23, 0},
	{OWNER_STATUS_HELP_CONDITION + 6, 25, 0},
	{OWNER_STATUS_HELP_LAST_CONDITION, 27, 0},
};
/* clang-format on */
