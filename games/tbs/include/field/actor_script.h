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

#define SC_WAIT(n) 0x00, (n)                     ///< Wait n frames.
#define SC_SYNC 0x01                             ///< Wait for arrival.
#define SC_POSITION(x, y, z) 0x02, (x), (y), (z) ///< Set position.
#define SC_MOVE(x, y, z) 0x03, (x), (y), (z)     ///< Move toward the target.
#define SC_MOVE_BY(x, y, z) 0x04, (x), (y), (z)  ///< Move by a relative offset.
#define SC_FACE_WATCHED 0x05                     ///< Face the watched actor.
#define SC_MOVE_TO_WATCHED 0x06                  ///< Move to the watched actor.
/// Move toward the actor's position plus an offset that is never filled in;
/// the operands are skipped unread.
#define SC_MOVE_LOCAL(a, b) 0x07, (a), (b)
// Wander to random points base to base + range away, within limit of home;
// SC_WANDER_HOME heads home when outside it.
#define SC_WANDER(a, b, c) 0x08, (a), (b), (c)
#define SC_WANDER_HOME(a, b, c) 0x09, (a), (b), (c)
#define SC_APPROACH_WATCHED 0x0a ///< Follow the watched actor to 16 pixels.
#define SC_CAMERA 0x0b           ///< Follow the leader.
/// Jump to label to n - 1 times, so what lies between runs n times; loops
/// do not nest.
#define SC_LOOP(n, to) 0x0c, (n), (to)
#define SC_JUMP(to) 0x0d, (to)  ///< Jump to label to.
#define SC_IF(to) 0x0e, (to)    ///< Branch if the result is true.
#define SC_IFNOT(to) 0x0f, (to) ///< Branch if the result is false.
#define SC_END 0x10             ///< Return to the main script.
// Test game flag f for SC_IF; ON, OFF and REV then set, clear or flip it.
#define SC_FLAG_CHECK(f) 0x11, (f)
#define SC_FLAG_ON(f) 0x12, (f)
#define SC_FLAG_OFF(f) 0x13, (f)
#define SC_FLAG_REV(f) 0x14, (f)
#define SC_SET(w, v) 0x15, (w), (v) ///< Write a work value.
#define SC_ADD(w, v) 0x16, (w), (v) ///< Add to a work value.
#define SC_CMP(w, v) 0x17, (w), (v) ///< Test whether work value w equals v.
#define SC_PLAYER 0x18              ///< Player input control
#define SC_KEYS 0x19    ///< Move 8 pixels at a time by the direction pad
#define SC_WORLD 0x1a   ///< World map control
#define SC_RELEASE 0x1b ///< Release
#define SC_ANIMATION(v) 0x1c, (v) ///< Switch the sprites to animation v.
#define SC_NOP(v) 0x1d, (v)       ///< Skip its operand.
#define SC_SE(n) 0x1e, (n)        ///< Play sound n
#define SC_BASE 0x1f              ///< Restart from the next word.
#define SC_HIDE_SPRITES 0x20      ///< Hide the sprites (ACTOR_SPRITE_NONE).
#define SC_SHOW_SPRITE 0x21       ///< Show one sprite (ACTOR_SPRITE_ONE).
/// Call fn with the actor each frame until it returns zero.
#define SC_CALL(fn) 0x22, (s32)(fn)
#define SC_KEY_COUNT 0x23 ///< Check the configured keys.

/// Script work values for SC_SET, SC_ADD and SC_CMP
enum ActorScriptValue {
  SW_SCRIPT = 0x00,
  SW_CURSOR = 0x01,
  SW_FACING = 0x03,
  SW_X = 0x05,
  SW_Y = 0x06,
  SW_Z = 0x07,
  SW_RADIUS = 0x08,
  SW_SCALE_X = 0x09,
  SW_SCALE_Y = 0x0a,
  SW_VELOCITY_X = 0x0c,
  SW_VELOCITY_Y = 0x0d,
  SW_VELOCITY_Z = 0x0e,
  SW_SPEED = 0x0f,
  SW_ACCELERATION = 0x10,
  SW_TARGET_X = 0x11,
  SW_TARGET_Y = 0x12,
  SW_TARGET_Z = 0x13,
  SW_BOUNCE = 0x14,
  SW_GRAVITY = 0x15,
  SW_GROUND = 0x16,
  SW_MOTION = 0x1a,
  SW_ARRIVAL = 0x1b,
  SW_RESULT = 0x1c,
  SW_SNAP = 0x1d,
  SW_COLLISION = 0x1e,
  SW_STEERING = 0x1f,
  SW_PAUSED = 0x20,
  SW_REPEAT = 0x21,
  SW_WAIT = 0x22,
  /// Actor.timer, the parameter an actor script reads; a scene's own
  /// scripts set it to 1 as they end, for the code that waits on them.
  SW_TIMER = 0x23,
  /// Actor.userValue, scene scratch: an event sets it for the scene code
  /// that reads or waits on it.
  SW_USER_VALUE = 0x24,
  SW_HOOK = 0x26,
  SW_RISE_COUNTER = 0x27, ///< Actor.riseCounter, scene code's own byte
  SW_RISE_ENABLED = 0x28  ///< Actor.riseEnabled, scene code's own byte
};

#endif
