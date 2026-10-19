#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/// @file
/// The views of an orbiting scene object's sprite.

#include "types.h"

/// A floating Nut's sprite, seen as the attribute bits, palette and state its
/// scenes set.
typedef struct OrbitingSceneObjectSprite {
  u8 padding00[5];
  u8 flags05Low : 5;
  u8 flags05Bit5 : 1;
  u8 flags05High : 2;
  u8 padding06[3];
  u8 flags09Low : 2;
  u8 flags09Mode : 2;
  u8 flags09High : 4;
  u8 padding0a[18];
  u8 palette;
  u8 padding1d[10];
  u8 state;
} OrbitingSceneObjectSprite;

/// A floating Nut's actor: its position, the spot it sways around and how far
/// through the sway it is, its sprite and its callback.
typedef struct OrbitingSceneObject {
  u8 padding00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 padding14[15];
  u8 flags23;
  u8 padding24[12];
  s32 orbitAngle; ///< 1/65536 turns
  u8 padding34[4];
  s32 orbitCenterX; ///< 16.16 fixed-point pixels
  s32 orbitCenterY; ///< 16.16 fixed-point pixels
  s32 orbitCenterZ; ///< 16.16 fixed-point pixels
  u8 padding44[12];
  OrbitingSceneObjectSprite *sprite;
  u8 padding54;
  u8 mode;
  u8 state;
  u8 padding57[5];
  u8 active;
  u8 padding5d[4];
  u8 visible;
  u8 padding62[10];
  u32 callback;
} OrbitingSceneObject;

#endif
