#ifndef ALCHEMY_PARTICLE_H
#define ALCHEMY_PARTICLE_H

/// @file
/// What games/tla/src/debug/ declares for the files that use it, written once.

#include "types.h"

struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 age;
};

#endif
