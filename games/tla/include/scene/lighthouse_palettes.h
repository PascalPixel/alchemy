#ifndef ALCHEMY_LIGHTHOUSE_PALETTES_H
#define ALCHEMY_LIGHTHOUSE_PALETTES_H

/// @file
/// How the Jupiter Lighthouse rooms, and one Mars Lighthouse room that keeps a
/// copy, load their cycling palette
/// (shared/jupiter_lighthouse_load_palettes.h).

#include "types.h"

/// How JupiterLighthouse_LoadPalettes treats a room.
enum LighthousePalettes {
  LIGHTHOUSE_PALETTES_BY_FLAG,    ///< lit once the lighthouse is
  LIGHTHOUSE_PALETTES_LIT,        ///< lit whatever the flag
  LIGHTHOUSE_PALETTES_KEEP_COPIES ///< by the flag, keeping the copies it has
};

void JupiterLighthouse_LoadPalettes(s32 mode);

#endif
