#ifndef ALCHEMY_MODULE_SCENE_OSENIA_CLIFFS_H
#define ALCHEMY_MODULE_SCENE_OSENIA_CLIFFS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

extern u32 gOseniaCliffs_PaletteCycleStep[];

#ifdef MODERN
// The rope routine is called with no prototype in scope, as its bytes need:
// called as returning int, the two post ids load before the first argument.
// A current GCC gets it here.
void Rope_Tie(s32 a, s32 b, s32 c);
#endif

void Rope_Place(s32 a, s32 flag, s32 b, s32 c);

void OseniaCliffs_CyclePalette(void);

#endif
