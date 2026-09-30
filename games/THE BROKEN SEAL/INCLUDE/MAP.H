#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "TYPES.H"

struct MapCell {
    u16 metatile_and_flags;
    u8 collision_code;
    u8 attribute_b;
};

struct MapLayer {
    struct MapCell *cells;
    u8 unknown04[0x2c];
};

/* A blend script's playback state. */
struct BlendScriptState {
    u16 *script;
    u16 *cursor;
    u16 delay;
    u16 paused;
};

struct MapState {
    u8 unknown000[0xd8];
    struct BlendScriptState blend;   /* 0xd8 */
    u8 unknown_e4[0x1f];
    u8 blend_control;                /* 0x103: last BLDCNT written */
    u8 unknown_104[0x2c];
    struct MapLayer layers[4];       /* 0x130 */
};

struct WorldPosition {
    s32 x;
    u8 unknown_04[4];
    s32 y;
};

u8 GetFocusedObjectCollision(void);
void UpdateMapRegionAtPosition(s32 x, s32 y, s32 z);
u8 GetMapCellCollision(s32 layer, s32 x, s32 y);
void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code);
s32 GetWorldMapCollision(struct WorldPosition *position);
s32 CheckMapPositionCellOccupied(struct WorldPosition *position);
s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position);
u8 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind);

/* The map's block table: a field map's metatiles, two words of tile entries
   each, or the world map's 32 by 32 grid of blocks. */
extern u32 gMapBlocks[];

/* Map data the world map and layered scenes decode beside their blocks. */
extern u8 gMapLayerData[];

/* The map's collision tables: a field map's cell attributes, or the world
   map's 4bpp collision tiles, back layer first and front layer at +0x800. */
extern u8 gMapCollision[];

/* The area's four tile banks, staged here and copied to BG character
   blocks 2 and 3. */
extern u8 gBgTileBuffer[];

/* The area's animation pages, which MapAnimation_Start decodes a frame
   from. */
extern u8 *gMapAnimationPages;

#endif
