#ifndef ALCHEMY_SCRIPT_OBJECT_RUNTIME_H
#define ALCHEMY_SCRIPT_OBJECT_RUNTIME_H

/// @file
/// ScriptObjectRuntime, the work of an object a script moves.

#include "types.h"
#include "static_assert.h"

union FieldObject;

/// A field object as its object script commands see it: the script and its
/// place, position, motion toward a target, and the home it wanders around.
struct ScriptObjectRuntime {
  const s32 *script; ///< null when the object is free
  u16 scriptCursor;  ///< in words
  /// The object's facing, 1/65536 turns: the heading SC_WANDER and
  /// SC_WANDER_HOME wander around, and where SC_FACE_LINK stores the angle to
  /// the linked object.
  u16 scriptValue;
  s32 x;             ///< 16.16 fixed-point pixels
  s32 y;             ///< 16.16 fixed-point pixels
  s32 z;             ///< 16.16 fixed-point pixels
  s32 terrainHeight; ///< 16.16 fixed-point pixels
  /// ObjectEvent's scales and radius; not named here.
  ///@{
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  ///@}
  u8 terrainId;     ///< the map layer its ground height is read from
  u8 priorityFlags; ///< ObjectEvent's; not named here
  s32 velocityX;    ///< 16.16 fixed-point pixels per frame
  s32 velocityY;    ///< 16.16 fixed-point pixels per frame
  s32 velocityZ;    ///< 16.16 fixed-point pixels per frame
  s32 speedLimit;   ///< 16.16 fixed-point pixels per frame
  s32 acceleration; ///< 16.16 fixed-point pixels per frame, per frame
  s32 targetX;      ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetY;      ///< 16.16 fixed-point pixels; 0x80000000 when unset
  s32 targetZ;      ///< 16.16 fixed-point pixels; 0x80000000 when unset
  /// ObjectEvent's bounce, gravity and riseBaseY; not named here.
  ///@{
  s32 bounce;
  s32 gravity;
  s32 riseBaseY;
  ///@}
  void *animation;  ///< its sprite, or its list of four, by animationKind
  u8 animationKind; ///< low four bits: 0 no sprite, 1 one, 2 up to four
  u8 flags;         ///< ObjectEvent.motionFlags
  /// ObjectEvent's arrival axis, last flag test and snap flag; not named
  /// here.
  ///@{
  u8 arrivalAxis;
  u8 conditionResult;
  u8 snapToTarget;
  ///@}
  /// ObjectEvent.collisionFlags. SC_WANDER_HOME sets bit 1 while it roams
  /// inside its home radius and clears it on heading home.
  u8 flags59;
  u8 actionFlags;   ///< ObjectEvent.steeringFlags
  u8 movementState; ///< nonzero skips its script and motion (paused)
  /// ObjectEvent's flag that keeps its sprite's VRAM, and the object script's
  /// SC_LOOP count; not named here.
  ///@{
  u8 retainSpriteCache;
  u8 repeatCount;
  ///@}
  /// The object script's wait, in frames (ScriptMotionObject.wait). SC_WANDER
  /// sets 1 when it turns back, to wait a frame.
  u16 turnedBack;
  /// Not named by TBS code through this struct: the refusal count, the
  /// frozen flag and two bytes map code uses.
  void *callback;
  /// Its home, in pixels: where it was created or placed. The bytes are
  /// ObjectRuntime's action and footprintSide too.
  ///@{
  s16 homeX;
  s16 homeZ;
  ///@}
  /// The object SC_FACE_LINK and SC_MOVE_LINK aim at.
  struct ScriptObjectRuntime *linkedObject;
  /// ObjectEvent's update hook; not named here.
  void (*update)(union FieldObject *object);
};

STATIC_ASSERT(sizeof(struct ScriptObjectRuntime) == 0x70);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, script) == 0x00);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, scriptCursor) == 0x04);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, scriptValue) == 0x06);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, x) == 0x08);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, speedLimit) == 0x30);
STATIC_ASSERT(offsetof(struct ScriptObjectRuntime, linkedObject) == 0x68);

#endif
