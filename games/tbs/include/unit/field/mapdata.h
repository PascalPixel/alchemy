#ifndef ALCHEMY_UNIT_FIELD_MAPDATA_H
#define ALCHEMY_UNIT_FIELD_MAPDATA_H

#include "types.h"

// 高さ関数はそれぞれ要る引数だけを読むので、引数の型は決めない。
typedef s32 (*TerrainHeightFn)();

extern const TerrainHeightFn gMap_TerrainHeightFunctions[16];
extern const s8 gWorldMap_DirectionOffsets[8][2];
extern const u32 gWorldMap_UnitScale[2];

#endif
