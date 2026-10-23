/// @file
/// World map graphics: a palette and five tile sets for each of its two looks.

#include "field.h"
#include "resource_ids.h"
#include "types.h"

/// Each set is a palette and five tile sets: WorldMap_LoadGraphics decodes
/// the palette into BG palettes 0 to 13, the next four into the two halves of
/// char blocks 2 and 3, and the last into gMapGridLayerData, where the tile
/// animation copies its frames from. It takes the second set when the cell it
/// loads at is WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD, and keeps the set's row in
/// the perspective work's resources, from which WorldMap_LoadAreaGraphics
/// reloads all but the last. Not const: the loader reads it through a u32 *.
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
