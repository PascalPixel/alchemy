#ifndef ALCHEMY_FORMATIONS_H
#define ALCHEMY_FORMATIONS_H

/* @file */
/* The enemy formations. */

#include "types.h"

/* FORMATION.layout and BATTLE_STATE.layout: the order */
/* btFormationBuildEnemyList draws a formation's enemies in, and how */
/* btUnitCollect stands them in their line. The two random orders stand */
/* left to right as drawn; btUnitCollect takes any other value as */
/* FORMATION_LAYOUT_CENTERED. */

/* Each kind's enemies together, the kinds in a random order. A link battle's */
/* opponents stand this way too. */
#define FORMATION_LAYOUT_SHUFFLED 0
/* Unused: each enemy drawn from a random kind with any left; no formation */
/* has it. */
#define FORMATION_LAYOUT_INTERLEAVED 1
/* The formation's own order, its first enemy in the centre and the rest */
/* alternately either side: eleven formations, among them the Bandit with his */
/* two Thieves and Menardi with Saturos. */
#define FORMATION_LAYOUT_CENTERED 2

/* The kinds of enemy a formation may hold. */
#define FORMATION_MEMBERS 5

/* An enemy formation: its battle type and up to five kinds of enemy, each */
/* with the fewest and most of it that appear. btFormationBuildEnemyList */
/* draws a battle's enemies from it. */
typedef struct FORMATION {
	u8 layout;						 /* a FORMATION_LAYOUT_ */
	u8 memberIds[FORMATION_MEMBERS]; /* rows of enemy_table.c (enemy id less */
									 /* ENEMY_ID_FIRST) */
	u8 minCnts[FORMATION_MEMBERS];	 /* each always appears this often */
	/* Each may appear up to this often while the six sprite slots hold them; */
	/* 0 for a member that does not appear. */
	u8 maxCnts[FORMATION_MEMBERS];
} FORMATION;

/* The rows of Formations. */
#define FORMATION_COUNT 380

/* The Fusion Dragon's battle on the Venus Lighthouse Aerie, the one formation
 */
/* the battle code itself tests for: it opens with the storm instead of the */
/* curtain (btTransitionRun). */
#define FORMATION_FUSION_DRAGON 347

/* The formations, by formation id (battle/formations.c). */
extern FORMATION Formations[FORMATION_COUNT];

/* The 20 enemies whose defeated flags a monster battle clears before it picks
 */
/* a formation (battle/formations.c). */
extern u16 BattleFormation_ExcludedEnemies[20];

#endif
