#ifndef ALCHEMY_BATTLE_ACTION_H
#define ALCHEMY_BATTLE_ACTION_H

/* A battle's battle objects and sprites, each unit's command and action, and
   the spoils. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

struct ACTOR;
struct SPR_ANIMATION;

/* A unit's place on the battlefield: its actor, resources and animations, its
   anchor spot and scale, and the statuses it shows. BATTLE_STATE.objs holds the
   14, reached by unit id through btObjectGet; btObjectMake fills one in when
   the unit is placed. */
typedef struct BATTLE_OBJECT {
	struct ACTOR *actor;	/* NULL once released */
	Uint16		  resource; /* the unit's sprite */
	Uint16 overlay; /* a second sprite drawn over it, the weapon; 0 for none */
	Uint16 anm;		/* the status icon's animation shown; 0 for none */
	Uint16 shadow; /* the shadow sprite under the unit (SPRITE_BATTLE_SHADOW) */
	FIXED  anchorX; /* 16.16 fixed-point pixels */
	FIXED  anchorZ; /* 16.16 fixed-point pixels */
	/* The unit sprite's SPR_COL_MODE: an enemy's own, 1 for a character on the
	   enemy side (a link opponent), else 0. */
	Sint32 pal;
	FIXED  scale;		   /* 16.16 fixed point, toFIXED(1) full size */
	Sint16 activeStatuses; /* a bit per status with an icon (ICON_) */
	Uint8  shownStat;	   /* the status whose icon is shown */
	Sint8  cycleTmr;	   /* ticks until the next status icon */
	struct SPR_ANIMATION *anmEntry;	   /* the status icon's sprite, or NULL */
	struct SPR_ANIMATION *shadowEntry; /* the shadow's sprite, or NULL */
	/* Set by btObjectMarkForRelease: its actor is released with the others by
	   btObjectDestroyAll. */
	Sint16 active;
	Sint16 fading; /* set while a fallen enemy's picture is being removed */
} BATTLE_OBJECT;

/* BATTLE_ACTION.command and BATTLE_ACTION_RUN's copy of it: what a unit does
   when it acts. */
typedef enum BATTLE_COMMAND {
	COMMAND_ATTACK = 0,
	COMMAND_PSYNERGY = 1,
	COMMAND_ITEM = 2,
	COMMAND_DEFEND = 3,
	COMMAND_ENEMY_ABILITY = 4,
	COMMAND_DJINN = 5,
	COMMAND_SUMMON = 6,
	/* An enemy's Total Defense (ABILITY_EFFECT_TOTAL_DEFENSE). */
	COMMAND_TOTAL_DEFENSE = 7,
	COMMAND_NONE = 8, /* nothing this turn */
	COMMAND_FLEE = 99
} BATTLE_COMMAND;

/* COMMAND_DJINN's parameter: the Djinni's element in bits 11 to 8, its number
   within the element below. */
#define DJINN_COMMAND(element, index)	 (((element) << 8) | (index))
#define DJINN_COMMAND_ELEMENT(parameter) (((parameter) >> 8) & 15)
#define DJINN_COMMAND_INDEX(parameter)	 ((parameter) & 255)

/* A unit's chosen action for the turn: its command, the command's parameter,
   its target and range, and its priority in the action order.
   BATTLE_STATE.actions holds the turn's actions. */
typedef struct BATTLE_ACTION {
	Sint16 unitId;
	Uint16 slot;	/* in a link battle, unitId as the opponent knows it */
	Sint16 pri;		/* from the unit's Agility; higher acts first */
	Sint16 command; /* BATTLE_COMMAND */
	/* an ability id, or COMMAND_ITEM's inventory index (-1 gone) */
	Sint16 prm;
	Sint16 tgt;	   /* the aimed unit's id */
	Sint16 range;  /* the action's range */
	Sint16 unused; /* no code names it */
} BATTLE_ACTION;

/* A battle's spoils so far: coins, experience, enemies defeated and up to four
   items. BATTLE_STATE.spoils, cleared as the battle starts and added to as each
   enemy falls. */
typedef struct BATTLE_SPOILS {
	Sint32 coins;
	Sint32 experience;
	Sint32 defeated; /* enemies defeated */
	/* Item ids dropped, 0 for an empty place; each id once, and a fifth drop
	   replaces the lowest ranked (btSpoilsRankItem) if it ranks higher. */
	Uint16 items[4];
} BATTLE_SPOILS;

STATIC_ASSERT(sizeof(BATTLE_OBJECT) == 44);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, anchorX) == 12);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, activeStatuses) == 28);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, cycleTmr) == 31);
STATIC_ASSERT(offsetof(BATTLE_OBJECT, active) == 40);

STATIC_ASSERT(sizeof(BATTLE_ACTION) == 16);
STATIC_ASSERT(offsetof(BATTLE_ACTION, pri) == 4);
STATIC_ASSERT(offsetof(BATTLE_ACTION, command) == 6);
STATIC_ASSERT(offsetof(BATTLE_ACTION, prm) == 8);
STATIC_ASSERT(offsetof(BATTLE_ACTION, tgt) == 10);
STATIC_ASSERT(sizeof(BATTLE_SPOILS) == 20);

#endif
