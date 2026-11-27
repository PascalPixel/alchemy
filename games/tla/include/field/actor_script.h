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
	SCRIPT_COMMAND_POSITION_ORIGIN = 3,
	SCRIPT_COMMAND_MOVE = 4,
	SCRIPT_COMMAND_MOVE_BY = 5,
	SCRIPT_COMMAND_FACE_WATCHED = 6,
	SCRIPT_COMMAND_MOVE_TO_WATCHED = 7,
	SCRIPT_COMMAND_MOVE_LOCAL = 8,
	SCRIPT_COMMAND_WANDER = 9,
	SCRIPT_COMMAND_WANDER_HOME = 10,
	SCRIPT_COMMAND_APPROACH_WATCHED = 11,
	SCRIPT_COMMAND_CAMERA = 12,
	SCRIPT_COMMAND_LOOP = 13,
	SCRIPT_COMMAND_JUMP = 14,
	SCRIPT_COMMAND_IF = 15,
	SCRIPT_COMMAND_IFNOT = 16,
	SCRIPT_COMMAND_END = 17,
	SCRIPT_COMMAND_FLAG_CHECK = 18,
	SCRIPT_COMMAND_FLAG_ON = 19,
	SCRIPT_COMMAND_FLAG_OFF = 20,
	SCRIPT_COMMAND_FLAG_REV = 21,
	SCRIPT_COMMAND_SET = 22,
	SCRIPT_COMMAND_ADD = 23,
	SCRIPT_COMMAND_CMP = 24,
	SCRIPT_COMMAND_PLAYER = 25,
	SCRIPT_COMMAND_WORLD = 26,
	SCRIPT_COMMAND_KEYS = 27,
	SCRIPT_COMMAND_ROPE = 28,
	SCRIPT_COMMAND_TIGHTROPE = 29,
	SCRIPT_COMMAND_SAND = 30,
	SCRIPT_COMMAND_SAIL = 31,
	SCRIPT_COMMAND_WORLD_SAIL = 32,
	SCRIPT_COMMAND_WORLD_FLY = 33,
	SCRIPT_COMMAND_WORLD_SAND = 34,
	SCRIPT_COMMAND_HOVER = 35,
	SCRIPT_COMMAND_WALK = 36,
	SCRIPT_COMMAND_KEY_COUNT = 37,
	SCRIPT_COMMAND_RELEASE = 38,
	SCRIPT_COMMAND_ANIMATION = 39,
	SCRIPT_COMMAND_NOP = 40,
	SCRIPT_COMMAND_SE = 41,
	SCRIPT_COMMAND_SE_IN_VIEW = 42,
	SCRIPT_COMMAND_GOTO = 43,
	SCRIPT_COMMAND_HIDE_SPRITES = 44,
	SCRIPT_COMMAND_SHOW_SPRITE = 45,
	SCRIPT_COMMAND_CALL = 46,
	SCRIPT_COMMAND_CAMERA_IN_BOUNDS = 47,
	SCRIPT_COMMAND_MOVE_XZ = 48,
	SCRIPT_COMMAND_COUNT = 49
} ACTOR_SCRIPT_COMMAND;

/* Each command's brief says what the actor does when the script reaches it. */

/* Mark label n, a jump's destination. */
#define SC_LABEL(n) (Sint32)(0x80000000 | (n) << 16)
/* Name label n as a jump's destination; fdActorScriptFindLabel matches it with
   SC_LABEL(n) without bit 30. */
#define SC_TO(n)	(Sint32)(0xc0000000 | (n) << 16)
#define SC_TOP		0	  /* Name the script's start as a jump's destination. */
#define SC_FOREVER	65535 /* Give SC_LOOP a count that never runs out. */

/* Wait n frames, this one the first. */
#define SC_WAIT(n) SCRIPT_COMMAND_WAIT, (n)
/* Wait until the actor reaches its target, or a solid actor has blocked it for
   60 frames. */
#define SC_SYNC	   SCRIPT_COMMAND_SYNC
/* Stand still at (x, y, z). */
#define SC_POSITION(x, y, z) \
	SCRIPT_COMMAND_POSITION, toFIXED(x), toFIXED(y), toFIXED(z)
/* Stand at x and z 0, keeping the height. */
#define SC_POSITION_ORIGIN SCRIPT_COMMAND_POSITION_ORIGIN
/* Move toward (x, y, z). */
#define SC_MOVE(x, y, z)   SCRIPT_COMMAND_MOVE, toFIXED(x), toFIXED(y), toFIXED(z)
/* Move toward the actor's place plus (x, y, z). */
#define SC_MOVE_BY(x, y, z) \
	SCRIPT_COMMAND_MOVE_BY, toFIXED(x), toFIXED(y), toFIXED(z)
/* Face the watched actor. */
#define SC_FACE_WATCHED		SCRIPT_COMMAND_FACE_WATCHED
/* Move toward the watched actor. */
#define SC_MOVE_TO_WATCHED	SCRIPT_COMMAND_MOVE_TO_WATCHED
/* Move toward the actor's place plus an offset that is never filled in; the
   operands are skipped unread. */
#define SC_MOVE_LOCAL(a, b) SCRIPT_COMMAND_MOVE_LOCAL, (a), (b)
/* Wander to random points base to base + range pixels away, within limit pixels
   of home; SC_WANDER_HOME heads home when outside it. */
#define SC_WANDER(base, range, limit) \
	SCRIPT_COMMAND_WANDER, toFIXED(base), toFIXED(range), toFIXED(limit)
#define SC_WANDER_HOME(base, range, limit) \
	SCRIPT_COMMAND_WANDER_HOME, toFIXED(base), toFIXED(range), toFIXED(limit)
/* Follow the watched actor to within 16 pixels. */
#define SC_APPROACH_WATCHED SCRIPT_COMMAND_APPROACH_WATCHED
/* Follow the leader with the camera. */
#define SC_CAMERA			SCRIPT_COMMAND_CAMERA
/* Jump to label to n - 1 times, so what lies between runs n times; loops do not
   nest. */
#define SC_LOOP(n, to)		SCRIPT_COMMAND_LOOP, (n), (to)
/* Jump to label to. */
#define SC_JUMP(to)			SCRIPT_COMMAND_JUMP, (to)
/* Jump to label to if the last test was true. */
#define SC_IF(to)			SCRIPT_COMMAND_IF, (to)
/* Jump to label to if the last test was false. */
#define SC_IFNOT(to)		SCRIPT_COMMAND_IFNOT, (to)
/* End the script, handing the actor back to the idle script. */
#define SC_END				SCRIPT_COMMAND_END
/* Test game flag f for SC_IF; ON, OFF and REV then set, clear or flip it. */
#define SC_FLAG_CHECK(f)	SCRIPT_COMMAND_FLAG_CHECK, (f)
#define SC_FLAG_ON(f)		SCRIPT_COMMAND_FLAG_ON, (f)
#define SC_FLAG_OFF(f)		SCRIPT_COMMAND_FLAG_OFF, (f)
#define SC_FLAG_REV(f)		SCRIPT_COMMAND_FLAG_REV, (f)
/* Write v to work value w. */
#define SC_SET(w, v)		SCRIPT_COMMAND_SET, (w), (v)
/* Add v to work value w. */
#define SC_ADD(w, v)		SCRIPT_COMMAND_ADD, (w), (v)
/* Test whether work value w equals v. */
#define SC_CMP(w, v)		SCRIPT_COMMAND_CMP, (w), (v)
/* Set the speed limit, in pixels a frame: SC_SPEED(0.8). */
#define SC_SPEED(speed)		SC_SET(SW_SPEED, toFIXED(speed))
/* Set the acceleration, in pixels a frame per frame. */
#define SC_ACCELERATION(acceleration) \
	SC_SET(SW_ACCELERATION, toFIXED(acceleration))

/* The leader's controls, one a movement mode: each runs every frame until the
   mode changes. Walk or run on the field by the direction pad. */
#define SC_PLAYER	  SCRIPT_COMMAND_PLAYER
/* Walk on the World Map by the direction pad. */
#define SC_WORLD	  SCRIPT_COMMAND_WORLD
/* Climb a wall 8 pixels at a time by the direction pad. */
#define SC_KEYS		  SCRIPT_COMMAND_KEYS
/* Cross a rope. */
#define SC_ROPE		  SCRIPT_COMMAND_ROPE
/* Cross a tightrope. */
#define SC_TIGHTROPE  SCRIPT_COMMAND_TIGHTROPE
/* Move as sand (the Sand Psynergy). */
#define SC_SAND		  SCRIPT_COMMAND_SAND
/* Sail the ship on a map. */
#define SC_SAIL		  SCRIPT_COMMAND_SAIL
/* Sail the ship on the World Map. */
#define SC_WORLD_SAIL SCRIPT_COMMAND_WORLD_SAIL
/* Fly the ship on the World Map. */
#define SC_WORLD_FLY  SCRIPT_COMMAND_WORLD_FLY
/* Move as sand on the World Map. */
#define SC_WORLD_SAND SCRIPT_COMMAND_WORLD_SAND
/* Hover over what lies below (the Hover Psynergy). */
#define SC_HOVER	  SCRIPT_COMMAND_HOVER
/* Walk at one pace, a pixel a frame whatever the keys. */
#define SC_WALK		  SCRIPT_COMMAND_WALK
/* Check the configured keys and go on. */
#define SC_KEY_COUNT  SCRIPT_COMMAND_KEY_COUNT

/* Release the actor and its sprites. */
#define SC_RELEASE			SCRIPT_COMMAND_RELEASE
/* Switch the sprites to animation v. */
#define SC_ANIMATION(v)		SCRIPT_COMMAND_ANIMATION, (v)
/* Skip operand v. */
#define SC_NOP(v)			SCRIPT_COMMAND_NOP, (v)
/* Play sound n. */
#define SC_SE(n)			SCRIPT_COMMAND_SE, (n)
/* Play sound n if the actor is in the camera's view. */
#define SC_SE_IN_VIEW(n)	SCRIPT_COMMAND_SE_IN_VIEW, (n)
/* Continue in another script. */
#define SC_GOTO(script)		SCRIPT_COMMAND_GOTO, (Sint32)(script)
/* Hide the sprites (ACTOR_SPRITE_NONE). */
#define SC_HIDE_SPRITES		SCRIPT_COMMAND_HIDE_SPRITES
/* Show one sprite (ACTOR_SPRITE_ONE). */
#define SC_SHOW_SPRITE		SCRIPT_COMMAND_SHOW_SPRITE
/* Call fn with the actor each frame until it returns zero. */
#define SC_CALL(fn)			SCRIPT_COMMAND_CALL, (Sint32)(fn)
/* Follow the watched actor with the camera, keeping the view in the room's
   bounds. */
#define SC_CAMERA_IN_BOUNDS SCRIPT_COMMAND_CAMERA_IN_BOUNDS
/* Move toward (x, z) at the same height. */
#define SC_MOVE_XZ(x, z)	SCRIPT_COMMAND_MOVE_XZ, toFIXED(x), toFIXED(z)

/* The work values SC_SET, SC_ADD and SC_CMP name. */
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
	/* Sets or tests the Y scale as SW_SCALE_Y does: the operand table gives
	   both one handler. */
	SW_SCALE_Y_DUPLICATE = 11,
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
	/* ACTOR.sprStorageKind: ACTOR_SPRITE_NONE hides its sprite and
	   ACTOR_SPRITE_ONE shows it again. */
	SW_SPRITE_KIND = 25,
	SW_MOTION = 26,
	SW_ARRIVAL = 27,
	SW_RESULT = 28,
	SW_SNAP = 29,
	SW_COLLISION = 30,
	SW_STEERING = 31,
	SW_PAUSED = 32,
	SW_REPEAT = 33,
	SW_WAIT = 34,
	/* ACTOR.tmr, the parameter an actor script reads. */
	SW_TIMER = 35,
	/* ACTOR.userValue, scene scratch: a script sets it for the scene code that
	   reads or waits on it. */
	SW_USER_VALUE = 36,
	/* ACTOR.watchedActor: a fireball's path sets it to the function the
	   fireball calls when it crashes (scActorRollAndCrumble). */
	SW_WATCHED_ACTOR = 37,
	SW_HOOK = 38,
	SW_SCENE_COUNTER = 39, /* ACTOR.sceneCnt, scene code's own byte */
	SW_SCENE_BYTE = 40	   /* ACTOR.sceneByte, scene code's own byte */
} ACTOR_SCRIPT_VALUE;

#endif
