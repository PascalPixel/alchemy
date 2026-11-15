#ifndef ALCHEMY_ACTOR_SCRIPT_H
#define ALCHEMY_ACTOR_SCRIPT_H

/// @file
/// Field object scripts: one s32 command word, then its operands, as the
/// command table in field/map_grid_data.c runs them. Each macro
/// expands to the words of one command. An unknown command keeps its number
/// as its name.
// The Lost Age numbers its commands as The Broken Seal does, with SC_CMD_03
// added before SC_MOVE, eleven more commands after SC_PLAYER, SC_SE_IN_VIEW
// after SC_SE and SC_MOVE_XZ after SC_CAMERA_IN_BOUNDS.

#include "types.h"

#define SC_LABEL(n) (s32)(0x80000000 | (n) << 16) ///< a jump target
#define SC_TO(n) (s32)(0xc0000000 | (n) << 16)    ///< jump to label n
#define SC_TOP 0                                  ///< jump to the first word
#define SC_FOREVER 0xffff ///< An SC_LOOP count that never runs out

#define SC_WAIT(n) 0x00, (n)                     ///< wait n frames
#define SC_SYNC 0x01                             ///< wait to arrive
#define SC_POSITION(x, y, z) 0x02, (x), (y), (z) ///< place at a position
#define SC_CMD_03 0x03
#define SC_MOVE(x, y, z) 0x04, (x), (y), (z)    ///< move to a target
#define SC_MOVE_BY(x, y, z) 0x05, (x), (y), (z) ///< move by an offset
#define SC_FACE_WATCHED 0x06                    ///< face the watched object
#define SC_MOVE_TO_WATCHED 0x07                 ///< move to the watched object
/// Move toward the actor's position plus an offset that is never filled in;
/// the operands are skipped unread.
#define SC_MOVE_LOCAL(a, b) 0x08, (a), (b)
#define SC_WANDER(a, b, c) 0x09, (a), (b), (c)
#define SC_WANDER_HOME(a, b, c) 0x0a, (a), (b), (c)
#define SC_APPROACH_WATCHED 0x0b ///< Follow the watched actor to 16 pixels.
#define SC_CAMERA 0x0c           ///< follow the leader
#define SC_LOOP(n, to) 0x0d, (n), (to) ///< repeat up to n times
#define SC_JUMP(to) 0x0e, (to)         ///< Jump to label to.
#define SC_IF(to) 0x0f, (to)           ///< if the result is true
#define SC_IFNOT(to) 0x10, (to)        ///< if the result is false
#define SC_END 0x11                    ///< return to the main script
#define SC_FLAG_CHECK(f) 0x12, (f)
#define SC_FLAG_ON(f) 0x13, (f)
#define SC_FLAG_OFF(f) 0x14, (f)
#define SC_FLAG_REV(f) 0x15, (f)
#define SC_SET(w, v) 0x16, (w), (v) ///< write a work value
/// Set the speed limit, in pixels a frame: SC_SPEED(0.8).
#define SC_SPEED(speed) SC_SET(SW_SPEED, Q16(speed))
/// Set the acceleration, in pixels a frame each frame.
#define SC_ACCELERATION(acceleration) SC_SET(SW_ACCEL, Q16(acceleration))
#define SC_ADD(w, v) 0x17, (w), (v) ///< Add to a work value.
#define SC_CMP(w, v) 0x18, (w), (v) ///< Test whether work value w equals v.
#define SC_PLAYER 0x19              ///< key control
#define SC_WORLD 0x1a               ///< World map control
#define SC_CMD_1B 0x1b
#define SC_ROPE 0x1c
#define SC_TIGHTROPE 0x1d
#define SC_SAND 0x1e
#define SC_SAIL 0x1f
#define SC_WORLD_SAIL 0x20
#define SC_WORLD_FLY 0x21
#define SC_WORLD_SAND 0x22
#define SC_HOVER 0x23
#define SC_WALK 0x24
#define SC_KEY_COUNT 0x25 ///< Check the configured keys.
#define SC_RELEASE 0x26   ///< release the object
#define SC_ANIMATION(v) 0x27, (v)
#define SC_NOP(v) 0x28, (v)        ///< Skip its operand.
#define SC_SE(n) 0x29, (n)         ///< a sound effect
#define SC_SE_IN_VIEW(n) 0x2a, (n) ///< sound n, if the actor is in view
#define SC_GOTO(script) 0x2b, (s32)(script) ///< continue in another script
#define SC_HIDE_SPRITES 0x2c ///< Hide the sprites (ACTOR_SPRITE_NONE).
#define SC_SHOW_SPRITE 0x2d  ///< Show one sprite (ACTOR_SPRITE_ONE).
/// Call fn with the actor each frame until it returns zero.
#define SC_CALL(fn) 0x2e, (s32)(fn)
#define SC_CAMERA_IN_BOUNDS 0x2f
#define SC_MOVE_XZ(x, z) 0x30, (x), (z) ///< move to (x, z) at the same height

/// Script work values for SC_SET, SC_ADD and SC_CMP
enum ScriptWork {
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
  SW_ACCEL = 0x10,
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
  SW_HOOK = 0x26,
  SW_BYTE_63 = 0x28
};

#endif
