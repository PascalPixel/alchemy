#ifndef ALCHEMY_ALTIN_PEAK_MOTION_H
#define ALCHEMY_ALTIN_PEAK_MOTION_H

#include "types.h"
#include "static_assert.h"

struct SceneMotion {
  u8 unk00[12];
  s32 y;
  u8 unk10[4];
  s32 ground;
  u8 unk18[16];
  s32 velocity;
  u8 unk2c[47];
  u8 state;
  u8 unk5c[8];
  s16 timer, delay;
  s32 active;
};

STATIC_ASSERT(offsetof(struct SceneMotion, velocity) == 0x28);
STATIC_ASSERT(offsetof(struct SceneMotion, state) == 0x5b);
STATIC_ASSERT(offsetof(struct SceneMotion, timer) == 0x64);
STATIC_ASSERT(offsetof(struct SceneMotion, delay) == 0x66);
STATIC_ASSERT(offsetof(struct SceneMotion, active) == 0x68);

#endif
