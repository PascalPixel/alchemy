#ifndef ALCHEMY_SCENE_H
#define ALCHEMY_SCENE_H

#include "types.h"
#include "edition.h"

s32 Area_GetNameIndex(s32 scene, s32 entrance);
s32 Area_GetNameIndexFar(s32 scene, s32 entrance);

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/// A position on the field in 16.16 fixed-point pixels.
struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

/// The running scene's overlay, loaded into this EWRAM at each scene change;
/// its code opens with its entry table.
extern void *gOverlayArea[];

#endif
