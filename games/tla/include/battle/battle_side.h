#ifndef ALCHEMY_BATTLE_SIDE_H
#define ALCHEMY_BATTLE_SIDE_H

/* @file */
/* The two sides of a battle, their unit lists, and the Djinn each side has on
 */
/* standby or recovering. */

#include "edition.h"
#include "static_assert.h"

/* UNIT.side: which side a unit fights on, if it is in the battle. */
typedef enum BATTLE_SIDE {
	BATTLE_SIDE_NONE = 0,
	BATTLE_SIDE_ENEMY = 1,
	BATTLE_SIDE_PARTY = 2
} BATTLE_SIDE;

/* Marks in a list of unit ids, such as BATTLE_STATE.partyUnits and enemyIds. */
typedef enum BATTLE_UNIT_LIST_MARK {
	BATTLE_UNIT_REMOVED = 254, /* a place whose unit has left the battle */
	BATTLE_UNIT_LIST_END = 255 /* ends the list */
} BATTLE_UNIT_LIST_MARK;

/* Which sides a unit list takes. */
typedef enum BATTLE_SIDE_MASK {
	BATTLE_SIDE_MASK_PARTY = 1,
	BATTLE_SIDE_MASK_ENEMIES = 2,
	BATTLE_SIDE_MASK_BOTH = BATTLE_SIDE_MASK_PARTY | BATTLE_SIDE_MASK_ENEMIES
} BATTLE_SIDE_MASK;

/* The order the two sides take their turn-end effects in. */
typedef struct BATTLE_SIDE_ORDER {
	s32 group[2]; /* BATTLE_SIDE_MASK: the party, then the enemies */
} BATTLE_SIDE_ORDER;

/* The Djinn recovery list's two tables (gmDjinnGetRecoveryTableStub): the */
/* party's and the link opponent's. */
#define DJINN_RECOVERY_PARTY	0
#define DJINN_RECOVERY_OPPONENT 1

/* DJINN_RECOVERY_ENTRY.turns of a Djinni on standby. In battle it stays so */
/* until a Djinn command sets it again or a summon uses it */
/* (DJINN_RECOVERY_SUMMONED), both in btActionRunBuild. */
#define DJINN_RECOVERY_AFTER_BATTLE -1

/* DJINN_RECOVERY_ENTRY.turns of a standby Djinni a summon has just used, until
 */
/* the summoner's action ends and sets its count of turns to recover. */
#define DJINN_RECOVERY_SUMMONED -2

/* A Djinni on standby or recovering: its element and index, its owner, and */
/* the turns until it is set again. */
typedef struct DJINN_RECOVERY_ENTRY {
	u8 element; /* ELEMENT */
	u8 idx;		/* the Djinni within its element */
	u8 unitId;	/* its owner */
	s8 turns;	/* turns until it is set again; -1 on standby, -2 in use */
} DJINN_RECOVERY_ENTRY;

STATIC_ASSERT(sizeof(DJINN_RECOVERY_ENTRY) == 4);

/* One side's Djinn recovery list: up to 72 entries and their count. */
typedef struct DJINN_RECOVERY_LIST {
	DJINN_RECOVERY_ENTRY entries[72];
	s32 cnt; /* entries in use */
} DJINN_RECOVERY_LIST;

/* One side's Djinn recovery table, as gmDjinnGetRecoveryTable returns it: the
 */
/* party's inside MainRoutineStateWork from summonMask on, the link opponent's
 */
/* in the fourth enemy unit (FIRST_ENEMY_UNIT + 3), which an opponent of three
 */
/* never fills. */
typedef struct DJINN_RECOVERY_TABLE {
	/* The summons the side can call, bit n for summon n (summon_table.c). */
	u32 summonMask;
	u32 partyCoins; /* in the party's table, MainRoutineStateWork.coins */
	DJINN_RECOVERY_LIST list;
} DJINN_RECOVERY_TABLE;

s32 btEnemyList(s16 *unitIds);
s32 btPartyAssignOpponentSlots(void);

s32 btUnitRollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias);
s32 btUnitCountDownAttackStage(s32 unitId);
s32 btUnitCountDownDefenseStage(s32 unitId);
s32 btUnitCountDownResistanceStage(s32 unitId);
s32 btUnitCountDownDelusion(s32 unitId);
s32 btUnitCountDownConfusion(s32 unitId);
s32 btUnitCountDownCharm(s32 unitId);
s32 btUnitCountDownStun(s32 unitId);
s32 btUnitCountDownSleep(s32 unitId);
s32 btUnitCountDownPsynergySeal(s32 unitId);
s32 btUnitCountDownReflect(s32 unitId);
s32 btUnitCountDownAgilityStage(s32 unitId);

#endif
