#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "types.h"

struct MapCell {
    u16 metatile_and_flags;
    u8 collision_code;
    u8 attribute_b;
};

struct MapLayer {
    struct MapCell *cells;
    u8 unknown04[0x2c];
};

struct MapState {
    u8 unknown000[0x130];
    struct MapLayer layers[4];
};

struct WorldPosition {
    s32 x;
    u8 unknown_04[4];
    s32 y;
};

#define GetFocusedObjectCollision Func_0808bd24
#define UpdateMapRegionAtPosition Func_0808bde0
#define GetMapCellCollision Func_08012038
#define SetMapCellCollision Func_08012078
#define GetWorldMapCollision Func_08012204
#define CheckMapPositionCellOccupied Func_0801219c
#define CheckWorldMapCollisionRange Func_080122ac
#define GetWorldMapTerrainBehavior Func_080122c8

u8 GetFocusedObjectCollision(void);
void UpdateMapRegionAtPosition(s32 x, s32 y, s32 z);
u8 GetMapCellCollision(s32 layer, s32 x, s32 y);
void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code);
s32 GetWorldMapCollision(struct WorldPosition *position);
s32 CheckMapPositionCellOccupied(struct WorldPosition *position);
s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position);
u8 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind);

#endif
