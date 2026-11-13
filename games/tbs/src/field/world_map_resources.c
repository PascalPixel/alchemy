/// @file
/// World map graphics: a palette and five tilesets for each of its two looks.

#include "field/field.h"
#include "resource_ids.h"
#include "types.h"

/// Each set is a palette and five tilesets: WorldMap_LoadGraphics decodes the
/// palette into BG palettes 0 to 13, the next four into the two halves of char
/// blocks 2 and 3, and the last into gMapGridAnimTiles, where the tileset
/// animations copy their pictures from.
u32 gWorldMap_GraphicsResources[2][6] = {
    {
        (u32)RESOURCE_WORLD_MAP_PALETTE_A,
        (u32)RESOURCE_WORLD_MAP_TILES_A,
        (u32)RESOURCE_WORLD_MAP_TILES_B,
        (u32)RESOURCE_WORLD_MAP_TILES_C,
        (u32)RESOURCE_WORLD_MAP_TILES_D,
        (u32)RESOURCE_WORLD_MAP_TILES_E,
    },
    {
        (u32)RESOURCE_WORLD_MAP_PALETTE_B,
        (u32)RESOURCE_WORLD_MAP_TILES_F,
        (u32)RESOURCE_WORLD_MAP_TILES_G,
        (u32)RESOURCE_WORLD_MAP_TILES_H,
        (u32)RESOURCE_WORLD_MAP_TILES_I,
        (u32)RESOURCE_WORLD_MAP_TILES_J,
    },
};
