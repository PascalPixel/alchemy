#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/// @file
/// The views of an orbiting map object's sprite.

#include "types.h"

/// A floating Nut's sprite, seen as the attribute bits, palette and state its
/// maps set.
typedef struct OrbitingMapObjectSprite {
  u8 padding00[5]; ///< Sprite's queue link and attribute 0's y
  /// Sprite's attribute 0 high byte.
  /// bits: 7..6 shape | 5 full colour | 4 mosaic | 3..2 object mode |
  /// 1..0 affine
  ///@{
  u8 affineModeMosaic : 5; ///< the maps leave these
  u8 fullColor : 1;        ///< the Nut's maps clear it
  u8 shape : 2;            ///< the maps leave it
  ///@}
  u8 padding06[3]; ///< attribute 1 and the low byte of attribute 2
  /// Sprite's attribute 2 high byte.
  /// bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8
  ///@{
  u8 tileHigh : 2;      ///< tile number bits 9..8; the maps leave them
  u8 priority : 2;      ///< the Nut's maps set 1
  u8 paletteNumber : 4; ///< the Nut's maps set 0
  ///@}
  u8 padding0a[18];
  /// Sprite's slot: the VRAM cache entry the Nut's icon is loaded
  /// into.
  u8 palette;
  u8 padding1d[10];
  u8 state; ///< Sprite's count of layers; the Nut's maps zero it
} OrbitingMapObjectSprite;

/// A floating Nut's object event: its position, the spot it sways around and
/// how far through the sway it is, its sprite and its callback.
typedef struct OrbitingMapObject {
  u8 padding00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 padding14[15];
  /// ObjectEvent's priorityFlags; the Nut's maps clear bit 0,
  /// OBJ_EVENT_PRIORITY_AUTOMATIC.
  u8 priorityFlags;
  u8 padding24[12];
  /// 1/65536 turns; advanced by 0x400 plus two random steps each frame.
  s32 orbitAngle;
  u8 padding34[4];
  s32 orbitCenterX; ///< 16.16 fixed-point pixels
  s32 orbitCenterY; ///< 16.16 fixed-point pixels
  s32 orbitCenterZ; ///< 16.16 fixed-point pixels
  u8 padding44[12];
  OrbitingMapObjectSprite *sprite;
  u8 padding54;
  u8 mode; ///< ObjectEvent's motionFlags; the Nut's maps zero it
  /// Object.arrivalAxis, the axis watched for arrival; Map_FloatNut
  /// zeroes it.
  u8 state;
  u8 padding57[5];
  /// ObjectEvent.retainSpriteCache: the Nut's maps keep its sprite's VRAM
  /// cache entry.
  u8 active;
  u8 padding5d[4];
  /// ObjectEvent's frozen, which skips steering, terrain, gravity and floating;
  /// the Nut's maps set it.
  u8 visible;
  u8 padding62[10];
  /// ObjectEvent's update hook; Map_FloatNut sets
  /// Map_UpdateFloatingNut.
  u32 callback;
} OrbitingMapObject;

#endif
