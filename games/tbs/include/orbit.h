#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/// @file
/// The views of an orbiting scene object's sprite.

#include "types.h"

/// A floating Nut's sprite, seen as the attribute bits, palette and state its
/// scenes set.
typedef struct OrbitingSceneObjectSprite {
  u8 padding00[5]; ///< AnimationObject's queue link and attribute 0's y
  /// AnimationObject's attribute 0 high byte.
  /// bits: 7..6 shape | 5 full colour | 4 mosaic | 3..2 object mode |
  /// 1..0 affine
  ///@{
  u8 flags05Low : 5;  ///< affine, object mode and mosaic; not named by scenes
  u8 flags05Bit5 : 1; ///< full colour; the Nut's scenes clear it
  u8 flags05High : 2; ///< shape; not named by scenes
  ///@}
  u8 padding06[3]; ///< attribute 1 and the low byte of attribute 2
  /// AnimationObject's attribute 2 high byte.
  /// bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8
  ///@{
  u8 flags09Low : 2;  ///< tile bits 9..8; not named by scenes
  u8 flags09Mode : 2; ///< priority; the Nut's scenes set 1
  u8 flags09High : 4; ///< palette; the Nut's scenes set 0
  ///@}
  u8 padding0a[18];
  /// AnimationObject's slot: the VRAM cache entry the Nut's icon is loaded
  /// into.
  u8 palette;
  u8 padding1d[10];
  u8 state; ///< AnimationObject's count of layers; the Nut's scenes zero it
} OrbitingSceneObjectSprite;

/// A floating Nut's actor: its position, the spot it sways around and how far
/// through the sway it is, its sprite and its callback.
typedef struct OrbitingSceneObject {
  u8 padding00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 padding14[15];
  /// FieldActor's priorityFlags; the Nut's scenes clear bit 0,
  /// ACTOR_PRIORITY_AUTOMATIC.
  u8 flags23;
  u8 padding24[12];
  /// 1/65536 turns; advanced by 0x400 plus two random steps each frame.
  s32 orbitAngle;
  u8 padding34[4];
  s32 orbitCenterX; ///< 16.16 fixed-point pixels
  s32 orbitCenterY; ///< 16.16 fixed-point pixels
  s32 orbitCenterZ; ///< 16.16 fixed-point pixels
  u8 padding44[12];
  OrbitingSceneObjectSprite *sprite;
  u8 padding54;
  u8 mode; ///< FieldActor's motionFlags; the Nut's scenes zero it
  /// ObjectRuntime.arrivalAxis, the axis watched for arrival; Scene_FloatNut
  /// zeroes it.
  u8 state;
  u8 padding57[5];
  /// FieldActor.retainSpriteCache: the Nut's scenes keep its sprite's VRAM
  /// cache entry.
  u8 active;
  u8 padding5d[4];
  /// FieldActor's frozen, which skips steering, terrain, gravity and floating;
  /// the Nut's scenes set it.
  u8 visible;
  u8 padding62[10];
  /// FieldActor's update hook; Scene_FloatNut sets
  /// SceneEffect_UpdateOrbitPosition.
  u32 callback;
} OrbitingSceneObject;

#endif
