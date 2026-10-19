#ifndef ALCHEMY_OBJCMD_H
#define ALCHEMY_OBJCMD_H

/// @file
/// Field object scripts: one s32 command word, then its operands, as
/// gScriptObject_CommandTable runs them. Each macro expands to the words of
/// one command. Coordinates are 16.16 fixed point (PIXELS).

#include "types.h"

#define SC_LABEL(n) (s32)(0x80000000 | (n) << 16) // Branch destination
#define SC_TO(n) (s32)(0xc0000000 | (n) << 16)
#define SC_TOP 0 // Script start
#define SC_FOREVER 0xffff

#define SC_WAIT(n) 0x00, (n)                    // Wait n frames.
#define SC_SYNC 0x01                            // Wait for arrival.
#define SC_POS(x, y, z) 0x02, (x), (y), (z)     // Set position.
#define SC_MOVE(x, y, z) 0x03, (x), (y), (z)    // Move toward the target.
#define SC_MOVE_BY(x, y, z) 0x04, (x), (y), (z) // Move by a relative offset.
#define SC_FACE_LINK 0x05                       // Face the linked object.
#define SC_MOVE_LINK 0x06                       // Move to the linked object.
#define SC_MOVE_LOCAL(a, b) 0x07, (a), (b)
#define SC_WANDER(a, b, c) 0x08, (a), (b), (c) // Wander.
#define SC_WANDER_HOME(a, b, c) 0x09, (a), (b), (c)
#define SC_APPROACH_LINK 0x0a
#define SC_CAMERA 0x0b                 // Follow the leader.
#define SC_LOOP(n, to) 0x0c, (n), (to) // Repeat up to n times.
#define SC_JUMP(to) 0x0d, (to)
#define SC_IF(to) 0x0e, (to)    // Branch if the result is true.
#define SC_IFNOT(to) 0x0f, (to) // Branch if the result is false.
#define SC_END 0x10             // Return to the main script.
#define SC_FLAG_CHECK(f) 0x11, (f)
#define SC_FLAG_ON(f) 0x12, (f)
#define SC_FLAG_OFF(f) 0x13, (f)
#define SC_FLAG_REV(f) 0x14, (f)
#define SC_SET(w, v) 0x15, (w), (v) // Write a work value.
#define SC_ADD(w, v) 0x16, (w), (v)
#define SC_CMP(w, v) 0x17, (w), (v)
#define SC_PLAYER 0x18 // Player input control
#define SC_KEYS 0x19
#define SC_WORLD 0x1a // World map control
#define SC_FREE 0x1b  // Release
#define SC_ARG(v) 0x1c, (v)
#define SC_NOP(v) 0x1d, (v)
#define SC_SE(n) 0x1e, (n) // Sound effect
#define SC_BASE 0x1f       // Restart from the next word.
#define SC_B54_OFF 0x20
#define SC_B54_ON 0x21
#define SC_CALL(fn) 0x22, (s32)(fn)
#define SC_KEY_COUNT 0x23

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
  SW_HOOK = 0x26
};

#endif
