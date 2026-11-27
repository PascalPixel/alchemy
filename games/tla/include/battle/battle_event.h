#ifndef ALCHEMY_BATTLE_EVENT_H
#define ALCHEMY_BATTLE_EVENT_H

/* The battle event queue: the names, numbers, messages, hits and defeats an
   action queues for its playback to show in order. */

#include "edition.h"
#include "static_assert.h"

/* The most events a playback queues (BATTLE_PLAYBACK_STATE's opcodes and
   operands). */
enum { BATTLE_PLAYBACK_CAPACITY = 64 };

/* BATTLE_PLAYBACK_STATE.phase: how far btPlaybackRun has got through the plan's
   targets. */
typedef enum BATTLE_PLAYBACK_PHASE {
	BATTLE_PLAYBACK_PHASE_IDLE = 0, /* paused until the effect resumes it */
	/* resolves the next target's events */
	BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET = 1,
	BATTLE_PLAYBACK_PHASE_DISPATCH = 2, /* plays the queued events in order */
	BATTLE_PLAYBACK_PHASE_READY = 4		/* every target has been played */
} BATTLE_PLAYBACK_PHASE;

/* The battle event queue: up to 64 events, each an opcode (enum
   BATTLE_PLAYBACK_COMMAND) and one operand, in the order they play. */
typedef struct BATTLE_PLAYBACK_QUEUE {
	Uint8  opcodes[BATTLE_PLAYBACK_CAPACITY];
	Uint32 operands[BATTLE_PLAYBACK_CAPACITY];
	Sint32 tgtIdx; /* the plan's next target to resolve */
	Sint32 cnt;	   /* events queued */
} BATTLE_PLAYBACK_QUEUE;

STATIC_ASSERT(sizeof(BATTLE_PLAYBACK_QUEUE) == 328);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_QUEUE, operands) == 64);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_QUEUE, tgtIdx) == 320);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_QUEUE, cnt) == 324);

/* The battle events btPlaybackPush queues, each with one operand, for
   btPlaybackRun (or btPlaybackRunAll) to play in order. The names, numbers and
   messages build what the player reads: the arguments first, then the message
   that shows them. */
typedef enum BATTLE_PLAYBACK_COMMAND {
	PLAYBACK_UNIT_NAME = 0,		  /* a unit's name for the next message */
	PLAYBACK_NUMBER = 1,		  /* a number for the next message */
	PLAYBACK_ITEM_NAME = 2,		  /* an item's name for the next message */
	PLAYBACK_ABILITY_NAME = 3,	  /* an ability's name for the next message */
	PLAYBACK_MESSAGE = 4,		  /* a message, waiting for the player */
	PLAYBACK_MESSAGE_NO_WAIT = 5, /* a message, going straight on */
	PLAYBACK_MARK = 6,			  /* sets BattleDisplayState->marked */
	PLAYBACK_CLEAR_ARGUMENTS = 7, /* forgets the names and numbers pushed */
	PLAYBACK_HIT = 8,			  /* the unit flinches and flashes */
	PLAYBACK_DEFEAT = 9,		  /* the unit falls; an enemy fades out */
	PLAYBACK_DRAW_STATUS = 10,	  /* redraws the party's status */
	PLAYBACK_REFRESH_UNIT = 11,	  /* the unit's status icons, colour, pose */
	PLAYBACK_REMOVE_UNIT = 12,	  /* an enemy leaves the battle */
	PLAYBACK_DJINN_BONUS = 13,	  /* later defeats give the Djinn bonus */
	PLAYBACK_SOUND = 14,		  /* plays a sound */
	PLAYBACK_BLOW_AWAY = 15 /* the unit leaves the lists, as one that flees */
} BATTLE_PLAYBACK_COMMAND;

Sint32 btUnitRemoveEnemy(Sint32 unitId);

void btPlaybackPush(Uint32 opcode, Uint32 operand);

/* Queues a message that waits for the player. */
#define PUSH_MESSAGE(message) \
	btPlaybackPush(PLAYBACK_MESSAGE, (Uint32)(message))
Uint32 btPlaybackRunAll(void);

#endif
