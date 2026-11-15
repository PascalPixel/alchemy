#ifndef ALCHEMY_ORBIT_H
#define ALCHEMY_ORBIT_H

/// @file
/// An effect that circles a point, as the field Psynergy and the scenes run it.

#include "types.h"

struct FieldSprite;

struct OrbitEffect {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 radius;
  u8 unk34[0x1c];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[0x0e];
  u16 angle;
  u16 unk66;
  u8 unk68[4];
  void (*update)(struct OrbitEffect *effect);
};

#endif
