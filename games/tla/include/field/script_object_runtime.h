#ifndef ALCHEMY_SCRIPT_OBJECT_RUNTIME_H
#define ALCHEMY_SCRIPT_OBJECT_RUNTIME_H

#include "types.h"
#include "static_assert.h"

/// A field object as its object script commands see it: the script and its
/// place, position, motion toward a target, and the home it wanders around.
struct ScriptObjectRuntime {
  const s32 *script; ///< null when the object is free
  u16 scriptCursor;  ///< in words
  /// The object's facing, 1/65536 turns: the heading the wander commands
  /// wander around, and where SC_FACE_WATCHED stores the angle to the watched
  /// object.
  u16 scriptValue;
  s32 x;             ///< 16.16 fixed-point pixels
  s32 y;             ///< 16.16 fixed-point pixels
  s32 z;             ///< 16.16 fixed-point pixels
  s32 terrainHeight; ///< 16.16 fixed-point pixels
  u8 unk18[0x0a];    ///< the x and y scales and the radius; not named here
  u8 terrainId;      ///< the map layer its ground height is read from
  u8 unk23;          ///< Actor.priorityFlags; not named here
  s32 velocityX;     ///< 16.16 fixed-point pixels per frame
  s32 velocityY;     ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;     ///< 16.16 fixed-point pixels per frame
  s32 speedLimit;    ///< 16.16 fixed-point pixels per frame
  s32 acceleration;  ///< 16.16 fixed-point pixels per frame, per frame
  s32 targetX;       ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY;       ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetZ;       ///< 16.16 fixed-point pixels; 0x80000000 when unset
  u8 unk44[0x0c];    ///< bounce, gravity and a spare word; not named here
  void *animation;   ///< its sprite, or its list of sprites, by animationKind
  u8 animationKind;  ///< 1 one sprite, 2 a group of sprites
  u8 flags;          ///< Actor.motionFlags
  /// The arrival axis, a byte no TLA code names, and the snap flag; not named
  /// here.
  u8 unk56[0x03];
  /// Actor.collisionFlags. SC_WANDER_HOME sets bit 1 while it roams
  /// inside its home radius and clears it on heading home.
  u8 flags59;
  u8 actionFlags;   ///< Actor.actionFlags: bit 0 faces its motion
  u8 movementState; ///< Actor.movementState
  u8 unk5c[0x02];   ///< not named here
  /// The object script's wait, in frames. SC_WANDER sets 1 when it turns
  /// back.
  u16 turnedBack;
  void *callback; ///< not named by TLA code through this struct
  /// Its home, in pixels: objects are put back there. The bytes are
  /// Actor's action and unk66 too.
  ///@{
  s16 homeX;
  s16 homeZ;
  ///@}
  /// The object SC_FACE_WATCHED and SC_MOVE_TO_WATCHED aim at.
  struct ScriptObjectRuntime *watchedObject;
  u8 unk6c[0x04]; ///< the update hook (Actor.update); not named here
};

STATIC_ASSERT(sizeof(struct ScriptObjectRuntime) == 0x70);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, scriptCursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, scriptValue) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, x) == 0x08);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, speedLimit) == 0x30);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, watchedObject) == 0x68);

#endif
