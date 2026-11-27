#ifndef ALCHEMY_BATTLE_ACTION_H
#define ALCHEMY_BATTLE_ACTION_H

/* @file */
/* A battle's battle objects, each unit's command and action, and the spoils. */

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"

struct MOTION_OBJECT;
struct SPR_ANIMATION;

/* A unit's place on the battlefield: its sprite object, resources and */
/* animations, its anchor spot and scale, and the conditions it shows. */
typedef struct BATTLE_OBJECT {
	struct MOTION_OBJECT *obj; /* NULL once released */
	u16 resource;			   /* the unit's sprite */
	u16 overlay;			   /* a second sprite drawn over it; 0 for none */
	u16 anm;			  /* the status icon's animation shown; 0 for none */
	u16 shadow;			  /* the shadow sprite drawn under the unit */
	FIXED anchorX;		  /* 16.16 fixed-point pixels */
	FIXED anchorZ;		  /* 16.16 fixed-point pixels */
	s32 pal;			  /* the unit sprite's colour mode, an SPR_COL_MODE */
	FIXED scale;		  /* 16.16 fixed point, FIXED_ONE full size */
	u16 activeStatuses;	  /* a bit per status with an icon */
	u8 selectedCondition; /* the status whose icon is shown */
	s8 cycleTmr;		  /* frames until the next status icon */
	struct SPR_ANIMATION *anmEntry;	   /* the status icon's sprite, or NULL */
	struct SPR_ANIMATION *shadowEntry; /* the shadow sprite, or NULL */
	/* Set by btObjectMarkForRelease: the object is released with the */
	/* others. */
	s16 active;
	/* The Broken Seal's last halfword, fading, is two bytes here: the battle */
	/* code reads and writes each on its own. */
	u8 fading;	/* set while a fallen enemy's picture is being removed */
	u8 visible; /* 1 from btObjectReset, 0 from btObjectMake */
} BATTLE_OBJECT;

/* BATTLE_ACTION_RECORD.command and BATTLE_PLAN's copy of it: what a unit does
 */
/* on its turn. */
typedef enum BATTLE_COMMAND {
	COMMAND_ATTACK = 0,
	COMMAND_PSYNERGY = 1,
	COMMAND_ITEM = 2,
	COMMAND_DEFEND = 3,
	COMMAND_ABILITY = 4, /* an enemy's ability */
	COMMAND_DJINN = 5,
	COMMAND_SUMMON = 6,
	COMMAND_TOTAL_DEFENSE = 7, /* an enemy's ABILITY_EFFECT_TOTAL_DEFENSE */
	COMMAND_NONE = 8,		   /* nothing this turn */
	/* The ABILITY_DJINN_COUNTER a unit strikes back with, outside the */
	/* turn order (btRun queues it for BATTLE_STATE.pendingUnit). */
	COMMAND_DJINN_COUNTER = 9,
	/* Daedalus's strike again at a turn's end, queued by btRun while */
	/* summons of Daedalus are owed (btStagingPlanDaedalus). */
	COMMAND_DAEDALUS = 10,
	COMMAND_FLEE = 99
} BATTLE_COMMAND;

/* COMMAND_DJINN's parameter: the Djinni's element in bits 11 to 8, its */
/* number within the element below. */
#define DJINN_COMMAND(element, index)	 (((element) << 8) | (index))
#define DJINN_COMMAND_ELEMENT(parameter) (((parameter) >> 8) & 15)
#define DJINN_COMMAND_INDEX(parameter)	 ((parameter) & 255)

/* The list place codes the target pickers return (btFindUnitPlace): a */
/* place in the party's list or the enemies', or'd with the side's tag; */
/* PLACE_PARTY alone is the party's first place, the default target. */
#define PLACE_PARTY 0x100
#define PLACE_ENEMY 0x180

/* A unit's chosen action for the turn: its command, the command's parameter, */
/* its target and range, and its priority in the turn order. */
typedef struct BATTLE_ACTION_RECORD {
	s16 unitId;
	u16 slot;	 /* in a link battle, unitId as the opponent knows it */
	u16 pri;	 /* its place in the turn order */
	s16 command; /* BATTLE_COMMAND */
	s16 prm;	 /* the command's ability, item or Djinni */
	s16 tgt;	 /* the aimed unit's id */
	s16 range;	 /* the action's range */
	/* Which of the unit's actions this turn it is, from 0: Poseidon's and the
	 */
	/* Doom Dragon's forms choose each by it (btAIChooseAction). */
	s16 turnAction;
} BATTLE_ACTION_RECORD;

/* A battle's spoils so far: coins, experience, enemies defeated and up to */
/* four items. */
typedef struct BATTLE_SPOILS {
	s32 coins;
	s32 experience;
	s32 defeated; /* enemies defeated */
	u16 items[4];
	/* The two enemies last felled (btEnemyRecordDefeat), which Regen Dance */
	/* (ABILITY_EFFECT_CALL_QUEUED) calls back: their enemy ids and name
	 * letters, */
	/* the place of the last felled and how many there are. */
	/* @{ */
	u16 queuedSummons[2];
	u8 queuedLetters[2];
	s8 queueHead;
	s8 queueCnt;
	/* @} */
} BATTLE_SPOILS;

STATIC_ASSERT(sizeof(BATTLE_OBJECT) == 44);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, anchorX) == 12);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, activeStatuses) == 28);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, cycleTmr) == 31);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, active) == 40);

STATIC_ASSERT(sizeof(BATTLE_ACTION_RECORD) == 16);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RECORD, pri) == 4);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RECORD, command) == 6);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RECORD, prm) == 8);
STATIC_ASSERT(offsetof(BATTLE_ACTION_RECORD, tgt) == 10);
STATIC_ASSERT(sizeof(BATTLE_SPOILS) == 28);

/* A unit's chosen action as the party's command menus write it: the */
/* BATTLE_ACTION_RECORD, read with its fields unsigned. */
typedef struct BATTLE_ACTION {
	u16 unitId;
	u16 slot;
	s16 pri;
	u16 command;
	u16 prm;
	u16 tgt;
	u16 range;
	u16 turnAction;
} BATTLE_ACTION;

#endif
