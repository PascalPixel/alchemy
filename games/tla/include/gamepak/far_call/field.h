#ifndef ALCHEMY_FAR_CALL_FIELD_H
#define ALCHEMY_FAR_CALL_FIELD_H

/// @file
/// What games/tla/src/field/field_sequence/ declares for the files that use it,
/// written once.

#include "types.h"

void WorldMap_LoadGraphicsFar(s32 x, s32 z);

void ObjectSystem_InitializeFar(s32 mode);

void Animation_ApplyChildArgumentFar(void *, s32);

s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer);

struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);

#endif
