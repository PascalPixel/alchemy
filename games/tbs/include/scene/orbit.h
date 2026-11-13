#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/// @file
/// The views of an orbiting scene actor's sprite.

#include "types.h"

/// A floating Nut's sprite, seen as the attribute bits, palette and state its
/// scenes set.
typedef struct OrbitingMapActorSprite {
  u8 padding00[5]; ///< Sprite's queue link and attribute 0's y
  /// Sprite's attribute 0 high byte.
  /// bits: 7..6 shape | 5 full colour | 4 mosaic | 3..2 sprite mode |
  /// 1..0 affine
  ///@{
  u8 affineModeMosaic : 5; ///< the scenes leave these
  u8 fullColor : 1;        ///< the Nut's scenes clear it
  u8 shape : 2;            ///< the scenes leave it
  ///@}
  u8 padding06[3]; ///< attribute 1 and the low byte of attribute 2
  /// Sprite's attribute 2 high byte.
  /// bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8
  ///@{
  u8 tileHigh : 2;      ///< tile number bits 9..8; the scenes leave them
  u8 priority : 2;      ///< the Nut's scenes set 1
  u8 paletteNumber : 4; ///< the Nut's scenes set 0
  ///@}
  u8 padding0a[18];
  /// Sprite's slot: the VRAM cache entry the Nut's icon is loaded
  /// into.
  u8 palette;
  u8 padding1d[10];
  u8 state; ///< Sprite's count of layers; the Nut's scenes zero it
} OrbitingMapActorSprite;

/// A floating Nut's actor: its position, the spot it sways around and
/// how far through the sway it is, its sprite and its update routine.
typedef struct OrbitingMapActor {
  u8 padding00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 padding14[15];
  /// Actor's priorityFlags; the Nut's scenes clear bit 0,
  /// ACTOR_PRIORITY_AUTOMATIC.
  u8 priorityFlags;
  u8 padding24[12];
  /// 1/65536 turns; advanced by TRIG_TURN / 64 plus two random steps each
  /// frame.
  s32 orbitAngle;
  u8 padding34[4];
  s32 orbitCenterX; ///< 16.16 fixed-point pixels
  s32 orbitCenterY; ///< 16.16 fixed-point pixels
  s32 orbitCenterZ; ///< 16.16 fixed-point pixels
  u8 padding44[12];
  OrbitingMapActorSprite *sprite;
  u8 padding54;
  u8 mode; ///< Actor's motionFlags; the Nut's scenes zero it
  /// Actor.arrivalAxis, the axis watched for arrival; Scene_FloatNut
  /// zeroes it.
  u8 state;
  u8 padding57[5];
  /// Actor.retainSpriteCache: the Nut's scenes keep its sprite's VRAM
  /// cache entry.
  u8 active;
  u8 padding5d[4];
  /// Actor's frozen, which skips steering, terrain, gravity and floating;
  /// the Nut's scenes set it.
  u8 visible;
  u8 padding62[10];
  /// Actor's update routine; Scene_FloatNut sets
  /// Scene_UpdateFloatingNut.
  u32 callback;
} OrbitingMapActor;

#endif
