#ifndef ALCHEMY_SCENE_H
#define ALCHEMY_SCENE_H

#include "fixed_math.h"
#include "types.h"
#include "edition.h"

s32 Area_GetNameIndex(s32 scene, s32 entrance);
s32 Area_GetNameIndexFar(s32 scene, s32 entrance);

/// A position on the field in 16.16 fixed-point pixels.
struct FieldPosition {
  FIXED x;
  FIXED y;
  FIXED z;
};

/// The running scene's overlay, loaded into this EWRAM at each scene change;
/// its code opens with its entry table.
extern void *gOverlayArea[];

#endif
