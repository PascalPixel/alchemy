#ifndef ALCHEMY_SCRIPT_MOTION_H
#define ALCHEMY_SCRIPT_MOTION_H

/// @file
/// ScriptMotionObject, an object moved by a field script.

#include "types.h"
#include "static_assert.h"

union FieldObject;

// UB: scripts have variable lengths, so a one-word member cannot describe
// them. Keep the native view outside UBFIX: a flat word pointer changes one
// Thumb load's base/index order; a pointer to an unbounded array does too.
#ifdef UBFIX
typedef s32 ScriptMotionWords;
#else
/// The words of an object's script, as its script pointer reads them.
struct ScriptMotionWords {
  s32 word[1];
};
typedef struct ScriptMotionWords ScriptMotionWords;
#endif

/// A field object as the motion code sees it: its script, position, motion
/// toward a target, bounce and gravity, motion flags and wait, and its hook.
struct ScriptMotionObject {
  const ScriptMotionWords *script; ///< null when the object is free
  u16 scriptCursor;                ///< in words; read signed
  u16 facing;                      ///< 1/65536 turns
  s32 x;                           ///< 16.16 fixed-point pixels
  s32 y;                           ///< 16.16 fixed-point pixels
  s32 z;                           ///< 16.16 fixed-point pixels
  s32 terrainHeight;               ///< 16.16 fixed-point pixels
  u8 unk18[0x0a]; ///< the x and y scales and the radius; not named here
  u8 terrainId;   ///< the map layer whose ground height it follows
  u8 unk23;       ///< ObjectEvent.priorityFlags; not named here
  s32 velocityX;  ///< 16.16 fixed-point pixels per frame
  s32 velocityY;  ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;  ///< 16.16 fixed-point pixels per frame
  s32 speedLimit; ///< 16.16 fixed-point pixels per frame
  /// Added toward the target each frame, or taken off the speed when there is
  /// none, in 16.16 fixed-point pixels per frame.
  s32 acceleration;
  s32 targetX; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetZ; ///< 16.16 fixed-point pixels; 0x80000000 when unset
  union {
    /// The share of the falling speed kept on landing, 16.16 fixed point.
    s32 bounce;
    /// A floating object's step along gScriptObject_CycleTable, low six bits.
    s32 phase;
  } vertical;
  /// Taken off velocityY each frame above the ground, 16.16 fixed-point
  /// pixels per frame; also scales a floating object's bob.
  s32 gravity;
  /// A spare word, the sprite pointer and the sprite kind; not named here.
  u8 unk4c[9];
  /// bits: 4 no slowdown on slopes | 3 bigger float | 2 floats | 1 falls |
  /// 0 follows the terrain; zero is free flight in 3D
  u8 motionFlags;
  u8 arrivalAxis;    ///< the ARRIVAL_AXIS_* that decides arrival
  u8 unk57;          ///< the object script's last flag test; not named here
  u8 snapToTarget;   ///< nonzero lands it exactly on its target
  u8 collisionFlags; ///< bit 7: a solid object refuses its move
  u8 steeringFlags;  ///< bit 0: turns toward its velocity, 1/16 turn a frame
  u8 paused;         ///< nonzero skips its script and motion, not its hook
  /// The flag that keeps its sprite's VRAM, and the object script's SC_LOOP
  /// count; not named here.
  u8 unk5c[2];
  s16 wait;   ///< frames left before its script runs again
  u8 refused; ///< frames in a row a solid object refused its move
  u8 frozen;  ///< nonzero skips steering, terrain, gravity and floating
  /// Two bytes scene code uses, the action parameter, the footprint side
  /// and the linked object; not named here.
  u8 unk62[10];
  /// Called each frame before its script.
  void (*hook)(union FieldObject *fieldObject);
};

STATIC_ASSERT(sizeof(struct ScriptMotionObject) == 0x70);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, scriptCursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, facing) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, vertical) == 0x44);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, arrivalAxis) == 0x56);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, wait) == 0x5e);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, frozen) == 0x61);
STATIC_ASSERT(offsetof(struct ScriptMotionObject, hook) == 0x6c);

#endif
