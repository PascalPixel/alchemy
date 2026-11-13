#ifndef ALCHEMY_MODULE_SCENE_CAVERNS_H
#define ALCHEMY_MODULE_SCENE_CAVERNS_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

extern u32 gData_0200116c[];

extern u32 gData_020011a8[];
extern u32 gData_020011e4[];

#endif
