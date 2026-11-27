#ifndef ALCHEMY_PLAYBACK_QUEUE_H
#define ALCHEMY_PLAYBACK_QUEUE_H

/* The playback queue: the commands btPlaybackPush queues and the playback
   phases that play them. */

#include "edition.h"
#include "static_assert.h"

/* The most commands the playback queue holds. */
enum { BATTLE_PLAYBACK_CAPACITY = 64 };

/* BATTLE_PLAYBACK_STATE.phase: how far btPlaybackRun has got through the
   action's targets. Its message phases (PLAY_MESSAGE and the like) are private
   to battle_playback.c. */
typedef enum BATTLE_PLAYBACK_PHASE {
	BATTLE_PLAYBACK_PHASE_IDLE = 0, /* paused until the effect resumes it */
	/* Resolves the next target's commands. */
	BATTLE_PLAYBACK_PHASE_RESOLVE_TARGET = 1,
	BATTLE_PLAYBACK_PHASE_DISPATCH = 2, /* plays the queued commands in order */
	BATTLE_PLAYBACK_PHASE_READY = 4		/* every target has been played */
} BATTLE_PLAYBACK_PHASE;

/* The playback queue: up to BATTLE_PLAYBACK_CAPACITY commands, each an opcode
   (BATTLE_PLAYBACK_COMMAND) and one operand, in the order they play. It lives
   in BATTLE_STATE.playback.queue: btPlaybackPush appends to it and playback
   reads it. */
typedef struct BATTLE_PLAYBACK_QUEUE {
	Uint8 opcodes[BATTLE_PLAYBACK_CAPACITY]; /* BATTLE_PLAYBACK_COMMAND */
	/* Each command's operand: a unit, item or ability id, a number, a message
	   or a sound. */
	Uint32 operands[BATTLE_PLAYBACK_CAPACITY];
	Sint32 tgtIdx; /* the plan's next target to resolve */
	Sint32 cnt;	   /* commands queued */
} BATTLE_PLAYBACK_QUEUE;

STATIC_ASSERT(sizeof(BATTLE_PLAYBACK_QUEUE) == 328);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_QUEUE, operands) == 64);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_QUEUE, tgtIdx) == 320);
STATIC_ASSERT(offsetof(BATTLE_PLAYBACK_QUEUE, cnt) == 324);

/* The playback commands btPlaybackPush queues, each with one operand, for
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
	PLAYBACK_MARK = 6,			  /* sets BattleMessageStateWork->marked */
	PLAYBACK_CLEAR_ARGUMENTS = 7, /* forgets the names and numbers pushed */
	PLAYBACK_HIT = 8,			  /* the unit flinches and flashes */
	PLAYBACK_DEFEAT = 9,		  /* the unit falls; an enemy fades out */
	PLAYBACK_DRAW_STATUS = 10,	  /* redraws the party's status */
	PLAYBACK_REFRESH_UNIT = 11,	  /* the unit's status icons, colour, pose */
	PLAYBACK_REMOVE_UNIT = 12,	  /* an enemy leaves the battle */
	PLAYBACK_DJINN_BONUS = 13,	  /* later defeats give the Djinn bonus */
	PLAYBACK_SOUND = 14			  /* plays a sound */
} BATTLE_PLAYBACK_COMMAND;

#endif
