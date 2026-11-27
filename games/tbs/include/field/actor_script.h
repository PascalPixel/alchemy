#ifndef ALCHEMY_ACTOR_SCRIPT_H
#define ALCHEMY_ACTOR_SCRIPT_H

/* Actor scripts: one Sint32 command word, then its operands, as
   ActorScript_CommandTable runs them. Each macro expands to the words of one
   command. The script stores positions and speeds in 16.16 fixed point; its
   macros take plain pixels and decimal speeds and convert them. */

#include "fixed_math.h"
#include "types.h"

/* The script commands, numbered by their place in ActorScript_CommandTable; the
   SC_ macros below write each with its operands. */
typedef enum ACTOR_SCRIPT_COMMAND {
	SCRIPT_COMMAND_WAIT = 0,
	SCRIPT_COMMAND_SYNC = 1,
	SCRIPT_COMMAND_POSITION = 2,
	SCRIPT_COMMAND_MOVE = 3,
	SCRIPT_COMMAND_MOVE_BY = 4,
	SCRIPT_COMMAND_FACE_WATCHED = 5,
	SCRIPT_COMMAND_MOVE_TO_WATCHED = 6,
	SCRIPT_COMMAND_MOVE_LOCAL = 7,
	SCRIPT_COMMAND_WANDER = 8,
	SCRIPT_COMMAND_WANDER_HOME = 9,
	SCRIPT_COMMAND_APPROACH_WATCHED = 10,
	SCRIPT_COMMAND_CAMERA = 11,
	SCRIPT_COMMAND_LOOP = 12,
	SCRIPT_COMMAND_JUMP = 13,
	SCRIPT_COMMAND_IF = 14,
	SCRIPT_COMMAND_IFNOT = 15,
	SCRIPT_COMMAND_END = 16,
	SCRIPT_COMMAND_FLAG_CHECK = 17,
	SCRIPT_COMMAND_FLAG_ON = 18,
	SCRIPT_COMMAND_FLAG_OFF = 19,
	SCRIPT_COMMAND_FLAG_REV = 20,
	SCRIPT_COMMAND_SET = 21,
	SCRIPT_COMMAND_ADD = 22,
	SCRIPT_COMMAND_CMP = 23,
	SCRIPT_COMMAND_PLAYER = 24,
	SCRIPT_COMMAND_KEYS = 25,
	SCRIPT_COMMAND_WORLD = 26,
	SCRIPT_COMMAND_RELEASE = 27,
	SCRIPT_COMMAND_ANIMATION = 28,
	SCRIPT_COMMAND_NOP = 29,
	SCRIPT_COMMAND_SE = 30,
	SCRIPT_COMMAND_BASE = 31,
	SCRIPT_COMMAND_HIDE_SPRITES = 32,
	SCRIPT_COMMAND_SHOW_SPRITE = 33,
	SCRIPT_COMMAND_CALL = 34,
	SCRIPT_COMMAND_KEY_COUNT = 35,
	SCRIPT_COMMAND_COUNT = 36
} ACTOR_SCRIPT_COMMAND;

/* The highest script word run as a command, an index of
   ActorScript_CommandTable; a larger one is stepped over. */
#define SCRIPT_COMMAND_MAX 63

#define SC_LABEL(n) (Sint32)(0x80000000 | (n) << 16) /* Branch destination */
/* A jump's operand: the label SC_LABEL(n), which fdActorScriptFindLabel matches
   without bit 30. */
#define SC_TO(n)	(Sint32)(0xc0000000 | (n) << 16)
#define SC_TOP		0	  /* Script start */
#define SC_FOREVER	65535 /* An SC_LOOP count that never runs out */

#define SC_WAIT(n) SCRIPT_COMMAND_WAIT, (n) /* Wait n frames. */
#define SC_SYNC	   SCRIPT_COMMAND_SYNC		/* Wait for arrival. */
/* Set position. */
#define SC_POSITION(x, y, z) \
	SCRIPT_COMMAND_POSITION, toFIXED(x), toFIXED(y), toFIXED(z)
/* Move toward the target. */
#define SC_MOVE(x, y, z) SCRIPT_COMMAND_MOVE, toFIXED(x), toFIXED(y), toFIXED(z)
/* Move by an offset. */
#define SC_MOVE_BY(x, y, z) \
	SCRIPT_COMMAND_MOVE_BY, toFIXED(x), toFIXED(y), toFIXED(z)
#define SC_FACE_WATCHED \
	SCRIPT_COMMAND_FACE_WATCHED /* Face the watched actor. */
/* Move to the watched actor. */
#define SC_MOVE_TO_WATCHED	SCRIPT_COMMAND_MOVE_TO_WATCHED
/* Move toward the actor's position plus an offset that is never filled in; the
   operands are skipped unread. */
#define SC_MOVE_LOCAL(a, b) SCRIPT_COMMAND_MOVE_LOCAL, (a), (b)
/* Wander to random points base to base + range pixels away, within limit pixels
   of home; SC_WANDER_HOME heads home when outside it. */
#define SC_WANDER(base, range, limit) \
	SCRIPT_COMMAND_WANDER, toFIXED(base), toFIXED(range), toFIXED(limit)
#define SC_WANDER_HOME(base, range, limit) \
	SCRIPT_COMMAND_WANDER_HOME, toFIXED(base), toFIXED(range), toFIXED(limit)
/* Follow the watched actor to 16 pixels. */
#define SC_APPROACH_WATCHED SCRIPT_COMMAND_APPROACH_WATCHED
#define SC_CAMERA			SCRIPT_COMMAND_CAMERA /* Follow the leader. */
/* Jump to label to n - 1 times, so what lies between runs n times; loops do not
   nest. */
#define SC_LOOP(n, to)		SCRIPT_COMMAND_LOOP, (n), (to)
#define SC_JUMP(to)			SCRIPT_COMMAND_JUMP, (to) /* Jump to label to. */
#define SC_IF(to)			SCRIPT_COMMAND_IF, (to) /* Branch if the result is true. */
#define SC_IFNOT(to) \
	SCRIPT_COMMAND_IFNOT, (to)				/* Branch if the result is false. */
#define SC_END			 SCRIPT_COMMAND_END /* Return to the main script. */
/* Test game flag f for SC_IF; ON, OFF and REV then set, clear or flip it. */
#define SC_FLAG_CHECK(f) SCRIPT_COMMAND_FLAG_CHECK, (f)
#define SC_FLAG_ON(f)	 SCRIPT_COMMAND_FLAG_ON, (f)
#define SC_FLAG_OFF(f)	 SCRIPT_COMMAND_FLAG_OFF, (f)
#define SC_FLAG_REV(f)	 SCRIPT_COMMAND_FLAG_REV, (f)
#define SC_SET(w, v)	 SCRIPT_COMMAND_SET, (w), (v) /* Write a state value. */
#define SC_ADD(w, v)	 SCRIPT_COMMAND_ADD, (w), (v) /* Add to a state value. */
/* Test whether state value w equals v. */
#define SC_CMP(w, v)	 SCRIPT_COMMAND_CMP, (w), (v)
/* Set the speed limit, in pixels a frame: SC_SPEED(0.8). */
#define SC_SPEED(speed)	 SC_SET(SW_SPEED, toFIXED(speed))
/* Set the acceleration, in pixels a frame per frame. */
#define SC_ACCELERATION(acceleration) \
	SC_SET(SW_ACCELERATION, toFIXED(acceleration))
#define SC_PLAYER SCRIPT_COMMAND_PLAYER /* Hand the actor to the player. */
/* Move 8 pixels at a time by the direction pad. */
#define SC_KEYS	  SCRIPT_COMMAND_KEYS
#define SC_WORLD \
	SCRIPT_COMMAND_WORLD /* Hand it to the player on the World Map. */
#define SC_RELEASE		SCRIPT_COMMAND_RELEASE /* Free the actor's slot. */
/* Switch the sprites to animation v. */
#define SC_ANIMATION(v) SCRIPT_COMMAND_ANIMATION, (v)
#define SC_NOP(v)		SCRIPT_COMMAND_NOP, (v) /* Skip its operand. */
#define SC_SE(n)		SCRIPT_COMMAND_SE, (n)	/* Play sound n. */
#define SC_BASE			SCRIPT_COMMAND_BASE /* Restart from the next word. */
/* Hide the sprites (ACTOR_SPRITE_NONE). */
#define SC_HIDE_SPRITES SCRIPT_COMMAND_HIDE_SPRITES
/* Show one sprite (ACTOR_SPRITE_ONE). */
#define SC_SHOW_SPRITE	SCRIPT_COMMAND_SHOW_SPRITE
/* Call fn with the actor each frame until it returns zero. */
#define SC_CALL(fn)		SCRIPT_COMMAND_CALL, (Sint32)(fn)
#define SC_KEY_COUNT	SCRIPT_COMMAND_KEY_COUNT /* Check the configured keys. */

/* Script state values for SC_SET, SC_ADD and SC_CMP: the actor's fields, by
   their place in ActorScript_OperandHandlerTable. */
typedef enum ACTOR_SCRIPT_VALUE {
	SW_SCRIPT = 0,
	SW_CURSOR = 1,
	SW_FACING = 3,
	SW_X = 5,
	SW_Y = 6,
	SW_Z = 7,
	SW_RADIUS = 8,
	SW_SCALE_X = 9,
	SW_SCALE_Y = 10,
	SW_VELOCITY_X = 12,
	SW_VELOCITY_Y = 13,
	SW_VELOCITY_Z = 14,
	SW_SPEED = 15,
	SW_ACCELERATION = 16,
	SW_TARGET_X = 17,
	SW_TARGET_Y = 18,
	SW_TARGET_Z = 19,
	SW_BOUNCE = 20,
	SW_GRAVITY = 21,
	SW_GROUND = 22,
	SW_SCENE_WORD = 23,	 /* ACTOR.sceneWord, scene code's own word */
	SW_SPRITE = 24,		 /* the sprite, or its list of four */
	SW_SPRITE_KIND = 25, /* ACTOR.sprStorageKind */
	SW_MOTION = 26,
	SW_ARRIVAL = 27,
	SW_RESULT = 28,
	SW_SNAP = 29,
	SW_COLLISION = 30,
	SW_STEERING = 31,
	SW_PAUSED = 32,
	SW_REPEAT = 33,
	SW_WAIT = 34,
	/* ACTOR.tmr, the parameter an actor script reads; a scene's own scripts set
	   it to 1 as they end, for the code that waits on them. */
	SW_TIMER = 35,
	/* ACTOR.userValue, scene scratch: a script sets it for the scene code that
	   reads or waits on it. */
	SW_USER_VALUE = 36,
	SW_WATCHED_ACTOR = 37, /* the actor it follows or faces */
	SW_HOOK = 38,
	SW_SCENE_COUNTER = 39, /* ACTOR.sceneCnt, scene code's own byte */
	SW_SCENE_BYTE = 40	   /* ACTOR.sceneByte, scene code's own byte */
} ACTOR_SCRIPT_VALUE;

#include "field/script_operands.h"

/* Commands receive the actor's interpreter view. A nonzero result continues its
   script this frame; zero waits until the next frame. */
extern const SCRIPT_COMMAND ActorScript_CommandTable[47];

#endif
