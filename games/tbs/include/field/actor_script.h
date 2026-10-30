#ifndef ALCHEMY_ACTOR_SCRIPT_H
#define ALCHEMY_ACTOR_SCRIPT_H

/// @file
/// Actor scripts: one s32 command word, then its operands, as
/// gActorScript_CommandTable runs them. Each macro expands to the words of
/// one command. Coordinates are 16.16 fixed point (PIXELS).

#include "types.h"

#define SC_LABEL(n) (s32)(0x80000000 | (n) << 16) ///< Branch destination
/// A jump's operand: the label SC_LABEL(n), which ActorScript_FindLabel
/// matches without bit 30.
#define SC_TO(n) (s32)(0xc0000000 | (n) << 16)
#define SC_TOP 0          ///< Script start
#define SC_FOREVER 0xffff ///< An SC_LOOP count that never runs out

#define SC_WAIT(n) 0, (n)                     ///< Wait n frames.
#define SC_SYNC 1                             ///< Wait for arrival.
#define SC_POSITION(x, y, z) 2, (x), (y), (z) ///< Set position.
#define SC_MOVE(x, y, z) 3, (x), (y), (z)     ///< Move toward the target.
#define SC_MOVE_BY(x, y, z) 4, (x), (y), (z)  ///< Move by a relative offset.
#define SC_FACE_WATCHED 5                     ///< Face the watched actor.
#define SC_MOVE_TO_WATCHED 6                  ///< Move to the watched actor.
/// Move toward the actor's position plus an offset that is never filled in;
/// the operands are skipped unread.
#define SC_MOVE_LOCAL(a, b) 7, (a), (b)
// Wander to random points base to base + range away, within limit of home;
// SC_WANDER_HOME heads home when outside it.
#define SC_WANDER(a, b, c) 8, (a), (b), (c)
#define SC_WANDER_HOME(a, b, c) 9, (a), (b), (c)
#define SC_APPROACH_WATCHED 10 ///< Follow the watched actor to 16 pixels.
#define SC_CAMERA 11           ///< Follow the leader.
/// Jump to label to n - 1 times, so what lies between runs n times; loops
/// do not nest.
#define SC_LOOP(n, to) 12, (n), (to)
#define SC_JUMP(to) 13, (to)  ///< Jump to label to.
#define SC_IF(to) 14, (to)    ///< Branch if the result is true.
#define SC_IFNOT(to) 15, (to) ///< Branch if the result is false.
#define SC_END 16             ///< Return to the main script.
// Test game flag f for SC_IF; ON, OFF and REV then set, clear or flip it.
#define SC_FLAG_CHECK(f) 17, (f)
#define SC_FLAG_ON(f) 18, (f)
#define SC_FLAG_OFF(f) 19, (f)
#define SC_FLAG_REV(f) 20, (f)
#define SC_SET(w, v) 21, (w), (v) ///< Write a work value.
#define SC_ADD(w, v) 22, (w), (v) ///< Add to a work value.
#define SC_CMP(w, v) 23, (w), (v) ///< Test whether work value w equals v.
#define SC_PLAYER 24              ///< Player input control
#define SC_KEYS 25    ///< Move 8 pixels at a time by the direction pad
#define SC_WORLD 26   ///< World map control
#define SC_RELEASE 27 ///< Release
#define SC_ANIMATION(v) 28, (v) ///< Switch the sprites to animation v.
#define SC_NOP(v) 29, (v)       ///< Skip its operand.
#define SC_SE(n) 30, (n)        ///< Play sound n
#define SC_BASE 31              ///< Restart from the next word.
#define SC_HIDE_SPRITES 32      ///< Hide the sprites (ACTOR_SPRITE_NONE).
#define SC_SHOW_SPRITE 33       ///< Show one sprite (ACTOR_SPRITE_ONE).
/// Call fn with the actor each frame until it returns zero.
#define SC_CALL(fn) 34, (s32)(fn)
#define SC_KEY_COUNT 35 ///< Check the configured keys.

/// Script work values for SC_SET, SC_ADD and SC_CMP
enum ActorScriptValue {
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
  SW_MOTION = 26,
  SW_ARRIVAL = 27,
  SW_RESULT = 28,
  SW_SNAP = 29,
  SW_COLLISION = 30,
  SW_STEERING = 31,
  SW_PAUSED = 32,
  SW_REPEAT = 33,
  SW_WAIT = 34,
  /// Actor.timer, the parameter an actor script reads; a scene's own
  /// scripts set it to 1 as they end, for the code that waits on them.
  SW_TIMER = 35,
  /// Actor.userValue, scene scratch: an event sets it for the scene code
  /// that reads or waits on it.
  SW_USER_VALUE = 36,
  SW_HOOK = 38,
  SW_RISE_COUNTER = 39, ///< Actor.riseCounter, scene code's own byte
  SW_RISE_ENABLED = 40  ///< Actor.riseEnabled, scene code's own byte
};

#endif
