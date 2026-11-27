/* The 16 summons: the standby Djinn each one needs, and the order the summon
   menus list them in. */

#include "ability_ids.h"
#include "game/game.h"

/* The menus list the summons in the order of their ids, Venus first and Thor
   last. */
const Uint8 SummonOrderList[16] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

/* Each summon's ability and its cost in standby Djinn of each element: one
   Djinn of an element calls its first summon, four call Judgment, Boreas,
   Meteor or Thor. */
/* clang-format off */
const SUMMON_DEFINITION SummonDefinitionTable[] = {
	/* Venus */
	{
		.abilityId = ABILITY_VENUS,
		.djinnRequired = {[ELEMENT_VENUS] = 1},
	},
	/* Mercury */
	{
		.abilityId = ABILITY_MERCURY,
		.djinnRequired = {[ELEMENT_MERCURY] = 1},
	},
	/* Mars */
	{
		.abilityId = ABILITY_MARS,
		.djinnRequired = {[ELEMENT_MARS] = 1},
	},
	/* Jupiter */
	{
		.abilityId = ABILITY_JUPITER,
		.djinnRequired = {[ELEMENT_JUPITER] = 1},
	},
	/* Ramses */
	{
		.abilityId = ABILITY_RAMSES,
		.djinnRequired = {[ELEMENT_VENUS] = 2},
	},
	/* Nereid */
	{
		.abilityId = ABILITY_NEREID,
		.djinnRequired = {[ELEMENT_MERCURY] = 2},
	},
	/* Kirin */
	{
		.abilityId = ABILITY_KIRIN,
		.djinnRequired = {[ELEMENT_MARS] = 2},
	},
	/* Atalanta */
	{
		.abilityId = ABILITY_ATALANTA,
		.djinnRequired = {[ELEMENT_JUPITER] = 2},
	},
	/* Cybele */
	{
		.abilityId = ABILITY_CYBELE,
		.djinnRequired = {[ELEMENT_VENUS] = 3},
	},
	/* Neptune */
	{
		.abilityId = ABILITY_NEPTUNE,
		.djinnRequired = {[ELEMENT_MERCURY] = 3},
	},
	/* Tiamat */
	{
		.abilityId = ABILITY_TIAMAT,
		.djinnRequired = {[ELEMENT_MARS] = 3},
	},
	/* Procne */
	{
		.abilityId = ABILITY_PROCNE,
		.djinnRequired = {[ELEMENT_JUPITER] = 3},
	},
	/* Judgment */
	{
		.abilityId = ABILITY_JUDGMENT,
		.djinnRequired = {[ELEMENT_VENUS] = 4},
	},
	/* Boreas */
	{
		.abilityId = ABILITY_BOREAS,
		.djinnRequired = {[ELEMENT_MERCURY] = 4},
	},
	/* Meteor */
	{
		.abilityId = ABILITY_METEOR,
		.djinnRequired = {[ELEMENT_MARS] = 4},
	},
	/* Thor */
	{
		.abilityId = ABILITY_THOR,
		.djinnRequired = {[ELEMENT_JUPITER] = 4},
	},
};
/* clang-format on */
