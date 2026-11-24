#ifndef ALCHEMY_SCENE_H
#define ALCHEMY_SCENE_H

/// @file
/// The running scene's overlay area, a field position, and the area name a
/// scene and entrance show.

#include "fixed_math.h"
#include "types.h"
#include "edition.h"

/// The area name number for `scene` entered at `entrance`, as the save files
/// and the world map show it.
s32 Area_GetNameIndex(s32 scene, s32 entrance);
s32 Area_GetNameIndexFar(s32 scene, s32 entrance);

/// The running scene's overlay, loaded into this EWRAM at each scene change;
/// its code opens with its entry table.
extern void *gOverlayArea[];

#endif
