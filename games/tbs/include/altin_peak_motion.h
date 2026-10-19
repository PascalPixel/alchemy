#ifndef ALCHEMY_ALTIN_PEAK_MOTION_H
#define ALCHEMY_ALTIN_PEAK_MOTION_H

/// @file
/// The hopping motion Altin Peak's scene code reads from an actor's work.

#include "types.h"
#include "static_assert.h"

struct AltinPeakHopMotion {
  u8 unk00[12];
  s32 height;
  u8 unk10[4];
  s32 terrainHeight;
  u8 unk18[16];
  s32 velocityY;
  u8 unk2c[47];
  u8 verticalStopped;
  u8 unk5c[8];
  s16 hopTimer, shakeDelay;
  s32 landingCuePending;
};

STATIC_ASSERT(offsetof(struct AltinPeakHopMotion, velocityY) == 0x28);
STATIC_ASSERT(offsetof(struct AltinPeakHopMotion, verticalStopped) == 0x5b);
STATIC_ASSERT(offsetof(struct AltinPeakHopMotion, hopTimer) == 0x64);
STATIC_ASSERT(offsetof(struct AltinPeakHopMotion, shakeDelay) == 0x66);
STATIC_ASSERT(offsetof(struct AltinPeakHopMotion, landingCuePending) == 0x68);

#endif
