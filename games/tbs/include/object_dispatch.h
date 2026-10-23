#ifndef ALCHEMY_OBJECT_DISPATCH_H
#define ALCHEMY_OBJECT_DISPATCH_H

/// @file
/// The object dispatcher's scripts: key checks, the player, the world map and
/// the camera.

#include "unit/field/field.h"
#include "types.h"

union FieldObject;

/// A field object as the object dispatcher's far calls take it.
struct DispatchObject {
  u32 value00; ///< the object script's address
  s16 value04; ///< the script cursor, in words
  /// ObjectEvent's facing, position, scales and velocity; not named here.
  ///@{
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 terrainHeight;
  s32 scaleX;
  s32 scaleY;
  u16 radius;
  u8 terrainId;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  ///@}
  s32 value30; ///< speed limit, 16.16 fixed-point pixels per frame
  s32 value34; ///< acceleration, 16.16 fixed point
  /// ObjectEvent's target, bounce, gravity and riseBaseY; not named here.
  ///@{
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  s32 riseBaseY;
  ///@}
  union {
    struct AnimationObject *child; ///< OBJECT_SPRITE_ONE
    void **children; ///< OBJECT_SPRITE_GROUP: four, empty ones null
  } target;
  u8 kind;           ///< low four bits: an OBJECT_SPRITE_ kind
  u8 motionFlags;    ///< ObjectEvent's; not named here
  u8 arrivalAxis;    ///< ObjectEvent's; not named here
  u8 value57;        ///< the script's last flag test; cleared with a new script
  u8 snapToTarget;   ///< ObjectEvent's; not named here
  u8 collisionFlags; ///< ObjectEvent's; not named here
  u8 steeringFlags;  ///< ObjectEvent's; not named here
  u8 value5b;        ///< paused; cleared with a new script
  u8 retainSpriteCache; ///< nonzero keeps its sprite's VRAM; not named here
  u8 value5d; ///< the script's SC_LOOP count; cleared with a new script
  /// ObjectEvent's script wait, refusals, frozen flag and the two bytes map
  /// code uses; not named here.
  ///@{
  u16 wait;
  u8 refused;
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  ///@}
  s16 value64;       ///< the action script's parameter; the camera clears it
  s16 footprintSide; ///< ObjectRuntime's; not named here
  s32 argument;      ///< the linked object a script approaches or follows
  /// ObjectEvent's update hook; not named here.
  void (*update)(union FieldObject *object);
};

extern const s32 gObjectDispatch_KeyCheckScript[];
extern const s32 gObjectDispatch_KeysScript[];
extern const s32 gObjectDispatch_PlayerScript[];
extern const s32 gObjectDispatch_WorldMapScript[];
extern const s32 gObjectDispatch_CameraScript[];
extern const s32 gObjectDispatch_ApproachLinkScript[];
extern const s32 gObjectDispatch_EndScript[];

#endif
