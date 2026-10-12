#ifndef ALCHEMY_UNIT_FIELD_MAPCELL_H
#define ALCHEMY_UNIT_FIELD_MAPCELL_H

#include "types.h"

struct FieldPosition;
struct MapScanline;
struct ObjectRuntime;
struct WorldPosition;

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y);
s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y);
s32 GetMapCellCollision(s32 layer, s32 x, s32 y);
void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code);
s32 Map_GetCellHighFlags(s32 x, s32 y);
s32 Map_CheckObjectStep(struct ObjectRuntime *object, struct FieldPosition *position);
s32 CheckMapPositionCellOccupied(struct WorldPosition *position);
s32 GetWorldMapCollision(struct WorldPosition *position);
s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position);
s32 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind);
void Runtime_SetWorkTripleIfNonNegative(s32 value0, s32 value1, s32 value2);
void Map_WaitWorkValuesBelow256(void);
void Resource_RunCopiedDecoder(s32 a, s32 b);
void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct MapScanline *row);

#endif
