/// @file
/// World map graphics: a palette and five tilesets for each of its two looks.

#include "field.h"
#include "resource_ids.h"
#include "types.h"

/// Each set is a palette and five tilesets: WorldMap_LoadGraphics decodes the
/// palette into BG palettes 0 to 13, the next four into the two halves of char
/// blocks 2 and 3, and the last into gMapGridAnimTiles, where the tileset
/// animations copy their pictures from.
u32 gWorldMap_GraphicsResources[2][6] = {
    {
        (u32)&ResourceId_WorldMapPaletteA,
        (u32)&ResourceId_WorldMapTilesA,
        (u32)&ResourceId_WorldMapTilesB,
        (u32)&ResourceId_WorldMapTilesC,
        (u32)&ResourceId_WorldMapTilesD,
        (u32)&ResourceId_WorldMapTilesE,
    },
    {
        (u32)&ResourceId_WorldMapPaletteB,
        (u32)&ResourceId_WorldMapTilesF,
        (u32)&ResourceId_WorldMapTilesG,
        (u32)&ResourceId_WorldMapTilesH,
        (u32)&ResourceId_WorldMapTilesI,
        (u32)&ResourceId_WorldMapTilesJ,
    },
};
